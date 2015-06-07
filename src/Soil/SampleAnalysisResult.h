/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include "AnalysisResults.h"
#include "ParticleAnalysisResults.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

namespace SoilAnalyzer {
class SampleAnalysisResult : public AnalysisResults {
public:
  SampleAnalysisResult();
  ~SampleAnalysisResult();

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnalysisResults);
  }
};
}
