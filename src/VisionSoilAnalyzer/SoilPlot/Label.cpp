#include "Label.h"

namespace SoilPlot
{
	cv::Mat Label::Draw()
	{
		int baseline = 0;
		cv::Size textSize = cv::getTextSize(Text, Font,	1, Thickness, &baseline);
		baseline += Thickness;
		Figure = cv::Mat(textSize, CV_8UC4, 0);
		
		cv::putText(Figure, Text, cv::Point(0,0), Font, 1, FillColor, Thickness);
		return Figure;
	}
}