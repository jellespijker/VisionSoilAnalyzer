#pragma once
#include <sstream>
#include <string>
#include <cairo/cairo.h>
#include <opencv2/highgui.hpp>
#include "DrawFigure.h"

namespace SoilPlot
{
	class Label : 
		public DrawFigure
	{
	public:
		Label();
		Label(const Label &rhs);
		Label(std::string const &text, std::string const& fontFace = "times", double fontSize = 12, cv::Scalar edgecolor = cv::Scalar(255, 255, 255, 255), cv::Scalar fillcolor = cv::Scalar(0, 0, 0, 0), bool fontItalic = false, bool fontBold = false);

		Label &operator=(const Label &rhs);

		~Label() {};

		//std::ostringstream Text;
		std::string Text;
		std::string FontFace;
		double FontSize;
		bool Bold;
		bool Italic;

		cv::Mat Draw();
		
		void putTextCairo(cv::Mat &targetImage, std::string const& text, std::string const& fontFace, double fontSize, cv::Scalar textColor, bool fontItalic, bool fontBold);
	};

}