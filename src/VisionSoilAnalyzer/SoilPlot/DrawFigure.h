#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <algorithm>
#include <stdint.h>

namespace SoilPlot
{
	class DrawFigure
	{
	public:
		cv::Mat Figure;
		cv::Scalar FillColor;
		cv::Scalar EdgeColor;
		cv::Point TopLeftCorner;
		float ForegrondBlend;
		int Thickness;

		DrawFigure();
		DrawFigure(const DrawFigure &rhs);

		DrawFigure &operator=(const DrawFigure &rhs);

		~DrawFigure() {};

		void DrawOnTop(cv::Mat &graphfigure, cv::Point LeftTopPosition);
		virtual cv::Mat Draw() { return Figure; };
	};
}
