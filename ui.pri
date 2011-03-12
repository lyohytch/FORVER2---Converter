QT += sql
RCC_DIR = ../build/res
INCLUDEPATH += ../model ../presenter ../query
DEPENDPATH += ../model ../query ../presenter
LIBS += -L$$LIBPATHMODEL -L$$LIBPATHQUERY -L$$LIBPATHPRESENT -lmodel -lquery -lpresenter
