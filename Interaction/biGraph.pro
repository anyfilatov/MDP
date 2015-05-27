#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T13:10:48
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = biGraph
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += main.cpp \
    ip.cpp \
    resource.cpp \
    dispetcher.cpp \
    realclients.cpp \
    client/client.cpp \
    client/Exception/abstractexception.cpp \
    client/Exception/clientexeption.cpp \
    client/Exception/connectionisnotcreatedexception.cpp \
    client/Exception/notfoundvalueexception.cpp \
    client/Exception/serverunavailableexception.cpp \
    DispatcherData/dispatcherclient.cpp \
    mywindow.cpp

HEADERS += \
    biGraph.h \
    GraphNode.h \
    dispetcher.h \
    abstractClient.h \
    resource.h \
    ip.h \
    realclients.h \
    allocator.h \
    mywindow.h \
    client/client.h \
    client/typerequest.h \
    client/StatusCodes.h \
    client/Exception/abstractexception.h \
    client/Exception/clientexeption.h \
    client/Exception/connectionisnotcreatedexception.h \
    client/Exception/notfoundvalueexception.h \
    client/Exception/serverunavailableexception.h \
    client/Exception/exception.h \
    client/Exception/exceptioncode.h \
    clientDisp/ExecutionClient.h \
    DispatcherData/dispatcherclient.h \
    threadpinger.h \
    bigraphexceptions.h \
    mymanipulators.h
