#define NO_OF_BLOBS 379
#define SMALL_NO_OF_BLOBS 4

#include <QString>
#include <QtTest>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "Vision.h"

class Vision_Test : public QObject
{
    Q_OBJECT

public:
    Vision_Test();

private Q_SLOTS:
    void TestCase_SimpleLabelBlob();
    void TestCase_SingleComplexLabelBlob();
    void TestCase_LabelBlobs();
};

Vision_Test::Vision_Test()
{
}

void Vision_Test::TestCase_SimpleLabelBlob() {
  uchar testImgData[100] = { 0, 0, 1, 1, 0, 0, 1, 1, 1, 1,
                             0, 0, 0, 1, 0, 0, 0, 1, 1, 1,
                             1, 0, 0, 1, 0, 1, 0, 0, 1, 1,
                             1, 1, 1, 1, 0, 1, 1, 0, 1, 1,
                             1, 1, 0, 0, 0, 0, 1, 0, 1, 1,
                             0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
                             1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
                             1, 1, 1, 0, 0, 1, 1, 0, 1, 1,
                             0, 0, 1, 1, 0, 1, 1, 0, 1, 1,
                             0, 0, 0, 1, 0, 0, 1, 1, 1, 1};

  cv::Mat testImg(10, 10, CV_8UC1, testImgData);
  testImg *= 255;
  Vision::Segment Segmenter(testImg);
  Segmenter.ConvertToBW(Vision::Segment::Bright);
  Segmenter.GetBlobList(true);
  uint16_t totalNoOfBlobs = Segmenter.MaxLabel;
  QCOMPARE(totalNoOfBlobs, uint16_t(SMALL_NO_OF_BLOBS));
}

void Vision_Test::TestCase_SingleComplexLabelBlob() {
  uchar testImgData[100] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
                             0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                             0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
                             0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             0, 0, 0, 1, 0, 0, 0, 1, 1, 1,
                             0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                             0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
                             0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
                             1, 1, 1, 1, 1, 1, 1, 0, 0, 0};

  cv::Mat testImg(10, 10, CV_8UC1, testImgData);
  testImg *= 255;
  Vision::Segment Segmenter(testImg);
  Segmenter.ConvertToBW(Vision::Segment::Bright);
  Segmenter.GetBlobList(true);
  uint16_t totalNoOfBlobs = Segmenter.MaxLabel;
  QCOMPARE(totalNoOfBlobs, uint16_t(1));

}

void Vision_Test::TestCase_LabelBlobs()
{
    cv::Mat testImg = cv::imread("379_blobs.ppm", 0);

    Vision::Segment Segmenter(testImg);
    Segmenter.ConvertToBW(Vision::Segment::Dark);
    Segmenter.GetBlobList(true);
    std::vector<int> params;
    params.push_back(CV_IMWRITE_PXM_BINARY);
    params.push_back(0);
    cv::Mat temp(Segmenter.LabelledImg.rows, Segmenter.LabelledImg.cols, CV_8UC1);
    Segmenter.LabelledImg.convertTo(temp, CV_8UC1);
    cv::imwrite("L.pgm", temp, params);
    SHOW_DEBUG_IMG(Segmenter.LabelledImg, uint16_t, uint16_t(-1), "segment labelblobs", true);
    uint16_t totalNoOfBlobs = Segmenter.MaxLabel;
    QCOMPARE(totalNoOfBlobs, uint16_t(NO_OF_BLOBS));
}

QTEST_APPLESS_MAIN(Vision_Test)

#include "tst_vision_test.moc"
