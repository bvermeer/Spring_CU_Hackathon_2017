#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T15:14:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = show_ip_widget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DISTFILES +=

# Target install path
target.path = /usr/bin
INSTALLS += target

RESOURCES += \
    resources.qrc
