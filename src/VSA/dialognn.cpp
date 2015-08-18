#include "dialognn.h"
#include "ui_dialognn.h"

DialogNN::DialogNN(QWidget *parent, SoilMath::NN *neuralnet,
                   SoilAnalyzer::SoilSettings *settings,
                   DialogSettings *settingsWindow)
    : QDialog(parent), ui(new Ui::DialogNN) {
  ui->setupUi(this);

  if (neuralnet == nullptr) {
    neuralnet = new SoilMath::NN;
  }
  NeuralNet = neuralnet;
  if (settings == nullptr) {
    settings = new SoilAnalyzer::SoilSettings;
  }
  Settings = settings;
  if (settingsWindow == nullptr) {
    settingsWindow = new DialogSettings;
  }
  SettingsWindow = settingsWindow;

  // Setup the Qplots
  ui->widget_NNError->addGraph();
  ui->widget_NNError->addGraph();

  ui->widget_NNError->xAxis->setLabel("Generation [-]");
  ui->widget_NNError->yAxis->setLabel("Error [%]");
  QCPPlotTitle *widget_NNErrorTitle = new QCPPlotTitle(ui->widget_NNError);
  widget_NNErrorTitle->setText("Learning error");
  widget_NNErrorTitle->setFont(QFont("sans", 10, QFont::Bold));
  ui->widget_NNError->plotLayout()->insertRow(0);
  ui->widget_NNError->plotLayout()->addElement(0, 0, widget_NNErrorTitle);

  setupErrorGraph();

  // Connect the NN learn error
  connect(NeuralNet, SIGNAL(learnErrorUpdate(double)), this,
          SLOT(on_learnErrorUpdate(double)));
}

DialogNN::~DialogNN() { delete ui; }

void DialogNN::on_pushButton_Settings_clicked() {
  SettingsWindow->openTab(2);
  SettingsWindow->show();
  setupErrorGraph();
}

void DialogNN::on_learnErrorUpdate(double newError) {
  ui->widget_NNError->graph(0)->addData(currentGeneration, newError);
  currentGeneration += 1;
  // ui->widget_NNError->yAxis->rescale();
  ui->widget_NNError->yAxis->setRange(0, 100);
  ui->widget_NNError->replot();
}

void DialogNN::setupErrorGraph() {
  errorTicks.clear();
  for (uint32_t i = 0; i < NeuralNet->MaxGenUsedByGA; i++) {
    errorTicks.push_back(i);
  }
  ui->widget_NNError->xAxis->setRange(0, NeuralNet->MaxGenUsedByGA);
  QVector<double> endErrorValue(2, NeuralNet->EndErrorUsedByGA);
  QVector<double> endErrorKey(2, 0);
  endErrorKey[1] = NeuralNet->MaxGenUsedByGA;
  ui->widget_NNError->graph(1)->setData(endErrorKey, endErrorValue);
  ui->widget_NNError->xAxis->setAutoTicks(false);
  ui->widget_NNError->xAxis->setTickVector(errorTicks);
  ui->widget_NNError->xAxis->setTickLabels(false);
  //ui->widget_NNError->yAxis->setScaleType(QCPAxis::stLogarithmic);
  ui->widget_NNError->replot();
}

void DialogNN::on_pushButton_SelectSamples_clicked() {
  fn = QFileDialog::getOpenFileNames(
      this, tr("Open Samples"), QString::fromStdString(Settings->SampleFolder),
      tr("Samples (*.VSA)"));
  for_each(fn.begin(), fn.end(), [](QString &f) {
    if (!f.contains(tr(".VSA"))) {
      f.append(tr(".VSA"));
    }
  });
}

void DialogNN::on_pushButton_Learn_clicked() {
  if (fn.size() < 1) {
    return;
  }
  InputLearnVector_t InputVec;
  OutputLearnVector_t OutputVec;
  makeLearnVectors(InputVec, OutputVec);
  NeuralNet->Learn(InputVec, OutputVec, NeuralNet->GetInputNeurons());
}

void DialogNN::makeLearnVectors(InputLearnVector_t &input,
                                OutputLearnVector_t &output) {
  for (uint32_t i = 0; i < fn.size(); i++) {
    SoilAnalyzer::Sample sample;
    sample.Load(fn[i].toStdString());
    for_each(sample.ParticlePopulation.begin(), sample.ParticlePopulation.end(),
             [&](SoilAnalyzer::Particle &P) {
               if (P.FFDescriptors.size() >= NeuralNet->GetInputNeurons()) {
                 ComplexVect_t ffdesc;
                 for (uint32_t j = 0; j < NeuralNet->GetInputNeurons(); j++) {
                   ffdesc.push_back(P.FFDescriptors[j]);
                 }
                 input.push_back(ffdesc);
                 Predict_t predict = P.Classification;
                 predict.OutputNeurons = SoilMath::makeOutput(P.Classification.Category, NeuralNet->GetOutputNeurons());
                 output.push_back(predict);
               }
             });
  }
}

void DialogNN::on_pushButton_SaveNN_clicked() {
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save NeuralNet"), QString::fromStdString(Settings->NNFolder),
      tr("NeuralNet (*.NN)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".NN"))) {
      fn.append(tr(".NN"));
    }
    NeuralNet->SaveState(fn.toStdString());
  }
}

void DialogNN::on_pushButton_OpenNN_clicked() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open NeuralNet"),
      QString::fromStdString(Settings->SampleFolder), tr("NeuralNet (*.NN)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".NN"))) {
      fn.append(tr(".NN"));
    }
    if (NeuralNet != nullptr) {
      delete NeuralNet;
    }
    NeuralNet->LoadState(fn.toStdString());
    connect(NeuralNet, SIGNAL(learnErrorUpdate(double)), this,
            SLOT(on_learnErrorUpdate(double)));
  }
}

void DialogNN::on_actionAbort_triggered()
{
    NeuralNet->EndErrorUsedByGA = ui->widget_NNError->graph(0)->data()->lastKey();
}
