#pragma once

#include <vector>
#include <complex>
#include <cmath>
#include <valarray>
#include <array>
#include <deque>
#include <queue>
#include <iterator>
#include <algorithm>
#include <stdint.h>
#include <opencv2/core.hpp>
#include "SoilMathTypes.h"
#include "MathException.h"

namespace SoilMath
{
	class FFT
	{
	public:
		FFT();
		~FFT();

		ComplexVect_t GetDescriptors(const cv::Mat &img);

	private:
		ComplexVect_t fftDescriptors;
		ComplexVect_t complexcontour;
		cv::Mat Img;

		ComplexVect_t Contour2Complex(const cv::Mat &img, float centerCol, float centerRow);

		iContour_t Neighbors(uchar *O, int pixel, uint32_t columns, uint32_t rows);

		void fft(ComplexArray_t  &CA);
		void ifft(ComplexArray_t &CA);

	};
}