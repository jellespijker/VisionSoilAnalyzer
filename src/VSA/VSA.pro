#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T16:50:24
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia multimediawidgets

TARGET = VSA
TEMPLATE = app
VERSION = 0.1.0

unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

SOURCES += main.cpp\
        vsamainwindow.cpp

HEADERS  += vsamainwindow.h

FORMS    += vsamainwindow.ui

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_serialization -lboost_system -lboost_iostreams

#SoilMath lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

#SoilHardware lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilHardware

INCLUDEPATH += $$PWD/../SoilHardware
DEPENDPATH += $$PWD/../SoilHardware

#SoilVision lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilVision

INCLUDEPATH += $$PWD/../SoilVision
DEPENDPATH += $$PWD/../SoilVision

#Soil lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoil

INCLUDEPATH += $$PWD/../Soil
DEPENDPATH += $$PWD/../Soil

#QCustomplot lib
DEFINES += QCUSTOMPLOT_USE_LIBRARY
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lqcustomplot

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot

#SoilSamples
IMGtarget.path += $${OUT_PWD}/SoilSamples
IMGtarget.files += $${PWD}/SoilSamples/*
INSTALLS += IMGtarget
bIMGtarget.path += $${PWD}/../../build/install/SoilSamples
bIMGtarget.files += $${PWD}/SoilSamples/*
INSTALLS += bIMGtarget

RESOURCES += \
    vsa_resources.qrc

#MainProg
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lQParticleSelector

INCLUDEPATH += $$PWD/../QParticleSelector
DEPENDPATH += $$PWD/../QParticleSelector
