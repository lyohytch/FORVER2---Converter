QT += sql
INCLUDEPATH += ../model ../presenter ../query
DEPENDPATH += ../model ../query ../presenter
LIBS += -L$$LIBPATHMODEL -L$$LIBPATHQUERY -L$$LIBPATHPRESENT -lmodel -lquery -lpresenter
