#include "vsagui.h"
#include "ui_vsagui.h"

VSAGUI::VSAGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VSAGUI)
{
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }

    ui->setupUi(this);

    // Startup the microscope
    cv::Mat WebCamImg;

    if (microscope.IsOpened())
    {
        microscope.GetHDRFrame(WebCamImg, 25);
                //GetFrame(WebCamImg);
    }

    QImage image = OpenCVQT::Mat2QImage(WebCamImg);

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView_soil->setScene(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    ui->graphicsView_soil->show();

    //    ui->plot_lab->addGraph();
    //    ui->plot_lab->graph(0)->setData(x,y);
    //    ui->plot_lab->xAxis->setLabel("x");
    //    ui->plot_lab->yAxis->setLabel("y");
    //    ui->plot_lab->xAxis->setRange(-1, 1);
    //    ui->plot_lab->yAxis->setRange(0,1);
    //    ui->plot_lab->replot();

}

VSAGUI::~VSAGUI()
{
    delete ui;
}
