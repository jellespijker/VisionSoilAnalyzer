/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#pragma once
#include <stdint.h>

namespace SoilMath {
/*!
 * \brief The Sort template class
 */
class Sort {
public:
  Sort() {}
  ~Sort() {}

  /*!
   * \brief QuickSort a static sort a Type T array with i values
   * \details Usage: QuickSort<type>(*type , i)
   * \param arr an array of Type T
   * \param i the number of elements
   */
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
        T t = *l;
        *l = *r;
        *r = t;
        l++;
        r--;
      }
    }
    Sort::QuickSort<T>(arr, r - arr + 1);
    Sort::QuickSort<T>(l, arr + i - l);
  }

  /*!
   * \brief QuickSort a static sort a Type T array with i values where the key
   * are also changed accordingly
   * \details Usage: QuickSort<type>(*type *type , i)
   * \param arr an array of Type T
   * \param key an array of 0..i-1 representing the index
   * \param i the number of elements
   */
  template <typename T> static void QuickSort(T *arr, T *key, int i) {
    if (i < 2)
      return;

    T p = arr[i / 2];

    T *l = arr;
    T *r = arr + i - 1;

    T *lkey = key;
    T *rkey = key + i - 1;

    while (l <= r) {
      if (*l < p) {
        l++;
        lkey++;
      } else if (*r > p) {
        r--;
        rkey--;
      } else {
       if (*l != *r) {
          T t = *l;
          *l = *r;
          *r = t;

          T tkey = *lkey;
          *lkey = *rkey;
          *rkey = tkey;
        }

        l++;
        r--;

        lkey++;
        rkey--;
      }
    }
    Sort::QuickSort<T>(arr, key, r - arr + 1);
    Sort::QuickSort<T>(l, lkey, arr + i - l);
  }
};
}
