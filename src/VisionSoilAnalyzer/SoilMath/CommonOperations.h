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

	static double quick_pow10(int n)
	{
		static double pow10[19] = {
			1, 10, 100, 1000, 10000,
			100000, 1000000, 10000000, 100000000, 1000000000,
			10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000,
			1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000
		};
		return pow10[n];
	}

}
