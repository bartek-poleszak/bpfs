TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    ../shared/blockDevice.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    blockDevice.h \
    ../shared/blockDevice.h

