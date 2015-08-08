#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T18:57:27
#
#-------------------------------------------------
QT -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = SoilAnalyzer
TEMPLATE = lib
VERSION = 0.9.9
CONFIG = shared

DEFINES += SOILANALYZER_LIBRARY

SOURCES += \
    soilsettings.cpp \
    sample.cpp \
    particle.cpp \
    analyzer.cpp

HEADERS +=\
    soilsettings.h \
    sample.h \
    particle.h \
    analyzer.h \
    soilanalyzerexception.h

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

LIBS += -L/opt/Qt/5.5/gcc_64/lib -lQt5Core
INCLUDEPATH += /opt/Qt/5.5/gcc_64/include

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

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath
INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilVision
INCLUDEPATH += $$PWD/../SoilVision
DEPENDPATH += $$PWD/../SoilVision
