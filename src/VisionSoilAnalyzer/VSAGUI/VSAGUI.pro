#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T16:56:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VSAGUI
TEMPLATE = app


SOURCES += main.cpp\
        vsagui.cpp

HEADERS  += vsagui.h

FORMS    += vsagui.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../SoilMath/Debug/release/ -lSoilMath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../SoilMath/Debug/debug/ -lSoilMath
else:unix: LIBS += -L$$PWD/../SoilMath/Debug/ -lSoilMath

INCLUDEPATH += $$PWD/../SoilMath/Debug
DEPENDPATH += $$PWD/../SoilMath/Debug
