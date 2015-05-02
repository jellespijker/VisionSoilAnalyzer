#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T16:56:27
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia multimediawidgets
RESOURCES     = VSA.qrc

#NeuralNetFiles
NNtarget.path += $${OUT_PWD}/NeuralNet
NNtarget.files += $${PWD}/NeuralNet/*
INSTALLS += NNtarget

#ImageFiles
IMGtarget.path += $${OUT_PWD}/Images
IMGtarget.files += $${PWD}/Images/*
INSTALLS += IMGtarget

#SettingFiles
INItarget.path += $${OUT_PWD}/Settings
INItarget.files += $${PWD}/Settings/*
INSTALLS += INItarget

#Copy the current libs
SMtarget.path += $${OUT_PWD}/Bin
CONFIG(debug,debug|release) {
EXTRA_BINFILES += \
    ../SoilMath/Debug/libSoilMath.so \
    ../Vision/Debug/libVision.so \
    ../Soil/Debug/libSoil.so \
    ../Hardware/Debug/libHardware.so
for(FILE,EXTRA_BINFILES){ QMAKE_PRE_LINK += $$quote(cp $$PWD/$${FILE} $$OUT_PWD/Bin$$escape_expand(\\n\\t))}
}
else {
EXTRA_BINFILES += \
    ../SoilMath/Release/libSoilMath.so \
    ../Vision/Release/libVision.so \
    ../Soil/Release/libSoil.so \
    ../Hardware/Release/libHardware.so
for(FILE,EXTRA_BINFILES){ QMAKE_PRE_LINK += $$quote(cp $$PWD/$${FILE} $$OUT_PWD/Bin$$escape_expand(\\n\\t)) }
}

TARGET = VSAGUI
TEMPLATE = app

SOURCES += main.cpp\
        vsagui.cpp \
    qcustomplot.cpp \
    visionsettings.cpp

HEADERS  += vsagui.h \
    qcustomplot.h \
    opencvqt.h \
    visionsettings.h

FORMS    += vsagui.ui \
    visionsettings.ui

#opencv
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_system -lboost_serialization

#own libs
unix:!macx: LIBS += -L$${OUT_PWD}/Bin -lHardware
INCLUDEPATH += $$PWD/../Hardware
DEPENDPATH += $$PWD/../Hardware

unix:!macx: LIBS += -L$${OUT_PWD}/Bin -L$${OUT_PWD}/Bin -lSoilMath
INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

unix:!macx: LIBS += -L$${OUT_PWD}/Bin -lVision
INCLUDEPATH += $$PWD/../Vision
DEPENDPATH += $$PWD/../Vision

unix:!macx: LIBS += -L$${OUT_PWD}/Bin -lSoil
INCLUDEPATH += $$PWD/../Soil
DEPENDPATH += $$PWD/../Soil
