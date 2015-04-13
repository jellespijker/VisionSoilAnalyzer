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

	Line::Line(cv::Point startpoint, cv::Point endpoint, uint32_t thickness, cv::Scalar edgecolor, cv::Scalar fillcolor)
	{
		Kwarter kwarter;
		float AngleError;
		Angle = calcAngle(startpoint, endpoint, kwarter, AngleError);
		Orientation = detOrientation(Angle);
		this->StartPoint = startpoint;
		this->EndPoint = endpoint;
		this->Thickness = thickness;
		this->EdgeColor = edgecolor;
		this->FillColor = fillcolor;
		cv::Point relPoint = abs(endpoint - startpoint);
		this->Length = static_cast<uint32_t>(roundf(sqrtf(powf((float)relPoint.x, 2.) + powf((float)relPoint.y, 2.))));

		switch (kwarter)
		{
		case SoilPlot::Line::I:
			this->RelStartPoint = cv::Point(0, 0);
			this->RelEndPoint = relPoint;
			this->TopLeftCorner = startpoint;
			break;
		case SoilPlot::Line::II:
			this->RelStartPoint = cv::Point(relPoint.x, 0);
			this->RelEndPoint = cv::Point(0, relPoint.y);
			this->TopLeftCorner = startpoint - cv::Point(relPoint.x, 0);
			break;
		case SoilPlot::Line::III:
			this->RelStartPoint = cv::Point(0, 0);
			this->RelEndPoint = relPoint;
			this->TopLeftCorner = startpoint - relPoint;
			break;
		case SoilPlot::Line::IV:
			this->RelStartPoint = cv::Point(0, relPoint.y);
			this->RelEndPoint = cv::Point(relPoint.x, 0);
			this->TopLeftCorner = startpoint - cv::Point(0, relPoint.y);
			break;
		case SoilPlot::Line::None:
			if (Orientation == Vertical)
			{
				if (AngleError == 157.) { this->TopLeftCorner = startpoint - cv::Point(thickness / 2, 0); }
				else { this->TopLeftCorner = endpoint - cv::Point(thickness / 2, 0); }
				this->RelStartPoint = cv::Point(thickness / 2, 0);
				this->RelEndPoint = cv::Point(thickness / 2, relPoint.y);
			}
			else if (Orientation == Horizontal)
			{
				if (AngleError == 0.) { this->TopLeftCorner = startpoint - cv::Point(0, thickness / 2); }
				else { this->TopLeftCorner = endpoint - cv::Point(0, thickness / 2); }
				this->RelStartPoint = cv::Point(0, thickness / 2);
				this->RelEndPoint = cv::Point(relPoint.x, thickness / 2);
			}
			break;
		}
	}

	Line::Line(const Line & rhs) : DrawFigure(rhs)
	{
		this->Angle = rhs.Angle;
		this->Orientation = rhs.Orientation;
		this->Length = rhs.Length;
		this->StartPoint = rhs.StartPoint;
		this->EndPoint = rhs.EndPoint;
		this->RelEndPoint = rhs.RelEndPoint;
		this->RelStartPoint = rhs.RelStartPoint;
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
			this->RelEndPoint = rhs.RelEndPoint;
			this->RelStartPoint = rhs.RelStartPoint;
		}
		return *this;
	}

	Line::~Line()
	{
	}

	cv::Mat Line::Draw()
	{
		if (Orientation == Horizontal) { Figure.create(Thickness, Length, CV_8UC4); }
		else if (Orientation == Vertical) { Figure.create(Length, Thickness, CV_8UC4); }
		else if (Orientation == Free) { Figure.create(cv::Size(abs(RelEndPoint - RelStartPoint) + cv::Point(1, 1)), CV_8UC4); }
		Figure.setTo(FillColor);
		cv::line(Figure, RelStartPoint, RelEndPoint, EdgeColor, Thickness);
		return Figure;
	}

	float Line::calcAngle(cv::Point startpoint, cv::Point endpoint, Kwarter &kwarter, float &angleerror)
	{
		cv::Point point = endpoint - startpoint;
		if (point.x > 0)
		{
			if (point.y > 0) { kwarter = Kwarter::I; }
			else { kwarter = Kwarter::IV; }
		}
		else
		{
			if (point.y < 0) { kwarter = Kwarter::III; }
			else { kwarter = Kwarter::II; }
		}
		float dxdy = 0.0;
		if (point.y != 0) { dxdy = point.x / (float)point.y; }
		float angle = atanf(dxdy);
		angleerror = roundf(angle * 100.0);
		if (angleerror == 0)
		{
			kwarter = Kwarter::None;
			if (point.y > 0) { angle = M_PI_2; }
			else if (point.y < 0) { angle = -M_PI_2; }
			else if (point.x < 0) { angle = M_PI; }
			angleerror = roundf(angle * 100.0);
		}
		return angle;
	}

	DrawFigure::Orientation_enum Line::detOrientation(float angle)
	{
		Orientation_enum retVal = Free;
		if (roundf(angle * 100.0) == 0 || roundf(angle * 100.0) == 314.0) { retVal = Horizontal; }
		else if (roundf(angle * 100.0) == 157. || roundf(angle * 100.0) == -157.) { retVal = Vertical; }
		return retVal;
	}
}