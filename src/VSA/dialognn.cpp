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
  ui->widget_NNError->xAxis->setLabel("Generation [-]");
  ui->widget_NNError->yAxis->setLabel("Error [%]");
  QCPPlotTitle *widget_NNErrorTitle = new QCPPlotTitle(ui->widget_NNError);
  widget_NNErrorTitle->setText("Learning error");
  widget_NNErrorTitle->setFont(QFont("sans", 10, QFont::Bold));
  ui->widget_NNError->plotLayout()->insertRow(0);
  ui->widget_NNError->plotLayout()->addElement(0, 0, widget_NNErrorTitle);
}

DialogNN::~DialogNN() { delete ui; }

void DialogNN::on_pushButton_Settings_clicked() {
  SettingsWindow->openTab(2);
  SettingsWindow->show();
}
