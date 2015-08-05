/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */
#define EXCEPTION_OPENCAM "Exception could not open cam!"
#define EXCEPTION_OPENCAM_NR 0
#define EXCEPTION_NOCAMS "Exception no cam available!"
#define EXCEPTION_NOCAMS_NR 1
#define EXCEPTION_QUERY "Exception could not query device!"
#define EXCEPTION_QUERY_NR 3
#define EXCEPTION_FORMAT_RESOLUTION "Exception No supported formats and resolutions!"
#define EXCEPTION_FORMAT_RESOLUTION_NR 4

#pragma once
#include <exception>
#include <string>

using namespace std;

namespace Hardware {
namespace Exception {
class MicroscopeException : public std::exception {
public:
  MicroscopeException(string m = EXCEPTION_OPENCAM,
                      int n = EXCEPTION_OPENCAM_NR) : msg{m}, nr{n} {  }
  ~MicroscopeException() _GLIBCXX_USE_NOEXCEPT {}
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); }
  const int *id() const _GLIBCXX_USE_NOEXCEPT { return &nr; }

private:
  string msg;
  int nr;
};
}
}
