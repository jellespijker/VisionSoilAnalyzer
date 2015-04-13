#pragma once
#include <sstream>
#include <string>

#include "DrawFigure.h"

namespace SoilPlot
{
	class Label : 
		public DrawFigure
	{
	public:
		Label();
		Label(const Label &rhs);

		Label &operator=(const Label &rhs);

		~Label() {};

		std::ostringstream Text;
		cv::HersheyFonts Font;
		int Scale;
		float BackgroundBlend;

		cv::Mat Draw();
	};

}