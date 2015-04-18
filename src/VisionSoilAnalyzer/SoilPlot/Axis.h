#pragma once

#include "DrawFigure.h"
#include "Label.h"
#include "Line.h"

namespace SoilPlot
{
	class Axis :
		public DrawFigure
	{
	public:
		enum ValuePosition
		{
			UnderTick,
			NextToTick
		};

		Axis();
		Axis(const Axis &rhs);
		Axis(cv::Point startpoint, uint32_t length, Orientation_enum orientation, std::vector<Label> &values, Label label, ValuePosition valuepos, bool showmajortick = true);

		Axis &operator=(const Axis &rhs);

		~Axis();

		Label AxisLabel;
		bool ShowLabel;

		uint32_t TickResolutionMajor;
		std::vector<Line> MajorTicks;
		bool ShowTickMajor;

		std::vector<Label> Values;
		bool ShowValues;
		ValuePosition ValuesPosition;

		uint32_t TickResolutionMinor;
		std::vector<Line> MinorTicks;
		bool ShowTickMinor;

		cv::Point StartPoint;
		Line Axii;
		Orientation_enum Orientation;
		
		cv::Mat Draw();

	private:
		cv::Size calcFigureSize();
		uint32_t noValues;
		cv::Point endPoint;
	};
}
