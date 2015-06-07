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
   * \param img contour in the form of a cv::Mat type CV_8UC1. Which should
   * consist of a continous contour. \f$ \{ img \in \mathbb{Z} | 0 \leq img \leq
   * 1 \} \f$
   * \return a vector with complex values, represing the contour in the
   * frequency domain, expressed as Fourier Descriptors
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
   * \brief Contour2Complex a private function which translates a continous
   * contour image
   * to a vector of complex values. The contour is found using a depth first
   * search with
   * extension list. The alghorithm is based upon <a href="http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-034-artificial-intelligence-fall-2010/lecture-videos/lecture-4-search-depth-first-hill-climbing-beam/">MIT opencourseware
   * 6-034-artificial-intelligence lecture 4</a>
   * \param img contour in the form of a cv::Mat type CV_8UC1. Which should
   * consist of a continous contour. \f$ \{ img \in \mathbb{Z} | 0 \leq img \leq
   * 1 \} \f$
   * \param centerCol centre of the contour X value
   * \param centerRow centre of the contour Y value
   * \return a vector with complex values, represing the contour as a function
   */
  ComplexVect_t Contour2Complex(const cv::Mat &img, float centerCol,
                                float centerRow);
  /*!
   * \brief Neighbors a private function returning the neighboring pixels which belong to a contour
   * \param O uchar pointer to the data
   * \param pixel current counter
   * \param columns total number of columns
   * \param rows total number of rows
   * \return
   */
  iContour_t Neighbors(uchar *O, int pixel, uint32_t columns, uint32_t rows);

  /*!
   * \brief fft a private function calculating the Fast Fourier Transform
   * let \f$ m \f$ be an integer and let \f$ N=2^m \f$ also
   * \f$ CA=[x_0,\ldots,x_{N-1}] \f$ is an \f$ N \f$ dimensional complex vector
   * let \f$ \omega=\exp({-2\pi i\over N}) \f$
   * then \f$ c_k={\frac{1}{N}}\sum_{j=0}^{j=N-1}CA_j\omega^{jk} \f$
   * \param CA a \f$ CA=[x_0,\ldots,x_{N-1}] \f$ is an \f$ N \f$ dimensional complex vector
   */
  void fft(ComplexArray_t &CA);

  /*!
   * \brief ifft
   * \param CA
   */
  void ifft(ComplexArray_t &CA);
};
}
