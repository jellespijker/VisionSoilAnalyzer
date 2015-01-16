#pragma once
#include <opencv/cv.h>
#include <opencv2/core.hpp>
#include <stdint.h>

namespace SoilAnalyzer
{
	class Soil
	{
	public:
		Soil();
		~Soil();
		bool Analyzed;
		cv::Mat BW;
		cv::Mat Intensity;
		cv::Mat LAB;
		cv::Mat RGB;
		cv::Mat RI;
		uint8_t version;
	};
}
