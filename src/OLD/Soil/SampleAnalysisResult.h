#pragma once
#include "AnalysisResults.h"
#include "ParticleAnalysisResults.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

namespace SoilAnalyzer
{
	class SampleAnalysisResult :
		public AnalysisResults
	{
	public:

		SampleAnalysisResult();
		~SampleAnalysisResult();
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnalysisResults);
		}

	};

}