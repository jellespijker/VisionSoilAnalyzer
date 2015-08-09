#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T12:07:42
#
#-------------------------------------------------

QT       -= core gui
QMAKE_CXXFLAGS += -std=c++11

TARGET = SoilVision
TEMPLATE = lib
VERSION = 0.9.0

DEFINES += SOILVISION_LIBRARY
unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

SOURCES += \
    Segment.cpp \
    MorphologicalFilter.cpp \
    ImageProcessing.cpp \
    Enhance.cpp \
    Conversion.cpp

HEADERS += \
    WrongKernelSizeException.h \
    VisionDebug.h \
    Vision.h \
    Segment.h \
    PixelValueOutOfBoundException.h \
    MorphologicalFilter.h \
    ImageProcessing.h \
    Enhance.h \
    EmptyImageException.h \
    ConversionNotSupportedException.h \
    Conversion.h \
    ChannelMismatchException.h \
    ../SoilMath/Mat_archive.h

unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath
