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

typedef std::complex<double> Complex_t;
typedef std::vector<Complex_t> ComplexVect_t;
typedef std::valarray<Complex_t> ComplexArray_t;
typedef std::vector<uint32_t> iContour_t;
typedef std::vector<std::pair<bool, iContour_t>> iCountourVect_t;

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

		ComplexVect_t DijkstraContour2Complex(const cv::Mat &img);
		ComplexVect_t Contour2Complex(const cv::Mat &img);

		iContour_t Neighbors(uchar *O, int pixel, uint32_t columns, uint32_t rows);

		void fft(ComplexArray_t  &CA);
		void ifft(ComplexArray_t &CA);

	};
}
