#-------------------------------------------------
#
# Project created by QtCreator 2015-05-25T17:33:03
#
#-------------------------------------------------

QT       -= gui
QMAKE_CXXFLAGS += -std=c++11

TARGET = TestParticleGenerator
TEMPLATE = lib

DEFINES += TESTPARTICLEGENERATOR_LIBRARY

SOURCES += testparticlegenerator.cpp

HEADERS += testparticlegenerator.h\
        testparticlegenerator_global.h

#opencv
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

unix {
    target.path = /usr/lib
    INSTALLS += target
}
