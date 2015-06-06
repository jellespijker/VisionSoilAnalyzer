#pragma once
#include "DrawFigure.h"

namespace SoilPlot
{
	class Grid :
		public DrawFigure
	{
	public:
		Grid();
		Grid(const Grid &rhs);

		Grid &operator=(const Grid &rhs);

		~Grid();

		cv::Mat Draw();
	};
}
