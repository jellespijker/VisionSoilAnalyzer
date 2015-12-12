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
  ui->widget->setSlideSize(QSize(230, 230));
  connect(ui->widget, SIGNAL(centerIndexChanged(int)), this,
          SLOT(on_selectedParticleChangedWidget(int)));
  connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_selectedParticleChangedSlider(int)));
  connect(ui->comboBoxFilter, SIGNAL(currentIndexChanged(int)), this,
          SLOT(on_filterChanged(ind)));

  // Set filtercombobox;
  ui->comboBoxFilter->addItem(tr("All"));
  ui->comboBoxFilter->addItem(tr("1.4 - 2"));
  ui->comboBoxFilter->addItem(tr("1 - 1.4"));
  ui->comboBoxFilter->addItem(tr("0.71 - 1"));
}

QParticleDisplay::~QParticleDisplay() {
  for (uint32_t i = 0; i < ui->widget->slideCount(); i++) {
    ui->widget->removeSlide(0);
  }
  delete ui->widget;
  delete ui;
}

void QParticleDisplay::setSelectedParticle(int newValue) {
  ui->widget->setCenterIndex(newValue);
  ui->horizontalSlider->setValue(newValue);
}

void QParticleDisplay::SetSample(SoilAnalyzer::Sample *sample) {
  this->Sample = sample;
  images.clear();
  ui->widget->clear();
  ui->horizontalSlider->setMaximum(this->Sample->ParticlePopulation.size() - 1);
  for (uint32_t i = 0; i < this->Sample->ParticlePopulation.size(); i++) {
    images.push_back(
        ConvertParticleToQImage(&Sample->ParticlePopulation.at(i)));
    ui->widget->addSlide(images[images.size() - 1]);
  }
  SelectedParticle = &Sample->ParticlePopulation[ui->widget->centerIndex()];
  on_selectedParticleChangedSlider(0);
}

QImage
QParticleDisplay::ConvertParticleToQImage(SoilAnalyzer::Particle *particle) {

  cv::Mat srcBorderBW;
  cv::Mat srcBorderRGB;

  int max = particle->BW.cols > particle->BW.rows ? particle->BW.cols
                                                  : particle->BW.rows;
  int min = particle->BW.cols < particle->BW.rows ? particle->BW.cols
                                                  : particle->BW.rows;
  max += 10;

  int diff = max - min;
  int rowPadding, colPadding;

  if (particle->BW.cols > particle->BW.rows) {
    rowPadding = (diff + 10) / 2;
    colPadding = 5;
  } else {
    colPadding = (diff + 1) / 2;
    rowPadding = 5;
  }

  cv::copyMakeBorder(particle->BW, srcBorderBW, rowPadding, rowPadding,
                     colPadding, colPadding, BORDER_CONSTANT, 0);
  cv::copyMakeBorder(particle->RGB, srcBorderRGB, rowPadding, rowPadding,
                     colPadding, colPadding, BORDER_CONSTANT, 0);

  QImage dst(srcBorderBW.cols, srcBorderBW.rows, QImage::Format_RGB32);

  uint32_t nData = srcBorderBW.cols * srcBorderBW.rows;
  uchar *QDst = dst.bits();
  uchar *CVBW = srcBorderBW.data;
  uchar *CVRGB = srcBorderRGB.data;

  for (uint32_t i = 0; i < nData; i++) {
    if (CVBW[i]) {
      *(QDst++) = *(CVRGB);
      *(QDst++) = *(CVRGB + 1);
      *(QDst++) = *(CVRGB + 2);
    } else {
      *(QDst++) = 255;
      *(QDst++) = 255;
      *(QDst++) = 255;
    }
    *(QDst++) = 0;
    CVRGB += 3;
  }
  return dst;
}

void QParticleDisplay::on_pushButton_delete_clicked() {
  Sample->ParticlePopulation.erase(Sample->ParticlePopulation.begin() +
                                   ui->widget->centerIndex());
  ui->widget->removeSlide(ui->widget->centerIndex());
  ui->horizontalSlider->setMaximum(this->Sample->ParticlePopulation.size() - 1);
  Sample->ParticleChangedStatePSD = true;
  Sample->ParticleChangedStateAngularity = true;
  Sample->ParticleChangedStateRoundness = true;
  Sample->ChangesSinceLastSave = true;
  Sample->ColorChange = true;
  SelectedParticle = &Sample->ParticlePopulation[ui->widget->centerIndex()];
  emit particleDeleted();
}

void QParticleDisplay::on_selectedParticleChangedWidget(int value) {
  if (!dontDoIt) {
    dontDoIt = true;
    ui->horizontalSlider->setValue(value);
    SelectedParticle = &Sample->ParticlePopulation[ui->widget->centerIndex()];
    QString volume;
    volume.sprintf("%6.2f", SelectedParticle->GetSiDiameter());
    ui->label_Volume->setText(volume);
    emit particleChanged(value);
    emit shapeClassificationChanged(SelectedParticle->Classification.Category);
    dontDoIt = false;
  }
}

void QParticleDisplay::on_selectedParticleChangedSlider(int value) {
  if (!dontDoIt) {
    dontDoIt = true;
    ui->widget->setCenterIndex(value);
    SelectedParticle = &Sample->ParticlePopulation[ui->widget->centerIndex()];
    QString volume;
    volume.sprintf("%6.2f", SelectedParticle->GetSiDiameter());
    ui->label_Volume->setText(volume);
    emit particleChanged(value);
    emit shapeClassificationChanged(SelectedParticle->Classification.Category);
    dontDoIt = false;
  }
}

void QParticleDisplay::wheelEvent(QWheelEvent *event) {
  int i = ui->widget->centerIndex();
  i -= event->delta() / 120;
  if (i < 0) {
    i = ui->widget->slideCount() - abs(i) - 1;
  } else if (i >= ui->widget->slideCount()) {
    i = 0;
  }
  ui->widget->setCenterIndex(i);
  on_selectedParticleChangedWidget(i);
}

void QParticleDisplay::next() {
  int i = ui->widget->centerIndex();
  i++;
  if (i < 0) {
    i = ui->widget->slideCount() - abs(i) - 1;
  } else if (i >= ui->widget->slideCount()) {
    i = 0;
  }
  ui->widget->setCenterIndex(i);
  on_selectedParticleChangedWidget(i);
}

void QParticleDisplay::on_filterChanged(int index) {}
