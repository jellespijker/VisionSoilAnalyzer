#include <QString>
#include <QtTest>
#include "Soil.h"

class Soil_TestTest : public QObject
{
  Q_OBJECT

public:
  Soil_TestTest();

private Q_SLOTS:
  void GetSimplePSD_testCase();
};

Soil_TestTest::Soil_TestTest()
{
}

void Soil_TestTest::GetSimplePSD_testCase()
{
  QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Soil_TestTest)

#include "tst_soil_testtest.moc"
