/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "particle.h"

namespace SoilAnalyzer {
namespace io = boost::iostreams;

Particle::Particle() {}

/*!
 * \brief Particle::Save
 * \param filename
 */
void Particle::Save(const std::string &filename) {
  std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary);
  {
    io::filtering_streambuf<io::output> out;

    out.push(io::zlib_compressor(io::zlib::best_compression));
    out.push(ofs);
    {
      boost::archive::binary_oarchive oa(out);
      oa << boost::serialization::make_nvp("Particle", *this);
    }
  }
  ofs.close();
}

/*!
 * \brief Particle::Load
 * \param filename
 */
void Particle::Load(const std::string &filename) {
  std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
  {
    io::filtering_streambuf<io::input> in;

    in.push(io::zlib_decompressor());
    in.push(ifs);
    {
      boost::archive::binary_iarchive ia(in);
      ia >> boost::serialization::make_nvp("Particle", *this);
    }
  }
  ifs.close();
}

/*!
 * \brief Particle::GetSIVolume
 * \return
 */
float Particle::GetSIVolume() {
  if (SIVolume == 0.) {
    if (PixelArea == 0) {
      throw Exception::SoilAnalyzerException(
          EXCEPTION_PARTICLE_NOT_ANALYZED, EXCEPTION_PARTICLE_NOT_ANALYZED_NR);
    }
    SIVolume = SoilMath::calcVolume(PixelArea) * SIPixelFactor * Eccentricty;
  }
  return SIVolume;
}

float Particle::GetSiDiameter() {
  if (SIDiameter == 0.) {
    if (PixelArea == 0) {
      throw Exception::SoilAnalyzerException(
          EXCEPTION_PARTICLE_NOT_ANALYZED, EXCEPTION_PARTICLE_NOT_ANALYZED_NR);
    }
    SIDiameter = SoilMath::calcDiameter(PixelArea) * SIPixelFactor;
  }
  return SIDiameter;
}

uint8_t Particle::GetAngularity() {
  uint8_t angularity = ((Classification.Category - 1) % 6) + 1;
  return angularity;
}

uint8_t Particle::GetRoundness() {
  uint8_t roundness = ((Classification.Category - 1) / 6) + 1;
  return roundness;
}
}
