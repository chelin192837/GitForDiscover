#ifndef _FXLIB_HEADERS_
#define _FXLIB_HEADERS_
//#define _FX_OS_ == _FX_IOS_
//#include <android/log.h>
#define LOG_TAG  "C_TAG"
#define LOGD_(format, args...) (printf( ESC_START COLOR_INFO "[INFO]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))
//#define LOGD_(...)  printf(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGD(fmt,...) \
    printf("%s(%d)-<%s>: "#fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

//#define GSDK_OES
#define OFD_CONVERTOR

#include "../fxlib/fxcore/include/fxcrt/fx_ext.h"
#include "../fxlib/fxcore/include/fxcodec/fx_codec.h"
#include "../fxlib/fxcore/include/fxge/fx_ge.h"
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
#include "../fxlib/fxcore/include/fxge/fx_ge_win32.h"
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
#include "../fxlib/fxrip/include/fx_rip.h"
#include "../fxlib/fxrip/include/fx_rip_linux.h"
#include "../fxlib/fxcore/include/fxge/fx_ge_linux_gdk.h"
#elif _FX_OS_ == _FX_IOS_ || _FX_OS_ == _FX_MACOSX_
#include "../fxlib/fxcore/include/fxge/fx_ge_apple.h"
#endif
#include "../fxlib/fxcore/include/fpdfapi/fpdfapi.h"
#include "../fxlib/fxcore/include/fxmath/fxmath.h"
#include "../fxlib/fxcore/include/fdrm/fdrm.h"
#include "../fxlib/fxcore/include/fxfontmgr/fxfm_font.h"
#include "../fxlib/fxcore/include/fxcrt/fx_stream.h"

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
#include "../fxlib/fxcore/include/fxmrc/fxmrc.h"
#endif
//#include "../fxlib/fxutil/include/fx_time.h"
//#include "../fxlib/fxutil/include/fx_zip.h"
//#include "../fxlib/fxutil/include/fx_xml.h"

//#include "../fxlib/fxcert/include/fx_ssl.h"
//#include "../fxlib/fxcert/include/fx_evps.h"



//#include "../fxlib/ceb2pdf/include/ceb2pdf.h"

#include "../fxlib/fxcore/include/fpdftext/fpdf_text.h"
#include "../fxlib/ofd/include/fx_time.h"
#include "../fxlib/ofd/include/ofd.h"
#include "../fxlib/ofd/include/ofd_serial.h"
#include "../fxlib/ofd/include/ofd_crypt.h"
#include "../fxlib/ofd/include/ofd_font.h"
#include "../fxlib/ofd/include/ofd_text.h"
#include "../fxlib/ofd/include/ofd_verify.h"
#include "../fxlib/ofd/include/ofd_render.h"
#include "../fxlib/ofd/include/ofd_pdfconvert.h"
#include "../fxlib/ofd/include/ofd_topdf.h"
#include "../fxlib/ofd/include/ofd_optimizer.h"

//#include "../fxlib/sep2ofd/include/SSToOFD.h"

//#include "../fxlib/fxfontmt/include/fx_mgr.h"
//#include "../fxlib/fxfontmt/include/fxmapper.h"

//#include "../fxlib/ps2pdf/include/PS2PDF.h"

#ifdef GSDK_OES
#include "../fxlib/oes/xa/include/oes.h"
#endif

#include "include/fs_ofdutils.h"
#include "../include/ofd_base_r.h"

#include "include/fs_ofdlicensemanager.h"

#endif //_FXLIB_HEADERS_
