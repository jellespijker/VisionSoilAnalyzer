#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T10:49:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = SoilHardware
TEMPLATE = lib
VERSION = 0.9.0

DEFINES += SOILHARDWARE_LIBRARY
QMAKE_CXXFLAGS += -std=c++11 -pthread

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
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
