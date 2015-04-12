#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <algorithm>
#include <stdint.h>
#include <stdexcept>

namespace SoilPlot
{
	class DrawFigure
	{
	public:
		enum Orientation_enum
		{
			Horizontal,
			Vertical,
			Free
		};

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
		cv::Point abs(const cv::Point &point);
		virtual cv::Mat Draw() { return Figure; };
	};
}
