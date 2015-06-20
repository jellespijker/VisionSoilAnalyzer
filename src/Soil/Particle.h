/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include <boost/serialization/base_object.hpp>

#include "Soil.h"
#include "../SoilMath/SoilMath.h"
#include "ParticleAnalysisResults.h"

namespace SoilAnalyzer {
/*!
 * \brief The Particle class
 * \details Representing an individual particle
 */
class Particle : public Soil {
public:
  /*!
 * \brief Particle the constructor
 */
  Particle();

  /*!
    * \brief the deconstructor
    */
  ~Particle();

  /*!
   * \brief Save the particle to disk
   * \param filename string indicating the filename
   */
  void Save(std::string &filename);

  /*!
   * \brief Load load the particle from disk
   * \param filename string indicating the filename
   */
  void Load(std::string &filename);

  ParticleAnalysisResults Analysis; /**< The Analysis results*/

  /*!
   * \brief Analyze the function which analyses the particle
   * \param nn the neural network to be used pased as a reference
   * \return thes analysis results
   */
  SoilAnalyzer::AnalysisResults Analyze(SoilMath::NN &nn);

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version __attribute__((unused))) {
    ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(Soil);
    ar &Analysis;
  }
};
}
