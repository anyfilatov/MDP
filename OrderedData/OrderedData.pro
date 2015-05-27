#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T12:15:26
#
#-------------------------------------------------

QT       += core concurrent
QT       += network
QT       -= gui

TARGET = OrderedData
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += \
    Router/router.cpp \
    cacheserver.cpp \
    main.cpp \
    NetworkManager/networkmanager.cpp \
    NetworkManager/node.cpp \
    NetworkManager/RecieveGossipTask.cpp \
    NetworkManager/SendGossipTask.cpp \
    NetworkManager/NetworkSettings.cpp \
    Router/hashring.cpp \
    Router/RouterClient/routerclient.cpp \
    Exception/abstractexception.cpp \
    Exception/clientexeption.cpp \
    Exception/serverunavailableexception.cpp \
    Exception/connectionisnotcreatedexception.cpp \
    Exception/notfoundvalueexception.cpp \
    ClientLib/client.cpp \
    ClientLib/abstractclient.cpp \
    Router/connection.cpp

HEADERS += \
    Router/router.h \
    ClientLib/typerequest.h \
    Router/RouterClient/routerclient.h \
    cacheserver.h \
    Cache/cache.h \
    Cache/icache.h \
    Cache/rbtree.h \
    Cache/tnode.h \
    Cache/qrbtree.h \
    Cache/qrbnode.h \
    NetworkManager/networkmanager.h \
    NetworkManager/node.h \
    NetworkManager/RecieveGossipTask.h \
    NetworkManager/SendGossipTask.h \
    NetworkManager/NetworkSettings.h \
    Router/hashring.h \
    ClientLib/StatusCodes.h \
    Exception/abstractexception.h \
    Exception/exceptioncode.h \
    Exception/clientexeption.h \
    Exception/serverunavailableexception.h \
    Exception/connectionisnotcreatedexception.h \
    Exception/exception.h \
    Exception/notfoundvalueexception.h \
    ClientLib/abstractclient.h \
    ClientLib/client.h \
    Router/connection.h
