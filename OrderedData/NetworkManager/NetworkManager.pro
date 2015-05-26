QT       += core
QT       += network
QT       -= gui

TARGET = Client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    NetworkSettings.cpp \
    RecieveGossipTask.cpp \
    SendGossipTask.cpp \
    networkmanager.cpp \
    node.cpp \
    main.cpp 

HEADERS += \
    NetworkSettings.h \
    RecieveGossipTask.h \
    SendGossipTask.h \
    networkmanager.h \
    node.h
    

DISTFILES += \
    settings/settings.conf