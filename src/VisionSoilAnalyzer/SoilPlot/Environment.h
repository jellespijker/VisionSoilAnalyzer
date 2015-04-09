#pragma once
#define TITLE_STANDARD_OFFSET 0.05

#include "DrawFigure.h"
#include "Background.h"
#include "Axis.h"
#include "Grid.h"
#include "Label.h"
#include <stdint.h>
#include <vector>

namespace SoilPlot
{
	class Environment :
		public DrawFigure
	{
	public:
		Environment() {};
		Environment(cv::Size size);
		Environment(const Environment &rhs) {};

		Environment &operator=(const Environment &rhs) {};

		~Environment() {};

		Background GraphBackground;
		Label GraphTitle;
		std::vector<Axis> GraphAxes;
		Grid GraphGrid;
		cv::Rect DataRegion;

		uint32_t GraphTitleBorderOffset;

		cv::Mat Draw();
	};
}
