#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T17:55:14
#
#-------------------------------------------------

QT       -= core gui

TEMPLATE = lib
CONFIG += staticlib

WORKDIR = $$PWD
EXTERNAL_ASN1 = $$WORKDIR/../External_ASN1
INTERNAL_SDK = $$WORKDIR/../../InternalSDK/
FX_LIB_PATH = $$WORKDIR/../../fxlib/x86

INCLUDEPATH += \
    $$WORKDIR/include \
    $$EXTERNAL_ASN1/include \
    $$INTERNAL_SDK/htfoxitSign/include \
    $$FX_LIB_PATH/fxutil/include \
    $$FX_LIB_PATH/ofd/include \
    $$FX_LIB_PATH/fgas/include \
    $$FX_LIB_PATH/fxutil/include \
    $$FX_LIB_PATH/fxcore/include

TARGET = OES_Base

DESTDIR = $$INTERNAL_SDK/../bin

SOURCES += \
    ./src/oes_base_lib.cpp \
    ./src/stdafx.cpp \
    ./src/ofd_verifyhandler.cpp \
    ./src/ofd_utils.cpp \
    ./src/ofd_signhandler.cpp \
    ./src/ofd_signEx.cpp \
    ./src/ofd_Internalhandler.cpp \
    ./src/ofd_basehandler.cpp \
    ./src/ofd_DigitalSignHandler.cpp \
    ./src/ofd_DigitalVerifyHandler.cpp \
    ../External_ASN1/src/xer_support.c \
    ../External_ASN1/src/xer_encoder.c \
    ../External_ASN1/src/xer_decoder.c \
    ../External_ASN1/src/UTF8String.c \
    ../External_ASN1/src/UTCTime.c \
    ../External_ASN1/src/TBS_Sign.c \
    ../External_ASN1/src/SESeal.c \
    ../External_ASN1/src/SES_SignInfo.c \
    ../External_ASN1/src/SES_Signature.c \
    ../External_ASN1/src/SES_SealInfo.c \
    ../External_ASN1/src/SES_Header.c \
    ../External_ASN1/src/SES_ESPropertyInfo.c \
    ../External_ASN1/src/SES_ESPictureInfo.c \
    ../External_ASN1/src/per_support.c \
    ../External_ASN1/src/per_opentype.c \
    ../External_ASN1/src/per_encoder.c \
    ../External_ASN1/src/per_decoder.c \
    ../External_ASN1/src/OCTET_STRING.c \
    ../External_ASN1/src/OBJECT_IDENTIFIER.c \
    ../External_ASN1/src/NativeInteger.c \
    ../External_ASN1/src/NativeEnumerated.c \
    ../External_ASN1/src/INTEGER.c \
    ../External_ASN1/src/IA5String.c \
    ../External_ASN1/src/GeneralizedTime.c \
    ../External_ASN1/src/ExtensionDatas.c \
    ../External_ASN1/src/ExtData.c \
    ../External_ASN1/src/der_encoder.c \
    ../External_ASN1/src/constraints.c \
    ../External_ASN1/src/constr_TYPE.c \
    ../External_ASN1/src/constr_SET.c \
    ../External_ASN1/src/constr_SET_OF.c \
    ../External_ASN1/src/constr_SEQUENCE.c \
    ../External_ASN1/src/constr_SEQUENCE_OF.c \
    ../External_ASN1/src/BOOLEAN.c \
    ../External_ASN1/src/BIT_STRING.c \
    ../External_ASN1/src/ber_tlv_tag.c \
    ../External_ASN1/src/ber_tlv_length.c \
    ../External_ASN1/src/ber_decoder.c \
    ../External_ASN1/src/asn_SET_OF.c \
    ../External_ASN1/src/asn_SEQUENCE_OF.c \
    ../External_ASN1/src/asn_codecs_prim.c

HEADERS += \
    ./include/oes_base_lib.h \
    ./include/stdafx.h \
    ./include/ofd_verifyhandler.h \
    ./include/ofd_utils.h \
    ./include/ofd_signhandler.h \
    ./include/ofd_signEx.h \
    ./include/ofd_Internalhandler.h \
    ./include/ofd_define.h \
    ./include/ofd_basehandler.h \
    ../External_ASN1/include/xer_support.h \
    ../External_ASN1/include/xer_encoder.h \
    ../External_ASN1/include/xer_decoder.h \
    ../External_ASN1/include/UTF8String.h \
    ../External_ASN1/include/UTCTime.h \
    ../External_ASN1/include/TBS_Sign.h \
    ../External_ASN1/include/SESeal.h \
    ../External_ASN1/include/SES_SignInfo.h \
    ../External_ASN1/include/SES_Signature.h \
    ../External_ASN1/include/SES_SealInfo.h \
    ../External_ASN1/include/SES_Header.h \
    ../External_ASN1/include/SES_ESPropertyInfo.h \
    ../External_ASN1/include/SES_ESPictureInfo.h \
    ../External_ASN1/include/per_support.h \
    ../External_ASN1/include/per_opentype.h \
    ../External_ASN1/include/per_encoder.h \
    ../External_ASN1/include/per_decoder.h \
    ../External_ASN1/include/OCTET_STRING.h \
    ../External_ASN1/include/OBJECT_IDENTIFIER.h \
    ../External_ASN1/include/NativeInteger.h \
    ../External_ASN1/include/NativeEnumerated.h \
    ../External_ASN1/include/INTEGER.h \
    ../External_ASN1/include/IA5String.h \
    ../External_ASN1/include/GeneralizedTime.h \
    ../External_ASN1/include/ExtensionDatas.h \
    ../External_ASN1/include/ExtData.h \
    ../External_ASN1/include/der_encoder.h \
    ../External_ASN1/include/constraints.h \
    ../External_ASN1/include/constr_TYPE.h \
    ../External_ASN1/include/constr_SET.h \
    ../External_ASN1/include/constr_SET_OF.h \
    ../External_ASN1/include/constr_SEQUENCE.h \
    ../External_ASN1/include/constr_SEQUENCE_OF.h \
    ../External_ASN1/include/BOOLEAN.h \
    ../External_ASN1/include/BIT_STRING.h \
    ../External_ASN1/include/ber_tlv_tag.h \
    ../External_ASN1/include/ber_tlv_length.h \
    ../External_ASN1/include/ber_decoder.h \
    ../External_ASN1/include/asn_system.h \
    ../External_ASN1/include/asn_SET_OF.h \
    ../External_ASN1/include/asn_SEQUENCE_OF.h \
    ../External_ASN1/include/asn_internal.h \
    ../External_ASN1/include/asn_codecs.h \
    ../External_ASN1/include/asn_codecs_prim.h \
    ../External_ASN1/include/asn_application.h \
    include/ofd_DigitalSignHandler.h \
    include/ofd_DigitalVerifyHandler.h

PRECOMPILED_HEADER = ./include/stdafx.h

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
