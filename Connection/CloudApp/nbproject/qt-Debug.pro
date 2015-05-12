# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = CloudApp
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets network
SOURCES += ../main.cpp ../src/Server.cpp ../src/Task.cpp ../src/host.cpp
HEADERS += ../include/Content.h ../include/DataBase.h ../include/JsonParser.h ../include/Logger.h ../include/LuaExecutor.h ../include/Queue.h ../include/RbTree.h ../include/Server.h ../include/Task.h ../include/TaskScheduler.h ../include/ThreadPool.h ../include/errors.h ../include/host.h ../include/hostsender.h ../include/orgraph.h ../include/selene/selene.h ../include/selene/selene/BaseFun.h ../include/selene/selene/Class.h ../include/selene/selene/ClassFun.h ../include/selene/selene/Ctor.h ../include/selene/selene/Dtor.h ../include/selene/selene/Fun.h ../include/selene/selene/LuaName.h ../include/selene/selene/LuaRef.h ../include/selene/selene/MetatableRegistry.h ../include/selene/selene/Obj.h ../include/selene/selene/ObjFun.h ../include/selene/selene/Registry.h ../include/selene/selene/Selector.h ../include/selene/selene/State.h ../include/selene/selene/Tuple.h ../include/selene/selene/exotics.h ../include/selene/selene/function.h ../include/selene/selene/primitives.h ../include/selene/selene/traits.h ../include/selene/selene/util.h ../include/util.h ../include/wrapper.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += DEBUG 
INCLUDEPATH += ../include ../include/selene /usr/include/lua . . 
LIBS += -llua -ldl  
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
