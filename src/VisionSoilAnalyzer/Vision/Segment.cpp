/*! \class  Segment
\brief Segmentation algorithms
With this class, various segmentation routines can be applied to a greyscale or black and white source image.
*/
#include "Segment.h"

namespace Vision
{
	//! Constructor of the Segmentation class
	Segment::Segment() { }

	//! Constructor of the Segmentation class
	Segment::Segment(const Mat &src)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		LabelledImg.create(OriginalImg.size(), CV_16UC1);
	}

	Segment::Segment(const Segment & rhs)
	{
		this->BlobList = rhs.BlobList;
		this->LabelledImg = rhs.LabelledImg;
		this->MaxLabel = rhs.MaxLabel;
		this->noOfFilteredBlobs = rhs.noOfFilteredBlobs;
		this->OriginalImg = rhs.OriginalImg;
		this->OriginalImgStats = rhs.OriginalImgStats;
		this->ProcessedImg = rhs.ProcessedImg;
		this->TempImg = rhs.TempImg;
		this->ThresholdLevel = rhs.ThresholdLevel;
	}

	//! De-constructor
	Segment::~Segment()
	{
	}

	Segment & Segment::operator=(Segment & rhs)
	{
		if (&rhs != this)
		{
			this->BlobList = rhs.BlobList;
			this->LabelledImg = rhs.LabelledImg;
			this->MaxLabel = rhs.MaxLabel;
			this->noOfFilteredBlobs = rhs.noOfFilteredBlobs;
			this->OriginalImg = rhs.OriginalImg;
			this->OriginalImgStats = rhs.OriginalImgStats;
			this->ProcessedImg = rhs.ProcessedImg;
			this->TempImg = rhs.TempImg;
			this->ThresholdLevel = rhs.ThresholdLevel;
		}
		return *this;
	}

	void Segment::LoadOriginalImg(const Mat & src)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		LabelledImg.create(OriginalImg.size(), CV_16UC1);
	}

	/*! Determine the threshold level by iteration, between two distribution, presumably back- and foreground. It works towards the average of the two averages and finally sets the threshold with two time the standard deviation from the mean of the set object
	\param TypeObject is an enumerator indicating if the bright or the dark pixels are the object and should be set to one
	\return The threshold level as an uint8_t	*/
	uint8_t Segment::GetThresholdLevel(TypeOfObjects TypeObject)
	{
		// Exception handling
		EMPTY_CHECK(OriginalImg);
		CV_Assert(OriginalImg.depth() != sizeof(uchar));

		// Calculate the statistics of the whole picture
		ucharStat_t OriginalImgStats(OriginalImg.data, OriginalImg.rows, OriginalImg.cols);

		// Sets the initial threshold with the mean of the total picture
		pair<uchar, uchar> T;
		T.first = (uchar)(OriginalImgStats.Mean + 0.5);
		T.second = 0;

		uchar Rstd = 0;
		uchar Lstd = 0;
		uchar Rmean = 0;
		uchar Lmean = 0;

		// Iterate till optimum Threshold is found between back- & foreground
		while (T.first != T.second)
		{
			// Gets an array of the left part of the histogram
			uint32_t i = T.first;
			uint32_t *Left = new uint32_t[i]{};
			while (i-- > 0) { Left[i] = OriginalImgStats.bins[i]; }

			// Gets an array of the right part of the histogram
			uint32_t rightEnd = 256 - T.first;
			uint32_t *Right = new uint32_t[rightEnd]{};
			i = rightEnd;
			while (i-- > 0) { Right[i] = OriginalImgStats.bins[i + T.first]; }

			// Calculate the statistics of both histograms,
			// taking into account the current threshold
			ucharStat_t sLeft(Left, 0, T.first);
			ucharStat_t sRight(Right, T.first, 256);

			// Calculate the new threshold the mean of the means
			T.second = T.first;
			T.first = (uchar)(((sLeft.Mean + sRight.Mean) / 2) + 0.5);

			Rmean = (uchar)(sRight.Mean + 0.5);
			Lmean = (uchar)(sLeft.Mean + 0.5);
			Rstd = (uchar)(sRight.Std + 0.5);
			Lstd = (uchar)(sLeft.Std + 0.5);
			delete[] Left;
			delete[] Right;
		}

		// Assumes the pixel value of the sought object lies between 2 sigma
		int val = 0;
		switch (TypeObject)
		{
		case Bright:
            val = Rmean - (sigma * Rstd) - thresholdOffset;
			if (val < 0) { val = 0; }
			else if (val > 255) { val = 255; }
			T.first = (uchar)val;
			break;
		case Dark:
            val = Lmean + (sigma * Lstd) + thresholdOffset;
			if (val < 0) { val = 0; }
			else if (val > 255) { val = 255; }
			T.first = (uchar)val;
			break;
		}

		return T.first;
	}

	/*! Convert a greyscale image to a BW using an automatic Threshold
	\param src is the source image as a cv::Mat
	\param dst destination image as a cv::Mat
	\param TypeObject is an enumerator indicating if the bright or the dark pixels are the object and should be set to one	*/
	void Segment::ConvertToBW(const Mat &src, Mat &dst, TypeOfObjects Typeobjects)
	{
		OriginalImg = src;
		ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		LabelledImg.create(OriginalImg.size(), CV_16UC1);
		ConvertToBW(Typeobjects);
		dst = ProcessedImg;
	}

	/*! Convert a greyscale image to a BW using an automatic Threshold
	\param TypeObject is an enumerator indicating if the bright or the dark pixels are the object and should be set to one 	*/
	void Segment::ConvertToBW(TypeOfObjects Typeobjects)
	{
		// Determine the threshold
		uchar T = GetThresholdLevel(Typeobjects);

		// Threshold the picture
		Threshold(T, Typeobjects);
	}

	/*! Convert a greyscale image to a BW
	\param t uchar set the value which is the tiping point
	\param TypeObject is an enumerator indicating if the bright or the dark pixels are the object and should be set to one 	*/
	void Segment::Threshold(uchar t, TypeOfObjects Typeobjects)
	{
		// Exception handling
		EMPTY_CHECK(OriginalImg);
		CV_Assert(OriginalImg.depth() != sizeof(uchar) ||
			OriginalImg.depth() != sizeof(uint16_t));

		// Create LUT
		uchar LUT_newValue[256]{ 0 };
		if (Typeobjects == Bright)
		{
			for (uint32_t i = t; i < 256; i++)
			{
				LUT_newValue[i] = 1;
			}
		}
		else
		{
			for (uint32_t i = 0; i <= t; i++)
			{
				LUT_newValue[i] = 1;
			}
		}

		// Create the pointers to the data
		uchar *P = ProcessedImg.data;
		uchar *O = OriginalImg.data;

		// Fills the ProcessedImg with either a 0 or 1
		for (uint32_t i = 0; i < OriginalImg.cols * OriginalImg.rows; i++)
		{
			P[i] = LUT_newValue[O[i]];
		}
	}

	/*! Set all the border pixels to a set value
	\param *P uchar pointer to the Mat.data
	\param setValue uchar the value which is written to the border pixels
	*/
	void Segment::SetBorder(uchar *P, uchar setValue)
	{
		// Exception handling
		EMPTY_CHECK(OriginalImg);
		CV_Assert(OriginalImg.depth() != sizeof(uchar) ||
			OriginalImg.depth() != sizeof(uint16_t));

		uint32_t nData = OriginalImg.cols * OriginalImg.rows;

		// Set borderPixels to 2
		uint32_t i = 0;
		uint32_t pEnd = OriginalImg.cols + 1;

		// Set the top row to value 2
		while (i < pEnd) { P[i++] = setValue; }

		// Set the bottom row to value 2
		i = nData + 1;
		pEnd = nData - OriginalImg.cols;
		while (i-- > pEnd) { P[i] = setValue; }

		//Sets the first and the last Column to 2
		i = 1;
		pEnd = OriginalImg.rows;
		while (i < pEnd)
		{
			P[(i * OriginalImg.cols) - 1] = setValue;
			P[(i++ * OriginalImg.cols)] = setValue;
		}
	}

	/*! Remove the blobs that are connected to the border
	\param conn set the pixel connection eight or four
	\param chain use the results from the previous operation default value = false;
	*/
	void Segment::RemoveBorderBlobs(uint32_t border, bool chain)
	{
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);
		// make Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		if (chain) { ProcessedImg = TempImg.clone(); } 
		else { ProcessedImg = OriginalImg.clone(); } 		

		SHOW_DEBUG_IMG(OriginalImg, uchar, 255, "Original Image RemoverBorderBlobs!", true);
		SHOW_DEBUG_IMG(TempImg, uchar, 255, "Temp Image RemoverBorderBlobs!", true);

		uchar *P = ProcessedImg.data;
		uint32_t cols = ProcessedImg.cols;
		uint32_t rows = ProcessedImg.rows;

        try
        {
            for (uint32_t i = 0; i < border; i++)
            {
                for (uint32_t j = 0; j < cols; j++)
                {
                    if (O[(i * cols) + j] == 1 && P[(i * cols) + j] != 2) { cv::floodFill(ProcessedImg, cv::Point(j, i), (uchar)2); }
                }
            }

            for (uint32_t i = rows - border - 1; i < rows; i++)
            {
                for (uint32_t j = 0; j < cols; j++)
                {
                    if (O[(i * cols) + j] == 1 && P[(i * cols) + j] != 2) { cv::floodFill(ProcessedImg, cv::Point(j, i), (uchar)2); }
                }
            }

            for (uint32_t i = border; i < rows - border; i++)
            {
                for (uint32_t j = 0; j < border; j++)
                {
                    if (O[(i * cols) + j] == 1 && P[(i * cols) + j] != 2) {	cv::floodFill(ProcessedImg, cv::Point(j, i), (uchar)2); }
                    if (O[(i * cols) + (cols - j - 1)] == 1 && P[(i * cols) + (cols - j - 1)] != 2)	{ cv::floodFill(ProcessedImg, cv::Point(cols - j - 1, i), (uchar)2);	}
                }
            }
        }
        catch (cv::Exception &e) { }
		SHOW_DEBUG_IMG(ProcessedImg, uchar, 255, "Processed Image RemoverBorderBlobs before LUT!", true);

		// Change values 2 -> 0
		uchar LUT_newValue[3]{ 0, 1, 0 };
		P = ProcessedImg.data;
		uint32_t nData = rows * cols;
		for (uint32_t i = 0; i < nData; i++) { P[i] = LUT_newValue[P[i]];}

		SHOW_DEBUG_IMG(ProcessedImg, uchar, 255, "Processed Image RemoverBorderBlobs!", true);
	}

	/*! Label all the individual blobs in a BW source image. The result are written to the labelledImg as an ushort
	\param conn set the pixel connection eight or four
	\param chain use the results from the previous operation default value = false;
	\param minBlobArea minimum area when an artifact is considered a blob
	*/
	void Segment::LabelBlobs(bool chain, uint16_t minBlobArea, Connected conn)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);

		// make the Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uint16_t *P = (uint16_t *)LabelledImg.data;

		uint32_t nCols = OriginalImg.cols;
		uint32_t nRows = OriginalImg.rows;
		uint32_t nData = nCols * nRows;
		uint32_t i = OriginalImg.cols + 2;
		uint32_t j = 4;
		uint32_t pEnd = nData - OriginalImg.cols;

		uint16_t currentlbl = 0;
		vector<vector<uint16_t>> connectedLabels;
		vector<uint16_t> zeroVector;
		zeroVector.push_back(currentlbl);
		connectedLabels.push_back(zeroVector);

		/* Four connected strategy... Although it's more code. If I place this check here it's less machine instructions compared to doing it's done
		inside the loop */
		if (conn == Four)
		{
			// Loop through the picture
			while (i < pEnd)
			{
				// If current value = zero processed value = zero
				if (O[i] == 0) { P[i] = 0; }

				// If current value = 1 check North and West and act accordingly
				else if (O[i] == 1)
				{
					uint16_t North = P[i - nCols];
					uint16_t West = P[i - 1];
					uint16_t minVal;
					uint16_t maxVal;

					// If North and West are both zero assume this is a new blob
					if (North == 0 && West == 0)
					{
						P[i] = ++currentlbl;
						vector<uint16_t> cVector;
						cVector.push_back(currentlbl);
						connectedLabels.push_back(cVector);
					}

					//Sets the processed value to the smallest non - zero value of North and West and update the connectedLabels
					else
					{
						maxVal = SoilMath::Max(North, West);
						if (North == 0 || West == 0) { minVal = maxVal; }
						else { minVal = SoilMath::Min(North, West); }

						P[i] = minVal;

						/* If North and West belong to two different connected components set the current processed value to the lowest value and remember that the highest value should be the lowest value */
						if (North != 0 && West != 0 && maxVal != minVal) { connectedLabels[maxVal].push_back(minVal); }
					}
				}

				// If there is a value greater then 1 or smaller then 1 throw error
				else { throw Exception::PixelValueOutOfBoundException(); }
				i++;
			}
		}

		// If eight connected is required
		else
		{
			// Loop through the picture
			while (i < pEnd)
			{
				// If current value = zero processed value = zero
				if (O[i] == 0) { P[i] = 0; }

				// If current value = 1 check North and West and act accordingly
				else if (O[i] == 1)
				{
					uint16_t *nPixels = new uint16_t[4];
					nPixels[0] = P[i - 1];
					nPixels[1] = P[i - nCols - 1];
					nPixels[2] = P[i - nCols];
					nPixels[3] = P[i - nCols + 1];
					uint16_t minVal;
					uint16_t maxVal;

					// Sort the neighbors for easier checking
					SoilMath::Sort::QuickSort<uint16_t>(nPixels, 4);

					//If North NorthWest, NorthEast and West are all zero assume this is a new blob
					if (nPixels[3] == 0)
					{
						P[i] = ++currentlbl;
						vector<uint16_t> cVector;
						cVector.push_back(currentlbl);
						connectedLabels.push_back(cVector);
					}

					// Sets the processed value to the smallest non-zero value of North and West and update the connectedLabels
					else
					{
						maxVal = nPixels[3];

						// If there is only 1 neighbor of importance
						if (nPixels[2] == 0) { minVal = nPixels[3]; }
						else if (nPixels[1] == 0) { minVal = nPixels[2]; }
						else if (nPixels[0] == 0) { minVal = nPixels[2]; }
						else { minVal = nPixels[0]; }

						P[i] = minVal;

						/* If North NorthWest, NorthEast and West belong to different connected components set the current processed value to the lowest value and remember that the other value should be the lowest value*/
						if (nPixels[0] != nPixels[3])
						{
							j = 4;
							while (j-- > 0)	{ if (nPixels[j] != 0 && nPixels[j] > minVal) { connectedLabels[nPixels[j]].push_back(minVal); } }
						}
					}
					delete[] nPixels;
				}
				// If there is a value greater then 1 or smaller then 1 throw error
				else { throw Exception::PixelValueOutOfBoundException(); }
				i++;
			}
		}

		// Sort all the vectors so the min value is easily obtained
		i = currentlbl + 1;
		while (i-- > 0) { std::sort(connectedLabels[i].begin(), connectedLabels[i].end()); }

		// Create the LUT
		uint16_t *LUT_newVal = new uint16_t[currentlbl + 1];
		i = currentlbl + 1;
		while (i-- > 0)
		{
			// If the value has a chain, crawl in that rabbit hole till the
			// lowest value is found and sets the LUT
			if (connectedLabels[i].size() > 1)
			{
				uint16_t pChainVal = connectedLabels[connectedLabels[i][0]][0];
				uint16_t cChainVal = connectedLabels[i][0];
				uint16_t lowestVal = pChainVal;

				// How far goes the rabbit hole
				while (pChainVal != cChainVal)
				{
					cChainVal = connectedLabels[pChainVal][0];
					pChainVal = connectedLabels[cChainVal][0];
					lowestVal = pChainVal;
				}

				// Write the lowest label to the Look-Up-Table
				LUT_newVal[i] = lowestVal;
			}
			else 
			{
				LUT_newVal[i] = i; 
			}	// End of the line so use the same label
		}

		// Make the labels consecutive numbers
		uint16_t *tempLUT = new uint16_t[currentlbl + 1];
		makeConsecutive(currentlbl, tempLUT, LUT_newVal);

		// Get the maximum value
		i = 0;
		while (i <= currentlbl)
		{
			if (LUT_newVal[i] > MaxLabel) { MaxLabel = LUT_newVal[i]; }
			i++;
		}

		// Second loop through each pixel to replace them with corresponding intermediate value
		i = 0;
		while (i < pEnd)
		{
			P[i] = LUT_newVal[P[i]];
			i++;
		}

		// Create a LUT_filter for each value that is smaller then minBlobArea
		uint16Stat_t ProcImgStats(P, nCols, nRows, MaxLabel + 1, 0, MaxLabel);
		uint16_t *LUT_finalVal = new uint16_t[MaxLabel + 1]{};
		uint16_t count = 0;
		i = 0;
		while (i <= MaxLabel)
		{
			if (ProcImgStats.bins[i] > minBlobArea) 
			{
				LUT_finalVal[i] = count++; 
			}
			i++;
		}

		noOfFilteredBlobs = MaxLabel - count - 1;
		MaxLabel = count - 1;

		// third loop through each pixel to replace them with corresponding final value
		i = 0;
		while (i < pEnd)
		{
			P[i] = LUT_finalVal[P[i]];
			i++;
		}

		delete[] tempLUT;
		delete[] LUT_newVal;
		delete[] LUT_finalVal;
	}

	/*! Create a BW image with only edges from a BW image
	\param src source image as a const cv::Mat
	\param dst destination image as a cv::Mat
	\param conn set the pixel connection eight or four
	\param chain use the results from the previous operation default value = false;
	*/
	void Segment::GetEdges(const Mat &src, Mat &dst, bool chain, Connected conn)
	{
		OriginalImg = src;
		GetEdges(chain, conn);
		dst = ProcessedImg;
	}

	/*! Create a BW image with only edges from a BW image
	\param conn set the pixel connection eight or four
	\param chain use the results from the previous operation default value = false;
	*/
	void Segment::GetEdges(bool chain, Connected conn)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);

		// make Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uchar *P = ProcessedImg.data;

		uint32_t nCols = OriginalImg.cols;
		uint32_t nRows = OriginalImg.rows;
		uint32_t nData = nCols * nRows;
		uint32_t pEnd = nData + 1;
		uint32_t i = 0;

		//Loop through the image and set each pixel which has a zero neighbor set it to two.
		if (conn == Four)
		{
			// Loop through the picture
			while (i < pEnd)
			{
				// If current value = zero processed value = zero
				if (O[i] == 0) { P[i] = 0; }
				// If current value = 1 check North West, South and East and act accordingly
				else if (O[i] == 1)
				{
					uchar *nPixels = new uchar[4];
					nPixels[0] = O[i - 1];
					nPixels[1] = O[i - nCols];
					nPixels[2] = O[i + 1];
					nPixels[3] = O[i + nCols];

					// Sort the neighbors for easier checking
					SoilMath::Sort::QuickSort<uchar>(nPixels, 4);
					if (nPixels[0] == 0) { P[i] = 1; }
					else { P[i] = 0; }
				}
				else { throw Exception::PixelValueOutOfBoundException(); }
				i++;
			}
		}
		else
		{
			// Loop through the picture
			while (i < pEnd)
			{
				// If current value = zero processed value = zero
				if (O[i] == 0) { P[i] = 0; }
				// If current value = 1 check North West, South and East and act accordingly
				else if (O[i] == 1)
				{
					uchar *nPixels = new uchar[8];
					nPixels[0] = O[i - 1];
					nPixels[1] = O[i - nCols];
					nPixels[2] = O[i - nCols - 1];
					nPixels[3] = O[i - nCols + 1];
					nPixels[4] = O[i + 1];
					nPixels[5] = O[i + nCols + 1];
					nPixels[6] = O[i + nCols];
					nPixels[7] = O[i + nCols - 1];

					// Sort the neighbors for easier checking
					SoilMath::Sort::QuickSort<uchar>(nPixels, 8);

					if (nPixels[0] == 0) { P[i] = 1; }
					else { P[i] = 0; }
				}
				else { throw Exception::PixelValueOutOfBoundException(); }
				i++;
			}
		}
	}

	void Segment::GetEdgesEroding(bool chain)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);

		// make Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		uchar *P = ProcessedImg.data;

		uint32_t nCols = OriginalImg.cols;
		uint32_t nRows = OriginalImg.rows;
		uint32_t nData = nCols * nRows;

		// Setup the erosion
		MorphologicalFilter eroder;
		if (chain) { eroder.OriginalImg = TempImg; }
		else { eroder.OriginalImg = OriginalImg; }
		// Setup the processed image of the eroder
		eroder.ProcessedImg.create(OriginalImg.size(), CV_8UC1);
		eroder.ProcessedImg.setTo(0);
		// Setup the mask
		Mat mask(3, 3, CV_8UC1, 1);
		// Erode the image
		eroder.Erosion(mask, false);

		// Loop through the image and set the not eroded pixels to zero
		for (uint32_t i = 0; i < nData; i++)
		{
			if (O[i] != eroder.ProcessedImg.data[i]) { P[i] = 1; }
			else { P[i] = 0; }
		}

		//ProcessedImg = OriginalImg.clone() - eroder.ProcessedImg.clone();

		SHOW_DEBUG_IMG(eroder.ProcessedImg, uchar, 255, "Eroded img Processed Image!", true);
		SHOW_DEBUG_IMG(ProcessedImg, uchar, 255, "GetEdgesEroding Processed Image!", true);
	}

	/*! Create a BlobList subtracting each individual blob out of a Labelled image. If the labelled image is empty build a new one with a BW image.
	\param conn set the pixel connection eight or four
	\param chain use the results from the previous operation default value = false;
	*/
	void Segment::GetBlobList(bool chain, Connected conn)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);

		// If there isn't a labelledImg make one
		if (MaxLabel < 1) { LabelBlobs(chain, 5, conn); }

		// Make an empty BlobList
		uint32_t nCols = OriginalImg.cols;
		uint32_t nRows = OriginalImg.rows;
		uint32_t nData = nCols * nRows;
		RectList_t rectList;

		// Calculate Stats the statistics
		uint16Stat_t LabelStats((uint16_t *)LabelledImg.data, LabelledImg.cols, LabelledImg.rows, MaxLabel + 1, 0, MaxLabel);

		BlobList.reserve(MaxLabel + 1);
		rectList.reserve(MaxLabel + 1);

		BlobList.push_back(Blob_t(0, 0));
		rectList.push_back(Rect_t(0, 0, 0, 0));

		for (uint32_t i = 1; i <= MaxLabel; i++)
		{
			BlobList.push_back(Blob_t(i, LabelStats.bins[i]));
			rectList.push_back(Rect_t(nCols, nRows, 0, 0));
		}

		// make Pointers
		uint16_t *L = (uint16_t *)LabelledImg.data;

		uint32_t currentX, currentY;
		uint16_t leftX, leftY, rightX, rightY;
		//Loop through the labeled image and extract the Blobs
		for (uint32_t i = 0; i < nData; i++)
		{
			if (L[i] != 0)
			{
				/* Determine the current x and y value of the current blob and
				checks if it is min/max */
				currentX = i / nCols;
				currentY = i % nCols;

				// Min value
				if (currentX < rectList[L[i]].leftX) { rectList[L[i]].leftX = currentX; }
				if (currentY < rectList[L[i]].leftY) { rectList[L[i]].leftY = currentY; }

				// Max value
				if (currentX > rectList[L[i]].rightX) { rectList[L[i]].rightX = currentX; }
				if (currentY > rectList[L[i]].rightY) { rectList[L[i]].rightY = currentY; }
			}
		}

		// Loop through the BlobList and finalize it
		uint8_t *LUT_filter = new uint8_t[MaxLabel + 1]{};
		for (uint32_t i = 1; i <= MaxLabel; i++)
		{
			LUT_filter[i] = 1;
			BlobList[i].ROI.y = rectList[i].leftX;
			BlobList[i].ROI.x = rectList[i].leftY;
			BlobList[i].ROI.height = rectList[i].rightX - rectList[i].leftX;
			BlobList[i].ROI.width = rectList[i].rightY - rectList[i].leftY;
			if (BlobList[i].ROI.height < 0)
			{
				BlobList[i].ROI.height -= BlobList[i].ROI.height;
				BlobList[i].ROI.y = rectList[i].rightX;
			}
			else if (BlobList[i].ROI.height == 0) { BlobList[i].ROI.height++;	}
			if (BlobList[i].ROI.width < 0)
			{
				BlobList[i].ROI.width -= BlobList[i].ROI.width;
				BlobList[i].ROI.x = rectList[i].rightY;
			}
			else if (BlobList[i].ROI.width == 0) { BlobList[i].ROI.width++; }
			BlobList[i].Img = CopyMat<uint8_t, uint16_t>(LabelledImg(BlobList[i].ROI).clone(), LUT_filter, CV_8UC1);
			LUT_filter[i] = 0;
		}
		delete[] LUT_filter;

		//Remove background blob
		BlobList.erase(BlobList.begin());
	}

	void Segment::makeConsecutive(uint16_t LastLabelUsed, uint16_t * tempLUT, uint16_t *LUT_newVal)
	{
		uint32_t i = LastLabelUsed + 1;
		while (i-- > 0) { tempLUT[i] = LUT_newVal[i]; }
		SoilMath::Sort::QuickSort<uint16_t>(tempLUT, LastLabelUsed + 1);
		std::vector<uint16_t> v(LUT_newVal, LUT_newVal + (LastLabelUsed + 1));

		uint16_t count = 0;
		i = 1;
		while (i <= LastLabelUsed)
		{
			if (tempLUT[i] != tempLUT[i - 1]) { std::replace(v.begin(), v.end(), tempLUT[i], ++count); }
			i++;
		}

		LUT_newVal = &v[0];
	}

	void Segment::FillHoles(bool chain)
	{
		// Exception handling
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);

		// make Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		if (chain) { ProcessedImg = TempImg.clone(); }
		else { ProcessedImg = OriginalImg.clone(); }

		uchar *P = ProcessedImg.data;

		// Determine the starting point of the floodfill
		int itt = -1;
		while (P[++itt] != 0);
		uint16_t row = static_cast<uint16_t>(itt / OriginalImg.rows);
		uint16_t col = static_cast<uint16_t>(itt % OriginalImg.rows);

		// Fill the outside
		//FloodFill(O, P, row, col, 2, 0);

        try
        {
            cv::floodFill(ProcessedImg, cv::Point(col, row), cv::Scalar(2));
        }
        catch (cv::Exception &e) { }

		// Set the unreached areas to 1 and the outside to 0;
		uchar LUT_newVal[3] = { 1, 1, 0 };
		uint32_t nData = OriginalImg.rows * OriginalImg.cols;
		uint32_t i = 0;
		while (i <= nData)
		{
			P[i] = LUT_newVal[P[i]];
			i++;
		}
	}

	void Segment::FloodFill(uchar *O, uchar *P, uint16_t row, uint16_t col, uchar fillValue, uchar OldValue)
	{
		if (row < 0 || row > OriginalImg.rows) { return; }
		if (col < 0 || col > OriginalImg.cols) { return; }
		if (P[col + row * OriginalImg.rows] == OldValue)
		{
			P[col + row * OriginalImg.rows] = fillValue;
			FloodFill(O, P, row + 1, col, fillValue, OldValue);
			FloodFill(O, P, row, col + 1, fillValue, OldValue);
			FloodFill(O, P, row - 1, col, fillValue, OldValue);
			FloodFill(O, P, row, col - 1, fillValue, OldValue);
		}
	}
}
