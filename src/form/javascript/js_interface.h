//#include "../inc/fpdf_define.h"

#if  _FSDK_JS_ENGINE_ != _FSDK_JS_ENGINE_DSCRIPT_
	#if defined(_WIN32) || defined(_WIN64)
	#pragma comment(lib,"ws2_32.lib")
	#pragma comment(lib, "winmm.lib")
	#endif
	#include "./v8/ijavascript.h"
#else
	#include "./dscript/js_interface.h"
#endif

