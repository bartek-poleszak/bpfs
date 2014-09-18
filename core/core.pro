TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    disk.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    disk.h

