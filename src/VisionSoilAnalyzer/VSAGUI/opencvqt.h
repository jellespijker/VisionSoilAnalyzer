#ifndef OPENCVQT_H
#define OPENCVQT_H
#include <QImage>
#include <opencv2/core.hpp>
#include <vector>

class OpenCVQT
{
public:
    OpenCVQT();
    static QImage Mat2QImage(const cv::Mat &src)
    {
       QImage dest ;
       if (src.channels() == 1)
       {
           cv::Mat destRGB;
           std::vector<cv::Mat> grayRGB(3, src);
           cv::merge(grayRGB, destRGB);
           dest = QImage((uchar *)destRGB.data, destRGB.cols, destRGB.rows, destRGB.step, QImage::Format_RGB888);
       }
       else
       {
           dest = QImage((uchar *)src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);
           dest = dest.rgbSwapped();
       }
       return dest;
    }
};

#endif // OPENCVQT_H
