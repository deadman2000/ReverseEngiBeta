QT += quick
QT -= gui
CONFIG += c++11

INCLUDEPATH += $$PWD/../3rdparty/msgpack/

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
    block.h \
    convert.h \
    filechunk.h \
    filedata.h \
    filedocument.h \
    filemodel.h \
    idatasource.h \
    sector.h \
    iengine.h \
    engine.h \
    struct_reader.h

SOURCES += \
    addressrange.cpp \
    areastyle.cpp \
    base_interpreters.cpp \
    basedatainterpreter.cpp \
    block.cpp \
    filechunk.cpp \
    filedata.cpp \
    filedocument.cpp \
    filemodel.cpp \
    idatasource.cpp \
    sector.cpp \
    iengine.cpp \
    engine.cpp \
    struct_reader.cpp
