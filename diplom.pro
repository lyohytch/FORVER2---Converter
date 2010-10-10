# -------------------------------------------------
# Project created by QtCreator 2010-04-12T22:15:53
# -------------------------------------------------
TARGET = Demo.Converter
TEMPLATE = app
QT += gui sql
SOURCES += main.cpp \
    converter.cpp \
    qmodeldescribing.cpp \
    mssqlquery.cpp \
    querymodel.cpp \
    correlationmodel.cpp \
    treeviewmodel.cpp
HEADERS += converter.h \
    qmodeldescribing.h \
    mssqlquery.h \
    querymodel.h \
    constants.h \
    correlationmodel.h \
    treeviewmodel.h
FORMS += converter.ui
#CONFIG settings
CONFIG(debug):DEFINES += DEBUGING
DEFINES+=PROS
#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT
#CONFIG(release):DEFINES += QT_NO_DEBUG

