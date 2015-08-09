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
  void SetParticlePopulation(SoilAnalyzer::Sample::ParticleVector_t *particlePopulation);

signals:
  void particleChanged(int newValue);

public slots:
  void setSelectedParticle(int newValue);

private slots:
  void on_horizontalSlider_valueChanged(int value);

private:
  Ui::QParticleDisplay *ui;
  SoilAnalyzer::Sample::ParticleVector_t *ParticlePopulation;
};
