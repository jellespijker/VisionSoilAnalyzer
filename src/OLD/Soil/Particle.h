#pragma once

#include <boost/serialization/base_object.hpp>

#include "Soil.h"
#include "../SoilMath/SoilMath.h"
#include "ParticleAnalysisResults.h"

namespace SoilAnalyzer
{
	class Particle :
		public Soil
	{
	public:
		Particle();
		~Particle();

		void Save(std::string &filename);
		void Load(std::string &filename);
		
		ParticleAnalysisResults Analysis;
		SoilAnalyzer::AnalysisResults Analyze(SoilMath::NN &nn);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Soil);
			ar & Analysis;
		}
	};
}
