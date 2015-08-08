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
VERSIOn = 1.0.0

SOURCES += qparticledisplay.cpp

HEADERS  += qparticledisplay.h

FORMS    += qparticledisplay.ui

#MainLib
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

#Soil lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoil

INCLUDEPATH += $$PWD/../Soil
DEPENDPATH += $$PWD/../Soil

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lQOpenCVQT

INCLUDEPATH += $$PWD/../QOpenCVQT
DEPENDPATH += $$PWD/../QOpenCVQT

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lpictureflow-qt

INCLUDEPATH += $$PWD/../pictureflow-qt
DEPENDPATH += $$PWD/../pictureflow-qt
