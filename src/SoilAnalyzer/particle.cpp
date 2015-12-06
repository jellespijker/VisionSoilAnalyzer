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
    SIVolume = SoilMath::calcVolume(PixelArea) * SIPixelFactor; // * (Eccentricty/2 + 0.5);
  }
  return SIVolume;
}

float Particle::GetSiDiameter() {
  if (SIDiameter == 0.) {
    if (PixelArea == 0) {
      throw Exception::SoilAnalyzerException(
          EXCEPTION_PARTICLE_NOT_ANALYZED, EXCEPTION_PARTICLE_NOT_ANALYZED_NR);
    }
    SIDiameter = SoilMath::calcDiameter(PixelArea) * SIPixelFactor;  // * (Eccentricty/2 + 0.5);
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

void Particle::SetRoundness() {
  uint8_t ang = GetAngularity() - 1;
  Classification.Category +=
      ang + (static_cast<uint8_t>(floor(Eccentricty / 0.33)) * 6);
  Classification.ManualSet = true;
}

Lab_t Particle::getMeanLab() {
  if (BW.empty() || RGB.empty()) {
    throw SoilAnalyzer::Exception::SoilAnalyzerException(
        EXCEPTION_NO_IMAGES_PRESENT, EXCEPTION_NO_IMAGES_PRESENT_NR);
  }
  if (meanLab.L == 0 && meanLab.a == 0 && meanLab.b == 0) {
    // convert to Lab
    if (LAB.empty()) {
      getLabImg();
    }
    std::vector<cv::Mat> LABvect = Vision::Conversion::extractChannel(LAB);
    std::vector<float> labvect;
    for_each(LABvect.begin(), LABvect.end(), [&](cv::Mat &I) {
      floatStat_t labStat((float *)I.data, I.rows, I.cols, (uchar *)BW.data, 1,
                          0, true);
      labvect.push_back(labStat.Mean);
    });
    meanLab.L = labvect[0];
    meanLab.a = labvect[1];
    meanLab.b = labvect[2];
  }
  return meanLab;
}

float Particle::GetMeanRI() {
  if (BW.empty() || RGB.empty()) {
    throw SoilAnalyzer::Exception::SoilAnalyzerException(
        EXCEPTION_NO_IMAGES_PRESENT, EXCEPTION_NO_IMAGES_PRESENT_NR);
  }
  if (meanRI == 0) {
    if (LAB.empty()) {
      getLabImg();
    }
    Vision::Conversion convertor(LAB);
    convertor.Convert(Vision::Conversion::CIE_lab, Vision::Conversion::RI);
    floatStat_t RIstat((float *)convertor.ProcessedImg.data, LAB.rows, LAB.cols,
                       (uchar *)BW.data, 1, 0, true);
    meanRI = RIstat.Mean;
  }
  return meanRI;
}

void Particle::getLabImg() {
  Vision::Conversion convertor(RGB);
  convertor.Convert(Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
  LAB = convertor.ProcessedImg.clone();
}
}
