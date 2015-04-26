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

    ui->plot_lab->addGraph();
    ui->plot_lab->graph(0)->setData(x,y);
    ui->plot_lab->xAxis->setLabel("x");
    ui->plot_lab->yAxis->setLabel("y");
    ui->plot_lab->xAxis->setRange(-1, 1);
    ui->plot_lab->yAxis->setRange(0,1);
    ui->plot_lab->replot();

    cv::Mat soil = cv::imread("../SoilSample1.png", 1);
    QImage image = OpenCVQT::Mat2QImage(soil);
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
