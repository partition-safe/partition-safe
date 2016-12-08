#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T10:57:10
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app
INCLUDEPATH += ../lib/libpartitionsafe

LIBS += ./src/lib/libPartitionSafe-Lib.a

SOURCES += main.cpp\
        mainwindow/mainwindow.cpp \
    mainwindow/dialogopen.cpp \
    mainwindow/dialognew.cpp \
    lib/PSFileSystemModel.cpp \

HEADERS  += mainwindow/mainwindow.h \
    mainwindow/dialogopen.h \
    mainwindow/dialognew.h \
    lib/PSFileSystemModel.h \


FORMS    += mainwindow/mainwindow.ui \
    mainwindow/dialogopen.ui \
    mainwindow/dialognew.ui \
