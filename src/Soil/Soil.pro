#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T12:18:09
#
#-------------------------------------------------

QT       -= core gui
QMAKE_CXXFLAGS += -std=c++11
unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

TARGET = Soil
TEMPLATE = lib
VERSION = 0.9.0

DEFINES += SOIL_LIBRARY

SOURCES += \
    soilsettings.cpp \
    Soil.cpp \
    ShapeClassification.cpp \
    SampleAnalysisResult.cpp \
    Sample.cpp \
    ParticleAnalysisResults.cpp \
    Particle.cpp \
    AnalysisResults.cpp

HEADERS += \
    VisionSoil.h \
    soilsettings.h \
    Soil.h \
    ShapeClassification.h \
    SampleAnalysisResult.h \
    Sample.h \
    ParticleAnalysisResults.h \
    Particle.h \
    Mat_archive.h \
    AnalysisResults.h \
    AnalysisException.h \
    AnalyseType.h

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_serialization

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilVision

INCLUDEPATH += $$PWD/../SoilVision
DEPENDPATH += $$PWD/../SoilVision
