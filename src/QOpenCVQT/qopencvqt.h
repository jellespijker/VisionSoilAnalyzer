/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#ifndef QOPENCVQT_H
#define QOPENCVQT_H

#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

class QOpenCVQT
{
public:
  QOpenCVQT();
  static cv::Mat WhiteBackground(const cv::Mat &src) {
    cv::Mat dst;
    cv::floodFill(src, dst, cv::Point(1,1), cv::Scalar_<uchar>(255,255,255));
    return dst;
  }

  static QImage Mat2QImage(const cv::Mat &src) {
    QImage dest;
    if (src.channels() == 1) {
      cv::Mat destRGB;
      std::vector<cv::Mat> grayRGB(3, src);
      cv::merge(grayRGB, destRGB);
      dest = QImage((uchar *)destRGB.data, destRGB.cols, destRGB.rows,
                    destRGB.step, QImage::Format_RGB888);
    } else {
      dest = QImage((uchar *)src.data, src.cols, src.rows, src.step,
                    QImage::Format_RGB888);
      dest = dest.rgbSwapped();
    }
    return dest;
  }
};

#endif // QOPENCVQT_H
