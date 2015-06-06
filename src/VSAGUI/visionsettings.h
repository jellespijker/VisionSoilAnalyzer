#ifndef VISIONSETTINGS_H
#define VISIONSETTINGS_H

#include <QDialog>

#include "VisionSoil.h"

namespace Ui {
class VisionSettings;
}

class VisionSettings : public QDialog
{
    Q_OBJECT

public:
    explicit VisionSettings(QWidget *parent = 0, SoilAnalyzer::SoilSettings *soilsetting =0);
    ~VisionSettings();

    SoilAnalyzer::SoilSettings *soilSetting = nullptr;

private slots:

    void on_cb_use_adaptContrast_stateChanged(int arg1);

    void on_sb_adaptContrKernel_editingFinished();

    void on_sb_adaptContrastFactor_editingFinished();

    void on_cb_useBlur_stateChanged(int arg1);

    void on_sb_blurMask_editingFinished();

    void on_rb_useLight_toggled(bool checked);
    void on_cb_ignoreBorder_stateChanged(int arg1);

    void on_cb_fillHoles_stateChanged(int arg1);

    void on_sb_sigmaFactor_editingFinished();

    void on_rb_useOpen_toggled(bool checked);

    void on_rb_useClose_toggled(bool checked);

    void on_rb_useErode_toggled(bool checked);

    void on_rb_useDilate_toggled(bool checked);

    void on_sb_morphMask_editingFinished();

private:
    Ui::VisionSettings *ui;
};

#endif // VISIONSETTINGS_H
