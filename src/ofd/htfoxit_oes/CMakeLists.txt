cmake_minimum_required(VERSION 3.6)

PROJECT(oes C CXX)

set(c_files_oes
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/asn_codecs_prim.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/asn_SEQUENCE_OF.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/asn_SET_OF.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/ber_decoder.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/ber_tlv_length.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/ber_tlv_tag.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/BIT_STRING.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/BOOLEAN.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/constr_SEQUENCE.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/constr_SEQUENCE_OF.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/constr_SET.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/constr_SET_OF.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/constr_TYPE.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/constraints.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/der_encoder.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/ExtData.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/ExtensionDatas.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/GeneralizedTime.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/IA5String.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/INTEGER.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/NativeEnumerated.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/NativeInteger.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/OBJECT_IDENTIFIER.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/OCTET_STRING.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/per_decoder.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/per_encoder.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/per_opentype.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/per_support.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SES_ESPictureInfo.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SES_ESPropertyInfo.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SES_Header.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SES_SealInfo.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SES_Signature.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SES_SignInfo.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/SESeal.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/TBS_Sign.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/UTCTime.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/UTF8String.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/xer_decoder.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/xer_encoder.c
        ${PROJECT_SOURCE_DIR}/External_ASN1/src/xer_support.c
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/oes_base_lib.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_basehandler.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_DigitalSignHandler.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_DigitalVerifyHandler.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_Internalhandler.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_signEx.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_signhandler.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_utils.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/ofd_verifyhandler.cpp
        ${PROJECT_SOURCE_DIR}/OES_Base_Lib/src/openssl_compat_layer.cpp
        ${PROJECT_SOURCE_DIR}/OES_DLL/src/oes_error.cpp
        ${PROJECT_SOURCE_DIR}/OES_DLL/src/OES_LibraryManage.cpp
        ${PROJECT_SOURCE_DIR}/OES_DLL/src/oes.cpp)

INCLUDE_DIRECTORIES("./")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/External_ASN1/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../../fxlib")
#add_definitions(-D EMIT_ASN_DEBUG=1)
#add_definitions(-D _REENTRANT)
set(FXLIB_PATH "${PROJECT_SOURCE_DIR}/../../../fxlib")

set(APP_ABI_TYPE "armeabi-v7a")
set(OFD_APP_ABI_TYPE "arm_32")
set(OFD_LIB_SUFFIX "rel_ios_arm64_clang")
set(LIB_SUFFIX "rel_arm_android_gcc46")
set(LIB_PATH_FXCORE "${FXLIB_PATH}/fxcore/lib/mini_rel")
set(LIB_PATH_FGAS "${FXLIB_PATH}/fgas/lib/rel")
#set(LIB_PATH_GSDK "${FXLIB_PATH}/fgsdk/lib/${APP_ABI_TYPE}")
set(LIB_PATH_DSCRIPT "${FXLIB_PATH}/dscript/lib/rel")
#set(LIB_PATH_OPENSSL "${FXLIB_PATH}/openssl/lib/${APP_ABI_TYPE}")
set(LIB_PATH_OFD "${FXLIB_PATH}/ofd/lib")
LINK_DIRECTORIES("${LIB_PATH_OFD}")
LINK_DIRECTORIES("${LIB_PATH_FXCORE}")
LINK_DIRECTORIES("${LIB_PATH_FGAS}")
#LINK_DIRECTORIES("${LIB_PATH_GSDK}")
LINK_DIRECTORIES("${LIB_PATH_DSCRIPT}")
LINK_DIRECTORIES("${LIB_PATH_OPENSSL}")
LINK_DIRECTORIES("/usr/lib")

add_library(fxoes STATIC ${c_files_oes} )



set_target_properties(fxoes PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(fxoes
        /usr/lib/libssl.dylib
        /usr/lib/libcrypto.dylib
        "fgas"
        "ofd[${OFD_LIB_SUFFIX}]"
        "ofdpdf[${OFD_LIB_SUFFIX}]"
        "pdfofd[${OFD_LIB_SUFFIX}]"
        "fxutil[${OFD_LIB_SUFFIX}]"
        "fxconv[${OFD_LIB_SUFFIX}]"
        "fxrip[${OFD_LIB_SUFFIX}]"
        "fxcert[${OFD_LIB_SUFFIX}]"
        "fxcrypto[${OFD_LIB_SUFFIX}]"
        "fxfontmt[${OFD_LIB_SUFFIX}]"
        "fxdriver[${OFD_LIB_SUFFIX}]"
        "fpdftext"
        "fpdfapi"
        "reflow"
        "fxge"
        "fpdfdoc"
        "fxmath"
        "fdrm"
        "fxcodec"
        "fxcrt"
        "fxpsi"
        "fxfontmgr"
        "fsignature"
        "dscript_rel"
        z)
