#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T19:27:25
#
#-------------------------------------------------

QT       += core gui network widgets



TARGET = Database
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Data.cpp \
    dbclient.cpp \
    Dispatcher/Dispatcher.cpp \
    Dispatcher/Session.cpp \
    Dispatcher/TableInfoHandler.cpp \
    HashTable/IntWithHash.cpp \
    HashTable/StringWithHash.cpp \
    HashTable/TableKey.cpp \
    GUIFileParser/GuiFileParser.cpp \
    gui.cpp

HEADERS += \
    Clonable.h \
    Serializible.h \
    Data.h \
    dbclient.h \
    Dispatcher/Dispatcher.h \
    Dispatcher/Session.h \
    Dispatcher/TableInfoHandler.h \
    HashTable/AbstractTableKey.h \
    HashTable/CellItem.h \
    HashTable/Hashable.h \
    HashTable/HashTable.h \
    HashTable/IntWithHash.h \
    HashTable/StringWithHash.h \
    HashTable/TableCell.h \
    HashTable/TableKey.h \
    GUIFileParser/GuiFileParser.h \
    gui.h
