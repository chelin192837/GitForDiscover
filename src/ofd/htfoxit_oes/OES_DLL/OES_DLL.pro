#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T18:09:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = oes
TEMPLATE = lib

DEFINES += OES_DLL_LIBRARY

WORKDIR = $$PWD
EXTERNAL_ASN1 = $$WORKDIR/../External_ASN1
INTERNAL_SDK = $$WORKDIR/../../InternalSDK
FX_LIB_PATH = $$WORKDIR/../../fxlib/x86

DESTDIR = $$WORKDIR/../../bin

INCLUDEPATH += \
    $$WORKDIR/include \
    $$EXTERNAL_ASN1/include \
    $$INTERNAL_SDK \
    $$INTERNAL_SDK/htfoxitSign/include\
    $$FX_LIB_PATH/fxutil/include \
    $$FX_LIB_PATH/ofd/include \
    $$FX_LIB_PATH/fgas/include \
    $$FX_LIB_PATH/fxutil/include \
    $$FX_LIB_PATH/fxcore/include \

SOURCES += \
    src/oes.cpp \
    src/OES_LibraryManage.cpp \
    src/oes_error.cpp

HEADERS += \
    include/oes_dll.h \
    include/oes_error.h \
    include/OES_DLL_global.h \
    include/OES_LibraryManage.h \

LIBS +=\
    -L"$$WORKDIR/../../bin" \
    -L"$$FX_LIB_PATH/ofd/lib" \
    -L"$$FX_LIB_PATH/fgas/lib/rel" \
    -L"$$FX_LIB_PATH/fxcore/lib/rel" \
    -L"$$FX_LIB_PATH/fxconv/lib" \
    -L"$$FX_LIB_PATH/fxutil/lib" \

LIBS += \
    -lOES_Base \
    -lfdrm[rel_linux_gcc] \
    -lfxcrt[rel_linux_gcc] \
    -lssl

CONFIG += precompile_header

PRECOMPILED_HEADER = include/oes_dll.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

QMAKE_CXXFLAGS +=-Wno-unused-parameter
QMAKE_CXXFLAGS +=-Wno-unused-variable
