#include "Graph.h"

namespace SoilPlot
{
	Graph::Graph(const Graph &rhs)
	{
		this->Data = new uint32_t[rhs.n];
		for (uint32_t i = 0; i < n; i++)
		{
			this->Data[i] = rhs.Data[i];
		}
		this->Env = rhs.Env;
		this->Figure = rhs.Figure.clone();
		this->Type = rhs.Type;
		this->n = rhs.n;
	}

	Graph::Graph()
	{
		this->Data = new uint32_t[1];
	}

	Graph::Graph(uint32_t * data, uint32_t n, cv::Size size)
	{
		this->Data = new uint32_t[n];
		for (uint32_t i = 0; i < n; i++)
		{
			this->Data[i] = data[i];
		}
		this->n = n;
		this->Figure = cv::Mat(size, CV_8UC4);
	}

	Graph::Graph(uint32_t * data, uint32_t n, cv::Size size, GraphType_enum type)
	{
		this->Data = new uint32_t[n];
		for (uint32_t i = 0; i < n; i++)
		{
			this->Data[i] = data[i];
		}
		this->n = n;
		this->Figure = cv::Mat(size, CV_8UC4);
		this->Type = type;
	}

	Graph & Graph::operator=(const Graph & rhs)
	{
		if (&rhs != this)
		{
			delete[] this->Data;
			this->Data = new uint32_t[rhs.n];
			for (uint32_t i = 0; i < n; i++)
			{
				this->Data[i] = rhs.Data[i];
			}
			this->Env = rhs.Env;
			this->Figure = rhs.Figure.clone();
			this->Type = rhs.Type;
			this->n = rhs.n;
		}
		return *this;
	}

	Graph::~Graph()
	{
		delete[] this->Data;
	}

	cv::Mat Graph::Draw()
	{
		if (Figure.rows > 0 && Figure.cols > 0)
		{
			Env.Draw();
			Env.DrawOnTop(this->Figure, Env.TopLeftCorner);
		}
		return Figure;
	}

}