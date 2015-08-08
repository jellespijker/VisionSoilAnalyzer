/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#ifndef SAMPLE_H
#define SAMPLE_H

#include "stdint.h"
#include <vector>
#include <string>
#include "SoilMath.h"
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

  uint32_t ID;
  std::string Location;
  std::string Name;

  typedef std::vector<Particle> ParticleVector_t;
  typedef std::vector<float> PSDVector_t;
  typedef std::vector<uint8_t> ClassVector_t;

  ParticleVector_t ParticlePopulation;

  floatStat_t PSD;
  uint32Stat_t Shape;
  floatStat_t CIELab;
  floatStat_t RI;

  void Save(const std::string &filename);
  void Load(const std::string &filename);

  PSDVector_t *GetPSDVector();
  ClassVector_t *GetClassVector();

private:
  PSDVector_t Volume;
  bool PSDGathered = false;
  ClassVector_t Class;
  bool ClassGathered = false;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version == 0) {
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
    }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Sample, 0)
#endif // SAMPLE_H
