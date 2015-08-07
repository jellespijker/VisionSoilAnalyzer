/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "Sample.h"

namespace SoilAnalyzer {
Sample::Sample(SoilSettings *settings) { Settings = settings; }

Sample::Sample(const Mat &src, SoilSettings *settings) {
  Settings = settings;
  OriginalImage = src.clone();
}

Sample::Sample(const Mat &src, const Mat &back_src, SoilSettings *settings) {
  Settings = settings;
  OriginalImage = src.clone();
  ProjectedImage = back_src.clone();
}

Sample::~Sample() {}

void Sample::Save(string &filename) {
  std::ofstream ofs(filename.c_str());
  boost::archive::binary_oarchive oa(ofs);
  oa << boost::serialization::make_nvp("SoilSample", *this);
}

void Sample::Load(string &filename) {
  std::ifstream ifs(filename.c_str());
  boost::archive::binary_iarchive ia(ifs);
  ia >> boost::serialization::make_nvp("SoilSample", *this);
}

void Sample::PrepImg(SoilSettings *settings) {
  // setup the settings
  if (settings == nullptr && Settings == nullptr) {
    Settings = new SoilSettings;
  } else {
    Settings = settings;
  }

  // error checking
  if (OriginalImage.empty()) {
    throw Exception::AnalysisException(EXCEPTION_NO_FORE_IMAGE_FOUND,
                                       EXCEPTION_NO_FORE_IMAGE_FOUND_NR);
  }
  if (Settings->useBacklightProjection && ProjectedImage.empty()) {
    throw Exception::AnalysisException(EXCEPTION_NO_PROJECTED_IMAGE_FOUND,
                                       EXCEPTION_NO_PROJECTED_IMAGE_FOUND_NR);
  }

  // Setup signals
  float currentProg = 0.;
  prog_sig(currentProg, "Starting segmentation process");
  uint32_t totalsteps = 5;

  float progstep = 1. / static_cast<float>(totalsteps);

  if (Settings->useBacklightProjection) {
    cv::Mat OIntBL;
    getEnhInt(ProjectedImage, OIntBL, progstep);

    getBW(OIntBL, BW, progstep);

  } else {
    cv::Mat OIntFL;
    getEnhInt(OriginalImage, OIntFL, progstep);
    getBW(OIntFL, BW, progstep);
  }
}

void Sample::Analyse(SoilMath::NN &nn) {
  if (!imgPrepped) {
    PrepImg();
  }
  // Calculate the statistics CIE La*b*
  // vector<Mat> LABextract = Vision::ImageProcessing::extractChannel(LAB);
  // for_each(LABextract.begin(), LABextract.end(), [&](Mat &lab) {
  // Results.LAB_Stat.push_back(floatStat_t((float *)lab.data, lab.rows,
  // lab.cols)); });

  // Calculate the statistics RI
  // Results.RI_Stat = floatStat_t((float *)RI.data, RI.rows, RI.cols);

  // Segment and analyze the particles
  SegmentParticles(Vision::Segment::SegmentationType::Normal);
  // for_each(Population.begin(), Population.end(), [&](Particle &P)
  //{
  //	P.Analyze(nn);
  //});

  // Analyze the image
}

void Sample::Analyse(const Mat &src, SoilMath::NN &nn) {
  OriginalImage = src;
  Analyse(nn);
}

bool Sample::AnalysePopVect(const vector<Particle> &population,
                            AnalysisResults &results) {
  return true;
}

void Sample::SegmentParticles(Vision::Segment::SegmentationType segType) {
  Vision::Segment Segmenter(BW);

  // Get the Particlelist
  Segmenter.GetBlobList();
  Population.resize(Segmenter.BlobList.size());
  uint32_t i = 0;
  // Analyze each particle
  for_each(Population.begin(), Population.end(), [&](Particle &P) {
    P.ID = Segmenter.BlobList[i].Label;
    P.Analysis.Analyzed = false;
    P.BW = Segmenter.BlobList[i].Img.clone();
    cv::Rect ROI = Segmenter.BlobList[i].ROI;
    P.Intensity =
        Vision::Segment::CopyMat<uchar>(Intensity(ROI).clone(), P.BW, CV_8UC1);
    P.LAB = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), P.BW, CV_32FC3);
    P.RGB = Vision::Segment::CopyMat<uchar>(LAB(ROI).clone(), P.BW, CV_8UC3);
    P.RI = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), P.BW, CV_32FC1);
    P.Edge = Vision::Segment::CopyMat<uchar>(Edge(ROI).clone(), P.BW, CV_8UC1);
    i++;
  });
}

void Sample::getEnhInt(cv::Mat &src, cv::Mat &dst, float &progstep) {
  Vision::Conversion IntConvertor(src);
  IntConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::Intensity);
  SHOW_DEBUG_IMG(IntConvertor.ProcessedImg, uchar, 255, "RGB 2 Int", false);

  if (Settings->useBlur) {
    Vision::Enhance IntBlur(IntConvertor.ProcessedImg.clone());
    IntBlur.Blur(Settings->blurKernelSize);
    uint32_t HBK = Settings->blurKernelSize / 2;
    uint32_t BK = Settings->blurKernelSize - 1;
    if (Settings->useAdaptiveContrast) {
      Vision::Enhance IntAdaptContrast(
          IntBlur.ProcessedImg(
                      cv::Rect(HBK, HBK, IntBlur.ProcessedImg.cols - BK,
                               IntBlur.ProcessedImg.rows - BK)).clone());
      IntAdaptContrast.AdaptiveContrastStretch(
          Settings->adaptContrastKernelSize,
          Settings->adaptContrastKernelFactor);
      uint32_t HAK = Settings->adaptContrastKernelSize / 2;
      uint32_t AK = Settings->adaptContrastKernelSize - 1;
      dst = IntAdaptContrast.ProcessedImg(
          cv::Rect(HAK, HAK, IntAdaptContrast.ProcessedImg.cols - AK,
                   IntAdaptContrast.ProcessedImg.rows - AK));
    } else {
      dst = IntBlur.ProcessedImg(cv::Rect(HBK, HBK,
                                          IntBlur.ProcessedImg.cols - BK,
                                          IntBlur.ProcessedImg.rows - BK));
    }
  } else if (Settings->useAdaptiveContrast) {
    Vision::Enhance IntAdaptContrast(IntConvertor.ProcessedImg.clone());
    IntAdaptContrast.AdaptiveContrastStretch(
        Settings->adaptContrastKernelSize, Settings->adaptContrastKernelFactor);
    uint32_t HAK = Settings->adaptContrastKernelSize / 2;
    uint32_t AK = Settings->adaptContrastKernelSize - 1;
    dst = IntAdaptContrast.ProcessedImg(
        cv::Rect(HAK, HAK, IntAdaptContrast.ProcessedImg.cols - AK,
                 IntAdaptContrast.ProcessedImg.rows - AK));
  } else {
    dst = IntConvertor.ProcessedImg;
  }
  SHOW_DEBUG_IMG(dst, uchar, 255, "Enhanced Int", false);
}

void Sample::getBW(cv::Mat &src, cv::Mat &dst, float &progstep) {
  Vision::Segment SegBL(src.clone());
  SegBL.sigma = Settings->sigmaFactor;
  SegBL.thresholdOffset = Settings->thresholdOffsetValue;
  SegBL.ConvertToBW(Settings->typeOfObjectsSegmented);

  SHOW_DEBUG_IMG(SegBL.ProcessedImg, uchar, 255, "Segment", true);

  cv::Mat BWholes;
  if (Settings->fillHoles) {
    Vision::Segment Fillholes(SegBL.ProcessedImg);
    Fillholes.FillHoles();
    BWholes = Fillholes.ProcessedImg;
    SHOW_DEBUG_IMG(BWholes, uchar, 255, "Fillholes", true);
  } else {
    BWholes = SegBL.ProcessedImg;
  }

  cv::Mat BWborder;
  if (Settings->ignorePartialBorderParticles) {
    Vision::Segment RemoveBB(BWholes.clone());
    RemoveBB.RemoveBorderBlobs();
    BWborder = RemoveBB.ProcessedImg;
    SHOW_DEBUG_IMG(BWborder, uchar, 255, "RemoveBorderBlobs", true);
  } else {
    BWborder = BWholes;
  }

  if (Settings->morphFilterType != Vision::MorphologicalFilter::NONE) {
    Vision::MorphologicalFilter Morph(BWborder.clone());
    cv::Mat kernel = cv::Mat::zeros(Settings->filterMaskSize,
                                    Settings->filterMaskSize, CV_8UC1);
    uint32_t hMaskSize = Settings->filterMaskSize / 2;
    cv::circle(kernel, cv::Point(hMaskSize, hMaskSize), hMaskSize + 1, 1, -1);
    switch (Settings->morphFilterType) {
    case Vision::MorphologicalFilter::CLOSE:
      Morph.Close(kernel);
      break;
    case Vision::MorphologicalFilter::OPEN:
      Morph.Open(kernel);
      break;
    case Vision::MorphologicalFilter::DILATE:
      Morph.Dilation(kernel);
      break;
    case Vision::MorphologicalFilter::ERODE:
      Morph.Erosion(kernel);
      break;
    }
    dst = Morph.ProcessedImg;
    SHOW_DEBUG_IMG(dst, uchar, 255, "Morphological operation", true);
  } else {
    dst = BWholes;
  }
}

boost::signals2::connection
Sample::connect_Progress(const Progress_t::slot_type &subscriber) {
  return prog_sig.connect(subscriber);
}
}
