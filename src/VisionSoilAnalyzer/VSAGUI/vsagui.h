#ifndef VSAGUI_H
#define VSAGUI_H

#define DEBUG

#include <string>
#include <vector>
#include <sys/utsname.h>

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QErrorMessage>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QLabel>
#include <QProgressBar>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <boost/filesystem.hpp>

#include "opencvqt.h"

#include "../Hardware.h"
#include "../SoilMath.h"
#include "../Vision.h"
#include "../VisionDebug.h"

class QErrorMessage;

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

    void on_SnapshotButton_clicked();

    void on_SegmentButton_clicked();

private:
    Ui::VSAGUI *ui;
    QErrorMessage *errorMessageDialog;

    cv::Mat *OrigImg;
    QImage *QOrigImg = nullptr;
    QPixmap *QOrigPix = nullptr;
    QProgressBar *progressBar = nullptr;
    QLabel *statusLabel = nullptr;

    Vision::Enhance enhancer;
    Vision::Conversion convertor;
    Vision::Segment segmenter;
    Vision::MorphologicalFilter filter;

    bool runFromBBB = false;
    bool Segmented = false;

    void LoadSample();
    std::vector<std::string> webCams;
    void SetMatToMainView(cv::Mat &img);
};

#endif // VSAGUI_H
