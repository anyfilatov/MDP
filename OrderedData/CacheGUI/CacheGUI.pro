#-------------------------------------------------
#
# Project created by QtCreator 2015-05-24T14:39:32
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CacheGUI
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += ..


SOURCES += main.cpp\
        MainWindow.cpp \
    CacheInfoWidget.cpp \
    CacheInfo.cpp \
    CacheInfoService.cpp \
    DiagramWidget.cpp \
    TabsWidget.cpp \
    ../ClientLib/guiclient.cpp \
    ../ClientLib/abstractclient.cpp \
    ../Exception/abstractexception.cpp \
    ../Exception/clientexeption.cpp \
    ../Exception/serverunavailableexception.cpp \
    ../Exception/connectionisnotcreatedexception.cpp \
    ../Exception/notfoundvalueexception.cpp

HEADERS  += MainWindow.h \
    CacheInfoWidget.h \
    CacheInfo.h \
    CacheInfoService.h \
    DiagramWidget.h \
    TabsWidget.h \
    ../ClientLib/guiclient.h \
    ../ClientLib/abstractclient.h \
    ../Exception/abstractexception.h \
    ../Exception/exceptioncode.h \
    ../Exception/clientexeption.h \
    ../Exception/serverunavailableexception.h \
    ../Exception/connectionisnotcreatedexception.h \
    ../Exception/exception.h \
    ../Exception/notfoundvalueexception.h

FORMS    += MainWindow.ui \
    CacheInfoWidget.ui

DISTFILES += \
    testgui.json

RESOURCES += \
    test.qrc
