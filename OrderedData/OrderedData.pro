#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T12:15:26
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = OrderedData
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ClientLib/client.cpp \
    Router/connect.cpp \
    Router/router.cpp \
    cacheserver.cpp \
    main.cpp \
    NetworkManager/networkmanager.cpp \
    NetworkManager/node.cpp \
    NetworkManager/RecieveGossipTask.cpp \
    NetworkManager/SendGossipTask.cpp \
    NetworkManager/NetworkSettings.cpp

HEADERS += \
    ClientLib/client.h \
    Router/connect.h \
    Router/router.h \
    Router/typerequest.h \
    Router/RouterClient/routerclient.h \
    cacheserver.h \
    Cache/cache.h \
    Cache/icache.h \
    Cache/rbtree.h \
    NetworkManager/networkmanager.h \
    NetworkManager/node.h \
    NetworkManager/RecieveGossipTask.h \
    NetworkManager/SendGossipTask.h \
    NetworkManager/NetworkSettings.h
