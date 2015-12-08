/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include <QWidget>
#include <QImage>
#include <qopencvqt.h>
#include <QColor>
#include <QWheelEvent>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>

#include "soilanalyzer.h"

namespace Ui {
  class QParticleDisplay;
}

class QParticleDisplay : public QWidget
{
  Q_OBJECT

public:
  explicit QParticleDisplay(QWidget *parent = 0);
  ~QParticleDisplay();
  void SetSample(SoilAnalyzer::Sample *sample);
  SoilAnalyzer::Particle *SelectedParticle;
  void wheelEvent( QWheelEvent * event );
  void next();

signals:
  void particleChanged(int newValue);
  void shapeClassificationChanged(int newValue);
  void particleDeleted();

public slots:
  void setSelectedParticle(int newValue);

private slots:
  void on_selectedParticleChangedWidget(int value);
  void on_selectedParticleChangedSlider(int value);
  void on_pushButton_delete_clicked();

private:
  Ui::QParticleDisplay *ui;
  SoilAnalyzer::Sample *Sample;
  QVector<QImage> images;
  QImage ConvertParticleToQImage(SoilAnalyzer::Particle *particle);
  bool dontDoIt = false;
};
