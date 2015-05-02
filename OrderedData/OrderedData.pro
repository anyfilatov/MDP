#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T12:15:26
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = Client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    Router/RouterClient/routerclient.cpp

HEADERS += \
    ClientLib/client.h \
    Router/typerequest.h \
    Router/RouterClient/routerclient.h
    

DISTFILES += \
    settings/settings.conf
