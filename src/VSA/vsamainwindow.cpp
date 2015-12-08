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
  SaveMeMessage->setText(tr("Sample is not saved, Save sample?"));
  SaveMeMessage->addButton(QMessageBox::Abort);
  SaveMeMessage->addButton(QMessageBox::Close);

  BacklightMessage = new QMessageBox(this);
  BacklightMessage->setText("Turn off Frontlight! Turn on Backlight!");
  ShakeItBabyMessage = new QMessageBox(this);

  // Load the Microscope
  Microscope = new Hardware::Microscope;
  Microscope->noOfshots = Settings->noOfShots;
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
        CamError->showMessage(
            tr("No cams found! Connect the cam and set the default"));
        Settings->defaultWebcam = Microscope->AvailableCams[0].Name;
        Settings->selectedResolution = 0;
        Microscope->SelectedCam = &Microscope->AvailableCams[0];
        settingsWindow = new DialogSettings(this, Settings, Microscope);
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
  // Setup the plot linestyles;
  QPen pdfPen;
  pdfPen.setColor(QColor("gray"));
  pdfPen.setStyle(Qt::DashDotDotLine);
  pdfPen.setWidthF(1);

  QPen meanPen;
  meanPen.setColor(QColor("darkBlue"));
  meanPen.setStyle(Qt::DashLine);
  meanPen.setWidthF(1);

  QPen binPen;
  binPen.setColor((QColor("blue")));
  binPen.setStyle(Qt::SolidLine);
  binPen.setWidthF(2);

  // Setup the PSD plot
  QCPPlotTitle *PSDtitle = new QCPPlotTitle(ui->Qplot_PSD);
  PSDtitle->setText("Particle Size Distribution");
  PSDtitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->Qplot_PSD->plotLayout()->insertRow(0);
  ui->Qplot_PSD->plotLayout()->addElement(0, 0, PSDtitle);

  ui->Qplot_PSD->addGraph(ui->Qplot_PSD->xAxis, ui->Qplot_PSD->yAxis);
  ui->Qplot_PSD->graph(0)
      ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));
  ui->Qplot_PSD->graph(0)->setPen(binPen);
  ui->Qplot_PSD->graph(0)->setName("Particle Size Distribution");
  ui->Qplot_PSD->graph(0)->addToLegend();

  ui->Qplot_PSD->xAxis->setLabel("Particle size [mm]");
  ui->Qplot_PSD->xAxis->setRange(0.01, 10);
  ui->Qplot_PSD->xAxis->setAutoTicks(false);
  ui->Qplot_PSD->xAxis->setTickVector(QVector<double>::fromStdVector(PSDTicks));
  ui->Qplot_PSD->xAxis->setTickLabelRotation(30);
  ui->Qplot_PSD->xAxis->setTickLabelFont(QFont("sans", 8, QFont::Normal));
  ui->Qplot_PSD->xAxis->setScaleType(QCPAxis::stLogarithmic);

  QFont legendfont;
  legendfont.setPointSize(10);
  ui->Qplot_PSD->legend->setFont(legendfont);
  ui->Qplot_PSD->legend->setSelectedFont(legendfont);
  ui->Qplot_PSD->legend->setVisible(true);
  ui->Qplot_PSD->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignLeft);

  ui->Qplot_PSD->yAxis->setLabel("Percentage [%]");
  ui->Qplot_PSD->yAxis->setRange(0, 100);
  ui->Qplot_PSD->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->Qplot_PSD->yAxis->grid()->setSubGridVisible(true);

  connect(ui->Qplot_PSD, SIGNAL(mouseDoubleClick(QMouseEvent *)), this,
          SLOT(on_reset_graph(QMouseEvent *)));
  ui->Qplot_PSD->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->Qplot_PSD, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(on_PSD_contextMenuRequest(QPoint)));

  // Setup the Roundness plot
  QCPPlotTitle *Roundnesstitle = new QCPPlotTitle(ui->QPlot_Roudness);
  Roundnesstitle->setText("Sphericity Histogram");
  Roundnesstitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Roudness->plotLayout()->insertRow(0);
  ui->QPlot_Roudness->plotLayout()->addElement(0, 0, Roundnesstitle);

  ui->QPlot_Roudness->addGraph(ui->QPlot_Roudness->xAxis,
                               ui->QPlot_Roudness->yAxis2);
  ui->QPlot_Roudness->addGraph(ui->QPlot_Roudness->xAxis,
                               ui->QPlot_Roudness->yAxis2);
  ui->QPlot_Roudness->graph(0)->setPen(pdfPen);
  ui->QPlot_Roudness->graph(1)->setPen(meanPen);

  RoundnessBars =
      new QCPBars(ui->QPlot_Roudness->xAxis, ui->QPlot_Roudness->yAxis);
  ui->QPlot_Roudness->addPlottable(RoundnessBars);
  RoundnessBars->setPen(binPen);

  ui->QPlot_Roudness->xAxis->setAutoTicks(false);
  ui->QPlot_Roudness->xAxis->setAutoTickLabels(false);
  ui->QPlot_Roudness->xAxis->setTickVector(
      QVector<double>::fromStdVector(RoundnessTicks));
  ui->QPlot_Roudness->xAxis->setTickVectorLabels(RoundnessCat);
  ui->QPlot_Roudness->xAxis->setTickLabelRotation(30);
  ui->QPlot_Roudness->xAxis->setSubTickCount(0);
  ui->QPlot_Roudness->xAxis->setTickLength(0, 4);
  ui->QPlot_Roudness->xAxis->grid()->setVisible(true);
  ui->QPlot_Roudness->xAxis->setRange(0, 4);
  ui->QPlot_Roudness->xAxis->setLabel("Count [-]");
  ui->QPlot_Roudness->xAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Roudness->xAxis->setTickLabelFont(QFont("sans", 8, QFont::Normal));
  ui->QPlot_Roudness->xAxis->setPadding(25);
  ui->QPlot_Roudness->yAxis->setLabel("Sphericity [-]");
  ui->QPlot_Roudness->yAxis->setLabelFont(QFont("sans", 8, QFont::Bold));

  // Setup the angularity plot
  QCPPlotTitle *Angularitytitle = new QCPPlotTitle(ui->QPlot_Angularity);
  Angularitytitle->setText("Angularity Histogram");
  Angularitytitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Angularity->plotLayout()->insertRow(0);
  ui->QPlot_Angularity->plotLayout()->addElement(0, 0, Angularitytitle);

  ui->QPlot_Angularity->addGraph(ui->QPlot_Angularity->xAxis,
                                 ui->QPlot_Angularity->yAxis2);
  ui->QPlot_Angularity->addGraph(ui->QPlot_Angularity->xAxis,
                                 ui->QPlot_Angularity->yAxis2);
  AngularityBars =
      new QCPBars(ui->QPlot_Angularity->xAxis, ui->QPlot_Angularity->yAxis);
  ui->QPlot_Angularity->addPlottable(AngularityBars);
  AngularityBars->setPen(binPen);

  ui->QPlot_Angularity->xAxis->setAutoTicks(false);
  ui->QPlot_Angularity->xAxis->setAutoTickLabels(false);
  ui->QPlot_Angularity->xAxis->setTickVector(
      QVector<double>::fromStdVector(AngularityTicks));
  ui->QPlot_Angularity->xAxis->setTickVectorLabels(AngularityCat);
  ui->QPlot_Angularity->xAxis->setTickLabelRotation(30);
  ui->QPlot_Angularity->xAxis->setSubTickCount(0);
  ui->QPlot_Angularity->xAxis->setTickLength(0, 4);
  ui->QPlot_Angularity->xAxis->grid()->setVisible(true);
  ui->QPlot_Angularity->xAxis->setRange(0, 7);
  ui->QPlot_Angularity->xAxis->setLabel("Count [-]");
  ui->QPlot_Angularity->xAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Angularity->xAxis->setTickLabelFont(
      QFont("sans", 8, QFont::Normal));
  ui->QPlot_Angularity->yAxis->setLabel("Sphericity [-]");
  ui->QPlot_Angularity->yAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Angularity->graph(0)->setPen(pdfPen);
  ui->QPlot_Angularity->graph(1)->setPen(meanPen);

  // Setup the Amplitude diagram
  QCPPlotTitle *Amptitle = new QCPPlotTitle(ui->QPlot_Amp);
  Amptitle->setText("Fast Fourier Amplitude for the current particle");
  Amptitle->setFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Amp->plotLayout()->insertRow(0);
  ui->QPlot_Amp->plotLayout()->addElement(0, 0, Amptitle);

  ui->QPlot_Amp->addGraph(ui->QPlot_Amp->xAxis, ui->QPlot_Amp->yAxis);

  ui->QPlot_Amp->xAxis->setTickLabelRotation(30);
  ui->QPlot_Amp->xAxis->setSubTickCount(0);
  ui->QPlot_Amp->xAxis->setTickLength(0, 4);
  ui->QPlot_Amp->xAxis->grid()->setVisible(true);
  ui->QPlot_Amp->xAxis->setRange(0, 512);
  ui->QPlot_Amp->xAxis->setLabel("Frequency [-]");
  ui->QPlot_Amp->xAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Amp->xAxis->setTickLabelFont(QFont("sans", 8, QFont::Normal));
  ui->QPlot_Amp->yAxis->setLabel("Amplitude [-]");
  ui->QPlot_Amp->yAxis->setLabelFont(QFont("sans", 8, QFont::Bold));
  ui->QPlot_Amp->yAxis->setScaleType(QCPAxis::stLogarithmic);
  ui->QPlot_Amp->graph()->setPen(binPen);
  ui->QPlot_Amp->graph()->setLineStyle(QCPGraph::lsLine);
  ui->QPlot_Amp->graph()->setBrush(QBrush(QColor(50, 50, 200, 40)));

  // Connect the Particle display and Selector
  connect(ui->widget_ParticleSelector, SIGNAL(valueChanged(int)), this,
          SLOT(on_Classification_changed(int)));
  connect(ui->widget_ParticleDisplay, SIGNAL(shapeClassificationChanged(int)),
          ui->widget_ParticleSelector, SLOT(setValue(int)));
  connect(ui->widget_ParticleDisplay, SIGNAL(particleDeleted()), this,
          SLOT(on_particle_deleted()));
  connect(ui->widget_ParticleDisplay, SIGNAL(particleChanged(int)), this,
          SLOT(on_particleChanged(int)));

  // Setup the bar
  ui->actionUseLearning->setChecked(Settings->PredictTheShape);

  // Setup the widgets
  ui->widget_ParticleSelector->setDisabled(true);
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

void VSAMainWindow::on_actionSettings_triggered() {
  settingsWindow->openTab(0);
  settingsWindow->show();
}

void VSAMainWindow::on_analyzer_finished() {
  if (!ParticleDisplayerFilled && Sample->ParticlePopulation.size() > 0) {
    ui->widget_ParticleDisplay->SetSample(Sample);
  }
  SetPSDgraph();
  setRoundnessHistogram();
  setAngularityHistogram();
  ParticleDisplayerFilled = true;
}

void VSAMainWindow::SetPSDgraph() {
  std::vector<double> stdPSDvalue(Sample->PSD.CFD, Sample->PSD.CFD + 15);
  ui->Qplot_PSD->graph(0)->setData(PSDTicks, stdPSDvalue);
  ui->Qplot_PSD->replot();
}

void VSAMainWindow::setRoundnessHistogram() {
  // Setup the Histogram bins
  std::vector<double> stdValues(Sample->Roundness.bins + 1,
                                Sample->Roundness.bins + 4);

  ui->QPlot_Roudness->yAxis->setRange(
      0, static_cast<double>(Sample->Roundness.HighestFrequency()));
  RoundnessBars->setData(RoundnessTicks, stdValues);

  // Setup the Prediction Density Function
  std::vector<double> stdPDFkey, stdPDFvalues;
  Sample->Roundness.GetPDFfunction(stdPDFkey, stdPDFvalues, 0.2, 0, 4);
  ui->QPlot_Roudness->graph(0)->setData(stdPDFkey, stdPDFvalues);
  ui->QPlot_Roudness->yAxis2->setRange(0, Sample->Roundness.HighestPDF);

  // Setup the mean Vector
  QVector<double> meanKey(2, static_cast<double>(Sample->Roundness.Mean));
  QVector<double> meanValue(2);
  meanValue[0] = 0;
  meanValue[1] = Sample->Roundness.HighestPDF;
  ui->QPlot_Roudness->graph(1)->setData(meanKey, meanValue);
  ui->QPlot_Roudness->replot();
}

void VSAMainWindow::setAngularityHistogram() {
  // Setup the Histogram bins
  std::vector<double> stdValues(Sample->Angularity.bins + 1,
                                Sample->Angularity.bins + 7);

  ui->QPlot_Angularity->yAxis->setRange(
      0, static_cast<double>(Sample->Angularity.HighestFrequency()));
  AngularityBars->setData(AngularityTicks, stdValues);

  // Setup the Prediction Density Function
  std::vector<double> stdPDFkey, stdPDFvalues;
  Sample->Angularity.GetPDFfunction(stdPDFkey, stdPDFvalues, 0.2, 0, 7);
  ui->QPlot_Angularity->graph(0)->setData(stdPDFkey, stdPDFvalues);
  ui->QPlot_Angularity->yAxis2->setRange(0, Sample->Angularity.HighestPDF);

  // Setup the mean Vector
  QVector<double> meanKey(2, static_cast<double>(Sample->Angularity.Mean));
  QVector<double> meanValue(2);
  meanValue[0] = 0;
  meanValue[1] = Sample->Angularity.HighestPDF;
  ui->QPlot_Angularity->graph(1)->setData(meanKey, meanValue);
  ui->QPlot_Angularity->replot();
}

void VSAMainWindow::setAmpgraph() {
  ui->QPlot_Amp->graph(0)->clearData();
  ComplexVect_t *comp =
      &ui->widget_ParticleDisplay->SelectedParticle->FFDescriptors;
  uint32_t count = (comp->size() > 64) ? 64 : comp->size();
  for (uint32_t i = 0; i < count; i++) {
    ui->QPlot_Amp->graph(0)->addData(i, abs(comp->at(i)));
  }
  ui->QPlot_Amp->rescaleAxes();
  ui->QPlot_Amp->replot();
}

void VSAMainWindow::on_particleChanged(int newPart) { setAmpgraph(); }

void VSAMainWindow::on_actionNeuralNet_triggered() {
  if (nnWindow != nullptr) {
    nnWindow =
        new DialogNN(this, &Analyzer->NeuralNet, Settings, settingsWindow);
  }
  nnWindow->show();
}

void VSAMainWindow::on_actionNewSample_triggered() {
  if (Sample->ChangesSinceLastSave) {
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
  Sample->isAnalysed = true;
  Sample->ChangesSinceLastSave = true;
  if (Sample->ParticlePopulation.size() > 0) {
    ui->widget_ParticleSelector->setDisabled(
        false,
        ui->widget_ParticleDisplay->SelectedParticle->Classification.Category);
  }
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
      ShowShaker(i);
    }
  } else if (Settings->useBacklightProjection && Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetHDRFrame(newShot.FrontLight, Settings->HDRframes);
      BacklightMessage->exec();
      Microscope->GetFrame(newShot.BackLight);
      Images->push_back(newShot);
      ShowShaker(i);
    }
  } else if (!Settings->useBacklightProjection && Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetHDRFrame(newShot.FrontLight, Settings->HDRframes);
      Images->push_back(newShot);
      ShowShaker(i);
    }
  } else if (!Settings->useBacklightProjection && !Settings->useHDR) {
    for (uint32_t i = 0; i < Settings->StandardNumberOfShots; i++) {
      SoilAnalyzer::Analyzer::Image_t newShot;
      newShot.SIPixelFactor = Analyzer->CurrentSIfactor;
      Microscope->GetFrame(newShot.FrontLight);
      Images->push_back(newShot);
      ShowShaker(i);
    }
  }
}

void VSAMainWindow::ShowShaker(int i) {
  int number = Settings->StandardNumberOfShots - i - 1;
  if (number != 0) {
    QString ShakeMsg = "Shake it baby! ";
    ShakeMsg.append(QString::number(number));
    ShakeMsg.append(" to go!");
    ShakeItBabyMessage->setText(ShakeMsg);
    ShakeItBabyMessage->exec();
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
    if (SaveMeMessage->exec() == QMessageBox::Abort) {
      return;
    }
  }

  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open Sample"), QString::fromStdString(Settings->SampleFolder),
      tr("Sample (*.VSA)"));
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
    } catch (boost::archive::archive_exception &e) {
      // qDebug() << *e.what();
    }
    ParticleDisplayerFilled = false;
    Sample->Angularity.Data = Sample->GetAngularityVector()->data();
    Sample->Roundness.Data = Sample->GetRoundnessVector()->data();
    Sample->PSD.Data = Sample->GetPSDVector()->data();
    Analyzer->Results = Sample;
    on_analyzer_finished();
    ui->widget_ParticleSelector->setDisabled(
        false,
        ui->widget_ParticleDisplay->SelectedParticle->Classification.Category);
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
  uint8_t *Cat =
      &ui->widget_ParticleDisplay->SelectedParticle->Classification.Category;
  if ((*Cat - 1) % 6 != (newValue - 1) % 6) {
    Sample->ParticleChangedStateAngularity = true;
  }
  if ((*Cat - 1) / 6 != (newValue - 1) / 6) {
    Sample->ParticleChangedStateRoundness = true;
  }
  ui->widget_ParticleDisplay->SelectedParticle->Classification.Category =
      newValue;
  ui->widget_ParticleDisplay->SelectedParticle->Classification.ManualSet = true;
  Sample->ChangesSinceLastSave = true;
  Analyzer->Analyse();
  ui->widget_ParticleDisplay->next();
}

void VSAMainWindow::on_particle_deleted() { Analyzer->Analyse(); }

void VSAMainWindow::on_actionAutomatic_Shape_Pediction_triggered(bool checked) {
  Settings->PredictTheShape = checked;
}

void VSAMainWindow::on_reset_graph(QMouseEvent *e) {
  ui->Qplot_PSD->xAxis->setRange(0, 10);
  ui->Qplot_PSD->yAxis->setRange(0, 100);
  ui->Qplot_PSD->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->Qplot_PSD->replot();
}

void VSAMainWindow::on_actionReport_Generator_triggered() {
  if (ReportGenWindow == nullptr) {
    ReportGenWindow =
        new QReportGenerator(this, Sample, Settings, ui->Qplot_PSD,
                             ui->QPlot_Roudness, ui->QPlot_Angularity);
  }
  ReportGenWindow->show();
}

void VSAMainWindow::on_PSD_contextMenuRequest(QPoint point) {
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  menu->addAction("Compare against...", this, SLOT(on_compare_against()));
  menu->addAction("Restore", this, SLOT(on_restore_PSD()));
  menu->popup(ui->Qplot_PSD->mapToGlobal(point));
}

void VSAMainWindow::on_compare_against() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open CSV"), QString::fromStdString(Settings->SampleFolder),
      tr("Comma Seperated Value (*.csv)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".csv"))) {
      fn.append(tr(".csv"));
    }

    if (ui->Qplot_PSD->graphCount() > 1) {
      ui->Qplot_PSD->legend->removeItem(1);
      ui->Qplot_PSD->removeGraph(1);
    }

    QStringList rows;
    QStringList cellValues;

    QFile f(fn);
    if (f.open(QIODevice::ReadOnly)) {
      QString data;
      data = f.readAll();
      rows = data.split('\n');
      f.close();
      for (uint32_t i = 0; i < rows.size(); i++) {
        QStringList cols = rows[i].split(',');
        for (uint32_t j = 0; j < cols.size(); j++) {
          cellValues.append(cols[j]);
        }
      }
      cellValues.removeLast();

      std::vector<double> compValues(15);
      for (uint32_t i = 0; i < cellValues.size(); i += 4) {
        bool conversionSucces = false;
        double binValue = cellValues[i].toDouble(&conversionSucces);
        qDebug() << cellValues[i + 3];
        if (conversionSucces) {
          for (uint32_t j = 0; j < 15; j++) {
            if (binValue == PSDTicks[j]) {
              compValues[j] = cellValues[i + 3].toDouble();
            }
          }
        }
      }
      ui->Qplot_PSD->addGraph(ui->Qplot_PSD->xAxis, ui->Qplot_PSD->yAxis);
      ui->Qplot_PSD->graph(1)->setData(PSDTicks, compValues);
      QPen compPen;
      compPen.setColor(QColor("darkBlue"));
      compPen.setStyle(Qt::DashLine);
      compPen.setWidthF(1);
      ui->Qplot_PSD->graph(1)->setPen(compPen);
      ui->Qplot_PSD->graph(1)->setName("Compared Particle Size Distribution");
      ui->Qplot_PSD->graph(1)->addToLegend();
      ui->Qplot_PSD->replot();
    }
  }
}

void VSAMainWindow::on_restore_PSD() {
  if (ui->Qplot_PSD->graphCount() > 1) {
    ui->Qplot_PSD->legend->removeItem(1);
    ui->Qplot_PSD->removeGraph(1);
  }
  on_reset_graph(nullptr);
}

void VSAMainWindow::on_actionExport_triggered() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save CSV"), QString::fromStdString(Settings->SampleFolder),
      tr("Export file (*.csv)"));
  if (!fn.isEmpty() && Sample->isAnalysed) {
    if (!fn.contains(tr(".csv"))) {
      fn.append(tr(".csv"));

      SoilAnalyzer::Analyzer::ExportParticles_t particles = Analyzer->Export();

      QFile f(fn);
      if (f.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream stream(&f);
        stream << "Radius\tArea\tSIfactor\tSphericity\n";
        std::for_each(particles.begin(), particles.end(),
                      [&](SoilAnalyzer::Analyzer::ExportData_t &E) {
                        stream << E.Radius << "\t" << E.Area << "\t"
                               << E.SIfactor << "\t" << E.Sphericity << "\n";
                      });
        f.close();
      }
    }
  }
}
