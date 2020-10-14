#include "../../inc/fpdf_define.h"
#if _FSDK_JS_ENGINE_ == _FSDK_JS_ENGINE_DSCRIPT_
/*#include "./dscript/jjs_object.cpp"*/
#else
#include "./v8/jfullscreen.cpp"
#endif