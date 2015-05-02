#pragma once
// Debuging helper macros
#ifndef DEBUG
    //#define DEBUG
#endif

#ifdef DEBUG
	#include <limits>
	#include <opencv2/highgui/highgui.hpp>
	#include <vector>
	#include "ImageProcessing.h"
	#ifndef SHOW_DEBUG_IMG
		#define SHOW_DEBUG_IMG(img, T1, maxVal, windowName, scale) Vision::ImageProcessing::ShowDebugImg<T1>(img, maxVal, windowName, scale)
	#endif // !SHOW_DEBUG_IMG
#else 
	#ifndef SHOW_DEBUG_IMG
		#define SHOW_DEBUG_IMG(img, T1, maxVal, windowName, scale)
	#endif // !SHOW_DEBUG_IMG
#endif
