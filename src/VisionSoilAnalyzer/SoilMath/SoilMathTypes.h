#pragma once

#include <vector>
#include <complex>

#include "Stats.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef SoilMath::Stats<float, double, long double> floatStat_t;
typedef SoilMath::Stats<uchar, uint32_t, uint64_t> ucharStat_t;
typedef std::vector<std::complex<float>> complexVect_t;
typedef std::vector<float> descriptor_t;
