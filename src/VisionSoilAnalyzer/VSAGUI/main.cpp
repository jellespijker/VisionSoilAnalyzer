#include "vsagui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VSAGUI w;
    w.show();

    return a.exec();
}
