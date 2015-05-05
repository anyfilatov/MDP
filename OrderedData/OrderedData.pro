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
    Router/router.cpp \
    Router/connect.cpp \
    Router/RouterClient/routerclient.cpp \
    cacheserver.cpp \
    main.cpp

HEADERS += \
    ClientLib/client.h \
    Router/typerequest.h \
    Router/router.h \
    Router/connect.h \
    Router/RouterClient/routerclient.h \
    cacheserver.h \
    Cache/icache.h \
    Cache/cache.h \
    Cache/rbtree.h
    

DISTFILES += \
    settings/settings.conf
