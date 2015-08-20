#include "qreportgenerator.h"
#include "ui_qreportgenerator.h"

QReportGenerator::QReportGenerator(QWidget *parent, SoilAnalyzer::Sample *sample, SoilAnalyzer::SoilSettings *settings, QCustomPlot *psd) :
  QMainWindow(parent),
  ui(new Ui::QReportGenerator)
{
  ui->setupUi(this);
  if (settings == nullptr) {
    settings = new SoilAnalyzer::SoilSettings;
  }
  this->Settings = settings;
  if (sample == nullptr) {
    sample = new SoilAnalyzer::Sample;
  }
  this->Sample = sample;

  if (psd == nullptr) {
      psd = new QCustomPlot;
    }
  this->PSD = psd;

  Report = new QTextDocument(ui->textEdit);
  ui->textEdit->setDocument(Report);
  if (ReportCursor == nullptr) {
    ReportCursor = new QTextCursor(Report);
  }

  // Setup the Title
  TitleFormat = new QTextBlockFormat;
  TitleFormat->setAlignment(Qt::AlignCenter);
  TitleTextFormat = new QTextCharFormat;
  TitleFont = new QFont;
  TitleFont->setBold(true);
  TitleFont->setPointSize(24);
  TitleTextFormat->setFont(*TitleFont);
  ReportCursor->insertBlock(*TitleFormat, *TitleTextFormat);
  ReportCursor->insertText("Soil Report");

  // Setup the General Information
  GeneralFormat = new QTextBlockFormat;
  GeneralFormat->setAlignment(Qt::AlignLeft);
  GeneralTextFormat = new QTextCharFormat;
  GeneralFont = new QFont;
  GeneralFont->setPointSize(10);
  GeneralFont->setBold(false);
  GeneralTextFormat->setFont(*GeneralFont);

  GeneralFieldTextFormat = new QTextCharFormat;
  FieldFont = new QFont(*GeneralFont);
  FieldFont->setBold(true);
  GeneralFieldTextFormat->setFont(*FieldFont);

  // Setup the general Text
  ReportCursor->insertBlock(*GeneralFormat);
  ReportCursor->insertText("Some general text here", *GeneralTextFormat);
  GeneralSampleList = new QTextListFormat;
  GeneralSampleList->setStyle(QTextListFormat::ListDisc);

  ReportCursor->insertList(*GeneralSampleList);
  ReportCursor->insertText("Date: ", *GeneralFieldTextFormat);
  ReportCursor->insertText("26-06-1979 23:23", *GeneralTextFormat);

  ReportCursor->insertList(*GeneralSampleList);
  ReportCursor->insertText("Location: ", *GeneralFieldTextFormat);
  ReportCursor->insertText("Smitweg 6, Kinderdijk, The Netherlands",
                           *GeneralTextFormat);

  // Insert the Google map
  double latitude = 52.0782597;
  double longtitude = 5.0950307;
  getLocationMap(latitude, longtitude);
  ReportCursor->insertBlock(*GeneralFormat);

  // Setup the QCustomplot handler
  QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);
  ui->textEdit->document()->documentLayout()->registerHandler(QCPDocumentObject::PlotTextFormat, plotObjectHandler);

  // Setup the PSD
  ReportCursor->insertText(QString(QChar::ObjectReplacementCharacter), QCPDocumentObject::generatePlotFormat(PSD, 600, 400));

}

void QReportGenerator::getLocationMap(double &latitude, double &longtitude) {
  QNetworkAccessManager *manager = new QNetworkAccessManager;
  connect(manager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(on_locationImageDownloaded(QNetworkReply *)));
  QString locationURL("http://maps.googleapis.com/maps/api/staticmap?center=");
  locationURL.append(QString::number(latitude));
  locationURL.append(",");
  locationURL.append(QString::number(longtitude));
  locationURL.append("&zoom=17&size=600x400&maptype=hybrid&&format=png&visual_refresh=true&markers=size:mid%7Ccolor:0xff0000%7Clabel:S%7C");
  locationURL.append(QString::number(latitude));
  locationURL.append(",");
  locationURL.append(QString::number(longtitude));
  qDebug() << locationURL;
  QUrl googleStaticMapUrl(locationURL);
  manager->get(QNetworkRequest(googleStaticMapUrl));
}

void QReportGenerator::on_locationImageDownloaded(QNetworkReply *reply) {
  if (mapLocation == nullptr) {
    mapLocation = new QImage;
  }
  mapLocation->loadFromData(reply->readAll());

  if (mapLocation->isNull()) {
    mapLocation->load("Maps/SampleLocation.png");
  }

  QTextBlock location = Report->findBlockByNumber(5);
  QTextCursor insertMap(location);
  insertMap.insertImage(*mapLocation);
}

QReportGenerator::~QReportGenerator()
{
  delete ui;
}
