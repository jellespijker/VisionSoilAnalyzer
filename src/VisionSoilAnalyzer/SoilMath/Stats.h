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
#include <string>

#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/math/distributions/students_t.hpp>

#include "MathException.h"
#include "SoilMathTypes.h"

using namespace std;

namespace SoilMath
{
	template <typename T1, typename T2, typename T3>
	class Stats
	{
	public:
		bool isDiscrete = true;

		T1 *Data;
		uint32_t *bins;
		bool Calculated = false;
		float Mean = 0.0;
		uint32_t n = 0;
		uint32_t noBins = 0;
		T1 Range = 0;
		T1 min = 0;
		T1 max = 0;
		T1 Startbin = 0;
		T1 EndBin = 0;
		T1 binRange = 0;
		float Std = 0.0;
		T3 Sum = 0;
		uint16_t Rows = 0;
		uint16_t Cols = 0;
		
		uint32_t *begin() {	return &bins[0]; }
		uint32_t *end() { return &bins[noBins]; }

		//Compare the sample using the Welch's Test (source: http://www.boost.org/doc/libs/1_57_0/libs/math/doc/html/math_toolkit/stat_tut/weg/st_eg/two_sample_students_t.html)
		bool WelchTest(SoilMath::Stats<T1, T2, T3> &statComp)
		{
			double alpha = 0.05;
			// Degrees of freedom:
			double v = statComp.Std * statComp.Std / statComp.n + this->Std * this->Std / this->n;
			v *= v;
			double t1 = statComp.Std * statComp.Std / statComp.n;
			t1 *= t1;
			t1 /= (statComp.n - 1);
			double t2 = this->Std * this->Std / this->n;
			t2 *= t2;
			t2 /= (this->n - 1);
			v /= (t1 + t2);
			// t-statistic:
			double t_stat = (statComp.Mean - this->Mean) / sqrt(statComp.Std * statComp.Std / statComp.n + this->Std * this->Std / this->n);
			//
			// Define our distribution, and get the probability:
			//
			boost::math::students_t dist(v);
			double q = cdf(complement(dist, fabs(t_stat)));

			bool rejected = false;
			// Sample 1 Mean == Sample 2 Mean test the NULL hypothesis, the two means are the same
			if (q < alpha / 2)
				rejected = false;
			else
				rejected = true;
			return rejected;
		}

		Stats(const Stats &rhs) : bins{ new uint32_t[rhs.noBins] }, Data{ new T1[rhs.n] }
		{
			this->binRange = rhs.binRange;
			this->Calculated = rhs.Calculated;
			this->Cols = rhs.Cols;
			this->EndBin = rhs.EndBin;
			this->isDiscrete = rhs.isDiscrete;
			this->max = rhs.max;
			this->Mean = rhs.Mean;
			this->min = rhs.min;
			this->n = rhs.n;
			this->noBins = rhs.noBins;
			this->n_end = rhs.n_end;
			this->Range = rhs.Range;
			this->Rows = rhs.Rows;
			this->Startbin = rhs.Startbin;
			this->Std = rhs.Std;
			this->Sum = rhs.Sum;
			std::copy(rhs.bins, rhs.bins + rhs.noBins, this->bins);
			this->Data = &rhs.Data[0];
		}

		Stats &operator=(Stats const &rhs)
		{
			if (&rhs != this) 
			{
				bins = new uint32_t[rhs.noBins];
				Data = new T1[rhs.n];
				this->binRange = rhs.binRange;
				this->Calculated = rhs.Calculated;
				this->Cols = rhs.Cols;
				this->EndBin = rhs.EndBin;
				this->isDiscrete = rhs.isDiscrete;
				this->max = rhs.max;
				this->Mean = rhs.Mean;
				this->min = rhs.min;
				this->n = rhs.n;
				this->noBins = rhs.noBins;
				this->n_end = rhs.n_end;
				this->Range = rhs.Range;
				this->Rows = rhs.Rows;
				this->Startbin = rhs.Startbin;
				this->Std = rhs.Std;
				this->Sum = rhs.Sum;
				this->Data = &rhs.Data[0];
				std::copy(rhs.bins, rhs.bins + rhs.noBins, this->bins);
			}
			return *this;
		}

		Stats(int noBins = 256, T1 startBin = 0, T1 endBin = 255)
		{
			min = numeric_limits<T1>::max();
			max = numeric_limits<T1>::min();
			Range = numeric_limits<T1>::max();
			Startbin = startBin;
			EndBin = endBin;
			this->noBins = noBins;
			bins = new uint32_t[noBins]{};

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
			EndBin = startBin + noBins;

			if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) || typeid(T1) == typeid(long double))
			{
				isDiscrete = false;
			}
			else
			{
				isDiscrete = true;
			}

			Data = data;
			Rows = rows;
			Cols = cols;
			bins = new uint32_t[noBins]{};
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
			}

			bins = new uint32_t[noBins]{};
			while (i-- > 0)
			{
				bins[i] = binData[i];
				n += binData[i];
			}
			BinCalculations(startC, endC);
		}

		~Stats()
		{
			delete[] bins;
		};

		void BasicCalculateFloat()
		{
			float sum_dev = 0.0;

			// Get number of samples
			n = Rows * Cols;

			// Get sum , min, max
			for (uint32_t i = 0; i < n; i++)
			{
				if (Data[i] > max) { max = Data[i]; }
				else if (Data[i] < min) { min = Data[i]; }
				Sum += Data[i];
			}

			binRange = (max - min) / noBins;
			uint32_t index = 0;

			// Get Mean
			Mean = Sum / (float)n;

			// Get Max;
			Range = max - min;
			bool troep = false;
			//  fill histogram
			for (uint32_t i = 0; i < n; i++)
			{
				index = static_cast<uint32_t>(floor((Data[i] - min) / binRange));
				if (index == noBins) { index -= 1; }
				bins[index]++;
				sum_dev += pow((Data[i] - Mean), 2);
			}

			Std = sqrt((float)(sum_dev / n));
			Calculated = true;
		}

		void BasicCalculate()
		{
			float sum_dev = 0.0;

			// Get number of samples
			n = Rows * Cols;

			// Get sum , min, max
			for (uint32_t i = 0; i < n; i++)
			{
				if (Data[i] > max) { max = Data[i]; }
				else if (Data[i] < min) { min = Data[i]; }
				Sum += Data[i];
			}

			binRange = static_cast<T1>(ceil((max - min) / static_cast<float>(noBins)));

			// Get Mean
			Mean = Sum / (float)n;

			// Get Max;
			Range = max - min;
			bool troep = true;
			// fills the histogram and calculate the std. dev
			uint32_t index;
			for (uint32_t i = 0; i < n; i++)
			{
				index = static_cast<uint32_t>(floor((Data[i] - min) / binRange));
				if (index == noBins) { index -= 1; }
				bins[index]++;
				sum_dev += pow((Data[i] - Mean), 2);
			}

			Std = sqrt((float)(sum_dev / n));
			Calculated = true;
		}

		/// <summary>
		/// Make the calculations using the histogram
		/// </summary>
		void BinCalculations(uint16_t startC, uint16_t endC)
		{
			float sum_dev = 0.0;
			// Get the Sum
			uint32_t i = 0;
			for_each(begin(), end(), [&](uint32_t &b) { Sum += b * (startC + i++); });

			// Get Mean
			Mean = Sum / (float)n;

			// Get max
			for (int i = noBins - 1; i >= 0; i--)
			{
				if (bins[i] != 0)
				{
					max = i + startC;
					break;
				}
			}

			// Get min
			for (uint32_t i = 0; i < noBins; i++)
			{
				if (bins[i] != 0)
				{
					min = i + startC;
					break;
				}
			}

			// Get Max;
			Range = max - min;

			// Calculate Standard Deviation
			i = 0;
			for_each(begin(), end(), [&](uint32_t &b) { sum_dev += b * pow(((i++ + startC) - Mean), 2); });
			Std = sqrt((float)(sum_dev / n));
			Calculated = true;
		}
	private:
		uint32_t n_end = 0;
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & isDiscrete;
			ar & n;
			for (size_t dc = 0; dc < n; dc++) { ar & Data[dc]; }
			ar & noBins;
			for (size_t dc = 0; dc < noBins; dc++) { ar & bins[dc]; }
			ar & Calculated;
			ar & Mean;
			ar & Range;
			ar & min;
			ar & max;
			ar & Startbin;
			ar & EndBin;
			ar & binRange;
			ar & Std;
			ar & Sum;
			ar & Rows;
			ar & Cols;
		}
	};
}

typedef SoilMath::Stats<float, double, long double> floatStat_t;
typedef SoilMath::Stats<uchar, uint32_t, uint64_t> ucharStat_t;
typedef SoilMath::Stats<uint16_t, uint32_t, uint64_t> uint16Stat_t;
typedef SoilMath::Stats<uint32_t, uint32_t, uint64_t> uint32Stat_t;