#pragma once
#define MORPHOLOGICALFILTER_VERSION 1

#include "ImageProcessing.h"

namespace Vision
{
	class MorphologicalFilter:
		public ImageProcessing
	{
	public:

		MorphologicalFilter();
		MorphologicalFilter(const Mat &src);
		MorphologicalFilter(const MorphologicalFilter &rhs);

		~MorphologicalFilter();

		MorphologicalFilter &operator=(MorphologicalFilter &rhs);

		void Dilation(const Mat &mask, bool chain = false);

		void Erosion(const Mat &mask, bool chain = false);
		void Erosion(const Mat &src, Mat &dst, const Mat &mask);

	private:

	};

}