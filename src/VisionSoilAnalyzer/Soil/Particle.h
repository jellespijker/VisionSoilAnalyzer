#pragma once

#include <boost/serialization/base_object.hpp>

#include "Soil.h"
#include "ShapeClassification.h"

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

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Soil);
		}
	};
}
