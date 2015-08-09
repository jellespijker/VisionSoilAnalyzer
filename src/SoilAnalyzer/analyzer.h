/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#define STARTING_ESTIMATE_PROGRESS 300

#include <opencv2/core.hpp>
#include <vector>

#include <boost/bind.hpp>

#include "sample.h"
#include "soilsettings.h"
#include "soilanalyzerexception.h"

#include "SoilMath.h"

//#include <QtCore/qglobal.h>
#include <QtCore/QObject>

namespace SoilAnalyzer {
class Analyzer : public QObject {
  Q_OBJECT

public:
  struct Image_t {
    cv::Mat FrontLight;
    cv::Mat BackLight;
    float SIPixelFactor = 1.;
  }; /*!< */

  typedef std::vector<Image_t> Images_t; /*!< */
  Images_t *Snapshots = nullptr;         /*!< */
  SoilSettings *Settings = nullptr;      /*!< */

  Sample *Results; /*!< */

  Analyzer(Images_t *snapshots, Sample *results, SoilSettings *settings);

  void Analyse();
  uint32_t MaxProgress = STARTING_ESTIMATE_PROGRESS; /*!< */

signals:
  void on_progressUpdate(int value);    /*!< */
  void on_maxProgressUpdate(int value); /*!< */
  void on_AnalysisFinished();  /*!< */

private:
  uint32_t currentProgress = 0;   /*!< */
  uint32_t currentParticleID = 0; /*!< */

  SoilMath::NN NeuralNet; /*!< */
  SoilMath::FFT fft;      /*!< */

  void CalcMaxProgress();
  void CalcMaxProgressAnalyze();
  void PrepImages();
  void GetBW(std::vector<cv::Mat> &images, std::vector<cv::Mat> &BWvector);
  void GetBW(cv::Mat &img, cv::Mat &BW);

  void GetEnhancedInt(Images_t *snapshots,
                      std::vector<cv::Mat> intensityVector);
  void GetEnhancedInt(cv::Mat &img, cv::Mat &intensity);

  void GetParticles(std::vector<cv::Mat> &BW, Images_t *snapshots,
                    Sample::ParticleVector_t &partPopulation);
  void GetParticlesFromBlobList(Vision::Segment::BlobList_t &bloblist,
                                Image_t *snapshot, cv::Mat &edge,
                                Sample::ParticleVector_t &partPopulation);

  void CleanUpMatVector(std::vector<cv::Mat> &mv);
  void CleanUpMatVector(Images_t *mv);

  void GetFFD(Sample::ParticleVector_t &particalPopulation);

  void GetPrediction(Sample::ParticleVector_t &particlePopulation);
};
}
