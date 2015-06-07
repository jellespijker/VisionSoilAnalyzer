/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <fstream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "GA.h"
#include "MathException.h"
#include "SoilMathTypes.h"
#include "FFT.h"

namespace SoilMath {
/*!
 * \brief The Neural Network class
 * \details This class is used to make prediction on large data set.
 */
class NN {
public:
    /*!
   * \brief NN
   * \param inputneurons
   * \param hiddenneurons
   * \param outputneurons
   */
  NN(uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons);

  /*!
   * \brief NN
   */
  NN();

  /*!
   * \brief ~NN
   */
  virtual ~NN();

  /*!
   * \brief Predict
   * \param input
   * \return
   */
  Predict_t Predict(ComplexVect_t input);

  /*!
   * \brief PredictLearn
   * \param input
   * \param inputweights
   * \param hiddenweights
   * \param inputneurons
   * \param hiddenneurons
   * \param outputneurons
   * \return
   */
  static Predict_t PredictLearn(ComplexVect_t input, Weight_t inputweights,
                                Weight_t hiddenweights, uint32_t inputneurons,
                                uint32_t hiddenneurons, uint32_t outputneurons);

  /*!
   * \brief SetInputWeights
   * \param value
   */
  void SetInputWeights(Weight_t value) { iWeights = value; }

  /*!
   * \brief SetHiddenWeights
   * \param value
   */
  void SetHiddenWeights(Weight_t value) { hWeights = value; }

  /*!
   * \brief SetBeta
   * \param value
   */
  void SetBeta(float value) { beta = value; }

  /*!
   * \brief Learn
   * \param input
   * \param cat
   * \param noOfDescriptorsUsed
   */
  void Learn(InputLearnVector_t input, OutputLearnVector_t cat,
             uint32_t noOfDescriptorsUsed);

  /*!
   * \brief SaveState
   * \param filename
   */
  void SaveState(string filename);

  /*!
   * \brief LoadState
   * \param filename
   */
  void LoadState(string filename);

  Weight_t iWeights; /**< */
  Weight_t hWeights; /**< */

private:
  std::vector<float> iNeurons;
  std::vector<float> hNeurons;
  std::vector<float> oNeurons;

  uint32_t hiddenNeurons;
  uint32_t inputNeurons;
  uint32_t outputNeurons;
  float beta;

  bool studied = false;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &BOOST_SERIALIZATION_NVP(inputNeurons);
    ar &BOOST_SERIALIZATION_NVP(hiddenNeurons);
    ar &BOOST_SERIALIZATION_NVP(outputNeurons);
    ar &BOOST_SERIALIZATION_NVP(iWeights);
    ar &BOOST_SERIALIZATION_NVP(hWeights);
    ar &BOOST_SERIALIZATION_NVP(beta);
    ar &BOOST_SERIALIZATION_NVP(studied);
  }
};
}
