#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T08:11:34
#
#-------------------------------------------------

TARGET = QOpenCVQT
TEMPLATE = lib

QT += gui

DEFINES += QOPENCVQT_LIBRARY
VERSION = 1.0.0
CONFIG += shared

SOURCES += qopencvqt.cpp

HEADERS += qopencvqt.h

#opencv
LIBS += -L/usr/local/lib -lopencv_core
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#MainLib
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
