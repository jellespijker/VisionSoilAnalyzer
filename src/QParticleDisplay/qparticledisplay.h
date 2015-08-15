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
  QImage ConvertParticleToQImage(SoilAnalyzer::Particle *particle);
  bool dontDoIt = false;
};
