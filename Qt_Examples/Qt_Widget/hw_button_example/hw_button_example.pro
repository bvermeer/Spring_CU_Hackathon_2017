#-------------------------------------------------
#
# Project created by QtCreator 2017-03-16T08:35:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hw_button_example
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GPIO.cpp

HEADERS  += mainwindow.h \
    GPIO.h \
    DebugDefines.h \
    gpio_raspberrypi3.h

FORMS    += mainwindow.ui

# Target install path
target.path = /usr/bin
INSTALLS += target
