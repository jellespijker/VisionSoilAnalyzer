#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <QDebug>

#include "Vision.h"
#include "VisionFunctions.h"

void VisionFunctions::CLAHE(Mat &src, Mat &newImage, int GridSize = 3){
    // apply the CLAHE algorithm to the L channel
    std::vector<cv::Mat> lab_planes(3);
    cv::split(src, lab_planes);

    // create the CLAHE algorithm and apply it
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    //clahe->setClipLimit(GetThresholdLevel(src, Vision::Segment::Dark, 0, 2));
    clahe->setTilesGridSize(cv::Size(GridSize,GridSize));
    clahe->apply(lab_planes[0], newImage);

};

void VisionFunctions::Laplace(Mat &src, Mat &newImage,int kernelSize){
    cv::Laplacian(src, newImage, -1, kernelSize); // CV_16S
};

void VisionFunctions::AdaptiveThreshold(Mat &src, Mat &newImage, uchar kernelSize, Vision::Segment::TypeOfObjects Object){

    // reset the values in the newImage
    newImage = Scalar(0);

    // determine the length of an arm of the window
    int length = (kernelSize - 1)/2;

    for(int i = length; i < (src.cols - length); i++)
    {
        for(int j = length; j < (src.rows - length) ; j++)
        {
            /*
            // determine the threshold level for a local pixel
            //cv::Mat subImage = src.colRange(i-length,i+length).rowRange(j-length,j+length);
            //Mat subImage(src, cv::Rect(i - length, j - length, kernelSize, kernelSize));

            if (j == length && i == length){
                //imshow("one",subImage);
            }

            // determine if the local pixel is 0 or 1
            if (src.at<uchar>(j,i) < T){
                newImage.at<uchar>(j,i) = 255;
            }
            else{
                newImage.at<uchar>(j,i) = 0;
            }
            */
        }
    }
};

uint8_t VisionFunctions::GetThresholdLevel(Mat &src, Vision::Segment::TypeOfObjects TypeObject,int thresholdOffset = 0, int sigma = 2) {
  // Calculate the statistics of the whole picture
  ucharStat_t OriginalImgStats(src.data, src.rows, src.cols);

  // Sets the initial threshold with the mean of the total picture
  pair<uchar, uchar> T;
  T.first = (uchar)(OriginalImgStats.Mean + 0.5);
  T.second = 0;

  uchar Rstd = 0;
  uchar Lstd = 0;
  uchar Rmean = 0;
  uchar Lmean = 0;

  // Iterate till optimum Threshold is found between back- & foreground
  while (T.first != T.second) {
    // Gets an array of the left part of the histogram
    uint32_t i = T.first;
    uint32_t *Left = new uint32_t[i]{};
    while (i-- > 0) {
      Left[i] = OriginalImgStats.bins[i];
    }

    // Gets an array of the right part of the histogram
    uint32_t rightEnd = 256 - T.first;
    uint32_t *Right = new uint32_t[rightEnd]{};
    i = rightEnd;
    while (i-- > 0) {
      Right[i] = OriginalImgStats.bins[i + T.first];
    }

    // Calculate the statistics of both histograms,
    // taking into account the current threshold
    ucharStat_t sLeft(Left, 0, T.first);
    ucharStat_t sRight(Right, T.first, 256);

    // Calculate the new threshold the mean of the means
    T.second = T.first;
    T.first = (uchar)(((sLeft.Mean + sRight.Mean) / 2) + 0.5);

    Rmean = (uchar)(sRight.Mean + 0.5);
    Lmean = (uchar)(sLeft.Mean + 0.5);
    Rstd = (uchar)(sRight.Std + 0.5);
    Lstd = (uchar)(sLeft.Std + 0.5);
    delete[] Left;
    delete[] Right;
  }

  // Assumes the pixel value of the sought object lies between 2 sigma
  int val = 0;
  switch (TypeObject) {
  case Vision::Segment::Bright: // bright
    val = Rmean - (sigma * Rstd) - thresholdOffset;
    if (val < 0) {
      val = 0;
    } else if (val > 255) {
      val = 255;
    }
    T.first = (uchar)val;
    break;
  case Vision::Segment::Dark: // dark
    val = Lmean + (sigma * Lstd) + thresholdOffset;
    if (val < 0) {
      val = 0;
    } else if (val > 255) {
      val = 255;
    }
    T.first = (uchar)val;
    break;
  }

  return T.first;
}

