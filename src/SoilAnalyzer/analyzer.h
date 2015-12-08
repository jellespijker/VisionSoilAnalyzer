/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#define STARTING_ESTIMATE_PROGRESS 300
#ifndef DEBUG
//#define DEBUG
#endif

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <cmath>

#include "sample.h"
#include "soilsettings.h"
#include "soilanalyzerexception.h"

#include "SoilMath.h"

#include <QtCore/QObject>
#include <QThread>
#include <QtConcurrent>

#include "Vision.h"

namespace SoilAnalyzer {
class Analyzer : public QObject {
  Q_OBJECT

public:
  struct ExportData_t {
    int ID;
    float Radius;
    int Area;
    float Sphericity;
    float SIfactor;
    cv::Mat Img;
  };
  typedef std::vector<ExportData_t> ExportParticles_t;

  bool PredictShape = true;
  float CurrentSIfactor = 0.003;
  bool SIfactorDet = false;
  struct Image_t {
    cv::Mat FrontLight;
    cv::Mat BackLight;
    float SIPixelFactor = 0.003;
  }; /*!< */

  typedef std::vector<Image_t> Images_t; /*!< */
  Images_t *Snapshots = nullptr;         /*!< */
  SoilSettings *Settings = nullptr;      /*!< */

  Sample *Results; /*!< */

  Analyzer(Images_t *snapshots, Sample *results, SoilSettings *settings);

  void Analyse();
  void Analyse(Images_t *snapshots, Sample *results, SoilSettings *settings);
  ExportParticles_t Export();

  float CalibrateSI(float SI, cv::Mat &img);

  uint32_t MaxProgress = STARTING_ESTIMATE_PROGRESS; /*!< */

  SoilMath::NN NeuralNet; /*!< */

signals:
  void on_progressUpdate(int value);    /*!< */
  void on_maxProgressUpdate(int value); /*!< */
  void on_AnalysisFinished();           /*!< */

private:
  uint32_t currentProgress = 0;   /*!< */
  uint32_t currentParticleID = 0; /*!< */
  double BinRanges[15]{0.0,  0.038, 0.045, 0.063, 0.075, 0.09, 0.125, 0.18,
                       0.25, 0.355, 0.5,   0.71,  1.0,   1.4,  2.0};

  SoilMath::FFT fft; /*!< */

  void CalcMaxProgress();
  void CalcMaxProgressAnalyze();
  void PrepImages();
  void GetBW(std::vector<cv::Mat> &images, std::vector<cv::Mat> &BWvector);
  void GetBW(cv::Mat &img, cv::Mat &BW);

  void GetEnhancedInt(Images_t *snapshots,
                      std::vector<cv::Mat> &intensityVector);
  void GetEnhancedInt(cv::Mat &img, cv::Mat &intensity);

  void GetParticles(std::vector<cv::Mat> &BW, Images_t *snapshots,
                    Particle::ParticleVector_t &partPopulation);
  void GetParticlesFromBlobList(Vision::Segment::BlobList_t &bloblist,
                                Image_t *snapshot,
                                Particle::ParticleVector_t &partPopulation);

  void CleanUpMatVector(std::vector<cv::Mat> &mv);
  void CleanUpMatVector(Images_t *mv);

  void GetFFD(Particle::ParticleVector_t &particalPopulation);

  void GetPrediction(Particle::ParticleVector_t &particlePopulation);
};
}
