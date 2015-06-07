/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */


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
