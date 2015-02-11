#pragma once
#define GENE_MAX 32

#include <bitset>
#include <random>

#include "NN.h"
#include "MathException.h"

typedef std::bitset<GENE_MAX> Genome_t;
typedef struct PopMemberStruct
{
	Complex_t Value;
	pair<float, float> Range;
	Genome_t RealGen;
	Genome_t ImagGen;
} PopMember_t;
typedef std::vector<PopMember_t> Population_t;
typedef std::vector<float> Weight_t;
typedef std::vector<pair<float, float>> MinMaxWeight_t;
typedef Predict_t(*NNfunctionType)(ComplexVect_t, Weight_t); 

namespace SoilMath
{

	class GA
	{
	public:
		GA();
		GA(NNfunctionType nnfunction);
		~GA();

		void Evolve(const ComplexVect_t &inputValues, Weight_t &weights, const MinMaxWeight_t &rangeweights, uint32_t maxGenerations = 100);
		void Evolve(NNfunctionType nnfunction, ComplexVect_t inputValues, Weight_t weights, uint32_t maxGenerations = 100);

	private:
		NNfunctionType NNfuction;

		Population_t Genesis(const Weight_t &weights, const MinMaxWeight_t &rangeweights);
		void CrossOver(Population_t &pop);
		void Mutate(Population_t &pop);
		void GrowToAdulthood(Population_t &pop);
		void LetsGetItOn(Population_t &pop);
		void TheMiricalOfLife(Population_t &pop);
		bool SurvivalOfTheFittest(Population_t &pop);
	};

}