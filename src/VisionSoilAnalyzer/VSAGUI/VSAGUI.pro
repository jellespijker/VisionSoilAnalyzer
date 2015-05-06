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
bNNtarget.path += $${PWD}/../VSA/NeuralNet
bNNtarget.files += $${PWD}/NeuralNet/*
INSTALLS += bNNtarget

#ImageFiles
IMGtarget.path += $${OUT_PWD}/Images
IMGtarget.files += $${PWD}/Images/*
INSTALLS += IMGtarget
bIMGtarget.path += $${PWD}/../VSA/Images
bIMGtarget.files += $${PWD}/Images/*
INSTALLS += bIMGtarget

#SettingFiles
INItarget.path += $${OUT_PWD}/Settings
INItarget.files += $${PWD}/Settings/*
INSTALLS += INItarget
bINItarget.path += $${PWD}/../VSA/Settings
bINItarget.files += $${PWD}/Settings/*
INSTALLS += bINItarget

#MainProg
VSAtarget.path += $${PWD}/../VSA
VSAtarget.files += $${OUT_PWD}/VSAGUI
INSTALLS += VSAtarget

TARGET = VSAGUI
TEMPLATE = app

SOURCES += main.cpp\
        vsagui.cpp \
    qcustomplot.cpp \
    visionsettings.cpp \
    hardwaresettings.cpp

HEADERS  += vsagui.h \
    qcustomplot.h \
    opencvqt.h \
    visionsettings.h \
    hardwaresettings.h

FORMS    += vsagui.ui \
    visionsettings.ui \
    hardwaresettings.ui

#opencv
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_imgproc -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_system -lboost_serialization

OTHER_FILES += \
    Settings/Default.ini \
    NeuralNet/Default.NN \
    Images/SoilSample1.png

#own Libs

unix:!macx: LIBS += -L$$PWD/../VSA/ -lHardware

INCLUDEPATH += $$PWD/../Hardware
DEPENDPATH += $$PWD/../Hardware

unix:!macx: LIBS += -L$$PWD/../VSA/ -lSoil

INCLUDEPATH += $$PWD/../Soil
DEPENDPATH += $$PWD/../Soil

unix:!macx: LIBS += -L$$PWD/../VSA/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

unix:!macx: LIBS += -L$$PWD/../VSA/ -lVision

INCLUDEPATH += $$PWD/../Vision
DEPENDPATH += $$PWD/../Vision
