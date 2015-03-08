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

		iNeurons.clear();
		hNeurons.clear();
		oNeurons.clear();

		// Set the bias in the input and hidden vector to 1 (real number)
		iNeurons.push_back(1.0f);
		hNeurons.push_back(1.0f);

		Predict_t retVal;
		uint32_t wCount = 0;

		// Init the network
		for (uint32_t i = 0; i < inputNeurons; i++) { iNeurons.push_back(static_cast<float>(abs(input[i]))); }
		for (uint32_t i = 0; i < hiddenNeurons; i++)	{ hNeurons.push_back(0.0f); }
		for (uint32_t i = 0; i < outputNeurons; i++)	{ oNeurons.push_back(0.0f); }

		for (uint32_t i = 1; i < hNeurons.size(); i++)
		{
			wCount = i - 1;
			for (uint32_t j = 0; j < iNeurons.size(); j++)
			{
				hNeurons[i] += iNeurons[j] * iWeights[wCount];
				wCount += hNeurons.size() - 1;
			}
			hNeurons[i] = 1 / (1 + pow(2.71828f, (-hNeurons[i] * BETA)));
		}

		for (uint32_t i = 0; i < oNeurons.size(); i++)
		{
			wCount = i;
			for (uint32_t j = 0; j < hNeurons.size(); j++)
			{
				oNeurons[i] += hNeurons[j] * hWeights[wCount];
				wCount += oNeurons.size();
			}
			oNeurons[i] = (2 / (1.0f + pow(2.71828f, (-oNeurons[i] * BETA)))) - 1; // Shift plus scale so the learning function can be calculated
		}

		retVal.OutputNeurons = oNeurons;
		return retVal;
	}

	void NN::Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed)
	{
		SoilMath::GA optim(PredictLearn, inputNeurons, hiddenNeurons, outputNeurons);
		ComplexVect_t inputTest;
		std::vector<Weight_t> weights;
		Weight_t weight(((inputNeurons + 1) * hiddenNeurons) + ((hiddenNeurons + 1) * outputNeurons), 0);
		// loop through each case and adjust the weights

		for (uint32_t i = 0; i < input.size(); i++)
		{
			optim.Evolve(input[i], weight, weights, MinMaxWeight_t(-1, 1), cat[i], 10, 50);
			weights.push_back(weight);
		}
		learnedWeights = weights;
		
		this->iWeights = Weight_t(weight.begin(), weight.begin() + ((inputNeurons + 1) * hiddenNeurons));
		this->hWeights = Weight_t(weight.begin() + ((inputNeurons + 1) * hiddenNeurons), weight.end());
	}

}