#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T12:15:26
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = Cache
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += \
    main.cpp
    

HEADERS += \
    icache.h \
    cache.h \
    rbtree.h \
    qrbtree.h \
    tnode.h \
    qrbnode.h
