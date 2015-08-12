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
#include <boost/serialization/version.hpp>

#include "GA.h"
#include "MathException.h"
#include "SoilMathTypes.h"
#include "FFT.h"

namespace SoilMath {
/*!
 * \brief The Neural Network class
 * \details This class is used to make prediction on large data set. Using self
 * learning algoritmes
 */
class NN {
public:
  /*!
 * \brief NN constructor for the Neural Net
 * \param inputneurons number of input neurons
 * \param hiddenneurons number of hidden neurons
 * \param outputneurons number of output neurons
 */
  NN(uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons);

  /*!
   * \brief NN constructor for the Neural Net
   */
  NN();

  /*!
   * \brief ~NN virtual deconstructor for the Neural Net
   */
  virtual ~NN();

  /*!
   * \brief Predict The prediction function.
   * \details In this function the neural net is setup and the input which are
   * the complex values descriping the contour in the frequency domein serve as
   * input. The absolute value of these im. number because I'm not interrested
   * in the orrientation of the particle but more in the degree of variations.
   * \param input vector of complex input values, these're the Fourier
   * descriptors
   * \return a real valued vector of the output neurons
   */
  Predict_t Predict(ComplexVect_t input);

  /*!
   * \brief PredictLearn a static function used in learning of the weights
   * \details It starts a new Neural Network object and passes all the
   * paramaters in to this newly created object. After this the predict function
   * is called and the value is returned. This work around was needed to pass
   * the neural network to the Genetic Algorithm class.
   * \param input a complex vector of input values
   * \param inputweights the input weights
   * \param hiddenweights the hidden weights
   * \param inputneurons the input neurons
   * \param hiddenneurons the hidden neurons
   * \param outputneurons the output neurons
   * \return
   */
  static Predict_t PredictLearn(ComplexVect_t input, Weight_t inputweights,
                                Weight_t hiddenweights, uint32_t inputneurons,
                                uint32_t hiddenneurons, uint32_t outputneurons);

  /*!
   * \brief SetInputWeights a function to set the input weights
   * \param value the real valued vector with the values
   */
  void SetInputWeights(Weight_t value) { iWeights = value; }

  /*!
   * \brief SetHiddenWeights a function to set the hidden weights
   * \param value the real valued vector with the values
   */
  void SetHiddenWeights(Weight_t value) { hWeights = value; }

  /*!
   * \brief SetBeta a function to set the beta value
   * \param value a floating value ussualy between 0.5 and 1.5
   */
  void SetBeta(float value) { beta = value; }
  float GetBeta() { return beta; }

  /*!
   * \brief Learn the learning function
   * \param input a vector of vectors with complex input values
   * \param cat a vector of vectors with the know output values
   * \param noOfDescriptorsUsed the total number of descriptos which should be
   * used
   */
  void Learn(InputLearnVector_t input, OutputLearnVector_t cat,
             uint32_t noOfDescriptorsUsed);

  /*!
   * \brief SaveState Serialize and save the values of the Neural Net to disk
   * \details Save the Neural Net in XML valued text file to disk so that a
   * object can
   * be reconstructed on a latter stadia.
   * \param filename a string indicating the file location and name
   */
  void SaveState(string filename);

  /*!
   * \brief LoadState Loads the previouse saved Neural Net from disk
   * \param filename a string indicating the file location and name
   */
  void LoadState(string filename);

  Weight_t iWeights; /**< a vector of real valued floating point input weights*/
  Weight_t hWeights; /**< a vector of real valued floating point hidden weight*/

  uint32_t MaxGenUsedByGA = 200;
  uint32_t PopulationSizeUsedByGA = 30;
  float MutationrateUsedByGA = 0.075f;
  uint32_t ElitismeUsedByGA = 4;
  float EndErrorUsedByGA = 0.001;
  uint32_t MaxWeightUsedByGA = 50;
  uint32_t MinWeightUSedByGa = -50;

  uint32_t GetInputNeurons() { return inputNeurons; }
  void SetInputNeurons(uint32_t value);

  uint32_t GetHiddenNeurons() { return hiddenNeurons; }
  void SetHiddenNeurons(uint32_t value);

  uint32_t GetOutputNeurons() { return outputNeurons; }
  void SetOutputNeurons(uint32_t value);

  bool studied =
      false; /**< a value indicating if the weights are a results of a
                learning curve*/
private:
  std::vector<float> iNeurons; /**< a vector of input values, the bias is
                                  included, the bias is included and
                                    is the first value*/
  std::vector<float>
      hNeurons; /**< a vector of hidden values, the bias is included and
                     is the first value*/
  std::vector<float> oNeurons; /**< a vector of output values*/

  uint32_t hiddenNeurons = 50; /**< number of hidden neurons minus bias*/
  uint32_t inputNeurons = 20;  /**< number of input neurons minus bias*/
  uint32_t outputNeurons = 18; /**< number of output neurons*/
  float beta; /**< the beta value, this indicates the steepness of the sigmoid
                 function*/

  friend class boost::serialization::access; /**< a private friend class so the
                                                serialization can access all
                                                the needed functions*/
  /*!
   * \brief serialization function
   * \param ar the object
   * \param version the version of the class
   */
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version == 0) {
      ar &BOOST_SERIALIZATION_NVP(inputNeurons);
      ar &BOOST_SERIALIZATION_NVP(hiddenNeurons);
      ar &BOOST_SERIALIZATION_NVP(outputNeurons);
      ar &BOOST_SERIALIZATION_NVP(iWeights);
      ar &BOOST_SERIALIZATION_NVP(hWeights);
      ar &BOOST_SERIALIZATION_NVP(beta);
      ar &BOOST_SERIALIZATION_NVP(studied);
      ar &BOOST_SERIALIZATION_NVP(MaxGenUsedByGA);
      ar &BOOST_SERIALIZATION_NVP(PopulationSizeUsedByGA);
      ar &BOOST_SERIALIZATION_NVP(MutationrateUsedByGA);
      ar &BOOST_SERIALIZATION_NVP(ElitismeUsedByGA);
      ar &BOOST_SERIALIZATION_NVP(EndErrorUsedByGA);
      ar &BOOST_SERIALIZATION_NVP(MaxWeightUsedByGA);
      ar &BOOST_SERIALIZATION_NVP(MinWeightUSedByGa);
    }
  }
};
}
BOOST_CLASS_VERSION(SoilMath::NN, 0)
