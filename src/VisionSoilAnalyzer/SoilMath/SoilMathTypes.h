#pragma once

#include <vector>
#include <complex>

#include "Stats.h"

namespace SoilMath
{
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef SoilMath::Stats<float, double, long double> floatStat_t;
	typedef SoilMath::Stats<uchar, uint32_t, uint64_t> ucharStat_t;
	typedef std::complex<float> comp_t;
	typedef std::vector<comp_t> complexVect_t;
}