INCLUDEPATH += . ..
TEMPLATE = lib

QT += gui core

win32:DESTDIR = ../build/lib/$$TARGET
win32:DLLDESTDIR = ../build/bin
unix:DESTDIR = ../build/bin
MOC_DIR = ../build/mocs/$$TARGET
OBJECTS_DIR = ../build/objs/$$TARGET

# CONFIG settings
DEFINES += DEBUGING
DEFINES += PROS
