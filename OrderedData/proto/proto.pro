#-------------------------------------------------
#
# Project created by QtCreator 2015-03-14T11:49:09
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = proto
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    tcpserver.cpp \
    main.cpp

HEADERS += \
    storage.h \
    prototypestorage.h \
    tcpserver.h \
