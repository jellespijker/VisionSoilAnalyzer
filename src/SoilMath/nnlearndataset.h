/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <fstream>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <opencv2/core/core.hpp>
#include "../Soil/Mat_archive.h"
#include "predict_t_archive.h"
#include "NN.h"

namespace SoilMath {
class NNLearnDataSet {
public:
  struct NNLearnItem_t {
    cv::Mat RGB;
    cv::Mat Edge;
    ComplexVect_t FFDescr;
    Predict_t Output;
    std::string SampleID;
    bool Learned;
  };
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar,
                 const unsigned int version __attribute__((unused))) {
    ar &NeuralNet;
    ar &RGB;
    ar &Edge;
    ar &InputDatatSet;
    ar &OutputDataSet;
    ar &SampleID;
    ar &Learned;
  }

  std::vector<cv::Mat> RGB;
  std::vector<cv::Mat> Edge;
  InputLearnVector_t InputDatatSet;
  OutputLearnVector_t OutputDataSet;
  std::vector<std::string> SampleID;
  std::vector<bool> Learned;
public:

  NN NeuralNet;

  NNLearnDataSet();
  NNLearnDataSet(const NNLearnDataSet &rhs);
  NNLearnDataSet &operator=(NNLearnDataSet const &rhs);

  ~NNLearnDataSet();

  NNLearnItem_t operator[](const size_t nIndex);

  void Save(std::string &filename);
  void Load(std::string &filename);

  NN Learn();
  void SetNeuralNet(uint32_t inputneurons, uint32_t hiddenneurons,
                    uint32_t outputneurons);
  void push_back(NNLearnItem_t &item);
  size_t size();
};
}
