#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T19:22:06
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = LuaTest
CONFIG   += console
CONFIG   -= app_bundle

QT_INCLUDE_PATH = /usr/include/qt5/
LUA_INCLUDE_PATH = /usr/include/lua-5.2.3
LUA_LIB_PATH = /usr/lib
INCLUDE_PATH = ../../include/
SOURCE_PATH = ../../src/

INCLUDEPATH += $$LUA_INCLUDE_PATH\
               $$INCLUDE_PATH\
                ../../../../Selene/include

QMAKE_CXXFLAGS += -std=c++11

LIBS += -L$$LUA_LIB_PATH -llua52 -ldl

TEMPLATE = app

HEADERS += $$INCLUDE_PATH/Script.h

SOURCES += main.cpp\
           $$SOURCE_PATH/Script.cpp

DISTFILES += \
    script.lua
