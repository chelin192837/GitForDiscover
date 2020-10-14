#include "../../inc/fpdf_define.h"
#if _FSDK_JS_ENGINE_ == _FSDK_JS_ENGINE_DSCRIPT_
#include "./dscript/jpublicmethods.cpp"
#else
#include "./v8/jjs_root.cpp"
#endif
