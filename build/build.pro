include( ../constants.pri )
include( ../build.pri )

HEADERS += version.h
SOURCES += main.cpp
win32:RC_FILE = ../icon.rc
OTHER_FILES =../icon.rc
