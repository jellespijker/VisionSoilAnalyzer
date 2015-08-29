/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#include "GPIO.h"

namespace Hardware {
GPIO::GPIO(int number) {

  this->number = number;
  gpiopath = GPIOS + NumberToString<int>(number);

  if (!isExported(number, direction, edge)) {
    ExportPin(number);
    direction = ReadsDirection(gpiopath);
    edge = ReadsEdge(gpiopath);
  }
  usleep(250000);
}

GPIO::~GPIO() { UnexportPin(number); }

int GPIO::WaitForEdge(CallbackType callback) {
  threadRunning = true;
  callbackFunction = callback;
  if (pthread_create(&this->thread, NULL, &threadedPollGPIO,
                     static_cast<void *>(this))) {
    threadRunning = false;
    throw Exception::FailedToCreateGPIOPollingThreadException();
  }
  return 0;
}

int GPIO::WaitForEdge() {
  if (direction == Output) {
    SetDirection(Input);
  }
  int fd, i, epollfd, count = 0;
  struct epoll_event ev;
  epollfd = epoll_create(1);
  if (epollfd == -1) {
    throw Exception::FailedToCreateGPIOPollingThreadException(
        "GPIO: Failed to create epollfd!");
  }
  if ((fd = open((gpiopath + VALUE).c_str(), O_RDONLY | O_NONBLOCK)) == -1) {
    throw Exception::GPIOReadException();
  }

  // read operation | edge triggered | urgent data
  ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
  ev.data.fd = fd;

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    throw Exception::FailedToCreateGPIOPollingThreadException(
        "GPIO: Failed to add control interface!");
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
  return 0;
}

GPIO::Value GPIO::GetValue() { return ReadsValue(gpiopath); }
void GPIO::SetValue(GPIO::Value value) { WritesValue(gpiopath, value); }

GPIO::Direction GPIO::GetDirection() { return direction; }
void GPIO::SetDirection(Direction direction) {
  this->direction = direction;
  WritesDirection(gpiopath, direction);
}

GPIO::Edge GPIO::GetEdge() { return edge; }
void GPIO::SetEdge(Edge edge) {
  this->edge = edge;
  WritesEdge(gpiopath, edge);
}

bool GPIO::isExported(int number __attribute__((unused)), Direction &dir,
                      Edge &edge) {
  // Checks if directory exist and therefore is exported
  if (!DirectoryExist(gpiopath)) {
    return false;
  }

  // Reads the data associated with the pin
  dir = ReadsDirection(gpiopath);
  edge = ReadsEdge(gpiopath);
  return true;
}

bool GPIO::ExportPin(int number) {
  switch (number) {
  case 7:
    system("config-pin P9.42 gpio");
    break;
  case 116:
    system("config-pin P9.91 gpio");
    break;
  case 112:
    system("config-pin P9.30 gpio");
    break;
  case 115:
    system("config-pin P9.27 gpio");
    break;
  case 14:
    system("config-pin P9.26 gpio");
    break;
  case 15:
    system("config-pin P9.24 gpio");
    break;
  case 49:
    system("config-pin P9.23 gpio");
    break;
  case 2:
    system("config-pin P9.22 gpio");
    break;
  case 3:
    system("config-pin P9.21 gpio");
    break;
  case 4:
    system("config-pin P9.18 gpio");
    break;
  case 5:
    system("config-pin P9.17 gpio");
    break;
  case 51:
    system("config-pin P9.16 gpio");
    break;
  case 48:
    system("config-pin P9.15 gpio");
    break;
  case 50:
    system("config-pin P9.14 gpio");
    break;
  case 31:
    system("config-pin P9.13 gpio");
    break;
  case 60:
    system("config-pin P9.12 gpio");
    break;
  case 30:
    system("config-pin P9.11 gpio");
    break;
  case 61:
    system("config-pin P8.26 gpio");
    break;
  case 22:
    system("config-pin P8.19 gpio");
    break;
  case 65:
    system("config-pin P8.18 gpio");
    break;
  case 27:
    system("config-pin P8.17 gpio");
    break;
  case 46:
    system("config-pin P8.16 gpio");
    break;
  case 47:
    system("config-pin P8.15 gpio");
    break;
  case 26:
    system("config-pin P8.14 gpio");
    break;
  case 23:
    system("config-pin P8.13 gpio");
    break;
  case 44:
    system("config-pin P8.12 gpio");
    break;
  case 45:
    system("config-pin P8.11 gpio");
    break;
  case 68:
    system("config-pin P8.10 gpio");
    break;
  case 69:
    system("config-pin P8.09 gpio");
    break;
  case 67:
    system("config-pin P8.08 gpio");
    break;
  case 66:
    system("config-pin P8.07 gpio");
    break;
  }
  usleep(250000);
}

bool GPIO::UnexportPin(int number) {
  //Write(UNEXPORT_PIN, NumberToString<int>(number));
}

GPIO::Direction GPIO::ReadsDirection(const string &gpiopath) {
  if (Read(gpiopath + DIRECTION) == "in") {
    return Input;
  } else {
    return Output;
  }
}

void GPIO::WritesDirection(const string &gpiopath, Direction direction) {
  switch (direction) {
  case Hardware::GPIO::Input:
    Write((gpiopath + DIRECTION), "in");
    break;
  case Hardware::GPIO::Output:
    Write((gpiopath + DIRECTION), "out");
    break;
  }
}

GPIO::Edge GPIO::ReadsEdge(const string &gpiopath) {
  string reader = Read(gpiopath + EDGE);
  if (reader == "none") {
    return None;
  } else if (reader == "rising") {
    return Rising;
  } else if (reader == "falling") {
    return Falling;
  } else {
    return Both;
  }
}

void GPIO::WritesEdge(const string &gpiopath, Edge edge) {
  switch (edge) {
  case Hardware::GPIO::None:
    Write((gpiopath + EDGE), "none");
    break;
  case Hardware::GPIO::Rising:
    Write((gpiopath + EDGE), "rising");
    break;
  case Hardware::GPIO::Falling:
    Write((gpiopath + EDGE), "falling");
    break;
  case Hardware::GPIO::Both:
    Write((gpiopath + EDGE), "both");
    break;
  default:
    break;
  }
}

GPIO::Value GPIO::ReadsValue(const string &gpiopath) {
  string path(gpiopath + VALUE);
  int res = StringToNumber<int>(Read(path));
  return (Value)res;
}

void GPIO::WritesValue(const string &gpiopath, Value value) {
  Write(gpiopath + VALUE, NumberToString<int>(value));
}

void *threadedPollGPIO(void *value) {
  GPIO *gpio = static_cast<GPIO *>(value);
  while (gpio->threadRunning) {
    gpio->callbackFunction(gpio->WaitForEdge());
    usleep(gpio->debounceTime * 1000);
  }
  return 0;
}
}
