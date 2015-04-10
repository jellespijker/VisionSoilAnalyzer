#include "Line.h"
#include <cmath>

namespace SoilPlot
{
	Line::Line()
	{
		this->Angle = 0.0;
		this->Orientation =Free;
		this->Length = 1;
		StartPoint = cv::Point(0, 0);
		EndPoint = cv::Point(0, 0);
	}

	Line::Line(cv::Point startpoint, cv::Point endpoint)
	{
		this->StartPoint = startpoint;
		this->EndPoint = endpoint;
		cv::Point diffPoint = EndPoint - StartPoint;
		Angle = calcAngle(diffPoint);
		Orientation = detOrientation(Angle, diffPoint);
		Figure.create(cv::Size(diffPoint + cv::Point(1, 1)), CV_8UC4);
		Figure.setTo(FillColor);
	}

	Line::Line(cv::Point startpoint, Orientation_enum orientation, uint32_t length, bool flip)
	{
		this->StartPoint = startpoint;
		this->Orientation = orientation;
		this->Length = length;
		if (Orientation == Vertical) { EndPoint = cv::Point(StartPoint.x, startpoint.y + Length); }
		else { EndPoint = cv::Point(StartPoint.x + Length, StartPoint.y); }
		cv::Point diffPoint = EndPoint - StartPoint;
		Angle = 0.0;
		Figure.create(cv::Size(diffPoint), CV_8UC4);
		Figure.setTo(FillColor);
	}

	Line::Line(const Line & rhs) : DrawFigure(rhs)
	{
		this->Angle = rhs.Angle;
		this->Orientation = rhs.Orientation;
		this->Length = rhs.Length;
		this->StartPoint = rhs.StartPoint;
		this->EndPoint = rhs.EndPoint;
	}

	Line & Line::operator=(const Line & rhs)
	{
		if (&rhs != this)
		{
			this->Angle = rhs.Angle;
			this->Orientation = rhs.Orientation;
			this->Length = rhs.Length;
			this->StartPoint = rhs.StartPoint;
			this->EndPoint = rhs.EndPoint;
		}
		return *this;
	}


	Line::~Line()
	{
	}

	cv::Mat Line::Draw()
	{
		if (Figure.rows > 0 && Figure.cols > 0 && StartPoint != EndPoint)
		{
			Figure.setTo(FillColor);
			cv::Point endPoint(Figure.cols - 1, Figure.rows - 1);
			cv::line(Figure, cv::Point(0,0),endPoint, EdgeColor, Thickness);
		}
		return Figure;
	}

	float Line::calcAngle(cv::Point point)
	{
		float dxdy = 0.0;
		if (point.y != 0) { dxdy = point.x / (float)point.y; }
		float angle = atanf(dxdy);
		return angle;
	}

	DrawFigure::Orientation_enum Line::detOrientation(float angle, cv::Point point)
	{
		Orientation_enum retVal = Free;
		if (roundf(angle * 100.0) == 0) 
		{
			if (point.x == 0) {	retVal = Horizontal; }
			else { retVal = Vertical; }
		}
		return retVal;
	}
}