#define BOOST_TEST_MODULE VSA_UNIT_TESTS

// Custom libraries
#include "../../../../src/VisionSoilAnalyzer/Soil/VisionSoil.h"
#include "../../../../src/VisionSoilAnalyzer/SoilMath/SoilMath.h"
#include "FloatTestMatrix.h"
#include "TestMatrix.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/results_reporter.hpp>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <math.h>
#include <cmath>
#include <random>
#include <sys/time.h>

// Create the Report Redirector
struct LogToFile
{
	LogToFile()
	{
		std::string logFileName(boost::unit_test::framework::master_test_suite().p_name);
		logFileName.append(".log");
		logFile.open(logFileName.c_str());
		boost::unit_test::unit_test_log.set_stream(logFile);
	}
	~LogToFile()
	{
		boost::unit_test::unit_test_log.test_finish();
		logFile.close();
		boost::unit_test::unit_test_log.set_stream(std::cout);
	}
	std::ofstream logFile;
};
BOOST_GLOBAL_FIXTURE(LogToFile);

BOOST_AUTO_TEST_SUITE(SoilMath_Test_Suit)

BOOST_AUTO_TEST_CASE(SoilMath_ucharStat_t)
{
	ucharStat_t Test((uint8_t *)testMatrix, 200, 200);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.bins, Test.bins + 255, histTestResult, histTestResult + 255);
	BOOST_CHECK_CLOSE(Test.Mean, meanTestResult, 0.0001);
	BOOST_CHECK_EQUAL(Test.n, nTestResult);
	BOOST_CHECK_EQUAL(Test.Sum, sumTestResult);
	BOOST_CHECK_EQUAL(Test.min, minTestResult);
	BOOST_CHECK_EQUAL(Test.max, maxTestResult);
	BOOST_CHECK_EQUAL(Test.Range, rangeTestResult);
	BOOST_CHECK_CLOSE(Test.Std, stdTestResult, 0.01);
}

BOOST_AUTO_TEST_CASE(SoilMath_floatStat_t)
{
	floatStat_t Test((float *)ftestMatrix, 50, 50);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.bins, Test.bins + 255, fhistTestResult, fhistTestResult + 255);
	BOOST_CHECK_CLOSE(Test.Mean, fmeanTestResult, 0.01);
	BOOST_CHECK_EQUAL(Test.n, fnTestResult);
	BOOST_CHECK_CLOSE(Test.Sum, fsumTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.min, fminTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.max, fmaxTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.Range, frangeTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.Std, fstdTestResult, 0.025);
}

BOOST_AUTO_TEST_CASE(SoilMath_FFT_GetDescriptors)
{
	uchar data[] =
	{ 0, 0, 1, 1, 0, 0,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 0, 0 };
	cv::Mat src(8, 6, CV_8UC1, &data, 1);
	SoilMath::FFT Test;
	ComplexVect_t desc = Test.GetDescriptors(src);
	Complex_t desc_exp[] = {
		Complex_t(-1.6666667, -6),
		Complex_t(2.02375780, -0.16742019),
		Complex_t(-3.1094758, -6.13316500),
		Complex_t(-1.8036530, 1.023864110),
		Complex_t(-0.6666667, -1),
		Complex_t(0.04350554, -2.55884126),
		Complex_t(-3.0522848, -0.63807119),
		Complex_t(-0.7780360, -0.31809187),
		Complex_t(-0.3333333, 0),
		Complex_t(-1.3856866, -1.43021101),
		Complex_t(-1.2238576, 0.466498316),
		Complex_t(-0.3295119, 1.459385072),
		Complex_t(1.33333331, -1),
		Complex_t(-0.3482434, 0.489805636),
		Complex_t(0.71895134, 0.304737878),
		Complex_t(5.24453433, -0.49849056)
	};

	BOOST_CHECK_EQUAL(desc.size(), 16);

	for (uint32_t i = 0; i < 16; i++)
	{
		BOOST_CHECK_CLOSE(desc[i].real(), desc_exp[i].real(), 0.0001);
		BOOST_CHECK_CLOSE(desc[i].imag(), desc_exp[i].imag(), 0.0001);
	}
}

BOOST_AUTO_TEST_CASE(SoilMath_FFT_GetDescriptors_Non_Continues_Contour)
{
	uchar data[] =
	{ 0, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 0, 0 };
	cv::Mat src(8, 6, CV_8UC1, &data, 1);
	SoilMath::FFT Test;
	BOOST_CHECK_THROW(Test.GetDescriptors(src), SoilMath::Exception::MathException);
}

BOOST_AUTO_TEST_CASE(SoilMath_NN_Save_And_Load)
{
	SoilMath::NN Test(3, 5, 2);

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

		if (i1 == 1.0 && i2 == 1.0 && i3 == 0.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else if (i1 == 0.0 && i2 == 0.0 && i3 == 1.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else
		{
			o1 = -1.0;
			o2 = 1.0;
		}

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
	Test.SaveState("NN.xml");

	SoilMath::NN loadTest;
	loadTest.LoadState("NN.xml");

	std::vector<float> test_out = Test.Predict(inputVect[0]).OutputNeurons;
	std::vector<float> loadtest_out = loadTest.Predict(inputVect[0]).OutputNeurons;

	BOOST_CHECK_EQUAL_COLLECTIONS(test_out.begin(), test_out.end(), loadtest_out.begin(), loadtest_out.end());

}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(SoilAnalyzer_Test_Suite)

BOOST_AUTO_TEST_CASE(Soil_Sample_Save_And_Load)
{
	SoilAnalyzer::Sample Test;
	std::string filename = "SoilSample.xml";
	Test.Save(filename);
}
BOOST_AUTO_TEST_SUITE_END()