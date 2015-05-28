#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T19:42:49
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mdp
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += ../include

SOURCES += main.cpp\
        mainwindow.cpp \
    result.cpp \
    dialog.cpp \
    codeeditor.cpp

HEADERS  += mainwindow.h \
    result.h \
    dialog.h \
    codeeditor.h \
    requester.h

FORMS    += mainwindow.ui \
    result.ui \
    dialog.ui
