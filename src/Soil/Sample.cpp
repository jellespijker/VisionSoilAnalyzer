/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "Sample.h"

namespace SoilAnalyzer {
Sample::Sample(SoilSettings *settings) { Settings = settings; }

Sample::Sample(const Mat &src, SoilSettings *settings) {
  Settings = settings;
  OriginalImage = src.clone();
}

Sample::~Sample() {}

void Sample::Save(string &filename) {
  std::ofstream ofs(filename.c_str());
  boost::archive::binary_oarchive oa(ofs);
  oa << boost::serialization::make_nvp("SoilSample", *this);
}

void Sample::Load(string &filename) {
  std::ifstream ifs(filename.c_str());
  boost::archive::binary_iarchive ia(ifs);
  ia >> boost::serialization::make_nvp("SoilSample", *this);
}

void Sample::PrepImg(SoilSettings *settings) {
  // setup the settings
  if (settings == nullptr && Settings == nullptr) {
    Settings = new SoilSettings;
  } else {
    Settings = settings;
  }

  // set up the progress signal
  float currentProg = 0.;
  prog_sig(currentProg, "Starting segmentation");

  uint32_t totalsteps = 5;
  if (Settings->useAdaptiveContrast) {
    totalsteps++;
  }
  if (Settings->useBlur) {
    totalsteps++;
  }
  if (Settings->fillHoles) {
    totalsteps++;
  }
  if (Settings->ignorePartialBorderParticles) {
    totalsteps++;
  }
  if (Settings->morphFilterType != Vision::MorphologicalFilter::NONE) {
    totalsteps++;
  }
  float progstep = 1. / static_cast<float>(totalsteps);

  if (OriginalImage.empty()) {
    throw Exception::AnalysisException("No Image found to analyze!", 1);
  }
  SHOW_DEBUG_IMG(OriginalImage, uchar, 255, "RGB", false);

  // Convert the image to an intensity image and an enhanced Intinsity for
  // better segmentation
  Vision::Conversion RGBConvertor(OriginalImage);
  RGBConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::Intensity);
  PROG_INCR("Converted to intensity");
  Intensity = RGBConvertor.ProcessedImg.clone();
  SHOW_DEBUG_IMG(Intensity, uchar, 255, "Intensity", false);

  // Enhance the image with an Adaptive contrast stretch and/or followed by a
  // blur
  Vision::Enhance IntEnchance(Intensity);
  if (Settings->useAdaptiveContrast) {
    IntEnchance.AdaptiveContrastStretch(Settings->adaptContrastKernelSize,
                                        Settings->adaptContrastKernelSize);
    PROG_INCR("Adaptive contrast stretch applied");
    if (Settings->useBlur) {
      IntEnchance.Blur(Settings->blurKernelSize, true);
      PROG_INCR("Blur applied");
    }
  } else if (Settings->useBlur) {
    IntEnchance.Blur(Settings->blurKernelSize, false);
    PROG_INCR("Blur applied");
  } else {
    IntEnchance.ProcessedImg = IntEnchance.OriginalImg;
  }
  OptimizedInt =
      IntEnchance.ProcessedImg(cv::Rect(10, 10, OriginalImage.cols - 20,
                                        OriginalImage.rows - 20)).clone();
  SHOW_DEBUG_IMG(OptimizedInt, uchar, 255, "IntEnchance", false);

  // Segment the Dark Objects en fill the holes
  Vision::Segment Segmenter(OptimizedInt);
  Segmenter.sigma = Settings->sigmaFactor;
  Segmenter.thresholdOffset = Settings->thresholdOffsetValue;
  Segmenter.ConvertToBW(Settings->typeOfObjectsSegmented);
  PROG_INCR("Threshold applied");
  if (Settings->fillHoles) {
    Segmenter.FillHoles(true);
    PROG_INCR("Holes filled");
  }
  if (Settings->ignorePartialBorderParticles) {
    Segmenter.RemoveBorderBlobs(1, true);
    PROG_INCR("Border Blobs removed");
  }
  SHOW_DEBUG_IMG(Segmenter.ProcessedImg, uchar, 255, "Segmenter", true);

  // Erode the segmented image and sets the BW image use it to create the NO
  // background RGB
  Vision::MorphologicalFilter Filter(Segmenter.ProcessedImg);
  uint kSize = Settings->filterMaskSize;
  Mat mask = cv::Mat::zeros(kSize, kSize, CV_8UC1);
  circle(mask, Point(kSize / 2, kSize / 2), (kSize / 2) + 1, 1, -1);
  switch (Settings->morphFilterType) {
  case Vision::MorphologicalFilter::CLOSE:
    Filter.Close(mask);
    PROG_INCR("Morphological filer - close applied");
    break;
  case Vision::MorphologicalFilter::DILATE:
    Filter.Dilation(mask);
    PROG_INCR("Morphological filer - dilate applied");
    break;
  case Vision::MorphologicalFilter::ERODE:
    Filter.Erosion(mask);
    PROG_INCR("Morphological filer - erode applied");
    break;
  case Vision::MorphologicalFilter::OPEN:
    Filter.Open(mask);
    PROG_INCR("Morphological filer - open applied");
    break;
  case Vision::MorphologicalFilter::NONE:
    Filter.ProcessedImg = Filter.OriginalImg;
  }
  BW = Filter.ProcessedImg.clone();
  SHOW_DEBUG_IMG(BW, uchar, 255,
                 "BW after segmentation, fill holes and erosion", true);
  RGB = Vision::ImageProcessing::CopyMat<uchar>(
      OriginalImage(cv::Rect(10, 10, OriginalImage.cols - 20,
                             OriginalImage.rows - 20)).clone(),
      BW, CV_8UC1);
  PROG_INCR("RGB masked image generated");
  SHOW_DEBUG_IMG(RGB, uchar, 255, "RGB no Background", false);

  // Create the Edge image
  Vision::Segment Edger(BW);
  Edger.GetEdgesEroding();
  Edge = Edger.ProcessedImg;
  PROG_INCR("Edge filter applied");
  SHOW_DEBUG_IMG(Edge, uchar, 255, "Edge", true);

  // Make the CIE La*b* conversion
  Vision::Conversion RGBnewConvertor(RGB);
  RGBnewConvertor.Convert(Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
  LAB = RGBnewConvertor.CopyMat<float>(RGB, BW, CV_32F);
  PROG_INCR("CIE La*b* conversion calculated");
  SHOW_DEBUG_IMG(LAB, float, 1.0, "LAB", true);

  // Create the Redness Index
  Vision::Conversion LABConvertor(LAB);
  LABConvertor.Convert(Vision::Conversion::CIE_lab, Vision::Conversion::RI);
  RI = LABConvertor.ProcessedImg;
  PROG_INCR("Redness conversion calculated");
  SHOW_DEBUG_IMG(RI, float, 1.0, "RI", true);

  imgPrepped = true;
}

void Sample::Analyse(SoilMath::NN &nn) {
  if (!imgPrepped) {
    PrepImg();
  }
  // Calculate the statistics CIE La*b*
  // vector<Mat> LABextract = Vision::ImageProcessing::extractChannel(LAB);
  // for_each(LABextract.begin(), LABextract.end(), [&](Mat &lab) {
  // Results.LAB_Stat.push_back(floatStat_t((float *)lab.data, lab.rows,
  // lab.cols)); });

  // Calculate the statistics RI
  // Results.RI_Stat = floatStat_t((float *)RI.data, RI.rows, RI.cols);

  // Segment and analyze the particles
  // SegmentParticles();
  // for_each(Population.begin(), Population.end(), [&](Particle &P)
  //{
  //	P.Analyze(nn);
  //});

  // Analyze the image
}

void Sample::Analyse(const Mat &src, SoilMath::NN &nn) {
  OriginalImage = src;
  Analyse(nn);
}

bool Sample::AnalysePopVect(const vector<Particle> &population,
                            AnalysisResults &results) {
  return true;
}

void Sample::SegmentParticles(Vision::Segment::SegmentationType segType) {
  Vision::Enhance Enhancer(Intensity);
  Vision::Segment Segmenter;
  Vision::MorphologicalFilter Eroder;

  switch (segType) {
  case Vision::Segment::Normal:
    // Optimize the intensity image
    Enhancer.Blur(15);
    Enhancer.AdaptiveContrastStretch(5, 0.125, true);

    // Segment the enhance image and remove borderblobs get the edges
    Segmenter.LoadOriginalImg(Enhancer.ProcessedImg);
    Segmenter.ConvertToBW(Vision::Segment::Dark);
    Segmenter.RemoveBorderBlobs(1, true);
    BW = Segmenter.ProcessedImg.clone();
    break;
  case Vision::Segment::LabNeuralNet:
    throw Exception::AnalysisException("Not yet implemented!", 1);
    break;
  case Vision::Segment::GraphMinCut:
    throw Exception::AnalysisException("Not yet implemented!", 1);
    break;
  default:
    break;
  }

  Vision::Segment getEdges(BW);
  getEdges.GetEdgesEroding();
  Edge = getEdges.ProcessedImg;

  // Get the Particlelist
  Segmenter.GetBlobList(true);
  Population.resize(Segmenter.BlobList.size());
  uint32_t i = 0;
  for_each(Population.begin(), Population.end(), [&](Particle &P) {
    P.ID = Segmenter.BlobList[i].Label;
    P.Analysis.Analyzed = false;
    P.BW = Segmenter.BlobList[i].Img.clone();
    cv::Rect ROI = Segmenter.BlobList[i].ROI;
    P.Intensity =
        Vision::Segment::CopyMat<uchar>(Intensity(ROI).clone(), P.BW, CV_8UC1);
    P.LAB = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), P.BW, CV_32FC3);
    P.RGB = Vision::Segment::CopyMat<uchar>(LAB(ROI).clone(), P.BW, CV_8UC3);
    P.RI = Vision::Segment::CopyMat<float>(LAB(ROI).clone(), P.BW, CV_32FC1);
    P.Edge = Vision::Segment::CopyMat<uchar>(Edge(ROI).clone(), P.BW, CV_8UC1);
    i++;
  });
}

boost::signals2::connection
Sample::connect_Progress(const Progress_t::slot_type &subscriber) {
  return prog_sig.connect(subscriber);
}
}
