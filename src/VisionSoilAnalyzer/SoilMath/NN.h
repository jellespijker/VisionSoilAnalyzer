#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "GA.h"
#include "MathException.h"
#include "SoilMathTypes.h"
#include "FFT.h"

namespace SoilMath
{
	class NN
	{
	public:
		NN();
		~NN();

		static Predict_t Predict(ComplexVect_t input, Weight_t weights);

		void Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed );
		void SaveState(string filename);
		void LoadState(string filename);

		Predict_t Process(ComplexVect_t input, uint32_t noOfDescriptorsUsed);

		Weight_t weights;


	private:
		float *inputWeights;
		float *outputWeights;
		float *hiddenWeights;
		float *biasWeights;

		uint32_t hiddenNeurons;
		uint32_t inputNeurons;
		uint32_t outputNeurons;

		bool studied = false;
	};
}