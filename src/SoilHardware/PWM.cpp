/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "PWM.h"

namespace Hardware {
/// <summary>
/// Constructeur
/// </summary>
/// <param name="pin">Pin</param>
PWM::PWM(Pin pin) {
  this->pin = pin;

  // Check if PWM cape is loaded, if not load it
  if (!CapeLoaded(PWM_CAPE)) {
    Write(SLOTS, PWM_CAPE);
  }

  // Init the pin
  switch (pin) {
  case Hardware::PWM::P8_13:
    system("config-pin P8.13 pwm");
    basepath = OCP_PATH;
    basepath.append("pwmchip4/pwm1");
    break;
  case Hardware::PWM::P8_19:
    system("config-pin P8.19 pwm");
    basepath = OCP_PATH;
    basepath.append("pwmchip4/pwm0");
    break;
  case Hardware::PWM::P9_14:
    system("config-pin P9.14 pwm");
    basepath = OCP_PATH;
    basepath.append("pwmchip2/pwm0");
    break;
  case Hardware::PWM::P9_16:
    system("config-pin P9.16 pwm");
    basepath.append("pwmchip2/pwm1");
    break;
  }

  // Get the working paths
  dutypath = basepath + "/duty_cycle";
  periodpath = basepath + "/period";
  runpath = basepath + "/run";
  polaritypath = basepath + "/polarity";

  // Give Linux time to setup directory structure;
  usleep(250000);

  // Read current values
  period = StringToNumber<int>(Read(periodpath));
  duty = StringToNumber<int>(Read(dutypath));
  run = static_cast<Run>(StringToNumber<int>(Read(runpath)));
  polarity = static_cast<Polarity>(StringToNumber<int>(Read(polaritypath)));

  // calculate the current intensity
  calcIntensity();
}

PWM::~PWM() {}

/// <summary>
/// Calculate the current intensity
/// </summary>
void PWM::calcIntensity() {
  if (polarity == Normal) {
    if (duty == 0) {
      intensity = 0.0f;
    } else {
      intensity = (float)period / (float)duty;
    }
  } else {
    if (period == 0) {
      intensity = 0.0f;
    } else {
      intensity = (float)duty / (float)period;
    }
  }
}

/// <summary>
/// Set the intensity level as percentage
/// </summary>
/// <param name="value">floating value multipication factor</param>
void PWM::SetIntensity(float value) {
  if (polarity == Normal) {
    SetDuty(static_cast<int>((value * duty) + 0.5));
  } else {
    SetPeriod(static_cast<int>((value * period) + 0.5));
  }
}

/// <summary>
/// Set the output as a corresponding uint8_t value
/// </summary>
/// <param name="value">pixel value 0-255</param>
void PWM::SetPixelValue(uint8_t value) {
  if (period != 255) {
    SetPeriod(255);
  }
  SetDuty(255 - value);
  pixelvalue = value;
}

/// <summary>
/// Set the period of the signal
/// </summary>
/// <param name="value">period : int</param>
void PWM::SetPeriod(int value) {
  string valstr = NumberToString<int>(value);
  Write(periodpath, valstr);
  period = value;

  calcIntensity();
}

/// <summary>
/// Set the duty of the signal
/// </summary>
/// <param name="value">duty : int</param>
void PWM::SetDuty(int value) {
  string valstr = NumberToString<int>(value);
  Write(dutypath, valstr);
  duty = value;

  calcIntensity();
}

/// <summary>
/// Run the signal
/// </summary>
/// <param name="value">On or Off</param>
void PWM::SetRun(Run value) {
  int valInt = static_cast<int>(value);
  string valstr = NumberToString<int>(valInt);
  Write(runpath, valstr);
  run = value;
}

/// <summary>
/// Set the polarity
/// </summary>
/// <param name="value">Normal or Inverted signal</param>
void PWM::SetPolarity(Polarity value) {
  int valInt = static_cast<int>(value);
  string valstr = NumberToString<int>(valInt);
  Write(runpath, valstr);
  polarity = value;
}
}
