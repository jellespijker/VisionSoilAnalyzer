/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "vsagui.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  VSAGUI w;
  w.show();

  return a.exec();
}
