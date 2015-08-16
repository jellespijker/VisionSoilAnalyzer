#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T22:02:49
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QParticleDisplay
TEMPLATE = lib
CONFIG += shared
VERSION = 1.3.21

SOURCES += qparticledisplay.cpp

HEADERS  += qparticledisplay.h

FORMS    += qparticledisplay.ui

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lpictureflow-qt

INCLUDEPATH += $$PWD/../pictureflow-qt
DEPENDPATH += $$PWD/../pictureflow-qt

#MainLib
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilAnalyzer

INCLUDEPATH += $$PWD/../SoilAnalyzer
DEPENDPATH += $$PWD/../SoilAnalyzer

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lQOpenCVQT

INCLUDEPATH += $$PWD/../QOpenCVQT
DEPENDPATH += $$PWD/../QOpenCVQT
