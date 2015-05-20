#define QCLOSE_COMPARE(actual, expected, A) QCOMPARE((int64)((actual) / A), (int64)((expected) / A))
#define QCOMPARE_RANGE(actual, expected, endcount, type) count = 0; for_each(actual, actual + endcount,[&](type &AC){ QCOMPARE(AC, (type)expected[count++]); })
#define QCLOSE_COMPARE_RANGE(actual, expected, endcount, A, type) count = 0; for_each(actual, actual + endcount,[&](type &AC){ QCLOSE_COMPARE(AC, (type)expected[count++], A); })

#include <QString>
#include <QtTest>

#include <stdint.h>

#include "SoilMath.h"

#include "StatisticalComparisonDefinition.h"
#include "FloatTestMatrix.h"
#include "TestMatrix.h"

class SoilMath_Test : public QObject
{
    Q_OBJECT

public:
    SoilMath_Test();

private Q_SLOTS:
    void testCase_Stats_BigNoDiscrete();
    void testCase_Stats_Shifted();

private:
    uint32_t count = 0;
};

SoilMath_Test::SoilMath_Test()
{
}

void SoilMath_Test::testCase_Stats_BigNoDiscrete()
{
    uint16_t *BigNoTestMatrix = new uint16_t[40000];
    uint32_t count = 0;
    for (uint32_t i = 0; i < 200; i++)
    {
        for (uint32_t j = 0; j < 200; j++)
        {
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

void SoilMath_Test::testCase_Stats_Shifted()
{
    uint16_t *BigNoTestMatrix = new uint16_t[40000];
    uint32_t count = 0;
    for (uint32_t i = 0; i < 200; i++)
    {
        for (uint32_t j = 0; j < 200; j++)
        {
            BigNoTestMatrix[count++] = testMatrix[i][j] + 10;
        }
    }

    uint32_t *histTestResultShifted = new uint32_t[266] {};
    for (uint32_t i = 10; i < 266; i++)
    {
        histTestResultShifted[i] = histTestResult[i - 10];
    }

    uint16Stat_t Test(BigNoTestMatrix, 200, 200, 266, 0, 265);

    QCOMPARE_RANGE(Test.bins, histTestResultShifted, 266, uint32_t);
    QCLOSE_COMPARE(Test.Mean + 10, meanTestResult + 10, 0.01);
    QCOMPARE(Test.n, nTestResult);
    //QCOMPARE((uint64_t)Test.Sum, (uint64_t)(sumTestResult * 10));
    //QCOMPARE((uint32_t)Test.min, (uint32_t)(minTestResult * 10));
    //QCOMPARE((uint32_t)Test.max, (uint32_t)(maxTestResult * 10));
    //QCOMPARE((uint32_t)Test.Range, (uint32_t)(rangeTestResult * 10));
    QCLOSE_COMPARE(Test.Std, stdTestResult * 10, 0.025);

    delete[] BigNoTestMatrix;
}


QTEST_APPLESS_MAIN(SoilMath_Test)

#include "tst_soilmath_test.moc"
