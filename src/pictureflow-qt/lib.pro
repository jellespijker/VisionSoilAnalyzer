TARGET = pictureflow-qt
QT += core widgets
TEMPLATE = lib
VERSION = 1.0.0
CONFIG += shared
SOURCES = pictureflow.cpp
HEADERS = pictureflow.h

#MainLib
unix {
    target.path = $PWD/../../../build/install
    INSTALLS += target
}
