#include "NN.h"

namespace SoilMath
{
	NN::NN(uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons)
	{
		// Set the number of neurons in the network
		inputNeurons = inputneurons;
		hiddenNeurons = hiddenneurons;
		outputNeurons = outputNeurons;

		// Reserve the vector space
		iNeurons.reserve(inputNeurons + 1); // input neurons + bias
		hNeurons.reserve(hiddenNeurons + 1); // hidden neurons + bias
		oNeurons.reserve(outputNeurons); // output neurons

		// Set the bias in the input and hidden vector to 1 (real number)
		iNeurons.push_back(1.0f);
		hNeurons.push_back(1.0f);
	}


	NN::~NN()
	{
	}

	void NN::LoadState(string filename)
	{

	}

	void NN::SaveState(string filename)
	{

	}


	Predict_t NN::PredictLearn(ComplexVect_t input, Weight_t inputweights, Weight_t hiddenweights, uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons)
	{
		NN neural(inputneurons, hiddenneurons, outputneurons);
		neural.SetInputWeights(inputweights);
		neural.SetHiddenWeights(hiddenweights);
		return neural.Predict(input);
	}

	Predict_t NN::Predict(ComplexVect_t input)
	{
		if (input.size() != inputNeurons) { throw Exception::MathException("Size of input Neurons Exception!"); }
		uint32_t totInputNeurons = inputNeurons + 1;

		iNeurons.push_back(1.0f);
		for (uint32_t i = 1; i < totInputNeurons; i++) { iNeurons.push_back(static_cast<float>(abs(input[i])));	}

		for (uint32_t i = 1; i < hiddenNeurons; i++)
		{
			for (uint32_t j = 0; j < iNeurons.size(); j++)
			{
				hNeurons[i] += iNeurons[j] * iWeights[j];
			}
			hNeurons[i] = 1 / (1 + pow(2.71828f, (-hNeurons[i] * BETA)));
		}

		for (uint32_t i = 0; i < outputNeurons; i++)
		{
			for (uint32_t j = 0; j < hNeurons.size(); j++)
			{
				oNeurons[i] += hNeurons[j] * iWeights[j];
			}
			oNeurons[i] = 1 / (1 + pow(2.71828f, (-oNeurons[i] * BETA)));
		}

		Predict_t retVal;
		retVal.OutputNeurons = oNeurons;
		return retVal;
	}

	void NN::Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed)
	{
		SoilMath::GA optim(PredictLearn);
		ComplexVect_t inputTest;
		std::vector<Weight_t> weights;
		// loop through each case and adjust the weights
		for (uint32_t i = 0; i < input.size(); i++)
		{
			Weight_t weight(inputNeurons + hiddenNeurons + 2, 0);
			optim.Evolve(input[i], weight, MinMaxWeight_t(0, 10), cat[i]);
			weights.push_back(weight);
		}

		//Test.Evolve(inputTest, weights, MinMaxWeight_t(-10.0f, 10.0f), -19.2085f);
	}

}