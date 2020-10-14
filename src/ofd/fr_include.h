#ifndef _FR_INCLUDE_H_
#define _FR_INCLUDE_H_

#include "../fxcore.h"

#if !(_FX_OS_ == _FX_WIN32_DESKTOP_)
#define ZeroMemory(ptr, len)		memset(ptr, 0x0, len)
#endif
#include "../../fxlib/ofd/include/fx_time.h"
#include "../../fxlib/ofd/include/fx_xml.h"
#include "../../fxlib/ofd/include/ofd.h"
#include "../../fxlib/ofd/include/ofd_render.h"
#include "../../fxlib/ofd/include/ofd_text.h"
#include "../../fxlib/ofd/include/ofd_serial.h"
#include "../../fxlib/ofd/include/ofd_crypt.h"
#include "../../fxlib/ofd/include/ofd_pdfconvert.h"
#include "../../fxlib/ofd/include/ofd_topdf.h"

#if defined(WIN32) || defined(_WIN32)
#include "../../fxlib/htfoxitSign/include/ofd_sign.h"
#else
#include "htfoxit_oes/OES_DLL/include/oes.h"
#include "htfoxit_oes/OES_DLL/include/oes_dll.h"
#include "htfoxit_oes/OES_DLL/include/OES_DLL_global.h"
#include "htfoxit_oes/OES_DLL/include/oes_error.h"
#include "htfoxit_oes/OES_DLL/include/OES_LibraryManage.h"
#endif

void DM_LogOut(const char* format, ...);

#define DM_DEBUG

#ifdef DM_DEBUG
#define LogOut					DM_LogOut
#else
#define LogOut(...)				((void)0)
#endif

#define	MM_PER_INCH				(25.4f)
#define PT_PER_INCH				(72.0f)
#define MM_2_PT(mm)				((mm) * PT_PER_INCH / MM_PER_INCH)
#define PT_2_MM(pt)				((pt) * MM_PER_INCH / PT_PER_INCH)
using namespace std;
#include <string.h>
#include <map>
#include <iostream>
#include <time.h>
#include <stdlib.h>


#if (_FX_OS_==_FX_ANDROID_) || (_FX_OS_==_FX_IOS_)
#define szOID_OIWSEC_sha1       "1.3.14.3.2.26"
#define szOID_NIST_sha256                   "2.16.840.1.101.3.4.2.1"
#define szOID_NIST_sha384                   "2.16.840.1.101.3.4.2.2"
#define szOID_NIST_sha512                   "2.16.840.1.101.3.4.2.3"

#define szOID_RSA_MD5RSA        "1.2.840.113549.1.1.4"
#define szOID_RSA_SHA1RSA       "1.2.840.113549.1.1.5"

#define szOID_RSAES_OAEP        "1.2.840.113549.1.1.7"
#define szOID_RSA_MGF1          "1.2.840.113549.1.1.8"
#define szOID_RSA_PSPECIFIED    "1.2.840.113549.1.1.9"
#define szOID_RSA_SSA_PSS       "1.2.840.113549.1.1.10"
#define szOID_RSA_SHA256RSA     "1.2.840.113549.1.1.11"
#define szOID_RSA_SHA384RSA     "1.2.840.113549.1.1.12"
#define szOID_RSA_SHA512RSA     "1.2.840.113549.1.1.13"
#endif

//#define __T(str)				L##str
//#define _T(str)				 __T(x)
class FSPDFPage;

bool isOFDPage(FSPDFPage* page);



//#if (_FX_OS_ != _FX_WIN32_DESKTOP_)

//#endif

#endif
