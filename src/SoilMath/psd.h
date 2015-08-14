/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include "Stats.h"
#include <boost/serialization/base_object.hpp>

namespace SoilMath {
class PSD : public SoilMath::Stats<double, double, long double> {
private:

  uint32_t DetBin(float value) {
    uint32_t i = noBins - 1;
    while (i > 0) {
      if (value > BinRanges[i]) {
        return i;
      }
      i--;
    }
    return 0;
  }

  void BasicCalculatePSD() {
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
    uint32_t index = 0;
    Mean = Sum / (float)n;
    Range = max - min;
    for (uint32_t i = 0; i < n; i++) {
      index = DetBin(Data[i]);
      bins[index]++;
      sum_dev += pow((Data[i] - Mean), 2);
    }
    Std = sqrt((float)(sum_dev / n));
    getCFD();
    Calculated = true;
  }
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version == 0) {
      ar &boost::serialization::base_object<
          SoilMath::Stats<double, double, long double>>(*this);
      for (size_t dc = 0; dc < noBins; dc++) {
        ar &BinRanges[dc];
      }
    }
  }

public:
  double *BinRanges;

  PSD() : SoilMath::Stats<double, double, long double>(1, 0, 0) {}

  PSD(double *data, uint32_t nodata, double *binranges, uint32_t nobins,
      uint32_t endbin)
      : SoilMath::Stats<double, double, long double>(nobins, 0, endbin), BinRanges(binranges) {
    Data = data;
    Rows = nodata;
    Cols = 1;

    BasicCalculatePSD();
  }
};
}
BOOST_CLASS_VERSION(SoilMath::PSD, 0)
