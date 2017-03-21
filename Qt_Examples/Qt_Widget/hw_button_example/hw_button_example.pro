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
    hw_button_polling.cpp \
    tiny_gpio.cpp

HEADERS  += mainwindow.h \
    DebugDefines.h \
    gpio_raspberrypi3.h \
    hw_button_polling.h \
    tiny_gpio.h

FORMS    += mainwindow.ui

# Target install path
target.path = /home/keysight/bin
INSTALLS += target
