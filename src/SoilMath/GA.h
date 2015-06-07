/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

//! Genetic Algorithmes used for optimization problems
/*!
  * Use this class for optimization problems. It's currently optimized for
 * Neural Network optimzation
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
   * \param nnfunction the Neural Network prediction function which results will
   * be optimized
   * \param inputneurons the number of input neurons in the Neural Network don't
   * count the bias
   * \param hiddenneurons the number of hidden neurons in the Neural Network
   * don't count the bias
   * \param outputneurons the number of output neurons in the Neural Network
   */
  GA(NNfunctionType nnfunction, uint32_t inputneurons, uint32_t hiddenneurons,
     uint32_t outputneurons);

  /*!
   * \brief GA standard de constructor
   */
  ~GA();

  /*!
   * \brief Evolve Darwin would be proud!!! This function creates a population
   * and itterates
   * through the generation till the maximum number off itterations has been
   * reached of the
   * error is acceptable
   * \param inputValues complex vector with a reference to the inputvalues
   * \param weights reference to the vector of weights which will be optimized
   * \param prevWeights pointer to the pevious weight results
   * \param rangeweights pointer to the range of weights, currently it doesn't
   * support indivudal ranges
   * this is because of the crossing
   * \param goal target value towards the Neural Network prediction function
   * will be optimized
   * \param maxGenerations maximum number of itterations default value is 200
   * \param popSize maximum number of population, this should be an even number
   */
  void Evolve(const ComplexVect_t &inputValues, Weight_t &weights,
              std::vector<Weight_t> &prevWeights, MinMaxWeight_t rangeweights,
              Predict_t goal, uint32_t maxGenerations = 200,
              uint32_t popSize = 30);

  /*!
   * \brief Evolve Darwin would be proud!!! This function creates a population
   * and itterates
   * through the generation till the maximum number off itterations has been
   * reached of the
   * error is acceptable
   * \param inputValues complex vector with a reference to the inputvalues
   * \param weights reference to the vector of weights which will be optimized
   * \param rangeweights reference to the range of weights, currently it doesn't
   * support indivudal ranges
   * this is because of the crossing
   * \param goal target value towards the Neural Network prediction function
   * will be optimized
   * \param maxGenerations maximum number of itterations default value is 200
   * \param popSize maximum number of population, this should be an even number
   */
  void Evolve(const InputLearnVector_t &inputValues, Weight_t &weights,
              MinMaxWeight_t rangeweights, OutputLearnVector_t &goal,
              uint32_t maxGenerations = 200, uint32_t popSize = 30);

private:
  NNfunctionType NNfuction; /**< The Neural Net work function*/
  uint32_t inputneurons;    /**< the total number of input neurons*/
  uint32_t hiddenneurons;   /**< the total number of hidden neurons*/
  uint32_t outputneurons;   /**< the total number of output neurons*/

  /*!
   * \brief Genesis private function which is the spark of live, using a random
   * seed
   * \param weights a reference to the used Weight_t vector
   * \param rangeweights pointer to the range of weights, currently it doesn't
   * support indivudal ranges
   * \param popSize maximum number of population, this should be an even number
   * \return
   */
  Population_t Genesis(const Weight_t &weights, MinMaxWeight_t rangeweights,
                       uint32_t popSize);

  /*!
   * \brief CrossOver a private function where the partners mate with each other
   * The values or PopMember_t are expressed as bits or ar cut at the point CROSSOVER
   * the population members are paired with the nearest neighbor and new members are
   * created pairing the Genome_t of each other at the CROSSOVER point. Afterwards all
   * the top tiers partners are allowed to mate again.
   * \param pop reference to the population
   */
  void CrossOver(Population_t &pop);

  /*!
   * \brief Mutate a private function where individual bits from the Genome_t are mutated
   * at a random uniform distribution event defined by the MUTATIONRATE
   * \param pop reference to the population
   */
  void Mutate(Population_t &pop);

  /*!
   * \brief GrowToAdulthood a private function where the new population members serve as the
   * the input for the Neural Network prediction function. The results are weight against
   * the goal and this weight determine the fitness of the population member
   * \param pop reference to the population
   * \param inputValues complex vector with a reference to the inputvalues
   * \param rangeweights pointer to the range of weights, currently it doesn't
   * support indivudal ranges
   * \param goal a Predict_t type with the expected value
   * \param totalFitness a reference to the total population fitness
   */
  void GrowToAdulthood(Population_t &pop, const ComplexVect_t &inputValues,
                       MinMaxWeight_t rangeweights, Predict_t goal,
                       float &totalFitness);

  /*!
   * \brief GrowToAdulthood a private function where the new population members serve as the
   * the input for the Neural Network prediction function. The results are weight against
   * the goal and this weight determine the fitness of the population member
   * \param pop reference to the population
   * \param inputValues a InputLearnVector_t with a reference to the inputvalues
   * \param rangeweights pointer to the range of weights, currently it doesn't
   * support indivudal ranges
   * \param goal a Predict_t type with the expected value
   * \param totalFitness a reference to the total population fitness
   */
  void GrowToAdulthood(Population_t &pop, const InputLearnVector_t &inputValues,
                       MinMaxWeight_t rangeweights, OutputLearnVector_t &goal,
                       float &totalFitness);

  /*!
   * \brief SurvivalOfTheFittest a private function where a battle to the death commences
   * The fittest population members have the best chance of survival. Death is instigated
   * with a random uniform distibution. The elite members don't partake in this desctruction
   * The ELITISME rate indicate how many top tier members survive this catastrophic event.
   * \param inputValues a InputLearnVector_t with a reference to the inputvalues
   * \param totalFitness a reference to the total population fitness
   * \return
   */
  bool SurvivalOfTheFittest(Population_t &pop, float &totalFitness);

  /*!
   * \brief PopMemberSort a private function where the members are sorted according to
   * there fitness ranking
   * \param i left hand population member
   * \param j right hand population member
   * \return true if the left member is closser to the goal as the right member.
   */
  static bool PopMemberSort(PopMember_t i, PopMember_t j) {
    return (i.Fitness < j.Fitness);
  }

  /*!
   * \brief Conversion of the value of type T to Genome_t
   * \details Usage: Use <tt>ConvertToGenome<Type>(type, range)</tt>
   * \param value The current value wich should be converted to a Genome_t
   * \param range the range in which the value should fall, this is to have a Genome_t
   * which utilizes the complete range 0000...n till 1111...n
   */
  template <typename T>
  inline Genome_t ConvertToGenome(T value, std::pair<T, T> range) {
    uint32_t intVal = static_cast<uint32_t>(
        (UINT32_MAX * (range.first + value)) / (range.second - range.first));
    Genome_t retVal(intVal);
    return retVal;
  }

  /*!
   * \brief Conversion of the Genome to a value
   * \details Usage: use <tt>ConvertToValue<Type>(genome, range)
   * \param gen is the Genome which is to be converted
   * \param range is the range in which the value should fall
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
