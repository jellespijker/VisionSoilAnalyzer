#pragma once

#include <vector>
#include <queue>
#include <string>
#include <stdint.h>
#include <iostream>
#include <algorithm>

#include "opencv2/imgproc/imgproc.hpp"

#include "ImageProcessing.h"
#include "MorphologicalFilter.h"
#include "../SoilMath/SoilMath.h"

namespace Vision
{
	class Segment :
		public ImageProcessing
	{
	public:
		/*! Coordinates for the region of interest*/
		typedef struct Rect
		{
			uint16_t leftX;		/*!< Left X coordinate*/
			uint16_t leftY;		/*!< Left Y coordinate*/
			uint16_t rightX;	/*!< Right X coordinate*/
			uint16_t rightY;	/*!< Right Y coordinate*/
			Rect(uint16_t lx, uint16_t ly, uint16_t rx, uint16_t ry) : leftX(lx), leftY(ly), rightX(rx), rightY(ry) {};
		} Rect_t;

		typedef std::vector<Vision::Segment::Rect_t> RectList_t;

		/*! Individual blob*/
		typedef struct Blob
		{
			uint16_t Label;	/*!< ID of the blob*/
			cv::Mat Img;	/*!< BW image of the blob all the pixel belonging to the blob are set to 1 others are 0*/
			cv::Rect ROI;	/*!< Coordinates for the blob in the original picture as a cv::Rect*/
			uint32_t Area;  /*!< Calculated stats of the blob*/
			Blob(uint16_t label, uint32_t area) : Label(label), Area(area) {};
		} Blob_t;

		typedef std::vector<Blob_t> BlobList_t;
		BlobList_t BlobList; /*!< vector with all the individual blobs*/

		/*! Enumerator to indicate what kind of object to extract  */
		enum TypeOfObjects
		{
			Bright,		/*!< Enum value Bright object */
			Dark		/*!< Enum value Dark object. */
		};

		/*! Enumerator to indicate how the pixel correlate between each other in a blob*/
		enum Connected
		{
			Four,	/*!< Enum Four connected, relation between Center, North, East, South and West*/
			Eight	/*!< Enum Eight connected, relation between Center, North, NorthEast, East, SouthEast, South, SouthWest, West and NorthWest */
		};

		/*!< Enumerator which indicate which Segmentation technique should be used */
		enum SegmentationType
		{
			Normal,			/*!< Segmentation looking at the intensity of an individual pixel */
			LabNeuralNet,	/*!< Segmentation looking at the chromatic a* and b* of the processed pixel and it's surrounding pixels, feeding it in an Neural Net */
			GraphMinCut		/*!< Segmentation using a graph function and the minimum cut */
		};

		cv::Mat LabelledImg;	/*!< Image with each individual blob labeled with a individual number */
		uint16_t MaxLabel = 0;	/*!< Maximum labels found in the labelled image*/
		uint16_t noOfFilteredBlobs = 0;	/*!< Total numbers of blobs that where filtered beacuse the where smaller than the minBlobArea*/

		ucharStat_t OriginalImgStats; /*!< Statistical data from the original image*/
		uint8_t ThresholdLevel = 0;				/*!< Current calculated threshold level*/
		
		Segment(); 
		Segment(const Mat &src); 
		~Segment(); 

		void ConvertToBW(TypeOfObjects Typeobjects);
		void ConvertToBW(const Mat &src, Mat &dst, TypeOfObjects Typeobjects);

		void GetEdges(bool chain = false, Connected conn = Eight);
		void GetEdges(const Mat &src, Mat &dst, bool chain = false, Connected conn = Eight);

		void GetEdgesEroding(bool chain = false);

		void GetBlobList(bool chain = false, Connected conn = Eight);

		void Threshold(uchar t, TypeOfObjects Typeobjects);

		void LabelBlobs(bool chain = false, uint16_t minBlobArea = 25, Connected conn = Eight);

		void RemoveBorderBlobs(uint32_t border = 1, bool chain = false);

		void FillHoles(bool chain = false);

	private:
		uint8_t GetThresholdLevel(TypeOfObjects TypeObject);
		void SetBorder(uchar *P, uchar setValue);
		void FloodFill(uchar *O, uchar *P, uint16_t x, uint16_t y, uchar fillValue, uchar OldValue);
		void makeConsecutive(uint16_t LastLabelUsed, uint16_t * tempLUT, uint16_t *LUT_newVal);
	};
}