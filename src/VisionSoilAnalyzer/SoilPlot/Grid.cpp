#include "Grid.h"


namespace SoilPlot
{
	Grid::Grid()
	{
	}

	Grid::Grid(const Grid & rhs) : DrawFigure(rhs)
	{
	
	}

	Grid & Grid::operator=(const Grid & rhs)
	{
		if (&rhs != this)
		{
			DrawFigure::operator=(rhs);
		}
		return *this;
	}

	cv::Mat Grid::Draw()
	{
		return Figure;
	}

	Grid::~Grid()
	{
	}
}