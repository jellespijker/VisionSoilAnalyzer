/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "analyzer.h"

namespace SoilAnalyzer {

Analyzer::Analyzer(Images_t *snapshots, Sample *results, SoilSettings *settings = nullptr) {
  this->Snapshots = snapshots;
  this->Results = results;
  if (settings == nullptr) {
      Settings = new SoilSettings;
    }
  else {
      this->Settings = settings;
    }
}

void Analyzer::PrepImages() {
  if (Snapshots == nullptr || Snapshots->size() == 0) {
      throw Exception::SoilAnalyzerException(EXCEPTION_NO_SNAPSHOTS,
                                             EXCEPTION_NO_SNAPSHOTS_NR);
    }
  
  std::vector<cv::Mat> intensityVector;
  GetEnhancedInt(Snapshots, intensityVector);

  std::vector<cv::Mat> BWVector;
  GetBW(intensityVector, BWVector);
  CleanUpMatVector(intensityVector);

  GetParticles(BWVector, Results->ParticlePopulation);
  CleanUpMatVector(BWVector);
  CleanUpMatVector(Snapshots);

}

void Analyzer::Analyse() {}

void Analyzer::CleanUpMatVector(std::vector<Mat> &mv) {
  for_each(mv.begin(), mv.end(),[](cv::Mat &I) {
    I.release();
  });
  mv.clear();
}

void Analyzer::CleanUpMatVector(Images_t *mv) {
  for_each(mv->begin(), mv->end(),[](Image_t &I) {
    I.BackLight.release();
    I.FrontLight.release();
  });
  mv->clear();
}

void Analyzer::CalcMaxProgress() {
  if (Settings->useBlur) {
      MaxProgress = 100;
    }

  emit on_maxProgressUpdate(MaxProgress);
}

void Analyzer::GetEnhancedInt(Images_t *snapshots, std::vector<Mat> intensityVector) {
  if (Settings->useBacklightProjection) {
      for_each(snapshots->begin(), snapshots->end(),[&](Image_t &I) {
        cv::Mat intensity;
        GetEnhancedInt(I.BackLight, intensity);
        intensityVector.push_back(intensity);
      });
    }
  else {
      for_each(snapshots->begin(), snapshots->end(),[&](Image_t &I) {
        cv::Mat intensity;
        GetEnhancedInt(I.FrontLight, intensity);
        intensityVector.push_back(intensity);
      });
    }
}

void Analyzer::GetEnhancedInt(Mat &img, Mat &intensity) {
  Vision::Conversion IntConvertor(img.clone());
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
      intensity = IntAdaptContrast.ProcessedImg(
          cv::Rect(HAK, HAK, IntAdaptContrast.ProcessedImg.cols - AK,
                   IntAdaptContrast.ProcessedImg.rows - AK));
    } else {
      intensity = IntBlur.ProcessedImg(cv::Rect(HBK, HBK,
                                          IntBlur.ProcessedImg.cols - BK,
                                          IntBlur.ProcessedImg.rows - BK));
    }
  } else if (Settings->useAdaptiveContrast) {
    Vision::Enhance IntAdaptContrast(IntConvertor.ProcessedImg.clone());
    IntAdaptContrast.AdaptiveContrastStretch(
        Settings->adaptContrastKernelSize, Settings->adaptContrastKernelFactor);
    uint32_t HAK = Settings->adaptContrastKernelSize / 2;
    uint32_t AK = Settings->adaptContrastKernelSize - 1;
    intensity = IntAdaptContrast.ProcessedImg(
        cv::Rect(HAK, HAK, IntAdaptContrast.ProcessedImg.cols - AK,
                 IntAdaptContrast.ProcessedImg.rows - AK));
  } else {
    intensity = IntConvertor.ProcessedImg;
  }
  SHOW_DEBUG_IMG(intensity, uchar, 255, "Enhanced Int", false);
}

void Analyzer::GetBW(std::vector<cv::Mat> &images, std::vector<cv::Mat> &BWvector) {
    for_each(images.begin(), images.end(), [&](cv::Mat &I) {
      cv::Mat BW;
      GetBW(I, BW);
  });
}

void Analyzer::GetBW(cv::Mat &img, cv::Mat &BW) {
  Vision::Segment SegBL(img.clone());
  SegBL.sigma = Settings->sigmaFactor;
  SegBL.thresholdOffset = Settings->thresholdOffsetValue;
  SegBL.ConvertToBW(Settings->typeOfObjectsSegmented);
  emit on_progressUpdate(currentProgress++);
  SHOW_DEBUG_IMG(SegBL.ProcessedImg, uchar, 255, "Segment", true);

  cv::Mat BWholes;
  if (Settings->fillHoles) {
    Vision::Segment Fillholes(SegBL.ProcessedImg);
    Fillholes.FillHoles();
    BWholes = Fillholes.ProcessedImg;
    emit on_progressUpdate(currentProgress++);
    SHOW_DEBUG_IMG(BWholes, uchar, 255, "Fillholes", true);
  } else {
    BWholes = SegBL.ProcessedImg;
  }

  cv::Mat BWborder;
  if (Settings->ignorePartialBorderParticles) {
    Vision::Segment RemoveBB(BWholes.clone());
    RemoveBB.RemoveBorderBlobs();
    BWborder = RemoveBB.ProcessedImg;
    emit on_progressUpdate(currentProgress++);
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
   case Vision::MorphologicalFilter::NONE:
      Morph.ProcessedImg = Morph.OriginalImg;
      break;
    }
    BW = Morph.ProcessedImg;
    emit on_progressUpdate(currentProgress++);
    SHOW_DEBUG_IMG(BW, uchar, 255, "Morphological operation", true);
  } else {
    BW = BWholes;
  }
}
}
