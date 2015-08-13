#include "qparticleselector.h"
#include "ui_qparticleselector.h"

QParticleSelector::QParticleSelector(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::QParticleSelector)
{
  ui->setupUi(this);
}

QParticleSelector::~QParticleSelector()
{
  delete ui;
}

void QParticleSelector::setValue(int newValue) {
  switch (newValue) {
    case 1:
      ui->pb_1->setChecked(true);
      break;
    case 2:
      ui->pb_2->setChecked(true);
      break;
    case 3:
      ui->pb_3->setChecked(true);
      break;
    case 4:
      ui->pb_4->setChecked(true);
      break;
    case 5:
      ui->pb_5->setChecked(true);
      break;
    case 6:
      ui->pb_6->setChecked(true);
      break;
    case 7:
      ui->pb_7->setChecked(true);
      break;
    case 8:
      ui->pb_8->setChecked(true);
      break;
    case 9:
      ui->pb_9->setChecked(true);
      break;
    case 10:
      ui->pb_10->setChecked(true);
      break;
    case 11:
      ui->pb_11->setChecked(true);
      break;
    case 12:
      ui->pb_12->setChecked(true);
      break;
    case 13:
      ui->pb_13->setChecked(true);
      break;
    case 14:
      ui->pb_14->setChecked(true);
      break;
    case 15:
      ui->pb_15->setChecked(true);
      break;
    case 16:
      ui->pb_16->setChecked(true);
      break;
    case 17:
      ui->pb_17->setChecked(true);
      break;
    case 18:
      ui->pb_18->setChecked(true);
      break;
    default:
      ui->pb_1->setChecked(true);
      break;

    }
}

void QParticleSelector::on_pb_1_clicked(bool checked)
{
    if (checked) {
        emit valueChanged(1);
      }
}

void QParticleSelector::on_pb_2_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(2);
    }
}

void QParticleSelector::on_pb_3_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(3);
    }
}

void QParticleSelector::on_pb_4_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(4);
    }
}

void QParticleSelector::on_pb_5_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(5);
    }
}

void QParticleSelector::on_pb_6_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(6);
    }
}

void QParticleSelector::on_pb_7_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(7);
    }
}

void QParticleSelector::on_pb_8_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(8);
    }
}

void QParticleSelector::on_pb_9_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(9);
    }
}

void QParticleSelector::on_pb_10_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(10);
    }
}

void QParticleSelector::on_pb_11_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(11);
    }
}

void QParticleSelector::on_pb_12_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(12);
    }
}

void QParticleSelector::on_pb_13_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(13);
    }
}

void QParticleSelector::on_pb_14_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(14);
    }
}

void QParticleSelector::on_pb_15_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(15);
    }
}

void QParticleSelector::on_pb_16_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(16);
    }
}

void QParticleSelector::on_pb_17_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(17);
    }
}

void QParticleSelector::on_pb_18_clicked(bool checked)
{
  if (checked) {
      emit valueChanged(18);
    }
}
