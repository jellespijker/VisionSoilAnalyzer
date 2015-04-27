#ifndef VSAGUI_H
#define VSAGUI_H

#include <string>
#include <vector>

#include <QMainWindow>
#include <QtGui>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <boost/filesystem.hpp>

#include "opencvqt.h"
#include "../Hardware.h"
#include "../SoilMath.h"

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
    Hardware::Microscope microscope;
    std::vector<std::string> webCams;
};

#endif // VSAGUI_H
