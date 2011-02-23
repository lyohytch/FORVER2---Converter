INCLUDEPATH += . ..
TEMPLATE = lib

QT += gui core

DESTDIR = ../build/lib/$$TARGET
DLLDESTDIR = ../build/bin
MOC_DIR = ../build/mocs/$$TARGET
OBJECTS_DIR = ../build/objs/$$TARGET

# CONFIG settings
DEFINES += DEBUGING
DEFINES += PROS
