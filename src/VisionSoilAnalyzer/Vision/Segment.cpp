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

	//! De-constructor
	Segment::~Segment()
	{
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
		}

		// Assumes the pixel value of the sought object lies between 2 sigma
		switch (TypeObject)
		{
		case Bright:
			T.first = Rmean - (3 * Rstd);
			break;
		case Dark:
			T.first = Lmean + (3 * Lstd);
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

		uint32_t i = 0;

		// Create LUT
		uchar LUT_newValue[256]{ 0 };
		if (Typeobjects == Bright)
		{
			i = 256;
			while (i-- > t) { LUT_newValue[i] = 1; }
		}
		else
		{
			i = t + 1;
			while (i-- > 0) { LUT_newValue[i] = 1; }
		}

		// Create the pointers to the data
		uchar *P = ProcessedImg.data;
		uchar *O = OriginalImg.data;

		// Fills the ProcessedImg with either a 0 or 1
		i = OriginalImg.cols * OriginalImg.rows + 1;
		while (i-- > 0) { *P++ = LUT_newValue[*O++]; }
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
		border + 1;
		CV_Assert(OriginalImg.depth() != sizeof(uchar));
		EMPTY_CHECK(OriginalImg);
		// make Pointers
		uchar *O;
		CHAIN_PROCESS(chain, O, uchar);
		ProcessedImg = OriginalImg.clone();
		uchar *P = ProcessedImg.data;
		uint32_t cols = ProcessedImg.cols;
		uint32_t rows = ProcessedImg.rows;
		for (uint32_t i = 0; i < border; i++)
		{
			for (uint32_t j = 0; j < cols; j++)
			{
				if (O[(i * cols) + j] == 1 && P[(i * cols) + j] != 2) { cv::floodFill(ProcessedImg, cv::Point(j, i), 2); }
			}
		}

		for (uint32_t i = rows - border - 1; i < rows; i++)
		{
			for (uint32_t j = 0; j < cols; j++)
			{
				if (O[(i * cols) + j] == 1 && P[(i * cols) + j] != 2) { cv::floodFill(ProcessedImg, cv::Point(j, i), 2); }
			}
		}

		for (uint32_t i = border; i < rows - border; i++)
		{
			for (uint32_t j = 0; j < border; j++)
			{
				if (O[(i * cols) + j] == 1 && P[(i * cols) + j] != 2) {	cv::floodFill(ProcessedImg, cv::Point(j, i), 2); }
				if (O[(i * cols) + (cols - j - 1)] == 1 && P[(i * cols) + (cols - j - 1)] != 2)	{ cv::floodFill(ProcessedImg, cv::Point(cols - j - 1, i), 2);	}
			}
		}

		// Change values 2 -> 0
		uchar LUT_newValue[3]{ 0, 1, 0 };
		P = ProcessedImg.data;
		uint32_t nData = rows * cols;
		for (uint32_t i = 0; i < nData; i++) { P[i] = LUT_newValue[P[i]];}
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
				else if (O[i] = 1)
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
				else if (O[i] = 1)
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
			else { LUT_newVal[i] = i; }	// End of the line so use the same label
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
		SoilMath::Stats<uint16_t, uint32_t, uint64_t> ProcImgStats(P, nCols, nRows, MaxLabel, 0, MaxLabel);
		LUT_newVal = new uint16_t[MaxLabel + 1]{};
		uint16_t count = 0;
		i = 0;
		while (i <= MaxLabel)
		{
			if (ProcImgStats.bins[i] > minBlobArea) 
			{
				LUT_newVal[i] = count++; 
			}
			i++;
		}

		noOfFilteredBlobs = MaxLabel - count - 1;
		MaxLabel = count - 1;

		// third loop through each pixel to replace them with corresponding final value
		i = 0;
		while (i < pEnd)
		{
			P[i] = LUT_newVal[P[i]];
			i++;
		}
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
		if (conn = Four)
		{
			// Loop through the picture
			while (i < pEnd)
			{
				// If current value = zero processed value = zero
				if (O[i] == 0) { P[i] = 0; }
				// If current value = 1 check North West, South and East and act accordingly
				else if (O[i] = 1)
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
				else if (O[i] = 1)
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
		uint32_t pEnd = nData + 1;
		uint32_t i = 0;

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
		while (i < pEnd)
		{
			if (O[i] != eroder.ProcessedImg.data[i]) { P[i] = 1; }
			else { P[i] = 0; }
			i++;
		}
		eroder.~MorphologicalFilter();
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
		if (MaxLabel < 1) { LabelBlobs(chain, 25, conn); }

		// Make an empty BlobList
		uint32_t pEnd = MaxLabel + 1;
		uint32_t nCols = OriginalImg.cols;
		uint32_t nRows = OriginalImg.rows;
		uint32_t nData = nCols * nRows;

		// Create the statistics
		SoilMath::Stats<uint16_t, uint32_t, uint64_t> LabelStats((uint16_t *)LabelledImg.data, LabelledImg.cols, LabelledImg.rows, pEnd, 0, pEnd - 1);

		Blob emptyBlob;
		for (uint32_t i = 0; i < pEnd - 1; i++)
		{
			emptyBlob.Label = i + 1;
			emptyBlob.ROI.leftX = nCols;
			emptyBlob.ROI.leftY = nRows;
			emptyBlob.ROI.rightX = 0;
			emptyBlob.ROI.rightY = 0;
			emptyBlob.Area = LabelStats.bins[emptyBlob.Label];
			BlobList.push_back(emptyBlob);
		}

		// make Pointers
		ushort *L = (ushort *)LabelledImg.data;

		pEnd = nData + 1;
		ushort currentBlob = 1;
		uint32_t currentX, currentY;
		uint16_t leftX, leftY, rightX, rightY, index;
		//Loop through the labeled image and extract the Blobs
		for (uint32_t i = 0; i < pEnd; i++)
		{
			index = L[i];
			if (index != 0)
			{
				index -= 1;
				/* Determine the current x and y value of the current blob and
				sees if it is min/max */
				currentX = i / nCols;
				currentY = i % nCols;

				leftX = BlobList[index].ROI.leftX;
				leftY = BlobList[index].ROI.leftY;
				rightX = BlobList[index].ROI.rightX;
				rightY = BlobList[index].ROI.rightY;

				// Min value
				if (currentX < leftX) { BlobList[index].ROI.leftX = currentX; }
				if (currentY < leftY) { BlobList[index].ROI.leftY = currentY; }

				// Max value
				if (currentX > rightX) { BlobList[index].ROI.rightX = currentX; }
				if (currentY > rightY) { BlobList[index].ROI.rightY = currentY; }
			}
		}

		// Loop through the BlobList and finalize it
		pEnd = BlobList.size();
		ushort *LUT_filter = new ushort[MaxLabel]{};
		uint32_t x, y;

		for (uint32_t i = 0; i < pEnd; i++)
		{
			LUT_filter[i + 1] = 1;
			// Fix swapping of x and y
			BlobList[i].cvROI.y = BlobList[i].ROI.leftX;
			BlobList[i].cvROI.x = BlobList[i].ROI.leftY;
			BlobList[i].cvROI.height = BlobList[i].ROI.rightX - BlobList[i].ROI.leftX;
			BlobList[i].cvROI.width = BlobList[i].ROI.rightY - BlobList[i].ROI.leftY;
			if (BlobList[i].cvROI.width == 0) { BlobList[i].cvROI.width = 1; }
			if (BlobList[i].cvROI.height == 0) { BlobList[i].cvROI.height = 1; }

			BlobList[i].Img = CopyMat<ushort>(LabelledImg(BlobList[i].cvROI).clone(), LUT_filter, CV_8UC1);

			LUT_filter[i + 1] = 0;
		}

		//final check the bloblist and remove Mat with zero rows or cols
		std::remove_if(BlobList.begin(), BlobList.end(),[](Blob &b) -> bool
		{
			if (b.Img.cols == 0 || b.Img.rows == 0)	return true;
			else return false;
		});

		// Sort BlobList with top most most location and remove duplicates
		//std::sort(BlobList.begin(), BlobList.end(), [](Blob & a, Blob & b) -> bool
		//{
		//	//if (a.ROI.leftY < b.ROI.leftY)	return true;
		//	//else if (a.ROI.leftY == b.ROI.leftY && a.ROI.leftX < b.ROI.leftX) return true;
		//	//else return false;
		//	return (pow((float)a.ROI.leftX, 2) + pow((float)a.ROI.leftY, 2)) > (pow((float)b.ROI.leftX, 2) + pow((float)b.ROI.leftY, 2));
		//});
		////BlobList.erase(unique(BlobList.begin(), BlobList.end()), BlobList.end());
	}

	void Segment::makeConsecutive(uint16_t LastLabelUsed, uint16_t * tempLUT, uint16_t * &LUT_newVal)
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
		cv::Rect rectangle;
		cv::floodFill(ProcessedImg, cv::Point(col, row), cv::Scalar(2));

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