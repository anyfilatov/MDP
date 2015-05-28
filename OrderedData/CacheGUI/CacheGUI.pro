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

HEADERS  += MainWindow.h \
    CacheInfoWidget.h \
    CacheInfo.h \
    CacheInfoService.h \
    DiagramWidget.h \
    TabsWidget.h \

FORMS    += MainWindow.ui \
    CacheInfoWidget.ui

DISTFILES += \
    testgui.json

RESOURCES += \
    test.qrc
