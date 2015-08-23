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
#include "soilanalyzertypes.h"

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

  Particle::ParticleVector_t
      ParticlePopulation; /*!< the individual particles of the sample*/

  SoilMath::PSD PSD; /*!< The Particle Size Distribution*/
  ucharStat_t Roundness;
  ucharStat_t Angularity;
  floatStat_t RI; /*!< The statistical Redness Index data*/

  void Save(const std::string &filename);
  void Load(const std::string &filename);

  Particle::PSDVector_t *GetPSDVector();
  Particle::ClassVector_t *GetRoundnessVector();
  Particle::ClassVector_t *GetAngularityVector();
  Particle::doubleVector_t *GetCIELab_aVector();
  Particle::doubleVector_t *GetCIELab_bVector();
  Particle::floatVector_t *GetRI_vector();

  bool isPreparedForAnalysis =
      false; /*!< is the sample ready for analysis, are all the particles
                extracted*/
  bool isAnalysed = false; /*!< is the sample analyzed*/

  bool ChangesSinceLastSave = false;
  bool ParticleChangedStatePSD = false;
  bool ParticleChangedStateClass = false;
  bool ParticleChangedStateRoundness = false;
  bool ParticleChangedStateAngularity = false;
  bool ColorChange = false;

  bool IsLoadedFromDisk = false;

private:
   Particle::PSDVector_t Diameter;     /*!< The PSD raw data*/
  bool PSDGathered = false; /*!< is the raw data gathered*/
    Particle::ClassVector_t RoundnessVec;
  bool RoundnessGathered = false;
    Particle::ClassVector_t AngularityVec;
  bool AngularityGathered = false;
    Particle::doubleVector_t CIELab_aVec;
  bool CIELab_aGathered = false;
    Particle::doubleVector_t CIELab_bVec;
  bool CIELab_bGathered = false;
    Particle::floatVector_t RIVec;
  bool RIGathered = false;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &ID;
    ar &Location;
    ar &Name;
    ar &ParticlePopulation;
    ar &Diameter;
    ar &RoundnessVec;
    ar &AngularityVec;
    ar &PSD;
    ar &Roundness;
    ar &Angularity;
    ar &RI;
    ar &isPreparedForAnalysis;
    ar &isAnalysed;
    ar &ChangesSinceLastSave;
    ar &ParticleChangedStatePSD;
    ar &ParticleChangedStateClass;
    ar &ParticleChangedStateAngularity;
    ar &ParticleChangedStateRoundness;
    ar &PSDGathered;
    ar &RoundnessGathered;
    ar &AngularityGathered;
    ar &IsLoadedFromDisk;
    if (version > 0) {
      ar &Longtitude;
      ar &Latitude;
      ar &Date;
      ar &Depth;
      ar &AngularityVec;
      ar &AngularityGathered;
      ar &CIELab_aVec;
      ar &CIELab_aGathered;
      ar &CIELab_bVec;
      ar &CIELab_bGathered;
      ar &RIVec;
      ar &RIGathered;
      ar &ColorChange;
    } else {
      Latitude = 51.8849149;
      Longtitude = 4.629618299999947;
      Date = "01-10-2015";
      Depth = 0;
      CIELab_aVec =   Particle::doubleVector_t();
      CIELab_aGathered = false;
      CIELab_bVec =   Particle::doubleVector_t();
      CIELab_bGathered = false;
      RIVec =   Particle::floatVector_t();
      RIGathered = false;
      ColorChange = false;
    }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Sample, 1)
