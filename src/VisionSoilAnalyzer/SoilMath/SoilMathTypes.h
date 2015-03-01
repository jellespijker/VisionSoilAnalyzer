#pragma once

#define GENE_MAX 32
#define CROSSOVER 12
#define MUTATIONRATE 0.08f
#define ELITISME 1
#define END_ERROR 0.03f

#include <stdint.h>
#include <bitset>
#include <vector>
#include <complex>
#include <valarray>
#include <array>

typedef unsigned char uchar;
typedef unsigned short ushort;

typedef std::complex<double> Complex_t;
typedef std::vector<Complex_t> ComplexVect_t;
typedef std::valarray<Complex_t> ComplexArray_t;
typedef std::vector<uint32_t> iContour_t;

typedef std::bitset<GENE_MAX> Genome_t;
typedef std::pair<std::bitset<CROSSOVER>, std::bitset<GENE_MAX - CROSSOVER>> SplitGenome_t;
typedef std::vector<float> Weight_t;
typedef std::vector<Genome_t> GenVect_t;
typedef struct PopMemberStruct
{
	Weight_t weights;
	GenVect_t weightsGen;
	float Calculated = 0.0;
	float Fitness = 0.0;
} PopMember_t;
typedef std::vector<PopMember_t> Population_t;
typedef std::pair<float, float> MinMaxWeight_t;

typedef std::vector<ComplexVect_t> InputLearnVector_t;
typedef std::vector<uint32_t> OutputLearnVector_t;

typedef struct Predict_struct
{
	uint32_t Category;
	float RealValue;
	float Accuracy;
	std::vector<float> OutputNeurons;
} Predict_t;
typedef Predict_t(*NNfunctionType)(ComplexVect_t, Weight_t, Weight_t, uint32_t, uint32_t, uint32_t);
