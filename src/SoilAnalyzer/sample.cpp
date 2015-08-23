/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "sample.h"
#include "particle.h"

namespace SoilAnalyzer {
namespace io = boost::iostreams;

/*!
 * \brief Sample::Sample
 */
Sample::Sample() {}

/*!
 * \brief Sample::Save
 * \param filename
 */
void Sample::Save(const std::string &filename) {
  std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary);
  {
    io::filtering_streambuf<io::output> out;

    out.push(io::zlib_compressor(io::zlib::best_compression));
    out.push(ofs);
    {
      boost::archive::binary_oarchive oa(out);
      oa << boost::serialization::make_nvp("Sample", *this);
    }
  }
  ofs.close();
}

/*!
 * \brief Sample::Load
 * \param filename
 */
void Sample::Load(const std::string &filename) {
  std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
  {
    io::filtering_streambuf<io::input> in;

    in.push(io::zlib_decompressor());
    in.push(ifs);
    {
      boost::archive::binary_iarchive ia(in);
      ia >> boost::serialization::make_nvp("Sample", *this);
    }
  }
  ifs.close();
}

/*!
 * \brief Sample::GetPSDVector
 * \return
 */
Particle::PSDVector_t *Sample::GetPSDVector() {
  if (!PSDGathered || ParticleChangedStatePSD) {
    Diameter.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { Diameter.push_back(P.GetSiDiameter()); });
    PSDGathered = true;
    ParticleChangedStatePSD = false;
  }
  return &Diameter;
}

Particle::ClassVector_t *Sample::GetAngularityVector() {
  if (!AngularityGathered || ParticleChangedStateAngularity) {
    AngularityVec.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { AngularityVec.push_back(P.GetAngularity()); });
    AngularityGathered = true;
    ParticleChangedStateAngularity = false;
  }
  return &AngularityVec;
}

Particle::ClassVector_t *Sample::GetRoundnessVector() {
  if (!RoundnessGathered || ParticleChangedStateRoundness) {
    RoundnessVec.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { RoundnessVec.push_back(P.GetRoundness()); });
    RoundnessGathered = true;
    ParticleChangedStateRoundness = false;
  }
  return &RoundnessVec;
}

Particle::doubleVector_t *Sample::GetCIELab_aVector() {
  if (!CIELab_aGathered || ColorChange) {
    CIELab_aVec.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { CIELab_aVec.push_back(P.getMeanLab().a); });
    CIELab_aGathered = true;
  }
  return &CIELab_aVec;
}

Particle::doubleVector_t *Sample::GetCIELab_bVector() {
  if (!CIELab_bGathered || ColorChange) {
    CIELab_bVec.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { CIELab_bVec.push_back(P.getMeanLab().b); });
    CIELab_bGathered = true;
  }
  return &CIELab_bVec;
}
}
