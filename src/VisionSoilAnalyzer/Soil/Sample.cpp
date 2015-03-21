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
		if (OriginalImage.empty())
		{
			throw Exception::AnalysisException("No Image found to analyze!", 1);
		}

		// Convert the image to an intensity image CIE La*b* and Redness Index
		Vision::Conversion Convertor;
		Convertor.Convert(OriginalImage, Intensity, Vision::Conversion::RGB, Vision::Conversion::Intensity);
		Convertor.Convert(OriginalImage, LAB, Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
		Convertor.Convert(LAB, RI, Vision::Conversion::CIE_lab, Vision::Conversion::RI);

		// Segment the particles and create the particle list
		std::vector<Particle> bloblist = SegmentParticles();
		


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

	bool Sample::AnalysePopVect(	const vector<Particle>& population,
									AnalysisResults& results)
	{

		return true;
	}

	std::vector<Particle> Sample::SegmentParticles(Vision::Segment::SegmentationType segType)
	{
		Vision::Enhance Enhancer(Intensity);
		Vision::Segment Segmenter;

		switch (segType)
		{
		case Vision::Segment::Normal:
			// Optimize the intensity image
			Enhancer.Blur(9);
			Enhancer.AdaptiveContrastStretch(9, 1, true);
			
			// Segment the enhance image and remove borderblobs get the edges
			Segmenter.OriginalImg = Enhancer.ProcessedImg;
			Segmenter.ConvertToBW(Vision::Segment::Dark);
			Segmenter.RemoveBorderBlobs(true);
			BW = Segmenter.ProcessedImg;
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
		Edge = Segmenter.ProcessedImg;

		// Get the Particlelist
		Segmenter.GetBlobList(true);
		for (uint32_t i = 0; i < Segmenter.BlobList.size(); i++)
		{
			Particle newPart;
			newPart.ID = i;
			newPart.Analyzed = false;
			newPart.BW = Segmenter.BlobList[i].Img;
			newPart.Edge = Segmenter.CopyMat<uchar>(Edge(Segmenter.BlobList[i].cvROI).clone(), newPart.BW, CV_8UC1);
			newPart.Intensity = Segmenter.CopyMat<uchar>(Intensity(Segmenter.BlobList[i].cvROI).clone(), newPart.BW, CV_8UC1);
			newPart.LAB = Segmenter.CopyMat<float>(LAB(Segmenter.BlobList[i].cvROI).clone(), newPart.BW, CV_8UC1);
			newPart.RGB = Segmenter.CopyMat<float>(RGB(Segmenter.BlobList[i].cvROI).clone(), newPart.BW, CV_8UC1);
			newPart.RI = Segmenter.CopyMat<float>(RI(Segmenter.BlobList[i].cvROI).clone(), newPart.BW, CV_8UC1);

			Population.push_back(newPart);
		}
		return Population;
	}

}