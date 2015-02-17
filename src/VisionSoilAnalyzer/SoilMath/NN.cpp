#include "NN.h"

namespace SoilMath
{
	NN::NN()
	{
	}


	NN::~NN()
	{
	}

	Predict_t NN::Predict(ComplexVect_t input, Weight_t weights)
	{
		Predict_t retVal;
		retVal.RealValue = -abs(sin(weights[0])*cos(weights[1])*exp(abs(1 - (sqrt(pow(weights[0], 2) + pow(weights[1], 2)) / M_PI))));
		return retVal;
	}

	void NN::Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed)
	{
		SoilMath::GA Test(Predict);
		ComplexVect_t inputTest;

		weights.push_back(0);
		weights.push_back(0);

		Test.Evolve(inputTest, weights, MinMaxWeight_t(-10.0f, 10.0f), -19.2085f);
	}

}