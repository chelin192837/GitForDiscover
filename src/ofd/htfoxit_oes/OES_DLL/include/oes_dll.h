#ifndef OES_DLL_H
#define OES_DLL_H

#include "OES_DLL_global.h"
//////////////////////////////////////////////////////////////
//STD header files
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////
#if !defined(WIN32) && !defined(_WIN32)
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
#endif

//////////////////////////////////////////////////////////////
//fxlib
#include "../../../../../fxlib/fxcore/include/fpdfapi/fpdfapi.h"
#include "../../../../../fxlib/fxcore/include/fxcrt/fx_string.h"
#include "../../../../../fxlib/fxcore/include/fxcrt/fx_system.h"
#include "../../../../../fxlib/fxcore/include/fdrm/fx_crypt.h"

//////////////////////////////////////////////////////////////
//Internal
#include "../../OES_Base_Lib/include/ofd_sign.h"
//#include "../../../Signature/oes.h"

//////////////////////////////////////////////////////////////
//ASN1
#include "SESeal.h"
#include "SES_Signature.h"
#include "SESeal.h"
#include "SES_Header.h"

//////////////////////////////////////////////////////////////
#include "../../OES_Base_Lib/include/ofd_define.h"
#include "../../OES_Base_Lib/include/ofd_utils.h"

//////////////////////////////////////////////////////////////

#endif // OES_DLL_H
