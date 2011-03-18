AUTHOR = Alexey Kulyasov
EMAIL = alexey.kulyasov@gmail.com
LICENSE = LGPL v2.1
LANGUAGE = C++

INCLUDEPATH += . .. ../includes
TEMPLATE = lib

QT += gui core

DESTDIR = ../build/lib/$$TARGET

win32:CONFIG += qaxcontainer
win32:DLLDESTDIR = ../build/bin

MOC_DIR = ../build/mocs/$$TARGET
OBJECTS_DIR = ../build/objs/$$TARGET

OTHER_FILES += ../includes/constants.h \
               ../includes/constants_query.h \
               ../includes/constants_presenter.h \
               ../includes/constants_model.h

## CONFIG settings(debug)
#CONFIG += debug
#CONFIG -= release
#DEFINES += DEBUGING
#DEFINES += PROS

## CONFIG settings (release)
CONFIG += release
CONFIG -= debug
DEFINES += QT_NO_DEBUG QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
