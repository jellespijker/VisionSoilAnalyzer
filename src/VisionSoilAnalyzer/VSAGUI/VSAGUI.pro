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

SOURCES += main.cpp\
        vsagui.cpp \
    qcustomplot.cpp

HEADERS  += vsagui.h \
    qcustomplot.h \
    opencvqt.h

FORMS    += vsagui.ui

LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_system

unix:!macx: LIBS += -L$$PWD/../Hardware/Debug/ -lHardware
INCLUDEPATH += $$PWD/../Hardware/Debug
DEPENDPATH += $$PWD/../Hardware/Debug

unix:!macx: LIBS += -L$$PWD/../SoilMath/Debug/ -lSoilMath
INCLUDEPATH += $$PWD/../SoilMath/Debug
DEPENDPATH += $$PWD/../SoilMath/Debug

unix:!macx: LIBS += -L$$PWD/../Vision/Debug/ -lVision
INCLUDEPATH += $$PWD/../Vision/Debug
DEPENDPATH += $$PWD/../Vision/Debug
