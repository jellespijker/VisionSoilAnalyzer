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

	void Sample::Analyse()
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

		// Segment the particles and create the particle list
		SegmentParticles();

		//if (!AnalysePopVect(Population, Results))
		//{
		//	throw Exception::AnalysisException("Analysis Failed!", 8);
		//}
	}

	void Sample::Analyse(const Mat& src)
	{
		OriginalImage = src;
		Analyse();
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

		switch (segType)
		{
		case Vision::Segment::Normal:
			// Optimize the intensity image
			Enhancer.AdaptiveContrastStretch(5, 0.5);
			Enhancer.Blur(9, true);

			// Segment the enhance image and remove borderblobs get the edges
			Segmenter = Vision::Segment(Enhancer.ProcessedImg);
			Segmenter.ConvertToBW(Vision::Segment::Dark);
			Segmenter.RemoveBorderBlobs(true);
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

		Segmenter.GetEdgesEroding(true);
		Edge = Segmenter.ProcessedImg.clone();

		// Get the Particlelist
		Segmenter.GetBlobList(true);
		for (uint32_t i = 0; i < Segmenter.BlobList.size(); i++)
		{
			if (Segmenter.BlobList[i].cvROI.height > 9 && Segmenter.BlobList[i].cvROI.width > 9)
			{
				Particle newPart;
				newPart.ID = Segmenter.BlobList[i].Label;
				newPart.Analyzed = false;
				newPart.BW = Segmenter.BlobList[i].Img.clone();
				cv::Rect ROI = Segmenter.BlobList[i].cvROI;
				newPart.Edge = Vision::Segment::CopyMat<uchar>(Edge(ROI).clone(), newPart.BW, CV_8UC1);
				newPart.Intensity = Vision::Segment::CopyMat<uchar>(Intensity(ROI).clone(), newPart.BW, CV_8UC1);
				newPart.LAB = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), newPart.BW, CV_32FC3);
				newPart.RGB = Vision::Segment::CopyMat<uchar>(LAB(ROI).clone(), newPart.BW, CV_8UC3);
				newPart.RI = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), newPart.BW, CV_32FC1);

				Population.push_back(newPart);
			}
		}
	}
}