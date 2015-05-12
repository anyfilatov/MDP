# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = LuaTest
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets
SOURCES += main.cpp
HEADERS += /home/savinov/edu/mdp/Selene/include/selene.h /home/savinov/edu/mdp/Selene/include/selene/BaseFun.h /home/savinov/edu/mdp/Selene/include/selene/Class.h /home/savinov/edu/mdp/Selene/include/selene/ClassFun.h /home/savinov/edu/mdp/Selene/include/selene/Ctor.h /home/savinov/edu/mdp/Selene/include/selene/Dtor.h /home/savinov/edu/mdp/Selene/include/selene/Fun.h /home/savinov/edu/mdp/Selene/include/selene/LuaName.h /home/savinov/edu/mdp/Selene/include/selene/LuaRef.h /home/savinov/edu/mdp/Selene/include/selene/MetatableRegistry.h /home/savinov/edu/mdp/Selene/include/selene/Obj.h /home/savinov/edu/mdp/Selene/include/selene/ObjFun.h /home/savinov/edu/mdp/Selene/include/selene/Registry.h /home/savinov/edu/mdp/Selene/include/selene/Selector.h /home/savinov/edu/mdp/Selene/include/selene/State.h /home/savinov/edu/mdp/Selene/include/selene/Tuple.h /home/savinov/edu/mdp/Selene/include/selene/exotics.h /home/savinov/edu/mdp/Selene/include/selene/function.h /home/savinov/edu/mdp/Selene/include/selene/primitives.h /home/savinov/edu/mdp/Selene/include/selene/traits.h /home/savinov/edu/mdp/Selene/include/selene/util.h /home/savinov/edu/mdp/mdp/Connection/tests/LuaTest/tes.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += DEBUG=1 
INCLUDEPATH += ../../include /usr/include/lua-5.2.3 /home/savinov/edu/mdp/Selene/include /home/savinov/edu/mdp/trunk/include /home/savinov/edu/mdp/LuaState/include 
LIBS += -llua52 -ldl  -Wl,-rpath,/usr/lib 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
