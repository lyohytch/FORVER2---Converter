INCLUDEPATH += . ..
TEMPLATE = lib

QT += gui core

win32:DESTDIR = ../build/lib/$$TARGET
win32:DLLDESTDIR = ../build/bin

unix:DESTDIR = ../build/bin

MOC_DIR = ../build/mocs/$$TARGET
OBJECTS_DIR = ../build/objs/$$TARGET

# CONFIG settings(debug)
CONFIG += debug
CONFIG -= release
DEFINES += DEBUGING
DEFINES += PROS

#CONFIG settings (release)
#CONFIG += release
#CONFIG -= debug
#DEFINES += QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
