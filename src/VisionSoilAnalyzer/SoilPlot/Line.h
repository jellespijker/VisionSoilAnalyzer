#pragma once
#include "DrawFigure.h"

namespace SoilPlot
{
	class Line :
		public DrawFigure
	{
	public:
		Line();
		Line(cv::Point startpoint, cv::Point endpoint);
		Line(cv::Point startpoint, Orientation_enum orientation, uint32_t length, bool flip = false);

		Line(const Line &rhs);

		Line &operator=(const Line &rhs);

		~Line();

		uint32_t Length;
		Orientation_enum Orientation;
		float Angle;
		cv::Point StartPoint;
		cv::Point EndPoint;

		cv::Mat Draw();
	private:
		float calcAngle(cv::Point point);
		DrawFigure::Orientation_enum detOrientation(float angle, cv::Point point);
	};
}