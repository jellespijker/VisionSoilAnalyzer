/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "qparticledisplay.h"
#include "ui_qparticledisplay.h"

QParticleDisplay::QParticleDisplay(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QParticleDisplay)
{
  ui->setupUi(this);
  ui->widget->setBackgroundColor(QColor("white"));
  ui->widget->setSlideSize(QSize(250, 250));
  connect(ui->widget, SIGNAL(centerIndexChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
  connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setCenterIndex(int)));
}

QParticleDisplay::~QParticleDisplay()
{
  delete ui;
}


void QParticleDisplay::on_horizontalSlider_valueChanged(int value)
{
  emit particleChanged(value);
}

void QParticleDisplay::setSelectedParticle(int newValue) {
  ui->widget->setCenterIndex(newValue);
}

void QParticleDisplay::SetParticlePopulation(SoilAnalyzer::Sample::Particles_t *particlePopulation) {
  this->ParticlePopulation = particlePopulation;
  ui->horizontalSlider->setMaximum(this->ParticlePopulation->size());
  for (uint32_t i = 0; i < ui->widget->slideCount(); i++) {
      ui->widget->removeSlide(0);
    }

  for (uint32_t i = 0; i < this->ParticlePopulation->size(); i++) {
      ui->widget->addSlide(QOpenCVQT::Mat2QImage(this->ParticlePopulation[i].RGB));
    }
}
