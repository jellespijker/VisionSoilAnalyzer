#include "GA.h"

namespace SoilMath
{
	GA::GA()
	{
	}


	GA::~GA()
	{
	}

	void GA::Evolve(const ComplexVect_t &inputValues, Weight_t &weights, const MinMaxWeight_t &rangeweights, uint32_t maxGenerations)
	{
		if (rangeweights.size() != weights.size()) { throw Exception::MathException("weights and range weights unequal vector Exception!"); }

		// Create the population
		Population_t pop = Genesis(weights, rangeweights);

		for (uint32_t i = 0; i < maxGenerations; i++)
		{
			CrossOver(pop);
			Mutate(pop);
			GrowToAdulthood(pop);
			LetsGetItOn(pop);
			TheMiricalOfLife(pop);
			if (SurvivalOfTheFittest(pop)) { break; }
		}

		// Convert winning population Member to weights
	}

	Population_t GA::Genesis(const Weight_t &weights, const MinMaxWeight_t &rangeweights)
	{
		//set up the random engines
		default_random_engine re;

		Population_t pop;
		for (uint32_t i = 0; i < weights.size(); i++)
		{
			uniform_real_distribution<double> unif(0, 1);
			re.seed(unsigned long(time(0)));

		}
	}
}