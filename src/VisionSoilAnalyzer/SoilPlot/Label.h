#pragma once
#include <string>

#include "DrawFigure.h"

namespace SoilPlot
{
	class Label : 
		public DrawFigure
	{
	public:
		Label() {};
		Label(const Label &rhs) {};

		Label &operator=(const Label &rhs) {};

		~Label() {};
		std::string Text;

		cv::HersheyFonts Font;

		cv::Mat Draw();

	};

}