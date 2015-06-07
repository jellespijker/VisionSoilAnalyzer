/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "USB.h"

namespace Hardware {
USB::USB() {}

USB::~USB() {}

void USB::ResetUSB() {
  int fd, rc;

  fd = open("/dev/bus/usb/001/002", O_WRONLY);
  rc = ioctl(fd, USBDEVFS_RESET, 0);
  if (rc < 0) {
    throw - 1;
  }
  close(fd);
}
}
