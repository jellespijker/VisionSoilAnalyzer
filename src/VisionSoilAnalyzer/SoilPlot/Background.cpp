#include "Background.h"


namespace SoilPlot
{
	Background::Background()
	{
	}

	Background::Background(const Background & rhs) : DrawFigure(rhs)
	{
	}

	Background::Background(cv::Size size)
	{
		Figure = cv::Mat(size, CV_8UC4);
	}

	Background & Background::operator=(const Background &rhs)
	{
		if (&rhs != this)
		{
			DrawFigure::operator=(rhs);
		}
		return *this;
	}

	cv::Mat Background::Draw()
	{
		if (Figure.cols > 0 && Figure.rows > 0)
		{
			if (this->Thickness > 0 && EdgeColor != FillColor)
			{
				Figure.setTo(EdgeColor);
				cv::Rect FillROI(Thickness, Thickness, Figure.cols - 2 * Thickness, Figure.rows - 2 * Thickness);
				Figure(FillROI).setTo(FillColor);
			}
			else
			{
				Figure.setTo(FillColor);
			}
		}
		return Figure;
	}
}