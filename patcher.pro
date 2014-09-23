#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T06:16:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = patcher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fs.cpp \
    animatedlabel.cpp

HEADERS  += mainwindow.h \
    fs.h \
    animatedlabel.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
