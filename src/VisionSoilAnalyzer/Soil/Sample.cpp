#include "Sample.h"

namespace SoilAnalyzer
{
	Sample::Sample()
	{
	}

	Sample::Sample(const Mat& src)
	{
		OriginalImage = src;
	}

	Sample::~Sample()
	{
	}

	void Sample::Save(string &filename)
	{
		std::ofstream ofs(filename.c_str());
		boost::archive::binary_oarchive oa(ofs);
		oa << boost::serialization::make_nvp("SoilSample", *this);
	}

	void Sample::Load(string &filename)
	{
		std::ifstream ifs(filename.c_str());
		boost::archive::binary_iarchive ia(ifs);
		ia >> boost::serialization::make_nvp("SoilSample", *this);
	}

	void Sample::Analyse(SoilMath::NN &nn)
	{
		if (OriginalImage.empty()) { throw Exception::AnalysisException("No Image found to analyze!", 1); }

		// Convert the image to an intensity image CIE La*b* and Redness Index
		Vision::Conversion RGBConvertor(OriginalImage);
		RGBConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::Intensity);
		Intensity = RGBConvertor.ProcessedImg.clone();
		RGBConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
		LAB = RGBConvertor.ProcessedImg.clone();
		Vision::Conversion LABConvertor(LAB);
		LABConvertor.Convert(Vision::Conversion::CIE_lab, Vision::Conversion::RI);
		RI = LABConvertor.ProcessedImg.clone();

		// Segment and analyze the particles
		SegmentParticles();
		for_each(Population.begin(), Population.end(), [&](Particle &P) { P.Analyze(nn); });

		// Analyze the image
	}

	void Sample::Analyse(const Mat& src, SoilMath::NN &nn)
	{
		OriginalImage = src;
		Analyse(nn);
	}

	bool Sample::AnalysePopVect(const vector<Particle>& population,
		AnalysisResults& results)
	{
		return true;
	}

	void Sample::SegmentParticles(Vision::Segment::SegmentationType segType)
	{
		Vision::Enhance Enhancer(Intensity);
		Vision::Segment Segmenter;
		Vision::MorphologicalFilter Eroder;

		switch (segType)
		{
		case Vision::Segment::Normal:
			// Optimize the intensity image
			Enhancer.Blur(15);
			Enhancer.AdaptiveContrastStretch(5, 0.125, true);

			// Segment the enhance image and remove borderblobs get the edges
			Segmenter = Vision::Segment(Enhancer.ProcessedImg);
			Segmenter.ConvertToBW(Vision::Segment::Dark);
			Segmenter.RemoveBorderBlobs(1, true);
			BW = Segmenter.ProcessedImg.clone();
			break;
		case Vision::Segment::LabNeuralNet:
			throw Exception::AnalysisException("Not yet implemented!", 1);
			break;
		case Vision::Segment::GraphMinCut:
			throw Exception::AnalysisException("Not yet implemented!", 1);
			break;
		default:
			break;
		}

		Vision::Segment getEdges(BW);
		getEdges.GetEdgesEroding();
		Edge = getEdges.ProcessedImg;

		// Get the Particlelist
		Segmenter.GetBlobList(true);
		Population.resize(Segmenter.BlobList.size());
		uint32_t i = 0;
		for_each(Population.begin(), Population.end(), [&](Particle &P) 
		{
			P.ID = Segmenter.BlobList[i].Label;
			P.Analysis.Analyzed = false;
			P.BW = Segmenter.BlobList[i].Img.clone();
			cv::Rect ROI = Segmenter.BlobList[i].ROI;
			P.Intensity = Vision::Segment::CopyMat<uchar>(Intensity(ROI).clone(), P.BW, CV_8UC1);
			P.LAB = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), P.BW, CV_32FC3);
			P.RGB = Vision::Segment::CopyMat<uchar>(LAB(ROI).clone(), P.BW, CV_8UC3);
			P.RI = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), P.BW, CV_32FC1);
			P.Edge = Vision::Segment::CopyMat<uchar>(Edge(ROI).clone(), P.BW, CV_8UC1);
			i++;
		});
	}
}