#ifndef HARDWARESETTINGS_H
#define HARDWARESETTINGS_H

#include <QDialog>
#include "VisionSoil.h"
#include <sys/utsname.h>
#include <string>

namespace Ui {
class HardwareSettings;
}

class HardwareSettings : public QDialog
{
    Q_OBJECT

public:
    explicit HardwareSettings(QWidget *parent = 0, SoilAnalyzer::SoilSettings *soilsetting =0);
    ~HardwareSettings();

private slots:
    void on_sb_HDRframes_editingFinished();

    void on_sb_lightLevel_editingFinished();

    void on_cb_encoderInv_clicked(bool checked);

    void on_cb_enableRainbow_clicked(bool checked);

private:
    Ui::HardwareSettings *ui;
    SoilAnalyzer::SoilSettings *soilSetting = nullptr;

};

#endif // HARDWARESETTINGS_H
