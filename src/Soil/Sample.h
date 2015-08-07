/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
//#define DEBUG
#define PROG_INCR(status)                                                      \
  currentProg += progstep;                                                     \
  prog_sig(currentProg, status)
#include "Soil.h"
#include "Particle.h"
#include "AnalysisResults.h"
#include "AnalysisException.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include "Mat_archive.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <string>

#include "../SoilVision/Vision.h"
#include "../SoilMath/SoilMath.h"
#include "soilsettings.h"

using namespace std;
using namespace cv;

namespace SoilAnalyzer {
/*!
 * \brief The Sample class
 * \details This class represent a single soilsample snapshot it inherents from
 * the class Soil
 */
class Sample : public Soil {
public:
  typedef boost::signals2::signal<void(
      float, std::string)> Progress_t; /**< A boost signal, used for progress
                                          update indicating the progres so far
                                          as a float between 0. and 1. and the
                                          current progress step as string*/

  /*!
   * \brief connect_Progress used to connect the progress signal
   * \param subscriber a reference to the subscriber
   * \return returns the signal
   */
  boost::signals2::connection
  connect_Progress(const Progress_t::slot_type &subscriber);

  /*!
   * \brief Sample the constructor
   * \param settings a pointer to the an object of the type SoilSettings used to
   * initalize the analyzing settings
   */
  Sample(SoilSettings *settings = nullptr);

  /*!
   * \brief Sample the constructor
   * \param src the source rgb image
   * \param settings a pointer to the an object of the type SoilSettings used to
   * initalize the analyzing settings
   */
  Sample(const Mat &src, SoilSettings *settings = nullptr);

  /*!
   * \brief Sample the constructor
   * \param src the source rgb image
   * \param back_src the source backgroudn image
   * \param settings a pointer to the an object of the type SoilSettings used to
   * initalize the analyzing settings
   */
  Sample(const Mat &src, const Mat &back_src,SoilSettings *settings = nullptr);

  /*!
    * \brief The Deconstructor
    */
  ~Sample();

  cv::Mat OriginalImage;       /**< The original image with light projected from the front*/
  cv::Mat ProjectedImage;      /**< The original image with light projected from behind*/
  vector<Particle> Population; /**< a Vector with original particles*/
  AnalysisResults Results;     /**< The analysis results*/

  SoilSettings *Settings =
      nullptr; /**< The Settings used to initialize the analyzis*/

  /*!
   * \brief PrepImg Prep te image for analysis
   * \param settings a pointer to the an object of the type SoilSettings used to
   * initalize the analyzing settings
   */
  void PrepImg(SoilSettings *settings = nullptr);
  bool imgPrepped = false; /**< */

  /*!
   * \brief Analyse the RGB image
   * \param nn a reference to the neural network used during analyzing
   */
  void Analyse(SoilMath::NN &nn);

  /*!
   * \brief Analyse the the RGB image
   * \param src the RGB image with needs to be analysed
   * \param nn a reference to the neural network used during analyzing
   */
  void Analyse(const Mat &src, SoilMath::NN &nn);

  /*!
   * \brief Save the SoilSample to disk
   * \param filename the where to save
   */
  void Save(string &filename);

  /*!
   * \brief Load the SoilSample from disk
   * \param filename where the saved Soilsample can be found
   */
  void Load(string &filename);

private:
  Progress_t prog_sig; /**< The progress signal*/

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &BOOST_SERIALIZATION_BASE_OBJECT_NVP(Soil);
    ar &OriginalImage;
    ar &ProjectedImage;
    ar &Population;
    ar &Results;
    ar &Settings;
    ar &imgPrepped;
  }

  /*!
   * \brief AnalysePopVect analyse the particle population vector
   * \param population a reference to individual particles
   * \param results
   * \return
   */
  bool AnalysePopVect(const vector<Particle> &population,
                      AnalysisResults &results);

  /*!
   * \brief SegmentParticles
   * \param segType
   */
  void SegmentParticles(
      Vision::Segment::SegmentationType segType = Vision::Segment::Normal);

  void getEnhInt(cv::Mat &src, cv::Mat &dst, float &progstep);
  void getBW(cv::Mat &src, cv::Mat &dst, float &progstep);
};
}
BOOST_CLASS_VERSION(SoilAnalyzer::Sample, 1)
