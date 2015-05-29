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
CONFIG   += c++11

INCLUDEPATH += ..

TEMPLATE = app


SOURCES += \ 
    main.cpp \
    ../Exception/abstractexception.cpp \
    ../Exception/clientexeption.cpp \
    ../Exception/connectionisnotcreatedexception.cpp \
    ../Exception/notfoundvalueexception.cpp \
    ../Exception/serverunavailableexception.cpp \
    abstractclient.cpp \
    client.cpp \
    server.cpp




HEADERS += \
#    typerequest.h \
#    StatusCodes.h \
    ../Exception/abstractexception.h \
    ../Exception/clientexeption.h \
    ../Exception/connectionisnotcreatedexception.h \
    ../Exception/notfoundvalueexception.h \
    ../Exception/serverunavailableexception.h \
    ../Exception/exception.h \
    ../Exception/exceptioncode.h \
    abstractclient.h \
    client.h \
    server.h


