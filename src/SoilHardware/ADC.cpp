/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "ADC.h"

namespace Hardware {
/*! Constructor
\param pin and ADCPin type indicating which analogue pin to use
*/
ADC::ADC(ADCPin pin) {
  this->Pin = pin;
  switch (pin) {
  case Hardware::ADC::ADC0:
    adcpath = ADC0_PATH;
    break;
  case Hardware::ADC::ADC1:
    adcpath = ADC1_PATH;
    break;
  case Hardware::ADC::ADC2:
    adcpath = ADC2_PATH;
    break;
  case Hardware::ADC::ADC3:
    adcpath = ADC3_PATH;
    break;
  case Hardware::ADC::ADC4:
    adcpath = ADC4_PATH;
    break;
  case Hardware::ADC::ADC5:
    adcpath = ADC5_PATH;
    break;
  case Hardware::ADC::ADC6:
    adcpath = ADC6_PATH;
    break;
  case Hardware::ADC::ADC7:
    adcpath = ADC7_PATH;
    break;
  }

  MinIntensity = 0;
  MaxIntensity = 4096;
}

/*! De-constructor*/
ADC::~ADC() {}

/*! Reads the current voltage in the pin
\return an integer between 0 and 4096
*/
int ADC::GetCurrentValue() {
  int retVal = StringToNumber<int>(Read(adcpath));
  Intensity = (float)(retVal - MinIntensity) /
              (4096 - (MinIntensity + (4096 - MaxIntensity)));
  return retVal;
}

/*! Set the current voltage at the pin as the minimum voltage*/
void ADC::SetMinIntensity() {
  MinIntensity = StringToNumber<int>(Read(adcpath));
}

void ADC::SetMaxIntensity() {
  MaxIntensity = StringToNumber<int>(Read(adcpath));
}

/*! Threading enabled polling of the analogue pin
\param callback the function which should be called when polling indicates a
change CallbackType
\return 0
*/
int ADC::WaitForValueChange(CallbackType callback) {
  threadRunning = true;
  callbackFunction = callback;
  if (pthread_create(&thread, NULL, &threadedPollADC,
                     static_cast<void *>(this))) {
    threadRunning = false;
    throw Exception::FailedToCreateGPIOPollingThreadException();
  }
  return 0;
}

/*! Polling of the analogue pin
\return the current value
*/
int ADC::WaitForValueChange() {
  int fd, i, epollfd, count = 0;
  struct epoll_event ev;
  epollfd = epoll_create(1);
  if (epollfd == -1) {
    throw Exception::FailedToCreateGPIOPollingThreadException(
        "GPIO: Failed to create epollfd!");
  }
  if ((fd = open(adcpath.c_str(), O_RDONLY | O_NONBLOCK)) == -1) {
    throw Exception::ADCReadException();
  }
  ev.events = EPOLLIN;
  ev.data.fd = fd;

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    throw Exception::FailedToCreateGPIOPollingThreadException(
        "ADC: Failed to add control interface!");
  }

  while (count <= 1) {
    i = epoll_wait(epollfd, &ev, 1, -1);
    if (i == -1) {
      close(fd);
      return -1;
    } else {
      count++;
    }
  }
  close(fd);
  return StringToNumber<int>(Read(adcpath));
}

/*! friendly function to start the threading*/
void *threadedPollADC(void *value) {
  ADC *adc = static_cast<ADC *>(value);
  while (adc->threadRunning) {
    adc->callbackFunction(adc->WaitForValueChange());
    usleep(200000);
  }
}
}
