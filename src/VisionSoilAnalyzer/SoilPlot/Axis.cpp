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
		Axii.Draw();
		Axii.DrawOnTop(Figure, Axii.TopLeftCorner);
		if (ShowLabel)
		{
			AxisLabel.Draw();
			AxisLabel.DrawOnTop(Figure, AxisLabel.TopLeftCorner);
		}
		if (ShowTickMajor)
		{
			for_each(MajorTicks.begin(), MajorTicks.end(), [&](Line &T)
			{
				T.Draw();
				T.DrawOnTop(Figure, T.TopLeftCorner);
			});
		}
		if (ShowTickMinor)
		{
			for_each(MinorTicks.begin(), MinorTicks.end(), [&](Line &T)
			{
				T.Draw();
				T.DrawOnTop(Figure, T.TopLeftCorner);
			});
		}
		if (ShowValues)
		{
			for_each(Values.begin(), Values.end(), [&](Label &V)
			{
				V.Draw();
				V.DrawOnTop(Figure, V.TopLeftCorner);
			});
		}
		return Figure;
	}

	cv::Size Axis::calcFigureSize()
	{
		cv::Size figSize(0, 0);
		cv::Size LabelSize(0, 0);
		cv::Size ValueSize(0, 0);

		int baseline = 0;

		if (Orientation == Horizontal)
		{
			// Calculate height
			if (ShowTickMajor) { figSize.height += MajorTicks[0].Length * 1.05; }			// 1/2 Major ticklength + 0.05 offset		
			else if (ShowTickMinor) { figSize.height += MinorTicks[0].Length * 1.05; }		// 1/2 Minor ticklength + 0.05 offset
			else { figSize.height += Axii.Thickness * 1.05; }								// Only line width  + 0.05 offset
			if (ShowLabel)
			{
				LabelSize = cv::getTextSize(AxisLabel.Text.str(), AxisLabel.Font, AxisLabel.Scale, AxisLabel.Thickness, &baseline);
				figSize.height += (LabelSize.height + AxisLabel.Thickness * 1.5) * 1.05; // + offset 0.05
			}
			if (ShowValues)
			{
				ValueSize = cv::getTextSize(Values[0].Text.str(), Values[0].Font, Values[0].Scale, Values[0].Thickness, &baseline);
				figSize.height += (ValueSize.height + Values[Values.size() - 1].Thickness * 1.5) * 1.05; // + offset 0.05
			}

			// Calculate width
			figSize.width += Axii.Length;
			if (ShowValues)
			{
				figSize.width += ValueSize.width * 0.5;		// 1/2 the last Value
				ValueSize = cv::getTextSize(Values[Values.size() - 1].Text.str(), Values[Values.size() - 1].Font, Values[Values.size() - 1].Scale, Values[Values.size() - 1].Thickness, &baseline);
				figSize.width += ValueSize.width * 0.5;		// 1/2 the first Value
			}
			else if (ShowTickMajor)
			{
				figSize.width += MajorTicks[0].Thickness;
			}
		}
		else if (Orientation == Vertical)
		{
			// Calculate width
			if (ShowTickMajor) { figSize.width += MajorTicks[0].Length * 1.05; }			// 1/2 Major ticklength + 0.05 offset		
			else if (ShowTickMinor) { figSize.width += MinorTicks[0].Length * 1.05; }		// 1/2 Minor ticklength + 0.05 offset
			else { figSize.width += Axii.Thickness * 1.05; }								// Only line width  + 0.05 offset
			if (ShowLabel)
			{
				LabelSize = cv::getTextSize(AxisLabel.Text.str(), AxisLabel.Font, AxisLabel.Scale, AxisLabel.Thickness, &baseline);
				figSize.width += (LabelSize.width + AxisLabel.Thickness * 1.5) * 1.05; // + offset 0.05
			}
			if (ShowValues)
			{
				ValueSize = cv::getTextSize(Values[0].Text.str(), Values[0].Font, Values[0].Scale, Values[0].Thickness, &baseline);
				figSize.width += ValueSize.width; 
			}

			// Calculate height
			figSize.height += Axii.Length;
			if (ShowValues)
			{
				figSize.height += ValueSize.height * 0.5;		// 1/2 the last Value
				ValueSize = cv::getTextSize(Values[Values.size() - 1].Text.str(), Values[Values.size() - 1].Font, Values[Values.size() - 1].Scale, Values[Values.size() - 1].Thickness, &baseline);
				figSize.height += ValueSize.height * 0.5;		// 1/2 the first Value
			}
			else if (ShowTickMajor)
			{
				figSize.height += MajorTicks[0].Thickness;
			}

		}
		else { throw std::logic_error("Not Implemented Exception!"); }
		return cv::Size();
	}

}