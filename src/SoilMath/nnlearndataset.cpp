/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "nnlearndataset.h"

namespace SoilMath {
NNLearnDataSet::NNLearnDataSet() {}

NNLearnDataSet::NNLearnDataSet(const NNLearnDataSet &rhs) {
  this->NeuralNet = rhs.NeuralNet;
  this->Edge = rhs.Edge;
  this->InputDatatSet = rhs.InputDatatSet;
  this->Learned = rhs.Learned;
  this->OutputDataSet = rhs.OutputDataSet;
  this->RGB = rhs.RGB;
  this->SampleID = rhs.SampleID;
}

void NNLearnDataSet::Save(std::string &filename) {
  std::ofstream ofs(filename.c_str());
  boost::archive::binary_oarchive oa(ofs);
  oa << boost::serialization::make_nvp("NNLearnDataSet", *this);
}

void NNLearnDataSet::Load(std::string &filename) {
  std::ifstream ifs(filename.c_str());
  boost::archive::binary_iarchive ia(ifs);
  ia >> boost::serialization::make_nvp("NNLearnDataSet", *this);
}

NN NNLearnDataSet::Learn() {
  if (this->size() > 0) {
    NeuralNet.Learn(InputDatatSet, OutputDataSet, 9);
  }
  return NeuralNet;
}

void NNLearnDataSet::push_back(NNLearnItem_t &item) {
  RGB.push_back(item.RGB);
  Edge.push_back(item.Edge);
  InputDatatSet.push_back(item.FFDescr);
  OutputDataSet.push_back(item.Output);
  SampleID.push_back(item.SampleID);
  Learned.push_back(item.Learned);
}

size_t NNLearnDataSet::size() { return Learned.size(); }

void NNLearnDataSet::SetNeuralNet(uint32_t inputneurons, uint32_t hiddenneurons,
                                  uint32_t outputneurons) {
  NeuralNet = NN(inputneurons, hiddenneurons, outputneurons);
}

NNLearnDataSet::NNLearnItem_t NNLearnDataSet::operator[](const size_t nIndex) {
  NNLearnItem_t retVal;
  retVal.Edge = Edge[nIndex];
  retVal.FFDescr = InputDatatSet[nIndex];
  retVal.Learned = Learned[nIndex];
  retVal.Output = OutputDataSet[nIndex];
  retVal.RGB = RGB[nIndex];
  retVal.SampleID = SampleID[nIndex];
  return retVal;
}

NNLearnDataSet &NNLearnDataSet::operator=(NNLearnDataSet const &rhs) {
  if (&rhs != this) {
    this->NeuralNet = rhs.NeuralNet;
    this->Edge = rhs.Edge;
    this->InputDatatSet = rhs.InputDatatSet;
    this->Learned = rhs.Learned;
    this->OutputDataSet = rhs.OutputDataSet;
    this->RGB = rhs.RGB;
    this->SampleID = rhs.SampleID;
  }
  return *this;
}

NNLearnDataSet::~NNLearnDataSet() { }

}
