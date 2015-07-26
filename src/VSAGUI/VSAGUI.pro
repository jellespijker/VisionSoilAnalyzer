#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T16:56:27
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia multimediawidgets
RESOURCES     = VSA.qrc

TARGET = VSAGUI
TEMPLATE = app
VERSION = 0.9.0

SOURCES += main.cpp\
        vsagui.cpp \
    visionsettings.cpp \
    hardwaresettings.cpp

HEADERS  += vsagui.h \
    opencvqt.h \
    visionsettings.h \
    hardwaresettings.h

FORMS    += vsagui.ui \
    visionsettings.ui \
    hardwaresettings.ui

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_serialization -lboost_system

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

OTHER_FILES += \
    Settings/Default.ini \
    NeuralNet/Default.NN \
    Images/SoilSample1.png

#NeuralNetFiles
NNtarget.path += $${OUT_PWD}/NeuralNet
NNtarget.files += $${PWD}/NeuralNet/*
INSTALLS += NNtarget
bNNtarget.path += $${PWD}/../../build/install/NeuralNet
bNNtarget.files += $${PWD}/NeuralNet/*
INSTALLS += bNNtarget

#ImageFiles
IMGtarget.path += $${OUT_PWD}/Images
IMGtarget.files += $${PWD}/Images/*
INSTALLS += IMGtarget
bIMGtarget.path += $${PWD}/../../build/install/Images
bIMGtarget.files += $${PWD}/Images/*
INSTALLS += bIMGtarget

#SettingFiles
INItarget.path += $${OUT_PWD}/Settings
INItarget.files += $${PWD}/Settings/*
INSTALLS += INItarget
bINItarget.path += $PWD/../../build/install/Settings
bINItarget.files += $PWD/Settings/*
INSTALLS += bINItarget

#MainProg
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

DISTFILES += \
    Images/sample.ppm
