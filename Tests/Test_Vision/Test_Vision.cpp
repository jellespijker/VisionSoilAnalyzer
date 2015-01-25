#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../src/VisionSoilAnalyzer/Vision/Vision.h"

using namespace std;
using namespace cv;
using namespace Vision;

void DisplayHelp()
{
	cout << "usage is --testfunction --testfunctionOptional --inputimg-filename" << endl;
	cout << "--Blur" << endl;
	cout << "--AdaptiveContrastStretch" << endl;
	cout << "--ConvertToBW" << endl;
	cout << "--RemoveBorderBlobs" << endl;
	cout << "--LabelBlobs" << endl;
	cout << "--Conversion" << endl;
	cout << "--GetEdges" << endl;
	cout << "--GetBlobList" << endl;
	cout << "--GetBlobEdges" << endl;
}

void TestBlur(const Mat &origImg, Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Enhance Test(origImg);

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.Blur(9);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for Blur : " << elapsedTime << " [ns] " << endl;

	procImg = Test.ProcessedImg;
}

void TestAdaptContrastStretch(const Mat &origImg, Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Enhance Test(origImg);
	Test.Blur(9);

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.AdaptiveContrastStretch(5, 2.5, true);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for AdaptiveContrastStretch : " << elapsedTime << " [ns] " << endl;

	procImg = Test.ProcessedImg;
}

void TestConvertToBW(const Mat &origImg, Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.ConvertToBW(Segment::Dark);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for ConvertToBW : " << elapsedTime << " [ns] " << endl;

	procImg = Test.ProcessedImg;
}

void TestRemoveBorderBlobs(const Mat &origImg, Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.RemoveBorderBlobs(true, Segment::Eight);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for RemoveBorderBlobs : " << elapsedTime << " [ns] " << endl;

	procImg = Test.ProcessedImg;
}

void TestLabelBlobs(const Mat &origImg, Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);
	Test.RemoveBorderBlobs(true, Segment::Eight);

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.LabelBlobs(true, 25, Segment::Eight);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for LabelBlobs : " << elapsedTime << " [ns] " << endl;

	procImg = Test.LabelledImg;
}

void TestConversion(const Mat &origImg, Mat &procImg, Conversion::ColorSpace dstColorspace)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Conversion Test(origImg);
	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.Convert(Conversion::RGB, dstColorspace);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for conversion from RGB 2 : " << dstColorspace << " took :" << elapsedTime << " [ns] " << endl;

	procImg = Test.ProcessedImg;
}

void TestGetEdges(const Mat &origImg, Mat &procImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);
	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.GetEdges(true, Segment::Eight);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for get Edges took :" << elapsedTime << " [ns] " << endl;

	procImg = Test.ProcessedImg;
}

void TestGetBlobList(const Mat &origImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);
	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.GetBlobList(true, Segment::Eight);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for get BlobList took :" << elapsedTime << " [ns] " << endl;
	imwrite("LabelBlobs.ppm", Test.LabelledImg);

	uint32_t i = 1;
	string filen;
	while (i < Test.MaxLabel)
	{
		ostringstream ss;
		ss << i;
		filen = ss.str() + ".ppm";
		imwrite(filen, Test.BlobList[i].Img);
		ss.str("");
		ss.clear();
		ss << i;
		filen = ss.str() + "L.ppm";
		imwrite(filen, Test.LabelledImg(Test.BlobList[i++].cvROI));
	}
}

void TestFillHoles(const Mat &origImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);
	imwrite("BW.ppm", Test.ProcessedImg);
	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.FillHoles(true);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time to fillholes :" << elapsedTime << " [ns] " << endl;
	imwrite("FilledHoles.ppm", Test.ProcessedImg);
}

void TestGetEdgeEroded(const Mat &origImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);

	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.GetEdgesEroding(true);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time to EdgeEroded :" << elapsedTime << " [ns] " << endl;
	imwrite("EdgeEroded.ppm", Test.ProcessedImg);
}

void TestErosion(const Mat &origImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment TestBW(origImg);
	TestBW.ConvertToBW(Segment::Dark);

	MorphologicalFilter Test;
	Mat dst;
	Mat mask(7, 7, CV_8UC1, 1);

	imwrite("BW.ppm", TestBW.ProcessedImg);
	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.Erosion(TestBW.ProcessedImg, dst, mask);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time to erode :" << elapsedTime << " [ns] " << endl;
	imwrite("eroded.ppm", dst);
}

void TestGetBlobEdges(const Mat &origImg)
{
	timespec tstart, tstop, tdiff;
	unsigned int elapsedTime;

	Segment Test(origImg);
	Test.ConvertToBW(Segment::Dark);
	Test.FillHoles(true);
	clock_gettime(CLOCK_REALTIME, &tstart);
	Test.GetBlobList(true, Segment::Eight);
	clock_gettime(CLOCK_REALTIME, &tstop);
	elapsedTime = (unsigned int)(tstop.tv_nsec - tstart.tv_nsec);
	cout << "Execution time for get BlobList took :" << elapsedTime << " [ns] " << endl;

	cv::Mat dst;
	uint32_t i = 1;
	string filen;
	ostringstream ss;

	while (i < Test.MaxLabel)
	{
		Segment edge(Test.BlobList[i].Img);
		edge.GetEdgesEroding();
		ss << i;
		filen = "E_" + ss.str() + ".ppm";
		imwrite(filen, edge.ProcessedImg);
		ss.str("");
		ss.clear();
		ss << i;
		filen = "BW_" + ss.str() + ".ppm";
		imwrite(filen, edge.OriginalImg);
		ss.str("");
		ss.clear();
		i++;
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	Mat origImg;
	Mat procImg;

	if (argc > 1)
	{
		std::string arg = argv[1];
		std::string filename = argv[argc - 1];
		filename = filename.substr(11, filename.length() - 11);

		if (arg == "--help") DisplayHelp();
		else
		{
			for (uint8_t i = 1; i < (argc-1); i++)
			{
				arg = argv[i];
				if (arg == "--Blur")
				{
					origImg = imread(filename, 0);
					TestBlur(origImg, procImg);
					imwrite("Blur.ppm", procImg);
				}
				else if (arg == "--AdaptiveContrastStretch")
				{
					origImg = imread(filename, 0);
					TestAdaptContrastStretch(origImg, procImg);
					imwrite("AdaptiveContrastStretch.ppm", procImg);
				}
				else if (arg == "--ConvertToBW")
				{
					origImg = imread(filename, 0);
					TestConvertToBW(origImg, procImg);
					imwrite("ConvertToBW.ppm", procImg);
				}
				else if (arg == "--RemoveBorderBlobs")
				{
					origImg = imread(filename, 0);
					TestRemoveBorderBlobs(origImg, procImg);
					imwrite("RemoveBorderBlobs.ppm", procImg);
				}
				else if (arg == "--LabelBlobs")
				{
					origImg = imread(filename, 0);
					TestLabelBlobs(origImg, procImg);
					imwrite("LabelBlobs.ppm", procImg);
				}
				else if (arg == "--Conversion")
				{
					origImg = imread(filename, 1);
					TestConversion(origImg, procImg, Conversion::Intensity);
					imwrite("Intensity.ppm", procImg);

					TestConversion(origImg, procImg, Conversion::CIE_XYZ);
					imwrite("XYZ.ppm", procImg);

					TestConversion(origImg, procImg, Conversion::CIE_lab);
					imwrite("Lab.ppm", procImg);

				}
				else if (arg == "--GetEdges")
				{
					origImg = imread(filename, 0);
					TestGetEdges(origImg, procImg);
					imwrite("Edges.ppm", procImg);
				}
				else if (arg == "--GetBlobList")
				{
					origImg = imread(filename, 0);
					TestGetBlobList(origImg);
				}
				else if (arg == "--FillHoles")
				{
					origImg = imread(filename, 0);
					TestFillHoles(origImg);
				}
				else if (arg == "--Erosion")
				{
					origImg = imread(filename, 0);
					TestErosion(origImg);
				}
				else if (arg == "--GetEdgesEroding")
				{
					origImg = imread(filename, 0);
					TestGetEdgeEroded(origImg);
				}
				else if (arg == "--GetBlobEdges")
				{
					origImg = imread(filename, 0);
					TestGetBlobEdges(origImg);

				}
				else
				{
					DisplayHelp();
				}
			}
		}
	}
	else {	DisplayHelp(); }

	return 0;
}