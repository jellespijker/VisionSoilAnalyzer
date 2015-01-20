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

		~MorphologicalFilter();

		void Erosion(const Mat &mask, bool chain = true);
		void Erosion(const Mat &src, Mat &dst, const Mat &mask);

	private:

	};

}