#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T06:16:11
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = patcher
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    animatedlabel.cpp \
    createpatch.cpp

HEADERS  += mainwindow.h \
    animatedlabel.h \
    createpatch.h

FORMS    += mainwindow.ui \
    createpatch.ui

RESOURCES += \
    resource.qrc
