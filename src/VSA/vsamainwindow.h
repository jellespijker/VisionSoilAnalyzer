#ifndef VSAMAINWINDOW_H
#define VSAMAINWINDOW_H

#include <QDebug>
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

class VSAMainWindow : public QMainWindow {
  Q_OBJECT

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

  void on_Classification_changed(int newValue);

  void on_particle_deleted();

private:
  Ui::VSAMainWindow *ui;
  DialogSettings *settingsWindow = nullptr;
  DialogNN *nnWindow = nullptr;
  QProgressBar *Progress;
  QErrorMessage *CamError = nullptr;
  QMessageBox *SaveMeMessage = nullptr;
  QMessageBox *BacklightMessage = nullptr;
  QMessageBox *ShakeItBabyMessage = nullptr;

  SoilAnalyzer::SoilSettings *Settings = nullptr;
  Hardware::Microscope *Microscope = nullptr;
  SoilAnalyzer::Sample *Sample = nullptr;
  SoilAnalyzer::Analyzer *Analyzer = nullptr;
  SoilAnalyzer::Analyzer::Images_t *Images = nullptr;
  QCPBars *RoundnessBars = nullptr;
  QCPBars *AngularityBars = nullptr;
  QVector<QString> RoundnessCat = {"High", "Medium", "Low"};
  QVector<double> RoundnessTicks = {1, 2, 3};
  QVector<QString> AngularityCat = {"Very Angular", "Angular", "Sub Angular",
                                    "Sub Rounded",  "Rounded", "Well Rounded"};
  QVector<double> AngularityTicks = {1, 2, 3, 4, 5, 6};

  bool ParticleDisplayerFilled = false;

  void SetPSDgraph();
  void setRoundnessHistogram();
  void setAngularityHistogram();
  void TakeSnapShots();
};

#endif // VSAMAINWINDOW_H
