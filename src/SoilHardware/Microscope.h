/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class Microscope
Interaction with the microscope
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

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <QtCore/QObject>
#include <QEventLoop>
#include <QDebug>

#include "MicroscopeNotFoundException.h"
#include "CouldNotGrabImageException.h"

namespace Hardware {
class Microscope : public QObject {
  Q_OBJECT

public:
  enum Arch { ARM, X64 };

  enum PixelFormat { YUYV, MJPG, GREY };

  struct Resolution_t {
    uint16_t Width = 2048;
    uint16_t Height = 1536;
    PixelFormat format = PixelFormat::MJPG;
    std::string to_string() {
      std::string retVal = std::to_string(Width);
      retVal.append(" x ");
      retVal.append(std::to_string(Height));
      if (format == PixelFormat::MJPG) {
          retVal.append(" - MJPG");
        }
      else if (format == PixelFormat::YUYV){
          retVal.append(" - YUYV");
        }
      else {
          retVal.append(" - GREY");
        }
      return retVal;
    }
    uint32_t ID;
  };

  struct Control_t {
    std::string name;
    int minimum;
    int maximum;
    int step;
    int default_value;
    int current_value;
    uint32_t ID = V4L2_CID_BASE;
    bool operator==(Control_t &rhs) {
      if (this->name.compare(rhs.name) == 0) {
        return true;
      } else {
        return false;
      }
    }
    bool operator!=(Control_t &rhs) {
      if (this->name.compare(rhs.name) != 0) {
        return true;
      } else {
        return false;
      }
    }
  };

  typedef std::vector<Control_t> Controls_t;

  typedef struct _CustomData {
  GMainLoop *main_loop;
  GstElement *pipeline;
  GstElement *source;
  GstElement *capsfilter;
  GstElement *tisvideobuffer;
  GstElement *tiscolorize;
  GstElement *bayer;
  GstElement *queue;
  GstElement *colorspace;
  GstElement *convert;
  GstElement *sink;
  GstBus *bus;
  GstCaps *caps;
  Hardware::Microscope *currentMicroscope;
  } CustomData;

  struct Cam_t {
    std::string Name;
    std::string devString;
    uint32_t ID;
    std::vector<Resolution_t> Resolutions;
    uint32_t delaytrigger = 1;
    Resolution_t *SelectedResolution = nullptr;
    Controls_t Controls;
    CustomData Pipe;
    int fd;
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
  void GetGstreamFrame(cv::Mat &dst);
  void GetHDRFrame(cv::Mat &dst, uint32_t noframes = 3);

  Control_t *GetControl(const std::string name);
  void SetControl(Control_t *control);

  Cam_t *FindCam(std::string cam);
  Cam_t *FindCam(int cam);
  cv::Mat lastFrame;

  void SendImageRetrieved();

public slots:
  void on_imageretrieved();

signals:
  void imageretrieved();

private:
  static void new_buffer(GstElement *sink, CustomData *data);
  void getResolutions(Cam_t &currentCam, int FormatType);

  cv::VideoCapture *cap = nullptr;

  std::vector<cv::Mat> HDRframes;

  std::vector<Cam_t> GetAvailableCams();
  Arch GetCurrentArchitecture();
  int fd;
};
}
