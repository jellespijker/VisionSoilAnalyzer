#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent,
                               SoilAnalyzer::SoilSettings *settings,
                               Hardware::Microscope *microscope)
    : QDialog(parent), ui(new Ui::DialogSettings) {
  ui->setupUi(this);
  if (settings == nullptr) {
    settings = new SoilAnalyzer::SoilSettings;
  }
  Settings = settings;
  if (microscope == nullptr) {
      microscope = new Hardware::Microscope;
    }
  Microscope = microscope;

  QStringList Cams;
  for (uint32_t i = 0; i < Microscope->AvailableCams.size(); i++) {
      Cams << Microscope->AvailableCams[i].Name.c_str();
    }
  ui->comboBox_Microscopes->addItems(Cams);

}

DialogSettings::~DialogSettings() { delete ui; }

void DialogSettings::on_pushButton_RestoreDefault_clicked() {
  Settings->LoadSettings("Settings/Default.ini");
}

void DialogSettings::on_pushButton_Open_clicked() {
  QString fn = QFileDialog::getOpenFileName(
      this, tr("Open Settings"),
      QDir::homePath(),
      tr("Settings (*.ini)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".ini"))) {
      fn.append(tr(".ini"));
    }
    Settings->LoadSettings(fn.toStdString());
  }
}



void DialogSettings::on_pushButton_Save_clicked()
{
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save Settings"),
      QDir::homePath(),
      tr("Settings (*.ini)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".ini"))) {
      fn.append(tr(".ini"));
    }
    Settings->SaveSettings(fn.toStdString());
  }
}

void DialogSettings::on_checkBox_Backlight_clicked(bool checked)
{
    ui->tabWidget_Hardware->setTabEnabled(2, checked);
}
