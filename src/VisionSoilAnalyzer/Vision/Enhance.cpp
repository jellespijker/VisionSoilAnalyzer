/*! \class Enhance
class which enhances a greyscale cv::Mat image
*/
#include "Enhance.h"

namespace Vision
{
	/*! Constructor*/
	Enhance::Enhance() {	}

	/*! Constructor
	\param src cv::Mat source image
	*/
	Enhance::Enhance(const Mat& src)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
	}

	/*! Constructor
	\param src cv::Mat source image
	\param dst cv::Mat destination image
	\param kernelsize an uchar which represent the kernelsize should be an uneven number higher than two
	\param factor float which indicates the amount the effect should take place standard value is 1.0 only used in the adaptive contrast stretch enhancement
	\param operation enumerator EnhanceOperation which enhancement should be performed
	*/
	Enhance::Enhance(const Mat& src, Mat& dst, uchar kernelsize, float factor, EnhanceOperation operation)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		switch (operation)
		{
		case Vision::Enhance::_AdaptiveContrastStretch:
			AdaptiveContrastStretch(kernelsize, factor);
			break;
		case Vision::Enhance::_Blur:
			Blur(kernelsize);
			break;
		case Vision::Enhance::_HistogramEqualization:
			HistogramEqualization();
			break;
		}
		dst = ProcessedImg;
	}

	/*! Dec-constructor*/
	Enhance::~Enhance() { }

	/*! Calculate the standard deviation of the neighboring pixels
	\param O uchar pointer to the current pixel of the original image
	\param i current counter
	\param hKsize half the kernelsize
	\param nCols total number of columns
	\param noNeighboursPix total number of neighboring pixels
	\param mean mean value of the neighboring pixels
	\return standard deviation
	*/
	float Enhance::CalculateStdOfNeighboringPixels(uchar *O, int i, int hKsize, int nCols, int noNeighboursPix, float mean)
	{
		register float sum_dev = 0.0;
		register float Std = 0.0;
		int k;
		int l;
		sum_dev = 0.0;
		Std = 0.0;
		k = -hKsize;
		while (k++ <= hKsize)
		{
			l = -hKsize;
			while (l++ <= hKsize) { sum_dev += pow((O[i + k * nCols + l] - mean), 2); }
		}
		Std = sqrt(sum_dev / noNeighboursPix);
		return Std;
	}

	/*! Calculate the sum of the neighboring pixels
	\param O uchar pointer to the current pixel of the original image
	\param i current counter
	\param hKsize half the kernelsize
	\param nCols total number of columns
	\param sum Total sum of the neighboringpixels
	*/
	void Enhance::CalculateSumOfNeighboringPixels(uchar *O, int i, int hKsize, int nCols, uint32_t &sum)
	{
		register int k;
		k = -hKsize;
		register int l;
		while (k++ <= hKsize)
		{
			l = -hKsize;
			while (l++ <= hKsize) { sum += O[i + k * nCols + l]; }
		}
	}

	/*! Homebrew AdaptiveContrastStretch function which calculate the mean and standard deviation from the neighboring pixels if the current pixel is higher then the mean the value is incremented with an given factor multiplied with the standard deviation, and decreased if it's lower then the mean.
	\param src cv::Mat source image
	\param dst cv::Mat destination image
	\param kernelsize an uchar which represent the kernelsize should be an uneven number higher than two
	\param factor float which indicates the amount the effect should take place standard value is 1.0 only used in the adaptive contrast stretch enhancement
	*/
	void Enhance::AdaptiveContrastStretch(const Mat &src, Mat &dst, uchar kernelsize, float factor)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		AdaptiveContrastStretch(kernelsize, factor);
		dst = ProcessedImg;
	}

	/*! Homebrew AdaptiveContrastStretch function which calculate the mean and standard deviation from the neighboring pixels if the current pixel is higher then the mean the value is incremented with an given factor multiplied with the standard deviation, and decreased if it's lower then the mean.
	\param kernelsize an uchar which represent the kernelsize should be an uneven number higher than two
	\param factor float which indicates the amount the effect should take place standard value is 1.0 only used in the adaptive contrast stretch enhancement
	\param chain use the results from the previous operation default value = false;
	*/
	void Enhance::AdaptiveContrastStretch(uchar kernelsize, float factor, bool chain)
	{
		// Exception handling
		EMPTY_CHECK(OriginalImg);
		if (kernelsize < 3 || (kernelsize % 2) == 0) { throw Exception::WrongKernelSizeException(); }
		CV_Assert(OriginalImg.depth() != sizeof(uchar));

		// Make the pointers to the Data
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uchar *P = ProcessedImg.data;

		register uint32_t i = 0;
		int hKsize = kernelsize / 2;
		int nCols = OriginalImg.cols;
		register int pStart = (hKsize * nCols) + hKsize + 1;

		int nData = OriginalImg.rows * OriginalImg.cols;
		register int pEnd = nData - pStart;
		uint32_t noNeighboursPix = kernelsize * kernelsize;
		register uint32_t sum;
		register float mean = 0.0;

		uchar *nRow = GetNRow(nData, hKsize, nCols, OriginalImg.rows);

		i = pStart;
		while (i++ < pEnd)
		{
			// Checks if pixel isn't a border pixel and progresses to the new row
			if (nRow[i] == 1) { i += kernelsize; }

			// Fill the neighboring pixel array
			sum = 0;
			mean = 0;

			// Calculate the statistics
			CalculateSumOfNeighboringPixels(O, i, hKsize, nCols, sum);
			mean = (float)(sum / noNeighboursPix);
			float Std = CalculateStdOfNeighboringPixels(O, i, hKsize, nCols, noNeighboursPix, mean);

			// Stretch

			if (O[i] > mean)
			{
				int addValue = O[i] + (int)(round(factor * Std));
				if (addValue < 255) { P[i] = addValue; }
				else { P[i] = 255; }
			}
			else if (O[i] < mean)
			{
				int subValue = O[i] - (int)(round(factor * Std));
				if (subValue > 0) { P[i] = subValue; }
				else { P[i] = 0; }
			}
			else { P[i] = O[i]; }
		}

		// Stretch the image with an normal histogram equalization
		HistogramEqualization(true);
	}

	/*! Blurs the image with a NxN kernel
	\param src cv::Mat source image
	\param dst cv::Mat destination image
	\param kernelsize an uchar which represent the kernelsize should be an uneven number higher than two
	*/
	void Enhance::Blur(const Mat& src, Mat& dst, uchar kernelsize)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		Blur(kernelsize);
		dst = ProcessedImg;
	}

	/*! Blurs the image with a NxN kernel
	\param kernelsize an uchar which represent the kernelsize should be an uneven number higher than two
	\param chain use the results from the previous operation default value = false;
	*/
	void Enhance::Blur(uchar kernelsize, bool chain)
	{
		// Exception handling
		EMPTY_CHECK(OriginalImg);
		if (kernelsize < 3 || (kernelsize % 2) == 0) { throw Exception::WrongKernelSizeException(); }
		CV_Assert(OriginalImg.depth() != sizeof(uchar));

		// Make the pointers to the Data
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uchar *P = ProcessedImg.data;

		int nData = OriginalImg.rows * OriginalImg.cols;
		int hKsize = kernelsize / 2;
		int nCols = OriginalImg.cols;
		register int pStart = (hKsize * nCols) + hKsize + 1;
		register int pEnd = nData - pStart;
		int noNeighboursPix = kernelsize * kernelsize;
		register uint32_t sum;

		uint32_t i;
		uchar *nRow = GetNRow(nData, hKsize, nCols, OriginalImg.rows);
		i = pStart;
		while (i++ < pEnd)
		{
			// Checks if pixel isn't a border pixel and progresses to the new row
			if (nRow[i] == 1) { i += kernelsize; }

			// Calculate the sum of the kernel
			sum = 0;
			CalculateSumOfNeighboringPixels(O, i, hKsize, nCols, sum);

			P[i] = (uchar)(round(sum / noNeighboursPix));
		}
	}

	/*! Stretches the image using a histogram
	\param chain use the results from the previous operation default value = false;
	*/
	void Enhance::HistogramEqualization(bool chain)
	{
		// Exception handling
		EMPTY_CHECK(OriginalImg);
		CV_Assert(OriginalImg.depth() != sizeof(uchar));

		// Make the pointers to the Data
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uchar *P = ProcessedImg.data;

		// Calculate the statics of the whole image
		ucharStat_t imgStats(O, OriginalImg.rows, OriginalImg.cols);
		register float sFact;
		if (imgStats.min != imgStats.max) { sFact = 255.0f / (imgStats.max - imgStats.min); }
		else { sFact = 1.0f; }

		uint32_t i = 256;
		register uchar LUT_changeValue[256];
		while (i-- > 0) { LUT_changeValue[i] = (uchar)(((float)(i)* sFact) + 0.5f); }

		O = OriginalImg.data;

		i = OriginalImg.cols * OriginalImg.rows + 1;
		while (i-- > 0) { *P++ = LUT_changeValue[*O++ - imgStats.min]; }
	}
}