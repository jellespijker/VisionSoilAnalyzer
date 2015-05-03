#include "hardwaresettings.h"
#include "ui_hardwaresettings.h"

HardwareSettings::HardwareSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HardwareSettings)
{
    ui->setupUi(this);
}

HardwareSettings::~HardwareSettings()
{
    delete ui;
}
