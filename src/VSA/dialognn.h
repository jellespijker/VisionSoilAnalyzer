#ifndef DIALOGNN_H
#define DIALOGNN_H

#include <QDialog>
#include "SoilMath.h"
#include "soilanalyzer.h"
#include "dialogsettings.h"
#include <qcustomplot.h>
#include <QDebug>

namespace Ui {
  class DialogNN;
}

class DialogNN : public QDialog
{
  Q_OBJECT

public:
  explicit DialogNN(QWidget *parent = 0, SoilMath::NN *neuralnet = nullptr, SoilAnalyzer::SoilSettings *settings = nullptr, DialogSettings *settingWindow = nullptr);
  ~DialogNN();

private slots:

  void on_pushButton_Settings_clicked();

  void on_learnErrorUpdate(double newError);

  void on_pushButton_SelectSamples_clicked();

  void on_pushButton_Learn_clicked();

  void on_pushButton_SaveNN_clicked();

  void on_pushButton_OpenNN_clicked();

  void on_actionAbort_triggered();

private:
  Ui::DialogNN *ui;
  DialogSettings *SettingsWindow = nullptr;
  SoilMath::NN *NeuralNet = nullptr;
  SoilAnalyzer::SoilSettings *Settings = nullptr;

  void setupErrorGraph();
  void makeLearnVectors(InputLearnVector_t &input, OutputLearnVector_t &output);

  QVector<double> currentError;
  QVector<double> errorTicks;
  double currentGeneration = 0;
  QStringList fn;
};

#endif // DIALOGNN_H
