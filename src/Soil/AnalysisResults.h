/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include <stdint.h>
#include <utility>
#include <vector>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "AnalyseType.h"
#include "../SoilMath/SoilMath.h"

namespace SoilAnalyzer {
class AnalysisResults {
public:
  AnalysisResults();
  ~AnalysisResults();

  std::vector<ucharStat_t> RGB_Stat;
  std::vector<floatStat_t> LAB_Stat;
  floatStat_t RI_Stat;

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &RGB_Stat;
    ar &LAB_Stat;
    ar &RI_Stat;
  }
};
}
