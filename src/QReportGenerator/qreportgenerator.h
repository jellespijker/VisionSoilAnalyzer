#ifndef QREPORTGENERATOR_H
#define QREPORTGENERATOR_H

#include <QMainWindow>
#include <QTextDocument>
#include <QDebug>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextBlock>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "soilanalyzer.h"
#include "SoilMath.h"

#include <qcustomplot.h>
#include "../qcustomplot/examples/text-document-integration/qcpdocumentobject.h"

namespace Ui {
  class QReportGenerator;
}

class QReportGenerator : public QMainWindow
{
  Q_OBJECT

public:
  QTextDocument *Report = nullptr;
  SoilAnalyzer::Sample *Sample = nullptr;
  SoilAnalyzer::SoilSettings *Settings = nullptr;
  QCustomPlot *PSD = nullptr;

  explicit QReportGenerator(QWidget *parent = 0, SoilAnalyzer::Sample *sample = nullptr, SoilAnalyzer::SoilSettings *settings = nullptr, QCustomPlot *psd = nullptr);
  ~QReportGenerator();

private slots:
  void on_locationImageDownloaded(QNetworkReply *reply);

private:
  Ui::QReportGenerator *ui;

  void getLocationMap(double &latitude, double &longtitude);

  QImage *mapLocation = nullptr;
  QTextCursor *ReportCursor = nullptr;

  // Text formats
  QTextBlockFormat *TitleFormat = nullptr;
  QTextCharFormat *TitleTextFormat = nullptr;
  QFont *TitleFont = nullptr;
  QTextBlockFormat *GeneralFormat = nullptr;
  QTextCharFormat *GeneralTextFormat = nullptr;
  QFont *GeneralFont = nullptr;
  QTextCharFormat *GeneralFieldTextFormat = nullptr;
  QFont *FieldFont = nullptr;
  QTextListFormat *GeneralSampleList = nullptr;
};

#endif // QREPORTGENERATOR_H
