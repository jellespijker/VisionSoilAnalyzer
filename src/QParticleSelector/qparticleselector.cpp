#include "qparticleselector.h"
#include "ui_qparticleselector.h"

QParticleSelector::QParticleSelector(QWidget *parent)
    : QWidget(parent), ui(new Ui::QParticleSelector) {
  ui->setupUi(this);
  btns.push_back(ui->pb_1);
  btns.push_back(ui->pb_2);
  btns.push_back(ui->pb_3);
  btns.push_back(ui->pb_4);
  btns.push_back(ui->pb_5);
  btns.push_back(ui->pb_6);
  btns.push_back(ui->pb_7);
  btns.push_back(ui->pb_8);
  btns.push_back(ui->pb_9);
  btns.push_back(ui->pb_10);
  btns.push_back(ui->pb_11);
  btns.push_back(ui->pb_12);
  btns.push_back(ui->pb_13);
  btns.push_back(ui->pb_14);
  btns.push_back(ui->pb_15);
  btns.push_back(ui->pb_16);
  btns.push_back(ui->pb_17);
  btns.push_back(ui->pb_18);
}

QParticleSelector::~QParticleSelector() {
  for (auto b : btns) {
    delete b;
  }
  btns.clear();
  delete ui;
}

void QParticleSelector::setValue(int newValue) {
  btns[newValue - 1]->setChecked(true);
}

void QParticleSelector::setDisabled(bool value, int currentClass) {
  for (auto b : btns) {
    b->setDisabled(value);
  }
  if (currentClass > 18 || currentClass < 1) {
    btns[0]->setChecked(true);
  } else {
    btns[currentClass - 1]->setChecked(true);
  }
}

void QParticleSelector::on_pb_1_clicked(bool checked) {
  if (checked) {
    emit valueChanged(1);
  }
}

void QParticleSelector::on_pb_2_clicked(bool checked) {
  if (checked) {
    emit valueChanged(2);
  }
}

void QParticleSelector::on_pb_3_clicked(bool checked) {
  if (checked) {
    emit valueChanged(3);
  }
}

void QParticleSelector::on_pb_4_clicked(bool checked) {
  if (checked) {
    emit valueChanged(4);
  }
}

void QParticleSelector::on_pb_5_clicked(bool checked) {
  if (checked) {
    emit valueChanged(5);
  }
}

void QParticleSelector::on_pb_6_clicked(bool checked) {
  if (checked) {
    emit valueChanged(6);
  }
}

void QParticleSelector::on_pb_7_clicked(bool checked) {
  if (checked) {
    emit valueChanged(7);
  }
}

void QParticleSelector::on_pb_8_clicked(bool checked) {
  if (checked) {
    emit valueChanged(8);
  }
}

void QParticleSelector::on_pb_9_clicked(bool checked) {
  if (checked) {
    emit valueChanged(9);
  }
}

void QParticleSelector::on_pb_10_clicked(bool checked) {
  if (checked) {
    emit valueChanged(10);
  }
}

void QParticleSelector::on_pb_11_clicked(bool checked) {
  if (checked) {
    emit valueChanged(11);
  }
}

void QParticleSelector::on_pb_12_clicked(bool checked) {
  if (checked) {
    emit valueChanged(12);
  }
}

void QParticleSelector::on_pb_13_clicked(bool checked) {
  if (checked) {
    emit valueChanged(13);
  }
}

void QParticleSelector::on_pb_14_clicked(bool checked) {
  if (checked) {
    emit valueChanged(14);
  }
}

void QParticleSelector::on_pb_15_clicked(bool checked) {
  if (checked) {
    emit valueChanged(15);
  }
}

void QParticleSelector::on_pb_16_clicked(bool checked) {
  if (checked) {
    emit valueChanged(16);
  }
}

void QParticleSelector::on_pb_17_clicked(bool checked) {
  if (checked) {
    emit valueChanged(17);
  }
}

void QParticleSelector::on_pb_18_clicked(bool checked) {
  if (checked) {
    emit valueChanged(18);
  }
}
