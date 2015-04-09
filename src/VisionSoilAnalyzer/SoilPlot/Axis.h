#pragma once
#include "DrawFigure.h"
#include "Label.h"

namespace SoilPlot
{
	class Axis :
		public DrawFigure
	{
	public:
		enum Orientation_enum
		{
			Horizontal,
			Vertical
		};

		Axis() {};
		Axis(const Axis &rhs);
		Axis(cv::Point startpoint, cv::Point endpoint, Orientation_enum orientation);

		Axis &operator=(const Axis &rhs);

		~Axis() {};

		Label AxisLabel;
		bool ShowLabel;

		uint32_t TickResolutionMajor;
		bool ShowTickMajor;
		bool ShowValues;

		uint32_t TickResolutionMinor;
		bool ShowTickMinor;

		cv::Point StartPoint;
		cv::Point EndPoint;
		Orientation_enum Orientation;
		
		int Thickness;
		
		cv::Mat Draw();


	};
}
