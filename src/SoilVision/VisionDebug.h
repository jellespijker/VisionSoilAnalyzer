/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
// Debuging helper macros
#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
#include <limits>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "ImageProcessing.h"
#ifndef SHOW_DEBUG_IMG
#define SHOW_DEBUG_IMG(img, T1, maxVal, windowName, scale)                     \
  Vision::ImageProcessing::ShowDebugImg<T1>(img, maxVal, windowName, scale)
#endif // !SHOW_DEBUG_IMG
#else
#ifndef SHOW_DEBUG_IMG
#define SHOW_DEBUG_IMG(img, T1, maxVal, windowName, scale)
#endif // !SHOW_DEBUG_IMG
#endif
