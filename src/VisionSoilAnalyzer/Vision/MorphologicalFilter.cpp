#include "MorphologicalFilter.h"

namespace Vision
{
	MorphologicalFilter::MorphologicalFilter() 	{	}

	MorphologicalFilter::MorphologicalFilter(const Mat &src)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
	}

	MorphologicalFilter::~MorphologicalFilter() {}

	void MorphologicalFilter::Erosion(const Mat &src, Mat &dst, const Mat &mask)
	{
		OriginalImg = src;
		ProcessedImg.create(src.size(), CV_8UC1);
		Erosion(mask);
		dst = ProcessedImg;
	}

	void MorphologicalFilter::Erosion(const Mat &mask, bool chain)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);
		if (mask.cols % 2 == 0 || mask.cols < 3) { throw Exception::WrongKernelSizeException("Wrong Kernelsize columns!"); }
		if (mask.rows % 2 == 0 || mask.rows < 3) { throw Exception::WrongKernelSizeException("Wrong Kernelsize rows!"); }

		// make Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uchar *P = ProcessedImg.data;

		// Init the relevant data
		uint32_t nCols = OriginalImg.cols;
		uint32_t nRows = OriginalImg.rows;
		uint32_t nData = nCols * nRows;
		uint32_t hKsizeCol = (mask.cols / 2);
		uint32_t hKszieRow = (mask.rows / 2);
		uint32_t pEnd = nData - hKsizeCol - (hKszieRow * nCols);
		uint32_t i = hKsizeCol + (hKszieRow * nCols);
		uint32_t j = 0;
		int currentKRow = -hKszieRow;
		int currentKcol = -hKsizeCol;
		bool isEroded = false;
		uint32_t nKData = mask.cols * mask.rows;
		uchar *nRow = GetNRow(nData, hKsizeCol, nCols, OriginalImg.rows);
		uchar *nKRow = GetNRow(nKData, 0, mask.cols, mask.rows);

		while (i < pEnd)
		{
			// Checks if pixel isn't a border pixel and progresses to the new row
			if (nRow[i] == 1) { i += mask.cols; }

			/*Loops through the mask to check if pixel is kept or deleted, check the zero's first because they're a multitude compared with the ones and use
			a else if loop so the other conditions are only checked  when it's not a zero*/
			if (O[i] == 0) { P[i] = 0; }
			else if (O[i])
			{
				//Checks for each mask pixel that is set to one if the corresponding pixel is one
				j = 0;
				currentKRow = -hKszieRow;
				currentKcol = -hKsizeCol - 1;
				isEroded = false;
				while (j < nKData && !isEroded)
				{
					// Checks if pixel isn't a border pixel from the kernel
					currentKcol += 1;
					if (nKRow[j] == 1) 
					{
						currentKRow++; 
						currentKcol = -hKsizeCol;
					}
					
					// If one of the pixels is different then the corresponding mask pixel set the processed pixel to zero and exit loop
					if (mask.data[j] != O[i + (currentKRow * nCols) + currentKcol]) 
					{
						P[i] = 0;
						isEroded = true;
					}
					j++;
				}
				// If all pixel in the ROI image correspond with the mask pixels set the processed pixel to one
				if (!isEroded) { P[i] = 1; }
			}
			else { throw Exception::PixelValueOutOfBoundException(); } // Unexpected value throw exception
			i++;
		}
	}
}