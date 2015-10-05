#include "vsamainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  VSAMainWindow w;
  w.show();

  return a.exec();
}
