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
		this->ShowLabel = rhs.ShowLabel;
		this->ShowTickMajor = rhs.ShowTickMajor;
		this->ShowTickMinor = rhs.ShowTickMinor;
		this->ShowValues = rhs.ShowValues;
		this->StartPoint = rhs.StartPoint;
		this->TickResolutionMajor = rhs.TickResolutionMajor;
		this->TickResolutionMinor = rhs.TickResolutionMinor;
		this->ValuesPosition = rhs.ValuesPosition;
		this->relEndPoint = rhs.relEndPoint;
		this->relStartPoint = rhs.relStartPoint;
		this->Orientation = rhs.Orientation;
		this->Values = rhs.Values;
	}

	Axis::Axis(cv::Point startpoint, uint32_t length, Orientation_enum orientation, std::vector<Label> &values, Label label, ValuePosition valuepos, bool showmajortick)
	{
		this->StartPoint = startpoint;
		this->relStartPoint = cv::Point(0, 0);
		this->Orientation = orientation;
		switch (Orientation)
		{
		case SoilPlot::DrawFigure::Horizontal:
			relEndPoint = relStartPoint + cv::Point(length, 0);
			break;
		case SoilPlot::DrawFigure::Vertical:
			relEndPoint = relStartPoint + cv::Point(0, length);
			break;
		case SoilPlot::DrawFigure::Free:
			throw std::logic_error("Free Orientation not supported for axis Exception!");
			break;
		}
	
		this->Values = values;
		if (values.size() > 0) { this->ShowValues = true; }
		else { this->ShowValues = false; }
		this->ValuesPosition = valuepos;

		this->ShowTickMinor = false;
		this->ShowTickMajor = showmajortick;
		this->AxisLabel = label;
		this->ShowLabel = true;
		this->EdgeColor = cv::Scalar(255, 255, 255, 255);
		this->FillColor = cv::Scalar(0, 0, 0, 0);
		this->Thickness = 1;
		this->TickResolutionMajor = values.size();
		this->TickResolutionMinor = 1;
		this->noValues = Values.size();
	}

	Axis & Axis::operator=(const Axis & rhs)
	{
		if (&rhs != this)
		{
			DrawFigure::operator=(rhs);
			this->AxisLabel = rhs.AxisLabel;
			this->ShowLabel = rhs.ShowLabel;
			this->ShowTickMajor = rhs.ShowTickMajor;
			this->ShowTickMinor = rhs.ShowTickMinor;
			this->ShowValues = rhs.ShowValues;
			this->StartPoint = rhs.StartPoint;
			this->TickResolutionMajor = rhs.TickResolutionMajor;
			this->TickResolutionMinor = rhs.TickResolutionMinor;
			this->ValuesPosition = rhs.ValuesPosition;
			this->relEndPoint = rhs.relEndPoint;
			this->relStartPoint = rhs.relStartPoint;
			this->Orientation = rhs.Orientation;
			this->Values = rhs.Values;
		}
		return *this;
	}

	Axis::~Axis()
	{
	}

	cv::Mat Axis::Draw()
	{
		// Draws the Axii
		if (Orientation == Horizontal)
		{
			relStartPoint += cv::Point(0, Thickness / 2);
			relEndPoint += cv::Point(0, Thickness / 2);
		}
		else
		{
			relStartPoint += cv::Point(Thickness / 2, 0);
			relEndPoint += cv::Point(Thickness / 2, 0);
		}
		Axii = Line(relStartPoint, relEndPoint, Thickness, EdgeColor, FillColor);
		Axii.Draw();

		// Draws the label, values and ticks, needed at this stage to calc the figure dimensions
		if (ShowLabel) { AxisLabel.Draw(); }
		if (ShowValues) { for_each(Values.begin(), Values.end(), [&](Label &V) { V.Draw(); }); 	}
		if (ShowTickMajor) { for_each(MajorTicks.begin(), MajorTicks.end(), [&](Line &T) { T.Draw(); }); }
 		if (ShowTickMinor) { for_each(MinorTicks.begin(), MinorTicks.end(), [&](Line &T) { T.Draw(); }); }
		
		// Calc the figure dimensions and the TopLeftCorner locations
		cv::Size figsize(Axii.Figure.cols, Axii.Figure.rows);
		
		if (Orientation == Horizontal)
		{
			if (ShowLabel) 
			{
				// If the Label is bigger then the axii, resize the label
				if (AxisLabel.Figure.cols > Axii.Figure.cols)
				{
					float factor = static_cast<float>(Axii.Figure.cols) / static_cast<float>(AxisLabel.Figure.cols);
					cv::resize(AxisLabel.Figure, AxisLabel.Figure,cv::Size(floor(factor * AxisLabel.Figure.cols), floor(factor * AxisLabel.Figure.rows)));
				}
				
				// Add the height of the Label to the Axis Figure
				figsize.height += AxisLabel.Figure.rows; 

				// Center the Label below the axii
				AxisLabel.TopLeftCorner = cv::Point((Axii.Figure.cols - AxisLabel.Figure.cols) / 2, Axii.Figure.rows);
			}
			if (ShowTickMajor)
			{
				// Make the tick vector
				MajorTicks.resize(Values.size(), Line(cv::Point(0, 0), cv::Point(0, Axii.Figure.rows * 2), Thickness, EdgeColor, FillColor));
				TickResolutionMajor = Axii.Figure.rows / MajorTicks.size();
				
				// Add the height of the major ticks to the Axis Figure
				figsize.height += MajorTicks[0].Figure.rows;
				
				// Add the width of the Major tick to the Axis Figure, but anly if the values aren't shown
				if (!ShowValues) { figsize.width += MajorTicks[0].Figure.cols; }
				
				// Set the TopLeftCorners for the major ticks
				MajorTicks[0].TopLeftCorner = cv::Point(0, 0);
				uint32_t N = 0;
				for_each(MajorTicks.begin() + 1, MajorTicks.end(), [&](Line &L) { L.TopLeftCorner = cv::Point(MajorTicks[N++].TopLeftCorner.x + TickResolutionMajor, 0); }); 				
				
				// Move the TopLeftCorner for the Axii
				Axii.TopLeftCorner += cv::Point(0, MajorTicks[0].Figure.rows / 2);
				// Move the TopLeftCorner for Label
				if (ShowLabel) { AxisLabel.TopLeftCorner += cv::Point(0, MajorTicks[0].Figure.rows); }
			}
			if (ShowValues)
			{
				// Add 1/2 of the two outer values to the Axis Figure width if they're shown under the tick
				if (ValuesPosition == UnderTick)
				{
					figsize.width += Values[0].Figure.cols / 2;
					figsize.width += Values.back().Figure.cols / 2;
				}
				
				// Set the TopLeftCorner for each Value
				uint32_t N = 0;
				Values[0].TopLeftCorner = cv::Point(0, figsize.height);
				for_each(Values.begin() + 1, Values.end(), [&](Label &V) 
				{
					V.TopLeftCorner = cv::Point((Values[N++].TopLeftCorner.x / 2) + TickResolutionMajor - (V.Figure.cols / 2), figsize.height);
				});

				// Add the height of the values to the Axis Figure
				figsize.height += Values[0].Figure.rows;

				// Move the TopLeftCorner of the Label
				if (ShowLabel) { AxisLabel.TopLeftCorner += cv::Point(0, Values[0].Figure.rows); }
			}
		}
		else
		{
			if (ShowLabel) { figsize.width += AxisLabel.Figure.rows; }
			if (ShowValues)
			{
				uint32_t maxValueLength = 0;
				for_each(Values.begin(), Values.end(), [&](Label &L) { if (maxValueLength < L.Figure.cols) { maxValueLength = L.Figure.cols; } });
				figsize.width += maxValueLength;
			}
			if (ShowTickMajor)
			{
				figsize.width += MajorTicks[0].Figure.rows;
				if (!ShowValues) { figsize.height += MajorTicks[0].Figure.cols; }
			}
		}
		Figure.create(figsize, CV_8UC4);
		Figure.setTo(FillColor);
		
		Axii.DrawOnTop(Figure);
		if (ShowLabel) { AxisLabel.DrawOnTop(Figure); }
		//if (ShowValues) { for_each(Values.begin(), Values.end(), [&](Label &V) { V.DrawOnTop(Figure); }); }
		//if (ShowTickMajor) { for_each(MajorTicks.begin(), MajorTicks.end(), [&](Line &T) { T.DrawOnTop(Figure); }); }
		//if (ShowTickMinor) { for_each(MinorTicks.begin(), MinorTicks.end(), [&](Line &T) { T.DrawOnTop(Figure); }); }
//
		SHOW_DEBUG_IMG(Figure, uchar, 255, "Axis", false);
		return Figure;
	}


}