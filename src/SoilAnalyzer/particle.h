/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */
#pragma once

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

  uint32_t ID; /*!< The particle ID*/

  cv::Mat BW; /*!< The binary image of the particle*/
  cv::Mat Edge; /*!< The binary edge image of the particle*/
  cv::Mat RGB; /*!< The RGB image of the particle*/

  std::vector<Complex_t> FFDescriptors; /*!< The Fast Fourier Descriptors describing the contour in the Frequency domain*/
  Predict_t Classification; /*!< The classification prediction*/
  float SIPixelFactor = 1.0; /*!< The conversion factor from pixel to SI*/
  uint32_t PixelArea = 0; /*!< The total area of the binary image*/

  float GetSIVolume();

  void Save(const std::string &filename);
  void Load(const std::string &filename);

  bool isPreparedForAnalysis = false; /*!< is the particle ready for analysis*/
  bool isAnalysed = false; /*!< is the particle analyzed*/
  bool isSmall = false;

private:
  float SIVolume = 0.; /*!< The correspondening SI volume*/

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
      ar &isPreparedForAnalysis;
      ar &isAnalysed;
    }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Particle, 0)
