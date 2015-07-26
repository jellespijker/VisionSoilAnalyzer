#-------------------------------------------------
#
# Project created by QtCreator 2015-06-21T19:38:09
#
#-------------------------------------------------

QT       += testlib
QMAKE_CXXFLAGS += -std=c++11

QT       -= gui

TARGET = tst_vision_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

SOURCES += tst_vision_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$PWD/../../../build/install/ -lSoilVision

INCLUDEPATH += $$PWD/../../SoilVision
DEPENDPATH += $$PWD/../../SoilVision

unix {
    #target.files += 379_blobs.ppm
    target.files += 42_blobs.ppm
    target.path += $$OUT_PWD
    INSTALLS += target
}

DISTFILES += \
    379_blobs.ppm
