#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent,
                               SoilAnalyzer::SoilSettings *settings,
                               Hardware::Microscope *microscope)
    : QDialog(parent), ui(new Ui::DialogSettings) {
  ui->setupUi(this);
  if (settings == nullptr) {
    settings = new SoilAnalyzer::SoilSettings;
  }
  Settings = settings;
  if (microscope == nullptr) {
    microscope = new Hardware::Microscope;
  }

  // Setup the cam combobox
  Microscope = microscope;
  QStringList Cams;
  for (uint32_t i = 0; i < Microscope->AvailableCams.size(); i++) {
    Cams << Microscope->AvailableCams[i].Name.c_str();
  }
  ui->comboBox_Microscopes->addItems(Cams);
  ui->comboBox_Microscopes->setCurrentIndex(Microscope->SelectedCam->ID);

  QStringList Resolutions;
  for (uint32_t i = 0; i < Microscope->SelectedCam->Resolutions.size(); i++) {
    Resolutions << Microscope->SelectedCam->Resolutions[i].to_string().c_str();
  }
  ui->comboBox_Resolution->addItems(Resolutions);
  ui->comboBox_Resolution->setCurrentIndex(
      Microscope->SelectedCam->SelectedResolution->ID);

  // Fill the dialog settings
  ui->spinBox_NoFrames->setDisabled(true);
  ui->label_nf->setDisabled(true);

  ui->checkBox_Backlight->setChecked(Settings->useBacklightProjection);
  ui->checkBox_InvertEncoder->setChecked(Settings->encInv);
  ui->checkBox_useCUDA->setChecked(Settings->useCUDA);
  ui->checkBox_useHDR->setChecked(Settings->useHDR);
  ui->checkBox_useRainbow->setChecked(Settings->enableRainbow);

  // Get system info
  struct utsname unameData;
  uname(&unameData);

  ui->label_machinename->setText(tr(unameData.machine));
  ui->label_nodename->setText(tr(unameData.nodename));
  ui->label_releasename->setText(tr(unameData.release));
  ui->label_systemname->setText(tr(unameData.sysname));
  ui->label_versioname->setText(tr(unameData.version));
  if (Microscope->RunEnv == Hardware::Microscope::X64) {
    ui->checkBox_useRainbow->setDisabled(true);
    ui->checkBox_InvertEncoder->setDisabled(true);
    ui->doubleSpinBox_LightLevel->setDisabled(true);
    ui->label_ll->setDisabled(true);
  }

  SetCamControl(
      Microscope->SelectedCam, ui->horizontalSlider_BrightFront,
      ui->horizontalSlider_ContrastFront, ui->horizontalSlider_SaturationFront,
      ui->horizontalSlider_HueFront, ui->horizontalSlider_SharpnessFront);
  ui->horizontalSlider_BrightFront->setValue(Settings->Brightness_front);
  ui->horizontalSlider_ContrastFront->setValue(Settings->Contrast_front);
  ui->horizontalSlider_HueFront->setValue(Settings->Hue_front);
  ui->horizontalSlider_SaturationFront->setValue(Settings->Saturation_front);
  ui->horizontalSlider_SharpnessFront->setValue(Settings->Sharpness_front);

  SetCamControl(
      Microscope->SelectedCam, ui->horizontalSlider_BrightProj,
      ui->horizontalSlider_ContrastProj, ui->horizontalSlider_SaturationProj,
      ui->horizontalSlider_HueProj, ui->horizontalSlider_SharpnessProj);
  ui->horizontalSlider_BrightProj->setValue(Settings->Brightness_proj);
  ui->horizontalSlider_ContrastProj->setValue(Settings->Contrast_proj);
  ui->horizontalSlider_HueProj->setValue(Settings->Hue_proj);
  ui->horizontalSlider_SaturationProj->setValue(Settings->Saturation_proj);
  ui->horizontalSlider_SharpnessProj->setValue(Settings->Sharpness_proj);

  initfase = false;
}

DialogSettings::~DialogSettings() { delete ui; }

void DialogSettings::on_pushButton_RestoreDefault_clicked() {
  Settings->LoadSettings("Settings/Default.ini");
}

void DialogSettings::on_pushButton_Open_clicked() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open Settings"), QDir::homePath(), tr("Settings (*.ini)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".ini"))) {
      fn.append(tr(".ini"));
    }
    Settings->LoadSettings(fn.toStdString());
  }
}

void DialogSettings::on_pushButton_Save_clicked() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save Settings"), QDir::homePath(), tr("Settings (*.ini)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".ini"))) {
      fn.append(tr(".ini"));
    }
    Settings->SaveSettings(fn.toStdString());
  }
}

void DialogSettings::on_checkBox_Backlight_clicked(bool checked) {
  ui->tabWidget_Hardware->setTabEnabled(2, checked);
}

void DialogSettings::on_comboBox_Microscopes_currentIndexChanged(
    const QString &arg1) {

  if (!initfase) {
    std::string selectedCam = arg1.toStdString();
    Microscope->openCam(selectedCam);
    Settings->defaultWebcam = selectedCam;

    ui->comboBox_Resolution->clear();
    QStringList Resolutions;
    for (uint32_t i = 0; i < Microscope->SelectedCam->Resolutions.size(); i++) {
      Resolutions
          << Microscope->SelectedCam->Resolutions[i].to_string().c_str();
    }
    ui->comboBox_Resolution->addItems(Resolutions);
    ui->comboBox_Resolution->setCurrentIndex(
        Microscope->SelectedCam->SelectedResolution->ID);
  }
}

void DialogSettings::on_comboBox_Resolution_currentIndexChanged(int index) {
  if (!initfase) {
    Microscope->SelectedCam->SelectedResolution =
        &Microscope->SelectedCam->Resolutions[index];
    Settings->selectedResolution = index;
  }
}

void DialogSettings::on_checkBox_useHDR_clicked(bool checked) {
  ui->spinBox_NoFrames->setDisabled(!checked);
  ui->label_nf->setDisabled(!checked);
}

void DialogSettings::SetCamControl(Hardware::Microscope::Cam_t *selectedCam,
                                   QSlider *Brightness, QSlider *Contrast,
                                   QSlider *Saturation, QSlider *Hue,
                                   QSlider *Sharpness) {
  for (uint32_t i = 0; i < selectedCam->Controls.size(); i++) {
    if (selectedCam->Controls[i].name.compare("Brightness") == 0) {
      Brightness->setMinimum(selectedCam->Controls[i].minimum);
      Brightness->setMaximum(selectedCam->Controls[i].maximum);
    } else if (selectedCam->Controls[i].name.compare("Contrast") == 0) {
      Contrast->setMinimum(selectedCam->Controls[i].minimum);
      Contrast->setMaximum(selectedCam->Controls[i].maximum);
    } else if (selectedCam->Controls[i].name.compare("Saturation") == 0) {
      Saturation->setMinimum(selectedCam->Controls[i].minimum);
      Saturation->setMaximum(selectedCam->Controls[i].maximum);
    } else if (selectedCam->Controls[i].name.compare("Hue") == 0) {
      Hue->setMinimum(selectedCam->Controls[i].minimum);
      Hue->setMaximum(selectedCam->Controls[i].maximum);
    } else if (selectedCam->Controls[i].name.compare("Sharpness") == 0) {
      Sharpness->setMinimum(selectedCam->Controls[i].minimum);
      Sharpness->setMaximum(selectedCam->Controls[i].maximum);
    }
  }
}
