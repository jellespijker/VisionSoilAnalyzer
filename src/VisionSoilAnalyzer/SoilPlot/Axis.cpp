#include "Axis.h"

namespace SoilPlot
{
	Axis::Axis(const Axis & rhs) : DrawFigure(rhs)
	{
		this->AxisLabel = rhs.AxisLabel;
		this->Length = rhs.Length;
		this->Orientation = rhs.Orientation;
		this->ShowLabel = rhs.ShowLabel;
		this->ShowTickMajor = rhs.ShowTickMajor;
		this->ShowTickMinor = rhs.ShowTickMinor;
		this->ShowValues = rhs.ShowValues;
		this->ShowValues = rhs.ShowValues;
		this->StartPoint = rhs.StartPoint;
		this->TickResolutionMajor = rhs.TickResolutionMajor;
		this->TickResolutionMinor = rhs.TickResolutionMinor;
	}

	Axis::Axis(cv::Point startpoint, uint32_t length, Orientation_enum orientation)
	{
		this->StartPoint = startpoint;
		this->Length = length;
		this->Orientation = orientation;
		cv::Point TLcorner = startpoint;
	}

	Axis & Axis::operator=(const Axis & rhs)
	{
		if (&rhs != this)
		{
			DrawFigure::operator=(rhs);
			this->AxisLabel = rhs.AxisLabel;
			this->Length = rhs.Length;
			this->Orientation = rhs.Orientation;
			this->ShowLabel = rhs.ShowLabel;
			this->ShowTickMajor = rhs.ShowTickMajor;
			this->ShowTickMinor = rhs.ShowTickMinor;
			this->ShowValues = rhs.ShowValues;
			this->ShowValues = rhs.ShowValues;
			this->StartPoint = rhs.StartPoint;
			this->TickResolutionMajor = rhs.TickResolutionMajor;
			this->TickResolutionMinor = rhs.TickResolutionMinor;
		}
		return *this;
	}

	cv::Mat Axis::Draw()
	{

		return Figure;
	}

}