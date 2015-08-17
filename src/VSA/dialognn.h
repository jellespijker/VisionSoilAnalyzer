#ifndef DIALOGNN_H
#define DIALOGNN_H

#include <QDialog>
#include "SoilMath.h"
#include "soilanalyzer.h"
#include "dialogsettings.h"
#include <qcustomplot.h>

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

private:
  Ui::DialogNN *ui;
  DialogSettings *SettingsWindow = nullptr;
  SoilMath::NN *NeuralNet = nullptr;
  SoilAnalyzer::SoilSettings *Settings = nullptr;
};

#endif // DIALOGNN_H
