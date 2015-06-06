#pragma once
#define MIN_EDGE_PIXELS 128
#define FFT_DESCRIPTORS 12

#include "AnalysisResults.h"
#include "ShapeClassification.h"
#include "../SoilVision/Vision.h"

#include <boost/serialization/base_object.hpp>

namespace SoilAnalyzer
{
	class ParticleAnalysisResults :
		public AnalysisResults
	{
	public:

		bool Analyzed = false;
		bool SmallParticle = false;
		uint32_t Area;
		ShapeClassification Shape;

		ParticleAnalysisResults();
		~ParticleAnalysisResults();

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & Analyzed;
			ar & SmallParticle;
			ar & Area;
			ar & Shape;
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnalysisResults);
		}
	};
}
