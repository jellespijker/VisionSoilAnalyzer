#include "vsagui.h"
#include "ui_vsagui.h"

VSAGUI::VSAGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VSAGUI)
{
    //Determine if the program is run from an ARM (BBB) device
    struct utsname unameData;
    uname(&unameData);
    std::string arch = static_cast<std::string>(unameData.machine);
    if (arch.find("armv7l") != string::npos) { runFromBBB = true; }

    // Startup the UI
    ui->setupUi(this);
    OrigImg = new cv::Mat;

    // Init the progress bar
    progressBar = new QProgressBar(ui->statusBar);
    progressBar->setAlignment(Qt::AlignLeft);
    progressBar->setMaximumSize(640, 19);
    ui->statusBar->addWidget(progressBar);
    progressBar->setValue(0);

    statusLabel = new QLabel(ui->statusBar);
    statusLabel->setAlignment(Qt::AlignRight);
    statusLabel->setMaximumSize(600, 19);
    ui->statusBar->addWidget(statusLabel);
    statusLabel->setText(tr("First Grab"));

    // Get HDR snapshot of the sample or normal shot when HDR grab faulters or test image if normal grab falters
    Hardware::Microscope microscope;
    try
    {
        if (microscope.IsOpened()) { microscope.GetHDRFrame(*OrigImg, 3);  }
    }
    catch (Hardware::Exception::MicroscopeNotFoundException &em)
    {
        try
        {
            errorMessageDialog->showMessage(tr("Microscope not found switching to first default Cam!"));
            if (microscope.AvailableCams().size() > 0) { microscope.openCam(0); }
        }
        catch (Hardware::Exception::MicroscopeNotFoundException &em2)
        {
            // display error dialog no cam found and show default test image
            errorMessageDialog->showMessage(tr("Microscope not found switching to test image!"));
            *OrigImg = cv::imread("../Images/SoilSample1.png");
        }
    }
    catch (Hardware::Exception::CouldNotGrabImageException &ei)
    {
        // HDRFrame not working switching to normal grab
        try
        {
            errorMessageDialog->showMessage(tr("HDR Grab failed switching to normal grab!"));
            microscope.GetFrame(*OrigImg);
        }
        catch (Hardware::Exception::CouldNotGrabImageException &ei2)
        {
            // show default test image and error dialog
            errorMessageDialog->showMessage(tr("Normal Grab failed switching to test image!"));
            *OrigImg = cv::imread("../Images/SoilSample1.png");
        }
    }
    SetMatToMainView(*OrigImg);
}

void VSAGUI::SetMatToMainView(cv::Mat &img)
{
    QImage *qOrigImg = new QImage(OpenCVQT::Mat2QImage(img));
    QPixmap *qOrigPix = new QPixmap(QPixmap::fromImage(*qOrigImg));
    ui->MainImg->setPixmap(*qOrigPix);
    ui->MainImg->show();
}

VSAGUI::~VSAGUI()
{
    delete ui;
}

void VSAGUI::on_SnapshotButton_clicked()
{
    delete OrigImg;
    OrigImg = new cv::Mat;
    Hardware::Microscope microscope;
    this->statusLabel->setText(tr("Grabbing new Image!"));
    finished_sig = microscope.connect_Finished(boost::bind(&VSAGUI::on_miscroscope_finished, this));
    progress_sig = microscope.connect_Progress(boost::bind(&VSAGUI::on_microscope_update, this, _1));
    microscope.GetHDRFrame(*OrigImg, 3);
}

void VSAGUI::on_SegmentButton_clicked()
{
    convertor = Vision::Conversion(*OrigImg);
    convertor.Convert(Vision::Conversion::RGB, Vision::Conversion::Intensity);
    enhancer = Vision::Enhance(convertor.ProcessedImg.clone());
    //enhancer.AdaptiveContrastStretch(9, 5);
    enhancer.Blur(9, true);
    SetMatToMainView(enhancer.ProcessedImg);
}

void VSAGUI::on_miscroscope_finished()
{
    SetMatToMainView(*OrigImg);
    this->statusLabel->setText(tr("New Image Grabbed"));
}

void VSAGUI::on_microscope_update(int prog)
{
    this->progressBar->setValue(prog);
}
