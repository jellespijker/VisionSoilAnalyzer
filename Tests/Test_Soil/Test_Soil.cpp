#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../src/VisionSoilAnalyzer/Soil/VisionSoil.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Soil_Test

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(XML_SERIALIZATION_SAMPLE)
{
	std::string filename = "Soil_Test_Sample.xml";

	uchar data[] =
	{ 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 0,
	1, 0, 0, 1, 0, 0,
	1, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0 };

	SoilAnalyzer::Sample Test_Save;

	Test_Save.BW = cv::Mat(6, 6, CV_8UC1, &data, 1);
	Test_Save.Save(filename);

	SoilAnalyzer::Sample Test_Load;
	Test_Load.Load(filename);

	BOOST_CHECK_EQUAL(Test_Save, Test_Load);
}

BOOST_AUTO_TEST_CASE(XML_SERIALIZATION_PARTICLE)
{
	std::string filename = "Soil_Test_Sample.xml";

	uchar data[] =
	{ 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 0,
	1, 0, 0, 1, 0, 0,
	1, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0 };

	SoilAnalyzer::Particle Test_Save;

	Test_Save.BW = cv::Mat(6, 6, CV_8UC1, &data, 1);
	Test_Save.Save(filename);

	SoilAnalyzer::Particle Test_Load;
	Test_Load.Load(filename);

	BOOST_CHECK_EQUAL(Test_Save, Test_Load);
}