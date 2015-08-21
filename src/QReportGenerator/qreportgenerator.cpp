#include "qreportgenerator.h"
#include "ui_qreportgenerator.h"

QReportGenerator::QReportGenerator(QWidget *parent,
                                   SoilAnalyzer::Sample *sample,
                                   SoilAnalyzer::SoilSettings *settings,
                                   QCustomPlot *psd, QCustomPlot *roundness,
                                   QCustomPlot *angularity)
    : QMainWindow(parent), ui(new Ui::QReportGenerator) {
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

  if (roundness == nullptr) {
    roundness = new QCustomPlot;
  }
  this->Roundness = roundness;

  if (angularity == nullptr) {
    angularity = new QCustomPlot;
  }
  this->Angularity = angularity;

  Report = new QTextDocument(ui->textEdit);
  ui->textEdit->setDocument(Report);
  rCurs = QTextCursor(Report);

  // Setup the layout
  TitleFormat.setAlignment(Qt::AlignCenter);
  TitleFont.setBold(true);
  TitleFont.setPointSize(36);
  TitleTextFormat.setFont(TitleFont);

  HeaderFormat.setAlignment(Qt::AlignCenter);
  HeaderFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  HeaderFormat.setTopMargin(40);
  HeaderFormat.setBottomMargin(10);
  HeaderFont.setBold(true);
  HeaderFont.setPointSize(18);
  HeaderTextFormat.setFont(HeaderFont);

  ImageGraphFormat.setAlignment(Qt::AlignCenter);
  ImageGraphFormat.setTopMargin(10);
  ImageGraphFormat.setBottomMargin(10);

  GeneralFormat.setAlignment(Qt::AlignLeft);

  GeneralFont.setPointSize(12);
  GeneralFont.setBold(false);
  GtxtFormat.setFont(GeneralFont);

  FieldFont.setBold(true);
  GFieldtxtFormat.setFont(FieldFont);

  GeneralSampleList.setStyle(QTextListFormat::ListDisc);

  GeneralTextTableFormat.setHeaderRowCount(1);
  GeneralTextTableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  GeneralTextTableFormat.setWidth(
      QTextLength(QTextLength::PercentageLength, 90));
  GeneralTextTableFormat.setAlignment(Qt::AlignCenter);

  // Setup the Title
  rCurs.setBlockFormat(TitleFormat);
  rCurs.insertText("Soil Report", TitleTextFormat);
  rCurs.insertBlock();

  // Setup the general Text
  rCurs.insertBlock(ImageGraphFormat);
  QTextTable *mainTable = rCurs.insertTable(5, 2, GeneralTextTableFormat);
  rCurs = mainTable->cellAt(0, 0).firstCursorPosition();
  rCurs.insertText("Sample name:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::fromStdString(Sample->Name), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Sample ID:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->ID), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Date:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::fromStdString(Sample->Date), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Location:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Latitude), GtxtFormat);
  rCurs.insertText(", ", GtxtFormat);
  rCurs.insertText(QString::number(Sample->Longtitude), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Sample depth:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Depth), GtxtFormat);
  rCurs.insertText(" [m]", GtxtFormat);
  rCurs.movePosition(QTextCursor::NextBlock);
  rCurs.insertBlock();

  // Insert the Google map
  getLocationMap(Sample->Latitude, Sample->Longtitude);

  // Setup the QCustomplot handler
  QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);
  ui->textEdit->document()->documentLayout()->registerHandler(
      QCPDocumentObject::PlotTextFormat, plotObjectHandler);

  // Setup the Textdata for the PSD
  rCurs.insertBlock(HeaderFormat, HeaderTextFormat);
  rCurs.insertText("Particle Size Distribution");

  rCurs.insertBlock(ImageGraphFormat);
  QTextTable *PSDdescr = rCurs.insertTable(6, 2, GeneralTextTableFormat);
  rCurs = PSDdescr->cellAt(0, 0).firstCursorPosition();
  rCurs.insertText("No of particles:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.n), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Mean: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.Mean), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Minimum: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.min), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Maximum: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.max), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Range: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.Range), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Standard deviation: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.Std), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextBlock);

  // Setup the PSD
  rCurs.insertBlock(ImageGraphFormat);
  rCurs.insertText(QString(QChar::ObjectReplacementCharacter),
                   QCPDocumentObject::generatePlotFormat(PSD, 600, 350));

  rCurs.insertBlock(ImageGraphFormat);
  QTextTable *PSDdata = rCurs.insertTable(16, 3, GeneralTextTableFormat);
  rCurs.insertText("Mesh Size [mm]", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("Cummulatief [%]", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("Retained [-]", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("2", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[14]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[14]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("1.4", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[13]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[13]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("1", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[12]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[12]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.71", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[11]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[11]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.5", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[10]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[10]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.355", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[9]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[9]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.25", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[8]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[8]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.18", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[7]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[7]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.125", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[6]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[6]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.09", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[5]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[5]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.075", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[4]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[4]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.063", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[3]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[3]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.045", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[2]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[2]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0.038", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[1]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[1]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText("0", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.CFD[0]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->PSD.bins[0]), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextBlock);

  // Setup the Textdata for the Roundness
  rCurs.insertBlock(HeaderFormat, HeaderTextFormat);
  rCurs.insertText("Sphericity Classification");

  rCurs.insertBlock(ImageGraphFormat);
  QTextTable *Rounddescr = rCurs.insertTable(6, 2, GeneralTextTableFormat);
  rCurs = Rounddescr->cellAt(0, 0).firstCursorPosition();
  rCurs.insertText("No of particles:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Roundness.n), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Mean: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Roundness.Mean), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Minimum: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Roundness.min), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Maximum: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Roundness.max), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Range: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Roundness.Range), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Standard deviation: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Roundness.Std), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextBlock);

  // Setup the Roundness Graph
  rCurs.insertBlock(ImageGraphFormat);
  rCurs.insertText(QString(QChar::ObjectReplacementCharacter),
                   QCPDocumentObject::generatePlotFormat(Roundness, 600, 400));

  // Setup the Textdata for the Roundness
  rCurs.insertBlock(HeaderFormat, HeaderTextFormat);
  rCurs.insertText("Angularity Classification");

  rCurs.insertBlock(ImageGraphFormat);
  QTextTable *Angularitydescr = rCurs.insertTable(6, 2, GeneralTextTableFormat);
  rCurs = Angularitydescr->cellAt(0, 0).firstCursorPosition();
  rCurs.insertText("No of particles:", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Angularity.n), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Mean: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Angularity.Mean), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Minimum: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Angularity.min), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Maximum: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Angularity.max), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Range: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Angularity.Range), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);

  rCurs.insertText("Standard deviation: ", GFieldtxtFormat);
  rCurs.movePosition(QTextCursor::NextCell);
  rCurs.insertText(QString::number(Sample->Angularity.Std), GtxtFormat);
  rCurs.movePosition(QTextCursor::NextBlock);

  // Setup the Roundness Graph
  rCurs.insertBlock(ImageGraphFormat);
  rCurs.insertText(QString(QChar::ObjectReplacementCharacter),
                   QCPDocumentObject::generatePlotFormat(Angularity, 600, 400));
}

void QReportGenerator::getLocationMap(double &latitude, double &longtitude) {
  QNetworkAccessManager *manager = new QNetworkAccessManager;
  connect(manager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(on_locationImageDownloaded(QNetworkReply *)));
  QString locationURL("http://maps.googleapis.com/maps/api/staticmap?center=");
  locationURL.append(QString::number(latitude));
  locationURL.append(",");
  locationURL.append(QString::number(longtitude));
  locationURL.append("&zoom=17&size=600x750&maptype=hybrid&&format=png&visual_"
                     "refresh=true&markers=size:mid%7Ccolor:0xff0000%7Clabel:S%"
                     "7C");
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

  QTextBlock location = Report->findBlockByNumber(15);
  QTextCursor insertMap(location);
  insertMap.setBlockFormat(ImageGraphFormat);
  insertMap.insertImage(*mapLocation);
  insertMap.insertBlock();
  insertMap.insertHtml("<br>");
}

QReportGenerator::~QReportGenerator() { delete ui; }

void QReportGenerator::on_actionSave_triggered()
{
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save Report"), QString::fromStdString(Settings->SampleFolder),
      tr("Report (*.odf)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".odf"))) {
      fn.append(tr(".odf"));
    }
    QTextDocumentWriter m_write;
    m_write.setFileName(fn);
    m_write.setFormat("odf");
    m_write.write(Report);
    }
}

void QReportGenerator::on_actionExport_to_PDF_triggered()
{
  QString fn = QFileDialog::getSaveFileName(
      this, tr("Save Report"), QString::fromStdString(Settings->SampleFolder),
      tr("Report (*.pdf)"));
  if (!fn.isEmpty()) {
    if (!fn.contains(tr(".pdf"))) {
      fn.append(tr(".pdf"));
    }
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fn);
    Report->print(&printer);
    }
}
