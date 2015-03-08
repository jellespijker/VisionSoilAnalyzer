#include <iostream>
#include <sys/time.h>
#include <array>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <cmath>
#include <random>


#include "TestMatrix.h"
#include "FloatTestMatrix.h"
#include "../../src/VisionSoilAnalyzer/SoilMath/SoilMath.h"


using namespace std;

void DisplayHelp()
{
	cout << "--Stats-float         Test floating type stats" << endl;
	cout << "--Stats-uchar         Test uchar type stats" << endl;
	cout << "--FFT                 Test Fast Fourier Transform of an Edge" << endl;
	cout << "--GA                  Test Genetic ALgorithm" << endl;
	cout << "--NN                  Test Neural Network" << endl;
	cout << "--NNPredict           Test Neural Network Predictionfunction" << endl;
}

bool checkArray(uint32_t *a, uint32_t *b)
{
	bool retVal = true;
	uint32_t i = 256;
	uint32_t c = 0;
	while (i-- > 0)	
	{ 
		cout << i << " = Calculated : " << (int)*a << " - Expected : " << (int)*b << endl;
		if (*a++ != *b++) 
		{
			retVal = false;
		} 
		c++;
	}
	return true;
}

bool checkfloatArray(uint32_t *a, uint32_t *b)
{
	bool retVal = true;
	uint32_t i = 256;
	uint32_t c = 0;
	while (i-- > 0)
	{
		cout << "Calculated : " << *a << " - Expected : " << *b << endl;
		if (*a++ != *b++)
		{
			retVal = false;
		}
		c++;
	}
	return true;
}


void TestUcharStat()
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	clock_gettime(CLOCK_REALTIME, &tstart);
	ucharStat_t Test((uint8_t *)testMatrix, 200, 200);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = tstop.tv_nsec - tstart.tv_nsec;

	uint8_t *p_histRes = (uint8_t *)histTestResult;

	cout << "------------------------------- Basic Calculations ----------------------------------" << endl;
	if (!checkArray((uint32_t *)Test.bins, (uint32_t *)p_histRes)) { cout << "Histograms are unequal!" << endl; }
	else { cout << "Histograms are equal!" << endl; }
	cout << "Mean Test | Expected Value : " << meanTestResult << " Calc value : " << Test.Mean << endl;
	cout << "n Test | Expected Value : " << nTestResult << " Calc value : " << Test.n << endl;
	cout << "Sum Test | Expected Value : " << sumTestResult << " Calc value : " << Test.Sum << endl;
	cout << "Min Test | Expected Value : " << +minTestResult << " Calc value : " << +Test.min << endl;
	cout << "Max Test | Expected Value : " << +maxTestResult << " Calc value : " << +Test.max << endl;
	cout << "Range Test | Expected Value : " << +rangeTestResult << " Calc value : " << +Test.Range << endl;
	cout << "Standard Deviation Test | Expected Value : " << stdTestResult << " Calc value : " << Test.Std << endl;
	cout << "Average Execution time was : " << elapsedTime << " [ns] " << endl;
	cout << "------------------------------- ????????????????? ----------------------------------" << endl;

}

void TestFloatStat()
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	clock_gettime(CLOCK_REALTIME, &tstart);
	floatStat_t Test((float *)ftestMatrix, 50, 50);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = tstop.tv_nsec - tstart.tv_nsec;

	uint8_t *p_histRes = (uint8_t *)fhistTestResult;

	cout << "------------------------------- Basic Calculations ----------------------------------" << endl;
	if (!checkArray((uint32_t *)Test.bins, (uint32_t *)p_histRes)) { cout << "Histograms are unequal!" << endl; }
	else { cout << "Histograms are equal!" << endl; }
	cout << "Mean Test | Expected Value : " << fmeanTestResult << " Calc value : " << Test.Mean << endl;
	cout << "n Test | Expected Value : " << fnTestResult << " Calc value : " << Test.n << endl;
	cout << "Sum Test | Expected Value : " << fsumTestResult << " Calc value : " << Test.Sum << endl;
	cout << "Min Test | Expected Value : " << +fminTestResult << " Calc value : " << +Test.min << endl;
	cout << "Max Test | Expected Value : " << +fmaxTestResult << " Calc value : " << +Test.max << endl;
	cout << "Range Test | Expected Value : " << +frangeTestResult << " Calc value : " << +Test.Range << endl;
	cout << "Standard Deviation Test | Expected Value : " << fstdTestResult << " Calc value : " << Test.Std << endl;
	cout << "Average Execution time was : " << elapsedTime << " [ns] " << endl;
	cout << "------------------------------- ????????????????? ----------------------------------" << endl;

}

void TestFFT()
{
	//uchar data[] = 
	//	{0, 0, 0, 0, 0, 0,
	//	 0, 1, 1, 0, 0, 0,
	//	 1, 0, 0, 1, 0, 0,
	//	 1, 1, 0, 0, 1, 0,
	//	 0, 0, 1, 1, 1, 0,
	//	 0, 0, 0, 0, 0, 0 };
	//cv::Mat src(6, 6, CV_8UC1, &data, 1);
	cv::Mat src = cv::imread("EdgeTest.ppm", 0);
	SoilMath::FFT Test;
	ComplexVect_t desc = Test.GetDescriptors(src);

	for_each(desc.begin(), desc.end(), [](Complex_t &e)
	{
		cout << e.real() << " + " << e.imag() << "i" << endl;
	});
}

Predict_t NNfunction(ComplexVect_t input, Weight_t weights)
{
	Predict_t retVal;
	retVal.RealValue = -abs(sin(weights[0])*cos(weights[1])*exp(abs(1 - (sqrt(pow(weights[0], 2) + pow(weights[1], 2)) / M_PI))));
	return retVal;
}

void TestGA()
{
	//SoilMath::GA Test(NNfunction);
	//Weight_t weights;
	//ComplexVect_t input;

	//weights.push_back(0);
	//weights.push_back(0);

	//Test.Evolve(input, weights, MinMaxWeight_t(-10.0f, 10.0f), -19.2085f);
	//for_each(weights.begin(), weights.end(), [](float &w)
	//{
	//	cout << w << endl;
	//});

	//cout << NNfunction(input, weights).RealValue << endl;
}

void TestNN()
{
	SoilMath::NN Test(3, 4, 2);

	InputLearnVector_t inputVect;
	OutputLearnVector_t outputVect;

	Population_t pop;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	float i1 = 0.0, i2 = 0.0, i3 = 0.0;
	float o1 = 0.0, o2 = 0.0;

	for (uint32_t i = 0; i < 200; i++)
	{
		if (dis(gen) > 0.5f) { i1 = 1.0; }
		else { i1 = 0.0; }
		if (dis(gen) > 0.5f) { i2 = 1.0; }
		else { i2 = 0.0; }
		if (dis(gen) > 0.5f) { i3 = 1.0; }
		else { i3 = 0.0; }

		//if (i3 == 0.0)
		//{
		//	if (i1 != i2)
		//	{
		//		o1 == 1.0;
		//		o2 == -1.0;
		//	}
		//	else if (i1 == 0.0 && i2 == 0.0)
		//	{
		//		o1 == 1.0;
		//		o2 == -1.0;
		//	}
		//	else
		//	{
		//		o1 == -1.0;
		//		o2 == 1.0;
		//	}
		//}
		//else
		//{
		//	o1 == -1.0;
		//	o2 == 1.0;
		//}

		if (i1 == 1.0 && i2 == 1.0 && i3 == 1.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else
		{
			o1 = -1.0;
			o2 = 1.0;
		}

		//if (i1 == 1.0 && i2 == 1.0) 
		//{ 
		//	o1 = -1.0;
		//	o2 = 1.0;
		//}
		//else 
		//{
		//	o1 = 1.0;
		//	o2 = -1.0;
		//}

		ComplexVect_t inputTemp;
		inputTemp.push_back(Complex_t(i1, 0));
		inputTemp.push_back(Complex_t(i2, 0));
		inputTemp.push_back(Complex_t(i3, 0));
		inputVect.push_back(inputTemp);

		Predict_t outputTemp;
		outputTemp.OutputNeurons.push_back(o1);
		outputTemp.OutputNeurons.push_back(o2);
		outputVect.push_back(outputTemp);
	}

	Test.Learn(inputVect, outputVect, 0);
	for (uint32_t i = 0; i < inputVect.size(); i++)
	{
		Predict_t outp = Test.Predict(inputVect[i]);
		float dev = 0.0;
		for (uint32_t j = 0; j < outp.OutputNeurons.size(); j++)
		{
			dev += outp.OutputNeurons[j] / outputVect[i].OutputNeurons[j];
		}
		dev = dev - outp.OutputNeurons.size();
		cout << "input : " << abs(inputVect[i][0]) << ", " << abs(inputVect[i][1]) << ", " << abs(inputVect[i][2]) << " = " << round(outp.OutputNeurons[0]) << " , " << round(outp.OutputNeurons[1]) << " acc = " << dev << endl;
	}

}

void TestNNPredict()
{
	SoilMath::NN Test(2, 2, 2);

	InputLearnVector_t inputVect;
	OutputLearnVector_t outputVect;

	ComplexVect_t inp;
	Weight_t iW;
	Weight_t hW;
	iW.push_back(2.17267);
	iW.push_back(0.616982);
	iW.push_back(2.87581);
	iW.push_back(2.9699);
	iW.push_back(0.885658);
	iW.push_back(-0.585273);
	hW.push_back(2.96601);
	hW.push_back(1.322);
	hW.push_back(0.795877);
	hW.push_back(-2.8764);
	hW.push_back(-0.244496);
	hW.push_back(-2.46119);

	inp.push_back(Complex_t(1, 0));
	inp.push_back(Complex_t(1, 0));
	Predict_t result = Test.PredictLearn(inp, iW, hW, 2, 2, 2);
	for_each(result.OutputNeurons.begin(), result.OutputNeurons.end(), [](float &x){cout << x << " "; });
	cout << endl;
}


int main(int argc, char *argv[])
{
	if (argc < 1) DisplayHelp();

	string arg = argv[1];

	if (argc > 0 || arg != "--help")
	{
		for (uint8_t i = 1; i < argc; i++)
		{
			arg = argv[i];
			if (arg == "--Stats-uchar")
			{
				TestUcharStat();
			}
			else if (arg == "--Stats-float")
			{
				TestFloatStat();
			}
			else if (arg == "--FFT")
			{
				TestFFT();
			}
			else if (arg == "--GA")
			{
				TestGA();
			}
			else if (arg == "--NN")
			{
				TestNN();
			}
			else if (arg == "--NNPredict")
			{
				TestNNPredict();
			}
			else { DisplayHelp(); }
		}
	}
	else { DisplayHelp(); }
}

