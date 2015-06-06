#include "DrawFigure.h"

namespace SoilPlot
{
	DrawFigure::DrawFigure()
	{
		this->EdgeColor = cv::Scalar(0, 0, 0, 0);
		this->FillColor = cv::Scalar(0, 0, 0, 0);
		this->Thickness = 1;
		this->TopLeftCorner = cv::Point(0,0);
		this->ForegrondBlend = 1.;
	}

	DrawFigure::DrawFigure(const DrawFigure & rhs)
	{
		this->EdgeColor = rhs.EdgeColor;
		this->Figure = rhs.Figure;
		this->FillColor = rhs.FillColor;
		this->Thickness = rhs.Thickness;
		this->TopLeftCorner = rhs.TopLeftCorner;
		this->ForegrondBlend = rhs.ForegrondBlend;
		this->Orientation = rhs.Orientation;
	}

	DrawFigure & DrawFigure::operator=(const DrawFigure & rhs)
	{
		if (&rhs != this)
		{
			this->EdgeColor = rhs.EdgeColor;
			this->Figure = rhs.Figure;
			this->FillColor = rhs.FillColor;
			this->Thickness = rhs.Thickness;
			this->TopLeftCorner = rhs.TopLeftCorner;
			this->ForegrondBlend = rhs.ForegrondBlend;
			this->Orientation = rhs.Orientation;
		}
		return *this;
	}

	void DrawFigure::DrawOnTop(cv::Mat & graphfigure)
	{
		cv::Mat workImg = graphfigure(cv::Rect(TopLeftCorner.x, TopLeftCorner.y, Figure.cols, Figure.rows));
		
		for (uint32_t i = 0; i < Figure.cols; i++)
		{
			for (uint32_t j = 0; j < Figure.rows; j++)
			{
				cv::Vec4b *F = &Figure.at<cv::Vec4b>(cv::Point(i, j));
				cv::Vec4b *G = &workImg.at<cv::Vec4b>(cv::Point(i, j));
				float ratio = (float)(*F)[3] / 255;
				for (uint32_t k = 0; k < 3; k++)
				{
					int newVal = (*G)[k] * (1 - ratio) + (*F)[k] * ratio;
					if (newVal < 0) { newVal = 0; }
					else if (newVal > 255) { newVal = 255; }
					(*G)[k] = static_cast<uchar>(newVal);
				}
				(*G)[3] = 255;
			}
		}
	}

	cv::Point DrawFigure::abs(const cv::Point &point)
	{
		return cv::Point(std::abs(point.x), std::abs(point.y));
	}
	
	void DrawFigure::Rotate()
	{
		//TODO make neat and 
		if (Orientation == Horizontal)
		{
			cv::transpose(Figure, Figure);
			TopLeftCorner -= cv::Point(0, Figure.rows); // Maybe not necessary, need to check
			Orientation = Vertical;
		}
		else if (Orientation == Vertical)
		{
			Orientation = Horizontal;
		}
	}
}