/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include "Mat_archive.h"
#include <opencv2/core/core.hpp>
#include <stdint.h>
#include <string>
#include "../SoilVision/VisionDebug.h"
#include <zlib.h>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

namespace SoilAnalyzer {
/*!
 * \brief The Soil class
 * \details The parent object of the Soil related objects
 */
class Soil {
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version __attribute__((unused))) {
    ar &ID;
    ar &Location;
    ar &TimeTaken;
    ar &TimeAnalyzed;
    ar &BW;
    ar &Intensity;
    ar &LAB;
    ar &RI;
    ar &RGB;
  }

public:
  /*!
   * \brief Soil the constructor
   */
  Soil();

  /*!
    * \brief Soil deconstructor
    */
  ~Soil();
  cv::Mat BW; /**< The black and white image consisting of values of 0 and 1
                 where 0 is the bacground*/
  cv::Mat
      Intensity;   /**< The intensity image after it is converted from the RGB
                      color model*/
  cv::Mat LAB;     /**< The CIE Lab color image*/
  cv::Mat RGB;     /**< The RGB color image*/
  cv::Mat RI;      /**< The individual RI image*/
  cv::Mat Edge;    /**< The black and white image consiting of values of 0 and 1
                      where 0 is background and 1 is the edge of the blob*/
  uint8_t version; /**< the version of the object*/
  std::string
      TimeTaken; /**< a string indicating which time the sample was taken*/
  std::string
      TimeAnalyzed;     /**< a string indicating which time it was analyzed*/
  std::string Location; /**< a string with the location of the soilsample*/
  uint32_t ID;          /**< the sample ID*/
};
}
