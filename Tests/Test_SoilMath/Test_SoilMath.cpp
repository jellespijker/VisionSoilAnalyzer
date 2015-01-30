#include <iostream>
#include <sys/time.h>
#include <array>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TestMatrix.h"
#include "FloatTestMatrix.h"
#include "../../src/VisionSoilAnalyzer/SoilMath/SoilMath.h"


using namespace std;

void DisplayHelp()
{
	cout << "--Stats-float         Test floating type stats" << endl;
	cout << "--Stats-uchar         Test uchar type stats" << endl;
	cout << "--FFT                 Test Fast Fourier Transform of an Edge" << endl;
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
	uchar data[] = 
		{0, 0, 0, 0, 0, 0,
		 0, 1, 1, 0, 0, 0,
		 1, 0, 0, 1, 0, 0,
		 1, 1, 0, 0, 1, 0,
		 0, 0, 1, 1, 1, 0,
		 0, 0, 0, 0, 0, 0 };
	cv::Mat src(6, 6, CV_8UC1, &data, 1);
	//src = cv::imread("EdgeTest.ppm", 0);
	SoilMath::FFT Test;
	Test.GetDescriptors(src);
	
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
			else { DisplayHelp(); }
		}
	}
	else { DisplayHelp(); }
}

