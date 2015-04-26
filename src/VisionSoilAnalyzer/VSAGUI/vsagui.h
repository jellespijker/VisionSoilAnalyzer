#ifndef VSAGUI_H
#define VSAGUI_H

#include <QMainWindow>
#include <QtGui>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "opencvqt.h"

namespace Ui {
class VSAGUI;
}

class VSAGUI : public QMainWindow
{
Q_OBJECT

public:
    explicit VSAGUI(QWidget *parent = 0);
    ~VSAGUI();

private slots:

private:
    Ui::VSAGUI *ui;
    void LoadSample();
};

#endif // VSAGUI_H
