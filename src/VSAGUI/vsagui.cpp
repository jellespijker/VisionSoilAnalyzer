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
  Hardware::Microscope microscope;

  finished_sig = microscope.connect_Finished([&]() {
    SetMatToMainView(SoilSample->OriginalImage);
    this->statusLabel->setText(tr("New Image Grabbed"));
  });
  progress_sig = microscope.connect_Progress(
      [&](int &prog) { this->progressBar->setValue(prog); });

  try {
    if (microscope.IsOpened()) {
      microscope.GetHDRFrame(SoilSample->OriginalImage, sSettings->HDRframes);
    }
  } catch (Hardware::Exception::MicroscopeNotFoundException &em) {
    try {
      errorMessageDialog->showMessage(
          tr("Microscope not found switching to first default Cam!"));
      if (microscope.AvailableCams().size() > 0) {
        microscope.openCam(0);
      }
    } catch (Hardware::Exception::MicroscopeNotFoundException &em2) {
      // display error dialog no cam found and show default test image
      errorMessageDialog->showMessage(
          tr("Microscope not found switching to test image!"));
      SoilSample->OriginalImage = cv::imread("/Images/SoilSample1.png");
    }
  } catch (Hardware::Exception::CouldNotGrabImageException &ei) {
    // HDRFrame not working switching to normal grab
    try {
      errorMessageDialog->showMessage(
          tr("HDR Grab failed switching to normal grab!"));
      microscope.GetFrame(SoilSample->OriginalImage);
    } catch (Hardware::Exception::CouldNotGrabImageException &ei2) {
      // show default test image and error dialog
      errorMessageDialog->showMessage(
          tr("Normal Grab failed switching to test image!"));
      SoilSample->OriginalImage = cv::imread("/Images/SoilSample1.png");
    }
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
  delete SoilSample;
  SoilSample = new SoilAnalyzer::Sample;
  SoilSample->connect_Progress(
      boost::bind(&VSAGUI::on_vision_update, this, _1, _2));
  this->statusLabel->setText(tr("Grabbing new Image!"));
  finished_sig = microscope.connect_Finished([&]() {
    SetMatToMainView(SoilSample->OriginalImage);
    this->statusLabel->setText(tr("New Image Grabbed"));
  });
  progress_sig = microscope.connect_Progress(
      [&](int &prog) { this->progressBar->setValue(prog); });
  microscope.GetHDRFrame(SoilSample->OriginalImage, sSettings->HDRframes);
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

void VSAGUI::on_AnalyzeButton_clicked() { SoilSample->Analyse(*NeuralNet); }

void VSAGUI::on_actionNew_triggered() {
  delete SoilSample;
  SoilSample = new SoilAnalyzer::Sample;
  SoilSample->connect_Progress(
      boost::bind(&VSAGUI::on_vision_update, this, _1, _2));
  on_SnapshotButton_clicked();
}

void VSAGUI::on_actionImport_triggered() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Import Neural Network"), tr("/home/"),
      tr("Neural Net (*.NN);;All Files (*)"));
  if (!fn.isEmpty())
  {
    std::string filename = fn.toStdString();
    NeuralNet->LoadState(filename);
  }
}

void VSAGUI::on_actionExport_triggered() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Export Neural Network"), tr("/home/"),
      tr("Neural Net (*.NN);;All Files (*)"));
  if (!fn.isEmpty())
  {
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
}

void VSAGUI::on_actionSave_Settings_triggered() {
  QString fn =
      QFileDialog::getSaveFileName(this, tr("Save Settings"), tr("/home/"),
                                   tr("Settings (*.ini);;All Files (*)"));
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
  if (!fn.isEmpty())  {
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

void VSAGUI::on_verticalSlider_sliderReleased() {
  sSettings->thresholdOffsetValue = ui->OffsetSlider->value();
  if (SoilSample->imgPrepped) {
    SoilSample->PrepImg(sSettings);
  }
}
void VSAGUI::on_OffsetSlider_valueChanged(int value) {
  sSettings->thresholdOffsetValue = ui->OffsetSlider->value();
}

void VSAGUI::on_OffsetSlider_sliderReleased() {}

void VSAGUI::on_actionHardware_Settings_triggered() {
  hsetttingWindow = new HardwareSettings(0, sSettings);
  hsetttingWindow->exec();
}

/*!
 * \brief VSAGUI::on_actionCheese_2_triggered
 */
void VSAGUI::on_actionCheese_2_triggered()
{
    // Get the name of the individual cams
    std::vector<std::string> availCams = Hardware::Microscope::AvailableCams();
    uint i = 0;
    for_each(availCams.begin(), availCams.end(),[&](std::string &C)
    {
        // If the current itterator is the Micrscope start cheese
        if (C.compare(MICROSCOPE_NAME) == 0) {
            std::string bashStr = "cheese --device=/dev/video";
            bashStr.append(std::to_string(i));
            std::system(bashStr.c_str());
            return;
        }
        i++;
    });

}
