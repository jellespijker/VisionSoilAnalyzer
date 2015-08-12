#ifndef VSAMAINWINDOW_H
#define VSAMAINWINDOW_H

#include <QMainWindow>
#include "qparticleselector.h"
#include "dialogsettings.h"
#include "soilanalyzer.h"
#include "Hardware.h"
#include <QErrorMessage>
#include "soilanalyzer.h"
#include <QProgressBar>
#include <qcustomplot.h>
#include "dialognn.h"

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

private:
  Ui::VSAMainWindow *ui;
  DialogSettings *settingsWindow = nullptr;
  DialogNN *nnWindow = nullptr;
  QProgressBar *Progress;
  QErrorMessage *CamError;

  SoilAnalyzer::SoilSettings *Settings = nullptr;
  Hardware::Microscope *Microscope = nullptr;
  SoilAnalyzer::Sample *Sample = nullptr;
  SoilAnalyzer::Analyzer *Analyzer = nullptr;
  SoilAnalyzer::Analyzer::Images_t *Images = nullptr;

};

#endif // VSAMAINWINDOW_H
