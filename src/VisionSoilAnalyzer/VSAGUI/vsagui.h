#ifndef VSAGUI_H
#define VSAGUI_H

#include <QMainWindow>

namespace Ui {
class VSAGUI;
}

class VSAGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit VSAGUI(QWidget *parent = 0);
    ~VSAGUI();

private:
    Ui::VSAGUI *ui;
};

#endif // VSAGUI_H
