#-------------------------------------------------
#
# Project created by QtCreator 2017-04-05T20:49:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MCP3008_ADC_Example
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mcp3008Spi.cpp

HEADERS  += mainwindow.h \
    mcp3008Spi.h

FORMS    += mainwindow.ui

# Target install path
target.path = /home/keysight/bin
INSTALLS += target
