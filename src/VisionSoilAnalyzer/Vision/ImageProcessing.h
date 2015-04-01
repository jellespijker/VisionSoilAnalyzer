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
#include <vector>

#include "EmptyImageException.h"
#include "WrongKernelSizeException.h"
#include "ChannelMismatchException.h"
#include "PixelValueOutOfBoundException.h"

using namespace cv;

namespace Vision
{
	class ImageProcessing
	{
	protected:
		uchar* GetNRow(int nData, int hKsize, int nCols, uint32_t totalRows);
		Mat TempImg;

	public:
		ImageProcessing();
		~ImageProcessing();
		Mat OriginalImg;
		Mat ProcessedImg;

		static std::vector<Mat> extractChannel(const Mat &src);

		/*! Copy a matrix to a new matrix with a LUT mask
		\param src the source image
		\param *LUT type T with a LUT to filter out unwanted pixel values
		\param cvType an in where you can pas CV_UC8C1 etc.
		\return The new matrix
		*/
		template <typename T>
		static void CopyMat(const Mat &src, Mat &dst, T *LUT, int cvType)
		{
			if (dst.empty() && src.rows > 0 && src.cols > 0) { dst.create(src.rows, src.cols, cvType); }
			uint32_t i = 0;
			uint32_t nData = dst.rows * dst.cols * dst.step[1];
			for (uint32_t i = 0; i < nData; i++)
			{
				dst.data[i] = LUT[(T)src.data[i * src.step[1]]];
			}
		}

		/*! Copy a matrix to a new matrix with a mask
		\param src the source image
		\param *LUT type T with a LUT to filter out unwanted pixel values
		\param cvType an in where you can pas CV_UC8C1 etc.
		\return The new matrix
		*/
		template <typename T>
		static Mat CopyMat(const Mat &src, const Mat &mask, int cvType)
		{
			if (src.size != mask.size) { throw Exception::WrongKernelSizeException("Mask not the same size as src Exception!"); }
			Mat dst(src.size(), cvType);
			uint32_t i = 0;
			uint32_t nData = mask.rows * mask.cols;
			switch (dst.channels())
			{
			case 1:
				for (uint32_t i = 0; i < nData; i++)
				{
					if (mask.data[i] == 1) { dst.data[i] = (T)src.data[i]; }
					else { dst.data[i] = (T)0; }
				}
				break;
			case 2:
				for (uint32_t i = 0; i < nData; i++)
				{
					if (mask.data[i] == 1)
					{
						dst.data[i * 2] = (T)src.data[i * 2];
						dst.data[i * 2 + 1] = (T)src.data[i * 2 + 1];
					}
					else
					{
						dst.data[i * 2] = (T)0;
						dst.data[i * 2 + 1] = (T)0;
					}
				}
				break;
			case 3:
				for (uint32_t i = 0; i < nData; i++)
				{
					if (mask.data[i] == 1)
					{
						dst.data[i * 3] = (T)src.data[i * 3];
						dst.data[i * 3 + 1] = (T)src.data[i * 3 + 1];
						dst.data[i * 3 + 2] = (T)src.data[i * 3 + 2];
					}
					else
					{
						dst.data[i * 3] = (T)0;
						dst.data[i * 3 + 1] = (T)0;
						dst.data[i * 3 + 2] = (T)0;
					}
				}
				break;
			default:
				for (uint32_t i = 0; i < nData; i++)
				{
					if (mask.data[i] == 1) { for (uint32_t j = 0; j < dst.channels(); j++) { dst.data[i * dst.channels() + j] = (T)src.data[i * dst.channels() + j]; } }
					else { for (uint32_t j = 0; j < dst.channels(); j++) { dst.data[i * dst.channels() + j] = (T)0; } }
				}
				break;
			}
			return dst;
		}
	};
}
