#include "Graph.h"

namespace SoilPlot
{
	Graph::Graph(Graph &lhs)
	{
		lhs.Data = this->Data;
		lhs.Env = this->Env;
		lhs.Figure = this->Figure;
		lhs.Type = this->Type;
	}

	Graph::Graph(uint32_t * data, cv::Size size)
	{
		this->Data = &data[0];
		this->Figure = cv::Mat(size, CV_8UC4);
	}

	Graph::Graph(uint32_t * data, cv::Size size, GraphType_enum type)
	{
	}

	Graph & Graph::operator=(const Graph & rhs)
	{
		this->Data = &rhs.Data[0];
		this->Env = rhs.Env;
		this->Figure = rhs.Figure;
		this->Type = rhs.Type;
		return *this;
	}

	Graph::~Graph()
	{
	}

	cv::Mat Graph::Draw()
	{
		Env.Draw();
		Env.DrawOnTop(this->Figure, Env.TopLeftCorner);
		return Figure;
	}

}