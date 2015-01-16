#pragma once
#include "Soil.h"
#include "Particle.h"
#include "AnalysisResults.h"
#include "AnalysisException.h"

#include <opencv2/core.hpp>
#include <vector>

using namespace std;
using namespace cv;

namespace SoilAnalyzer
{
	class Sample :
		public Soil
	{
	public:
		Sample();
		Sample(const Mat& src);
		~Sample();

		cv::Mat OriginalImage;
		vector<Particle> Population;
		AnalysisResults Resulst;

		void Analyse();
		void Analyse(const Mat& src);

	private:
		bool AnalysePopVect(const vector<Particle>& population, AnalysisResults& results);
		bool ConvertToBW(const Mat& src, Mat& dst);
		bool ConvertToInt(const Mat& src, Mat& dst);
		bool ConvertToLAB(const Mat& src, Mat& dst);
		bool ConvertToRI(const Mat& src, Mat& dst);
		bool EnhanceImg(const Mat& src, Mat& dst);
		bool SegmentParticles(const Mat& rgb, const Mat& intensity, const Mat& bw, const Mat& lab, const Mat& ri, vector<Particle>& population);
	};
}
