#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T10:57:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow/mainwindow.cpp \
    lib/psfilesystemmodel/psfilesystemmodel.cpp

HEADERS  += mainwindow/mainwindow.h \
    lib/psfilesystemmodel/psfilesystemmodel.h

FORMS    += mainwindow/mainwindow.ui
