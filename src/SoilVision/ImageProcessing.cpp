/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class ImageProcessing
\brief Core class of all the image classes
Core class of all the image classes with a few commonly shared functions and
variables
*/
#include "ImageProcessing.h"

namespace Vision {
/*! Constructor of the core class*/
ImageProcessing::ImageProcessing() {}

/*! De-constructor of the core class*/
ImageProcessing::~ImageProcessing() {}

/*! Create a LUT indicating which iteration variable i is the end of an row
\param nData an int indicating total pixels
\param hKsize int half the size of the kernel, if any. which acts as an offset
from the border pixels
\param nCols int number of columns in a row
\return array of uchars where a zero is a middle column and a 1 indicates an end
of an row minus the offset from half the kernel size
*/
uchar *ImageProcessing::GetNRow(int nData, int hKsize, int nCols,
                                uint32_t totalRows) {
  // Create LUT to determine when there is an new row
  uchar *nRow = new uchar[nData + 1]{};
  // int i = 0;
  int shift = nCols - hKsize - 1;
  for (uint32_t i = 0; i < totalRows; i++) {
    nRow[(i * nCols) + shift] = 1;
  }
  return nRow;
}

std::vector<Mat> ImageProcessing::extractChannel(const Mat &src) {
  vector<Mat> chans;
  split(src, chans);
  return chans;
}

void ImageProcessing::getOrientented(cv::Mat &BW, cv::Point_<double> &centroid,
                                     double &theta, double &eccentricty) {
  cv::Moments Mu = cv::moments(BW, true);

  centroid.x = Mu.m10 / Mu.m00;
  centroid.y = Mu.m01 / Mu.m00;

  theta = 0;
  double muPrime20 = (Mu.m20 / Mu.m00) - pow(centroid.x, 2);
  double muPrime02 = (Mu.m02 / Mu.m00) - pow(centroid.y, 2);
  double diffmuprime2 = muPrime20 - muPrime02;
  double muPrime11 = (Mu.m11 / Mu.m00) - (centroid.x * centroid.y);

  if (diffmuprime2 != 0) {
    theta = 0.5 * atan((2 * muPrime11) / diffmuprime2);
  }

  double term1 = (muPrime20 + muPrime02) /2;
  double term2 = sqrt(4 * pow(muPrime11, 2) + pow(diffmuprime2, 2)) / 2;
  eccentricty = sqrt(1-(term1 - term2)/ (term1 + term2));
}

void ImageProcessing::RotateImg(Mat &src, Mat &dst, double &theta,
                                cv::Point_<double> &Centroid, cv::Rect &ROI) {
  cv::Mat temp;
  temp.setTo(0);
  double alpha = cos(theta);
  double beta = sin(theta);
  double cx = src.cols / 2;
  double cy = src.rows / 2;
  double dx = cx - Centroid.x;
  double dy = cy - Centroid.y;
  double rotData[2][3]{{alpha, beta, alpha * dx + beta * dy + Centroid.x},
                       {-beta, alpha, alpha * dy + beta * dx + Centroid.y}};
  cv::Mat totalrot(2, 3, CV_64FC1, rotData);

  cv::warpAffine(src, temp, totalrot, cv::Size(src.rows * 2.5, src.cols * 2.5),
                 INTER_LINEAR);
  // determine the actual ROI
  cv::Point minP(0, 0);
  if (src.channels() == 1) {
    uchar *O = temp.data;
    uint32_t nData = temp.rows * temp.cols;
    minP.x = temp.rows;
    minP.y = temp.cols;
    cv::Point maxP(0, 0);
    int X, Y;
    for (uint32_t i = 0; i < nData; i++) {
      if (O[i] != 0) {
        Y = floor(i / temp.cols);
        X = (i % temp.cols);
        if (X < minP.x) {
          minP.x = X;
        }
        if (Y < minP.y) {
          minP.y = Y;
        }
        if (X > maxP.x) {
          maxP.x = X;
        }
        if (Y > maxP.y) {
          maxP.y = Y;
        }
      }
    }
    ROI = cv::Rect(minP, maxP);
  }

  if (src.channels() > 1) {
    Centroid.x -= cx;
    Centroid.y -= cy;

    double xnew = Centroid.x * alpha - Centroid.y * beta;
    double ynew = Centroid.x * beta - Centroid.y * alpha;

    Centroid.x = xnew + cx + minP.x;
    Centroid.y = ynew + cy + minP.y;
  }
  dst = temp(ROI).clone();
}

boost::signals2::connection
ImageProcessing::connect_Progress(const Progress_t::slot_type &subscriber) {
  return prog_sig.connect(subscriber);
}
}
