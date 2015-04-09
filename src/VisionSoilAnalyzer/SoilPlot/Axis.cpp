#include "Axis.h"

namespace SoilPlot
{
	Axis::Axis(const Axis & rhs)
	{
	}

	Axis::Axis(cv::Point startpoint, cv::Point endpoint, Orientation_enum orientation)
	{
		this->StartPoint = startpoint;
		this->EndPoint = endpoint;
		this->Orientation = orientation;
		cv::Point TLcorner = startpoint;
	}

	Axis & Axis::operator=(const Axis & rhs)
	{
		return *this;
	}

	cv::Mat Axis::Draw()
	{

		return Figure;
	}

}