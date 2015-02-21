#include "NN.h"

namespace SoilMath
{
	NN::NN()
	{
		hNeurons.reserve(hiddenNeurons + 1);
		oNeurons.reserve(outputNeurons + 1);

		iNeurons.push_back(Complex_t(1.0, 0.0));
		hNeurons.push_back(Complex_t(1.0, 0.0));
	}


	NN::~NN()
	{
	}

	Predict_struct NN::PredictLearn(NN neuralnet)
	{
		//Predict_t retVal;
		//retVal.RealValue = -abs(sin(weights[0])*cos(weights[1])*exp(abs(1 - (sqrt(pow(weights[0], 2) + pow(weights[1], 2)) / M_PI))));
		//return retVal;
	}


	Predict_t NN::Predict(ComplexVect_t input)
	{
		iNeurons = input;
		Complex_t sigmoid;

		for (uint32_t i = 1; i < hiddenNeurons; i++)
		{
			for (uint32_t j = 0; j < iNeurons.size(); j++)
			{
				hNeurons[i] += iNeurons[j] * iWeights[j];
			}

			sigmoid = 1 / (1 + pow(2.71828f, (- hNeurons[i] / Complex_t(BETA, BETA))));
			if (std::abs(sigmoid) > 0.95) { hNeurons[i] = Complex_t(1, 0); }
			else { hNeurons = Complex_t(0, 0); }
		}

		for (uint32_t i = 0; i < outputNeurons; i++)
		{
			for (uint32_t j = 0; j < hNeurons.size(); j++)
			{
				oNeurons[i] += hNeurons[j] * iWeights[j];
			}
			sigmoid = 1 / (1 + pow(2.71828f, (-oNeurons[i] / Complex_t(BETA, BETA))));
			if (std::abs(sigmoid) > 0.95) { oNeurons[i] = Complex_t(1, 0); }
			else { oNeurons = Complex_t(0, 0); }
		}


		
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