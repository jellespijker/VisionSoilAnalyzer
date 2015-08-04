#define NO_OF_BLOBS 379
#define SMALL_NO_OF_BLOBS 4

#include <QString>
#include <QtTest>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "Vision.h"
#include "VisionFunctions.h"

class Vision_Test : public QObject
{
    Q_OBJECT

public:
    Vision_Test();

private Q_SLOTS:
    void TestCase_SimpleLabelBlob();
    void TestCase_SingleComplexLabelBlob();
    void TestCase_LabelBlobs();
    void TestCase_PhaseContrast();
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
    uint16_t totalNoOfBlobs = Segmenter.MaxLabel;
    QCOMPARE(totalNoOfBlobs, uint16_t(NO_OF_BLOBS));
}

void Vision_Test::TestCase_PhaseContrast()
{
    cv::Mat I = cv::imread("berg1_image_1.png", 1);
    cv::Mat dst1;
    cv::Mat dst2;
    Vision::Conversion Converter;
    Vision::Segment Segmenter;
    Vision::Enhance Enhancer;
    VisionFunctions Tests;
    // 2intensity
    //Converter.Convert(I, dst, Vision::Conversion::RGB, Vision::Conversion::Intensity);
    /* idea RGB -> BW via threshold for each seperate channel
     * perform local histogram stretching
     * perform local histogram thresholding
     * blur -> threshold -> laplace
     */

    // rgb2i
    Converter.Convert(I, dst1, Vision::Conversion::RGB, Vision::Conversion::Intensity);
    namedWindow(to_string(1), WINDOW_NORMAL);
    imshow(to_string(1), dst1);

    // blur
    //Enhancer.Blur(dst1, dst2, 7);
    //namedWindow(to_string(2), WINDOW_NORMAL);
    //imshow(to_string(2), dst2);
    for(int i = 3; i < 20; i+= 2){
        // blur
        Enhancer.Blur(dst1, dst2, i);
        namedWindow(to_string(2), WINDOW_NORMAL);
        imshow(to_string(2), dst2);

        // adaptive contrast stretch
        Tests.CLAHE(dst2, dst2, 3);
        namedWindow(to_string(i+2), WINDOW_NORMAL);
        imshow(to_string(i+2), dst2);
    }
    /*
    // blur
    Enhancer.Blur(dst2, dst1, 11);
    namedWindow(to_string(4), WINDOW_NORMAL);
    imshow(to_string(4), dst1);

    // add images dst1 + dst2
    cv::Mat sumImage = dst1 + dst2;
    namedWindow(to_string(5), WINDOW_NORMAL);
    imshow(to_string(5), sumImage);

    // convert BW standard
    Segmenter.ConvertToBW(sumImage, dst1, Vision::Segment::Dark);
    namedWindow(to_string(6), WINDOW_NORMAL);
    dst1 *= 255;
    imshow(to_string(6), dst1);

*/
    /*
    // convert BW adaptive
    Tests.AdaptiveThreshold(sumImage, dst1, 201, Vision::Segment::Dark);
    namedWindow(to_string(3), WINDOW_NORMAL);
    dst1 *= 255;
    imshow(to_string(3), dst1);

    // laplace HPF
    Tests.Laplace(dst1, dst2, 11);
    namedWindow(to_string(5), WINDOW_NORMAL);
    imshow(to_string(5), dst2);
    */


    SHOW_DEBUG_IMG(I, uint16_t, 1, "Image 1", false);
}

QTEST_APPLESS_MAIN(Vision_Test)

#include "tst_vision_test.moc"
