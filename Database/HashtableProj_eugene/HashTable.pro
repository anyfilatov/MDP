#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T19:27:25
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = HashTable
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Dispatcher.cpp \
    StringWithHash.cpp \
    IntWithHash.cpp \
    TableKey.cpp \
    Data.cpp \
    dbclient.cpp

HEADERS += \
    Hashable.h \
    IntWithHash.h \
    Clonable.h \
    CellItem.h \
    TableCell.h \
    AbstractTableKey.h \
    HashTable.h \
    StringWithHash.h \
    Serializible.h \
    TableKey.h \
    Data.h \
    Dispatcher.h \
    dbclient.h
