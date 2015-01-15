#pragma once
#define COMMONOPERATIONS_VERSION 1

#include <algorithm> 
#include <stdint.h>

namespace SoilMath
{
	inline uint16_t MinNotZero(uint16_t a, uint16_t b)
	{
		if (a != 0 && b != 0) { return (a < b) ? a : b; }
		else { return (a > b) ? a : b; }
	}

	inline uint16_t Max(uint16_t a, uint16_t b)
	{
		return (a > b) ? a : b;
	}

	inline uint16_t Max(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
	{
		return (Max(a, b) > Max(c, d)) ? Max(a, b) : Max(c, d);
	}

	inline uint16_t Min(uint16_t a, uint16_t b)
	{
		return (a < b) ? a : b;
	}

	inline uint16_t Min(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
	{
		return (Min(a, b) > Min(c, d)) ? Min(a, b) : Min(c, d);
	}
}
