#pragma once
#define GENE_MAX 32
#define CROSSOVER 16
#define MUTATIONRATE 0.01f
#define END_ERROR 0.001f

#include <bitset>
#include <random>
#include <string>
#include <algorithm>

#include "NN.h"
#include "MathException.h"

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
typedef Predict_t(*NNfunctionType)(ComplexVect_t, Weight_t); 

namespace SoilMath
{

	class GA
	{
	public:
		GA();
		GA(NNfunctionType nnfunction);
		~GA();

		void Evolve(const ComplexVect_t &inputValues, Weight_t &weights, MinMaxWeight_t rangeweights, float goal, uint32_t maxGenerations = 100, uint32_t popSize = 20);

	private:
		NNfunctionType NNfuction;

		Population_t Genesis(const Weight_t &weights, MinMaxWeight_t rangeweights, uint32_t popSize);
		void CrossOver(Population_t &pop);
		void Mutate(Population_t &pop);
		void GrowToAdulthood(Population_t &pop, const ComplexVect_t &inputValues, MinMaxWeight_t rangeweights, float goal, float &totalFitness);
		bool SurvivalOfTheFittest(Population_t &pop, float &totalFitness);

		bool PopMemberSort(PopMember_t i, PopMember_t j) { return (i.Fitness < j.Fitness); }

		template <typename T>
		inline Genome_t ConvertToGenome(T value, std::pair<T, T> range)
		{
			return Genome_t(static_cast<uint32_t>(((value + range.first) / range.second) * UINT32_MAX));
		};

		template <typename T>
		inline T ConvertToValue(Genome_t gen, std::pair<T, T> range)
		{
			return static_cast<T>(gen.to_ulong() / UINT32_MAX) - range.first);
		};
	};

}