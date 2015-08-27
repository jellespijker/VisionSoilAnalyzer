/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#define EXCEPTION_MATH "Math Exception!"
#define EXCEPTION_MATH_NR 0
#define EXCEPTION_NO_CONTOUR_FOUND                                             \
  "No continuous contour found, or less then 8 pixels long!"
#define EXCEPTION_NO_CONTOUR_FOUND_NR 1
#define EXCEPTION_SIZE_OF_INPUT_NEURONS                                        \
  "Size of input unequal to input neurons exception!"
#define EXCEPTION_SIZE_OF_INPUT_NEURONS_NR 2
#define EXCEPTION_NEURAL_NET_NOT_STUDIED "Neural net didn't study exception!"
#define EXCEPTION_NEURAL_NET_NOT_STUDIED_NR 3
#define EXCEPTION_TYPE_NOT_SUPPORTED                                           \
  "Type not supported for operation exception!"
#define EXCEPTION_TYPE_NOT_SUPPORTED_NR 4

#pragma once
#include <exception>
#include <string>

namespace SoilMath {
namespace Exception {
class MathException : public std::exception {
public:
  MathException(std::string m = EXCEPTION_MATH, int n = EXCEPTION_MATH_NR)
      : msg(m), nr(n){};
  ~MathException() _GLIBCXX_USE_NOEXCEPT{};
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); };
  const int *id() const _GLIBCXX_USE_NOEXCEPT { return &nr; }

private:
  std::string msg;
  int nr;
};
}
}
