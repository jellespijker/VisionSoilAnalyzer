#pragma once
#define PROG_INCR(status) currentProg += progstep; prog_sig(currentProg, status)
#include "Soil.h"
#include "Particle.h"
#include "AnalysisResults.h"
#include "AnalysisException.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include "Mat_archive.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <string>

#include "../Vision/Vision.h"
#include "../SoilMath/SoilMath.h"
#include "soilsettings.h"

using namespace std;
using namespace cv;

namespace SoilAnalyzer
{
	class Sample :
		public Soil
	{
	public:
        typedef boost::signals2::signal<void (float, std::string)> Progress_t;
        boost::signals2::connection connect_Progress(const Progress_t::slot_type &subscriber);

        boost::signals2::connection ImageProcessing::connect_Progress(const Progress_t::slot_type &subscriber)
        {
            return prog_sig.connect(subscriber);
        }

        Sample(SoilSettings *settings = nullptr);
        Sample(const Mat& src, SoilSettings *settings = nullptr);
		~Sample();

		cv::Mat OriginalImage;
		vector<Particle> Population;
		AnalysisResults Results;

        SoilSettings *Settings = nullptr;

        void PrepImg(SoilSettings *settings = nullptr);
        bool imgPrepped = false;

        void Analyse(SoilMath::NN &nn);
		void Analyse(const Mat& src, SoilMath::NN &nn);
		void Save(string &filename);
		void Load(string &filename);

	private:
        Progress_t prog_sig;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Soil);
			ar & OriginalImage;
			ar & Population;
			ar & Results;
            ar & Settings;
            ar & imgPrepped;
		}
		bool AnalysePopVect(const vector<Particle>& population, AnalysisResults& results);
		void SegmentParticles(Vision::Segment::SegmentationType segType = Vision::Segment::Normal);
	};
}
