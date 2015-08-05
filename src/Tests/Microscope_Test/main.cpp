#include <iostream>
#include "Microscope.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

int main()
{
  Hardware::Microscope cam;
  cv::Mat frames[5];

  cam.openCam(&cam.AvailableCams[0]);
  cam.GetFrame(frames[0]);
  cv::imshow("Frame 1 cam 0",frames[0]);
  cam.GetFrame(frames[1]);
  cv::imshow("Frame 2 cam 0",frames[1]);
  cam.openCam(&cam.AvailableCams[1]);
  cam.GetFrame(frames[2]);
  cv::imshow("Frame 1 cam 1",frames[2]);
  cam.GetFrame(frames[3]);
  cv::imshow("Frame 2 cam 1",frames[3]);
  cv::waitKey(0);
  cam.openCam(&cam.AvailableCams[0]);
  cam.GetHDRFrame(frames[5]);
  cv::imshow("HDR",frames[5]);
  cv::waitKey(0);

  cout << "Hello World!" << endl;
  return 0;
}

