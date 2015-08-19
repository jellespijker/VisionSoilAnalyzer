#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T11:59:21
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoilMath
TEMPLATE = lib
VERSION = 0.9.8

DEFINES += SOILMATH_LIBRARY
QMAKE_CXXFLAGS += -std=c++11
unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

SOURCES += \
    NN.cpp \
    GA.cpp \
    FFT.cpp

HEADERS += \
    Stats.h \
    Sort.h \
    SoilMathTypes.h \
    SoilMath.h \
    NN.h \
    MathException.h \
    GA.h \
    FFT.h \
    CommonOperations.h \
    predict_t_archive.h \
    Mat_archive.h \
    psd.h

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_serialization -lboost_iostreams

#Zlib
LIBS += -L/usr/local/lib -lz
INCLUDEPATH += /usr/local/include

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
