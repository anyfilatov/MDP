#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T08:21:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT_INCLUDE_PATH = /usr/include/qt5/
LUA_INCLUDE_PATH = /usr/include/lua-5.2.3
LUA_LIB_PATH = /usr/lib
SOURCE_PATH=./src
INCLUDE_PATH=./include
LUA_STATE_PATH=../../Selene/include

TARGET = CloudApp
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

LIBS += -LLUA_LIB_PATH -llua52
INCLUDEPATH += $$QT_INCLUDE_PATH/QtNetwork\
               $$QT_INCLUDE_PATH/QtCore\
               $$LUA_INCLUDE_PATH\
               $$INCLUDE_PATH\
               $$LUA_STATE_PATH\

SOURCES += main.cpp \
    $$SOURCE_PATH/host.cpp \
    $$SOURCE_PATH/Script.cpp \
    $$SOURCE_PATH/Server.cpp \
    $$SOURCE_PATH/Task.cpp \
    src/LuaExecutor.cpp

HEADERS += \
    $$INCLUDE_PATH/Content.h \
    $$INCLUDE_PATH/DataBase.h \
    $$INCLUDE_PATH/host.h \
    $$INCLUDE_PATH/hostsender.h \
    $$INCLUDE_PATH/Logger.h \
    $$INCLUDE_PATH/LuaExecutor.h \
    $$INCLUDE_PATH/orgraph.h \
    $$INCLUDE_PATH/Queue.h \
    $$INCLUDE_PATH/RbTree.h \
    $$INCLUDE_PATH/Script.h \
    $$INCLUDE_PATH/Server.h \
    $$INCLUDE_PATH/Task.h \
    $$INCLUDE_PATH/TaskScheduler.h \
    $$INCLUDE_PATH/ThreadPool.h \
    $$INCLUDE_PATH/wrapper.h
