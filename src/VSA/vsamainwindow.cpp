#include "vsamainwindow.h"
#include "ui_vsamainwindow.h"

VSAMainWindow::VSAMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::VSAMainWindow) {
  ui->setupUi(this);

  // Load the usersettings
  Settings = new SoilAnalyzer::SoilSettings;
  Settings->LoadSettings("Settings/User.ini");

  // Set the Error windows
  CamError = new QErrorMessage(this);

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
  ui->widget_ParticleDisplay->SetParticlePopulation(
      &Sample->ParticlePopulation);

  //  float PSD_Y[sample->PSD.noBins];
  //  for (uint32_t i = 0; i < sample->PSD.noBins; i++) {
  //      PSD_Y = i * (1 / sample->PSD.noBins);
  //    }

  //  ui->Qplot_PSD->graph(0)->setData(sample->PSD.CFD, PSD_Y);
}

void VSAMainWindow::on_actionNeuralNet_triggered() {
  if (nnWindow != nullptr) {
      nnWindow = new DialogNN(this, &Analyzer->NeuralNet, Settings, settingsWindow);
  }
  nnWindow->show();
}
