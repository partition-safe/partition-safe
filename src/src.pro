#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T10:57:10
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app
INCLUDEPATH += ../lib/libpartitionsafe

SOURCES += main.cpp\
        mainwindow/mainwindow.cpp \
    mainwindow/dialogopen.cpp \
    mainwindow/dialognew.cpp \
    ../lib/libfatfs/src/diskio.h \
    ../lib/libfatfs/src/diskio.c \
    ../lib/libfatfs/src/ff.h \
    ../lib/libfatfs/src/ff.c \
    ../lib/libfatfs/src/ffconf.h \
    ../lib/libfatfs/src/integer.h \
    ../lib/libfatfs/src//option/syscall.c \
    ../lib/libfatfs/src/option/unicode.c \
    ../lib/libpartitionsafe/Partition.cpp \
    ../lib/libpartitionsafe/common.cpp \
    ../lib/libpartitionsafe/KeyStore.cpp \
    ../lib/libpartitionsafe/PartitionSafe.cpp \
    ../lib/libpartitionsafe/Vault.cpp \

HEADERS  += mainwindow/mainwindow.h \
    mainwindow/dialogopen.h \
    mainwindow/dialognew.h \
    ../lib/libpartitionsafe/Partition.h \
    ../lib/libpartitionsafe/KeyStore.h \
    ../lib/libpartitionsafe/PartitionSafe.h \
    ../lib/libpartitionsafe/Vault.h \

FORMS    += mainwindow/mainwindow.ui \
    mainwindow/dialogopen.ui \
    mainwindow/dialognew.ui \
