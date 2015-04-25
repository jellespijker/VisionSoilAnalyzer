#include "vsagui.h"
#include "ui_vsagui.h"

VSAGUI::VSAGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VSAGUI)
{
    ui->setupUi(this);
}

VSAGUI::~VSAGUI()
{
    delete ui;
}
