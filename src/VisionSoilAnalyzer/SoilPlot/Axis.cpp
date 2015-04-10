#include "Axis.h"

namespace SoilPlot
{
	Axis::Axis()
	{
		this->Orientation = Horizontal;
		this->ShowLabel = false;
		this->ShowTickMajor = false;
		this->ShowTickMinor = false;
		this->ShowValues = false;
		this->StartPoint = cv::Point(0 ,0);
		this->TickResolutionMajor = 5;
		this->TickResolutionMinor = 1;
	}

	Axis::Axis(const Axis & rhs) : DrawFigure(rhs)
	{
		this->AxisLabel = rhs.AxisLabel;
		this->Orientation = rhs.Orientation;
		this->ShowLabel = rhs.ShowLabel;
		this->ShowTickMajor = rhs.ShowTickMajor;
		this->ShowTickMinor = rhs.ShowTickMinor;
		this->ShowValues = rhs.ShowValues;
		this->StartPoint = rhs.StartPoint;
		this->TickResolutionMajor = rhs.TickResolutionMajor;
		this->TickResolutionMinor = rhs.TickResolutionMinor;
	}

	Axis::Axis(cv::Point startpoint, uint32_t length, Orientation_enum orientation)
	{
		this->StartPoint = startpoint;
		this->Orientation = orientation;
		cv::Point TLcorner = startpoint;
	}

	Axis & Axis::operator=(const Axis & rhs)
	{
		if (&rhs != this)
		{
			DrawFigure::operator=(rhs);
			this->AxisLabel = rhs.AxisLabel;
			this->Orientation = rhs.Orientation;
			this->ShowLabel = rhs.ShowLabel;
			this->ShowTickMajor = rhs.ShowTickMajor;
			this->ShowTickMinor = rhs.ShowTickMinor;
			this->ShowValues = rhs.ShowValues;
			this->StartPoint = rhs.StartPoint;
			this->TickResolutionMajor = rhs.TickResolutionMajor;
			this->TickResolutionMinor = rhs.TickResolutionMinor;
		}
		return *this;
	}

	Axis::~Axis()
	{
	}

	cv::Mat Axis::Draw()
	{
		Figure.create(calcFigureSize(), CV_8UC4);
		Figure.setTo(FillColor);
		return Figure;
	}

	cv::Size Axis::calcFigureSize()
	{
		cv::Size figSize(0, 0);
		cv::Size LabelSize(0, 0);
		cv::Size ValueSize(0, 0);

		int baseline = 0;

		// Calculate height
		if (ShowTickMajor) { figSize.height += MajorTicks[0].Length * 1.05;}			// 1/2 Major ticklength + 0.05 offset		
		else if (ShowTickMinor) { figSize.height += MinorTicks[0].Length * 1.05; }		// 1/2 Minor ticklength + 0.05 offset
		else { figSize.height += Axii.Thickness * 1.05; }								// Only line width  + 0.05 offset
		if (ShowLabel) 
		{
			LabelSize = cv::getTextSize(AxisLabel.Text.str(), AxisLabel.Font, AxisLabel.Scale, AxisLabel.Thickness, &baseline);
			figSize.height += AxisLabel.Thickness * 1.575; // + offset 0.05
		}
		if (ShowValues)
		{
			ValueSize = cv::getTextSize(Values[Values.size() - 1].Text.str(), Values[Values.size() - 1].Font, Values[Values.size() - 1].Scale, Values[Values.size() -1].Thickness, &baseline);
			figSize.height += (ValueSize.height + Values[Values.size() - 1].Thickness * 1.5) * 1.05; // + offset 0.05
		}

		// Calculate width
		figSize.width += Axii.Length;
		if (ShowValues) 
		{
			figSize.width += ValueSize.width * 0.5;		// 1/2 the last Value
			ValueSize = cv::getTextSize(Values[0].Text.str(), Values[0].Font, Values[0].Scale, Values[0].Thickness, &baseline);
			figSize.width += ValueSize.width * 0.5;		// 1/2 the first Value
		}
		else if (ShowTickMajor)
		{
			figSize.width += MajorTicks[0].Thickness;
		}
		return cv::Size();
	}

}