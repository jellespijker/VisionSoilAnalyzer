/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include <string>
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "../SoilVision/Vision.h"

namespace SoilAnalyzer {
/*!
 * \brief The SoilSettings class
 * \details A class with which the used settings can easily transfered to setup
 * the Sample class in one go. This class is also used in the GUI. and has a
 * possibility to saved to disk as a serialized object
 */
class SoilSettings {
public:
  SoilSettings();

  /*!
   * \brief SaveSettings a function to save the settings to disk
   * \param filename a string with the filename
   */
  void SaveSettings(std::string filename);

  /*!
   * \brief LoadSettings a function to load the settings from disk
   * \param filename a string with the filename
   */
  void LoadSettings(std::string filename);

  bool useAdaptiveContrast =
      true; /**< Should adaptive contrast stretch be used default is true*/
  uint32_t adaptContrastKernelSize =
      9; /**< The size of the adaptive contrast kernelsize*/
  float adaptContrastKernelFactor = 1.; /**< the factor with which to multiply
                                           the effect of the adaptive contrast
                                           stretch*/

  bool useBlur = true; /**< Should the mediaan blur be used during analsyis*/
  uint32_t blurKernelSize = 5; /**< the median blurkernel*/

  Vision::Segment::TypeOfObjects typeOfObjectsSegmented =
      Vision::Segment::Dark; /**< Which type of object should be segmented*/
  bool ignorePartialBorderParticles =
      true; /**< Indication of partial border particles should be used*/
  bool fillHoles = true; /**< should the holes be filled*/
  float
      sigmaFactor = 2; /**< The sigma factor or the bandwidth indicating which
                          pixel intensity values count belong to an object*/
  int thresholdOffsetValue = 0; /**< an tweaking offset value*/

  Vision::MorphologicalFilter::FilterType morphFilterType =
      Vision::MorphologicalFilter::OPEN; /**< Indicating which type of
                                            morhpological filter should be
                                            used*/
  uint32_t filterMaskSize = 5;           /**< the filter mask*/

  uint32_t HDRframes =
      5; /**< The number of frames which should be used for the HDR image*/
  float lightLevel = 0.5; /**< The light level of the environmental case*/
  bool encInv = false;    /**< invert the values gained form the encoder*/
  bool enableRainbow =
      true; /**< run a rainbow loop on the RGB encoder during analysis*/

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version __attribute__((unused))) {
    ar &BOOST_SERIALIZATION_NVP(useAdaptiveContrast);
    ar &BOOST_SERIALIZATION_NVP(adaptContrastKernelFactor);
    ar &BOOST_SERIALIZATION_NVP(adaptContrastKernelSize);
    ar &BOOST_SERIALIZATION_NVP(useBlur);
    ar &BOOST_SERIALIZATION_NVP(blurKernelSize);
    ar &BOOST_SERIALIZATION_NVP(typeOfObjectsSegmented);
    ar &BOOST_SERIALIZATION_NVP(ignorePartialBorderParticles);
    ar &BOOST_SERIALIZATION_NVP(fillHoles);
    ar &BOOST_SERIALIZATION_NVP(sigmaFactor);
    ar &BOOST_SERIALIZATION_NVP(morphFilterType);
    ar &BOOST_SERIALIZATION_NVP(filterMaskSize);
    ar &BOOST_SERIALIZATION_NVP(thresholdOffsetValue);
    ar &BOOST_SERIALIZATION_NVP(HDRframes);
    ar &BOOST_SERIALIZATION_NVP(lightLevel);
    ar &BOOST_SERIALIZATION_NVP(encInv);
    ar &BOOST_SERIALIZATION_NVP(enableRainbow);
  }
};
}
