/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include <opencv2/core.hpp>
#include <stdint.h>
#include <vector>
#include "SoilMath.h"
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include "Mat_archive.h"
#include "zlib.h"
#include "soilanalyzerexception.h"

namespace SoilAnalyzer {
class Particle {
public:
  Particle();

  uint32_t ID;

  cv::Mat BW;
  cv::Mat Edge;
  cv::Mat RGB;

  std::vector<Complex_t> FFDescriptors;
  Predict_t Classification;
  float SIPixelFactor = 1.0;
  uint32_t PixelArea = 0;

  float GetSIVolume();

  void Save(const std::string &filename);
  void Load(const std::string &filename);

private:
  float SIVolume = 0.;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version == 0) {
      ar &ID;
      ar &BW;
      ar &Edge;
      ar &RGB;
      ar &FFDescriptors;
      ar &Classification;
      ar &SIPixelFactor;
      ar &PixelArea;
      ar &SIVolume;
    }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Particle, 0)
#endif // PARTICLE_H
