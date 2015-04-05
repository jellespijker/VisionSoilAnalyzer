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

		// Convert the image to an intensity image and an enhanced Intinsity for better segmentation
		Vision::Conversion RGBConvertor(OriginalImage);
		RGBConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::Intensity);
		Intensity = RGBConvertor.ProcessedImg.clone();
		SHOW_DEBUG_IMG(Intensity, uchar, 255, "Intensity");

		// Enhance the image with an Adaptive contrast stretch followed by a blur
		Vision::Enhance IntEnchance(Intensity);
		IntEnchance.AdaptiveContrastStretch(7, 10);
		IntEnchance.Blur(7);
		OptimizedInt = IntEnchance.ProcessedImg(cv::Rect(10, 10, OriginalImage.cols - 20, OriginalImage.rows - 20)).clone();
		SHOW_DEBUG_IMG(OptimizedInt, uchar, 255, "IntEnchance");

		// Segment the Dark Objects en fill the holes
		Vision::Segment Segmenter(OptimizedInt);
		Segmenter.ConvertToBW(Vision::Segment::Dark);
		Segmenter.FillHoles(true);
		Segmenter.RemoveBorderBlobs(1, true);
		//Segmenter.GetEdgesEroding(true);
		SHOW_DEBUG_IMG(Segmenter.ProcessedImg, uchar, 255, "Segmenter");

		// Erode the segmented image and sets the BW image
		Vision::MorphologicalFilter Filter(Segmenter.ProcessedImg);
		Mat mask = cv::Mat::zeros(5, 5, CV_8UC1); 
		circle(mask, Point(3, 3), 3, 1, -1);
		Filter.Erosion(mask);
		BW = Filter.ProcessedImg.clone();
		SHOW_DEBUG_IMG(BW, uchar, 255, "BW after segmentation, fill holes and erosion");
		
		//// Make the CIE La*b* conversion
		//cv::Mat RGBwork = cv::Mat(RGB(cv::Rect(10, 10, OriginalImage.cols - 20, OriginalImage.rows - 20))).clone();
		//Vision::Conversion RGBnewConvertor(RGBwork);
		//RGBnewConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
		//LAB = RGBnewConvertor.CopyMat<float>(RGB, BW, CV_32F);
		//SHOW_DEBUG_IMG(LAB, float, 1.0, "LAB");

		//// Create the Redness Index
		//Vision::Conversion LABConvertor(LAB);
		//LABConvertor.Convert(Vision::Conversion::CIE_lab, Vision::Conversion::RI);
		//RI = LABConvertor.ProcessedImg;
		//SHOW_DEBUG_IMG(RI, float, 1.0, "RI");

		// Create the Edge image
		//Vision::Segment Edger(BW);
		//Edger.GetEdgesEroding(false);
		//Edge = Edger.ProcessedImg;



		// Segment and analyze the particles
		//SegmentParticles();
		//for_each(Population.begin(), Population.end(), [&](Particle &P) 
		//{
		//	P.Analyze(nn); 
		//});

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
			Segmenter.LoadOriginalImg(Enhancer.ProcessedImg);
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