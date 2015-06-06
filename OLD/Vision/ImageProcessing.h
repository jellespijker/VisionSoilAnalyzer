#pragma once
/*! Current class version*/
#define IMAGEPROCESSING_VERSION 1

/*! MACRO which sets the original pointer to the original image or a clone of the earlier processed image */
#define CHAIN_PROCESS(chain, O, type)	if (chain) { TempImg = ProcessedImg.clone(); O = (type *)TempImg.data; } else { O = (type *)OriginalImg.data; }
/*! MACRO which trows an EmtpyImageException if the matrix is empty*/
#define EMPTY_CHECK(img) if (img.empty()) { throw Exception::EmtpyImageException(); }

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <stdint.h>
#include <cmath>
#include <vector>
#include <string>

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include "EmptyImageException.h"
#include "WrongKernelSizeException.h"
#include "ChannelMismatchException.h"
#include "PixelValueOutOfBoundException.h"
#include "VisionDebug.h"

using namespace cv;

namespace Vision
{
	class ImageProcessing
	{
    public:
        typedef boost::signals2::signal<void (float, std::string)> Progress_t;
        boost::signals2::connection connect_Progress(const Progress_t::slot_type &subscriber);

	protected:
		uchar* GetNRow(int nData, int hKsize, int nCols, uint32_t totalRows);
		Mat TempImg;

        Progress_t prog_sig;

	public:
		ImageProcessing();
		~ImageProcessing();
		Mat OriginalImg;
		Mat ProcessedImg;

        double currentProg = 0.;
        double ProgStep = 0.;

		static std::vector<Mat> extractChannel(const Mat &src);

		/*! Copy a matrix to a new matrix with a LUT mask
		\param src the source image
		\param *LUT type T with a LUT to filter out unwanted pixel values
		\param cvType an in where you can pas CV_UC8C1 etc.
		\return The new matrix
		*/
		template <typename T1, typename T2>
		static Mat CopyMat(const Mat &src, T1 *LUT, int cvType)
		{
			Mat dst(src.size(), cvType);
			uint32_t i = 0, i_src = 0;
			uint32_t nData = src.rows * src.cols * dst.step[1];
			if (cvType == 0 || cvType == 8 || cvType == 16 || cvType == 24)
			{
				for (uint32_t i = 0; i < nData; i += dst.step[1])
				{
					dst.data[i] = static_cast<uint8_t>(LUT[*(T2*)(src.data + (i * src.step[1]))]);
				}
			}
			else if (cvType == 1 || cvType == 9 || cvType == 17 || cvType == 25)
			{
				for (uint32_t i = 0; i < nData; i += src.step[1])
				{
					dst.data[i] = static_cast<int8_t>(LUT[*(T2*)(src.data + (i * src.step[1]))]);
				}
			}
			else if (cvType == 2 || cvType == 10 || cvType == 18 || cvType == 26)
			{
				for (uint32_t i = 0; i < nData; i += src.step[1])
				{
					dst.data[i] = static_cast<uint16_t>(LUT[*(T2*)(src.data + (i * src.step[1]))]);
				}
			}
			else if (cvType == 3 || cvType == 11 || cvType == 19 || cvType == 27)
			{
				for (uint32_t i = 0; i < nData; i += src.step[1])
				{
					dst.data[i] = static_cast<int16_t>(LUT[*(T2*)(src.data + (i * src.step[1]))]);
				}
			}
			else if (cvType == 4 || cvType == 12 || cvType == 20 || cvType == 28)
			{
				for (uint32_t i = 0; i < nData; i += src.step[1])
				{
					dst.data[i] = static_cast<int32_t>(LUT[*(T2*)(src.data + (i * src.step[1]))]);
				}
			}
			return dst;
		}

		/*! Copy a matrix to a new matrix with a mask
		\param src the source image
		\param *LUT type T with a LUT to filter out unwanted pixel values
		\param cvType an in where you can pas CV_UC8C1 etc.
		\return The new matrix
		*/
		template <typename T1>
		static Mat CopyMat(const Mat &src, const Mat &mask, int cvType)
		{
			if (src.size != mask.size) { throw Exception::WrongKernelSizeException("Mask not the same size as src Exception!"); }
			if (mask.channels() != 1) { throw Exception::WrongKernelSizeException("Mask has more then 1 channel Exception!"); }
			Mat dst(src.size(), cvType);

			vector<Mat> exSrc = Vision::ImageProcessing::extractChannel(src);
			vector<Mat> exDst;

			int cvBaseType = cvType % 8;
			for_each(exSrc.begin(), exSrc.end(), [&](const Mat & sItem) 
			{	
				Mat dItem(src.size(), cvBaseType);
				std::transform(sItem.begin<T1>(), sItem.end<T1>(), mask.begin<T1>(), dItem.begin<T1>(), [](const T1 &s, const T1 &m) -> T1 { return s * m; });
				exDst.push_back(dItem);
			});

			merge(exDst, dst);

			return dst;
		}

		template<typename T1>
		static void ShowDebugImg(cv::Mat img, T1 maxVal, std::string windowName, bool scale = true)
		{
			if (img.rows > 0 && img.cols > 0)
			{
				cv::Mat tempImg(img.size(), img.type());
				if (scale == true)
				{
					std::vector<cv::Mat> exSrc = extractChannel(img);
					std::vector<cv::Mat> exDst;
					int cvBaseType = img.type() % 8;
					T1 MatMin = std::numeric_limits<T1>::max();
					T1 MatMax = std::numeric_limits<T1>::min();

					// Find the global max and min
					for_each(exSrc.begin(), exSrc.end(), [&](const Mat & sItem)
					{
						std::for_each(sItem.begin<T1>(), sItem.end<T1>(), [&](const T1 &s) { if (s > MatMax) { MatMax = s; } else if (s < MatMin) { MatMin = s; } });
					});

					int Range = MatMax - MatMin;
					if (Range < 1) Range = maxVal;

					// Convert the values
					for_each(exSrc.begin(), exSrc.end(), [&](const cv::Mat & sItem)
					{
						Mat dItem(img.size(), cvBaseType);
						std::transform(sItem.begin<T1>(), sItem.end<T1>(), dItem.begin<T1>(), [&](const T1 &s) -> T1 { return (T1)round(((s - MatMin) * maxVal) / Range); });
						exDst.push_back(dItem);
					});

					merge(exDst, tempImg);
				}
				else
				{
					tempImg = img;
				}
				cv::namedWindow(windowName, cv::WINDOW_NORMAL);
				cv::imshow(windowName, tempImg);
				cv::waitKey(0);
				cv::destroyWindow(windowName);
			};
		};

	};
}
