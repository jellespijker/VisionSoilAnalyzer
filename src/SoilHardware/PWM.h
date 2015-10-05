/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include "BBB.h"
#include <dirent.h>

#define OCP_PATH "/sys/class/pwm/"
#define PWM_CAPE "Override Board Name,00A0,Override Manuf,cape-universaln"

namespace Hardware {
class PWM : public BBB {
public:
  enum Pin // Four possible PWM pins
  { P8_13,
    P8_19,
    P9_14,
    P9_16 };
  enum Run // Signal generating
  { On = 1,
    Off = 0 };
  enum Polarity // Inverse duty polarity
  { Normal = 1,
    Inverted = 0 };

  Pin pin; // Current pin

  uint8_t GetPixelValue() { return pixelvalue; }
  void SetPixelValue(uint8_t value);

  float GetIntensity() { return intensity; };
  void SetIntensity(float value);

  int GetPeriod() { return period; };
  void SetPeriod(int value);

  int GetDuty() { return duty; };
  void SetDuty(int value);
  void SetIntensity();

  Run GetRun() { return run; };
  void SetRun(Run value);

  Polarity GetPolarity() { return polarity; };
  void SetPolarity(Polarity value);

  PWM(Pin pin);
  ~PWM();

private:
  int period;         // current period
  int duty;           // current duty
  float intensity;    // current intensity
  uint8_t pixelvalue; // current pixelvalue
  Run run;            // current run state
  Polarity polarity;  // current polaity

  string basepath;     // the basepath ocp
  string dutypath;     // base + duty path
  string periodpath;   // base + period path
  string runpath;      // base + run path
  string polaritypath; // base + polarity path

  void calcIntensity();
};
}
