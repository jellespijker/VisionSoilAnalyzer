#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T16:50:24
#
#
#-------------------------------------------------

QT       += core gui concurrent
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia multimediawidgets

TARGET = VSA
TEMPLATE = app
VERSION = 0.9.7

unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

SOURCES += main.cpp\
        vsamainwindow.cpp \
    dialogsettings.cpp \
    dialognn.cpp

HEADERS  += vsamainwindow.h \
    dialogsettings.h \
    dialognn.h

FORMS    += vsamainwindow.ui \
    dialogsettings.ui \
    dialognn.ui

#opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#boost
DEFINES += BOOST_ALL_DYN_LINK
INCLUDEPATH += /usr/include/boost
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_serialization -lboost_system -lboost_iostreams

#SoilMath lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

#SoilHardware lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilHardware

INCLUDEPATH += $$PWD/../SoilHardware
DEPENDPATH += $$PWD/../SoilHardware

#SoilVision lib
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilVision

INCLUDEPATH += $$PWD/../SoilVision
DEPENDPATH += $$PWD/../SoilVision

#QCustomplot lib
DEFINES += QCUSTOMPLOT_USE_LIBRARY
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lqcustomplot

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot

#NeuralNetFiles
NNtarget.path += $${OUT_PWD}/NeuralNet
NNtarget.files += $${PWD}/NeuralNet/*.NN
INSTALLS += NNtarget
bNNtarget.path += $${PWD}/../../build/install/NeuralNet
bNNtarget.files += $${PWD}/NeuralNet/*.NN
INSTALLS += bNNtarget

#SettingFiles
INItarget.path += $${OUT_PWD}/Settings
INItarget.files += $${PWD}/Settings/*.ini
INSTALLS += INItarget
bINItarget.path += $$PWD/../../build/install/Settings
bINItarget.files += $$PWD/Settings/*.ini
INSTALLS += bINItarget

#SoilSamples
IMGtarget.path += $${OUT_PWD}/SoilSamples
IMGtarget.files += $${PWD}/SoilSamples/*.VSA
INSTALLS += IMGtarget
bIMGtarget.path += $${PWD}/../../build/install/SoilSamples
bIMGtarget.files += $${PWD}/SoilSamples/*.VSA
INSTALLS += bIMGtarget

#Images
Imgtarget.path += $${OUT_PWD}/Images
Imgtarget.files += $${PWD}/Images/*
INSTALLS += Imgtarget
bImgtarget.path += $${PWD}/../../build/install/Images
bImgtarget.files += $${PWD}/Images/*
INSTALLS += bImgtarget

RESOURCES += \
    vsa_resources.qrc

#MainProg
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lQParticleSelector

INCLUDEPATH += $$PWD/../QParticleSelector
DEPENDPATH += $$PWD/../QParticleSelector

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lQParticleDisplay

INCLUDEPATH += $$PWD/../QParticleDisplay
DEPENDPATH += $$PWD/../QParticleDisplay

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lQOpenCVQT

INCLUDEPATH += $$PWD/../QOpenCVQT
DEPENDPATH += $$PWD/../QOpenCVQT

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilAnalyzer

INCLUDEPATH += $$PWD/../SoilAnalyzer
DEPENDPATH += $$PWD/../SoilAnalyzer

DISTFILES += \
    Settings/Default.ini \
    NeuralNet/Default.NN \
    Settings/User.ini \
    SoilSamples/Eurogrit_B3_01__Cat.VSA \
    SoilSamples/Gran_K1_0.5_2.5__01_Cat.VSA
