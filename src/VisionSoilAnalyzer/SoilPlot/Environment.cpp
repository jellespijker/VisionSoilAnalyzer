#include "Environment.h"

namespace SoilPlot
{
	Environment::Environment(cv::Size size)
	{
		Figure = cv::Mat(size, CV_8UC4);
		this->FillColor = cv::Scalar_<uchar>(0, 0, 0, 255);
		this->EdgeColor = cv::Scalar_<uchar>(0, 0, 0, 255);
		this->Thickness = -1;
		this->TopLeftCorner = cv::Point(0, 0);

		// Create the standard Background
		GraphBackground = Background(size);
		GraphBackground.FillColor = this->FillColor;
		GraphBackground.EdgeColor = this->EdgeColor;
		GraphBackground.Thickness = this->Thickness;
		GraphBackground.TopLeftCorner = this->TopLeftCorner;

		this->GraphTitleBorderOffset = size.height * TITLE_STANDARD_OFFSET;

	}

	Environment::Environment(const Environment & rhs) : DrawFigure(rhs)
	{
		this->DataRegion = rhs.DataRegion;
		this->GraphAxes = rhs.GraphAxes;
		this->GraphBackground = rhs.GraphBackground;
		this->GraphGrid = rhs.GraphGrid;
		this->GraphTitle = rhs.GraphTitle;
		this->GraphTitleBorderOffset = rhs.GraphTitleBorderOffset;
	}

	Environment & Environment::operator=(const Environment & rhs)
	{
		if (&rhs != this)
		{
			DrawFigure::operator=(rhs);
			this->DataRegion = rhs.DataRegion;
			this->GraphAxes = rhs.GraphAxes;
			this->GraphBackground = rhs.GraphBackground;
			this->GraphGrid = rhs.GraphGrid;
			this->GraphTitle = rhs.GraphTitle;
			this->GraphTitleBorderOffset = rhs.GraphTitleBorderOffset;
		}
		return *this;
	}

	cv::Mat Environment::Draw()
	{
		if (Figure.rows > 0 && Figure.cols > 0)
		{
			GraphBackground.Draw();
			GraphBackground.DrawOnTop(Figure, GraphBackground.TopLeftCorner);
			for_each(GraphAxes.begin(), GraphAxes.end(), [&](Axis &X) 
			{
				X.Draw();
				X.DrawOnTop(Figure, X.TopLeftCorner);
			});
			//GraphGrid.Draw();
			//GraphGrid.DrawOnTop(Figure, GraphGrid.TopLeftCorner);
			GraphTitle.Draw();
			GraphTitle.TopLeftCorner = cv::Point((Figure.cols - GraphTitle.Figure.cols) / 2, GraphTitleBorderOffset);
			GraphTitle.DrawOnTop(Figure, GraphTitle.TopLeftCorner);
		}
		return Figure;
	}
}