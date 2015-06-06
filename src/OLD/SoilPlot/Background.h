#pragma once
#include "DrawFigure.h"

namespace SoilPlot
{
	class Background :
		public DrawFigure
	{
	public:
		Background();
		Background(const Background &rhs);
		Background(cv::Size size);

		Background &operator=(const Background &rhs);

		~Background() {};

		cv::Mat Draw();
	};
}
