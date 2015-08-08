#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T18:56:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QParticleSelector
TEMPLATE = lib
CONFIG += shared
VERSION = 0.1.0

SOURCES += qparticleselector.cpp

HEADERS  += qparticleselector.h

FORMS    += qparticleselector.ui

RESOURCES += \
    qparticleselector.qrc

#MainLib
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
