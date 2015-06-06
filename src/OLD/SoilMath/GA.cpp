#include "GA.h"

namespace SoilMath
{
	GA::GA() { } 

	GA::GA(NNfunctionType nnfunction, uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons)
	{
		this->NNfuction = nnfunction;
		this->inputneurons = inputneurons;
		this->hiddenneurons = hiddenneurons;
		this->outputneurons = outputneurons;
	}

	
	GA::~GA() { }

	void GA::Evolve(const ComplexVect_t &inputValues, Weight_t &weights, std::vector<Weight_t> &prevWeights, MinMaxWeight_t rangeweights, Predict_t goal, uint32_t maxGenerations, uint32_t popSize)
	{
		// Create the population
		uint32_t NOprevPopUsed = prevWeights.size() < popSize ? prevWeights.size() : popSize;
		Population_t pop = Genesis(weights, rangeweights, popSize - NOprevPopUsed);
		for (uint32_t i = 0; i < NOprevPopUsed; i++)
		{
			PopMember_t newMember;
			newMember.weights = prevWeights[i];
			for (uint32_t j = 0; j < newMember.weights.size(); j++) { newMember.weightsGen.push_back(ConvertToGenome<float>(newMember.weights[j], rangeweights)); }
			pop.push_back(newMember);
		}
		float totalFitness = 0.0;
		for (uint32_t i = 0; i < maxGenerations; i++)
		{
			CrossOver(pop);
			Mutate(pop);
			totalFitness = 0.0;
			GrowToAdulthood(pop, inputValues, rangeweights, goal, totalFitness);
			if (SurvivalOfTheFittest(pop, totalFitness)) { break; }
		}

		weights = pop[0].weights;

	}

	void GA::Evolve(const InputLearnVector_t &inputValues, Weight_t &weights, MinMaxWeight_t rangeweights, OutputLearnVector_t &goal, uint32_t maxGenerations, uint32_t popSize)
	{
		// Create the population
		Population_t pop = Genesis(weights, rangeweights, popSize);
		float totalFitness = 0.0;
		for (uint32_t i = 0; i < maxGenerations; i++)
		{
			CrossOver(pop);
			Mutate(pop);
			totalFitness = 0.0;
			GrowToAdulthood(pop, inputValues, rangeweights, goal, totalFitness);
			if (SurvivalOfTheFittest(pop, totalFitness)) { break; }
		}
		weights = pop[0].weights;
	}



	Population_t GA::Genesis(const Weight_t &weights, MinMaxWeight_t rangeweights, uint32_t popSize)
	{
		if (popSize < 1) return Population_t();

		Population_t pop;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine gen(seed);
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
		PopMember_t newPopMembers[2];
		SplitGenome_t Split[2];

		for (uint32_t i = 0; i < pop.size(); i += 2)
		{

			for (uint32_t j = 0; j < pop[i].weights.size(); j++)
			{
				// Split A
				Split[0].first = bitset<CROSSOVER>(pop[i].weightsGen[j].to_string().substr(0, CROSSOVER));
				Split[0].second = bitset<GENE_MAX - CROSSOVER>(pop[i].weightsGen[j].to_string().substr(CROSSOVER, GENE_MAX - CROSSOVER));

				// Split B
				Split[1].first = bitset<CROSSOVER>(pop[i + 1].weightsGen[j].to_string().substr(0, CROSSOVER));
				Split[1].second = bitset<GENE_MAX - CROSSOVER>(pop[i + 1].weightsGen[j].to_string().substr(CROSSOVER, GENE_MAX - CROSSOVER));

				// Mate A and B to AB and BA
				newPopMembers[0].weightsGen.push_back(Genome_t(Split[0].first.to_string() + Split[1].second.to_string()));
				newPopMembers[1].weightsGen.push_back(Genome_t(Split[1].first.to_string() + Split[0].second.to_string()));
			}
			newPop.push_back(newPopMembers[0]);
			newPop.push_back(newPopMembers[1]);
			newPopMembers[0].weightsGen.clear();
			newPopMembers[1].weightsGen.clear();
		}

		//Allow the top tiers population partners to mate again
		uint32_t halfN = pop.size() / 2;
		for (uint32_t i = 0; i < halfN; i++)
		{
			for (uint32_t j = 0; j < pop[i].weights.size(); j++)
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
			newPopMembers[0].weightsGen.clear();
			newPopMembers[1].weightsGen.clear();
		}
		pop = newPop;
	}

	void GA::Mutate(Population_t &pop)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine gen(seed);
		std::uniform_real_distribution<float> dis(0, 1);

		std::default_random_engine genGen(seed);
		std::uniform_int_distribution<int> disGen(0, (GENE_MAX - 1));

		for (uint32_t i = 0; i < pop.size(); i++)
		{
			for (uint32_t j = 0; j < pop[i].weightsGen.size(); j++) { if (dis(gen) < MUTATIONRATE) { pop[i].weightsGen[j][disGen(genGen)].flip(); }	}
		}
	}

	void GA::GrowToAdulthood(Population_t &pop, const ComplexVect_t &inputValues, MinMaxWeight_t rangeweights, Predict_t goal, float &totalFitness)
	{
		for (uint32_t i = 0; i < pop.size(); i++)
		{
			for (uint32_t j = 0; j < pop[i].weightsGen.size(); j++)	{ pop[i].weights.push_back(ConvertToValue<float>(pop[i].weightsGen[j], rangeweights));	}
			Weight_t iWeight(pop[i].weights.begin(), pop[i].weights.begin() + ((inputneurons + 1) * hiddenneurons));
			Weight_t hWeight(pop[i].weights.begin() + ((inputneurons + 1) * hiddenneurons), pop[i].weights.end());
			Predict_t results = NNfuction(inputValues, iWeight, hWeight, inputneurons, hiddenneurons, outputneurons);
			for (uint32_t j = 0; j < results.OutputNeurons.size(); j++)
			{
				pop[i].Fitness -= results.OutputNeurons[j] / goal.OutputNeurons[j];
			}
			pop[i].Fitness += results.OutputNeurons.size();
			totalFitness += pop[i].Fitness;
		}
	}

	void GA::GrowToAdulthood(Population_t &pop, const InputLearnVector_t &inputValues, MinMaxWeight_t rangeweights, OutputLearnVector_t &goal, float &totalFitness)
	{
		for (uint32_t i = 0; i < pop.size(); i++)
		{
			for (uint32_t j = 0; j < pop[i].weightsGen.size(); j++)	{ pop[i].weights.push_back(ConvertToValue<float>(pop[i].weightsGen[j], rangeweights)); }
			Weight_t iWeight(pop[i].weights.begin(), pop[i].weights.begin() + ((inputneurons + 1) * hiddenneurons));
			Weight_t hWeight(pop[i].weights.begin() + ((inputneurons + 1) * hiddenneurons), pop[i].weights.end());
			for (uint32_t j = 0; j < inputValues.size(); j++)
			{
				Predict_t results = NNfuction(inputValues[j], iWeight, hWeight, inputneurons, hiddenneurons, outputneurons);
				for (uint32_t k = 0; k < results.OutputNeurons.size(); k++)
				{
					pop[i].Fitness -= results.OutputNeurons[k] / goal[j].OutputNeurons[k];
				}
				pop[i].Fitness += results.OutputNeurons.size();
			}
			pop[i].Fitness /= inputValues.size();
			totalFitness += pop[i].Fitness;
		}
	}


	bool GA::SurvivalOfTheFittest(Population_t &pop, float &totalFitness)
	{
		bool retVal = false;
		uint32_t decimationCount = pop.size() / 2;

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine gen(seed);

		std::sort(pop.begin(), pop.end(), PopMemberSort);

		uint32_t i = ELITISME;
		while (pop.size() > decimationCount)
		{
			if (i >= pop.size()) { i = ELITISME; }
			std::uniform_real_distribution<float> dis(0, totalFitness);
			if (dis(gen) < pop[i].Fitness) 
			{
				pop.erase(pop.begin() + i--); 
				totalFitness -= pop[i].Fitness;
			}
			i++;
		}

		if (pop[0].Fitness < END_ERROR) { retVal = true; }
		return retVal;
	}
}