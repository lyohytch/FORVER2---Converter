## CONFIG settings(debug)
#CONFIG += debug
#CONFIG -= release
#DEFINES += DEBUGING
#DEFINES += PROS

## CONFIG settings (release)
CONFIG += release
CONFIG -= debug
DEFINES += QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

TARGET = Demo.Converter
TEMPLATE = app
DESTDIR = ./bin
MOC_DIR = mocs/$$TARGET
OBJECTS_DIR = objs/$$TARGET
QT += sql gui core
INCLUDEPATH += ../model ../presenter ../query ../ui
DEPENDPATH += ../model ../query ../presenter ../ui
LIBS += -L$$LIBPATHMODEL -L$$LIBPATHQUERY -L$$LIBPATHPRESENT -L$$LIBPATHUI -lmodel -lquery -lpresenter -lui
