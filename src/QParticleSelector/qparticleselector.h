#ifndef QPARTICLESELECTOR_H
#define QPARTICLESELECTOR_H

#include <QWidget>

namespace Ui {
  class QParticleSelector;
}

class QParticleSelector : public QWidget
{
  Q_OBJECT

public:
  explicit QParticleSelector(QWidget *parent = 0);
  ~QParticleSelector();

signals:
  void valueChanged(int newValue);

public slots:
  void setValue(int newValue);

private slots:
  void on_pb_1_clicked(bool checked);

  void on_pb_2_clicked(bool checked);

  void on_pb_3_clicked(bool checked);

  void on_pb_4_clicked(bool checked);

  void on_pb_5_clicked(bool checked);

  void on_pb_6_clicked(bool checked);

  void on_pb_7_clicked(bool checked);

  void on_pb_8_clicked(bool checked);

  void on_pb_9_clicked(bool checked);

  void on_pb_10_clicked(bool checked);

  void on_pb_11_clicked(bool checked);

  void on_pb_12_clicked(bool checked);

  void on_pb_13_clicked(bool checked);

  void on_pb_14_clicked(bool checked);

  void on_pb_15_clicked(bool checked);

  void on_pb_16_clicked(bool checked);

  void on_pb_17_clicked(bool checked);

  void on_pb_18_clicked(bool checked);

private:
  Ui::QParticleSelector *ui;
};

#endif // QPARTICLESELECTOR_H
