#ifndef OPENCVQT_H
#define OPENCVQT_H
#include <QImage>
#include <opencv2/core.hpp>

class OpenCVQT
{
public:
    OpenCVQT();
    static QImage Mat2QImage(const cv::Mat &src, int cvType = 0)
    {
       QImage dest((uchar*)src.data, src.cols, src.rows, src.step1(), QImage::Format_RGB888);
       dest = dest.rgbSwapped();
       return dest;
    }
};

#endif // OPENCVQT_H
