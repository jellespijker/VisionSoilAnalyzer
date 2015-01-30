#pragma once

#include <vector>
#include <complex>
#include <cmath>
#include <valarray>
#include <iterator>
#include <algorithm>
#include <stdint.h>
#include <opencv2/core.hpp>
#include "SoilMathTypes.h"

typedef std::complex<double> Complex_t;
typedef std::vector<Complex_t> ComplexVect_t;
typedef std::valarray<Complex_t> ComplexArray_t;

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

		ComplexVect_t Contour2Complex(const cv::Mat &img);
		void fft(ComplexArray_t  &CA);
		void ifft(ComplexArray_t &CA);

	};
}
