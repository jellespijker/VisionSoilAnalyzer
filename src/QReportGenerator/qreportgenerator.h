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
#include <QTextDocumentWriter>
#include <QPrinter>

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
  QCustomPlot *Roundness = nullptr;
  QCustomPlot *Angularity = nullptr;

  explicit QReportGenerator(QWidget *parent = 0, SoilAnalyzer::Sample *sample = nullptr, SoilAnalyzer::SoilSettings *settings = nullptr, QCustomPlot *psd = nullptr, QCustomPlot *roundness = nullptr, QCustomPlot *angularity = nullptr);
  ~QReportGenerator();

private slots:
  void on_locationImageDownloaded(QNetworkReply *reply);

  void on_actionSave_triggered();

  void on_actionExport_to_PDF_triggered();

private:
  Ui::QReportGenerator *ui;
  QCustomPlot *CIElabPlot = nullptr;

  void getLocationMap(double &latitude, double &longtitude);
  void SetupCIElabPLot();

  QImage *mapLocation = nullptr;

  QTextCursor rCurs;

  // Layout formats
  QTextBlockFormat TitleFormat;
  QTextBlockFormat HeaderFormat;
  QTextBlockFormat GeneralFormat;
  QTextBlockFormat ImageGraphFormat;

  QTextCharFormat TitleTextFormat;
  QTextCharFormat HeaderTextFormat;
  QTextCharFormat GtxtFormat;
  QTextCharFormat GFieldtxtFormat;

  QTextListFormat GeneralSampleList;
  QTextTableFormat GeneralTextTableFormat;


  QFont TitleFont;
  QFont HeaderFont;
  QFont GeneralFont;
  QFont FieldFont;
};

#endif // QREPORTGENERATOR_H
