TRANSLATIONS = ../Resources/Translations/converter_presenter_ru.ts

HEADERS += iview.h \
           presenter.h \
           presenterforfunctionui.h \
           presenterfornonfunctionui.h \ 
    description.h

SOURCES += presenter.cpp \
           presenterforfunctionui.cpp \
           presenterfornonfunctionui.cpp 

include( ../common.pri )
include( ../constants.pri )
include( ../presenter.pri )
