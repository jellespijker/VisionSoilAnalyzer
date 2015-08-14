/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "qparticledisplay.h"
#include "ui_qparticledisplay.h"

QParticleDisplay::QParticleDisplay(QWidget *parent)
    : QWidget(parent), ui(new Ui::QParticleDisplay) {
  ui->setupUi(this);
  ui->widget->setBackgroundColor(QColor("white"));
  ui->widget->setSlideSize(QSize(200, 200));
  connect(ui->widget, SIGNAL(centerIndexChanged(int)), this, SLOT(on_selectedParticleChanged(int)));
  connect(ui->widget, SIGNAL(centerIndexChanged(int)), ui->horizontalSlider,
          SLOT(setValue(int)));

  connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_selectedParticleChanged(int)));
  connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->widget,
          SLOT(setCenterIndex(int)));
}

QParticleDisplay::~QParticleDisplay() { delete ui; }

void QParticleDisplay::on_horizontalSlider_valueChanged(int value) {
  emit particleChanged(value);
}

void QParticleDisplay::setSelectedParticle(int newValue) {
  ui->widget->setCenterIndex(newValue);
}

void QParticleDisplay::SetParticlePopulation(
    SoilAnalyzer::Sample::ParticleVector_t *particlePopulation) {
  this->ParticlePopulation = particlePopulation;
  ui->horizontalSlider->setMaximum(this->ParticlePopulation->size() - 1);
  for (uint32_t i = 0; i < ui->widget->slideCount(); i++) {
    ui->widget->removeSlide(0);
  }
  for (uint32_t i = 0; i < this->ParticlePopulation->size(); i++) {
    ui->widget->addSlide(
        ConvertParticleToQImage(&ParticlePopulation->at(i)));
  }
}

QImage
QParticleDisplay::ConvertParticleToQImage(SoilAnalyzer::Particle *particle) {
  QImage dst(particle->BW.cols, particle->BW.rows, QImage::Format_RGB32);
  uint32_t nData = particle->BW.cols * particle->BW.rows;
  uchar *QDst = dst.bits();
  uchar *CVBW = particle->BW.data;
  uchar *CVRGB = particle->RGB.data;
  for (uint32_t i = 0; i < nData; i++) {
    if (CVBW[i]) {
        *(QDst++) = *(CVRGB + 2);
        *(QDst++) = *(CVRGB + 1);
        *(QDst++) = *(CVRGB );
        *(QDst++) = 0;
        CVRGB += 3;
    } else {
        *(QDst++) = 255;
        *(QDst++) = 255;
        *(QDst++) = 255;
        *(QDst++) = 0;
        CVRGB += 3;
      }
  }
  return dst;
}

void QParticleDisplay::on_pushButton_delete_clicked()
{
  ParticlePopulation->erase(ParticlePopulation->begin() + ui->widget->centerIndex());
  ui->widget->removeSlide(ui->widget->centerIndex());
  ui->horizontalSlider->setMaximum(this->ParticlePopulation->size());
}

void QParticleDisplay::on_selectedParticleChanged(int value) {
  QString volume;
  volume.sprintf("%+06.2f", ParticlePopulation->at(value).GetSIVolume());
  ui->label_Volume->setText(volume);
}
