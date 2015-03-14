#pragma once

// Needed for serialization
#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
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
			ar & BOOST_SERIALIZATION_NVP(ID);
			ar & BOOST_SERIALIZATION_NVP(Location);
			ar & BOOST_SERIALIZATION_NVP(TimeTaken);
			ar & BOOST_SERIALIZATION_NVP(TimeAnalyzed);
			ar & BOOST_SERIALIZATION_NVP(Analyzed);
			ar & BOOST_SERIALIZATION_NVP(BW);
			ar & BOOST_SERIALIZATION_NVP(Intensity);
			ar & BOOST_SERIALIZATION_NVP(LAB);
			ar & BOOST_SERIALIZATION_NVP(RI);
			ar & BOOST_SERIALIZATION_NVP(RGB);
		}

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
		std::string TimeTaken;
		std::string TimeAnalyzed;
		std::string Location;
		uint32_t ID;

	};
}
