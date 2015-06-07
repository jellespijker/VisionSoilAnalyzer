/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once

#include "EC12P.h"
#include "GPIO.h"
#include "PWM.h"
#include "ADC.h"

namespace Hardware {
class SoilCape {
public:
  EC12P RGBEncoder;
  PWM MicroscopeLEDs{PWM::P9_14};
  ADC MicroscopeLDR{ADC::ADC0};

  SoilCape();
  ~SoilCape();
};
}
