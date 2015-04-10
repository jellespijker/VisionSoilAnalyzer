#include "Label.h"

namespace SoilPlot
{
	Label::Label()
	{
		this->Font = cv::FONT_HERSHEY_PLAIN;
		this->Scale = 1;
		this->BackgroundBlend = 1.;
	}

	Label::Label(const Label & rhs) : DrawFigure(rhs)
	{
		this->Font = rhs.Font;
		this->Text << rhs.Text;
		this->Scale = rhs.Scale;
		this->BackgroundBlend = rhs.BackgroundBlend;
	}

	Label & Label::operator=(const Label & rhs)
	{
		if (&rhs != this)
		{
			this->Text.clear();
			DrawFigure::operator=(rhs);
			this->Font = rhs.Font;
			this->Text << rhs.Text;
			this->Scale = rhs.Scale;
			this->BackgroundBlend = rhs.BackgroundBlend;
		}
		return *this;
	}

	cv::Mat Label::Draw()
	{
		int baseline = 0;
		cv::Size textSize = cv::getTextSize(Text.str(), Font, Scale, Thickness, &baseline);
		textSize.height += Thickness + (Thickness / 2);
		Figure.create(textSize, CV_8UC4);
		baseline += Thickness;
		Figure.setTo(FillColor);
		cv::putText(Figure, Text.str(), cv::Point((Thickness / 2), (Figure.rows + 1 - (Thickness / 2))), Font, Scale, EdgeColor, Thickness, 8, false);
		return Figure;
	}
}