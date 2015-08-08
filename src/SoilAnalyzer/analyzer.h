/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include <opencv2/core.hpp>
#include <vector>

#include <boost/bind.hpp>

#include "sample.h"
#include "soilsettings.h"
#include "soilanalyzerexception.h"

#include "SoilMath.h"

#include <QtCore/qglobal.h>
#include <QtCore/QObject>

namespace SoilAnalyzer {
class Analyzer : public QObject {
  Q_OBJECT

public:
  struct Image_t {
    cv::Mat FrontLight;
    cv::Mat BackLight;
  };

  typedef std::vector<Image_t> Images_t;
  Images_t *Snapshots = nullptr;
  SoilSettings *Settings = nullptr;

  Sample *Results;

  Analyzer(Images_t *snapshots, Sample *results, SoilSettings *settings);

  void Analyse();
  uint32_t MaxProgress = 100;

signals:
  void on_progressUpdate(uint32_t value);
  void on_maxProgressUpdate(uint32_t value);
  void on_AnalysisFinished(Sample *result);

private:
  uint32_t currentProgress = 0;

  SoilMath::NN NeuralNet;
  SoilMath::FFT fft;

  void CalcMaxProgress();
  void PrepImages();
  void GetBW(std::vector<cv::Mat> &images, std::vector<cv::Mat> &BWvector);
  void GetBW(cv::Mat &img, cv::Mat &BW);

  void GetEnhancedInt(Images_t *snapshots,
                      std::vector<cv::Mat> intensityVector);
  void GetEnhancedInt(cv::Mat &img, cv::Mat &intensity);

  void GetParticles(std::vector<cv::Mat> &BW,
                    Sample::ParticleVector_t &partPopulation);

  void CleanUpMatVector(std::vector<cv::Mat> &mv);
  void CleanUpMatVector(Images_t *mv);
};
}
