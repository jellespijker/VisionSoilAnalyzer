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

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
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

		Stats(int noBins = 256, T1 startBin = 0, T1 endBin = 255)
		{
			min = numeric_limits<T1>::max();
			max = numeric_limits<T1>::min();
			Range = numeric_limits<T1>::max();
			Startbin = startBin;
			EndBin = endBin;
			this->noBins = noBins;
			bins = new uint32_t[noBins] {};

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
				//binRange = static_cast<T1>((EndBin + 1 - Startbin) / noBins);
			}
			else
			{
				isDiscrete = true;
				//binRange = static_cast<T1>(round((EndBin + 1 - Startbin) / noBins));
			}

			Data = data;
			Rows = rows;
			Cols = cols;
			bins = new uint32_t[noBins] {};
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

			bins = new uint32_t[noBins] {};
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

			//  fill histogram
			for (uint32_t i = 0; i < n; i++)
			{
				index = static_cast<uint32_t>(floor((Data[i] - min) / binRange));
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

			// fills the histogram and calculate the std. dev
			uint32_t index;
			for (uint32_t i = 0; i < n; i++)
			{
				index = static_cast<uint32_t>(floor((Data[i] - min) / binRange));
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
			for (uint32_t i = 0; i < noBins; i++)
			{
				Sum += bins[i] * (startC + i);
			}

			// Get Mean
			Mean = Sum / (float)n;

			// Get max
			for (uint32_t i = noBins; i > 0; i--)
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
			for (uint32_t i = 0; i < noBins; i++)
			{
				sum_dev += bins[i] * pow(((i + startC) - Mean), 2);
			}
			Std = sqrt((float)(sum_dev / n));
			Calculated = true;
		}
	private:
		bool MinMaxSumCalc = false;
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(isDiscrete);
			ar & BOOST_SERIALIZATION_NVP(n);
			for (size_t dc = 0; dc < n; dc++) {
				std::stringstream ss;
				ss << "Data_" << dc;
				ar & boost::serialization::make_nvp(ss.str().c_str(), Data[dc]);
			}
			ar & BOOST_SERIALIZATION_NVP(noBins);
			for (size_t dc = 0; dc < noBins; dc++) {
				std::stringstream ss;
				ss << "Bin_" << dc;
				ar & boost::serialization::make_nvp(ss.str().c_str(), bins[dc]);
			}
			ar & BOOST_SERIALIZATION_NVP(Calculated);
			ar & BOOST_SERIALIZATION_NVP(Mean);
			ar & BOOST_SERIALIZATION_NVP(Range);
			ar & BOOST_SERIALIZATION_NVP(min);
			ar & BOOST_SERIALIZATION_NVP(max);
			ar & BOOST_SERIALIZATION_NVP(Startbin);
			ar & BOOST_SERIALIZATION_NVP(EndBin);
			ar & BOOST_SERIALIZATION_NVP(binRange);
			ar & BOOST_SERIALIZATION_NVP(Std);
			ar & BOOST_SERIALIZATION_NVP(Sum);
			ar & BOOST_SERIALIZATION_NVP(Rows);
			ar & BOOST_SERIALIZATION_NVP(Cols);
		}
	};
}

typedef SoilMath::Stats<float, double, long double> floatStat_t;
typedef SoilMath::Stats<uchar, uint32_t, uint64_t> ucharStat_t;