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
/*!
 * \brief The AnalysisResults class
 * \details the analysis results this is the base class for particle and soil
 * analysis results
 */
class AnalysisResults {
public:
  /*!
 * \brief AnalysisResults Constructor
 */
  AnalysisResults();

  /*!
    * \brief AnalysisResult de-constructor
    */
  ~AnalysisResults();

  std::vector<
      ucharStat_t> RGB_Stat; /**< A Vector with the Stats class for each color
                                channel in the RGB*/
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
