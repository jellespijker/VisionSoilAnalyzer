#include "vsamainwindow.h"
#include "ui_vsamainwindow.h"

VSAMainWindow::VSAMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::VSAMainWindow)
{
  ui->setupUi(this);
  connect(ui->widget_ParticleSelector, SIGNAL(valueChanged(int)), this, SLOT(setParticleValue(int)));

}

VSAMainWindow::~VSAMainWindow()
{
  delete ui;
}

void VSAMainWindow::setParticleValue(int newValue) {
  bool test = false;
  if (newValue == 1) {
      test = true;
    }
}
