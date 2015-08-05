/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "vsagui.h"
#include "ui_vsagui.h"

VSAGUI::VSAGUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::VSAGUI) {
  // Determine if the program is run from an ARM (BBB) device
  struct utsname unameData;
  uname(&unameData);
  std::string arch = static_cast<std::string>(unameData.machine);
  if (arch.find("armv7l") != string::npos) {
    runFromBBB = true;
  }

  // Startup the UI
  ui->setupUi(this);
  SoilSample = new SoilAnalyzer::Sample;
  SoilSample->connect_Progress(
      boost::bind(&VSAGUI::on_vision_update, this, _1, _2));
  NeuralNet = new SoilMath::NN;
  NeuralNet->LoadState("NeuralNet/Default.NN");
  sSettings = new SoilAnalyzer::SoilSettings;
  sSettings->LoadSettings("Settings/Default.ini");
  ui->OffsetSlider->setValue(sSettings->thresholdOffsetValue);

  errorMessageDialog = new QErrorMessage(this);

  // Init the status bar
  progressBar = new QProgressBar(ui->statusBar);
  progressBar->setAlignment(Qt::AlignLeft);
  progressBar->setMaximumSize(640, 19);
  ui->statusBar->addWidget(progressBar);
  progressBar->setValue(0);

  statusLabel = new QLabel(ui->statusBar);
  statusLabel->setAlignment(Qt::AlignRight);
  statusLabel->setMinimumSize(600, 19);
  statusLabel->setMaximumSize(600, 19);
  ui->statusBar->addWidget(statusLabel);
  statusLabel->setText(tr("First Grab"));

  // Get HDR snapshot of the sample or normal shot when HDR grab faulters or
  // test image if normal grab falters
  microscope = new Hardware::Microscope;
  finished_sig = microscope->connect_Finished([&]() {
    SetMatToMainView(SoilSample->OriginalImage);
    this->statusLabel->setText(tr("New Image Grabbed"));
  });
  progress_sig = microscope->connect_Progress(
      [&](int &prog) { this->progressBar->setValue(prog); });

  if (microscope->AvailableCams.size() == 0) {
    errorMessageDialog->showMessage(tr("No Microscopes found!"));
    SoilSample->OriginalImage = cv::imread("/Images/SoilSample1.png");
  } else {
    try {
      if (!microscope->openCam(sSettings->defaultWebcam)) {
        errorMessageDialog->showMessage(
            tr("Microscope not connected switching to default!"));
        int defCam = 0;
        microscope->openCam(defCam);
        sSettings->defaultWebcam = microscope->SelectedCam->Name;
      }
    } catch (Hardware::Exception::MicroscopeException &e) {
    }

    menuCamgroup = new QActionGroup(ui->menuMicroscopes);
    menuCamgroup->setExclusive(true);
    mapper = new QSignalMapper(menuCamgroup);
    for (uint32_t i = 0; i < microscope->AvailableCams.size(); i++) {
      QAction *camLabel =
          new QAction(microscope->AvailableCams[i].Name.c_str(), menuCamgroup);
      camLabel->setCheckable(true);
      if (microscope->AvailableCams[i] == *microscope->SelectedCam) {
        camLabel->setChecked(true);
      }
      ui->menuMicroscopes->addAction(camLabel);
      mapper->setMapping(camLabel, i);
      connect(camLabel, SIGNAL(toggled(bool)), mapper, SLOT(map()));
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(selectCam(int)));
  }
}

void VSAGUI::SetMatToMainView(cv::Mat &img) {
  QImage *qOrigImg = new QImage(OpenCVQT::Mat2QImage(img));
  QPixmap *qOrigPix = new QPixmap(QPixmap::fromImage(*qOrigImg));
  ui->MainImg->clear();
  ui->MainImg->setPixmap(*qOrigPix);
  ui->MainImg->show();
}

VSAGUI::~VSAGUI() { delete ui; }

void VSAGUI::on_SnapshotButton_clicked() {
  Hardware::Microscope microscope;
  CreateNewSoilSample();
  this->statusLabel->setText(tr("Grabbing new Image!"));
  finished_sig = microscope.connect_Finished([&]() {
    SetMatToMainView(SoilSample->OriginalImage);
    this->statusLabel->setText(tr("New Image Grabbed"));
  });
  progress_sig = microscope.connect_Progress(
      [&](int &prog) { this->progressBar->setValue(prog); });
  makeSnapShot();
}

void VSAGUI::on_vision_update(float prog, string statusText) {
  int progress = prog * 100;
  this->progressBar->setValue(progress);
  this->statusLabel->setText(tr(statusText.c_str()));
}

void VSAGUI::on_actionSave_triggered() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save Soil Sample"), tr("/home/"),
      tr("Soil Samples (*.VSS);; Soil Particles (*.VPS);; All Files (*)"));
  this->raise();
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".VSS"))) {
      fn.append(tr(".VSS"));
    }
    std::string filename = fn.toStdString();
    SoilSample->Save(filename);
  }
}

void VSAGUI::on_actionLoad_triggered() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Load Soil Sample"), tr("/home/"),
      tr("Soil Samples (*.VSS);; Soil Particles (*.VPS);; All Files (*)"));
  this->raise();
  if (!fn.isEmpty() && fn.contains(tr("VSS"))) {
    delete SoilSample;
    SoilSample = new SoilAnalyzer::Sample;
    std::string filename = fn.toStdString();
    SoilSample->Load(filename);
    SoilSample->connect_Progress(
        boost::bind(&VSAGUI::on_vision_update, this, _1, _2));
    SetMatToMainView(SoilSample->OriginalImage);
  }
}

/*!
 * \brief VSAGUI::on_AnalyzeButton_clicked Analyze the sample
 */
void VSAGUI::on_AnalyzeButton_clicked() {
  SoilSample->Analyse(*NeuralNet);
  SetMatToMainView(SoilSample->RGB);
}

void VSAGUI::on_actionNew_triggered() {
  CreateNewSoilSample();
  on_SnapshotButton_clicked();
}

void VSAGUI::on_actionImport_triggered() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Import Neural Network"), tr("/home/"),
      tr("Neural Net (*.NN);;All Files (*)"));
  this->raise();
  if (!fn.isEmpty()) {
    std::string filename = fn.toStdString();
    NeuralNet->LoadState(filename);
  }
}

void VSAGUI::on_actionExport_triggered() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Export Neural Network"), tr("/home/"),
      tr("Neural Net (*.NN);;All Files (*)"));
  this->raise();
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".NN"))) {
      fn.append(tr(".NN"));
    }
    std::string filename = fn.toStdString();
    NeuralNet->SaveState(filename);
  }
}

void VSAGUI::on_actionSegmentation_Settings_triggered() {
  settingWindow = new VisionSettings(0, sSettings);
  settingWindow->exec();
  this->raise();
}

void VSAGUI::on_actionSave_Settings_triggered() {
  QString fn =
      QFileDialog::getSaveFileName(this, tr("Save Settings"), tr("/home/"),
                                   tr("Settings (*.ini);;All Files (*)"));
  this->raise();
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".ini"))) {
      fn.append(tr(".ini"));
    }
    std::string filename = fn.toStdString();
    sSettings->SaveSettings(filename);
  }
}

void VSAGUI::on_actionLoad_Settings_triggered() {
  QString fn =
      QFileDialog::getOpenFileName(this, tr("Load Settings"), tr("/home/"),
                                   tr("Settings (*.ini);;All Files (*)"));
  this->raise();
  if (!fn.isEmpty()) {
    std::string filename = fn.toStdString();
    sSettings->LoadSettings(filename);
  }
}

void VSAGUI::on_actionRestore_Default_triggered() {
  std::string filename = "Settings/Default.ini";
  sSettings->LoadSettings(filename);
}

void VSAGUI::on_SegmentButton_clicked() {
  SoilSample->PrepImg(sSettings);
  SetMatToMainView(SoilSample->RGB);
}

void VSAGUI::on_OffsetSlider_valueChanged(int value) {
  sSettings->thresholdOffsetValue = ui->OffsetSlider->value();
}

void VSAGUI::on_actionHardware_Settings_triggered() {
  hsetttingWindow = new HardwareSettings(0, sSettings);
  hsetttingWindow->exec();
  this->raise();
}

/*!
 * \brief VSAGUI::on_actionCheese_2_triggered Load the cheese program which can
 * stream the webcam
 */
void VSAGUI::on_actionCheese_2_triggered() {
  std::string bashStr = "cheese --device=";
  bashStr.append(microscope->SelectedCam->devString);
  std::system(bashStr.c_str());
  this->raise();
}

/*!
 * \brief VSAGUI::on_actionImport_RGB_Snapshot_triggered Imports an RGB image to
 * be used for
 */
void VSAGUI::on_actionImport_RGB_Snapshot_triggered() {
  this->statusLabel->setText(tr("Importing new Image!"));

  // Create the new SoilSample
  CreateNewSoilSample();
  // Show the filedialog and import the RGB image
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Load Image"), tr("/home/"),
      tr("Image (*.jpg *.JPG *.png *.PNG *.gif *.GIF *.bmp *.BMP *.ppm *.PPM"));
  this->raise();
  if (!fn.isEmpty()) {
    std::string filename = fn.toStdString();
    SoilSample->OriginalImage = cv::imread(filename, 1);
    if (SoilSample->OriginalImage.channels() != 3) {
      errorMessageDialog->showMessage(tr("No RGB image"));
      on_actionImport_RGB_Snapshot_triggered();
    }
    SetMatToMainView(SoilSample->OriginalImage);
    this->statusLabel->setText(tr("New Image Imported"));
  }
}

/*!
 * \brief VSAGUI::CreateNewSoilSample Create the new Soil Sample
 */
void VSAGUI::CreateNewSoilSample() {
  delete SoilSample;
  SoilSample = new SoilAnalyzer::Sample;
  SoilSample->connect_Progress(
      boost::bind(&VSAGUI::on_vision_update, this, _1, _2));
}

/*!
 * \brief VSAGUI::on_actionExport_RGB_Snapshot_triggered Export the RGB snapshot
 */
void VSAGUI::on_actionExport_RGB_Snapshot_triggered() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Load Image"), tr("/home/"), tr("PPM Image (*.ppm *.PPM"));
  this->raise();
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".ppm"))) {
      fn.append(tr(".ppm"));
    }
    std::string filename = fn.toStdString();
    cv::imwrite(filename, SoilSample->OriginalImage);
  }
}

/*!
 * \brief VSAGUI::on_actionLearn_triggered
 */
void VSAGUI::on_actionLearn_triggered() {
  teacherWindow = new NNteacher(0);
  teacherWindow->show();
}

/*!
 * \brief VSAGUI::makeSnapShot make a snapshot according the soilsettings
 * \param microscope
 */
void VSAGUI::makeSnapShot() {
  if (microscope->IsOpened()) {
    if (sSettings->useHDR) {
      microscope->GetHDRFrame(SoilSample->OriginalImage, sSettings->HDRframes);
      if (sSettings->useBacklightProjection) {
        QMessageBox::information(
            0, "Backlight shot",
            "Activate your backlight and turn of the front light!");
        microscope->GetFrame(SoilSample->ProjectedImage);
      }
    } else {
      microscope->GetFrame(SoilSample->OriginalImage);
      if (sSettings->useBacklightProjection) {
        QMessageBox::information(
            0, "Backlight shot",
            "Activate your backlight and turn of the front light!");
        microscope->GetFrame(SoilSample->ProjectedImage);
      }
    }
  }
  microscope->fin_sig();
}

/*!
 * \brief VSAGUI::on_actionExport_Particles_triggered
 */
void VSAGUI::on_actionExport_Particles_triggered() {
  if (!SoilSample->imgPrepped) {
    SoilSample->PrepImg(sSettings);
  }
}

void VSAGUI::on_OffsetSlider_sliderReleased() {
  sSettings->thresholdOffsetValue = ui->OffsetSlider->value();
  if (SoilSample->imgPrepped) {
    SoilSample->PrepImg(sSettings);
  }
}

void VSAGUI::selectCam(int cam) {
  if (cam != microscope->SelectedCam->ID) {
    microscope->openCam(&microscope->AvailableCams[cam]);
  }
}
