#pragma once

#include "Stats.h"
#include "Sort.h"
#include "CommonOperations.h"
#include "FFT.h"

namespace SoilMath
{
	typedef unsigned char uchar;
	typedef Stats<float, double, long double> floatStat_t;
	typedef Stats<uchar, uint32_t, uint64_t> ucharStat_t;
}