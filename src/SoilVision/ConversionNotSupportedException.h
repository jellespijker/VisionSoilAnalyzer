/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class ConversionNotSupportedException
Exception class which is thrown when an illegal conversion is requested.
*/
#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Vision {
namespace Exception {
class ConversionNotSupportedException : public std::exception {
public:
  ConversionNotSupportedException(
      string m = "Requested conversion is not supported!")
      : msg(m){};
  ~ConversionNotSupportedException() _GLIBCXX_USE_NOEXCEPT{};
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); };

private:
  string msg;
};
}
}
