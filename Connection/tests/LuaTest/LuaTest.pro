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

TEMPLATE = app
INCLUDE_PATH = ../../include/
SOURCE_PATH = ../../src/

HEADERS += $$INCLUDE_PATH/Script.h

SOURCES += main.cpp\
           $$SOURCE_PATH/Script.cpp
