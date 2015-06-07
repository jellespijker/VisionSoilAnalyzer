/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include <stdint.h>

namespace SoilMath {
class Sort {
public:
  Sort() {}
  ~Sort() {}

  template <typename T> static void QuickSort(T *arr, int i) {
    if (i < 2)
      return;

    T p = arr[i / 2];
    T *l = arr;
    T *r = arr + i - 1;
    while (l <= r) {
      if (*l < p) {
        l++;
      } else if (*r > p) {
        r--;
      } else {
        int t = *l;
        *l = *r;
        *r = t;
        l++;
        r--;
      }
    }
    Sort::QuickSort<T>(arr, r - arr + 1);
    Sort::QuickSort<T>(l, arr + i - l);
  }
};
}
