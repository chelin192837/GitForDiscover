#include "../../inc/fpdf_define.h"
#if _FSDK_JS_ENGINE_ == _FSDK_JS_ENGINE_DSCRIPT_
#include "./dscript/jutil.cpp"
#else
#include "./v8/jutil.cpp"
#endif
