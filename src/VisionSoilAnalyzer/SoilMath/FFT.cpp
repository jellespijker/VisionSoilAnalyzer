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


	// Find the shortest path
	ComplexVect_t FFT::DijkstraContour2Complex(const cv::Mat &img)
	{

	}

	ComplexVect_t FFT::Contour2Complex(const cv::Mat &img)
	{
		uchar *O = img.data;

		uint  cContour = 0;
		iCountourVect_t Contours;

		uint32_t pEnd = img.rows * img.cols;

		for (uint32_t i = 0; i < pEnd; i++)
		{
			if (O[i] == 1)
			{
				Contours[cContour].second.push_back(i);
				iContour_t nBors = Neighbors(O, i, img.cols, img.rows);
				if (nBors.size() > 2) // Choice point
				{
					for_each(nBors.begin(), nBors.end(), [&nBors, &Contours, &cContour](uint32_t &n)
					{
						uint32_t count = 0;
						bool first = true;
						if (n != Contours[cContour].second[Contours[cContour].second.size() - 2] && n != Contours[cContour].second[0]) // Continuous loop
						{
							if (first == true) { Contours[cContour].second.push_back(n); }
							else
							{
								Contours.push_back(Contours[cContour]);
								Contours[cContour].second[Contours[cContour].second.size() - 1] = n;
							}
						}
						else if (n != Contours[cContour].second[Contours[cContour].second.size() - 2] && n == Contours[cContour].second[0]) // Back @ start point
						{
							Contours[cContour + count].first = true;;
						}
						count++;
					});
				}
				else if (nBors.size() == 1) // death-end
				{

				}

				// Normal handling
			}
		}
		if (cContour.size() < 1) { throw Exception::MathException("No contour found in image!"); }

	}

	iContour_t Neighbors(uchar *O, int pixel, uint32_t columns, uint32_t rows)
	{
		int LUT_nBore[8] = { 1, 1 + columns, columns, columns - 1, -1, -columns - 1, -columns, -columns + 1 };
		iContour_t neighbors;
		uint32_t pEnd = rows * columns;
		uint32_t count = 0;
		for (uint32_t i = 0; i < 8; i++)
		{
			count = pixel + LUT_nBore[i];
			while ((count < 0 || count >= pEnd) && i < 8) { count = pixel + LUT_nBore[++i]; }
			if (O[count] == 1) neighbors.push_back(LUT_nBore[i]);
		}
		return neighbors;
	}

	//Can only be used on contours that can't double back up on them self
	ComplexVect_t FFT::Contour2Complex(const cv::Mat &img)
	{
		uchar *O = img.data;

		uint32_t i = 0, j = 0, k = 1, count = 0, backup = 0, bcount;
		uint32_t pEnd = img.cols * img.rows;
		int LUT_nBore[8] = { 1, 1 + img.cols, img.cols, img.cols - 1, -1, -img.cols - 1, -img.cols, -img.cols + 1 };

		std::vector<uint32_t> foundEdge;
		std::vector<uint32_t> foundAllEdge;
		ComplexVect_t contour;
		bool troep = false;
		for (uint32_t i = 0; i < pEnd; i++)
		{
			if (O[i] == 1)
			{
				foundEdge.push_back(i);
				for (uint32_t j = 0; j < 8; j++)
				{
					count = i + LUT_nBore[j];
					while ((count < 0 || count >= pEnd) && j < 8) {	count = i + LUT_nBore[++j];	}
					if (O[count] == 1)
					{
						std::vector<uint32_t>::iterator itt = std::find(foundEdge.begin(), foundEdge.end(), count);
						if (itt == foundEdge.end())  // newly found point hasn't been previously visited
						{
							i = count - 1;
							break;
						}
						else if (itt == foundEdge.begin()) // newly found point is the start of contour and therefore the current point is the end
						{
							i = pEnd;
							break;
						}
						else if (foundEdge[foundEdge.size() - 2] != count) // if the newly found point deviate from the previous point but is still visited before iterate back 
						{
							k = 1;
							bool runItt = true;
							while (runItt)
							{
								backup = foundEdge[foundEdge.size() - k];
								j = 0;
								while (j < 8)
								{
									bcount = backup + LUT_nBore[j++];
									while ((bcount < 0 || bcount >= pEnd) && j < 8) { bcount = i + LUT_nBore[++j]; }
									if (O[bcount] == 1 && std::find(foundEdge.begin(), foundEdge.end(), bcount) == foundEdge.end() && bcount != count)
									{
										i = bcount - 1;
										foundAllEdge.reserve(foundAllEdge.size() + k);
										foundAllEdge.insert(foundAllEdge.end(), foundEdge.end() - k, foundEdge.end());
										foundEdge.erase(foundEdge.end() - k, foundEdge.end());
										runItt = false;
										break;
									}
								}
								k++;
							}
						}
					}
				}
			}
		}

		Complex_t cPoint;
	
		for_each(foundEdge.begin(), foundEdge.end(), [&img, &cPoint, &contour](uint32_t &e)
		{
			if (e % img.cols == 0) { cPoint.real(1.0); }
			else { cPoint.real((double)((e % img.cols) / img.cols)); }
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