#ifndef _FXCORE_HEADERS_
#define _FXCORE_HEADERS_

#include "../fxlib/fxcore/include/fxcrt/fx_ext.h"
#include "../fxlib/fxcore/include/fxcodec/fx_codec.h"
#include "../fxlib/fxcore/include/fxge/fx_ge565.h"
#include "../fxlib/fxcore/include/fxge/fx_ge.h"
#include "../fxlib/fxcore/include/fxge/fx_ge_win32.h"
#include "../fxlib/fxcore/include/fpdfapi/fpdfapi.h"
#include "../fxlib/fxcore/include/fpdfdoc/fpdf_doc.h"
#include "../fxlib/fxcore/include/fpdfdoc/fpdf_ap.h"
#include "../fxlib/fxcore/include/fpdfdoc/fpdf_vt.h"
#include "../fxlib/fxcore/include/fdrm/fdrm.h"
#include "../fxlib/fxcore/include/fxmath/fxmath.h"
#include "../fxlib/fxcore/include/fpdftext/fpdf_text.h"
#include "../fxlib/fxcore/include/reflow/reflowengine.h"
#include "../fxlib/fxcore/include/fxpsi/fx_paint.h"
#include "../fxlib/fxcore/include/fxfontmgr/fxfm_font.h"
#include "../fxlib/fxcore/include/fxmath/fxmath_image.h"
#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
#include "../fxlib/fxcore/include/fxfontmgr/fxfm_fontex.h"
#endif
// modify by gm 20170220
#define ENABLE_SIGNATURE
#define ENABLE_JSENGINE
#if (_FX_OS_==_FX_WIN32_DESKTOP_) || (_FX_OS_==_FX_WIN64_)
#undef ENABLE_SIGNATURE
#undef ENABLE_JSENGINE
#endif
#if defined(ENABLE_SIGNATURE)
#include "../fxlib/fxcore/include/signature/fpdf_signature.h"
#else
class CPDF_Signature;
class CPDF_SignatureEdit;
class CPDF_SignatureSign;
class CPDF_SignatureVerify;
#endif
// modify end 20170220

#include "../fxlib/fgas/include/fgas.h"
//#include "../fxlib/dscript/include/dfxapi.h"

#ifdef _FGSDK_BARCODE_SUPPORT_
#include "../fxlib/fxcore/include/fxbarcode/Thunder_MultiBarCodes.h"
#endif

#if _FX_OS_ == _FX_IOS_ 
#include <CoreGraphics/CoreGraphics.h>
#elif _FX_OS_ == _FX_MACOSX_
#include <ApplicationServices/ApplicationServices.h>
#endif

#if _FX_OS_ == _FX_IOS_ || _FX_OS_ == _FX_MACOSX_
#include "../fxlib/fxcore/include/fxge/fx_ge_apple.h"
#endif

#if _FX_OS_ == _FX_ANDROID_
#include "../fxlib/fxcore/include/fxge/fpf.h"
#endif

#endif//_FXCORE_HEADERS_
