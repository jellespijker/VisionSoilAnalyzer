#include "FFT.h"

namespace SoilMath
{
	FFT::FFT()
	{
	}


	FFT::~FFT()
	{
	}

	ComplexVect_t FFT::GetDescriptors(const cv::Mat &img)
	{
		if (!fftDescriptors.empty()) { return fftDescriptors; }

		complexcontour = Contour2Complex(img);

		// Supplement the vector of complex numbers so that N = 2^m
		uint32_t N = complexcontour.size();
		double logN = log(static_cast<double>(N)) / log(2.0);
		if (floor(logN) !=  logN)
		{
			// Get the next power of 2
			double nextLogN = floor(logN + 1.0);
			N = static_cast<uint32_t>(pow(2, nextLogN));

			uint32_t i = complexcontour.size();
			// Append the vector with zeros
			while (i++ < N)	{ complexcontour.push_back(Complex_t(0.0, 0.0)); }
		}

		ComplexArray_t ca(complexcontour.data(), complexcontour.size());
		fft(ca);
		fftDescriptors.assign(std::begin(ca), std::end(ca));
		return fftDescriptors;
	}

	ComplexVect_t FFT::Contour2Complex(const cv::Mat &img)
	{
		uchar *O = img.data;

		uint32_t i = 0, j = 0, count = 0;
		uint32_t pEnd = img.cols * img.rows;
		int LUT_nBore[8] = { 1, 1 + img.cols, img.cols, img.cols - 1, -1, -img.cols - 1, -img.cols, -img.cols + 1 };

		std::vector<uint32_t> foundEdge;
		ComplexVect_t contour;

		for (uint32_t i = 0; i < pEnd; i++)
		{
			if (foundEdge.size() > 1 && i == foundEdge[foundEdge.size() - 1] + 1) { break; } // End of the contour

			if (O[i] == 1)
			{
				foundEdge.push_back(i); 
				for (uint32_t j = 0; j < 8; j++)
				{
					count = i + LUT_nBore[j];
					while ((count < 0 || count > pEnd) && j < 8) { count = i + LUT_nBore[++j];	} // correction for edges of the image
					if (O[count] == 1)
					{
						if ((std::find(foundEdge.begin(), foundEdge.end(), count) == foundEdge.end())) // if i already present move to the next neighbore
						{
							i = count - 1;
							break;
						}
					}
				}
			}
		}

		Complex_t cPoint;
	
		for_each(foundEdge.begin(), foundEdge.end(), [&img, &cPoint, &contour](uint32_t &e)
		{
			cPoint.real((double)((e % img.rows) / img.cols));
			cPoint.imag((double)(floorf(e / img.cols) / img.rows));
			contour.push_back(cPoint);
		});

		return contour;
	}

	void FFT::fft(ComplexArray_t &CA)
	{
		const size_t N = CA.size();
		if (N <= 1) { return; }

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