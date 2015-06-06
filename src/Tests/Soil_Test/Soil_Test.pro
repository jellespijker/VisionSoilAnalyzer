#-------------------------------------------------
#
# Project created by QtCreator 2015-05-24T16:11:40
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_soil_testtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#opencv
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

SOURCES += tst_soil_testtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$PWD/../VSA/ -lSoil

INCLUDEPATH += $$PWD/../Soil
DEPENDPATH += $$PWD/../Soil

#Comparision Pictures
IMGtarget.path += $${OUT_PWD}/Images
IMGtarget.files += $${PWD}/../ComparisionPictures/*
INSTALLS += IMGtarget
