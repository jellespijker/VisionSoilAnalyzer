/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class Microscope
Interaction with the USB 5 MP microscope
*/

#pragma once
#define MICROSCOPE_VERSION 1 /*!< Version of the class*/

#define MIN_BRIGHTNESS -64
#define MAX_BRIGHTNESS 64
#define MIN_CONTRAST 0
#define MAX_CONTRAST 64
#define MIN_SATURATION 0
#define MAX_SATURATION 128
#define MIN_HUE -40
#define MAX_HUE 40
#define MIN_GAMMA 40
#define MAX_GAMMA 500
#define MIN_SHARPNESS 1
#define MAX_SHARPNESS 25

#include "stdint.h"
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/utsname.h>

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include "USB.h"

#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2/videoio.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

namespace Hardware {
class Microscope {
public:
  /*! Struct that represent the Resolution that is used */
  struct Resolution {
  public:
    uint16_t Width;  /*!< Width of the image*/
    uint16_t Height; /*!< Height of the image*/
  };

  typedef boost::signals2::signal<void()> Finished_t;
  typedef boost::signals2::signal<void(int)> Progress_t;

  boost::signals2::connection
  connect_Finished(const Finished_t::slot_type &subscriber);
  boost::signals2::connection
  connect_Progress(const Progress_t::slot_type &subscriber);

  uint8_t FrameDelayTrigger; /*!< Delay in seconds */
  cv::Mat LastFrame;         /*!< Last grabbed and processed frame */
  Resolution Dimensions;     /*!< Dimensions of the frame */

  Microscope(uint8_t frameDelayTrigger = 3,
             Resolution dimensions = Resolution{2048, 1536},
             bool firstdefault = true);
  ~Microscope();

  std::vector<std::string> AvailableCams();

  void GetFrame(cv::Mat &dst);
  void GetHDRFrame(cv::Mat &dst, uint32_t noframes = 5);

  bool IsOpened();
  void Release();

  void openCam(int dev);

private:
  Finished_t fin_sig;
  Progress_t prog_sig;

  std::string arch;

  cv::VideoCapture
      captureDevice; /*!< An openCV instance of the capture device*/
  void StartupSeq(bool firstdefault);

  std::vector<cv::Mat> HDRframes;
  std::vector<float> times;

  bool exist(const std::string &name);
};
}
