#pragma once
#include "DrawFigure.h"

namespace SoilPlot
{
	class Grid :
		public DrawFigure
	{
	public:
		Grid() {};
		~Grid() {};

		cv::Mat Draw();
	};
}
