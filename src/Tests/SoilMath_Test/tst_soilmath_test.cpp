/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */

#define QCLOSE_COMPARE(actual, expected, A)                                    \
  QCOMPARE((int64)((actual) / A), (int64)((expected) / A))
#define QCOMPARE_RANGE(actual, expected, endcount, type)                       \
  count = 0;                                                                   \
  for_each(actual, actual + endcount,                                          \
           [&](type & AC) { QCOMPARE(AC, (type)expected[count++]); })
#define QCLOSE_COMPARE_RANGE(actual, expected, endcount, A, type)              \
  count = 0;                                                                   \
  for_each(actual, actual + endcount,                                          \
           [&](type & AC) { QCLOSE_COMPARE(AC, (type)expected[count++], A); })

#include <QString>
#include <QtTest>

#include <stdint.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "SoilMath.h"

#include "StatisticalComparisonDefinition.h"
#include "FloatTestMatrix.h"
#include "TestMatrix.h"
#include "beanhisttest.h"

class SoilMath_Test : public QObject {
  Q_OBJECT

public:
  SoilMath_Test();

private:
  uint32_t count = 0;

private Q_SLOTS:
  void testCase_SingleValueBin();

  void testCase_PSD();

  void testCase_Sort();
  void testCase_Sort_withKey();

  void testCase_Stats_BigNoDiscrete();
  void testCase_Stats_Shifted();

  void testCase_Mat_Stats();
  void testCase_Mat_Stats_Mask();

  void testCase_benchmark_roundcast();
  void testCase_benchmark_float2intround();

  void testCase_quickPow();
  void testCase_fastPow();

  void testCase_benchmark_quickPow();
  void testCase_benchmark_fastPow();
  void testCase_benchmark_normalPow();
};

SoilMath_Test::SoilMath_Test() {}

void SoilMath_Test::testCase_SingleValueBin() {
  uchar *testData = new uchar[10] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  ucharStat_t Test;
  Test = ucharStat_t(testData, 10, 1, 3, 0, true);

  uint32_t *compBin = new uint32_t[3] {0, 10, 0};
  QCOMPARE_RANGE(Test.bins, compBin, 3, uint32_t);
  delete[] compBin;
  delete[] testData;
}

void SoilMath_Test::testCase_PSD() {
  double *testData =
      new double[20]{0.12, 0.43, 1.34, 1.2,  0.79, 0.79, 0.23, 0.24, 0.95, 1.01,
                    2.03, 4.30, 1.57, 1.23, 1.77, 1.99, 1,    0.02, 0.32, 0.55};
  double *binrange =
      new double[15]{0.0,  0.038, 0.045, 0.063, 0.075, 0.09, 0.125, 0.18,
                    0.25, 0.355, 0.5,   0.71,  1.0,   1.4,  2.0};

  uint32_t *resultBins =
      new uint32_t[15]{1, 0, 0, 0, 0, 1, 0, 2, 1, 1, 1, 4, 4, 3, 2};
  double *resultCFD =
      new double[15]{100 * 1/20, 100 * 1/20, 100 * 1/20, 100 * 1/20, 100 * 1/20, 100 * 2/20, 100 * 2/20, 100 * 4/20, 100 * 5/20, 100 * 6/20, 100 * 7/20, 100 * 11/20, 100 * 15/20, 100 * 18/20, 100 * 20/20};
  SoilMath::PSD psd(testData, 20, binrange, 15, 14);

  QCOMPARE_RANGE(psd.bins, resultBins, 15, uint32_t);
  QCOMPARE_RANGE(psd.CFD, resultCFD, 15, double);

  delete[] testData;
  delete[] binrange;
  delete[] resultBins;
  delete[] resultCFD;
}

void SoilMath_Test::testCase_Sort() {
  uint16_t compArr[10] = {1, 2, 4, 4, 5, 6, 7, 10, 10, 10};
  uint16_t startArr[10] = {10, 2, 5, 1, 7, 10, 4, 10, 4, 6};
  SoilMath::Sort::QuickSort<uint16_t>(startArr, 10);

  QCOMPARE_RANGE(startArr, compArr, 10, uint16_t);

  QBENCHMARK {
    uint16_t sArr[10] = {10, 2, 5, 1, 7, 10, 4, 10, 4, 6};
    SoilMath::Sort::QuickSort<uint16_t>(sArr, 10);
  }
}

void SoilMath_Test::testCase_Sort_withKey() {
  uint16_t compArr[10] = {1, 2, 4, 4, 5, 6, 7, 10, 10, 10};
  uint16_t compKeyArr[10] = {3, 1, 6, 8, 2, 9, 4, 7, 5, 0};
  uint16_t startArr[10] = {10, 2, 5, 1, 7, 10, 4, 10, 4, 6};
  uint16_t startKeyarr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  SoilMath::Sort::QuickSort<uint16_t>(startArr, startKeyarr, 10);
  QCOMPARE_RANGE(startArr, compArr, 10, uint16_t);
  QCOMPARE_RANGE(startKeyarr, compKeyArr, 10, uint16_t);

  QBENCHMARK {
    uint16_t sArr[10] = {10, 2, 5, 1, 7, 10, 4, 10, 4, 6};
    uint16_t sKeyarr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    SoilMath::Sort::QuickSort<uint16_t>(sArr, sKeyarr, 10);
  }
}

void SoilMath_Test::testCase_Stats_BigNoDiscrete() {
  uint16_t *BigNoTestMatrix = new uint16_t[40000];
  uint32_t count = 0;
  for (uint32_t i = 0; i < 200; i++) {
    for (uint32_t j = 0; j < 200; j++) {
      BigNoTestMatrix[count++] = testMatrix[i][j] * 10;
    }
  }

  uint16Stat_t Test(BigNoTestMatrix, 200, 200);

  QCOMPARE_RANGE(Test.bins, histTestResult, 255, uint32_t);
  QCLOSE_COMPARE(Test.Mean, meanTestResult * 10, 0.01);
  QCOMPARE(Test.n, nTestResult);
  QCOMPARE((uint64_t)Test.Sum, (uint64_t)(sumTestResult * 10));
  QCOMPARE((uint32_t)Test.min, (uint32_t)(minTestResult * 10));
  QCOMPARE((uint32_t)Test.max, (uint32_t)(maxTestResult * 10));
  QCOMPARE((uint32_t)Test.Range, (uint32_t)(rangeTestResult * 10));
  QCLOSE_COMPARE(Test.Std, stdTestResult * 10, 0.025);

  delete[] BigNoTestMatrix;
}

void SoilMath_Test::testCase_Stats_Shifted() {
  uint16_t *BigNoTestMatrix = new uint16_t[40000];
  uint32_t count = 0;
  for (uint32_t i = 0; i < 200; i++) {
    for (uint32_t j = 0; j < 200; j++) {
      BigNoTestMatrix[count++] = testMatrix[i][j] + 10;
    }
  }

  uint32_t *histTestResultShifted = new uint32_t[266]{};
  for (uint32_t i = 10; i < 266; i++) {
    histTestResultShifted[i] = histTestResult[i - 10];
  }

  uint16Stat_t Test(BigNoTestMatrix, 200, 200, 266, 0, 265);

  QCOMPARE_RANGE(Test.bins, histTestResultShifted, 266, uint32_t);
  QCLOSE_COMPARE(Test.Mean, meanTestResult + 10, 0.01);
  QCOMPARE(Test.n, nTestResult);
  QCLOSE_COMPARE(Test.Std, stdTestResult, 0.025);

  delete[] BigNoTestMatrix;
}

void SoilMath_Test::testCase_Mat_Stats() {
  cv::Mat beans = cv::imread("Images/beans.ppm", 0);
  ucharStat_t Test((uchar *)beans.data, beans.cols, beans.rows);
  QCLOSE_COMPARE(Test.Mean, actualMeanBean, 0.001);
  QCOMPARE_RANGE(Test.bins, actualHistBean, 256, uint32_t);
  QCOMPARE(Test.min, actualminBean);
  QCOMPARE(Test.max, actualMaxBean);
  QCLOSE_COMPARE(Test.Std, actualStdBean, 0.01);
  QCOMPARE((uint64_t)Test.Sum, (uint64_t)actualSumBean);

  QBENCHMARK {
    ucharStat_t TestBench((uchar *)beans.data, beans.cols, beans.rows);
  }
}

void SoilMath_Test::testCase_Mat_Stats_Mask() {
  cv::Mat beans = cv::imread("Images/beans.ppm", 0);
  cv::Mat mask = cv::imread("Images/beansmask.ppm", 0);
  ucharStat_t Test((uchar *)beans.data, beans.cols, beans.rows,
                   (uchar *)mask.data);
  QCLOSE_COMPARE(Test.Mean, acutalMeanBeanMask, 0.001);
  QCOMPARE_RANGE(Test.bins, actualHistBeanMask, 256, uint32_t);
  QCOMPARE(Test.min, actualminBeanMask);
  QCOMPARE(Test.max, actualMaxBeanMask);
  QCOMPARE((uint64_t)Test.Sum, (uint64_t)actualSumBeanMask);
  QCLOSE_COMPARE(Test.Std, actualStdBeanMask, 0.01);
  QBENCHMARK {
    ucharStat_t TestBench((uchar *)beans.data, beans.cols, beans.rows,
                          (uchar *)mask.data);
  }
}

void SoilMath_Test::testCase_benchmark_roundcast() {
  float testf = 254.5;

  QBENCHMARK { static_cast<int>(std::round(testf)); }
}

void SoilMath_Test::testCase_benchmark_float2intround() {
  float testf = 254.5;

  QBENCHMARK { SoilMath::float2intRound(testf); }
}

void SoilMath_Test::testCase_quickPow() {
  for (int i = -255; i < 256; i++) {
    QCLOSE_COMPARE(SoilMath::quick_pow2(i), std::pow(i, 2), 0.0001);
  }

  QBENCHMARK { SoilMath::quick_pow2(127); }
}

void SoilMath_Test::testCase_fastPow() {
  for (int i = 1; i < 256; i++) {
    QCLOSE_COMPARE(SoilMath::fastPow(i, 2), std::pow(i, 2), 1.2);
  }

  QBENCHMARK { SoilMath::fastPow(127, 2); }
}

void SoilMath_Test::testCase_benchmark_quickPow() {
  QBENCHMARK { SoilMath::quick_pow2(127); }
}

void SoilMath_Test::testCase_benchmark_fastPow() {
  QBENCHMARK { SoilMath::fastPow(127, 2); }
}

void SoilMath_Test::testCase_benchmark_normalPow() {
  QBENCHMARK { std::pow(127, 2); }
}

QTEST_APPLESS_MAIN(SoilMath_Test)

#include "tst_soilmath_test.moc"
