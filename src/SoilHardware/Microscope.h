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

#include <stdint.h>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <fstream>
#include <fcntl.h>

#include <linux/videodev2.h>
#include <linux/v4l2-controls.h>
#include <linux/v4l2-common.h>

#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "MicroscopeNotFoundException.h"
#include "CouldNotGrabImageException.h"

namespace Hardware {
class Microscope {
private:
  struct v4l2_queryctrl queryctrl;
  struct v4l2_querymenu querymenu;
  struct v4l2_control controlctrl;
  struct v4l2_format format;

public:
  enum Arch { ARM, X64 };

  enum PixelFormat { YUYV, MJPG };

  struct Resolution_t {
    uint16_t Width = 2048;
    uint16_t Height = 1536;
  };

  struct Control_t {
    std::string name;
    int minimum;
    int maximum;
    int step;
    int default_value;
    int current_value;
    bool operator==(Control_t &rhs) {
      if (this->name.compare(rhs.name) == 0) {
          return true;
        }
      else {
          return false;
        }
    }
    bool operator!=(Control_t &rhs) {
    if (this->name.compare(rhs.name) != 0) {
        return true;
      }
    else {
        return false;
      }
    }
  };

  typedef std::vector<Control_t> Controls_t;

  struct Cam_t {
    std::string Name;
    std::string devString;
    uint32_t ID;
    std::vector<std::pair<PixelFormat, Resolution_t>> Resolutions;
    uint32_t delaytrigger = 1;
    std::pair<PixelFormat, Resolution_t> *SelectedResolution = nullptr;
    Controls_t Controls;
    std::vector<v4l2_queryctrl> ctrls;
    std::vector<v4l2_querymenu> menus;
    bool operator==(Cam_t const &rhs) {
      if (this->ID == rhs.ID || this->Name == rhs.Name) {
        return true;
      } else {
        return false;
      }
    }
    bool operator!=(Cam_t const &rhs) {
      if (this->ID != rhs.ID && this->Name != rhs.Name) {
        return true;
      } else {
        return false;
      }
    }
  };

  std::vector<Cam_t> AvailableCams;
  Cam_t *SelectedCam = nullptr;
  Arch RunEnv;

  typedef boost::signals2::signal<void()> Finished_t;
  typedef boost::signals2::signal<void(int)> Progress_t;

  boost::signals2::connection
  connect_Finished(const Finished_t::slot_type &subscriber);
  boost::signals2::connection
  connect_Progress(const Progress_t::slot_type &subscriber);

  Microscope();
  Microscope(const Microscope &rhs);

  ~Microscope();

  Microscope operator=(Microscope const &rhs);

  bool IsOpened();
  bool openCam(Cam_t *cam);
  bool openCam(int &cam);
  bool openCam(std::string &cam);

  bool closeCam(Cam_t *cam);

  void GetFrame(cv::Mat &dst);
  void GetHDRFrame(cv::Mat &dst, uint32_t noframes = 3);

  Control_t *GetControl(const std::string name);

  Finished_t fin_sig;
  Progress_t prog_sig;

private:
  cv::VideoCapture *cap = nullptr;

  std::vector<cv::Mat> HDRframes;

  std::vector<Cam_t> GetAvailableCams();
  Arch GetCurrentArchitecture();
  int fd;
};
}
