/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include <exception>
#include <string>
#include <stdint.h>

using namespace std;

namespace SoilAnalyzer {
namespace Exception {
class AnalysisException : public std::exception {
public:
  AnalysisException(string m = "Analysis Failed!") : msg(m){};
  AnalysisException(string m = "Analysis Failed!", uint8_t id = 0) : msg(m) {
    exid = id;
  };
  ~AnalysisException() _GLIBCXX_USE_NOEXCEPT{};
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); };
  const uint8_t ID() const _GLIBCXX_USE_NOEXCEPT { return exid; };

private:
  string msg;
  uint8_t exid;
};
}
}
