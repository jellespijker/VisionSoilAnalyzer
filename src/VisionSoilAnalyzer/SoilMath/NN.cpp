#include "NN.h"

namespace SoilMath
{
	NN::NN(uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons)
	{
		// Set the number of neurons in the network
		inputNeurons = inputneurons;
		hiddenNeurons = hiddenneurons;
		outputNeurons = outputneurons;

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

		Predict_t retVal;

		uint32_t totInputNeurons = inputNeurons + 1;
		uint32_t totHiddenNeurons = hiddenNeurons + 1;
		uint32_t wCount = 0;

		// Init the network
		for (uint32_t i = 1; i < totInputNeurons; i++) { iNeurons.push_back(static_cast<float>(abs(input[i])));	}
		for (uint32_t i = 1; i < totHiddenNeurons; i++)	{ hNeurons.push_back(0.0f);	}
		for (uint32_t i = 0; i < outputNeurons; i++)	{ oNeurons.push_back(0.0f); }

		for (uint32_t i = 0; i < iNeurons.size(); i++)
		{
			for (uint32_t j = 1; j < hNeurons.size(); j++)
			{
				hNeurons[j] += iNeurons[i] * iWeights[wCount++];
			}
		}

		for (uint32_t i = 0; i < hNeurons.size(); i++)
		{
			hNeurons[i] = 1 / (1 + pow(2.71828f, (-hNeurons[i] * BETA)));
		}

		wCount = 0;
		for (uint32_t i = 1; i < hNeurons.size(); i++)
		{
			for (uint32_t j = 1; j < oNeurons.size(); j++)
			{
				oNeurons[i] += hNeurons[j] * hWeights[wCount++];
			}
		}

		retVal.RealValue = oNeurons[0];
		retVal.OutputNeurons = oNeurons;
		return retVal;
	}

	void NN::Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed)
	{
		SoilMath::GA optim(PredictLearn, inputNeurons, hiddenNeurons, outputNeurons);
		ComplexVect_t inputTest;
		std::vector<Weight_t> weights;
		// loop through each case and adjust the weights
		for (uint32_t i = 0; i < input.size(); i++)
		{
			Weight_t weight(((inputNeurons + 1) * hiddenNeurons) + ((hiddenNeurons + 1) * outputNeurons), 0);
			optim.Evolve(input[i], weight, MinMaxWeight_t(0, 2), cat[i]);
			weights.push_back(weight);
		}
		learnedWeights = weights;
	}

}