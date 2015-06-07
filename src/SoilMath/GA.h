//! Genetic Algorithmes used for optimization problems
/*!
  * Use this class for optimization problems. It's currently optimized for Neural Network optimzation
  */
#pragma once

#include <bitset>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>

#include "NN.h"
#include "SoilMathTypes.h"
#include "MathException.h"

namespace SoilMath {

class GA {
public:
    /*!
   * \brief GA Standard constructor
   */
  GA();

  /*!
   * \brief GA Construction with a Neural Network initializers
   * \param nnfunction the Neural Network prediction function which results will be optimized
   * \param inputneurons the number of input neurons in the Neural Network don't count the bias
   * \param hiddenneurons the number of hidden neurons in the Neural Network don't count the bias
   * \param outputneurons the number of output neurons in the Neural Network
   */
  GA(NNfunctionType nnfunction, uint32_t inputneurons, uint32_t hiddenneurons,
     uint32_t outputneurons);

  /*!
   * \brief GA standard de constructor
   */
  ~GA();

  /*!
   * \brief Evolve Darwin would be proud!!! This function creates a population and itterates
   * through the generation till the maximum number off itterations has been reached of the
   * error is acceptable
   * \param inputValues complex vector with a pointer to the inputvalues
   * \param weights pointer to the vector of weights which will be optimized
   * \param prevWeights pointer to the pevious weight results
   * \param rangeweights pointer to the range of weights, currently it doesn't support indivudal ranges
   * this is because of the crossing
   * \param goal target value towards the Neural Network prediction function will be optimized
   * \param maxGenerations maximum number of itterations default value is 200
   * \param popSize maximum number of population, this should be an even number
   */
  void Evolve(const ComplexVect_t &inputValues, Weight_t &weights,
              std::vector<Weight_t> &prevWeights, MinMaxWeight_t rangeweights,
              Predict_t goal, uint32_t maxGenerations = 200,
              uint32_t popSize = 30);

  /*!
   * \brief Evolve Darwin would be proud!!! This function creates a population and itterates
   * through the generation till the maximum number off itterations has been reached of the
   * error is acceptable
   * \param inputValues complex vector with a pointer to the inputvalues
   * \param weights pointer to the vector of weights which will be optimized
   * \param rangeweights pointer to the range of weights, currently it doesn't support indivudal ranges
   * this is because of the crossing
   * \param goal target value towards the Neural Network prediction function will be optimized
   * \param maxGenerations maximum number of itterations default value is 200
   * \param popSize maximum number of population, this should be an even number
   */
  void Evolve(const InputLearnVector_t &inputValues, Weight_t &weights,
              MinMaxWeight_t rangeweights, OutputLearnVector_t &goal,
              uint32_t maxGenerations = 200, uint32_t popSize = 30);

private:
  NNfunctionType NNfuction; /**< The Neural Net work function*/
  uint32_t inputneurons; /**< the total number of input neurons*/
  uint32_t hiddenneurons; /**< the total number of hidden neurons*/
  uint32_t outputneurons; /**< the total number of output neurons*/

  /*!
   * \brief Genesis private function which is the spark of live, using a random seed
   * \param weights a pointer to the
   * \param rangeweights
   * \param popSize
   * \return
   */
  Population_t Genesis(const Weight_t &weights, MinMaxWeight_t rangeweights,
                       uint32_t popSize);

  /*!
   * \brief CrossOver
   * \param pop
   */
  void CrossOver(Population_t &pop);

  /*!
   * \brief Mutate
   * \param pop
   */
  void Mutate(Population_t &pop);

  /*!
   * \brief GrowToAdulthood
   * \param pop
   * \param inputValues
   * \param rangeweights
   * \param goal
   * \param totalFitness
   */
  void GrowToAdulthood(Population_t &pop, const ComplexVect_t &inputValues,
                       MinMaxWeight_t rangeweights, Predict_t goal,
                       float &totalFitness);

  /*!
   * \brief GrowToAdulthood
   * \param pop
   * \param inputValues
   * \param rangeweights
   * \param goal
   * \param totalFitness
   */
  void GrowToAdulthood(Population_t &pop, const InputLearnVector_t &inputValues,
                       MinMaxWeight_t rangeweights, OutputLearnVector_t &goal,
                       float &totalFitness);

  /*!
   * \brief SurvivalOfTheFittest
   * \param pop
   * \param totalFitness
   * \return
   */
  bool SurvivalOfTheFittest(Population_t &pop, float &totalFitness);

  /*!
   * \brief PopMemberSort
   * \param i
   * \param j
   * \return
   */
  static bool PopMemberSort(PopMember_t i, PopMember_t j) {
    return (i.Fitness < j.Fitness);
  }

  /*!
   *
   */
  template <typename T>
  inline Genome_t ConvertToGenome(T value, std::pair<T, T> range) {
    uint32_t intVal = static_cast<uint32_t>(
        (UINT32_MAX * (range.first + value)) / (range.second - range.first));
    Genome_t retVal(intVal);
    return retVal;
  }

  /*!
   *
   */
  template <typename T>
  inline T ConvertToValue(Genome_t gen, std::pair<T, T> range) {
    T retVal =
        range.first +
        (((range.second - range.first) * static_cast<T>(gen.to_ulong())) /
         UINT32_MAX);
    return retVal;
  }
};
}
