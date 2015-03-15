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

		// Convert the image to an intensity image
		Vision::Conversion ConvFromRGB(OriginalImage);
		ConvFromRGB.Convert(Vision::Conversion::RGB, Vision::Conversion::Intensity);
		Intensity = ConvFromRGB.ProcessedImg;

		// Convert the image to an CIE La*b* colorspace
		ConvFromRGB.Convert(Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
		LAB = ConvFromRGB.ProcessedImg;

		// Convert the CIE La*b* colorspace image to a Redness Index image
		Vision::Conversion ConvFromLAB(LAB);
		ConvFromLAB.Convert(Vision::Conversion::CIE_lab, Vision::Conversion::RI);
		
		// 



		//ConvertToInt(OriginalImage, Intensity);


		//if (!EnhanceImg(Intensity, Intensity)) 
		//{ 
		//	throw Exception::AnalysisException("Enhance Image failed!", 3); 
		//}

		//if (!ConvertToBW(Intensity, BW))
		//{
		//	throw Exception::AnalysisException("Conversion to BW failed!", 4);
		//}

		//if (!ConvertToLAB(OriginalImage, LAB))
		//{
		//	throw Exception::AnalysisException("Conversion to CIE La*b* failed!", 5);
		//}

		//if (!ConvertToRI(LAB, RI))
		//{
		//	throw Exception::AnalysisException("Conversion to RI failed!", 6);
		//}

		//if (!SegmentParticles(OriginalImage, Intensity, BW, LAB, RI, Population))
		//{
		//	throw Exception::AnalysisException("Segmentation of Particles failed!", 7);
		//}

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

	bool Sample::ConvertToBW(	const Mat& src,
								Mat& dst)
	{

		return true;
	}

	void Sample::ConvertToInt(	const Mat& src,
								Mat& dst)
	{
	}

	bool Sample::ConvertToLAB(	const Mat& src,
								Mat& dst)
	{

		return true;
	}

	bool Sample::ConvertToRI(	const Mat& src,
								Mat& dst)
	{

		return true;
	}

	bool Sample::EnhanceImg(	const Mat& src,
								Mat& dst)
	{

		return true;
	}

	bool Sample::SegmentParticles(	const Mat& rgb,
									const Mat& intensity,
									const Mat& bw,
									const Mat& lab,
									const Mat& ri,
									vector<Particle>& population)
	{

		return true;
	}

}