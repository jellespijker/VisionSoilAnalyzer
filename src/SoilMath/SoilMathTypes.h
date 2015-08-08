/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#define GENE_MAX 32         /**< maximum number of genes*/
#define CROSSOVER 16        /**< crossover location*/
#define MUTATIONRATE 0.075f /**< mutation rate*/
#define ELITISME 4          /**< total number of the elite bastard*/
#define END_ERROR 0.005f    /**< acceptable error between last itteration*/

#include <stdint.h>
#include <bitset>
#include <vector>
#include <complex>
#include <valarray>
#include <array>

typedef unsigned char uchar;   /**< unsigned char*/
typedef unsigned short ushort; /**< unsigned short*/
typedef unsigned int uint32_t;

typedef std::complex<double> Complex_t;       /**< complex vector of doubles*/
typedef std::vector<Complex_t> ComplexVect_t; /**< vector of Complex_t*/
typedef std::valarray<Complex_t> ComplexArray_t; /**< valarray of Complex_t*/
typedef std::vector<uint32_t> iContour_t;        /**< vector of uint32_t*/

typedef std::bitset<GENE_MAX> Genome_t; /**< Bitset repressenting a genome*/
typedef std::pair<std::bitset<CROSSOVER>, std::bitset<GENE_MAX - CROSSOVER>>
    SplitGenome_t;                       /**< a matted genome*/
typedef std::vector<float> Weight_t;     /**< a float vector*/
typedef std::vector<Genome_t> GenVect_t; /**< a vector of genomes*/
typedef struct PopMemberStruct {
  Weight_t weights;       /**< the weights the core of a population member*/
  GenVect_t weightsGen;   /**< the weights as genomes*/
  float Calculated = 0.0; /**< the calculated value*/
  float Fitness = 0.0;    /**< the fitness of the population member*/
} PopMember_t;            /**< a population member*/
typedef std::vector<PopMember_t> Population_t; /**< Vector with PopMember_t*/
typedef std::pair<float, float>
    MinMaxWeight_t; /**< floating pair weight range*/

typedef struct Predict_struct {
  uint8_t Category; /**< the category number */
  float RealValue;   /**< category number as float in order to estimate how
                        precise to outcome is*/
  float Accuracy;    /**< the accuracy of the category*/
  std::vector<float> OutputNeurons; /**< the output Neurons*/
} Predict_t;                        /**< The prediction results*/
typedef Predict_t (*NNfunctionType)(
    ComplexVect_t, Weight_t, Weight_t, uint32_t, uint32_t,
    uint32_t); /**< The prediction function from the Neural Net*/

typedef std::vector<ComplexVect_t>
    InputLearnVector_t; /**< Vector of a vector with complex values*/
typedef std::vector<Predict_t> OutputLearnVector_t; /**< vector with results*/
