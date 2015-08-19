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
                          Hardware::Microscope *microscope = nullptr,
                          SoilMath::NN *nn = nullptr, bool openNN = false);
  ~DialogSettings();

  void openTab(int newValue);
private slots:

  void on_pushButton_RestoreDefault_clicked();

  void on_pushButton_Open_clicked();

  void on_pushButton_Save_clicked();

  void on_checkBox_Backlight_clicked(bool checked);

  void on_comboBox_Microscopes_currentIndexChanged(const QString &arg1);

  void on_comboBox_Resolution_currentIndexChanged(int index);

  void on_checkBox_useHDR_clicked(bool checked);

  void on_spinBox_NoFrames_editingFinished();

  void on_doubleSpinBox_LightLevel_editingFinished();

  void on_checkBox_useRainbow_clicked(bool checked);

  void on_checkBox_InvertEncoder_clicked(bool checked);

  void on_checkBox_useCUDA_clicked(bool checked);

  void on_horizontalSlider_BrightFront_valueChanged(int value);

  void on_horizontalSlider_ContrastFront_valueChanged(int value);

  void on_horizontalSlider_SaturationFront_valueChanged(int value);

  void on_horizontalSlider_HueFront_valueChanged(int value);

  void on_horizontalSlider_SharpnessFront_valueChanged(int value);

  void on_horizontalSlider_BrightProj_valueChanged(int value);

  void on_horizontalSlider_ContrastProj_valueChanged(int value);

  void on_horizontalSlider_SaturationProj_valueChanged(int value);

  void on_horizontalSlider_HueProj_valueChanged(int value);

  void on_horizontalSlider_SharpnessProj_valueChanged(int value);

  void on_cb_use_adaptContrast_3_clicked(bool checked);

  void on_cb_useBlur_3_clicked(bool checked);

  void on_rb_useDark_3_toggled(bool checked);

  void on_cb_ignoreBorder_3_clicked(bool checked);

  void on_cb_fillHoles_3_clicked(bool checked);

  void on_sb_sigmaFactor_3_editingFinished();

  void on_rb_useOpen_3_clicked(bool checked);

  void on_rb_useClose_3_clicked(bool checked);

  void on_rb_useErode_3_clicked(bool checked);

  void on_rb_useDilate_3_clicked(bool checked);

  void on_sb_morphMask_3_editingFinished();

  void on_spinBox_MaxGen_editingFinished();

  void on_spinBox_PopSize_editingFinished();

  void on_doubleSpinBox_MutationRate_editingFinished();

  void on_spinBox_Elitisme_editingFinished();

  void on_doubleSpinBox_endError_editingFinished();

  void on_doubleSpinBox_maxWeight_editingFinished();

  void on_doubleSpinBox_MinWeight_editingFinished();

  void on_doubleSpinBox_Beta_editingFinished();

  void on_spinBox_InputNeurons_editingFinished();

  void on_spinBox_HiddenNeurons_editingFinished();

  void on_spinBox_OutputNeurons_editingFinished();

  void on_pushButton_selectSampleFolder_clicked();

  void on_pushButton_SelectSettingFolder_clicked();

  void on_pushButton_SelectNNFolder_clicked();

  void on_pushButton_SelectNN_clicked();

  void on_spinBox_NoShots_editingFinished();

  void on_checkBox_PredictShape_clicked(bool checked);

  void on_checkBox_revolt_clicked(bool checked);

private:
  Ui::DialogSettings *ui;
  Hardware::Microscope *Microscope;
  SoilMath::NN *NN;
  bool initfase = true;
  void SetCamControl(Hardware::Microscope::Cam_t *selectedCam,
                     QSlider *Brightness, QSlider *Contrast,
                     QSlider *Saturation, QSlider *Hue, QSlider *Sharpness);
};

#endif // DIALOGSETTINGS_H
