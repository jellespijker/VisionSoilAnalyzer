#define BOOST_TEST_MODULE VSA_UNIT_TESTS

// Custom libraries
#include "../../../../src/VisionSoilAnalyzer/Vision/Vision.h"
#include "../../../../src/VisionSoilAnalyzer/Soil/VisionSoil.h"
#include "../../../../src/VisionSoilAnalyzer/SoilMath/SoilMath.h"
#include "../../../../src/VisionSoilAnalyzer/SoilPlot/SoilPlot.h"

// Test Libraries
#include "FloatTestMatrix.h"
#include "TestMatrix.h"
#include "StatisticalComparisonDefinition.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/results_reporter.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

// Statistical analysis

#include <math.h>
#include <cmath>
#include <random>
#include <sys/time.h>

using namespace cv;
using namespace std;

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

struct M {
	M()
	{
		BOOST_TEST_MESSAGE("setup fixture");
		src = imread("../ComparisionPictures/SoilSampleRGB.ppm");
	}
	~M() { BOOST_TEST_MESSAGE("teardown fixture"); }

	friend class SoilAnalyzer::Sample;
	Mat src;
	Mat dst;
	Mat comp;
};

struct B {
	B()
	{
		BOOST_TEST_MESSAGE("Setup fixture");
		src = imread("../ComparisionPictures/BlobTest.ppm", 0);
	}
	~B() 
	{
		src.release();
		BOOST_TEST_MESSAGE("teardown fixture");
	}

	Mat src;
	Mat dst;
	Mat comp;
};

struct BM {
	BM()
	{
		BOOST_TEST_MESSAGE("Setup fixture");
		src = imread("../ComparisionPictures/BlobTestMany.ppm", 0);
	}
	~BM() { BOOST_TEST_MESSAGE("teardown fixture"); }

	Mat src;
	Mat dst;
	Mat comp;
};

struct PM {
	PM()
	{
		BOOST_TEST_MESSAGE("Setup fixture");
		checker = imread("../ComparisionPictures/checker.ppm", 0);
		std::vector<cv::Mat> RGB;
		for (uint32_t i = 0; i < 3; i++)
		{
			RGB.push_back(checker);
		}
		cv::Mat alpha(checker.rows, checker.cols, CV_8UC1);
		alpha.setTo(255);
		RGB.push_back(alpha);
		merge(RGB, RGBchecker);
	}
	~PM() { BOOST_TEST_MESSAGE("teardown fixture"); }

	std::string TestWindow = "SoilPlot Image window";
	Mat checker;
	Mat RGBchecker;
	Mat comp;
};

//SoilPlot Test
BOOST_FIXTURE_TEST_CASE(SoilPlot_Label, PM)
{
	SoilPlot::Label Test;
	Test.EdgeColor = cv::Scalar(255, 0, 0, 255);
	Test.FillColor = cv::Scalar::all(0);
	Test.Text << "Soil!";
	Test.Font = cv::FONT_HERSHEY_COMPLEX_SMALL;
	Test.Scale = 2;
	Test.Draw();
	Test.DrawOnTop(RGBchecker, Test.TopLeftCorner);
	namedWindow(TestWindow, cv::WINDOW_NORMAL);
	imshow(TestWindow, RGBchecker);
	cv::waitKey(0);
}

BOOST_FIXTURE_TEST_CASE(SoilPlot_Lines, PM)
{
	std::vector<SoilPlot::Line> Lines;
	Lines.push_back(SoilPlot::Line(cv::Point(80, 10), cv::Point(80, 150), 3, cv::Scalar(255, 0, 0, 255)));
	Lines.push_back(SoilPlot::Line(cv::Point(10, 80), cv::Point(150, 80), 3, cv::Scalar(0, 255, 0, 255)));
	Lines.push_back(SoilPlot::Line(cv::Point(10, 10), cv::Point(150, 150), 5, cv::Scalar(0, 0, 255, 255)));
	Lines.push_back(SoilPlot::Line(cv::Point(150, 10), cv::Point(10, 150), 10, cv::Scalar(255, 255, 255, 127)));
	for_each(Lines.begin(), Lines.end(), [&](SoilPlot::Line &L) 
	{
		L.Draw();
		L.DrawOnTop(RGBchecker, L.TopLeftCorner);
	});

	namedWindow(TestWindow, cv::WINDOW_NORMAL);
	imshow(TestWindow, RGBchecker);
	cv::waitKey(0);
}


BOOST_AUTO_TEST_CASE(SoilPlot_Bar_Graph)
{
	SoilPlot::Graph Test;
	
	Test.Figure.create(500, 500, CV_8UC4);
	Test.Env.Figure.create(500, 500, CV_8UC4);
	Test.Env.GraphBackground.Figure.create(500, 500, CV_8UC4);
	Test.Env.GraphBackground.FillColor = cv::Scalar(0, 0, 0, 255);

	Test.Env.GraphTitleBorderOffset = 500 * 0.05;

	Test.Env.GraphTitle.Text << "Soil!";
	Test.Env.GraphTitle.Thickness = 2;
	Test.Env.GraphTitle.Font = cv::FONT_HERSHEY_COMPLEX;
	Test.Env.GraphTitle.FillColor = cv::Scalar(0, 0, 0, 0);
	Test.Env.GraphTitle.EdgeColor = cv::Scalar(255, 0, 120, 255);
	Test.Env.GraphTitle.Scale = 3;
	Test.Draw();

	SoilPlot::Line test(cv::Point(250, 10), cv::Point(400, 400));
	test.FillColor = cv::Scalar(255, 255, 255, 0);
	test.EdgeColor = cv::Scalar(255, 255, 0, 255);
	test.Thickness = 4;
	test.Draw();
	test.DrawOnTop(Test.Figure, test.StartPoint);


	//Test.Env.X0.StartPoint = cv::Point(10, 10);
	//Test.Env.X0.EndPoint = cv::Point(490, 10);
	//Test.Env.X0.Thickness = 5;
	//Test.Draw();

	namedWindow("Test uint32_t Bar Graph", cv::WINDOW_NORMAL);
	imshow("Test uint32_t Bar Graph", Test.Figure);
	cv::waitKey(0);
}

BOOST_AUTO_TEST_CASE(Put_text)
{
	string text = "Funny text inside the box";
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 2;
	int thickness = 3;

	Mat img(600, 800, CV_8UC4, Scalar::all(0));

	int baseline = 0;
	Size textSize = getTextSize(text, fontFace,
		fontScale, thickness, &baseline);
	baseline += thickness;

	// center the text
	Point textOrg((img.cols - textSize.width) / 2,
		(img.rows + textSize.height) / 2);

	// draw the box
	rectangle(img, textOrg + Point(0, baseline),
		textOrg + Point(textSize.width, -textSize.height),
		Scalar(0, 0, 255));
	// ... and the baseline first
	line(img, textOrg + Point(0, thickness),
		textOrg + Point(textSize.width, thickness),
		Scalar(0, 0, 255));

	// then put the text itself
	putText(img, text, textOrg, fontFace, fontScale,
		Scalar::all(255), thickness, 8);

	namedWindow("Test text", cv::WINDOW_NORMAL);
	imshow("Test text", img);
	cv::waitKey(0);

}


// SoilMath Test
BOOST_AUTO_TEST_CASE(Vision_Create_Blobs)
{
	cv::Mat src = imread("../ComparisionPictures/BlobTest.ppm", 0);

	Vision::Segment Test(src);
	Test.ConvertToBW(Vision::Segment::Bright);
	Test.GetBlobList(true);
	imwrite("LabeledBlobs.ppm", Test.LabelledImg);
	BOOST_CHECK_EQUAL(42, Test.BlobList.size());
	for_each(Test.BlobList.begin(), Test.BlobList.end(), [](Vision::Segment::Blob_t &b)
	{
		stringstream ss;
		ss << b.Label << "_few_1nd.ppm";
		imwrite(ss.str(), b.Img);
	});
}

BOOST_FIXTURE_TEST_CASE(Vision_Create_Many_Blobs, BM)
{
	Vision::Segment Test(src);
	Test.ConvertToBW(Vision::Segment::Bright);
	Test.GetBlobList(true);
	imwrite("LabeledBlobsMany.ppm", Test.LabelledImg);
	BOOST_CHECK_EQUAL(370, Test.BlobList.size());
	for_each(Test.BlobList.begin(), Test.BlobList.end(), [](Vision::Segment::Blob_t &b)
	{
		stringstream ss;
		ss << b.Label << "_many_.ppm";
		imwrite(ss.str(), b.Img);
	});
}

BOOST_AUTO_TEST_CASE(SoilMath_Stats_DiscreteMath_BigNumber)
{
	uint16_t *BigNoTestMatrix = new uint16_t[40000];
	uint32_t count = 0;
	for (uint32_t i = 0; i < 200; i++)
	{
		for (uint32_t j = 0; j < 200; j++)
		{
			BigNoTestMatrix[count++] = testMatrix[i][j] * 10;
		}
	}

	uint16Stat_t Test(BigNoTestMatrix, 200, 200);
	BOOST_CHECK_EQUAL_COLLECTIONS(Test.bins, Test.bins + 255, histTestResult, histTestResult + 255);
	BOOST_CHECK_CLOSE(Test.Mean, meanTestResult * 10, 0.0001);
	BOOST_CHECK_EQUAL(Test.n, nTestResult);
	BOOST_CHECK_EQUAL(Test.Sum, sumTestResult * 10);
	BOOST_CHECK_EQUAL(Test.min, minTestResult * 10);
	BOOST_CHECK_EQUAL(Test.max, maxTestResult * 10);
	BOOST_CHECK_EQUAL(Test.Range, rangeTestResult * 10);
	BOOST_CHECK_CLOSE(Test.Std, stdTestResult * 10, 0.01);
	delete[] BigNoTestMatrix;
}

BOOST_AUTO_TEST_CASE(SoilMath_Sort)
{
	int testDiscrete[] = { 2, 8, 1, 3, 0, 7, 4, 5, 9, 6, 6 };
	int ucomp[] = { 0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 9 };
	SoilMath::Sort::QuickSort<int>(testDiscrete, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(testDiscrete, testDiscrete + 10, ucomp, ucomp + 10);
}

BOOST_AUTO_TEST_CASE(SoilMath_Stats_ucharStat_t)
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

BOOST_AUTO_TEST_CASE(SoilMath_Stats_floatStat_t)
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

	//Population_t pop;
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

	BOOST_REQUIRE_EQUAL_COLLECTIONS(Test.hWeights.begin(), Test.hWeights.end(), loadTest.hWeights.begin(), loadTest.hWeights.end());
	BOOST_REQUIRE_EQUAL_COLLECTIONS(Test.iWeights.begin(), Test.iWeights.end(), loadTest.iWeights.begin(), loadTest.iWeights.end());
}

BOOST_AUTO_TEST_CASE(SoilMath_NN_Prediction_Accurancy)
{
	SoilMath::NN Test;
	Test.LoadState("../ComparisionPictures/NN_test.xml");

	InputLearnVector_t inputVect;
	OutputLearnVector_t outputVect;
	OutputLearnVector_t outputPredictVect;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	float i1 = 0.0, i2 = 0.0, i3 = 0.0;
	float o1 = 0.0, o2 = 0.0;

	for (uint32_t i = 0; i < 10; i++)
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

		Predict_t outputPredictTemp;
		outputPredictTemp.OutputNeurons = Test.Predict(inputTemp).OutputNeurons;

		for (uint32_t j = 0; j < outputTemp.OutputNeurons.size(); j++)
		{
			BOOST_CHECK_CLOSE(outputPredictTemp.OutputNeurons[j], outputTemp.OutputNeurons[j], 5);
		}
	}
}

// Vision Test

BOOST_FIXTURE_TEST_CASE(Vision_Convert_RGB_To_Intensity, M)
{
	// Convert the RGB picture to an intensity picture
	Vision::Conversion Test;
	Test.Convert(src, dst, Vision::Conversion::RGB, Vision::Conversion::Intensity);

	// Read in the Matlab converted intensity picture converted with the Matlab command:
	// Matlab_int=0.2126*RGB(:,:,1)+0.7152*RGB(:,:,2)+0.0722*RGB(:,:,3);
	comp = imread("../ComparisionPictures/Matlab_int.ppm", 0);

	// Calculate the statistics of the two images
	ucharStat_t statDst(dst.data, dst.rows, dst.cols);
	ucharStat_t statComp(comp.data, comp.rows, comp.cols);

	// Simple comparison
	BOOST_CHECK_CLOSE(statDst.Mean, statComp.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDst.Std, statComp.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.Range, (double)statComp.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.min, (double)statComp.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.max, (double)statComp.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.Sum, (double)statComp.Sum, 0.5);
}

BOOST_FIXTURE_TEST_CASE(Vision_Convert_RGB_To_CIEXYZ, M)
{
	// Convert the RGB to an CIElab
	Vision::Conversion Test;
	Test.Convert(src, dst, Vision::Conversion::RGB, Vision::Conversion::CIE_XYZ);
	vector<Mat> LAB = Test.extractChannel(dst);

	floatStat_t statDstX((float *)LAB[0].data, src.rows, src.cols);
	floatStat_t statCompX;
	statCompX.Std = X_STD;
	statCompX.n = N_MAT;
	statCompX.Mean = X_MEAN;
	statCompX.Range = X_RANGE;
	statCompX.min = X_MIN;
	statCompX.max = X_MAX;
	statCompX.Sum = X_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstX.Mean, statCompX.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstX.Std, statCompX.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.Range, (double)statCompX.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.min, (double)statCompX.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.max, (double)statCompX.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.Sum, (double)statCompX.Sum, 0.5);

	floatStat_t statDstY((float *)LAB[1].data, src.rows, src.cols);
	floatStat_t statCompY;
	statCompY.Std = Y_STD;
	statCompY.n = N_MAT;
	statCompY.Mean = Y_MEAN;
	statCompY.Range = Y_RANGE;
	statCompY.min = Y_MIN;
	statCompY.max = Y_MAX;
	statCompY.Sum = Y_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstY.Mean, statCompY.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstY.Std, statCompY.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.Range, (double)statCompY.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.min, (double)statCompY.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.max, (double)statCompY.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.Sum, (double)statCompY.Sum, 0.5);

	floatStat_t statDstZ((float *)LAB[2].data, src.rows, src.cols);
	floatStat_t statCompZ;
	statCompZ.Std = Z_STD;
	statCompZ.n = N_MAT;
	statCompZ.Mean = Z_MEAN;
	statCompZ.Range = Z_RANGE;
	statCompZ.min = Z_MIN;
	statCompZ.max = Z_MAX;
	statCompZ.Sum = Z_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstZ.Mean, statCompZ.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstZ.Std, statCompZ.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.Range, (double)statCompZ.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.min, (double)statCompZ.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.max, (double)statCompZ.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.Sum, (double)statCompZ.Sum, 0.5);
}

BOOST_FIXTURE_TEST_CASE(Vision_Convert_RGB_To_CIElab, M)
{
	// Convert the RGB to an CIElab
	Vision::Conversion Test;
	Test.Convert(src, dst, Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
	vector<Mat> LAB = Test.extractChannel(dst);

	floatStat_t statDstL((float *)LAB[0].data, src.rows, src.cols);
	floatStat_t statCompL;
	statCompL.Std = L_STD;
	statCompL.n = N_MAT;
	statCompL.Mean = L_MEAN;
	statCompL.Range = L_RANGE;
	statCompL.min = L_MIN;
	statCompL.max = L_MAX;
	statCompL.Sum = L_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstL.Mean, statCompL.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstL.Std, statCompL.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.Range, (double)statCompL.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.min, (double)statCompL.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.max, (double)statCompL.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.Sum, (double)statCompL.Sum, 0.5);

	floatStat_t statDstA((float *)LAB[1].data, src.rows, src.cols);
	floatStat_t statCompA;
	statCompA.Std = A_STD;
	statCompA.n = N_MAT;
	statCompA.Mean = A_MEAN;
	statCompA.Range = A_RANGE;
	statCompA.min = A_MIN;
	statCompA.max = A_MAX;
	statCompA.Sum = A_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstA.Mean, statCompA.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstA.Std, statCompA.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.Range, (double)statCompA.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.min, (double)statCompA.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.max, (double)statCompA.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.Sum, (double)statCompA.Sum, 0.5);

	floatStat_t statDstB((float *)LAB[2].data, src.rows, src.cols);
	floatStat_t statCompB;
	statCompB.Std = B_STD;
	statCompB.n = N_MAT;
	statCompB.Mean = B_MEAN;
	statCompB.Range = B_RANGE;
	statCompB.min = B_MIN;
	statCompB.max = B_MAX;
	statCompB.Sum = B_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstB.Mean, statCompB.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstB.Std, statCompB.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.Range, (double)statCompB.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.min, (double)statCompB.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.max, (double)statCompB.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.Sum, (double)statCompB.Sum, 0.5);
}

BOOST_FIXTURE_TEST_CASE(Vision_Convert_LAB_To_RI, M)
{
	// Convert the RGB to an CIElab
	Vision::Conversion Test;
	Test.Convert(src, src, Vision::Conversion::RGB, Vision::Conversion::CIE_lab);

	Test.Convert(src, dst, Vision::Conversion::CIE_lab, Vision::Conversion::RI);

	floatStat_t statDstRI((float *)dst.data, src.rows, src.cols);
	floatStat_t statCompRI;
	statCompRI.Std = RI_STD;
	statCompRI.n = N_MAT;
	statCompRI.Mean = RI_MEAN;
	statCompRI.Range = RI_RANGE;
	statCompRI.min = RI_MIN;
	statCompRI.max = RI_MAX;
	statCompRI.Sum = RI_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstRI.Mean, statCompRI.Mean, 1.25);
	BOOST_CHECK_CLOSE(statDstRI.Std, statCompRI.Std, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.Range, (double)statCompRI.Range, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.min, (double)statCompRI.min, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.max, (double)statCompRI.max, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.Sum, (double)statCompRI.Sum, 1.25);
}

BOOST_AUTO_TEST_CASE(Vision_CopyMat_3_Uchar_Channel_With_Mask)
{
	Mat RGB_crop = imread("../ComparisionPictures/RGB_crop.ppm", 1);
	Mat mask = imread("../ComparisionPictures/mask.ppm", 0);
	Mat RGB_res = imread("../ComparisionPictures/RGB_res.ppm", 1);

	Mat copiedMat = Vision::ImageProcessing::CopyMat<uchar>(RGB_crop, mask, CV_8UC1);

	BOOST_CHECK_EQUAL_COLLECTIONS(copiedMat.begin<cv::Vec3b>(), copiedMat.end<cv::Vec3b>(), RGB_res.begin<cv::Vec3b>(), RGB_res.end<cv::Vec3b>());
	cv::imwrite("Copy_with_Mask.ppm", copiedMat);
}

BOOST_AUTO_TEST_CASE(Vision_CopyMat_1_Uchar_Channel_With_Mask)
{
	Mat Int = imread("../ComparisionPictures/RGB_crop.ppm", 0);
	Mat mask = imread("../ComparisionPictures/mask.ppm", 0);
	Mat Int_res = imread("../ComparisionPictures/RGB_res.ppm", 0);

	Mat copiedMat = Vision::ImageProcessing::CopyMat<uchar>(Int, mask, CV_8UC1);

	BOOST_CHECK_EQUAL_COLLECTIONS(copiedMat.begin<uchar>(), copiedMat.end<uchar>(), Int_res.begin<uchar>(), Int_res.end<uchar>());
	cv::imwrite("Copy_with_Mask.ppm", copiedMat);
}

BOOST_AUTO_TEST_CASE(Vision_CopyMat_1_Float_Channel_With_Mask)
{
	Mat floatTest = (Mat_<float>(5, 5) <<	0.1, 0.2, 0.3, 0.4, 0.5,
											1.1, 1.2, 1.3, 1.4, 1.5,
											2.1, 2.2, 2.3, 2.4, 2.5,
											3.1, 3.2, 3.3, 3.4, 3.5,
											4.1, 4.2, 4.3, 4.4, 4.5,
											5.1, 5.2, 5.3, 5.4, 5.5);

	Mat mask = (Mat_<float>(5, 5) <<		0, 0, 0, 0, 0,
											0, 1, 1, 0, 0,
											1, 1, 1, 1, 0,
											0, 0, 0, 1, 1,
											0, 0, 0, 0, 0,
											0, 0, 0, 0, 0);

	Mat floatres = (Mat_<float>(5, 5) <<	0, 0, 0, 0, 0,
											0, 1.2, 1.3,0, 0,
											2.1, 2.2, 2.3, 2.4,0,
											0, 0, 0, 3.4, 3.5,
											0, 0, 0, 0, 0,
											0, 0, 0, 0, 0);
	Mat copiedMat = Vision::ImageProcessing::CopyMat<float>(floatTest, mask, CV_32F);
	BOOST_CHECK_EQUAL_COLLECTIONS(copiedMat.begin<float>(), copiedMat.end<float>(), floatres.begin<float>(), floatres.end<float>());
}

BOOST_AUTO_TEST_CASE(Vision_CopyMat_With_LUT)
{
	Mat BW_res = imread("../ComparisionPictures/mask.ppm", 0);
	Mat Label = imread("../ComparisionPictures/Label.ppm", 0);
	uint8_t *LUT = new uint8_t[300]{};
	LUT[255] = 1;

	Mat copiedMat = Vision::ImageProcessing::CopyMat<uint8_t, uint8_t>(Label, LUT, CV_8UC1);
	imwrite("copiedMat.ppm", copiedMat);
	BOOST_CHECK_EQUAL_COLLECTIONS(copiedMat.data, copiedMat.data + (copiedMat.rows * copiedMat.cols), BW_res.data, BW_res.data + (BW_res.rows * BW_res.cols));
	delete[] LUT;
}

BOOST_AUTO_TEST_CASE(Vision_RemoveBorder)
{
	cv::Mat Border = imread("../ComparisionPictures/Border.ppm", 0);
	cv::Mat noBorderComp = imread("../ComparisionPictures/noBorder.ppm", 0);
	
	Vision::Segment Test(Border);
	Test.RemoveBorderBlobs(1);
	imwrite("noBorder.ppm", Test.ProcessedImg);
	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.data, Test.ProcessedImg.data + (Test.ProcessedImg.rows * Test.ProcessedImg.cols), noBorderComp.data, noBorderComp.data + (noBorderComp.rows * noBorderComp.cols));
}

BOOST_AUTO_TEST_CASE(Vision_RemoveBorder_Chain_Event)
{
	cv::Mat Border = imread("../ComparisionPictures/Border.ppm", 0);
	cv::Mat noBorderComp = imread("../ComparisionPictures/noBorder.ppm", 0);
	Border *= 255;

	Vision::Segment Test(Border);
	Test.ConvertToBW(Vision::Segment::Bright);
	Test.FillHoles(true);
	Test.RemoveBorderBlobs(1, true);
	imwrite("noBorder.ppm", Test.ProcessedImg);
	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.data, Test.ProcessedImg.data + (Test.ProcessedImg.rows * Test.ProcessedImg.cols), noBorderComp.data, noBorderComp.data + (noBorderComp.rows * noBorderComp.cols));
}

BOOST_AUTO_TEST_CASE(Vision_MorphologicalFilter_Erode)
{
	cv::Mat FilterImg = imread("../ComparisionPictures/FilterImg.ppm", 0);
	cv::Mat ErosionComp = imread("../ComparisionPictures/erodedResult.ppm", 0);
	Mat mask = imread("../ComparisionPictures/MorphMask.ppm", 0);
	Vision::MorphologicalFilter Test(FilterImg);
	Test.Erosion(mask);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.begin<uchar>(), Test.ProcessedImg.end<uchar>(), ErosionComp.begin<uchar>(), ErosionComp.end<uchar>());
}

BOOST_AUTO_TEST_CASE(Vision_MorphologicalFilter_Dilate)
{
	cv::Mat FilterImg = imread("../ComparisionPictures/FilterImg.ppm", 0);
	cv::Mat DilateComp = imread("../ComparisionPictures/dilatedResult.ppm", 0);
	Mat mask = imread("../ComparisionPictures/MorphMask.ppm", 0);
	Vision::MorphologicalFilter Test(FilterImg);
	Test.Dilation(mask);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.begin<uchar>(), Test.ProcessedImg.end<uchar>(), DilateComp.begin<uchar>(), DilateComp.end<uchar>());
}

BOOST_AUTO_TEST_CASE(Vision_MorphologicalFilter_Open)
{
	cv::Mat FilterImg = imread("../ComparisionPictures/FilterImg.ppm", 0);
	cv::Mat OpenComp = imread("../ComparisionPictures/openResult.ppm", 0);
	Mat mask = imread("../ComparisionPictures/MorphMask.ppm", 0);
	Vision::MorphologicalFilter Test(FilterImg);
	Test.Open(mask);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.begin<uchar>(), Test.ProcessedImg.end<uchar>(), OpenComp.begin<uchar>(), OpenComp.end<uchar>());
}

BOOST_AUTO_TEST_CASE(Vision_MorphologicalFilter_Close)
{
	cv::Mat FilterImg = imread("../ComparisionPictures/FilterImg.ppm", 0);
	cv::Mat CloseComp = imread("../ComparisionPictures/closeResult.ppm", 0);
	Mat mask = imread("../ComparisionPictures/MorphMask.ppm", 0);
	Vision::MorphologicalFilter Test(FilterImg);
	Test.Close(mask);
	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.begin<uchar>(), Test.ProcessedImg.end<uchar>(), CloseComp.begin<uchar>(), CloseComp.end<uchar>());
}

BOOST_AUTO_TEST_CASE(Vision_Segment_GetEdgesByErosion)
{
	cv::Mat BlobImg = imread("../ComparisionPictures/FilterImg.ppm", 0);
	cv::Mat EdgeComp = imread("../ComparisionPictures/BlobTestEdgeResult.ppm", 0);

	Vision::Segment Test(BlobImg);
	Test.GetEdgesEroding();
	BOOST_CHECK_EQUAL_COLLECTIONS(Test.ProcessedImg.begin<uchar>(), Test.ProcessedImg.end<uchar>(), EdgeComp.begin<uchar>(), EdgeComp.end<uchar>());
}

// Soil Test

BOOST_FIXTURE_TEST_CASE(Soil_Particle_Analyze, M)
{
	SoilMath::NN nn;
	nn.LoadState("../ComparisionPictures/NN_test.xml");

	SoilAnalyzer::Sample Test(src);
	Test.Analyse(nn);

	std::string filename = "SoilSample.vsa";
	Test.Save(filename);

	imwrite("BW.ppm", Test.BW);
//	imwrite("Edge.ppm", Test.Edge);

	//uint32_t i = 0;
	//for_each(Test.Population.begin(), Test.Population.end(), [&](SoilAnalyzer::Particle &p) 
	//{	
	//	stringstream ss_edge;
	//	stringstream ss_RGB;
	//	ss_edge << i << "_edge.ppm";
	//	ss_RGB << i << "_RGB.ppm";
	//	imwrite(ss_edge.str(), p.Edge);
	//	imwrite(ss_RGB.str(), p.RGB);
	//});
}

BOOST_FIXTURE_TEST_CASE(Soil_Sample_Save_And_Load, M)
{
	SoilAnalyzer::Sample Test(src);
	std::string filename = "SoilSample.vsa";
	Test.Save(filename);

	SoilAnalyzer::Sample TestLoad;
	TestLoad.Load(filename);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.OriginalImage.data, Test.OriginalImage.data + 1000, TestLoad.OriginalImage.data, TestLoad.OriginalImage.data + 1000);
}

BOOST_FIXTURE_TEST_CASE(Soil_Sample_Analyze, M)
{
	//SoilAnalyzer::Sample Test(src);
	//Test.Analyse();

	//imwrite("A_BW.ppm", Test.BW);
	//imwrite("A_Edge.ppm", Test.Edge);
	//imwrite("A_Int.ppm", Test.Intensity);
	//
	//for (uint32_t i = 0; i < Test.Population.size(); i++)
	//{
	//	std::stringstream ss;
	//	ss << i << "_BW.ppm";
	//	imwrite(ss.str(), Test.Population[i].BW);
	//	std::stringstream sss;
	//	sss << i << "_Edge.ppm";
	//	imwrite(sss.str(), Test.Population[i].Edge);
	//	std::stringstream ssss;
	//	ssss << i << "_RGB.ppm";
	//	imwrite(ssss.str(), Test.Population[i].RGB);
	//}
}

