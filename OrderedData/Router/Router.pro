#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T14:48:43
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = Router
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    router.cpp \
    connect.cpp \
    RouterClient/routerclient.cpp

HEADERS += \
    router.h \
    connect.h \
    typerequest.h \
    RouterClient/routerclient.h
