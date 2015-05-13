#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T08:21:39
#
#-------------------------------------------------

QT       += core network

QT       -= gui

QT_INCLUDE_PATH = /home/savinov/Qt/5.4/gcc_64/include/
LUA_INCLUDE_PATH = /usr/include/lua-5.2.3
LUA_LIB_PATH = /usr/lib
SOURCE_PATH=./src
INCLUDE_PATH=./include
LUA_STATE_PATH=$$INCLUDE_PATH/selene

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
                ../Database/HashTable/

SOURCES += main.cpp \
    $$SOURCE_PATH/host.cpp \
    $$SOURCE_PATH/Server.cpp \
    $$SOURCE_PATH/Task.cpp \
    ../Database/HashTable/dbclient.cpp \
    ../Database/HashTable/Data.cpp

HEADERS += \
    $$INCLUDE_PATH/Content.h \
    $$INCLUDE_PATH/DataBase.h \
    $$INCLUDE_PATH/host.h \
    $$INCLUDE_PATH/hostsender.h \
    $$INCLUDE_PATH/Logger.h \
    $$INCLUDE_PATH/LuaExecutor.h \
    $$INCLUDE_PATH/Queue.h \
    $$INCLUDE_PATH/RbTree.h \
    $$INCLUDE_PATH/Server.h \
    $$INCLUDE_PATH/Task.h \
    $$INCLUDE_PATH/TaskScheduler.h \
    $$INCLUDE_PATH/ThreadPool.h \
    $$INCLUDE_PATH/wrapper.h \
    $$INCLUDE_PATH/util.h \
    $$INCLUDE_PATH/OrGraph.h \
    $$INCLUDE_PATH/Node.h \
    $$INCLUDE_PATH/JsonParser.h \
    $$INCLUDE_PATH/Iterator.h \
    $$INCLUDE_PATH/errors.h \
    ../Database/HashTable/dbclient.h \
    ../Database/HashTable/Data.h \
    ../Database/HashTable/Clonable.h \
    ../Database/HashTable/Serializible.h
