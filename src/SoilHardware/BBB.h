/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

/*! \class BBB
The core BeagleBone Black class used for all hardware related classes.
Consisting of universal used method, functions and variables. File operations,
polling and threading
*/

#pragma once

#define SLOTS                                                                  \
  "/sys/devices/platform/bone_capemgr/slots" /*!< Beaglebone capemanager slots file*/

#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <regex>
#include <stdexcept>

#include "GPIOReadException.h"
#include "FailedToCreateGPIOPollingThreadException.h"
#include "ValueOutOfBoundsException.h"

using namespace std;

namespace Hardware {
typedef int (*CallbackType)(
    int); /*!< CallbackType used to pass a function to a thread*/

class BBB {
public:
  int debounceTime; /*!< debounce time for a button in milliseconds*/

  BBB();
  ~BBB();

protected:
  bool threadRunning;            /*!< used to stop the thread*/
  pthread_t thread;              /*!< The thread*/
  CallbackType callbackFunction; /*!< the callbakcfunction*/

  bool DirectoryExist(const string &path);
  bool CapeLoaded(const string &shield);

  string Read(const string &path);
  void Write(const string &path, const string &value);

  /*! Converts a number to a string
  \param Number as typename
  \returns the number as a string
  */
  template <typename T> string NumberToString(T Number) {
    ostringstream ss;
    ss << Number;
    return ss.str();
  };

  /*! Converts a string to a number
  \param Text the string that needs to be converted
  \return the number as typename
  */
  template <typename T> T StringToNumber(string Text) {
    stringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
  };
};
}
