/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class EC12P
Interaction with the sparksfun RGB encoder
*/

#pragma once

#include "eqep.h"
#include "GPIO.h"
#include "FailedToCreateThreadException.h"

#include <pthread.h>

using namespace std;

namespace Hardware {
class EC12P {
public:
  EC12P();
  ~EC12P();

  /*! Enumerator indicating the color of the encoder shaft*/
  enum Color {
    Red,     /*!< Red*/
    Pink,    /*!< Pink*/
    Blue,    /*!< Blue*/
    SkyBlue, /*!< SkyBlue*/
    Green,   /*!< Green*/
    Yellow,  /*!< Yellow*/
    White,   /*!< White*/
    None     /*!< Off*/
  };

  void SetPixelColor(Color value);
  Color GetPixelColor() { return PixelColor; };

  void RainbowLoop(int sleepperiod);
  void StopRainbowLoop() { threadRunning = false; };

  eQEP Rotary{eQEP2, eQEP::eQEP_Mode_Absolute}; /*!< The encoder*/
  GPIO Button{68};                              /*!< The pushbutton*/

private:
  Color PixelColor; /*!< Current shaft color*/

  GPIO R{31}; /*!< Red LED*/
  GPIO B{48}; /*!< Blue LED*/
  GPIO G{51}; /*!< Green LED*/

  pthread_t thread;   /*!< the thread*/
  bool threadRunning; /*!< Bool used to stop the thread*/
  int sleepperiod;    /*!< Sleep period*/
  friend void *colorLoop(void *value);
};
void *colorLoop(void *value);
}
