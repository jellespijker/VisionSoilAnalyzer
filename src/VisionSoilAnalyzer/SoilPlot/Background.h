#pragma once
#include "DrawFigure.h"

namespace SoilPlot
{
	class Background :
		public DrawFigure
	{
	public:
		Background() {};
		Background(const Background &rhs) = default;
		Background(cv::Size size);

		Background &operator=(const Background &rhs) = default;

		~Background() {};

		cv::Mat Draw();
	};
}
