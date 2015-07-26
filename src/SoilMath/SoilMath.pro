#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T11:59:21
#
#-------------------------------------------------

QT       -= core gui

TARGET = SoilMath
TEMPLATE = lib
VERSION = 0.9.0

DEFINES += SOILMATH_LIBRARY
QMAKE_CXXFLAGS += -std=c++11

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
    CommonOperations.h

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_serialization

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
