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

	void MorphologicalFilter::Open(const Mat &mask, bool chain)
	{
		Erosion(mask, chain);
		Dilation(mask, true);
	}

	void MorphologicalFilter::Close(const Mat &mask, bool chain)
	{
		Dilation(mask, chain);
		Erosion(mask, true);
	}

	void MorphologicalFilter::Dilation(const Mat &mask, bool chain)
	{
		Filter(mask, chain, 0, 1, 1);
	}

	void MorphologicalFilter::Erosion(const Mat &mask, bool chain)
	{
		Filter(mask, chain, 1, 0, 0);
	}

	void MorphologicalFilter::Filter(const Mat &mask, bool chain, uchar startVal, uchar newVal, uchar switchVal)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);
		if (mask.cols % 2 == 0 || mask.cols < 3) { throw Exception::WrongKernelSizeException("Wrong Kernelsize columns!"); }
		if (mask.rows % 2 == 0 || mask.rows < 3) { throw Exception::WrongKernelSizeException("Wrong Kernelsize rows!"); }

		uint32_t hKsizeCol = (mask.cols / 2);
		uint32_t hKsizeRow = (mask.rows / 2);

		// make Pointers
		Mat workOrigImg(ProcessedImg.rows + mask.rows, ProcessedImg.cols + mask.cols, CV_8UC1);
		workOrigImg.setTo(0);
		if (chain)
		{
			ProcessedImg.copyTo(workOrigImg(cv::Rect(hKsizeCol, hKsizeRow, ProcessedImg.cols, ProcessedImg.rows)));
			//workOrigImg(cv::Rect(hKsizeCol, hKsizeRow, ProcessedImg.cols, ProcessedImg.rows)) = ProcessedImg.clone();
		}
		else
		{
			OriginalImg.copyTo(workOrigImg(cv::Rect(hKsizeCol, hKsizeRow, ProcessedImg.cols, ProcessedImg.rows)));
			//workOrigImg(cv::Rect(hKsizeCol, hKsizeRow, ProcessedImg.cols, ProcessedImg.rows)) = OriginalImg.clone();
		}
		uchar *O = workOrigImg.data;

		Mat workProcImg(ProcessedImg.rows + mask.rows, ProcessedImg.cols + mask.cols, CV_8UC1);
		uchar *P = workProcImg.data;

		// Init the relevant data
		uint32_t nData = OriginalImg.cols * OriginalImg.rows;
		uint32_t nWData = workProcImg.cols * workProcImg.rows;
		uint32_t nWStart = (hKsizeRow * workProcImg.cols) + hKsizeRow;
		uint32_t nWEnd = nWData - hKsizeCol - hKsizeRow * workProcImg.cols - 1;
		uchar *nRow = GetNRow(nWData, hKsizeCol, workProcImg.cols, workProcImg.rows);
		int MaskPixel = 0, OPixel = 0;

		workProcImg.setTo(0);
		if (startVal != 0) { workProcImg(cv::Rect(hKsizeCol, hKsizeRow, ProcessedImg.cols, ProcessedImg.rows)).setTo(startVal); }
		SHOW_DEBUG_IMG(workOrigImg, uchar, 255, "workOrigImg Filter!");
		SHOW_DEBUG_IMG(mask, uchar, 255, "Filter mask");

		for (uint32_t i = nWStart; i < nWEnd; i++)
		{
			// Checks if pixel isn't a border pixel and progresses to the new row
			if (nRow[i] == 1) { i += mask.cols; }
			for (uint32_t r = 0; r < mask.rows; r++)
			{
				for (uint32_t c = 0; c < mask.cols; c++)
				{
					MaskPixel = c + r * mask.cols;
					OPixel = i - hKsizeCol + c + (r - hKsizeRow) * workProcImg.cols;
					if (mask.data[MaskPixel] == 1 && O[OPixel] == switchVal)
					{
						P[i] = newVal;
						c = mask.cols;
						r = mask.rows;
					}
				}
			}
		}
		delete[] nRow;
		SHOW_DEBUG_IMG(workProcImg, uchar, 255, "workProcImg Filter!");
		ProcessedImg = workProcImg(Rect(hKsizeCol, hKsizeRow, ProcessedImg.cols, ProcessedImg.rows)).clone();
		SHOW_DEBUG_IMG(ProcessedImg, uchar, 255, "Processed Image Filter!");
	}
}