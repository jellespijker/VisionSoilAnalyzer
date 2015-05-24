#include "Label.h"

namespace SoilPlot
{
	Label::Label()
	{
	}

	Label::Label(const Label & rhs) : DrawFigure(rhs)
	{
		this->Text =rhs.Text;
		this->Bold = rhs.Bold;
		this->FontFace =rhs.FontFace;
		this->FontSize = rhs.FontSize;
		this->Italic = rhs.Italic;
        this->Orientation = Orientation_enum::Horizontal;
	}
	
	Label::Label(std::string const& text, std::string const& fontFace, double fontSize, cv::Scalar edgecolor, cv::Scalar fillcolor, bool fontItalic, bool fontBold)
	{
		this->Text = text;
		this->EdgeColor = edgecolor;
		this->FillColor = fillcolor;
		this->Bold = fontBold;
        this->FontFace = fontFace;
        this->FontSize = fontSize;
        this->Orientation = Orientation_enum::Horizontal;
	}

	Label & Label::operator=(const Label & rhs)
	{
		if (&rhs != this)
		{
			this->Text.clear();
			DrawFigure::operator=(rhs);
			this->Text = rhs.Text;
			this->Bold = rhs.Bold;
			this->FontFace = rhs.FontFace;
			this->FontSize = rhs.FontSize;
			this->Italic = rhs.Italic;
			}
		return *this;
	}

	cv::Mat Label::Draw()
	{
		putTextCairo(Figure, Text, FontFace, FontSize, EdgeColor, Italic, Bold); 
		return Figure;
	}
	
	// Source: http://stackoverflow.com/questions/11917124/opencv-how-to-use-other-font-than-hershey-with-cvputtext-like-arial	
	// Alterted by Jelle Spijker 2015
	void Label::putTextCairo(cv::Mat &targetImage, std::string const& text, std::string const& fontFace, double fontSize, cv::Scalar textColor, bool fontItalic, bool fontBold)
	{
	    // Create Cairo
		cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 2000, 100);
		cairo_t* cairo = cairo_create(surface);

		// Wrap Cairo with a Mat
		cv::Mat cairoTarget(cairo_image_surface_get_height(surface), cairo_image_surface_get_width(surface), CV_8UC4, cairo_image_surface_get_data(surface), cairo_image_surface_get_stride(surface)); 

		 // Set font and write text
		cairo_select_font_face( cairo, fontFace.c_str(), fontItalic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL, fontBold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL); 
		cairo_set_font_size(cairo, fontSize);
        cairo_set_source_rgba(cairo, textColor[0], textColor[1], textColor[2], textColor[3]);

		cairo_text_extents_t extents;
		cairo_text_extents(cairo, text.c_str(), &extents);
		
		cairo_move_to(cairo, 0, extents.height);
		cairo_show_text(cairo, text.c_str());

	    // Copy the data to the output image
        cv::Mat tImg(cairo_image_surface_get_height(surface), cairo_image_surface_get_width(surface), CV_8UC4, cairo_image_surface_get_data(surface));
        cv::Rect ROI(cv::Point(0, 0), cv::Point(extents.width, extents.height));
        targetImage = tImg(ROI).clone();

		cairo_destroy(cairo);
		cairo_surface_destroy(surface);
	}
}
