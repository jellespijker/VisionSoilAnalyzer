#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include <opencv2/core.hpp>

DialogSettings::DialogSettings(QWidget *parent,
                               SoilAnalyzer::SoilSettings *settings,
                               Hardware::Microscope *microscope,
                               SoilMath::NN *nn)
    : QDialog(parent), ui(new Ui::DialogSettings) {
  ui->setupUi(this);
  if (settings == nullptr) {
    settings = new SoilAnalyzer::SoilSettings;
  }
  Settings = settings;
  if (microscope == nullptr) {
    microscope = new Hardware::Microscope;
  }
  if (nn == nullptr) {
    nn = new SoilMath::NN;
  }

  // Setup the Hardware tab
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

  ui->spinBox_NoShots->setValue(Settings->StandardNumberOfShots);

  ui->spinBox_NoFrames->setValue(Settings->HDRframes);
  ui->spinBox_NoFrames->setDisabled(true);
  ui->label_nf->setDisabled(true);

  ui->checkBox_Backlight->setChecked(Settings->useBacklightProjection);
  ui->checkBox_InvertEncoder->setChecked(Settings->encInv);
  ui->checkBox_useCUDA->setChecked(Settings->useCUDA);

  Settings->useCUDA = false;
  ui->checkBox_useCUDA->setDisabled(true);

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

  // Setup the Vision tab
  ui->cb_fillHoles_3->setChecked(Settings->fillHoles);
  ui->cb_ignoreBorder_3->setChecked(Settings->ignorePartialBorderParticles);
  ui->cb_useBlur_3->setChecked(Settings->useBlur);
  if (!Settings->useBlur) {
    ui->sb_blurMask_3->setEnabled(false);
  }
  ui->cb_use_adaptContrast_3->setChecked(Settings->useAdaptiveContrast);
  if (!Settings->useAdaptiveContrast) {
    ui->sb_adaptContrastFactor_3->setEnabled(false);
    ui->sb_adaptContrKernel_3->setEnabled(false);
  }
  switch (Settings->typeOfObjectsSegmented) {
  case Vision::Segment::Bright:
    ui->rb_useDark_3->setChecked(false);
    ui->rb_useLight_3->setChecked(true);
    break;
  case Vision::Segment::Dark:
    ui->rb_useDark_3->setChecked(true);
    ui->rb_useLight_3->setChecked(false);
    break;
  }
  switch (Settings->morphFilterType) {
  case Vision::MorphologicalFilter::CLOSE:
    ui->rb_useClose_3->setChecked(true);
    ui->rb_useDilate_3->setChecked(false);
    ui->rb_useErode_3->setChecked(false);
    ui->rb_useOpen_3->setChecked(false);
    break;
  case Vision::MorphologicalFilter::OPEN:
    ui->rb_useClose_3->setChecked(false);
    ui->rb_useDilate_3->setChecked(false);
    ui->rb_useErode_3->setChecked(false);
    ui->rb_useOpen_3->setChecked(true);
    break;
  case Vision::MorphologicalFilter::ERODE:
    ui->rb_useClose_3->setChecked(false);
    ui->rb_useDilate_3->setChecked(false);
    ui->rb_useErode_3->setChecked(true);
    ui->rb_useOpen_3->setChecked(false);
    break;
  case Vision::MorphologicalFilter::DILATE:
    ui->rb_useClose_3->setChecked(false);
    ui->rb_useDilate_3->setChecked(true);
    ui->rb_useErode_3->setChecked(false);
    ui->rb_useOpen_3->setChecked(false);
    break;
  }

  ui->sb_adaptContrastFactor_3->setValue(Settings->adaptContrastKernelFactor);
  ui->sb_adaptContrKernel_3->setValue(Settings->adaptContrastKernelSize);
  ui->sb_blurMask_3->setValue(Settings->blurKernelSize);
  ui->sb_morphMask_3->setValue(Settings->filterMaskSize);
  ui->sb_sigmaFactor_3->setValue(Settings->sigmaFactor);

  // Setup the neural Network tab
  NN = nn;
  QPixmap NNpix("Images/feedforwardnetwork2.png");
  ui->label_NNimage->setPixmap(NNpix);
  ui->label_NNimage->setScaledContents(true);

  ui->spinBox_InputNeurons->setValue(NN->GetInputNeurons());
  ui->spinBox_HiddenNeurons->setValue(NN->GetInputNeurons());
  ui->spinBox_OutputNeurons->setValue(NN->GetOutputNeurons());
  ui->spinBox_Elitisme->setValue(NN->ElitismeUsedByGA);
  ui->spinBox_MaxGen->setValue(NN->MaxGenUsedByGA);
  ui->spinBox_PopSize->setValue(NN->PopulationSizeUsedByGA);
  ui->doubleSpinBox_endError->setValue(NN->EndErrorUsedByGA);
  ui->doubleSpinBox_MutationRate->setValue(NN->MutationrateUsedByGA);
  ui->doubleSpinBox_Beta->setValue(NN->GetBeta());
  ui->doubleSpinBox_maxWeight->setValue(NN->MaxWeightUsedByGA);
  ui->doubleSpinBox_MinWeight->setValue(NN->MinWeightUSedByGa);
  initfase = false;

  // Setup the preference tab
  ui->lineEdit_NeuralNetFolder->setText(
      QString::fromStdString(Settings->NNFolder));
  ui->lineEdit_Printer->setText(
      QString::fromStdString(Settings->StandardPrinter));
  ui->lineEdit_Samplefolder->setText(
      QString::fromStdString(Settings->SampleFolder));
  ui->lineEdit_SendTo->setText(
      (QString::fromStdString(Settings->StandardSentTo)));
  ui->lineEdit_SettingFolder->setText(
      QString::fromStdString(Settings->SettingsFolder));
  ui->lineEdit__NeuralNet->setText(
      QString::fromStdString(Settings->NNlocation));
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
  Settings->useBacklightProjection = checked;
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
  Settings->useHDR = checked;
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

void DialogSettings::on_spinBox_NoFrames_editingFinished() {
  Settings->HDRframes = ui->spinBox_NoFrames->value();
}

void DialogSettings::on_doubleSpinBox_LightLevel_editingFinished() {
  Settings->lightLevel =
      static_cast<float>(ui->doubleSpinBox_LightLevel->value());
}

void DialogSettings::on_checkBox_useRainbow_clicked(bool checked) {
  Settings->enableRainbow = checked;
}

void DialogSettings::on_checkBox_InvertEncoder_clicked(bool checked) {
  Settings->encInv = checked;
}

void DialogSettings::on_checkBox_useCUDA_clicked(bool checked) {
  Settings->useCUDA = checked;
}

void DialogSettings::on_horizontalSlider_BrightFront_valueChanged(int value) {
  if (!initfase) {
    Settings->Brightness_front = value;
  }
}

void DialogSettings::on_horizontalSlider_ContrastFront_valueChanged(int value) {
  if (!initfase) {
    Settings->Contrast_front = value;
  }
}

void DialogSettings::on_horizontalSlider_SaturationFront_valueChanged(
    int value) {
  if (!initfase) {
    Settings->Saturation_front = value;
  }
}

void DialogSettings::on_horizontalSlider_HueFront_valueChanged(int value) {
  if (!initfase) {
    Settings->Hue_front = value;
  }
}

void DialogSettings::on_horizontalSlider_SharpnessFront_valueChanged(
    int value) {
  if (!initfase) {
    Settings->Sharpness_front = value;
  }
}

void DialogSettings::on_horizontalSlider_BrightProj_valueChanged(int value) {
  if (!initfase) {
    Settings->Brightness_proj = value;
  }
}

void DialogSettings::on_horizontalSlider_ContrastProj_valueChanged(int value) {
  if (!initfase) {
    Settings->Contrast_proj = value;
  }
}

void DialogSettings::on_horizontalSlider_SaturationProj_valueChanged(
    int value) {
  if (!initfase) {
    Settings->Saturation_proj = value;
  }
}

void DialogSettings::on_horizontalSlider_HueProj_valueChanged(int value) {
  if (!initfase) {
    Settings->Hue_proj = value;
  }
}

void DialogSettings::on_horizontalSlider_SharpnessProj_valueChanged(int value) {
  if (!initfase) {
    Settings->Sharpness_proj = value;
  }
}

void DialogSettings::on_cb_use_adaptContrast_3_clicked(bool checked) {
  Settings->useAdaptiveContrast = checked;
  ui->sb_adaptContrastFactor_3->setDisabled(!checked);
  ui->sb_adaptContrKernel_3->setDisabled(!checked);
}

void DialogSettings::on_cb_useBlur_3_clicked(bool checked) {
  Settings->useBlur = checked;
  ui->sb_blurMask_3->setDisabled(!checked);
}

void DialogSettings::on_rb_useDark_3_toggled(bool checked) {
  if (checked) {
    Settings->typeOfObjectsSegmented = Vision::Segment::Dark;
  } else {
    Settings->typeOfObjectsSegmented = Vision::Segment::Bright;
  }
}

void DialogSettings::on_cb_ignoreBorder_3_clicked(bool checked) {
  Settings->ignorePartialBorderParticles = checked;
}

void DialogSettings::on_cb_fillHoles_3_clicked(bool checked) {
  Settings->fillHoles = checked;
}

void DialogSettings::on_sb_sigmaFactor_3_editingFinished() {
  Settings->sigmaFactor = ui->sb_sigmaFactor_3->value();
}

void DialogSettings::on_rb_useOpen_3_clicked(bool checked) {
  Settings->morphFilterType = Vision::MorphologicalFilter::OPEN;
}

void DialogSettings::on_rb_useClose_3_clicked(bool checked) {
  Settings->morphFilterType = Vision::MorphologicalFilter::CLOSE;
}

void DialogSettings::on_rb_useErode_3_clicked(bool checked) {
  Settings->morphFilterType = Vision::MorphologicalFilter::ERODE;
}

void DialogSettings::on_rb_useDilate_3_clicked(bool checked) {
  Settings->morphFilterType = Vision::MorphologicalFilter::DILATE;
}

void DialogSettings::on_sb_morphMask_3_editingFinished() {
  Settings->filterMaskSize = ui->sb_morphMask_3->value();
}

void DialogSettings::on_spinBox_MaxGen_editingFinished() {
  NN->MaxGenUsedByGA = ui->spinBox_MaxGen->value();
}

void DialogSettings::on_spinBox_PopSize_editingFinished() {
  NN->PopulationSizeUsedByGA = ui->spinBox_PopSize->value();
}

void DialogSettings::on_doubleSpinBox_MutationRate_editingFinished() {
  NN->MutationrateUsedByGA = ui->doubleSpinBox_MutationRate->value();
}

void DialogSettings::on_spinBox_Elitisme_editingFinished() {
  NN->ElitismeUsedByGA = ui->spinBox_Elitisme->value();
}

void DialogSettings::on_doubleSpinBox_endError_editingFinished() {
  NN->EndErrorUsedByGA = ui->doubleSpinBox_endError->value();
}

void DialogSettings::on_doubleSpinBox_maxWeight_editingFinished() {
  NN->MaxWeightUsedByGA = ui->doubleSpinBox_maxWeight->value();
}

void DialogSettings::on_doubleSpinBox_MinWeight_editingFinished() {
  NN->MinWeightUSedByGa = ui->doubleSpinBox_MinWeight->value();
}

void DialogSettings::on_doubleSpinBox_Beta_editingFinished() {
  NN->SetBeta(ui->doubleSpinBox_Beta->value());
}

void DialogSettings::on_spinBox_InputNeurons_editingFinished() {
  NN->SetInputNeurons(ui->spinBox_InputNeurons->value());
}

void DialogSettings::on_spinBox_HiddenNeurons_editingFinished() {
  NN->SetHiddenNeurons(ui->spinBox_HiddenNeurons->value());
}

void DialogSettings::on_spinBox_OutputNeurons_editingFinished() {
  NN->SetOutputNeurons(ui->spinBox_OutputNeurons->value());
}

void DialogSettings::on_pushButton_selectSampleFolder_clicked() {
  QString fn = QFileDialog::getExistingDirectory(
      this, tr("Select the Sample Directory"),
      QString::fromStdString(Settings->SampleFolder),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!fn.isEmpty()) {
    ui->lineEdit_Samplefolder->setText(fn);
    Settings->SampleFolder = fn.toStdString();
  }
}

void DialogSettings::on_pushButton_SelectSettingFolder_clicked() {
  QString fn = QFileDialog::getExistingDirectory(
      this, tr("Select the Setting Directory"),
      QString::fromStdString(Settings->SettingsFolder),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!fn.isEmpty()) {
    ui->lineEdit_SettingFolder->setText(fn);
    Settings->SettingsFolder = fn.toStdString();
  }
}

void DialogSettings::on_pushButton_SelectNNFolder_clicked() {
  QString fn = QFileDialog::getExistingDirectory(
      this, tr("Select the NeuralNet Directory"),
      QString::fromStdString(Settings->NNFolder),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!fn.isEmpty()) {
    ui->lineEdit_NeuralNetFolder->setText(fn);
    Settings->NNFolder = fn.toStdString();
  }
}

void DialogSettings::on_pushButton_SelectNN_clicked()
{
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Select the standard Neural Net"), QDir::homePath(), tr("NeuralNet (*.NN)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".NN"))) {
      fn.append(tr(".NN"));
    }
    Settings->NNlocation = fn.toStdString();
    ui->lineEdit__NeuralNet->setText(fn);
  }
}

void DialogSettings::on_spinBox_NoShots_editingFinished()
{
    Settings->StandardNumberOfShots = ui->spinBox_NoShots->value();
}
