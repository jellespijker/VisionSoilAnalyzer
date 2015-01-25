#include "FFT.h"

namespace SoilMath
{
	FFT::FFT()
	{
	}


	FFT::~FFT()
	{
	}

	ComplexVect_t FFT::GetDescriptors(const cv::Mat img)
	{
		if (!fftDescriptors.empty()) { return fftDescriptors; }

		Contour2Complex(img);
	}

	ComplexVect_t FFT::Contour2Complex(cv::Mat img)
	{
		ComplexVect_t contour;
		int col = img.cols;
		int row = img.rows;

		// Loop through the image to find the first top/left most pixel and moves CW from there
		uint32_t i = 0, j = 0;
		int LUT_nBore[8] = { 1, 1 + col, col, col - 1, -1, -col - 1, -col, -col + 1 };
		uint32_t pEnd = col * row;
		while (i < pEnd)
		{
			if (img.data[i] == 1) { break; }
			i++;
		}

		// First Point
		Complex_t cPoint;
		cPoint.real((float)(i % row) / col);
		cPoint.imag(floorf((float)(i / col)) / row);
		contour.push_back(cPoint);

		// Loop through
		i--;
		while (contour.begin() != contour.end())
		{
			j = 0;
			while (true)
			{
				if (img.data[i + LUT_nBore[j]] == 1) { break; }
				j++;
			}
			cPoint.real((float)(i + LUT_nBore[j] % row) / col);
			cPoint.imag(floorf((float)(i + LUT_nBore[j] / col)) / row);
			contour.push_back(cPoint);
			i++;
		}
		contour.erase(contour.end());

		return contour;
	}

	void FFT::fft(ComplexArray_t &CA)
	{
		const size_t N = CA.size();

		ComplexArray_t even = CA[std::slice(0, N / 2, 2)];
		ComplexArray_t  odd = CA[std::slice(1, N / 2, 2)];

		fft(even);
		fft(odd);

		for (size_t k = 0; k < N / 2; ++k)
		{
			Complex_t ct = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
			CA[k] = even[k] + ct;
			CA[k + N / 2] = even[k] - ct;
		}
	}

	void FFT::ifft(ComplexArray_t &CA)
	{
		CA = CA.apply(std::conj);
		fft(CA);
		CA = CA.apply(std::conj);
		CA /= CA.size();
	}

	
}