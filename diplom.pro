TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += model \
           query \
           presenter \
           ui \
           build
TARGET = Demo.Converter
DESTDIR = ./build/bin

