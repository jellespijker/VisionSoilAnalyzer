#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T10:49:23
#
#-------------------------------------------------
QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoilHardware
TEMPLATE = lib
VERSION = 0.9.3

DEFINES += SOILHARDWARE_LIBRARY
QMAKE_CXXFLAGS += -std=c++11 -pthread
unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

SOURCES += \
    USB.cpp \
    SoilCape.cpp \
    PWM.cpp \
    Microscope.cpp \
    GPIO.cpp \
    eqep.cpp \
    EC12P.cpp \
    BBB.cpp \
    ADC.cpp

HEADERS += \
    ValueOutOfBoundsException.h \
    USB.h \
    SoilCape.h \
    PWM.h \
    MicroscopeNotFoundException.h \
    Microscope.h \
    Hardware.h \
    GPIOReadException.h \
    GPIO.h \
    FailedToCreateThreadException.h \
    FailedToCreateGPIOPollingThreadException.h \
    eqep.h \
    EC12P.h \
    CouldNotGrabImageException.h \
    BBB.h \
    ADCReadException.h \
    ADC.h

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_photo -lopencv_imgcodecs -lopencv_videoio
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost

contains(QT_ARCH, x86_64) {
    LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_system
}
contains(QT_ARCH, arm) {
    LIBS += -L/usr/lib/arm-linux-gnueabihf/ -lboost_filesystem -lboost_system
}

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

#Gstreamer
INCLUDEPATH += /usr/include/gstreamer-0.10
INCLUDEPATH += /usr/include/glib-2.0/
contains(QT_ARCH, x86_64) {
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include/
}
contains(QT_ARCH, arm) {
    INCLUDEPATH += //usr/lib/arm-linux-gnueabihf/glib-2.0/include
}

INCLUDEPATH += /usr/include/libxml2/
LIBS += `pkg-config --cflags --libs gstreamer-0.10`
