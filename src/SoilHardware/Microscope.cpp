/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "Microscope.h"

namespace Hardware {

Microscope::Microscope() {
  RunEnv = GetCurrentArchitecture();
  AvailableCams = GetAvailableCams();
}

Microscope::~Microscope() { delete cap; }

Microscope::Arch Microscope::GetCurrentArchitecture() {
  struct utsname unameData;
  Arch retVal;
  uname(&unameData);
  std::string archString = static_cast<std::string>(unameData.machine);
  if (archString.find("armv7l") != string::npos) {
    retVal = Arch::ARM;
  } else {
    retVal = Arch::X64;
  }
  return retVal;
}

std::vector<Microscope::Cam_t> Microscope::GetAvailableCams() {
  const string path_ss = "/sys/class/video4linux";
  const string path_ss_dev = "/dev/video";
  std::vector<Cam_t> retVal;

  if (!boost::filesystem::exists("/sys/class/video4linux/video0")) {
    throw Exception::MicroscopeException(EXCEPTION_NOCAMS, EXCEPTION_NOCAMS_NR);
  }

  for (boost::filesystem::directory_iterator itr(path_ss);
       itr != boost::filesystem::directory_iterator(); ++itr) {
    string videoln = itr->path().string();
    videoln.append("/name");
    if (boost::filesystem::exists(videoln)) {
      Cam_t currentCam;
      std::ifstream camName;
      camName.open(videoln);
      std::getline(camName, currentCam.Name);
      camName.close();
      currentCam.ID =
          std::atoi(itr->path().string().substr(28, std::string::npos).c_str());

      // Open Cam
      string camdev = path_ss_dev + std::to_string(currentCam.ID);
      if ((fd = open(camdev.c_str(), O_RDWR)) == -1) {
        throw Exception::MicroscopeException(EXCEPTION_NOCAMS,
                                             EXCEPTION_NOCAMS_NR);
      }

      // Get controls
      memset(&queryctrl, 0, sizeof(queryctrl));

      for (queryctrl.id = V4L2_CID_BASE; queryctrl.id < V4L2_CID_LASTP1;
           queryctrl.id++) {
        if (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
          if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
            continue;
          }
          currentCam.ctrls.push_back(queryctrl);
          int value;
          controlctrl.id = queryctrl.id;
          if (0 == ioctl(fd, VIDIOC_G_CTRL, &controlctrl)) {
            value = controlctrl.value;
          } else {
            throw Exception::MicroscopeException(EXCEPTION_QUERY,
                                                 EXCEPTION_QUERY_NR);
          }
          // Get control value
          std::pair<string, int> currentControl((char *)queryctrl.name, value);
          currentCam.Controls.push_back(currentControl);
          if (queryctrl.type == V4L2_CTRL_TYPE_MENU) {
            enumerate_menu(currentCam);
          }
        } else {
          if (errno == EINVAL) {
            break;
          }
          throw Exception::MicroscopeException(EXCEPTION_QUERY,
                                               EXCEPTION_QUERY_NR);
        }
      }

      for (queryctrl.id = V4L2_CID_PRIVATE_BASE;; queryctrl.id++) {
        if (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
          if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
            continue;
          }
          currentCam.ctrls.push_back(queryctrl);
          int value;
          controlctrl.id = queryctrl.id;
          if (0 == ioctl(fd, VIDIOC_G_CTRL, &controlctrl)) {
            value = controlctrl.value;
          } else {
            throw Exception::MicroscopeException(EXCEPTION_QUERY,
                                                 EXCEPTION_QUERY_NR);
          }
          // Get control value
          std::pair<string, int> currentControl((char *)queryctrl.name, value);
          currentCam.Controls.push_back(currentControl);
          if (queryctrl.type == V4L2_CTRL_TYPE_MENU) {
            enumerate_menu(currentCam);
          }
        } else {
          if (errno == EINVAL) {
            break;
          }
          throw Exception::MicroscopeException(EXCEPTION_QUERY,
                                               EXCEPTION_QUERY_NR);
        }
      }

      // Get image formats
      uint32_t width[5] = {640, 800, 1280, 1600, 2048};
      uint32_t height[6] = {480, 600, 960, 1200, 1536};

      // YUYV

      for (uint32_t i = 0; i < 5; i++) {
        memset(&format, 0, sizeof(format));
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        format.fmt.pix.width = width[i];
        format.fmt.pix.height = height[i];
        int ret = ioctl(fd, VIDIOC_S_FMT, &format);
        if (ret != -1 && format.fmt.pix.height == height[i] &&
            format.fmt.pix.width == width[i]) {
          Resolution_t res;
          res.Width = format.fmt.pix.width;
          res.Height = height[i];
          std::pair<PixelFormat, Resolution_t> currentRes(PixelFormat::YUYV,
                                                          res);
          currentCam.Resolutions.push_back(currentRes);
        }
      }

      // MJPEG
      for (uint32_t i = 0; i < 5; i++) {
        memset(&format, 0, sizeof(format));
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        format.fmt.pix.width = width[i];
        format.fmt.pix.height = height[i];
        int ret = ioctl(fd, VIDIOC_S_FMT, &format);
        if (ret != -1 && format.fmt.pix.height == height[i] &&
            format.fmt.pix.width == width[i]) {
          Resolution_t res;
          res.Width = format.fmt.pix.width;
          res.Height = format.fmt.pix.height;
          std::pair<PixelFormat, Resolution_t> currentRes(PixelFormat::MJPG,
                                                          res);
          currentCam.Resolutions.push_back(currentRes);
        }
      }

      currentCam.SelectedResolution =
          &currentCam.Resolutions[currentCam.Resolutions.size() - 1];

      close(fd);
      retVal.push_back(currentCam);
    }
  }

  for (uint32_t i = 0; i < retVal.size(); i++) {
    if (retVal[i].Resolutions.size() == 0) {
      retVal.erase(retVal.begin() + i);
      i--;
    }
  }

  return retVal;
}

void Microscope::enumerate_menu(Cam_t &currentCam) {
  memset(&querymenu, 0, sizeof(querymenu));
  querymenu.id = queryctrl.id;

  for (querymenu.index = queryctrl.minimum;
       querymenu.index <= queryctrl.maximum; querymenu.index++) {
    if (0 == ioctl(fd, VIDIOC_QUERYMENU, &querymenu)) {
      currentCam.menus.push_back(querymenu);
    } else {
      throw Exception::MicroscopeException(EXCEPTION_QUERY, EXCEPTION_QUERY_NR);
    }
  }
}

bool Microscope::IsOpened() { cap->isOpened(); }

void Microscope::openCam(Cam_t *cam) {
  if (cap != nullptr) {
    cap->release();
    delete cap;
  }
  SelectedCam = cam;
  cap = new cv::VideoCapture(SelectedCam->ID);
  if (!cap->isOpened()) {
    throw Exception::MicroscopeException(EXCEPTION_NOCAMS, EXCEPTION_NOCAMS_NR);
  }
  cap->set(CV_CAP_PROP_FRAME_WIDTH,
           SelectedCam->SelectedResolution->second.Width);
  cap->set(CV_CAP_PROP_FRAME_HEIGHT,
           SelectedCam->SelectedResolution->second.Height);
}

void Microscope::GetFrame(cv::Mat &dst) {
  openCam(SelectedCam);
  if (RunEnv == Arch::ARM) {
    for (uint32_t i = 0; i < 2; i++) {
      if (!cap->grab()) {
        throw Exception::CouldNotGrabImageException();
      }
      sleep(SelectedCam->delaytrigger);
    }
    cap->retrieve(dst);
  } else {
    for (uint32_t i = 0; i < 2; i++) {
      if (!cap->read(dst)) {
        throw Exception::CouldNotGrabImageException();
      }
    }
  }
}

void Microscope::GetHDRFrame(cv::Mat &dst, uint32_t noframes) {
  prog_sig(0);
  // create the brightness steps
  uint32_t minBr = SelectedCam->ctrls[0].minimum;
  uint32_t maxBr = SelectedCam->ctrls[0].maximum;
  uint32_t maxCon = SelectedCam->ctrls[1].maximum;
  uint32_t brightnessStep = (maxBr - minBr) / noframes;
  int8_t currentBrightness = cap->get(CV_CAP_PROP_BRIGHTNESS);
  int8_t currentContrast = cap->get(CV_CAP_PROP_CONTRAST);
  cap->set(CV_CAP_PROP_CONTRAST, maxCon);

  int progStep = 70 / noframes;
  cv::Mat currentImg;
  // take the shots at different brightness levels
  for (uint32_t i = 1; i <= noframes; i++) {
    cap->set(CV_CAP_PROP_BRIGHTNESS,
                      (minBr + (i * brightnessStep)));
    GetFrame(currentImg);
    HDRframes.push_back(currentImg);
    prog_sig(i * progStep);
  }

  // Set the brightness and back to the previous used level
  cap->set(CV_CAP_PROP_BRIGHTNESS, currentBrightness);
  cap->set(CV_CAP_PROP_CONTRAST, currentContrast);

  // Perform the exposure fusion
  cv::Mat fusion;
  cv::Ptr<cv::MergeMertens> merge_mertens = cv::createMergeMertens();
  merge_mertens->process(HDRframes, fusion);
  prog_sig(80);
  fusion *= 255;
  prog_sig(85);
  fusion.convertTo(dst, CV_8UC1);
  prog_sig(100);
  //fin_sig();
}

 boost::signals2::connection
 Microscope::connect_Finished(const Finished_t::slot_type &subscriber) {
  return fin_sig.connect(subscriber);
}

 boost::signals2::connection
 Microscope::connect_Progress(const Progress_t::slot_type &subscriber) {
  return prog_sig.connect(subscriber);
}
}
