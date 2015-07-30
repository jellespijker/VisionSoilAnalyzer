#
#  Project to compile QCustomPlot as shared library (.so/.dll) from the amalgamated sources
#

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QMAKE_CXXFLAGS += -std=c++11
unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

DEFINES += QCUSTOMPLOT_COMPILE_LIBRARY
TEMPLATE = lib
CONFIG += shared
VERSION = 1.3.1

TARGET = qcustomplot

SOURCES += qcustomplot.cpp
HEADERS += qcustomplot.h

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
