/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#define MORPHOLOGICALFILTER_VERSION 1

#include "ImageProcessing.h"

namespace Vision {
class MorphologicalFilter : public ImageProcessing {
public:
  enum FilterType { OPEN, CLOSE, ERODE, DILATE, NONE };

  MorphologicalFilter();
  MorphologicalFilter(FilterType filtertype);
  MorphologicalFilter(const Mat &src, FilterType filtertype = FilterType::NONE);
  MorphologicalFilter(const MorphologicalFilter &rhs);

  ~MorphologicalFilter();

  MorphologicalFilter &operator=(MorphologicalFilter &rhs);

  void Dilation(const Mat &mask, bool chain = false);
  void Erosion(const Mat &mask, bool chain = false);

  void Close(const Mat &mask, bool chain = false);
  void Open(const Mat &mask, bool chain = false);

private:
  void Filter(const Mat &mask, bool chain, uchar startVal, uchar newVal,
              uchar switchVal);
};
}
