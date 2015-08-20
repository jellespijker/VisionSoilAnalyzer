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
#include "psd.h"
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

  uint32_t ID;          /*!< The sample ID*/
  std::string Location; /*!< The Location where the sample was taken*/
  double Longtitude = 4.629618299999947;
  double Latitude = 51.8849149;
  double Depth = 0;
  std::string Date = "01-09-2015";
  std::string Name; /*!< The sample name identifier*/

  typedef std::vector<Particle>
      ParticleVector_t; /*!< a vector consisting of individual particles*/
  typedef std::vector<double> PSDVector_t; /*!< a vector used in the PSD*/
  typedef std::vector<uint8_t>
      ClassVector_t; /*!< a vector used in the classification histogram*/

  ParticleVector_t
      ParticlePopulation; /*!< the individual particles of the sample*/

  SoilMath::PSD PSD; /*!< The Particle Size Distribution*/
  ucharStat_t Roundness;
  ucharStat_t Angularity;
  // ucharStat_t Shape; /*!< The Shape classification distribution*/
  floatStat_t CIELab; /*!< The statistical CIE Lab color data*/
  floatStat_t RI;     /*!< The statistical Redness Index data*/

  void Save(const std::string &filename);
  void Load(const std::string &filename);

  PSDVector_t *GetPSDVector();
  ClassVector_t *GetClassVector();
  ClassVector_t *GetRoundnessVector();
  ClassVector_t *GetAngularityVector();

  bool isPreparedForAnalysis =
      false; /*!< is the sample ready for analysis, are all the particles
                extracted*/
  bool isAnalysed = false; /*!< is the sample analyzed*/

  bool ChangesSinceLastSave = false;
  bool ParticleChangedStatePSD = false;
  bool ParticleChangedStateClass = false;
  bool ParticleChangedStateRoundness = false;
  bool ParticleChangedStateAngularity = false;

  bool IsLoadedFromDisk = false;

private:
  PSDVector_t Diameter;       /*!< The PSD raw data*/
  bool PSDGathered = false;   /*!< is the raw data gathered*/
  ClassVector_t Class;        /*!< The classification raw data*/
  bool ClassGathered = false; /*!< is the classification data gathered*/
  ClassVector_t RoundnessVec;
  bool RoundnessGathered = false;
  ClassVector_t AngularityVec;
  bool AngularityGathered = false;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &ID;
    ar &Location;
    ar &Name;
    ar &ParticlePopulation;
    ar &Diameter;
    ar &Class;
    ar &RoundnessVec;
    ar &AngularityVec;
    ar &PSD;
    ar &Roundness;
    ar &Angularity;
    ar &CIELab;
    ar &RI;
    ar &isPreparedForAnalysis;
    ar &isAnalysed;
    ar &ChangesSinceLastSave;
    ar &ParticleChangedStatePSD;
    ar &ParticleChangedStateClass;
    ar &ParticleChangedStateAngularity;
    ar &ParticleChangedStateRoundness;
    ar &PSDGathered;
    ar &ClassGathered;
    ar &RoundnessGathered;
    ar &AngularityGathered;
    ar &IsLoadedFromDisk;
    if (version > 0) {
        ar &Longtitude;
        ar &Latitude;
        ar &Date;
        ar &Depth;
      }
    else {
        Latitude = 51.8849149;
        Longtitude = 4.629618299999947;
        Date = "01-10-2015";
        Depth = 0;
      }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Sample, 1)
