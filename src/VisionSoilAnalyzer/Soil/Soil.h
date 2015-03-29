#pragma once

// Needed for serialization
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/string.hpp>
#include "Mat_archive.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
			ar & Analyzed;
			ar & BW;
			ar & Intensity;
			ar & LAB;
			ar & RI;
			ar & RGB;
			ar & OptimizedInt;
		}

	protected:
		cv::Mat OptimizedInt;

	public:
		Soil();
		~Soil();
		bool Analyzed = false;
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
