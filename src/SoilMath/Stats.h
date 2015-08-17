/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#define MAX_UINT8_VALUE 256
#define VECTOR_CALC 1

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
#include <boost/serialization/version.hpp>
#include <boost/math/distributions/students_t.hpp>

#include "MathException.h"
#include "SoilMathTypes.h"
#include "CommonOperations.h"

namespace SoilMath {

/*!
 * \brief Stats class
 * \details Usage Stats<type1, type2, type3>Stats() type 1, 2 and 3 shoudl be of
 * the same value and concecuative in size
 */
template <typename T1, typename T2, typename T3> class Stats {
public:
  bool isDiscrete = true; /**< indicates if the data is discrete or real*/

  T1 *Data = nullptr;       /**< Pointer the data*/
  uint32_t *bins = nullptr; /**< the histogram*/
  double *CFD = nullptr;    /**< the CFD*/
  bool Calculated = false;  /**< indication if the data has been calculated*/
  float Mean = 0.0;         /**< the mean value of the data*/
  uint32_t n = 0;           /**< number of data points*/
  uint32_t noBins = 0;      /**< number of bins*/
  T1 Range = 0;             /**< range of the data*/
  T1 min = 0;               /**< minimum value*/
  T1 max = 0;               /**< maximum value*/
  T1 Startbin = 0;          /**< First bin value*/
  T1 EndBin = 0;            /**< End bin value*/
  T1 binRange = 0;          /**< the range of a single bin*/
  float Std = 0.0;          /**< standard deviation*/
  T3 Sum = 0;               /**< total sum of all the data values*/
  uint16_t Rows = 0;        /**< number of rows from the data matrix*/
  uint16_t Cols = 0;        /**< number of cols from the data matrix*/
  bool StartAtZero = true;  /**< indication of the minimum value starts at zero
                               or could be less*/
  double *BinRanges = nullptr;
  double HighestPDF = 0.;

  uint32_t *begin() { return &bins[0]; }    /**< pointer to the first bin*/
  uint32_t *end() { return &bins[noBins]; } /**< pointer to the last + 1 bin*/

  /*!
   * \brief WelchTest Compare the sample using the Welch's Test
   * \details (source:
   * http://www.boost.org/doc/libs/1_57_0/libs/math/doc/html/math_toolkit/stat_tut/weg/st_eg/two_sample_students_t.html)
   * \param statComp Statiscs Results of which it should be tested against
   * \return
   */
  bool WelchTest(SoilMath::Stats<T1, T2, T3> &statComp) {
    double alpha = 0.05;
    // Degrees of freedom:
    double v = statComp.Std * statComp.Std / statComp.n +
               this->Std * this->Std / this->n;
    v *= v;
    double t1 = statComp.Std * statComp.Std / statComp.n;
    t1 *= t1;
    t1 /= (statComp.n - 1);
    double t2 = this->Std * this->Std / this->n;
    t2 *= t2;
    t2 /= (this->n - 1);
    v /= (t1 + t2);
    // t-statistic:
    double t_stat = (statComp.Mean - this->Mean) /
                    sqrt(statComp.Std * statComp.Std / statComp.n +
                         this->Std * this->Std / this->n);
    //
    // Define our distribution, and get the probability:
    //
    boost::math::students_t dist(v);
    double q = cdf(complement(dist, fabs(t_stat)));

    bool rejected = false;
    // Sample 1 Mean == Sample 2 Mean test the NULL hypothesis, the two means
    // are the same
    if (q < alpha / 2)
      rejected = false;
    else
      rejected = true;
    return rejected;
  }

  /*!
   * \brief Stats Constructor
   * \param rhs Right hand side
   */
  Stats(const Stats &rhs)
      : bins{new uint32_t[rhs.noBins]{0}}, CFD{new double[rhs.noBins]{}},
        BinRanges{new double[rhs.noBins]{}} {
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
    std::copy(rhs.CFD, rhs.CFD + rhs.noBins, this->CFD);
    std::copy(rhs.BinRanges, rhs.BinRanges + rhs.noBins, this->BinRanges);
    this->Data = rhs.Data;
    this->StartAtZero = rhs.StartAtZero;
    this->HighestPDF = rhs.HighestPDF;
  }

  /*!
   * \brief operator = Assigmnet operator
   * \param rhs right hand side
   * \return returns the right hand side
   */
  Stats &operator=(Stats const &rhs) {
    if (&rhs != this) {
      Data = rhs.Data;

      if (bins != nullptr) {
        delete[] bins;
        bins = nullptr;
      }
      if (CFD != nullptr) {
        delete[] CFD;
        CFD = nullptr;
      }
      if (BinRanges != nullptr) {
        delete[] BinRanges;
        BinRanges = nullptr;
      }

      bins = new uint32_t[rhs.noBins];    // leak
      CFD = new double[rhs.noBins];       // leak
      BinRanges = new double[rhs.noBins]; // leak
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
      std::copy(rhs.CFD, rhs.CFD + rhs.noBins, this->CFD);
      std::copy(rhs.BinRanges, rhs.BinRanges + rhs.noBins, this->BinRanges);
      this->StartAtZero = rhs.StartAtZero;
      this->HighestPDF = rhs.HighestPDF;
    }
    return *this;
  }

  /*!
   * \brief Stats Constructor
   * \param noBins number of bins with which to build the histogram
   * \param startBin starting value of the first bin
   * \param endBin end value of the second bin
   */
  Stats(int noBins = 256, T1 startBin = 0, T1 endBin = 255) {
    min = std::numeric_limits<T1>::max();
    max = std::numeric_limits<T1>::min();
    Range = std::numeric_limits<T1>::max();
    Startbin = startBin;
    EndBin = endBin;
    this->noBins = noBins;
    bins = new uint32_t[noBins]{0};   // leak
    CFD = new double[noBins]{};       // leak
    BinRanges = new double[noBins]{}; // leak

    if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) ||
        typeid(T1) == typeid(long double)) {
      isDiscrete = false;
      binRange = static_cast<T1>((EndBin - Startbin) / noBins);
    } else {
      isDiscrete = true;
      binRange = static_cast<T1>(round((EndBin - Startbin) / noBins));
    }
  }

  /*!
   * \brief Stats constructor
   * \param data Pointer to the data
   * \param rows Number of rows
   * \param cols Number of Columns
   * \param noBins Number of bins
   * \param startBin Value of the start bin
   * \param startatzero bool indicating if the bins should be shifted from zero
   */
  Stats(T1 *data, uint16_t rows, uint16_t cols, int noBins = 256,
        T1 startBin = 0, bool startatzero = true) {
    min = std::numeric_limits<T1>::max();
    max = std::numeric_limits<T1>::min();
    Range = max - min;

    Startbin = startBin;
    EndBin = startBin + noBins;
    StartAtZero = startatzero;

    if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) ||
        typeid(T1) == typeid(long double)) {
      isDiscrete = false;
    } else {
      isDiscrete = true;
    }

    Data = data;
    Rows = rows;
    Cols = cols;
    bins = new uint32_t[noBins]{0};
    CFD = new double[noBins]{};
    BinRanges = new double[noBins]{};
    this->noBins = noBins;
    if (isDiscrete) {
      BasicCalculate();
    } else {
      BasicCalculateFloat();
    }
  }

  /*!
   * \brief Stats Constructor
   * \param data Pointer the data
   * \param rows Number of rows
   * \param cols Number of Columns
   * \param mask the mask should have the same size as the data a value of zero
   * indicates that the data pointer doesn't exist. A 1 indicates that the data
   * pointer is to be used
   * \param noBins Number of bins
   * \param startBin Value of the start bin
   * \param startatzero indicating if the bins should be shifted from zero
   */
  Stats(T1 *data, uint16_t rows, uint16_t cols, uchar *mask, int noBins = 256,
        T1 startBin = 0, bool startatzero = true) {
    min = std::numeric_limits<T1>::max();
    max = std::numeric_limits<T1>::min();
    Range = max - min;

    Startbin = startBin;
    EndBin = startBin + noBins;
    StartAtZero = startatzero;

    if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) ||
        typeid(T1) == typeid(long double)) {
      isDiscrete = false;
    } else {
      isDiscrete = true;
    }

    Data = data;
    Rows = rows;
    Cols = cols;
    bins = new uint32_t[noBins]{0};
    CFD = new double[noBins]{};
    BinRanges = new double[noBins]{};
    this->noBins = noBins;
    if (isDiscrete) {
      BasicCalculate(mask);
    } else {
      BasicCalculateFloat(mask);
    }
  }

  /*!
   * \brief Stats Constructor
   * \param binData The histogram data
   * \param startC start counter
   * \param endC end counter
   */
  Stats(T2 *binData, uint16_t startC, uint16_t endC) {
    noBins = endC - startC;
    Startbin = startC;
    EndBin = endC;
    uint32_t i = noBins;

    if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double) ||
        typeid(T1) == typeid(long double)) {
      isDiscrete = false;
      throw Exception::MathException(EXCEPTION_TYPE_NOT_SUPPORTED,
                                     EXCEPTION_TYPE_NOT_SUPPORTED_NR);
    } else {
      isDiscrete = true;
    }

    bins = new uint32_t[noBins]{0};
    CFD = new double[noBins]{};
    BinRanges = new double[noBins]{};
    while (i-- > 0) {
      bins[i] = binData[i];
      n += binData[i];
    }
    BinCalculations(startC, endC);
  }

  ~Stats() {
    Data == nullptr;
    if (bins != nullptr) {
      delete[] bins;
      bins = nullptr;
    }
    if (CFD != nullptr) {
      delete[] CFD;
      CFD = nullptr;
    }
    if (BinRanges != nullptr) {
      delete[] BinRanges;
      BinRanges = nullptr;
    }
  }

  /*!
   * \brief BasicCalculateFloat execute the basic float data calculations
   */
  void BasicCalculateFloat() {
    float sum_dev = 0.0;
    n = Rows * Cols;
    for (uint32_t i = 0; i < n; i++) {
      if (Data[i] > max) {
        max = Data[i];
      }
      if (Data[i] < min) {
        min = Data[i];
      }
      Sum += Data[i];
    }
    binRange = (max - min) / noBins;
    uint32_t index = 0;
    Mean = Sum / (float)n;
    Range = max - min;

    if (StartAtZero) {
      for (uint32_t i = 0; i < n; i++) {
        index = static_cast<uint32_t>(Data[i] / binRange);
        if (index == noBins) {
          index -= 1;
        }
        bins[index]++;
        sum_dev += pow((Data[i] - Mean), 2);
      }
    } else {
      for (uint32_t i = 0; i < n; i++) {
        index = static_cast<uint32_t>((Data[i] - min) / binRange);
        if (index == noBins) {
          index -= 1;
        }
        bins[index]++;
        sum_dev += pow((Data[i] - Mean), 2);
      }
    }
    Std = sqrt((float)(sum_dev / n));
    getCFD();
    Calculated = true;
  }

  /*!
   * \brief BasicCalculateFloat execute the basic float data calculations with a
   * mask
   * \param mask uchar mask type 0 don't calculate, 1 calculate
   */
  void BasicCalculateFloat(uchar *mask) {
    float sum_dev = 0.0;
    n = Rows * Cols;
    uint32_t nmask = 0;
    for (uint32_t i = 0; i < n; i++) {
      if (mask[i] != 0) {
        if (Data[i] > max) {
          max = Data[i];
        }
        if (Data[i] < min) {
          min = Data[i];
        }
        Sum += Data[i];
        nmask++;
      }
    }
    binRange = (max - min) / noBins;
    uint32_t index = 0;
    Mean = Sum / (float)nmask;
    Range = max - min;
    if (StartAtZero) {
      for (uint32_t i = 0; i < n; i++) {
        if (mask[i] != 0) {
          index = static_cast<uint32_t>(Data[i] / binRange);
          if (index == noBins) {
            index -= 1;
          }
          bins[index]++;
          sum_dev += pow((Data[i] - Mean), 2);
        }
      }
    } else {
      for (uint32_t i = 0; i < n; i++) {
        if (mask[i] != 0) {
          index = static_cast<uint32_t>((Data[i] - min) / binRange);
          if (index == noBins) {
            index -= 1;
          }
          bins[index]++;
          sum_dev += pow((Data[i] - Mean), 2);
        }
      }
    }
    Std = sqrt((float)(sum_dev / nmask));
    getCFD();
    Calculated = true;
  }

  /*!
   * \brief BasicCalculate execute the basic discrete data calculations
   */
  void BasicCalculate() {
    double sum_dev = 0.0;
    n = Rows * Cols;
    for (uint32_t i = 0; i < n; i++) {
      if (Data[i] > max) {
        max = Data[i];
      }
      if (Data[i] < min) {
        min = Data[i];
      }
      Sum += Data[i];
    }
    binRange = static_cast<T1>(ceil((max - min) / static_cast<float>(noBins)));
    if (binRange == 0) {
      binRange = 1;
    }
    Mean = Sum / (float)n;
    Range = max - min;

    uint32_t index;
    if (StartAtZero) {
      std::for_each(Data, Data + n, [&](T1 &d) {
        index = static_cast<uint32_t>(d / binRange);
        if (index == noBins) {
          index -= 1;
        }
        bins[index]++;
        sum_dev += pow((d - Mean), 2);
      });
    } else {
      std::for_each(Data, Data + n, [&](T1 &d) {
        index = static_cast<uint32_t>((d - min) / binRange);
        if (index == noBins) {
          index -= 1;
        }
        bins[index]++;
        sum_dev += pow((d - Mean), 2);
      });
    }
    Std = sqrt((float)(sum_dev / n));
    getCFD();
    Calculated = true;
  }

  /*!
   * \brief BasicCalculate execute the basic discrete data calculations with
   * mask
   * \param mask uchar mask type 0 don't calculate, 1 calculate
   */
  void BasicCalculate(uchar *mask) {
    double sum_dev = 0.0;
    n = Rows * Cols;
    uint32_t nmask = 0;
    uint32_t i = 0;
    std::for_each(Data, Data + n, [&](T1 &d) {
      if (mask[i++] != 0) {
        if (d > max) {
          max = d;
        }
        if (d < min) {
          min = d;
        }
        Sum += d;
        nmask++;
      }
    });
    binRange = static_cast<T1>(ceil((max - min) / static_cast<float>(noBins)));
    Mean = Sum / (float)nmask;
    Range = max - min;

    uint32_t index;
    if (StartAtZero) {
      i = 0;
      std::for_each(Data, Data + n, [&](T1 &d) {
        if (mask[i++] != 0) {
          index = static_cast<uint32_t>(d / binRange);
          if (index == noBins) {
            index -= 1;
          }
          bins[index]++;
          sum_dev += pow((d - Mean), 2);
        }
      });
    } else {
      i = 0;
      std::for_each(Data, Data + n, [&](T1 &d) {
        if (mask[i++] != 0) {
          index = static_cast<uint32_t>((d - min) / binRange);
          if (index == noBins) {
            index -= 1;
          }
          bins[index]++;
          sum_dev += pow((d - Mean), 2);
        }
      });
    }
    Std = sqrt((float)(sum_dev / nmask));
    getCFD();
    Calculated = true;
  }

  /*!
   * \brief BinCalculations excute the cacluations with the histogram
   * \param startC start counter
   * \param endC end counter
   */
  void BinCalculations(uint16_t startC, uint16_t endC __attribute__((unused))) {
    float sum_dev = 0.0;
    // Get the Sum
    uint32_t i = 0;
    for_each(begin(), end(), [&](uint32_t &b) { Sum += b * (startC + i++); });

    // Get Mean
    Mean = Sum / (float)n;

    // Get max
    for (int i = noBins - 1; i >= 0; i--) {
      if (bins[i] != 0) {
        max = i + startC;
        break;
      }
    }

    // Get min
    for (uint32_t i = 0; i < noBins; i++) {
      if (bins[i] != 0) {
        min = i + startC;
        break;
      }
    }

    // Get Range;
    Range = max - min;

    // Calculate Standard Deviation
    i = 0;
    for_each(begin(), end(), [&](uint32_t &b) {
      sum_dev += b * pow(((i++ + startC) - Mean), 2);
    });
    Std = sqrt((float)(sum_dev / n));
    getCFD();
    Calculated = true;
  }

  uint32_t HighestFrequency() {
    uint32_t freq = 0;
    std::for_each(begin(), end(), [&](uint32_t &B) {
      if (B > freq) {
        freq = B;
      }
    });
    return freq;
  }

  void GetPDFfunction(std::vector<double> &xAxis, std::vector<double> &yAxis,
                      double Step, double start = 0, double stop = 7) {
    uint32_t resolution;
    resolution = static_cast<uint32_t>(((stop - start) / Step) + 0.5);
    xAxis.push_back(min);
    for (uint32_t i = 1; i < resolution; i++) {
      double xVal = xAxis[xAxis.size() - 1] + Step;
      xAxis.push_back(xVal);
      double yVal = (1 / (Std * 2.506628274631)) *
                    exp(-(pow((xVal - Mean), 2) / (2 * pow(Std, 2))));
      yAxis.push_back(yVal);
    }
  }

protected:
  uint32_t n_end = 0; /**< data end counter used with mask*/

  /*!
   * \brief getCFD get the CFD matrix;
   */
  void getCFD() {
    uint32_t *sumBin = new uint32_t[noBins];
    sumBin[0] = bins[0];
    CFD[0] = (static_cast<double>(sumBin[0]) / static_cast<double>(n)) * 100.;
    for (uint32_t i = 1; i < noBins; i++) {
      sumBin[i] = (sumBin[i - 1] + bins[i]);
      CFD[i] = (static_cast<double>(sumBin[i]) / static_cast<double>(n)) * 100.;
      if (CFD[i] > HighestPDF) {
        HighestPDF = CFD[i];
      }
    }
    delete[] sumBin;
  }

  friend class boost::serialization::access; /**< Serialization class*/

  /*!
   * \brief serialize the object
   * \param ar argument
   * \param version
   */
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version == 0) {
      ar &isDiscrete;
      ar &n;
      ar &noBins;
      for (size_t dc = 0; dc < noBins; dc++) {
        ar &bins[dc];
      }
      for (size_t dc = 0; dc < noBins; dc++) {
        ar &CFD[dc];
      }
      for (size_t dc = 0; dc < noBins; dc++) {
        ar &BinRanges[dc];
      }
      ar &Calculated;
      ar &Mean;
      ar &Range;
      ar &min;
      ar &max;
      ar &Startbin;
      ar &EndBin;
      ar &binRange;
      ar &Std;
      ar &Sum;
      ar &Rows;
      ar &Cols;
      ar &StartAtZero;
      ar &HighestPDF;
    }
  }
};
}

typedef SoilMath::Stats<float, double, long double>
    floatStat_t; /**< floating Stat type*/
typedef SoilMath::Stats<uchar, uint32_t, uint64_t>
    ucharStat_t; /**< uchar Stat type*/
typedef SoilMath::Stats<uint16_t, uint32_t, uint64_t>
    uint16Stat_t; /**< uint16 Stat type*/
typedef SoilMath::Stats<uint32_t, uint32_t, uint64_t>
    uint32Stat_t; /**< uint32 Stat type*/
BOOST_CLASS_VERSION(floatStat_t, 0)
BOOST_CLASS_VERSION(ucharStat_t, 0)
BOOST_CLASS_VERSION(uint16Stat_t, 0)
BOOST_CLASS_VERSION(uint32Stat_t, 0)
