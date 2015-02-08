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

	iContour_t FFT::Neighbors(uchar *O, int pixel, uint32_t columns, uint32_t rows)
	{
		//long int LUT_nBore[8] = { 1, 1 + columns, columns, columns - 1, -1, -columns - 1, -columns, -columns + 1 };
		long int LUT_nBore[8] = { -columns + 1, -columns, -columns - 1, -1, columns - 1, columns, 1 + columns, 1 };
		iContour_t neighbors;
		uint32_t pEnd = rows * columns;
		uint32_t count = 0;
		for (uint32_t i = 0; i < 8; i++)
		{
			count = pixel + LUT_nBore[i];
			while ((count < 0 || count >= pEnd) && i < 8) { count = pixel + LUT_nBore[++i]; }
			if (i >= 8) { break; }
			if (O[count] == 1) neighbors.push_back(count);
		}
		return neighbors;
	}

	// Depth first search with extension list,
	// based upon: http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-034-artificial-intelligence-fall-2010/lecture-videos/lecture-4-search-depth-first-hill-climbing-beam/
	ComplexVect_t FFT::Contour2Complex(const cv::Mat &img)
	{
		uchar *O = img.data;
		uint32_t pEnd = img.cols * img.rows;

		std::deque<std::deque<uint32_t>> sCont;
		std::deque<uint32_t> eList;
		
		//Initialize the queue
		for (uint32_t i = 0; i < pEnd; i++)
		{
			if (O[i] == 1) 
			{
				std::deque<uint32_t> tmpQ;
				tmpQ.push_back(i);
				sCont.push_back(tmpQ); 
				break;
			}
		}

		if (sCont.front().size() < 1) { throw Exception::MathException("No contour found in image!"); } // Exception handling

		uint32_t prev = -1;

		// Extend path on queue
		for (uint32_t i = sCont.front().front(); i < pEnd;)
		{
			iContour_t nBors = Neighbors(O, i, img.cols, img.rows); // find neighboring pixels
			std::deque<uint32_t> cQ = sCont.front(); //store first queue;
			sCont.erase(sCont.begin());	// erase first queue from beginning 
			if (cQ.size() > 1) { prev = cQ.size() - 2; }
			else { prev = 0; }
			// Loop through each neighbor
			for (uint32_t j = 0; j < nBors.size(); j++)
			{
				if (nBors[j] != cQ[prev]) // No backtracking
				{
					if (nBors[j] == cQ.front() && cQ.size() > 8) { i = pEnd; } // Back at first node
					if (std::find(eList.begin(), eList.end(), nBors[j]) == eList.end()) // Check if this current route is extended elsewhere
					{
						std::deque<uint32_t> nQ = cQ;
						nQ.push_back(nBors[j]); // Add the neighbor to the queue
						sCont.push_front(nQ); // add the sequence to the front of the queue
					}
				}
			}
			if (nBors.size() > 2) { eList.push_back(i); } // if there are multiple choices put current node in extension List
			if (i != pEnd) { i = sCont.front().back(); } // If it isn't the end set i to the last node of the first queue
			if (sCont.size() == 0) { throw Exception::MathException("No continuous contour found, or less then 8 pixels long!"); }
		}

		// convert the first queue to a complex normalized vector
		Complex_t cPoint;
		ComplexVect_t contour;

		//Normalize and convert the complex function
		for_each(sCont.front().begin(), sCont.front().end(), [&img, &cPoint, &contour](uint32_t &e)
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