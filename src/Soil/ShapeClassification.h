/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/serialization/vector.hpp>

#include "../SoilMath/SoilMath.h"

/*!
 * \brief The ShapeClassification class the class which describes the shape as
 * category and with FFT descriptor
 */
class ShapeClassification {
public:
  unsigned char Category;        /**< The category class*/
  ComplexVect_t FFT_descriptors; /**< The Fast Fourier Descriptors*/

  /*!
   * \brief ShapeClassification the constructor
   */
  ShapeClassification();

  /*!
   * \brief ShapeClassification the constructor
   * \param fft_descriptors teh fast fourier descriptors
   */
  ShapeClassification(ComplexVect_t fft_descriptors)
      : FFT_descriptors(fft_descriptors) {}

  /*!
    * \brief The descontructor
    */
  ~ShapeClassification();

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version __attribute__((unused))) {
    ar &Category;
    ar &FFT_descriptors;
  }
};
