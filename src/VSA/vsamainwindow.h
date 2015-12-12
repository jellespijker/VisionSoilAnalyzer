#ifndef VSAMAINWINDOW_H
#define VSAMAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QErrorMessage>
#include <QMessageBox>
#include <QProgressBar>
#include <QBrush>

#include <stdint.h>
#include <stdlib.h>

#include <qcustomplot.h>

#include "soilanalyzer.h"
#include "Hardware.h"

#include "dialognn.h"
#include "dialogsettings.h"
#include "qparticleselector.h"
#include "qreportgenerator.h"

#include <algorithm>
#include <opencv2/highgui.hpp>
#include <sstream>
#include <string>

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

  void on_actionAutomatic_Shape_Pediction_triggered(bool checked);

  void on_reset_graph(QMouseEvent * e);

  void on_actionReport_Generator_triggered();

  void on_particleChanged(int newPart);

  void on_PSD_contextMenuRequest(QPoint point);

  void on_compare_against();

  void on_restore_PSD();

  void on_actionExport_triggered();

private:
  Ui::VSAMainWindow *ui;
  DialogSettings *settingsWindow = nullptr;
  DialogNN *nnWindow = nullptr;
  QProgressBar *Progress;
  QErrorMessage *CamError = nullptr;
  QMessageBox *SaveMeMessage = nullptr;
  QMessageBox *BacklightMessage = nullptr;
  QMessageBox *ShakeItBabyMessage = nullptr;
  QReportGenerator *ReportGenWindow = nullptr;

  SoilAnalyzer::SoilSettings *Settings = nullptr;
  Hardware::Microscope *Microscope = nullptr;
  SoilAnalyzer::Sample *Sample = nullptr;
  SoilAnalyzer::Analyzer *Analyzer = nullptr;
  SoilAnalyzer::Analyzer::Images_t *Images = nullptr;
  QCPBars *RoundnessBars = nullptr;
  QCPBars *AngularityBars = nullptr;
  std::vector<double> PSDTicks = {0.0,  0.038, 0.045, 0.063, 0.075,
                              0.09, 0.125, 0.18,  0.25,  0.355,
                              0.5,  0.71,  1.0,   1.4,   2.0};
  QVector<QString> RoundnessCat = {"High", "Medium", "Low"};
  std::vector<double> RoundnessTicks = {1, 2, 3};
  QVector<QString> AngularityCat = {"Very Angular", "Angular", "Sub Angular",
                                    "Sub Rounded",  "Rounded", "Well Rounded"};
  std::vector<double> AngularityTicks = {1, 2, 3, 4, 5, 6};

  bool ParticleDisplayerFilled = false;

  void SetPSDgraph();
  void setRoundnessHistogram();
  void setAngularityHistogram();
  void setAmpgraph();
  void TakeSnapShots();
  void ShowShaker(int i);
};

#endif // VSAMAINWINDOW_H
