#-------------------------------------------------
#
# Project created by QtCreator 2015-08-20T08:46:42
#
#-------------------------------------------------

QT       += core gui concurrent network
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport multimedia multimediawidgets

@
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
@

unix:!macx: QMAKE_RPATHDIR += $$PWD/../../../build/install/

TARGET = QReportGenerator
TEMPLATE = lib
CONFIG += shared
VERSION = 0.1.00

SOURCES += \
    qreportgenerator.cpp \
    ../qcustomplot/examples/text-document-integration/qcpdocumentobject.cpp

HEADERS  += \
    qreportgenerator.h \
    ../qcustomplot/examples/text-document-integration/qcpdocumentobject.h

FORMS    += \
    qreportgenerator.ui

#MainLib
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath
DEPENDPATH += $$PWD/../SoilMath

DEFINES += QCUSTOMPLOT_USE_LIBRARY
unix:!macx: LIBS += -L$$PWD/../../build/install/ -lqcustomplot

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilAnalyzer
INCLUDEPATH += $$PWD/../SoilAnalyzer
DEPENDPATH += $$PWD/../SoilAnalyzer

unix:!macx: LIBS += -L$$PWD/../../build/install/ -lSoilVision
INCLUDEPATH += $$PWD/../SoilVision
DEPENDPATH += $$PWD/../SoilVision

RESOURCES += \
    qreportresources.qrc \
    ../VSA/vsa_resources.qrc

#maps
Mapstarget.path += $${OUT_PWD}/Maps
Mapstarget.files += $${PWD}/Maps/*
INSTALLS += Mapstarget
bMapstarget.path += $${PWD}/../../build/install/Maps
bMapstarget.files += $${PWD}/Maps/*
INSTALLS += bMapstarget
