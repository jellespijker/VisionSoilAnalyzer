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
  for_each(AvailableCams.begin(), AvailableCams.end(), [](Cam_t &C) {
    C.SelectedResolution = &C.Resolutions[C.Resolutions.size() - 1];
  });
}

Microscope::Microscope(const Microscope &rhs) {
  std::copy(rhs.AvailableCams.begin(), rhs.AvailableCams.end(),
            this->AvailableCams.begin());
  this->RunEnv = rhs.RunEnv;
  this->SelectedCam = rhs.SelectedCam;
  this->cap = rhs.cap;
  this->fd = rhs.fd;
  this->HDRframes = rhs.HDRframes;
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
  struct v4l2_queryctrl queryctrl;
  struct v4l2_control controlctrl;

  // Check if there're videodevices installed
  // Itterate through the cams
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
      currentCam.devString = path_ss_dev + std::to_string(currentCam.ID);
      if ((currentCam.fd = open(currentCam.devString.c_str(), O_RDWR)) == -1) {
        throw Exception::MicroscopeException(EXCEPTION_NOCAMS,
                                             EXCEPTION_NOCAMS_NR);
      }

      // Get controls
      memset(&queryctrl, 0, sizeof(queryctrl));
      memset(&controlctrl, 0, sizeof(controlctrl));
      for (queryctrl.id = V4L2_CID_BASE; queryctrl.id < V4L2_CID_LASTP1;
           queryctrl.id++) {

        if (ioctl(currentCam.fd, VIDIOC_QUERYCTRL, &queryctrl) == 0) {
          if (!(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)) {
            Control_t currentControl;
            currentControl.ID = queryctrl.id;
            currentControl.name = (char *)queryctrl.name;
            currentControl.minimum = queryctrl.minimum;
            currentControl.maximum = queryctrl.maximum;
            currentControl.default_value = queryctrl.default_value;
            currentControl.step = queryctrl.step;
            controlctrl.id = queryctrl.id;
            if (ioctl(currentCam.fd, VIDIOC_G_CTRL, &controlctrl) == 0) {
              currentControl.current_value = controlctrl.value;
            }
            currentCam.Controls.push_back(currentControl);
          }
        } else {
          if (errno == EINVAL)
            continue;
          throw Exception::MicroscopeException(EXCEPTION_QUERY,
                                               EXCEPTION_QUERY_NR);
        }
      }

      // Get image formats
      struct v4l2_format format;
      memset(&format, 0, sizeof(format));

      uint32_t width[5] = {640, 800, 1280, 1600, 2048};
      uint32_t height[6] = {480, 600, 960, 1200, 1536};

      uint32_t ResolutionID = 0;

      // YUYV
      for (uint32_t i = 0; i < 5; i++) {
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        format.fmt.pix.width = width[i];
        format.fmt.pix.height = height[i];
        int ret = ioctl(currentCam.fd, VIDIOC_S_FMT, &format);
        if (ret != -1 && format.fmt.pix.height == height[i] &&
            format.fmt.pix.width == width[i]) {
          Resolution_t res;
          res.Width = format.fmt.pix.width;
          res.Height = height[i];
          res.ID = ResolutionID++;
          res.format = PixelFormat::YUYV;
          currentCam.Resolutions.push_back(res);
        }
      }

      // MJPEG
      for (uint32_t i = 0; i < 5; i++) {
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        format.fmt.pix.width = width[i];
        format.fmt.pix.height = height[i];
        int ret = ioctl(currentCam.fd, VIDIOC_S_FMT, &format);
        if (ret != -1 && format.fmt.pix.height == height[i] &&
            format.fmt.pix.width == width[i]) {
          Resolution_t res;
          res.Width = format.fmt.pix.width;
          res.Height = format.fmt.pix.height;
          res.ID = ResolutionID++;
          res.format = PixelFormat::MJPG;
          currentCam.Resolutions.push_back(res);
        }
      }

      close(currentCam.fd);
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

bool Microscope::IsOpened() {
  if (cap == nullptr) {
    return false;
  } else {
    return cap->isOpened();
  }
}

bool Microscope::openCam(Cam_t *cam) {
  for (uint32_t i = 0; i < AvailableCams.size(); i++) {
    if (AvailableCams[i] == *cam) {
      closeCam(SelectedCam);
      SelectedCam = cam;
      cap = new cv::VideoCapture(SelectedCam->ID);
      if (!cap->isOpened()) {
        throw Exception::MicroscopeException(EXCEPTION_NOCAMS,
                                             EXCEPTION_NOCAMS_NR);
      }
      cap->set(CV_CAP_PROP_FRAME_WIDTH, SelectedCam->SelectedResolution->Width);
      cap->set(CV_CAP_PROP_FRAME_HEIGHT,
               SelectedCam->SelectedResolution->Height);
      for (Controls_t::iterator it = SelectedCam->Controls.begin();
           it != SelectedCam->Controls.end(); ++it) {
        SetControl(&*it);
      }
      return true;
    }
  }
  return false;
}

bool Microscope::openCam(std::string &cam) { return openCam(FindCam(cam)); }

bool Microscope::openCam(int &cam) { return openCam(FindCam(cam)); }

Microscope::Cam_t *Microscope::FindCam(int cam) {
  for (uint32_t i = 0; i < AvailableCams.size(); i++) {
    if (cam == AvailableCams[i].ID) {
      return &AvailableCams[i];
    }
  }
  return nullptr;
}

Microscope::Cam_t *Microscope::FindCam(string cam) {
  for (uint32_t i = 0; i < AvailableCams.size(); i++) {
    if (cam.compare(AvailableCams[i].Name) == 0) {
      return &AvailableCams[i];
    }
  }
  return nullptr;
}

bool Microscope::closeCam(Cam_t *cam) {
  if (cap != nullptr) {
    if (cap->isOpened()) {
      cap->release();
    }
    delete cap;
    cap = nullptr;
  }
}

void Microscope::GetFrame(cv::Mat &dst) {
  openCam(SelectedCam);
  sleep(SelectedCam->delaytrigger);
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
  // create the brightness steps
  Control_t *brightness = GetControl("Brightness");
  Control_t *contrast = GetControl("Contrast");

  uint32_t brightnessStep =
      (brightness->maximum - brightness->minimum) / noframes;
  int8_t currentBrightness = brightness->current_value;
  int8_t currentContrast = contrast->current_value;
  contrast->current_value = contrast->maximum;

  cv::Mat currentImg;
  // take the shots at different brightness levels
  for (uint32_t i = 1; i <= noframes; i++) {
    brightness->current_value = brightness->minimum + (i * brightnessStep);
    GetFrame(currentImg);
    HDRframes.push_back(currentImg);
  }

  // Set the brightness and back to the previous used level
  brightness->current_value = currentBrightness;
  contrast->current_value = currentContrast;

  // Perform the exposure fusion
  cv::Mat fusion;
  cv::Ptr<cv::MergeMertens> merge_mertens = cv::createMergeMertens();
  merge_mertens->process(HDRframes, fusion);
  fusion *= 255;
  fusion.convertTo(dst, CV_8UC1);
}

Microscope::Control_t *Microscope::GetControl(const string name) {
  for (Controls_t::iterator it = SelectedCam->Controls.begin();
       it != SelectedCam->Controls.end(); ++it) {
    if (name.compare(it->name) == 0) {
      return &*it;
    }
  }
  return nullptr;
}

void Microscope::SetControl(Control_t *control) {
  if ((SelectedCam->fd = open(SelectedCam->devString.c_str(), O_RDWR)) == -1) {
    throw Exception::MicroscopeException(EXCEPTION_NOCAMS, EXCEPTION_NOCAMS_NR);
  }

  struct v4l2_queryctrl queryctrl;
  struct v4l2_control controlctrl;

  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = control->ID;
  if (ioctl(SelectedCam->fd, VIDIOC_QUERYCTRL, &queryctrl) == -1) {
    if (errno != EINVAL) {
      close(SelectedCam->fd);
      throw Exception::MicroscopeException(EXCEPTION_QUERY, EXCEPTION_QUERY_NR);
    } else {
      close(SelectedCam->fd);
      throw Exception::MicroscopeException(EXCEPTION_CTRL_NOT_FOUND,
                                           EXCEPTION_CTRL_NOT_FOUND_NR);
    }
  } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    close(SelectedCam->fd);
    throw Exception::MicroscopeException(EXCEPTION_CTRL_NOT_FOUND,
                                         EXCEPTION_CTRL_NOT_FOUND_NR);
  } else {
    memset(&controlctrl, 0, sizeof(controlctrl));
    controlctrl.id = control->ID;
    controlctrl.value = control->current_value;

    if (ioctl(SelectedCam->fd, VIDIOC_S_CTRL, &controlctrl) == -1) {
      // Fails on auto white balance
      // throw Exception::MicroscopeException(EXCEPTION_CTRL_VALUE,
      //                                     EXCEPTION_CTRL_VALUE_NR);
    }
  }
  close(SelectedCam->fd);
}
}
