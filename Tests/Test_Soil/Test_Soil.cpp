#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../src/VisionSoilAnalyzer/Soil/VisionSoil.h"

using namespace std;

void DisplayHelp()
{
	cout << "--Vision         Test floating type stats" << endl;
}

void SoilSaveLoad()
{
	SoilAnalyzer::Sample Test;
	uchar data[] =
	{ 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 0,
	1, 0, 0, 1, 0, 0,
	1, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0 };
	Test.BW = cv::Mat(6, 6, CV_8UC1, &data, 1);
	std::string filename = "Sample.xml";
	Test.Save(filename);
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
			if (arg == "--SoilSaveLoad")
			{
				SoilSaveLoad();
			}
			else { DisplayHelp(); }
		}
	}

	return 0;
}