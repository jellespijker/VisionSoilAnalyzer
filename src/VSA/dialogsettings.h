#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <soilsettings.h>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QSlider>
#include "Hardware.h"

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog {
  Q_OBJECT

public:
  SoilAnalyzer::SoilSettings *Settings = nullptr;
  explicit DialogSettings(QWidget *parent = 0,
                          SoilAnalyzer::SoilSettings *settings = nullptr,
                          Hardware::Microscope *microscope = nullptr);
  ~DialogSettings();

private slots:

  void on_pushButton_RestoreDefault_clicked();

  void on_pushButton_Open_clicked();

  void on_pushButton_Save_clicked();

  void on_checkBox_Backlight_clicked(bool checked);

  void on_comboBox_Microscopes_currentIndexChanged(const QString &arg1);

  void on_comboBox_Resolution_currentIndexChanged(int index);

  void on_checkBox_useHDR_clicked(bool checked);

private:
  Ui::DialogSettings *ui;
  Hardware::Microscope *Microscope;
  bool initfase = true;
  void SetCamControl(Hardware::Microscope::Cam_t *selectedCam, QSlider *Brightness, QSlider *Contrast, QSlider *Saturation, QSlider *Hue, QSlider *Sharpness);
};

#endif // DIALOGSETTINGS_H
