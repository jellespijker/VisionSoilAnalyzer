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
Sample::PSDVector_t *Sample::GetPSDVector() {
  if (!PSDGathered) {
    Volume.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { Volume.push_back(P.GetSIVolume()); });
    PSDGathered = true;
  }
  return &Volume;
}

/*!
 * \brief Sample::GetClassVector
 * \return
 */
Sample::ClassVector_t *Sample::GetClassVector() {
  if (!ClassGathered) {
    Class.clear();
    for_each(ParticlePopulation.begin(), ParticlePopulation.end(),
             [&](Particle &P) { Class.push_back(P.Classification.Category); });
  }
  return &Class;
}
}
