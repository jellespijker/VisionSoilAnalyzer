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
		cv::Scalar_<uchar> FillColor;
		cv::Scalar_<uchar> EdgeColor;
		cv::Point TopLeftCorner;
		int Thickness;

		DrawFigure() {};
		DrawFigure(const DrawFigure &rhs) = default;

		DrawFigure &operator=(const DrawFigure &lhs) = default;

		~DrawFigure() {};

		void DrawOnTop(cv::Mat &graphfigure, cv::Point LeftTopPosition);
		virtual cv::Mat Draw();
	};
}
