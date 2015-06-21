/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#ifndef VSAGUI_H
#define VSAGUI_H

//#define DEBUG

#include <string>
#include <vector>
#include <sys/utsname.h>
#include <stdlib.h>

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QErrorMessage>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QLabel>
#include <QProgressBar>
#include <QFileDialog>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/serialization.hpp>

#include "opencvqt.h"
#include "visionsettings.h"
#include "hardwaresettings.h"

#include "Hardware.h"
#include "SoilMath.h"
#include "Vision.h"
#include "VisionDebug.h"
#include "VisionSoil.h"

class QErrorMessage;

namespace Ui {
class VSAGUI;
}

class VSAGUI : public QMainWindow {
  Q_OBJECT

public:
  explicit VSAGUI(QWidget *parent = 0);
  ~VSAGUI();

private slots:

  void on_SnapshotButton_clicked();

  void on_actionSave_triggered();

  void on_actionLoad_triggered();

  void on_AnalyzeButton_clicked();

  void on_actionNew_triggered();

  void on_actionImport_triggered();

  void on_actionExport_triggered();

  void on_actionSegmentation_Settings_triggered();

  void on_actionSave_Settings_triggered();

  void on_actionLoad_Settings_triggered();

  void on_actionRestore_Default_triggered();

  void on_SegmentButton_clicked();

  void on_verticalSlider_sliderReleased();

  void on_OffsetSlider_valueChanged(int value);

  void on_OffsetSlider_sliderReleased();

  void on_actionHardware_Settings_triggered();

  void on_actionCheese_2_triggered();

  void on_actionImport_RGB_Snapshot_triggered();

private:
  Ui::VSAGUI *ui;
  QErrorMessage *errorMessageDialog;
  VisionSettings *settingWindow;
  HardwareSettings *hsetttingWindow;

  SoilAnalyzer::SoilSettings *sSettings;

  boost::signals2::connection finished_sig;
  boost::signals2::connection progress_sig;
  boost::signals2::connection visionprogress_seg;

  void on_vision_update(float prog, string statusText);

  SoilAnalyzer::Sample *SoilSample = nullptr;
  SoilMath::NN *NeuralNet = nullptr;

  cv::Mat *OrigImg;
  QProgressBar *progressBar = nullptr;
  QLabel *statusLabel = nullptr;

  bool runFromBBB = false;
  bool Segmented = false;

  void LoadSample();
  std::vector<std::string> webCams;
  void SetMatToMainView(cv::Mat &img);
  void CreateNewSoilSample();
};

#endif // VSAGUI_H
