#pragma once

#define BETA 1

#include <stdint.h>
#include <vector>
#include <string>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "GA.h"
#include "MathException.h"
#include "SoilMathTypes.h"
#include "FFT.h"

namespace SoilMath
{
	class NN
	{
	public:
		NN(uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons);
		~NN();

		Predict_t Predict(ComplexVect_t input);
		static Predict_t PredictLearn(ComplexVect_t input, Weight_t inputweights, Weight_t hiddenweights, uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons);
		void SetInputWeights(Weight_t value) { iWeights = value; };
		void SetHiddenWeights(Weight_t value) { hWeights = value; };

		void Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed );

		void SaveState(string filename);
		void LoadState(string filename);

		std::vector<Weight_t> learnedWeights;

	private:
		std::vector<float> iNeurons;
		Weight_t iWeights;

		std::vector<float> hNeurons;
		Weight_t hWeights;

		std::vector<float> oNeurons;

		uint32_t hiddenNeurons;
		uint32_t inputNeurons;
		uint32_t outputNeurons;

		bool studied = false;

	};
}