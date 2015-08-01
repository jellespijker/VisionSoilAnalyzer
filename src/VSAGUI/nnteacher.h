/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#ifndef NNTEACHER_H
#define NNTEACHER_H

#include <QMainWindow>

namespace Ui {
  class NNteacher;
}

class NNteacher : public QMainWindow
{
  Q_OBJECT

public:
  explicit NNteacher(QWidget *parent = 0);
  ~NNteacher();

private:
  Ui::NNteacher *ui;
};

#endif // NNTEACHER_H
