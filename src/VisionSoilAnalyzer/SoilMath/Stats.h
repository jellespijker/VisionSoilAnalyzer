#pragma once
#define MAX_UINT8_VALUE 256
#define VECTOR_CALC 1
#define STATS_VERSION 1

#include <stdint.h>
#include <utility>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <typeinfo>

#include "MathException.h"

using namespace std;

namespace SoilMath
{
	template <typename T1, typename T2, typename T3>
	class Stats
	{
	private:
		bool isDiscrete = true;

	public:

		T1 *Data;
		uint32_t *bins;
		bool Calculated = false;
		float Mean = 0.0;
		uint32_t n = 0;
		uint32_t noBins = 0;
		T1 Range;
		T1 min;
		T1 max;
		T1 Startbin;
		T1 EndBin;
		T1 binRange;
		float Std = 0.0;
		T3 Sum = 0;
		uint16_t Rows = 0;
		uint16_t Cols = 0;

		Stats(int noBins = 256, T1 startBin = 0, T1 endBin = 255)
		{
			min = numeric_limits<T1>::max();
			max = numeric_limits<T1>::min();
			Range = numeric_limits<T1>::max();
			Startbin = startBin;
			EndBin = endBin;
			this->noBins = noBins;
			bins = new uint32_t[noBins] { };

			if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) || typeid(T1) == typeid(long double))
			{
				isDiscrete = false;
				binRange = static_cast<T1>((EndBin - Startbin) / noBins);
			}
			else
			{
				isDiscrete = true;
				binRange = static_cast<T1>(round((EndBin - Startbin) / noBins));
			}
		}

		Stats(T1 *data, uint16_t rows, uint16_t cols, int noBins = 256, T1 startBin = 0, T1 endBin = 255)
		{
			min = numeric_limits<T1>::max();
			max = numeric_limits<T1>::min();
			Range = max - min;

			Startbin = startBin;
			EndBin = endBin;

			if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) || typeid(T1) == typeid(long double))
			{
				isDiscrete = false;
				binRange = static_cast<T1>((EndBin - Startbin) / noBins);
			}
			else
			{
				isDiscrete = true;
				binRange = static_cast<T1>(round((EndBin - Startbin) / noBins));
			}

			Data = data;
			Rows = rows;
			Cols = cols;
			bins = new uint32_t[noBins] { };
			this->noBins = noBins;
			if (isDiscrete) { BasicCalculate(); }
			else { BasicCalculateFloat(); }
		}

		/// <summary>
		/// Constructor when the data is given as an histogram
		/// </summary>
		/// <param name="binData">A histogram of [256] bins</param>
		/// <param name="offset">offset when the data starts</param>
		Stats(T2 *binData, uint16_t startC, uint16_t endC)
		{
			noBins = endC - startC;
			Startbin = startC;
			EndBin = endC;
			uint32_t i = noBins;

			if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) || typeid(T1) == typeid(long double))
			{
				isDiscrete = false;
				throw Exception::MathException("Calculations using histogram not supported with floating-type!");
			}
			else
			{
				isDiscrete = true;
				binRange = static_cast<T1>(round((EndBin - Startbin) / noBins));
			}

			bins = new uint32_t[noBins] { };
			while (i-- > 0)
			{
				bins[i] = binData[i];
				n += binData[i];
			}
			BinCalculations(startC, endC);
		}

		~Stats() {};

		void BasicCalculateFloat()
		{
			float sum_dev = 0.0;

			// Make copy of the starting pointer
			T1 *StartDataPointer = Data;

			// Get number of samples
			n = Rows * Cols;
			uint32_t i = n;

			// Get sum , min, max, fill histogram
			while (i-- > 0)
			{
				if (*Data > max) { max = *Data; }
				else if (*Data < min) { min = *Data; }
				Sum += *Data++;
			}

			binRange = (max - min) / noBins;
			uint16_t index = 0;
			T1 shift = -min;

			i = n - 1 ;
			Data = StartDataPointer;
			while (i > 0)
			{
				index = (shift + Data[i]) / binRange;
				bins[index]++;
				i--;
			}

			// Get Mean
			Mean = Sum / (float)n;

			// Get Max;
			Range = max - min;

			// Calculate Standard Deviation
			Data = StartDataPointer;
			i = n;
			while (i-- > 0)	{ sum_dev += pow((*Data++ - Mean), 2); }
			Std = sqrt((float)(sum_dev / n));
			Calculated = true;

			// Reset the pointer
			Data = StartDataPointer;
		}

		void BasicCalculate()
		{
			float sum_dev = 0.0;

			// Make copy of the starting pointer
			T1 *StartDataPointer = Data;

			// Get number of samples
			n = Rows * Cols;

			// fills the histogram
			uint32_t i = n;

			while (i-- > 0) { bins[(uint32_t)*Data++]++; }

			// Depending on the data size choose between using the histogram or
			// actual data for efficient calculations
			if (n > MAX_UINT8_VALUE) { BinCalculations(0, 256); }
			else
			{
				Data = StartDataPointer;

				// Get sum , min, max
				i = n;
				while (i-- > 0)
				{
					if (*Data > max) { max = *Data; }
					else if (*Data < min) { min = *Data; }
					Sum += *Data++;
				}

				// Get Mean
				Mean = Sum / (float)n;

				// Get Max;
				Range = max - min;

				// Calculate Standard Deviation
				Data = StartDataPointer;
				i = n;
				while (i-- > 0)	{ sum_dev += pow((*Data++ - Mean), 2); }
				Std = sqrt((float)(sum_dev / n));
				Calculated = true;
			}

			// Reset the pointer
			Data = StartDataPointer;
		}

		/// <summary>
		/// Make the calculations using the histogram
		/// </summary>
		void BinCalculations(uint16_t startC, uint16_t endC)
		{
			float sum_dev = 0.0;
			uint32_t lastC = endC - startC;
			int32_t i = lastC;
			// Get sum
			while (i-- > 0) { Sum += bins[i] * (startC + i); }

			// Get Mean
			Mean = Sum / (float)n;

			// Get max
			i = lastC;
			while (i-- > 0)
			{
				if (bins[i] != 0)
				{
					max = i;
					break;
				}
			}
			max += startC;

			// Get min
			i = 0;
			while (i < lastC)
			{
				if (bins[i] != 0)
				{
					min = i;
					break;
				}
				i++;
			}
			min += startC;

			// Get Max;
			Range = max - min;

			// Calculate Standard Deviation
			i = lastC;
			while (i-- > 0)	{ sum_dev += bins[i] * pow(((i + startC) - Mean), 2); }
			Std = sqrt((float)(sum_dev / n));
			Calculated = true;
		}
	};
}
