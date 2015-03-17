#pragma once
#include "ImageProcessing.h"
#include "ConversionNotSupportedException.h"

namespace Vision
{
	class Conversion :
		public ImageProcessing
	{
	public:
		/*! Enumerator which indicates the colorspace used*/
		enum ColorSpace
		{
			CIE_lab,	/*!< CIE La*b* colorspace */
			CIE_XYZ,	/*!< CIE XYZ colorspace */
			RI,			/*!< Redness Index colorspace */
			RGB,		/*!< RGB colorspace */
			Intensity,	/*!< Grayscale colorspace */
			None		/*!< none */
		};
		ColorSpace OriginalColorSpace;	/*!< The original colorspace*/
		ColorSpace ProcessedColorSpace;	/*!< The destination colorspace*/

		Conversion();
		Conversion(const Mat &src);
		~Conversion();

		void Convert(ColorSpace convertFrom, ColorSpace convertTo, bool chain = false);
		void Convert(const Mat &src, Mat &dst, ColorSpace convertFrom, ColorSpace convertTo, bool chain = false);

	private:

		/*!< Conversion matrix used in the conversion between RGB and CIE XYZ*/
		float XYZmat[3][3] =
		{
			{ 0.412453, 0.357580, 0.180423 },
			{0.212671, 0.715160, 0.072169 },
			{0.019334, 0.119194, 0.950227 }
		};
		
		float whitePoint[3] = { 0.9504, 1.0000, 1.0889 }; /*!< Natural whitepoint in XYZ colorspace D65 according to Matlab */
		//float whitePoint[3] = { 0.9642, 1.0000, 0.8251 }; /*!< Natural whitepoint in XYZ colorspace D50 according to Matlab */

		void Lab2RI(float *O, float *P, int nData);
		void RGB2XYZ(uchar *O, float *P, int nData);
		void XYZ2Lab(float *O, float *P, int nData);
		void RGB2Intensity(uchar *O, uchar *P, int nData);
		inline float f_xyz2lab(float t);
	};
}