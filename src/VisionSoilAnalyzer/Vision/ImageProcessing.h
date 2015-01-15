#pragma once
/*! Current class version*/
#define IMAGEPROCESSING_VERSION 1

/*! MACRO which sets the original pointer to the original image or a clone of the earlier processed image */
#define CHAIN_PROCESS(chain, O, type)	if (chain) { TempImg = ProcessedImg.clone(); O = (type *)TempImg.data; } else { O = (type *)OriginalImg.data; }
/*! MACRO which trows an EmtpyImageException if the matrix is empty*/
#define EMPTY_CHECK(img) if (img.empty()) { throw Exception::EmtpyImageException(); }

#include <stdint.h>
#include <opencv2/core.hpp>
#include <cmath>

#include "EmptyImageException.h"
#include "WrongKernelSizeException.h"
#include "PixelValueOutOfBoundException.h"

using namespace cv;

namespace Vision
{
	class ImageProcessing
	{
	protected:
		uchar* GetNRow(int nData, int hKsize, int nCols);
		Mat TempImg;

	public:
		ImageProcessing();
		~ImageProcessing();
		Mat OriginalImg;
		Mat ProcessedImg;

		/*! Copy a matrix to a new matrix with a LUT mask
		\param src the source image
		\param *LUT type T with a LUT to filter out unwanted pixel values
		\param cvType an in where you can pas CV_UC8C1 etc.
		\return The new matrix
		*/
		template <typename T>	Mat CopyMat(const Mat src, T *LUT, int cvType)
		{
			Mat dst(src.size(), cvType);
			uint32_t i = 0;
			uint32_t nData = src.rows * src.cols;
			while (i < nData)
			{
				dst.data[i] = LUT[src.data[i]];
				i++;
			}
			return dst;
		}
	};
}
