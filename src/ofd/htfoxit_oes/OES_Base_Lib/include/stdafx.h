// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#ifndef  __OES_PREDEFINE_HEADER__
#define __OES_PREDEFINE_HEADER__

// define OPENSSL_API_COMPAT in android
#ifndef OPENSSL_API_COMPAT
//#define OPENSSL_API_COMPAT			0x10001000L
#endif

//////////////////////////////////////////////////////////////
//OpenSSL
#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/x509v3.h>
#include <openssl/asn1.h>


#if (OPENSSL_VERSION_NUMBER<0x10100000L)

#include "openssl_compat_layer.h"
#endif

//#if _FX_OS_==_FX_IOS_
#if 0

#ifdef OPENSSL_API_COMPAT
#define   PRINT_MACRO_HELPER(x)   #x
#define   PRINT_MACRO(x)   #x"=" PRINT_MACRO_HELPER(x)
#pragma message(PRINT_MACRO(OPENSSL_API_COMPAT))
#else
#error “OPENSSL_API_COMPAT NOT defined”
#endif

#endif

//////////////////////////////////////////////////////////////
//fxlib
#include "fxcore/include/fpdfapi/fpdfapi.h"
#include "fxcore/include/fxcrt/fx_string.h"
#include "fxcore/include/fxcrt/fx_system.h"
#include "fxcore/include/fdrm/fx_crypt.h"

//////////////////////////////////////////////////////////////
//Standard
#include <iostream>
#include <string>

//////////////////////////////////////////////////////////////
//ASN1
#include "SES_Signature.h"
#include "SESeal.h"
#include "SES_Header.h"

//////////////////////////////////////////////////////////////
//Internal
#include "ofd_sign.h"
#include "ofd_define.h"
#include "ofd_utils.h"
#include "ofd_basehandler.h"
#include "ofd_signhandler.h"
#include "ofd_verifyhandler.h"
#include "ofd_DigitalSignHandler.h"
#include "ofd_DigitalVerifyHandler.h"
#include "ofd_Internalhandler.h"
#include "ofd_signEx.h"


#endif
