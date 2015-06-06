#-------------------------------------------------
#
# Project created by QtCreator 2015-05-20T21:18:24
#
#-------------------------------------------------

QT       += testlib
QMAKE_CXXFLAGS += -std=c++11

QT       -= gui

TARGET = tst_soilmath_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_soilmath_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
unix:!macx: LIBS += -L$$PWD/../VSA/ -lSoilMath

#opencv
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

HEADERS += \
    TestMatrix.h \
    StatisticalComparisonDefinition.h \
    FloatTestMatrix.h \
    beanhisttest.h

#Comparision Pictures
IMGtarget.path += $${OUT_PWD}/Images
IMGtarget.files += $${PWD}/../ComparisionPictures/*
INSTALLS += IMGtarget
