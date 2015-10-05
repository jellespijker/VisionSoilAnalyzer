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
#define EXCEPTION_CTRL_NOT_FOUND "Control not found!"
#define EXCEPTION_CTRL_NOT_FOUND_NR 5
#define EXCEPTION_CTRL_VALUE "Control value not set!"
#define EXCEPTION_CTRL_VALUE_NR 5
#define EXCEPTION_GSTREAM_INIT_EXCEPTION "Gstream could not be initialize exception!"
#define EXCEPTION_GSTREAM_INIT_EXCEPTION_NR 6
#define EXCEPTION_GSTREAM_ELEM_EXCEPTION "Gstream elements could not be linked exception!"
#define EXCEPTION_GSTREAM_ELEM_EXCEPTION_NR 7
#define EXCEPTION_GSTREAM_PLAYSTATE_EXCEPTION "Gstream unable to set playstate exception!"
#define EXCEPTION_GSTREAM_PLAYSTATE_EXCEPTION_NR 9
#define EXCEPTION_GSTREAM_VARIOUS_EXCEPTION_NR 10

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
