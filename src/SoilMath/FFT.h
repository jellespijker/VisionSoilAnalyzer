//*! Fast Fourier Transform class
/*!
 * Use this class to transform a black and white blob presented as a cv::Mat
 * with values
 * 0 or 1 to a vector of complex values representing the Fourier Descriptors.
*/
#pragma once

#include <vector>
#include <complex>
#include <cmath>
#include <valarray>
#include <array>
#include <deque>
#include <queue>
#include <iterator>
#include <algorithm>
#include <stdint.h>
#include <opencv2/core.hpp>
#include "SoilMathTypes.h"
#include "MathException.h"

namespace SoilMath {
class FFT {
public:
  /*!
 * \brief Standard constructor
 */
  FFT();

  /*!
  * \brief Standard deconstructor
  */
 ~FFT();

  /*!
   * \brief Transforming the img to the frequency domain and returning the
   * Fourier Descriptors
   * \param img contour img in the form of a cv::Mat type CV_8UC1. Which should
   * consist of a continous contour. \f$ \{ img \in \mathbb{Z} | 0 \leq img \leq 1 \} \f$
   * \return a vector with complex values, represing the contour in the frequency domain, expressed as Fourier Descriptors
   */
  ComplexVect_t GetDescriptors(const cv::Mat &img);

private:
  ComplexVect_t
      fftDescriptors; /**< Vector with complex values which represent the
                         descriptors*/
  ComplexVect_t
      complexcontour; /**< Vector with complex values which represent the
                         contour*/
  cv::Mat Img;        /**< Img which will be analysed*/

  /*!
   * \brief Contour2Complex
   * \param img
   * \param centerCol
   * \param centerRow
   * \return
   */
  ComplexVect_t Contour2Complex(const cv::Mat &img, float centerCol,
                                float centerRow);
  /*!
   * \brief Neighbors
   * \param O
   * \param pixel
   * \param columns
   * \param rows
   * \return
   */
  iContour_t Neighbors(uchar *O, int pixel, uint32_t columns, uint32_t rows);

  void fft(ComplexArray_t &CA);
  void ifft(ComplexArray_t &CA);
};
}
