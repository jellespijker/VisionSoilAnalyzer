#ifndef VSAMAINWINDOW_H
#define VSAMAINWINDOW_H

#include <QMainWindow>
#include "qparticleselector.h"

namespace Ui {
  class VSAMainWindow;
}

class VSAMainWindow : public QMainWindow
{
  Q_OBJECT

public slots:
  void setParticleValue(int newValue);

public:
  explicit VSAMainWindow(QWidget *parent = 0);
  ~VSAMainWindow();

private:
  Ui::VSAMainWindow *ui;
};

#endif // VSAMAINWINDOW_H
