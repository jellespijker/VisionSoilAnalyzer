#define NO_OF_BLOBS 379

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
    void TestCase_LabelBlobs();
};

Vision_Test::Vision_Test()
{
}

void Vision_Test::TestCase_SimpleLabelBlob() {
  uchar testImgData[100] = { 0, 0, 1, 1, 0, 0, 0, 1, 1, 1,
                             0, 0, 0, 1, 0, 0, 0, 1, 1, 1,
                             1, 0, 0, 1, 0, 1, 0, 0, 0, 1,
                             1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
                             1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
                             0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
                             0, 0, 0, 1, 1, 1, 0, 0, 1, 1,
                             0, 0, 1, 1, 0, 1, 0, 0, 1, 1,
                             0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
                             0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

  cv::Mat testImg(10, 10, CV_8UC1, testImgData);
  testImg *= 255;
  Vision::Segment Segmenter(testImg);
  Segmenter.ConvertToBW(Vision::Segment::Bright);
  Segmenter.GetBlobList(true);
  uint16_t totalNoOfBlobs = Segmenter.MaxLabel;
  QCOMPARE(totalNoOfBlobs, uint16_t(3));
}

void Vision_Test::TestCase_LabelBlobs()
{
    cv::Mat testImg = cv::imread("379_blobs.ppm", 1);

    Vision::Segment Segmenter(testImg);
    Segmenter.ConvertToBW(Vision::Segment::Dark);
    Segmenter.GetBlobList(true);
    uint16_t totalNoOfBlobs = Segmenter.MaxLabel;
    QCOMPARE(totalNoOfBlobs, uint16_t(NO_OF_BLOBS));
}

QTEST_APPLESS_MAIN(Vision_Test)

#include "tst_vision_test.moc"
