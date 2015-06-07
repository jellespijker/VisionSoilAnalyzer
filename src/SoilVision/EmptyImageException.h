/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class EmtpyImageException
Exception class which is thrown when operations are about to start on a empty
image.
*/

#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Vision {
namespace Exception {
class EmtpyImageException : public std::exception {
public:
  EmtpyImageException(string m = "Empty Image!") : msg(m){};
  ~EmtpyImageException() _GLIBCXX_USE_NOEXCEPT{};
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); };

private:
  string msg;
};
}
}
