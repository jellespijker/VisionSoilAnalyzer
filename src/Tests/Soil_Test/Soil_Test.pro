#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T20:12:46
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = tst_soil_testtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_soil_testtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$PWD/../../../build/install/ -lSoil

INCLUDEPATH += $$PWD/../../Soil
DEPENDPATH += $$PWD/../../Soil

unix:!macx: LIBS += -L$$PWD/../../../build/install/ -lSoilVision

INCLUDEPATH += $$PWD/../../SoilVision
DEPENDPATH += $$PWD/../../SoilVision

unix:!macx: LIBS += -L$$PWD/../../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../../SoilMath
DEPENDPATH += $$PWD/../../SoilMath
