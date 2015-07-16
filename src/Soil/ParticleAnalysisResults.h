/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#define MIN_EDGE_PIXELS                                                        \
  128 /**< the minimum amount of pixels needed as an edge before it can be     \
         analysed*/
#define FFT_DESCRIPTORS 12 /**< the minimum amount of FFT descriptors*/

#include "AnalysisResults.h"
#include "ShapeClassification.h"
#include "../SoilVision/Vision.h"

#include <boost/serialization/base_object.hpp>

namespace SoilAnalyzer {
/*!
 * \brief The ParticleAnalysisResults class
 * \details The Analysis results of an individual particle, it inherents form
 * the class AnalysisResults
 */
class ParticleAnalysisResults : public AnalysisResults {
public:
  bool Analyzed = false;      /**< Indicates whether the results are analyzed*/
  bool SmallParticle = false; /**< Indicates if the particle is considered to
                                 small to analyze the voor shape*/
  uint32_t Area;              /**< The total area of the particle as a pixel*/
  ShapeClassification Shape;  /**< The Shape indicator*/

  /*!
   * \brief ParticleAnalysisResults The constructor
   */
  ParticleAnalysisResults();

  /*!
    * \brief Deconstructor
    */
  ~ParticleAnalysisResults();

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version __attribute__((unused))) {
    ar &Analyzed;
    ar &SmallParticle;
    ar &Area;
    ar &Shape;
    ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnalysisResults);
  }
};
}
