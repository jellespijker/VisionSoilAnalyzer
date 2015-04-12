#pragma once
#include "DrawFigure.h"

namespace SoilPlot
{
	class Line :
		public DrawFigure
	{
	public:
		Line();
		Line(cv::Point startpoint, cv::Point endpoint, uint32_t thickness = 1, cv::Scalar edgecolor = cv::Scalar(255, 255, 255, 255), cv::Scalar fillcolor = cv::Scalar(0, 0, 0, 0));

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
		float calcAngle(cv::Point startpoint, cv::Point endpoint);
		DrawFigure::Orientation_enum detOrientation(float angle);
		cv::Point RelStartPoint;
		cv::Point RelEndPoint;
	};
}