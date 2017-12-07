QT += quick
QT -= gui
CONFIG += c++11

TEMPLATE = lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

PRECOMPILED_HEADER += stdafx.h

SUBDIRS += \
    structure.pro

HEADERS += \
    addressrange.h \
    areastyle.h \
    base_interpreters.h \
    basedatainterpreter.h \
    blob.h \
    block.h \
    convert.h \
    filechunk.h \
    filedata.h \
    filedocument.h \
    filemodel.h \
    idatasource.h \
    number.h \
    sector.h \
    structure.h \
    structuremodel.h \
    text.h \
    iengine.h \
    engine.h

SOURCES += \
    addressrange.cpp \
    areastyle.cpp \
    base_interpreters.cpp \
    basedatainterpreter.cpp \
    blob.cpp \
    block.cpp \
    filechunk.cpp \
    filedata.cpp \
    filedocument.cpp \
    filemodel.cpp \
    idatasource.cpp \
    number.cpp \
    sector.cpp \
    structuremodel.cpp \
    text.cpp \
    iengine.cpp \
    engine.cpp
