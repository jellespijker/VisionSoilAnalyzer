/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "GA.h"

namespace SoilMath {
GA::GA() {}

GA::GA(NNfunctionType nnfunction, uint32_t inputneurons, uint32_t hiddenneurons,
       uint32_t outputneurons) {
  this->NNfuction = nnfunction;
  this->inputneurons = inputneurons;
  this->hiddenneurons = hiddenneurons;
  this->outputneurons = outputneurons;
}

GA::~GA() {}

void GA::Evolve(const InputLearnVector_t &inputValues, Weight_t &weights,
                MinMaxWeight_t rangeweights, OutputLearnVector_t &goal,
                uint32_t maxGenerations, uint32_t popSize) {
  minOptim = inputValues.size() * goal[0].OutputNeurons.size();
  minOptim = -minOptim;
  maxOptim = 3 * goal[0].OutputNeurons.size() * inputValues.size();
  oldElit = Elitisme;
  oldMutation = MutationRate;
  this->inputValues = inputValues;
  this->rangeweights = rangeweights;
  this->goal = goal;

  // Create the population
  Population_t pop = Genesis(weights, popSize);
  float totalFitness = 0.0;
  for (uint32_t i = 0; i < maxGenerations; i++) {
    CrossOver(pop);
    Mutate(pop);
    totalFitness = 0.0;
    GrowToAdulthood(pop, totalFitness);
    if (SurvivalOfTheFittest(pop, totalFitness)) {
      break;
    }
  }
  weights = pop[0].weights;
}

Population_t GA::Genesis(const Weight_t &weights, uint32_t popSize) {
  if (popSize < 1)
    return Population_t();

  Population_t pop;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);
  std::uniform_real_distribution<float> dis(rangeweights.first,
                                            rangeweights.second);

  for (uint32_t i = 0; i < popSize; i++) {
    PopMember_t I;
    for (uint32_t j = 0; j < weights.size(); j++) {
      I.weights.push_back(dis(gen));
      I.weightsGen.push_back(
          ConvertToGenome<float>(I.weights[j], rangeweights));
    }
    pop.push_back(I);
  }
  return pop;
}

void GA::CrossOver(Population_t &pop) {
  Population_t newPop; // create a new population
  PopMember_t newPopMembers[2];
  SplitGenome_t Split[2];

  for (uint32_t i = 0; i < pop.size(); i += 2) {

    for (uint32_t j = 0; j < pop[i].weights.size(); j++) {
      // Split A
      Split[0].first = std::bitset<CROSSOVER>(
          pop[i].weightsGen[j].to_string().substr(0, CROSSOVER));
      Split[0].second = std::bitset<GENE_MAX - CROSSOVER>(
          pop[i].weightsGen[j].to_string().substr(CROSSOVER,
                                                  GENE_MAX - CROSSOVER));

      // Split B
      Split[1].first = std::bitset<CROSSOVER>(
          pop[i + 1].weightsGen[j].to_string().substr(0, CROSSOVER));
      Split[1].second = std::bitset<GENE_MAX - CROSSOVER>(
          pop[i + 1].weightsGen[j].to_string().substr(CROSSOVER,
                                                      GENE_MAX - CROSSOVER));

      // Mate A and B to AB and BA
      newPopMembers[0].weightsGen.push_back(
          Genome_t(Split[0].first.to_string() + Split[1].second.to_string()));
      newPopMembers[1].weightsGen.push_back(
          Genome_t(Split[1].first.to_string() + Split[0].second.to_string()));
    }
    newPop.push_back(newPopMembers[0]);
    newPop.push_back(newPopMembers[1]);
    newPopMembers[0].weightsGen.clear();
    newPopMembers[1].weightsGen.clear();
  }

  // Allow the top tiers population partners to mate again
  uint32_t halfN = pop.size() / 2;
  for (uint32_t i = 0; i < halfN; i++) {
    for (uint32_t j = 0; j < pop[i].weights.size(); j++) {
      Split[0].first = std::bitset<CROSSOVER>(
          pop[i].weightsGen[j].to_string().substr(0, CROSSOVER));
      Split[0].second = std::bitset<GENE_MAX - CROSSOVER>(
          pop[i].weightsGen[j].to_string().substr(CROSSOVER,
                                                  GENE_MAX - CROSSOVER));

      Split[1].first = std::bitset<CROSSOVER>(
          pop[i + 2].weightsGen[j].to_string().substr(0, CROSSOVER));
      Split[1].second = std::bitset<GENE_MAX - CROSSOVER>(
          pop[i + 2].weightsGen[j].to_string().substr(CROSSOVER,
                                                      GENE_MAX - CROSSOVER));

      newPopMembers[0].weightsGen.push_back(
          Genome_t(Split[0].first.to_string() + Split[1].second.to_string()));
      newPopMembers[1].weightsGen.push_back(
          Genome_t(Split[1].first.to_string() + Split[0].second.to_string()));
    }
    newPop.push_back(newPopMembers[0]);
    newPop.push_back(newPopMembers[1]);
    newPopMembers[0].weightsGen.clear();
    newPopMembers[1].weightsGen.clear();
  }
  pop = newPop;
}

void GA::Mutate(Population_t &pop) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);
  std::uniform_real_distribution<float> dis(0, 1);

  std::default_random_engine genGen(seed);
  std::uniform_int_distribution<int> disGen(0, (GENE_MAX - 1));

  QtConcurrent::blockingMap<Population_t>(pop, [&](PopMember_t &P){
    for (uint32_t j = 0; j < P.weightsGen.size(); j++) {
      if (dis(gen) < MutationRate) {
        P.weightsGen[j][disGen(genGen)].flip();
      }
    }
  });
}

void GA::GrowToAdulthood(Population_t &pop, float &totalFitness) {

  QtConcurrent::blockingMap<Population_t>(
      pop, [&](PopMember_t &P) {
        for (uint32_t j = 0; j < P.weightsGen.size(); j++) {
          P.weights.push_back(
              ConvertToValue<float>(P.weightsGen[j], rangeweights));
          }
          Weight_t iWeight(P.weights.begin(),
                           P.weights.begin() +
                               ((inputneurons + 1) * hiddenneurons));
          Weight_t hWeight(P.weights.begin() +
                               ((inputneurons + 1) * hiddenneurons),
                           P.weights.end());

          for (uint32_t j = 0; j < inputValues.size(); j++) {
            Predict_t results =
                NNfuction(inputValues[j], iWeight, hWeight, inputneurons,
                          hiddenneurons, outputneurons);
            // See issue #85
            bool allGood = true;
            bool oneGood = true;
            float fitness = 0.0;
            for (uint32_t k = 0; k < results.OutputNeurons.size(); k++) {
              bool resultSign = std::signbit(results.OutputNeurons[k]);
              bool goalSign = std::signbit(goal[j].OutputNeurons[k]);
              fitness += (resultSign == goalSign) ? 1 : -1;
              if (resultSign != goalSign) {
                if (goalSign == false) {
                  oneGood = false;
                }
                allGood = false;
              }
            }
            fitness += (allGood) ? results.OutputNeurons.size() : 0;
            fitness += (oneGood) ? results.OutputNeurons.size() : 0;
            P.Fitness += fitness;
          }
      });

  for_each(pop.begin(), pop.end(),
           [&](PopMember_t &P) { totalFitness += P.Fitness; });
}

bool GA::SurvivalOfTheFittest(Population_t &pop, float &totalFitness) {
  bool retVal = false;
  uint32_t decimationCount = pop.size() / 2;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);

  std::sort(pop.begin(), pop.end(),
            [](const PopMember_t &L, const PopMember_t &R) {
              return L.Fitness < R.Fitness;
            });

  float maxFitness = pop[pop.size() - 1].Fitness * pop.size();
  uint32_t i = Elitisme;
  while (pop.size() > decimationCount) {
    if (i == pop.size()) {
      i = Elitisme;
    }
    std::uniform_real_distribution<float> dis(0, maxFitness);
    if (dis(gen) > pop[i].Fitness) {
      totalFitness -= pop[i].Fitness;
      pop.erase(pop.begin() + i);
    }
    i++;
  }

  std::sort(pop.begin(), pop.end(),
            [](const PopMember_t &L, const PopMember_t &R) {
              return L.Fitness > R.Fitness;
            });

  float learnError = 1 - ((pop[0].Fitness - minOptim) / (maxOptim - minOptim));

  // Viva la Revolution
  if (currentGeneration > 9) {
    double avg = 0;
    for_each(last10Gen.begin(), last10Gen.end(), [&](double &G) { avg += G; });
    avg /= 10;
    double minMax[2] = {avg * 0.98, avg * 1.02};
    if (learnError > minMax[0] && learnError < minMax[1]) {
      if (!revolutionOngoing) {
        qDebug() << "Viva la revolution!";
        oldElit = Elitisme;
        Elitisme = 0;
        oldMutation = MutationRate;
        MutationRate = 0.25;
        revolutionOngoing = true;
      }
    } else if (revolutionOngoing) {
      qDebug() << "Peace has been restort";
      Elitisme = oldElit;
      MutationRate = oldMutation;
      revolutionOngoing = false;
    }
    last10Gen.pop_front();
    last10Gen.push_back(learnError);
  } else {
    last10Gen.push_back(learnError);
  }
  currentGeneration++;
  emit learnErrorUpdate(static_cast<double>(learnError));
  if (learnError < EndError) {
    retVal = true;
  }
  return retVal;
}
}
