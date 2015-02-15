#include "GA.h"

namespace SoilMath
{
	GA::GA() { } 
	
	GA::~GA() { }

	void GA::Evolve(const ComplexVect_t &inputValues, Weight_t &weights, MinMaxWeight_t rangeweights, float goal, uint32_t maxGenerations, uint32_t popSize)
	{
		// Create the population
		Population_t pop = Genesis(weights, rangeweights, popSize);
		float totalFitness = 0.0;
		for (uint32_t i = 0; i < maxGenerations; i++)
		{
			CrossOver(pop);
			Mutate(pop);
			totalFitness = 0.0;
			GrowToAdulthood(pop, inputValues,rangeweights, goal, totalFitness);
			if (SurvivalOfTheFittest(pop, totalFitness)) { break; }
		}

		weights = pop[0].weights;
	}

	Population_t GA::Genesis(const Weight_t &weights, MinMaxWeight_t rangeweights, uint32_t popSize)
	{
		Population_t pop;
		std::default_random_engine gen;
		std::uniform_real_distribution<float> dis(rangeweights.first, rangeweights.second);

		for (uint32_t i = 0; i < popSize; i++)
		{
			PopMember_t I;
			for (uint32_t j = 0; j < weights.size(); j++)
			{
				I.weights.push_back(dis(gen));
				I.weightsGen.push_back(ConvertToGenome<float>(I.weights[j], rangeweights));
			}
			pop.push_back(I);
		}
		return pop;
	}

	void GA::CrossOver(Population_t &pop)
	{
		Population_t newPop; // create a new population

		SplitGenome_t Split[2];
		PopMember_t newPopMembers[2];

		for (uint32_t i = 0; i < pop.size(); i += 2)
		{
			for (uint32_t j = 0; j < pop[i].weights.size(); j += 2)
			{
				Split[0].first = bitset<CROSSOVER>(pop[i].weightsGen[j].to_string().substr(0, CROSSOVER));
				Split[0].second = bitset<GENE_MAX - CROSSOVER>(pop[i].weightsGen[j].to_string().substr(CROSSOVER, GENE_MAX - CROSSOVER));

				Split[1].first = bitset<CROSSOVER>(pop[i + 1].weightsGen[j].to_string().substr(0, CROSSOVER));
				Split[1].second = bitset<GENE_MAX - CROSSOVER>(pop[i + 1].weightsGen[j].to_string().substr(CROSSOVER, GENE_MAX - CROSSOVER));

				newPopMembers[0].weightsGen.push_back(Genome_t(Split[0].first.to_string() + Split[1].second.to_string()));
				newPopMembers[1].weightsGen.push_back(Genome_t(Split[1].first.to_string() + Split[0].second.to_string()));
			}
			newPop.push_back(newPopMembers[0]);
			newPop.push_back(newPopMembers[1]);
		}

		//Allow the top tiers population partners to mate again
		uint32_t halfN = pop.size() / 2;
		for (uint32_t i = 0; i < halfN; i++)
		{
			for (uint32_t j = 0; j < pop[i].weights.size(); j += 2)
			{
				Split[0].first = bitset<CROSSOVER>(pop[i].weightsGen[j].to_string().substr(0, CROSSOVER));
				Split[0].second = bitset<GENE_MAX - CROSSOVER>(pop[i].weightsGen[j].to_string().substr(CROSSOVER, GENE_MAX - CROSSOVER));

				Split[1].first = bitset<CROSSOVER>(pop[i + 2].weightsGen[j].to_string().substr(0, CROSSOVER));
				Split[1].second = bitset<GENE_MAX - CROSSOVER>(pop[i + 2].weightsGen[j].to_string().substr(CROSSOVER, GENE_MAX - CROSSOVER));

				newPopMembers[0].weightsGen.push_back(Genome_t(Split[0].first.to_string() + Split[1].second.to_string()));
				newPopMembers[1].weightsGen.push_back(Genome_t(Split[1].first.to_string() + Split[0].second.to_string()));
			}
			newPop.push_back(newPopMembers[0]);
			newPop.push_back(newPopMembers[1]);
		}
		pop = newPop;
	}

	void GA::Mutate(Population_t &pop)
	{
		std::default_random_engine gen;
		std::uniform_real_distribution<float> dis(0, 1);

		std::default_random_engine genGen;
		std::uniform_int_distribution<int> disGen(0, (GENE_MAX - 1));

		uint32_t chance = MUTATIONRATE * GENE_MAX;
		for (uint32_t i = 0; i < pop.size(); i++)
		{
			for (uint32_t j = 0; j < pop[i].weightsGen.size(); j++)
			{
				if (dis(gen) < chance) { pop[i].weightsGen[j][disGen(genGen)].flip(); }
			}
		}
	}

	void GA::GrowToAdulthood(Population_t &pop, const ComplexVect_t &inputValues, MinMaxWeight_t rangeweights, float goal, float &totalFitness)
	{
		for (uint32_t i = 0; i < pop.size(); i++)
		{
			for (uint32_t j = 0; j < pop[i].weightsGen.size(); j++)
			{
				pop[i].weights.push_back(ConvertToValue<float>(pop[i].weightsGen[j], rangeweights));
			}
			pop[i].Calculated = NNfuction(inputValues, pop[i].weights).RealValue;
			pop[i].Fitness = fabsf(1.0 - (pop[i].Calculated / goal));
			totalFitness += pop[i].Fitness;
		}
	}

	bool GA::SurvivalOfTheFittest(Population_t &pop, float &totalFitness)
	{
		bool retVal = false;
		uint32_t decimationCount = pop.size() / 2;
		std::default_random_engine gen;
		std::uniform_real_distribution<float> dis(0, totalFitness);

		uint32_t i = 0;
		while (pop.size() > decimationCount)
		{
			if (i >= pop.size()) { i = 0; }
			if (dis(gen) < pop[i].Fitness) { pop.erase(pop.begin() + i); }
			i++;
		}

		std::sort(pop.begin(), pop.end(), PopMemberSort);

		if (pop[0].Fitness < END_ERROR) { retVal = true; }
		return retVal;
	}
}