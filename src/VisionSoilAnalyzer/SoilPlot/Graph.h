#pragma once
#include <opencv2/core.hpp>

#include "Environment.h"

namespace SoilPlot
{
	class Graph 
	{
	public:
		enum GraphType_enum
		{
			Bar,
			Point,
			Line
		};
		Environment Env;
		GraphType_enum Type;
		cv::Mat Figure;

		Graph(Graph &lhs);
		Graph() {};
		Graph(cv::Size size) : Figure(size, CV_8UC4) {};
		Graph(uint32_t *data, cv::Size size);
		Graph(uint32_t *data, cv::Size size, GraphType_enum type);

		Graph &operator=(const Graph &rhs);
		~Graph();

		cv::Mat Draw();

	private:
		uint32_t *Data;

	};
}
