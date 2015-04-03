#include "Particle.h"

namespace SoilAnalyzer
{
	Particle::Particle()
	{
	}


	Particle::~Particle()
	{
	}

	void Particle::Save(std::string &filename)
	{
		std::ofstream ofs(filename.c_str());
		boost::archive::binary_oarchive oa(ofs);
		oa << BOOST_SERIALIZATION_NVP(*this);
	}

	void Particle::Load(std::string &filename)
	{
		std::ifstream ifs(filename.c_str());
		boost::archive::binary_iarchive ia(ifs);
		ia >> BOOST_SERIALIZATION_NVP(*this);
	}

	SoilAnalyzer::AnalysisResults Particle::Analyze(SoilMath::NN &nn)
	{
		if (Analysis.Analyzed) { return Analysis; }

		// Calc the LAB stats
		std::vector<cv::Mat> lab = Vision::ImageProcessing::extractChannel(LAB);
		for_each(lab.begin(), lab.end(), [&](cv::Mat &M)
		{
			Analysis.LAB_Stat.push_back(floatStat_t((float *)M.data, M.rows, M.cols));
		});

		// Calc the RGB stats
		std::vector<cv::Mat> rgb = Vision::ImageProcessing::extractChannel(RGB);
		for_each(lab.begin(), lab.end(), [&](cv::Mat &M)
		{
			Analysis.RGB_Stat.push_back(ucharStat_t((uchar *)M.data, M.rows, M.cols));
		});

		// Calc the Redness Index stats
		Analysis.RI_Stat = floatStat_t((float *)RI.data, RI.rows, RI.cols);

		// Calc the area
		for_each(BW.data, BW.data + (BW.rows * BW.cols), [&](uchar P) {Analysis.Area += P; });

		// Calc the edge area
		uint32_t edgeArea = 0;
		for_each(Edge.data, Edge.data + (Edge.rows * Edge.cols), [&](uchar P) {edgeArea += P; });

		if (edgeArea >= MIN_EDGE_PIXELS) { Analysis.SmallParticle = false; }
		else { Analysis.SmallParticle = true; }

		// Determine the shape Classifiation, but only for pixels that are big enough
		if (!Analysis.SmallParticle)
		{
			// Calculate the FFT Descriptors 
			SoilMath::FFT fft;
			Analysis.Shape.FFT_descriptors = fft.GetDescriptors(Edge);
			if (Analysis.Shape.FFT_descriptors.size() >= FFT_DESCRIPTORS)
			{
				Analysis.Shape.FFT_descriptors.erase(Analysis.Shape.FFT_descriptors.begin() + FFT_DESCRIPTORS, Analysis.Shape.FFT_descriptors.end());
			}
			else
			{
				while (Analysis.Shape.FFT_descriptors.size() < FFT_DESCRIPTORS)
				{
					Analysis.Shape.FFT_descriptors.push_back(Complex_t(0, 0));
				}
			}

			Predict_t sp = nn.Predict(Analysis.Shape.FFT_descriptors);
			uchar i = 0;
			float maxValue = 0.0;
			for_each(sp.OutputNeurons.begin(), sp.OutputNeurons.end(), [&](float &n)
			{
				if (n > maxValue)
				{
					Analysis.Shape.Category = i;
					maxValue = n;
				}
				i++;
			});
		}
		Analysis.Analyzed = true;
		return Analysis;
	}
}