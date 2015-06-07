/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "BBB.h"

namespace Hardware {
/*! Constructor*/
BBB::BBB() {
  threadRunning = false;
  callbackFunction = NULL;
  debounceTime = 0;
  thread = (pthread_t)NULL;
}

/*! De-constructor*/
BBB::~BBB() {}

/*! Reads the first line from a file
\param path constant string pointing towards the file
\returns this first line
*/
string BBB::Read(const string &path) {
  ifstream fs;
  fs.open(path.c_str());
  if (!fs.is_open()) {
    throw Exception::GPIOReadException(("Can't open: " + path).c_str());
  }
  string input;
  getline(fs, input);
  fs.close();
  return input;
}

/*! Writes a value to a file
\param path a constant string pointing towards the file
\param value a constant string which should be written in the file
*/
void BBB::Write(const string &path, const string &value) {
  ofstream fs;
  fs.open(path.c_str());
  if (!fs.is_open()) {
    throw Exception::GPIOReadException(("Can't open: " + path).c_str());
  }
  fs << value;
  fs.close();
}

/*! Checks if a directory exist
\returns true if the directory exists and false if not
*/
bool BBB::DirectoryExist(const string &path) {
  struct stat st;
  if (stat((char *)path.c_str(), &st) != 0) {
    return false;
  }
  return true;
}

/*! Checks if a cape is loaded in the file /sys/devices/bone_capemgr.9/slots
\param shield a const search string which is a (part) of the shield name
\return true if the search string is found otherwise false
*/
bool BBB::CapeLoaded(const string &shield) {
  bool shieldFound = false;

  ifstream fs;
  fs.open(SLOTS);
  if (!fs.is_open()) {
    throw Exception::GPIOReadException("Can't open SLOTS");
  }

  string line;
  while (getline(fs, line)) {
    if (line.find(shield) != string::npos) {
      shieldFound = true;
      break;
    }
  }
  fs.close();
  return shieldFound;
}
}
