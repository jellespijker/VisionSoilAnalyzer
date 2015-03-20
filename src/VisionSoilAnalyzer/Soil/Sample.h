#pragma once
#include "Soil.h"
#include "Particle.h"
#include "AnalysisResults.h"
#include "AnalysisException.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include "Mat_archive.h"

#include <opencv2/core.hpp>
#include <vector>
#include <string>

#include "../Vision/Vision.h"

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
		AnalysisResults Results;

		void Analyse();
		void Analyse(const Mat& src);
		void Save(string &filename);
		void Load(string &filename);

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Soil);
			ar & OriginalImage;
			ar & Population;
			ar & Results;
		}

		bool AnalysePopVect(const vector<Particle>& population, AnalysisResults& results);
		std::vector<Particle> SegmentParticles(Vision::Segment::SegmentationType segType = Vision::Segment::Normal);
	};
}
