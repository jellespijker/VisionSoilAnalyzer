#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "MathException.h"
#include "SoilMathTypes.h"
#include "FFT.h"

typedef std::vector<ComplexVect_t> InputLearnVector_t;
typedef std::vector<uint32_t> OutputLearnVector_t;

typedef struct Predict_struct
{
	uint32_t Category;
	float Accuracy;
} Predict_t;

namespace SoilMath
{
	class NN
	{
	public:
		NN();
		~NN();

		void Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed );
		void SaveState(string filename);
		void LoadState(string filename);

		Predict_t Process(ComplexVect_t input, uint32_t noOfDescriptorsUsed);

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