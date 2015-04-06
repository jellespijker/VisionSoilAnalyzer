#include "MorphologicalFilter.h"

namespace Vision
{
	MorphologicalFilter::MorphologicalFilter() {	}

	MorphologicalFilter::MorphologicalFilter(const Mat &src)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
	}

	MorphologicalFilter::MorphologicalFilter(const MorphologicalFilter & rhs)
	{
		this->OriginalImg = rhs.OriginalImg;
		this->ProcessedImg = rhs.ProcessedImg;
		this->TempImg = rhs.ProcessedImg;
	}

	MorphologicalFilter::~MorphologicalFilter() {}

		MorphologicalFilter & MorphologicalFilter::operator=(MorphologicalFilter & rhs)
		{
			if (&rhs != this)
			{
				this->OriginalImg = rhs.OriginalImg;
				this->ProcessedImg = rhs.ProcessedImg;
				this->TempImg = rhs.TempImg;
			}
			return *this;
		}

	void MorphologicalFilter::Erosion(const Mat &src, Mat &dst, const Mat &mask)
	{
		OriginalImg = src;
		ProcessedImg.create(src.size(), CV_8UC1);
		Erosion(mask);
		dst = ProcessedImg;
	}

	void MorphologicalFilter::Dilation(const Mat &mask, bool chain)
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
		//uint32_t j = 0;
		int currentKRow = -hKszieRow;
		int currentKcol = -hKsizeCol;
		uint32_t nKData = mask.cols * mask.rows;
		uchar *nRow = GetNRow(nData, hKsizeCol, nCols, nRows);
		uint32_t cc = 0, rr = 0, c = 0, r = 0;
		uint32_t MaskPixel = 0, OPixel = 0;
		bool nextPixel = true;
		bool nextCol = true;

		SHOW_DEBUG_IMG(OriginalImg, uchar, 255, "Original Image Dilate!");
		SHOW_DEBUG_IMG(TempImg, uchar, 255, "Temp Image Dilate!");
		SHOW_DEBUG_IMG(mask, uchar, 255, "Dilate mask");

		for (uint32_t i = hKsizeCol + (hKszieRow * nCols); i < pEnd; i++)
		{
			// Checks if pixel isn't a border pixel and progresses to the new row
			if (nRow[i] == 1) { i += mask.cols; }

			/*Loops through the mask to check if pixel is kept or deleted, check the zero's first because they're a multitude compared with the ones and use
			a else if loop so the other conditions are only checked  when it's not a zero*/
			if (O[i] == 0) { P[i] = 0; }
			else if (O[i] == 1)
			{
				P[i] = 1;
				rr = 0;
				nextPixel = true;
				while (nextPixel)
				{
					cc = 0;
					nextCol = true;
					while (nextCol)
					{
						MaskPixel = cc + rr * mask.cols;
						OPixel = i - hKsizeCol + cc + (rr - hKszieRow) * ProcessedImg.cols;
						if (mask.data[MaskPixel] == 1 && O[OPixel] == 1)
						{
							r = 0;
							while (r < mask.rows)
							{
								c = 0;
								while (c < mask.cols)
								{
									MaskPixel = c + r * mask.cols;
									if (mask.data[MaskPixel] == 1)
									{
										OPixel = i - hKsizeCol + c + r * ProcessedImg.cols;
										P[OPixel] = 1;
									}
									c++;
								}
								r++;
							}
							nextPixel = false;
							nextCol = false;
						}
						cc++;
						if (cc >= mask.rows) { nextCol = false; }
					}
					rr++;
					if (rr >= mask.cols) { nextPixel = false; }
				}
			}
			else { throw Exception::PixelValueOutOfBoundException(); } // Unexpected value throw exception
		}

		delete[] nRow;

		SHOW_DEBUG_IMG(ProcessedImg, uchar, 255, "Processed Image Dilate!");
		Mat tijdelijk = ProcessedImg - OriginalImg;
		SHOW_DEBUG_IMG(tijdelijk, uchar, 255, "Difference");
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
		//uint32_t j = 0;
		int currentKRow = -hKszieRow;
		int currentKcol = -hKsizeCol;
		uint32_t nKData = mask.cols * mask.rows;
		uchar *nRow = GetNRow(nData, hKsizeCol, nCols, nRows);
		uint32_t cc = 0, rr = 0;
		uint32_t MaskPixel = 0, OPixel = 0;
		bool nextPixel = true;
		bool nextCol = true;
		
		SHOW_DEBUG_IMG(OriginalImg, uchar, 255, "Original Image Erode!");
		SHOW_DEBUG_IMG(TempImg, uchar, 255, "Temp Image Erode!");
		SHOW_DEBUG_IMG(mask, uchar, 255, "Erode mask");

		for (uint32_t i = hKsizeCol + (hKszieRow * nCols); i < pEnd; i++)
		{
			// Checks if pixel isn't a border pixel and progresses to the new row
			if (nRow[i] == 1) { i += mask.cols; }

			/*Loops through the mask to check if pixel is kept or deleted, check the zero's first because they're a multitude compared with the ones and use
			a else if loop so the other conditions are only checked  when it's not a zero*/
			if (O[i] == 0) { P[i] = 0; }
			else if (O[i] == 1)
			{
				P[i] = 1;
				rr = 0;
				nextPixel = true;
				while (nextPixel)
				{
					cc = 0;
					nextCol = true;
					while (nextCol)
					{
						MaskPixel = cc + rr * mask.cols;
						OPixel = i - hKsizeCol + cc + (rr - hKszieRow) * ProcessedImg.cols;
						if (mask.data[MaskPixel] == 1 && O[OPixel] == 0)
						{
							P[i] = 0;
							nextPixel = false;
							nextCol = false;
						}
						cc++;
						if (cc >= mask.rows) { nextCol = false; }
					}
					rr++;
					if (rr >= mask.cols) { nextPixel = false; }
				}
			}
			else { throw Exception::PixelValueOutOfBoundException(); } // Unexpected value throw exception
		}

		delete[] nRow;

		SHOW_DEBUG_IMG(ProcessedImg, uchar, 255, "Processed Image Erode!");
	}
}