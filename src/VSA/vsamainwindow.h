#ifndef VSAMAINWINDOW_H
#define VSAMAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QMessageBox>
#include <QProgressBar>

#include <qcustomplot.h>

#include "soilanalyzer.h"
#include "Hardware.h"

#include "dialognn.h"
#include "dialogsettings.h"
#include "qparticleselector.h"

namespace Ui {
  class VSAMainWindow;
}

class VSAMainWindow : public QMainWindow
{
  Q_OBJECT

public slots:
  void setParticleValue(int newValue);

public:
  explicit VSAMainWindow(QWidget *parent = 0);
  ~VSAMainWindow();

private slots:
  void on_actionSettings_triggered();

  void on_analyzer_finished();

  void on_actionNeuralNet_triggered();

  void on_actionNewSample_triggered();

  void on_actionSaveSample_triggered();

  void on_actionLoadSample_triggered();

  void on_actionUseLearning_toggled(bool arg1);

  void on_actionCalibrate_triggered();

private:
  Ui::VSAMainWindow *ui;
  DialogSettings *settingsWindow = nullptr;
  DialogNN *nnWindow = nullptr;
  QProgressBar *Progress;
  QErrorMessage *CamError;
  QMessageBox *SaveMeMessage;
  QMessageBox *BacklightMessage;
  QMessageBox *ShakeItBabyMessage;

  SoilAnalyzer::SoilSettings *Settings = nullptr;
  Hardware::Microscope *Microscope = nullptr;
  SoilAnalyzer::Sample *Sample = nullptr;
  SoilAnalyzer::Analyzer *Analyzer = nullptr;
  SoilAnalyzer::Analyzer::Images_t *Images = nullptr;

  void TakeSnapShots();
};

#endif // VSAMAINWINDOW_H
