/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include "stdint.h"
#include <vector>
#include <string>
#include "Stats.h"
#include "particle.h"
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include "zlib.h"

namespace SoilAnalyzer {
class Sample {
public:
  Sample();

  uint32_t ID; /*!< The sample ID*/
  std::string Location; /*!< The Location where the sample was taken*/
  std::string Name; /*!< The sample name identifier*/

  typedef std::vector<Particle> ParticleVector_t; /*!< a vector consisting of individual particles*/
  typedef std::vector<float> PSDVector_t; /*!< a vector used in the PSD*/
  typedef std::vector<uint8_t> ClassVector_t; /*!< a vector used in the classification histogram*/

  ParticleVector_t ParticlePopulation; /*!< the individual particles of the sample*/

  floatStat_t PSD; /*!< The Particle Size Distribution*/
  ucharStat_t Shape; /*!< The Shape classification distribution*/
  floatStat_t CIELab; /*!< The statistical CIE Lab color data*/
  floatStat_t RI; /*!< The statistical Redness Index data*/

  void Save(const std::string &filename);
  void Load(const std::string &filename);

  PSDVector_t *GetPSDVector();
  ClassVector_t *GetClassVector();

  bool isPreparedForAnalysis = false; /*!< is the sample ready for analysis, are all the particles extracted*/
  bool isAnalysed = false; /*!< is the sample analyzed*/

  bool ChangesSinceLastSave = false;
private:
  PSDVector_t Volume; /*!< The PSD raw data*/
  bool PSDGathered = false; /*!< is the raw data gathered*/
  ClassVector_t Class; /*!< The classification raw data*/
  bool ClassGathered = false; /*!< is the classification data gathered*/

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version >= 0) {
      ar &ID;
      ar &Location;
      ar &Name;
      ar &ParticlePopulation;
      ar &Volume;
      ar &Class;
      ar &PSD;
      ar &Shape;
      ar &CIELab;
      ar &RI;
      ar &isPreparedForAnalysis;
      ar &isAnalysed;
    }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Sample, 0)
