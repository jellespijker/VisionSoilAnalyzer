/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class ImageProcessing
\brief Core class of all the image classes
Core class of all the image classes with a few commonly shared functions and
variables
*/
#include "ImageProcessing.h"

namespace Vision {
/*! Constructor of the core class*/
ImageProcessing::ImageProcessing() {}

/*! De-constructor of the core class*/
ImageProcessing::~ImageProcessing() {}

/*! Create a LUT indicating which iteration variable i is the end of an row
\param nData an int indicating total pixels
\param hKsize int half the size of the kernel, if any. which acts as an offset
from the border pixels
\param nCols int number of columns in a row
\return array of uchars where a zero is a middle column and a 1 indicates an end
of an row minus the offset from half the kernel size
*/
uchar *ImageProcessing::GetNRow(int nData, int hKsize, int nCols,
                                uint32_t totalRows) {
  // Create LUT to determine when there is an new row
  uchar *nRow = new uchar[nData + 1]{};
  // int i = 0;
  int shift = nCols - hKsize - 1;
  for (uint32_t i = 0; i < totalRows; i++) {
    nRow[(i * nCols) + shift] = 1;
  }
  return nRow;
}

std::vector<Mat> ImageProcessing::extractChannel(const Mat &src) {
  vector<Mat> chans;
  split(src, chans);
  return chans;
}

boost::signals2::connection
ImageProcessing::connect_Progress(const Progress_t::slot_type &subscriber) {
  return prog_sig.connect(subscriber);
}
}
