HEADERS += converter.h

SOURCES += converter.cpp

FORMS += converter.ui

TRANSLATIONS = ../Resources/Translations/converter_ui_ru.ts

include( ../common.pri )
include( ../constants.pri )
include( ../ui.pri )

RESOURCES += \
    ../resources.qrc
