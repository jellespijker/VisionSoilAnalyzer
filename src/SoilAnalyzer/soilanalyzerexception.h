/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */
#define EXCEPTION_PARTICLE_NOT_ANALYZED "Particle not analyzed Exception!"
#define EXCEPTION_PARTICLE_NOT_ANALYZED_NR 0
#define EXCEPTION_NO_SNAPSHOTS "No snapshots Exception!"
#define EXCEPTION_NO_SNAPSHOTS_NR 1
#define EXCEPTION_NO_IMAGES_PRESENT "No images to analyse exception!"
#define EXCEPTION_NO_IMAGES_PRESENT_NR 2

#pragma once
#include <exception>
#include <string>

namespace SoilAnalyzer {
  namespace Exception {
    class SoilAnalyzerException : public std::exception {
    public:
      SoilAnalyzerException(std::string m = EXCEPTION_PARTICLE_NOT_ANALYZED,
                            int n = EXCEPTION_PARTICLE_NOT_ANALYZED_NR) : msg(m), nr(n) { }
      ~SoilAnalyzerException() _GLIBCXX_USE_NOEXCEPT {}
      const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); }
      const int *id() const _GLIBCXX_USE_NOEXCEPT { return &nr; }

    private:
      std::string msg;
      int nr;
    };
  }
}
