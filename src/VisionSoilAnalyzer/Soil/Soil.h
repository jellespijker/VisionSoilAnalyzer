#pragma once

// Debuging helper macros
#ifdef DEBUG
	#include <limits>
	#include <opencv2/highgui/highgui.hpp>
	#include <vector>
	#include "../Vision/Vision.h"
	#define SHOW_DEBUG_IMG(img, T1, maxVal, windowName) ShowDebugImg<T1>(img, maxVal, windowName)
#else 
	#define SHOW_DEBUG_IMG(img, T1, maxVal, windowName)
#endif

#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include "Mat_archive.h"
#include <opencv2/core/core.hpp>
#include <stdint.h>
#include <string>

namespace SoilAnalyzer
{
	class Soil
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & ID;
			ar & Location;
			ar & TimeTaken;
			ar & TimeAnalyzed;
			ar & BW;
			ar & Intensity;
			ar & LAB;
			ar & RI;
			ar & RGB;
			ar & OptimizedInt;
		}

	protected:
		cv::Mat OptimizedInt;

		// Only compile ShowDebugImg when DEBUG preprocessor is defined
#ifdef DEBUG
		template<typename T1>
		void ShowDebugImg(cv::Mat img, T1 maxVal, std::string windowName)
		{
			if (img.empty())
			{
				std::vector<cv::Mat> exSrc = Vision::ImageProcessing::extractChannel(img);
				std::vector<cv::Mat> exDst;
				int cvBaseType = img.type() % 8;
				T1 MatMin = std::numeric_limits<T1>::max();
				T1 MatMax = std::numeric_limits<T1>::min();

				// Find the global max and min
				for_each(exSrc.begin(), exSrc.end(), [&](const Mat & sItem)
				{
					std::for_each(sItem.begin<T1>(), sItem.end<T1>(), [&](const T1 &s) { if (s > MatMax) { MatMax = s; } else if (s < MatMin) { MatMin = s; } });
				});

				// Convert the values
				for_each(exSrc.begin(), exSrc.end(), [&](const Mat & sItem)
				{
					Mat dItem(img.size(), cvBaseType);
					std::transform(sItem.begin<T1>(), sItem.end<T1>(), dItem.begin<T1>(), [&](const T1 &s) -> T1 { return (T1)round(((s - MatMin) * maxVal) / (MatMax - MatMin)); });
					exDst.push_back(dItem);
				});

				cv::Mat tempImg(img.size(), img.type());
				merge(exDst, tempImg);
				cv::namedWindow(windowName, cv::WINDOW_NORMAL);
				cv::imshow(windowName, tempImg);
				cv::waitKey(0);
			}
		};
#endif

	public:
		Soil();
		~Soil();
		cv::Mat BW;
		cv::Mat Intensity;
		cv::Mat LAB;
		cv::Mat RGB;
		cv::Mat RI;
		cv::Mat Edge;
		uint8_t version;
		std::string TimeTaken;
		std::string TimeAnalyzed;
		std::string Location;
		uint32_t ID;
	};
}
