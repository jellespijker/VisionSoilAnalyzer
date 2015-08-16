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

  // Setup the PSD plot
  QCPPlotTitle *PSDtitle = new QCPPlotTitle(ui->Qplot_PSD);
  PSDtitle->setText("Particle Size Distribution");
  PSDtitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->Qplot_PSD->plotLayout()->insertRow(0);
  ui->Qplot_PSD->plotLayout()->addElement(0, 0, PSDtitle);

  ui->Qplot_PSD->addGraph();
  ui->Qplot_PSD->graph(0)
      ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));

  ui->Qplot_PSD->xAxis->setLabel("Particle size [mm]");
  ui->Qplot_PSD->xAxis->setRange(0.01, 10);
  ui->Qplot_PSD->xAxis->setScaleType(QCPAxis::stLogarithmic);

  ui->Qplot_PSD->yAxis->setLabel("Percentage [%]");
  ui->Qplot_PSD->yAxis->setRange(0, 100);
  ui->Qplot_PSD->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->Qplot_PSD->yAxis->grid()->setSubGridVisible(true);

  // Setup the Roundness plot
  QCPPlotTitle *Roundnesstitle = new QCPPlotTitle(ui->QPlot_Roudness);
  Roundnesstitle->setText("Sphericity Histogram");
  Roundnesstitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Roudness->plotLayout()->insertRow(0);
  ui->QPlot_Roudness->plotLayout()->addElement(0, 0, Roundnesstitle);

  RoundnessBars =
      new QCPBars(ui->QPlot_Roudness->xAxis, ui->QPlot_Roudness->yAxis);
  ui->Qplot_PSD->addPlottable(RoundnessBars);

  ui->QPlot_Roudness->xAxis->setAutoTicks(false);
  ui->QPlot_Roudness->xAxis->setAutoTickLabels(false);
  ui->QPlot_Roudness->xAxis->setTickVector(RoundnessTicks);
  ui->QPlot_Roudness->xAxis->setTickVectorLabels(RoundnessCat);
  ui->QPlot_Roudness->xAxis->setTickLabelRotation(30);
  ui->QPlot_Roudness->xAxis->setSubTickCount(0);
  ui->QPlot_Roudness->xAxis->setTickLength(0, 4);
  ui->QPlot_Roudness->xAxis->grid()->setVisible(true);
  ui->QPlot_Roudness->xAxis->setRange(0, 4);
  ui->QPlot_Roudness->xAxis->setLabel("Count [-]");
  ui->QPlot_Roudness->xAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Roudness->xAxis->setTickLabelFont(QFont("sans", 8, QFont::Light));
  ui->QPlot_Roudness->xAxis->setPadding(25);
  ui->QPlot_Roudness->yAxis->setLabel("Sphericity [-]");
  ui->QPlot_Roudness->yAxis->setLabelFont(QFont("sans", 8, QFont::Bold));

  // Setup the angularity plot
  ui->QPlot_Angularity->addGraph(ui->QPlot_Angularity->xAxis2,
                                 ui->QPlot_Angularity->yAxis2);

  QCPPlotTitle *Angularitytitle = new QCPPlotTitle(ui->QPlot_Angularity);
  Angularitytitle->setText("Angularity Histogram");
  Angularitytitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Angularity->plotLayout()->insertRow(0);
  ui->QPlot_Angularity->plotLayout()->addElement(0, 0, Angularitytitle);
  AngularityBars =
      new QCPBars(ui->QPlot_Angularity->xAxis, ui->QPlot_Angularity->yAxis);
  ui->QPlot_Angularity->addPlottable(AngularityBars);
  ui->QPlot_Angularity->xAxis->setAutoTicks(false);
  ui->QPlot_Angularity->xAxis->setAutoTickLabels(false);
  ui->QPlot_Angularity->xAxis->setTickVector(AngularityTicks);
  ui->QPlot_Angularity->xAxis->setTickVectorLabels(AngularityCat);
  ui->QPlot_Angularity->xAxis->setTickLabelRotation(30);
  ui->QPlot_Angularity->xAxis->setSubTickCount(0);
  ui->QPlot_Angularity->xAxis->setTickLength(0, 4);
  ui->QPlot_Angularity->xAxis->grid()->setVisible(true);
  ui->QPlot_Angularity->xAxis->setRange(0, 7);
  ui->QPlot_Angularity->xAxis->setLabel("Count [-]");
  ui->QPlot_Angularity->xAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Angularity->xAxis->setTickLabelFont(QFont("sans", 8, QFont::Light));
  ui->QPlot_Angularity->yAxis->setLabel("Sphericity [-]");
  ui->QPlot_Angularity->yAxis->setLabelFont(QFont("sans", 8, QFont::Bold));

  // Connect the Particle display and Selector
  connect(ui->widget_ParticleSelector, SIGNAL(valueChanged(int)), this,
          SLOT(on_Classification_changed(int)));
  connect(ui->widget_ParticleDisplay, SIGNAL(shapeClassificationChanged(int)),
          ui->widget_ParticleSelector, SLOT(setValue(int)));
  connect(ui->widget_ParticleDisplay, SIGNAL(particleDeleted()), this,
          SLOT(on_particle_deleted()));
}

VSAMainWindow::~VSAMainWindow() {
  delete Settings;
  delete Microscope;
  delete Analyzer;
  delete Sample;
  delete Images;

  delete settingsWindow;
  delete nnWindow;
  delete CamError;
  delete SaveMeMessage;
  delete BacklightMessage;
  delete ShakeItBabyMessage;
  delete ui;
}

void VSAMainWindow::on_actionSettings_triggered() { settingsWindow->show(); }

void VSAMainWindow::on_analyzer_finished() {
  if (!ParticleDisplayerFilled) {
    ui->widget_ParticleDisplay->SetSample(Sample);
  }
  SetPSDgraph();
  setRoundnessHistogram();
  setAngularityHistogram();
  ParticleDisplayerFilled = true;
}

void VSAMainWindow::SetPSDgraph() {
  ui->Qplot_PSD->graph(0)->clearData();
  QVector<double> xPSD(Sample->PSD.noBins), yPSD(Sample->PSD.noBins);
  for (uint32_t i = 0; i < Sample->PSD.noBins; i++) {
    xPSD[i] = static_cast<double>(Sample->PSD.BinRanges[i]);
    yPSD[i] = static_cast<double>(Sample->PSD.CFD[i]);
  }
  ui->Qplot_PSD->graph(0)->setData(xPSD, yPSD);
  ui->Qplot_PSD->replot();
}

void VSAMainWindow::setRoundnessHistogram() {
  std::vector<double> stdValues(Sample->Roundness.bins + 1,
                                Sample->Roundness.bins + 4);
  QVector<double> Values = QVector<double>::fromStdVector(stdValues);

  ui->QPlot_Roudness->yAxis->setRange(
      0, static_cast<double>(Sample->Roundness.HighestFrequency()));
  RoundnessBars->setData(RoundnessTicks, Values);
  ui->QPlot_Roudness->replot();
}

void VSAMainWindow::setAngularityHistogram() {
  std::vector<double> stdValues(Sample->Angularity.bins + 1,
                                Sample->Angularity.bins + 7);
  QVector<double> Values = QVector<double>::fromStdVector(stdValues);

  ui->QPlot_Angularity->yAxis->setRange(
      0, static_cast<double>(Sample->Angularity.HighestFrequency()));
  AngularityBars->setData(AngularityTicks, Values);
  ui->QPlot_Angularity->replot();
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
  Sample = nullptr;
  delete Images;
  Images = nullptr;
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
    Sample->IsLoadedFromDisk = true;
    Sample->ChangesSinceLastSave = false;
    Sample->Save(fn.toStdString());
    qDebug() << "Saving finished";
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

  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open Sample"), QString::fromStdString(Settings->SampleFolder),
      tr("Settings (*.VSA)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".VSA"))) {
      fn.append(tr(".VSA"));
    }
    delete Sample;
    Sample = nullptr;
    delete Images;
    Images = nullptr;
    Sample = new SoilAnalyzer::Sample;
    Images = new SoilAnalyzer::Analyzer::Images_t;
    try {
    Sample->Load(fn.toStdString());
    }
    catch (boost::archive::archive_exception &e) {
      //qDebug() << *e.what();
    }
    ParticleDisplayerFilled = false;
    Sample->Angularity.Data = Sample->GetAngularityVector()->data();
    Sample->Roundness.Data = Sample->GetRoundnessVector()->data();
    Sample->PSD.Data = Sample->GetPSDVector()->data();
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
  Sample->ParticleChangedStateAngularity = true;
  Sample->ParticleChangedStateClass = true;
  Sample->ParticleChangedStatePSD = true;
  Sample->ParticleChangedStateRoundness = true;
  Analyzer->Analyse();
}

void VSAMainWindow::on_particle_deleted() { Analyzer->Analyse(); }
