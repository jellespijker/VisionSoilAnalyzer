#include "vsagui.h"
#include "ui_vsagui.h"

VSAGUI::VSAGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VSAGUI)
{
    // Startup the UI
    ui->setupUi(this);

    // Startup the microscope
    cv::Mat WebCamImg;

    // Get HDR snapshot of the sample
    if (microscope.IsOpened())
    {
        microscope.GetHDRFrame(WebCamImg, 25);
    }

    QImage image = OpenCVQT::Mat2QImage(WebCamImg);

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView_soil->setScene(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    ui->graphicsView_soil->show();
}

VSAGUI::~VSAGUI()
{
    delete ui;
}
