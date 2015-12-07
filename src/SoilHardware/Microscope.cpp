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
  connect(this, SIGNAL(imageretrieved()), this, SLOT(on_imageretrieved()));
}

Microscope::Microscope(const Microscope &rhs) {
  std::copy(rhs.AvailableCams.begin(), rhs.AvailableCams.end(),
            this->AvailableCams.begin());
  this->RunEnv = rhs.RunEnv;
  this->SelectedCam = rhs.SelectedCam;
  this->cap = rhs.cap;
  this->fd = rhs.fd;
  this->HDRframes = rhs.HDRframes;
  connect(this, SIGNAL(imageretrieved()), this, SLOT(on_imageretrieved()));
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

      uint32_t ResolutionID = 0;
      getResolutions(currentCam, V4L2_PIX_FMT_YUYV, ResolutionID);
      getResolutions(currentCam, V4L2_PIX_FMT_MJPEG, ResolutionID);
      getResolutions(currentCam, V4L2_PIX_FMT_GREY, ResolutionID);
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

void Microscope::getResolutions(Cam_t &currentCam, int FormatType,
                                uint32_t &ResolutionID) {
  // Get image formats
  struct v4l2_format format;
  memset(&format, 0, sizeof(format));

  uint32_t width[10] = {640,  800,  1280, 1280, 1920,
                        1600, 2048, 2560, 3840, 3872};
  uint32_t height[10] = {480,  600,  720,  960,  1080,
                         1200, 1536, 1440, 2160, 2764};

  for (uint32_t i = 0; i < 10; i++) {
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = FormatType;
    format.fmt.pix.width = width[i];
    format.fmt.pix.height = height[i];
    int ret = ioctl(currentCam.fd, VIDIOC_S_FMT, &format);
    if (ret != -1 && format.fmt.pix.height == height[i] &&
        format.fmt.pix.width == width[i]) {
      Resolution_t res;
      res.Width = format.fmt.pix.width;
      res.Height = height[i];
      res.ID = ResolutionID++;
      switch (FormatType) {
      case V4L2_PIX_FMT_YUYV:
        res.format = PixelFormat::YUYV;
        break;
      case V4L2_PIX_FMT_MJPEG:
        res.format = PixelFormat::MJPG;
        break;
      case V4L2_PIX_FMT_GREY:
        res.format = PixelFormat::GREY;
        break;
      default:
        break;
      }
      currentCam.Resolutions.push_back(res);
    }
  }
}

bool Microscope::IsOpened() { return openedUptheCam; }

bool Microscope::openCam(Cam_t *cam) {
  for (uint32_t i = 0; i < AvailableCams.size(); i++) {
    if (AvailableCams[i] == *cam) {
      closeCam(SelectedCam);
      SelectedCam = cam;
      for (Controls_t::iterator it = SelectedCam->Controls.begin();
           it != SelectedCam->Controls.end(); ++it) {
        SetControl(&*it);
      }

      SelectedCam->Pipe.currentMicroscope = this;
      gst_init(NULL, NULL);

      SelectedCam->Pipe.pipeline = gst_pipeline_new("SoilCam");
      if (!SelectedCam->Pipe.pipeline) {
        throw Exception::MicroscopeException(
            EXCEPTION_GSTREAM_INIT_EXCEPTION,
            EXCEPTION_GSTREAM_INIT_EXCEPTION_NR);
      }

      SelectedCam->Pipe.source = gst_element_factory_make("v4l2src", "source");
      SelectedCam->Pipe.capsfilter =
          gst_element_factory_make("capsfilter", "filter");
      SelectedCam->Pipe.colorspace =
          gst_element_factory_make("ffmpegcolorspace", "colorspace");
      SelectedCam->Pipe.convert =
          gst_element_factory_make("capsfilter", "convert");
      SelectedCam->Pipe.sink = gst_element_factory_make("appsink", "output");

      if (!SelectedCam->Pipe.source || !SelectedCam->Pipe.capsfilter ||
          !SelectedCam->Pipe.colorspace || !SelectedCam->Pipe.sink ||
          !SelectedCam->Pipe.convert) {
        throw Exception::MicroscopeException(
            EXCEPTION_GSTREAM_ELEM_EXCEPTION,
            EXCEPTION_GSTREAM_ELEM_EXCEPTION_NR);
      }

      if (SelectedCam->Name.compare("DFK 24UJ003") == 0) {
        SelectedCam->Pipe.tisvideobuffer = gst_element_factory_make(
            "tisvideobufferfilter", "tisvidebufferfilter");
        SelectedCam->Pipe.tisautoexposure =
            gst_element_factory_make("tis_auto_exposure", "tis_auto_exposure");
        SelectedCam->Pipe.tiscolorize =
            gst_element_factory_make("tiscolorize", "tiscolorize");
        SelectedCam->Pipe.queue = gst_element_factory_make("queue", "queue");
        SelectedCam->Pipe.tiswhitebalance =
            gst_element_factory_make("tiswhitebalance", "tiswhitebalance");
        SelectedCam->Pipe.queue2 = gst_element_factory_make("queue", "queue2");
        SelectedCam->Pipe.bayer =
            gst_element_factory_make("bayer2rgb", "bayer");
        if (!SelectedCam->Pipe.tisvideobuffer ||
            !SelectedCam->Pipe.tiscolorize || !SelectedCam->Pipe.queue ||
            !SelectedCam->Pipe.bayer) {
          throw Exception::MicroscopeException(
              EXCEPTION_GSTREAM_ELEM_EXCEPTION,
              EXCEPTION_GSTREAM_ELEM_EXCEPTION_NR);
        }
      }

      g_object_set(SelectedCam->Pipe.source, "device",
                   SelectedCam->devString.c_str());

      switch (SelectedCam->SelectedResolution->format) {
      case PixelFormat::MJPG:
        SelectedCam->Pipe.caps = gst_caps_new_simple(
            "video/x-raw-rgb", "width", G_TYPE_INT,
            SelectedCam->SelectedResolution->Width, "height", G_TYPE_INT,
            SelectedCam->SelectedResolution->Height, NULL);
        break;
      case PixelFormat::GREY:
        SelectedCam->Pipe.caps = gst_caps_new_simple(
            "video/x-raw-gray", "width", G_TYPE_INT,
            SelectedCam->SelectedResolution->Width, "height", G_TYPE_INT,
            SelectedCam->SelectedResolution->Height, NULL);
        break;
      case PixelFormat::YUYV:
        SelectedCam->Pipe.caps = gst_caps_new_simple(
            "video/x-raw-gray", "format", G_TYPE_STRING, "(fourcc)UYVY",
            "width", G_TYPE_INT, SelectedCam->SelectedResolution->Width,
            "height", G_TYPE_INT, SelectedCam->SelectedResolution->Height,
            NULL);
      default:
        break;
      }
      g_object_set(SelectedCam->Pipe.capsfilter, "caps", SelectedCam->Pipe.caps,
                   NULL);
      gst_caps_unref(SelectedCam->Pipe.caps);
      SelectedCam->Pipe.caps = gst_caps_new_simple(
          "video/x-raw-rgb", "width", G_TYPE_INT,
          SelectedCam->SelectedResolution->Width, "height", G_TYPE_INT,
          SelectedCam->SelectedResolution->Height, NULL);
      g_object_set(SelectedCam->Pipe.convert, "caps", SelectedCam->Pipe.caps,
                   NULL);

      SelectedCam->Pipe.bus = gst_element_get_bus(SelectedCam->Pipe.pipeline);
      g_object_set(SelectedCam->Pipe.sink, "emit-signals", TRUE, NULL);
      g_signal_connect(SelectedCam->Pipe.sink, "new-buffer",
                       G_CALLBACK(new_buffer), &SelectedCam->Pipe);

      if (SelectedCam->Name.compare("DFK 24UJ003") == 0) {
        gst_bin_add_many(
            GST_BIN(SelectedCam->Pipe.pipeline), SelectedCam->Pipe.source,
            SelectedCam->Pipe.capsfilter, SelectedCam->Pipe.tisvideobuffer,
            SelectedCam->Pipe.tisautoexposure, SelectedCam->Pipe.tiscolorize,
            SelectedCam->Pipe.queue, SelectedCam->Pipe.tiswhitebalance,
            SelectedCam->Pipe.queue2, SelectedCam->Pipe.bayer,
            SelectedCam->Pipe.sink, NULL);
        gst_element_link_many(
            SelectedCam->Pipe.source, SelectedCam->Pipe.capsfilter,
            SelectedCam->Pipe.tisvideobuffer, SelectedCam->Pipe.tiscolorize,
            SelectedCam->Pipe.queue, SelectedCam->Pipe.bayer,
            SelectedCam->Pipe.sink, NULL);
      } else {
        gst_bin_add_many(
            GST_BIN(SelectedCam->Pipe.pipeline), SelectedCam->Pipe.source,
            SelectedCam->Pipe.capsfilter, SelectedCam->Pipe.colorspace,
            SelectedCam->Pipe.convert, SelectedCam->Pipe.sink, NULL);
        gst_element_link_many(
            SelectedCam->Pipe.source, SelectedCam->Pipe.capsfilter,
            SelectedCam->Pipe.colorspace, SelectedCam->Pipe.convert,
            SelectedCam->Pipe.sink, NULL);
      }
      openedUptheCam = true;
      return true;
    }
  }
  openedUptheCam = false;
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
  throw Exception::MicroscopeException(EXCEPTION_OPENCAM, EXCEPTION_OPENCAM_NR);
  return nullptr;
}

bool Microscope::closeCam(Cam_t *cam) {
  if (openedUptheCam) {
    gst_element_set_state(cam->Pipe.pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(cam->Pipe.pipeline));
    openedUptheCam = false;
    lastFrame = cv::Mat();
  }
}

void Microscope::GetFrame(cv::Mat &dst) {
  if (!IsOpened()) {
    openCam(SelectedCam);
  }

  QEventLoop loop;
  loop.connect(this, SIGNAL(imageretrieved()), SLOT(quit()));
  gst_element_set_state(SelectedCam->Pipe.pipeline, GST_STATE_PLAYING);
  loop.exec();
  dst = lastFrame;
  closeCam(SelectedCam);
}

void Microscope::on_imageretrieved() { return; }

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

void Microscope::SendImageRetrieved() { emit imageretrieved(); }

void Microscope::new_buffer(GstElement *sink, CustomData *data) {
  GstBuffer *buffer;
  g_signal_emit_by_name(sink, "pull-buffer", &buffer);
  if (buffer) {
    cv::Mat bufferMat(
        data->currentMicroscope->SelectedCam->SelectedResolution->Height,
        data->currentMicroscope->SelectedCam->SelectedResolution->Width,
        CV_8UC4, (uchar *)buffer->data);
    std::vector<cv::Mat> chans;
    cv::split(bufferMat, chans);
    chans.erase(chans.begin() + 4);
    cv::merge(chans, data->currentMicroscope->lastFrame);
    // cv::namedWindow("test");
    // cv::imshow("test", data->currentMicroscope->lastFrame);
    // cv::waitKey(0);

    data->currentMicroscope->SendImageRetrieved();
    //      gst_element_set_state(data->currentMicroscope->SelectedCam->Pipe.pipeline,
    //                          GST_STATE_PAUSED);
    gst_buffer_unref(buffer);
  }
}
}
