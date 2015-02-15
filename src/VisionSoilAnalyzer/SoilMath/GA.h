#pragma once
#define GENE_MAX 32
#define CROSSOVER 12
#define MUTATIONRATE 0.08f
#define ELITISME 1
#define END_ERROR 0.01f

#include <bitset>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>

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

		void Evolve(const ComplexVect_t &inputValues, Weight_t &weights, MinMaxWeight_t rangeweights, float goal, uint32_t maxGenerations = 200, uint32_t popSize = 30);

	private:
		NNfunctionType NNfuction;

		Population_t Genesis(const Weight_t &weights, MinMaxWeight_t rangeweights, uint32_t popSize);
		void CrossOver(Population_t &pop);
		void Mutate(Population_t &pop);
		void GrowToAdulthood(Population_t &pop, const ComplexVect_t &inputValues, MinMaxWeight_t rangeweights, float goal, float &totalFitness);
		bool SurvivalOfTheFittest(Population_t &pop, float &totalFitness);

		static bool PopMemberSort(PopMember_t i, PopMember_t j) { return (i.Fitness < j.Fitness); }

		template <typename T>
		inline Genome_t ConvertToGenome(T value, std::pair<T, T> range)
		{
			uint32_t intVal = static_cast<uint32_t>((UINT32_MAX * (range.first + value)) / (range.second - range.first));
			Genome_t retVal(intVal);
			return retVal;
		};

		template <typename T>
		inline T ConvertToValue(Genome_t gen, std::pair<T, T> range)
		{
			T retVal = range.first + (((range.second - range.first) * static_cast<T>(gen.to_ulong())) / UINT32_MAX);
			//if (retVal < range.first) { retVal = range.first; }
			//else if (retVal > range.second) { retVal = range.second; }
			return retVal;
		};
	};

}