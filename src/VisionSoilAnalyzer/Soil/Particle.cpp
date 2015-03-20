#include "Particle.h"

namespace SoilAnalyzer
{
	Particle::Particle()
	{
	}


	Particle::~Particle()
	{
	}

	void Particle::Save(std::string &filename)
	{
		std::ofstream ofs(filename.c_str());
		boost::archive::binary_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(*this);
	}

	void Particle::Load(std::string &filename)
	{
		std::ifstream ifs(filename.c_str());
		boost::archive::binary_iarchive ia(ifs);
		ia >> BOOST_SERIALIZATION_NVP(*this);
	}
}