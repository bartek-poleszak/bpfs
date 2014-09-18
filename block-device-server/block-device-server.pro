TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    blockDevice.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    blockDevice.h

