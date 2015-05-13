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
    main.cpp \
    client.cpp

HEADERS += \
    client.h 
    ../Router/typerequest.h 

DISTFILES += \
    ../settings/settings.conf \
    ../build-Client-Desktop_Qt_5_4_1_GCC_64bit-Debug/settings.conf
