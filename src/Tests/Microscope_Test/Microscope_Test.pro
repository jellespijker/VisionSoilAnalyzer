TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

QMAKE_CXXFLAGS += -std=c++11

unix:!macx: LIBS += -L$$PWD/../../../build/install/ -lSoilHardware

INCLUDEPATH += $$PWD/../../SoilHardware
DEPENDPATH += $$PWD/../../SoilHardware

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_system

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_photo -lopencv_imgcodecs -lopencv_videoio
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include
