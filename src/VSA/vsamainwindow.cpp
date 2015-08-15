#include "vsamainwindow.h"
#include "ui_vsamainwindow.h"

VSAMainWindow::VSAMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::VSAMainWindow) {
  ui->setupUi(this);

  // Load the usersettings
  Settings = new SoilAnalyzer::SoilSettings;
  Settings->LoadSettings("Settings/User.ini");

  // Set the message windows
  CamError = new QErrorMessage(this);
  SaveMeMessage = new QMessageBox(this);
  BacklightMessage = new QMessageBox(this);
  BacklightMessage->setText("Turn off Frontlight! Turn on Backlight!");
  ShakeItBabyMessage = new QMessageBox(this);

  // Load the Microscope
  Microscope = new Hardware::Microscope;
  try {
    Microscope->FindCam(Settings->defaultWebcam)->SelectedResolution =
        &Microscope->FindCam(Settings->defaultWebcam)
             ->Resolutions[Settings->selectedResolution];
  } catch (exception &e) {
    Microscope->FindCam(0)->SelectedResolution =
        &Microscope->FindCam(0)->Resolutions[Settings->selectedResolution];
  }
  try {
    if (!Microscope->openCam(Settings->defaultWebcam)) {
      int defaultCam = 0;
      Microscope->openCam(defaultCam);
      Settings->defaultWebcam = Microscope->SelectedCam->Name;
    }
  } catch (Hardware::Exception::MicroscopeException &e) {
    if (*e.id() == EXCEPTION_OPENCAM_NR) {
      try {
        int defaultCam = 0;
        Microscope->openCam(defaultCam);
        Settings->defaultWebcam = Microscope->SelectedCam->Name;
      } catch (Hardware::Exception::MicroscopeException &e) {
        if (*e.id() == EXCEPTION_NOCAMS_NR) {
          CamError->showMessage(
              tr("No cams found! Connect the cam and set the default"));
          settingsWindow = new DialogSettings(this, Settings, Microscope);
        }
      }
    }
  }

  // Setup the sample
  Sample = new SoilAnalyzer::Sample;
  Images = new SoilAnalyzer::Analyzer::Images_t;
  Analyzer = new SoilAnalyzer::Analyzer(Images, Sample, Settings);

  // Setup the setting Window
  if (settingsWindow == nullptr) {
    settingsWindow =
        new DialogSettings(this, Settings, Microscope, &Analyzer->NeuralNet);
  }

  // Setup the NN window
  if (nnWindow == nullptr) {
    nnWindow =
        new DialogNN(this, &Analyzer->NeuralNet, Settings, settingsWindow);
  }

  connect(ui->widget_ParticleSelector, SIGNAL(valueChanged(int)), this,
          SLOT(setParticleValue(int)));

  // Setup the progresbar and connect it to the Analyzer
  Progress = new QProgressBar(ui->statusBar);
  Progress->setMaximum(Analyzer->MaxProgress);
  Progress->setValue(0);
  Progress->setAlignment(Qt::AlignLeft);
  Progress->setMinimumSize(750, 19);
  ui->statusBar->addWidget(Progress);
  connect(Analyzer, SIGNAL(on_progressUpdate(int)), Progress,
          SLOT(setValue(int)));
  connect(Analyzer, SIGNAL(on_progressUpdate(int)), Progress,
          SLOT(setMaximum(int)));
  connect(Analyzer, SIGNAL(on_AnalysisFinished()), this,
          SLOT(on_analyzer_finished()));

  // Setup the Qplots
  ui->Qplot_PSD->addGraph();
  ui->Qplot_PSD->xAxis->setLabel("Particle size in [mm]");
  ui->Qplot_PSD->yAxis->setLabel("Percentage [%]");
  ui->Qplot_PSD->xAxis->setScaleType(QCPAxis::stLogarithmic);
  QCPPlotTitle *PSDtitle = new QCPPlotTitle(ui->Qplot_PSD);
  PSDtitle->setText("Particle Size Distribution");
  PSDtitle->setFont(QFont("sans", 10, QFont::Bold));
  ui->Qplot_PSD->plotLayout()->insertRow(0);
  ui->Qplot_PSD->plotLayout()->addElement(0, 0, PSDtitle);

  ui->QPlot_Texture->addGraph();
  ui->QPlot_Texture->xAxis->setLabel("Classification");
  ui->QPlot_Texture->yAxis->setLabel("Count [-]");
  QCPPlotTitle *Classtitle = new QCPPlotTitle(ui->QPlot_Texture);
  Classtitle->setText("Classification Histogram");
  Classtitle->setFont(QFont("sans", 10, QFont::Bold));
  ui->QPlot_Texture->plotLayout()->insertRow(0);
  ui->QPlot_Texture->plotLayout()->addElement(0, 0, Classtitle);

  // Connect the Particle display and Selector
  connect(ui->widget_ParticleSelector, SIGNAL(valueChanged(int)), this,
          SLOT(on_Classification_changed(int)));
  connect(ui->widget_ParticleDisplay, SIGNAL(shapeClassificationChanged(int)),
          ui->widget_ParticleSelector, SLOT(setValue(int)));
  connect(ui->widget_ParticleDisplay, SIGNAL(particleDeleted()), this,
          SLOT(on_particle_deleted()));
}

VSAMainWindow::~VSAMainWindow() { delete ui; }

void VSAMainWindow::setParticleValue(int newValue) {
  bool test = false;
  if (newValue == 1) {
    test = true;
  }
}

void VSAMainWindow::on_actionSettings_triggered() { settingsWindow->show(); }

void VSAMainWindow::on_analyzer_finished() {
  if (!ParticleDisplayerFilled) {
    ui->widget_ParticleDisplay->SetSample(Sample);
  }
  SetPSDgraph();
  SetClassHistogram();
  ParticleDisplayerFilled = true;
}

void VSAMainWindow::SetPSDgraph() {
  ui->Qplot_PSD->graph(0)->clearData();
  QVector<double> xPSD(Sample->PSD.noBins), yPSD(Sample->PSD.noBins);
  for (uint32_t i = 0; i < Sample->PSD.noBins; i++) {
    xPSD[i] = Sample->PSD.BinRanges[i];
    yPSD[i] = Sample->PSD.CFD[i];
  }
  ui->Qplot_PSD->graph(0)->setData(xPSD, yPSD);
  ui->Qplot_PSD->xAxis->setRange(0.01, 100);
  ui->Qplot_PSD->yAxis->setRange(0, 100);
  ui->Qplot_PSD->replot();
}

void VSAMainWindow::SetClassHistogram() {
  ui->QPlot_Texture->removePlottable(0);
  QVector<double> xClass(Sample->Shape.noBins), yClass(Sample->Shape.noBins);
  for (uint32_t i = 0; i < Sample->Shape.noBins; i++) {
    xClass[i] = i + 1;
    yClass[i] = Sample->Shape.bins[i];
  }
  QCPBars *ClassBars =
      new QCPBars(ui->QPlot_Texture->xAxis, ui->QPlot_Texture->yAxis);
  ui->QPlot_Texture->addPlottable(ClassBars);
  ClassBars->setName("Classification Histogram");
  ClassBars->setData(xClass, yClass);
  ClassBars->setWidthType(QCPBars::WidthType::wtPlotCoords);
  ClassBars->setWidth(1);
  ui->QPlot_Texture->xAxis->setRange(0, Sample->Shape.noBins);
  ui->QPlot_Texture->yAxis->setRange(0, Sample->Shape.HighestFrequency());
  ui->QPlot_Texture->xAxis->setAutoTicks(false);
  ui->QPlot_Texture->xAxis->setTickVector(xClass);
  ui->QPlot_Texture->replot();
}

void VSAMainWindow::on_actionNeuralNet_triggered() {
  if (nnWindow != nullptr) {
    nnWindow =
        new DialogNN(this, &Analyzer->NeuralNet, Settings, settingsWindow);
  }
  nnWindow->show();
}

void VSAMainWindow::on_actionNewSample_triggered() {
  if (Sample->ChangesSinceLastSave) {
    SaveMeMessage->setText(tr("Sample is not saved, Save sample?"));
    SaveMeMessage->addButton(QMessageBox::Abort);
    SaveMeMessage->addButton(QMessageBox::Close);
    if (SaveMeMessage->exec() == QMessageBox::Abort) {
      return;
    }
  }
  delete Sample;
  delete Images;
  Sample = new SoilAnalyzer::Sample;
  Images = new SoilAnalyzer::Analyzer::Images_t;
  TakeSnapShots();
  try {
    Analyzer->Analyse(Images, Sample, Settings);
  } catch (SoilAnalyzer::Exception::SoilAnalyzerException &e) {
    if (*e.id() == EXCEPTION_NO_SNAPSHOTS_NR) {
      CamError->showMessage(
          "No images acquired! Check you microscope settings");
      return;
    }
  }

  Sample->ChangesSinceLastSave = true;
}

void VSAMainWindow::TakeSnapShots() {
  Analyzer->SIfactorDet = true; // remeber to remove
  if (!Analyzer->SIfactorDet) {
    QMessageBox *DetSIFactor = new QMessageBox(this);
    DetSIFactor->setText("Put calibration Disc under the microscope");
    DetSIFactor->exec();
    on_actionCalibrate_triggered();
    DetSIFactor->setText("Place sample under the microscope");
    DetSIFactor->exec();
  }
  if (Settings->useBacklightProjection && !Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetFrame(newShot.FrontLight);
      BacklightMessage->exec();
      Microscope->GetFrame(newShot.BackLight);
      Images->push_back(newShot);
      QString ShakeMsg = "Shake it baby! ";
      int number = i - Settings->StandardNumberOfShots + 1;
      ShakeMsg.append(QString::number(number));
      ShakeMsg.append(" to go!");
      ShakeItBabyMessage->setText(ShakeMsg);
      ShakeItBabyMessage->exec();
    }
  } else if (Settings->useBacklightProjection && Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetHDRFrame(newShot.FrontLight, Settings->HDRframes);
      BacklightMessage->exec();
      Microscope->GetFrame(newShot.BackLight);
      Images->push_back(newShot);
      QString ShakeMsg = "Shake it baby! ";
      int number = i - Settings->StandardNumberOfShots + 1;
      ShakeMsg.append(QString::number(number));
      ShakeMsg.append(" to go!");
      ShakeItBabyMessage->setText(ShakeMsg);
      ShakeItBabyMessage->exec();
    }
  } else if (!Settings->useBacklightProjection && Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetHDRFrame(newShot.FrontLight, Settings->HDRframes);
      Images->push_back(newShot);
      QString ShakeMsg = "Shake it baby! ";
      int number = i - Settings->StandardNumberOfShots + 1;
      ShakeMsg.append(QString::number(number));
      ShakeMsg.append(" to go!");
      ShakeItBabyMessage->setText(ShakeMsg);
      ShakeItBabyMessage->exec();
    }
  } else if (!Settings->useBacklightProjection && !Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetFrame(newShot.FrontLight);
      Images->push_back(newShot);
      QString ShakeMsg = "Shake it baby! ";
      int number = i - Settings->StandardNumberOfShots + 1;
      ShakeMsg.append(QString::number(number));
      ShakeMsg.append(" to go!");
      ShakeItBabyMessage->setText(ShakeMsg);
      ShakeItBabyMessage->exec();
    }
  }
}

void VSAMainWindow::on_actionSaveSample_triggered() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save Sample"), QString::fromStdString(Settings->SampleFolder),
      tr("Sample (*.VSA)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".VSA"))) {
      fn.append(tr(".VSA"));
    }
    Sample->ChangesSinceLastSave = false;
    Sample->Save(fn.toStdString());
  }
}

void VSAMainWindow::on_actionLoadSample_triggered() {
  if (Sample->ChangesSinceLastSave) {
    SaveMeMessage->setText(tr("Sample is not saved, Save sample?"));
    SaveMeMessage->addButton(QMessageBox::Abort);
    SaveMeMessage->addButton(QMessageBox::Close);
    if (SaveMeMessage->exec() == QMessageBox::Abort) {
      return;
    }
  }
  delete Sample;
  delete Images;
  Sample = new SoilAnalyzer::Sample;
  Images = new SoilAnalyzer::Analyzer::Images_t;

  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open Sample"), QString::fromStdString(Settings->SampleFolder),
      tr("Settings (*.VSA)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".VSA"))) {
      fn.append(tr(".VSA"));
    }
    Sample->Load(fn.toStdString());
    on_analyzer_finished();
  }
}

void VSAMainWindow::on_actionUseLearning_toggled(bool arg1) {
  Analyzer->PredictShape = !arg1;
}

void VSAMainWindow::on_actionCalibrate_triggered() {
  cv::Mat calib;
  Microscope->GetFrame(calib);
  Analyzer->CalibrateSI(16.25, calib);
}

void VSAMainWindow::on_Classification_changed(int newValue) {
  ui->widget_ParticleDisplay->SelectedParticle->Classification.Category =
      newValue;
  ui->widget_ParticleDisplay->SelectedParticle->Classification.ManualSet = true;
  Sample->ChangesSinceLastSave = true;
  Analyzer->Analyse();
}

void VSAMainWindow::on_particle_deleted() { Analyzer->Analyse(); }
