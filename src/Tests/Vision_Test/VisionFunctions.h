#ifndef VISIONFUNCTIONS_H
#define VISIONFUNCTIONS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "Vision.h"

class VisionFunctions{

    public:
        void Laplace(Mat &src, Mat &newImage,int kernelSize);
        void CLAHE(Mat &src, Mat &newImage, int GridSize);
        void AdaptiveThreshold(Mat &src, Mat &newImage, uchar kernelSize, Vision::Segment::TypeOfObjects Object);

    private:
        uint8_t GetThresholdLevel(Mat &src, Vision::Segment::TypeOfObjects TypeObject,int thresholdOffset, int sigma);

};

#endif // VISIONFUNCTIONS_H
