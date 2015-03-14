#pragma once
#include <stdint.h>
#include <utility>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

#include "AnalyseType.h"
#include "../SoilMath/SoilMath.h"

namespace SoilAnalyzer
{
	class AnalysisResults
	{
	public:
		AnalysisResults();
		~AnalysisResults();
		void Analyse(AnalyseType analyseType);
		uint8_t version;
		ucharStat_t ClassRoundness;
		std::pair<float, float> LAB;
		ucharStat_t PSD;
		ucharStat_t RI;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(ClassRoundness);
			ar & BOOST_SERIALIZATION_NVP(LAB);
			ar & BOOST_SERIALIZATION_NVP(PSD);
			ar & BOOST_SERIALIZATION_NVP(RI);
		}
	};
}
