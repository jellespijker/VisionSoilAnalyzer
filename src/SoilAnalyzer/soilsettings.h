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
#include <boost/serialization/version.hpp>
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
      false; /**< Should adaptive contrast stretch be used default is true*/
  uint32_t adaptContrastKernelSize =
      9; /**< The size of the adaptive contrast kernelsize*/
  float adaptContrastKernelFactor = 1.; /**< the factor with which to multiply
                                           the effect of the adaptive contrast
                                           stretch*/

  bool useBlur = false; /**< Should the mediaan blur be used during analsyis*/
  uint32_t blurKernelSize = 5; /**< the median blurkernel*/

  Vision::Segment::TypeOfObjects typeOfObjectsSegmented =
      Vision::Segment::Dark; /**< Which type of object should be segmented*/
  bool ignorePartialBorderParticles =
      true; /**< Indication of partial border particles should be used*/
  bool fillHoles = true; /**< should the holes be filled*/
  float sigmaFactor = 2; /**< The sigma factor or the bandwidth indicating which
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
  bool useBacklightProjection = true;           /*!< use Projection*/
  bool useHDR = false;                          /*!< use HDR*/
  std::string defaultWebcam = "USB Microscope"; /*!< The defaultWebcam string*/
  int Brightness_front = 0;  /*!< cam brightness setting front light*/
  int Brightness_proj = -10; /*!< cam brightness setting projected light*/
  int Contrast_front = 36;   /*!< cam contrast setting front light*/
  int Contrast_proj = 36;    /*!< cam contrast setting projected light*/
  int Saturation_front = 64; /*!< cam saturation setting front light*/
  int Saturation_proj = 0;   /*!< cam saturation setting projected light*/
  int Hue_front = 0;         /*!< cam hue setting front light*/
  int Hue_proj = -40;        /*!< cam hue setting projected light*/
  int Gamma_front = 100;     /*!< cam gamma setting front light*/
  int Gamma_proj = 200;      /*!< cam gamma setting projected light*/
  int PowerLineFrequency_front =
      1; /*!< cam powerline freq setting front light*/
  int PowerLineFrequency_proj =
      1;                    /*!< cam powerline freq setting projected light*/
  int Sharpness_front = 12; /*!< cam sharpness setting front light*/
  int Sharpness_proj = 25;  /*!< cam sharpness setting projected light*/
  int BackLightCompensation_front =
      1; /*!< cam backlight compensation setting front light*/
  int BackLightCompensation_proj =
      1; /*!< cam backlight compensation setting projected light*/
  std::string NNlocation = "NeuralNet/Default.NN";
  bool useCUDA = false; /*!< CUDA enabled*/
  int selectedResolution = 0;
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    if (version >= 0) {
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
      ar &BOOST_SERIALIZATION_NVP(useBacklightProjection);
      ar &BOOST_SERIALIZATION_NVP(useHDR);
      ar &BOOST_SERIALIZATION_NVP(defaultWebcam);
      ar &BOOST_SERIALIZATION_NVP(Brightness_front);
      ar &BOOST_SERIALIZATION_NVP(Brightness_proj);
      ar &BOOST_SERIALIZATION_NVP(Contrast_front);
      ar &BOOST_SERIALIZATION_NVP(Contrast_proj);
      ar &BOOST_SERIALIZATION_NVP(Saturation_front);
      ar &BOOST_SERIALIZATION_NVP(Saturation_proj);
      ar &BOOST_SERIALIZATION_NVP(Hue_front);
      ar &BOOST_SERIALIZATION_NVP(Hue_proj);
      ar &BOOST_SERIALIZATION_NVP(Gamma_front);
      ar &BOOST_SERIALIZATION_NVP(Gamma_proj);
      ar &BOOST_SERIALIZATION_NVP(PowerLineFrequency_front);
      ar &BOOST_SERIALIZATION_NVP(PowerLineFrequency_proj);
      ar &BOOST_SERIALIZATION_NVP(Sharpness_front);
      ar &BOOST_SERIALIZATION_NVP(Sharpness_proj);
      ar &BOOST_SERIALIZATION_NVP(BackLightCompensation_front);
      ar &BOOST_SERIALIZATION_NVP(BackLightCompensation_proj);
      ar &BOOST_SERIALIZATION_NVP(NNlocation);
      ar &BOOST_SERIALIZATION_NVP(useCUDA);
      ar &BOOST_SERIALIZATION_NVP(selectedResolution);
    }
  }
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::SoilSettings, 0)
