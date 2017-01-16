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

RESOURCES     = application.qrc \
    application.qrc

LIBS += ../cmake-build-debug/libpslib.a \
        ../cmake-build-debug/lib/libmbedtls/library/libmbedcrypto.a

unix {
    LIBS += -ldl
}

SOURCES += main.cpp \
        mainwindow/mainwindow.cpp \
    mainwindow/dialogopen.cpp \
    mainwindow/dialognew.cpp \
    mainwindow/dialognewdirectory.cpp \
    mainwindow/dialognewuser.cpp \
    mainwindow/dialognotifications.cpp \
    maintreeviewfiles.cpp \
    mainwindow/dialogrename.cpp \
    lib/PSFileSystemModel.cpp \
    lib/pstreefilesystemmodel.cpp

HEADERS  += mainwindow/mainwindow.h \
    mainwindow/dialogopen.h \
    mainwindow/dialognew.h \
    mainwindow/dialognewdirectory.h \
    mainwindow/dialognewuser.h \
    maintreeviewfiles.h \
    mainwindow/dialognotifications.h \
    mainwindow/dialogrename.h \
    lib/PSFileSystemModel.h \
    lib/pstreefilesystemmodel.h

FORMS    += mainwindow/mainwindow.ui \
    mainwindow/dialogopen.ui \
    mainwindow/dialognew.ui \
    mainwindow/dialognewuser.ui \
    mainwindow/dialognotifications.ui \
    mainwindow/dialognewdirectory.ui \
    mainwindow/dialogrename.ui
