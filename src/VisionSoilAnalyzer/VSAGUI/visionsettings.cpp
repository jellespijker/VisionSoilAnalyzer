#include "visionsettings.h"
#include "ui_visionsettings.h"

VisionSettings::VisionSettings(QWidget *parent, SoilAnalyzer::SoilSettings *soilsetting) :
    QDialog(parent),
    ui(new Ui::VisionSettings)
{
    ui->setupUi(this);
    if (soilsetting != 0)
    {
        soilSetting = soilsetting;
    }
    else
    {
        soilSetting = new SoilAnalyzer::SoilSettings;
    }

    // Read Segmentation Values from setting file
    ui->cb_fillHoles->setChecked(soilSetting->fillHoles);
    ui->cb_ignoreBorder->setChecked(soilSetting->ignorePartialBorderParticles);
    ui->cb_useBlur->setChecked(soilSetting->useBlur);
    if (!soilSetting->useBlur)
    {
        ui->sb_blurMask->setEnabled(false);
    }
    ui->cb_use_adaptContrast->setChecked(soilSetting->useAdaptiveContrast);
    if (!soilSetting->useAdaptiveContrast)
    {
        ui->sb_adaptContrastFactor->setEnabled(false);
        ui->sb_adaptContrKernel->setEnabled(false);
    }
    switch (soilSetting->typeOfObjectsSegmented)
    {
    case Vision::Segment::Bright:
        ui->rb_useDark->setChecked(false);
        ui->rb_useLight->setChecked(true);
        break;
    case Vision::Segment::Dark:
        ui->rb_useDark->setChecked(true);
        ui->rb_useLight->setChecked(false);
        break;
    }
    switch (soilSetting->morphFilterType)
    {
    case Vision::MorphologicalFilter::CLOSE:
        ui->rb_useClose->setChecked(true);
        ui->rb_useDilate->setChecked(false);
        ui->rb_useErode->setChecked(false);
        ui->rb_useOpen->setChecked(false);
        break;
    case Vision::MorphologicalFilter::OPEN:
        ui->rb_useClose->setChecked(false);
        ui->rb_useDilate->setChecked(false);
        ui->rb_useErode->setChecked(false);
        ui->rb_useOpen->setChecked(true);
        break;
    case Vision::MorphologicalFilter::ERODE:
        ui->rb_useClose->setChecked(false);
        ui->rb_useDilate->setChecked(false);
        ui->rb_useErode->setChecked(true);
        ui->rb_useOpen->setChecked(false);
        break;
    case Vision::MorphologicalFilter::DILATE:
        ui->rb_useClose->setChecked(false);
        ui->rb_useDilate->setChecked(true);
        ui->rb_useErode->setChecked(false);
        ui->rb_useOpen->setChecked(false);
        break;
    }

    ui->sb_adaptContrastFactor->setValue(soilSetting->adaptContrastKernelFactor);
    ui->sb_adaptContrKernel->setValue(soilSetting->adaptContrastKernelSize);
    ui->sb_blurMask->setValue(soilSetting->blurKernelSize);
    ui->sb_morphMask->setValue(soilSetting->filterMaskSize);
    ui->sb_sigmaFactor->setValue(soilSetting->sigmaFactor);
}

VisionSettings::~VisionSettings()
{
    delete ui;
}


void VisionSettings::on_cb_use_adaptContrast_stateChanged(int arg1)
{
    soilSetting->useAdaptiveContrast = static_cast<bool>(arg1);
}

void VisionSettings::on_sb_adaptContrKernel_editingFinished()
{
    int val = ui->sb_adaptContrKernel->value();
    if (val % 2 == 0) { ui->sb_adaptContrKernel->setValue(val + 1); }
    soilSetting->adaptContrastKernelSize = ui->sb_adaptContrKernel->value();
}

void VisionSettings::on_sb_adaptContrastFactor_editingFinished()
{
    soilSetting->adaptContrastKernelFactor = static_cast<float>(ui->sb_adaptContrastFactor->value());
}

void VisionSettings::on_cb_useBlur_stateChanged(int arg1)
{
    soilSetting->useBlur = static_cast<bool>(arg1);
}

void VisionSettings::on_sb_blurMask_editingFinished()
{
    int val = ui->sb_blurMask->value();
    if (val % 2 == 0) { ui->sb_blurMask->setValue(val + 1); }
    soilSetting->blurKernelSize = ui->sb_blurMask->value();
}

void VisionSettings::on_rb_useLight_toggled(bool checked)
{
    if (checked)
    {
        soilSetting->typeOfObjectsSegmented = Vision::Segment::Bright;
    }
    else
    {
        soilSetting->typeOfObjectsSegmented = Vision::Segment::Dark;
    }
}

void VisionSettings::on_cb_ignoreBorder_stateChanged(int arg1)
{
    soilSetting->ignorePartialBorderParticles = static_cast<bool>(arg1);
}

void VisionSettings::on_cb_fillHoles_stateChanged(int arg1)
{
    soilSetting->fillHoles = static_cast<bool>(arg1);
}

void VisionSettings::on_sb_sigmaFactor_editingFinished()
{
    soilSetting->sigmaFactor = static_cast<float>(ui->sb_blurMask->value());
}

void VisionSettings::on_rb_useOpen_toggled(bool checked)
{
    if (checked)
    {
        soilSetting->morphFilterType = Vision::MorphologicalFilter::OPEN;
    }
}

void VisionSettings::on_rb_useClose_toggled(bool checked)
{
    if (checked)
    {
        soilSetting->morphFilterType = Vision::MorphologicalFilter::CLOSE;
    }
}

void VisionSettings::on_rb_useErode_toggled(bool checked)
{
    if (checked)
    {
        soilSetting->morphFilterType = Vision::MorphologicalFilter::ERODE;
    }
}

void VisionSettings::on_rb_useDilate_toggled(bool checked)
{
    if (checked)
    {
        soilSetting->morphFilterType = Vision::MorphologicalFilter::DILATE;
    }
}

void VisionSettings::on_sb_morphMask_editingFinished()
{
    int val = ui->sb_morphMask->value();
    if (val % 2 == 0) { ui->sb_morphMask->setValue(val + 1); }
    soilSetting->filterMaskSize = ui->sb_morphMask->value();
}
