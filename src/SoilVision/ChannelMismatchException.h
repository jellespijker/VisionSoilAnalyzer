/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class ChannelMismatchException
Exception class which is thrown when Extracted channel out of bounds exception
*/

#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Vision {
namespace Exception {
class ChannelMismatchException : public std::exception {
public:
  ChannelMismatchException(
      string m = "Extracted channel out of bounds exception!")
      : msg(m){};
  ~ChannelMismatchException() _GLIBCXX_USE_NOEXCEPT{};
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); };

private:
  string msg;
};
}
}
