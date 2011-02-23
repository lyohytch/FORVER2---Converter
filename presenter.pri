QT += sql
INCLUDEPATH += ../model ../query
DEPENDPATH += ../model ../query
LIBS += -L$$LIBPATHMODEL -L$$LIBPATHQUERY -lmodel -lquery
