#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <fstream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>

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
		NN();
		virtual ~NN();

		Predict_t Predict(ComplexVect_t input);
		static Predict_t PredictLearn(ComplexVect_t input, Weight_t inputweights, Weight_t hiddenweights, uint32_t inputneurons, uint32_t hiddenneurons, uint32_t outputneurons);
		void SetInputWeights(Weight_t value) { iWeights = value; };
		void SetHiddenWeights(Weight_t value) { hWeights = value; };
		void SetBeta(float value) { beta = value; };

		void Learn(InputLearnVector_t input, OutputLearnVector_t cat, uint32_t noOfDescriptorsUsed );

		void SaveState(string filename);
		void LoadState(string filename);

	private:
		std::vector<float> iNeurons;
		Weight_t iWeights;

		std::vector<float> hNeurons;
		Weight_t hWeights;

		std::vector<float> oNeurons;

		uint32_t hiddenNeurons;
		uint32_t inputNeurons;
		uint32_t outputNeurons;
		float beta;

		bool studied = false;
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(inputNeurons);
			ar & BOOST_SERIALIZATION_NVP(hiddenNeurons);
			ar & BOOST_SERIALIZATION_NVP(outputNeurons);
			ar & BOOST_SERIALIZATION_NVP(iWeights);
			ar & BOOST_SERIALIZATION_NVP(hWeights);
			ar & BOOST_SERIALIZATION_NVP(beta);
			ar & BOOST_SERIALIZATION_NVP(studied);
		}

	};
}