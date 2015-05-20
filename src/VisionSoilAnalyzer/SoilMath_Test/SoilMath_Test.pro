#-------------------------------------------------
#
# Project created by QtCreator 2015-05-20T21:18:24
#
#-------------------------------------------------

QT       += testlib
QMAKE_CXXFLAGS += -std=c++11

QT       -= gui

TARGET = tst_soilmath_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_soilmath_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$PWD/../VSA/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

HEADERS += \
    TestMatrix.h \
    StatisticalComparisonDefinition.h \
    FloatTestMatrix.h
