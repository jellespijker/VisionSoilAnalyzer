#include <QString>
#include <QtTest>

class Soil_TestTest : public QObject
{
    Q_OBJECT

public:
    Soil_TestTest();

private Q_SLOTS:
    void testCase1();
};

Soil_TestTest::Soil_TestTest()
{
}

void Soil_TestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Soil_TestTest)

#include "tst_soil_testtest.moc"
