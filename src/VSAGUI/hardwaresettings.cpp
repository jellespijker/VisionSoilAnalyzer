/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "hardwaresettings.h"
#include "ui_hardwaresettings.h"

HardwareSettings::HardwareSettings(QWidget *parent,
                                   SoilAnalyzer::SoilSettings *soilsetting)
    : QDialog(parent), ui(new Ui::HardwareSettings) {
  ui->setupUi(this);
  if (soilsetting != 0) {
    soilSetting = soilsetting;
  } else {
    soilSetting = new SoilAnalyzer::SoilSettings;
  }

  ui->cb_enableRainbow->setChecked(soilSetting->enableRainbow);
  ui->cb_encoderInv->setChecked(soilSetting->encInv);
  ui->sb_HDRframes->setValue(soilSetting->HDRframes);
  ui->sb_lightLevel->setValue(soilSetting->lightLevel);
  ui->cb_useHDR->setChecked(soilSetting->useHDR);
  ui->cb_useBacklightProjection->setChecked(soilSetting->useBacklightProjection);

  // Get system info
  struct utsname unameData;
  uname(&unameData);

  ui->label_machinename->setText(tr(unameData.machine));
  ui->label_nodename->setText(tr(unameData.nodename));
  ui->label_releasename->setText(tr(unameData.release));
  ui->label_systemname->setText(tr(unameData.sysname));
  ui->label_versioname->setText(tr(unameData.version));
  std::string arch = static_cast<std::string>(unameData.machine);
  if (arch.find("armv7l") == string::npos) {
    ui->cb_enableRainbow->setDisabled(true);
    ui->cb_encoderInv->setDisabled(true);
    ui->sb_lightLevel->setDisabled(true);
    ui->label_ll->setDisabled(true);
  }
}

HardwareSettings::~HardwareSettings() { delete ui; }

void HardwareSettings::on_sb_HDRframes_editingFinished() {
  soilSetting->HDRframes = ui->sb_HDRframes->value();
}

void HardwareSettings::on_sb_lightLevel_editingFinished() {
  soilSetting->lightLevel = static_cast<float>(ui->sb_lightLevel->value());
}

void HardwareSettings::on_cb_encoderInv_clicked(bool checked) {
  soilSetting->encInv = checked;
}

void HardwareSettings::on_cb_enableRainbow_clicked(bool checked) {
  soilSetting->enableRainbow = checked;
}

void HardwareSettings::on_cb_useHDR_toggled(bool checked)
{
    soilSetting->useHDR = checked;
}

void HardwareSettings::on_cb_useBacklightProjection_toggled(bool checked)
{
    soilSetting->useBacklightProjection = checked;
}
