#pragma once
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/serialization/vector.hpp>

#include "../SoilMath/SoilMath.h"

class ShapeClassification
{
public:
	unsigned char Category;
	ComplexVect_t FFT_descriptors;

	ShapeClassification();
	ShapeClassification(ComplexVect_t fft_descriptors) : FFT_descriptors(fft_descriptors) {};

	~ShapeClassification();
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & Category;
		ar & FFT_descriptors;
	}

};

