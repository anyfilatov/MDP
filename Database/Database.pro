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
    Dispatcher/HardDiskWorker.cpp \
    Dispatcher/MetaData.cpp \
    HashTable/IntWithHash.cpp \
    HashTable/StringWithHash.cpp \
    HashTable/TableKey.cpp \
    GUI/GUIFileParser/GuiFileParser.cpp \
    GUI/gui.cpp

HEADERS += \
    Clonable.h \
    Serializible.h \
    Data.h \
    dbclient.h \
    Dispatcher/Dispatcher.h \
    Dispatcher/HardDiskWorker.h \
    Dispatcher/MetaData.h \
    HashTable/AbstractTableKey.h \
    HashTable/CellItem.h \
    HashTable/Hashable.h \
    HashTable/HashTable.h \
    HashTable/IntWithHash.h \
    HashTable/StringWithHash.h \
    HashTable/TableCell.h \
    HashTable/TableKey.h \
    GUI/GUIFileParser/GuiFileParser.h \
    GUI/gui.h
