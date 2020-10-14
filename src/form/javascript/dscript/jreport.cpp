#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "report.h"

/* ---------------------- report ---------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Report)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Report)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Report) 
	JS_STATIC_METHOD_ENTRY(save, 1)
	JS_STATIC_METHOD_ENTRY(writeText,1)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Report, JReport, Report)
	
JReport::JReport(CFXJS_Object* pJSObject) : CFXJS_EmbedObj(pJSObject)
{

}

JReport::~JReport()
{
	
}

FX_BOOL JReport::writeText(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	return TRUE;
}

FX_BOOL JReport::save(OBJ_METHOD_PARAMS)
{
	if (IsSafeMode(cc)) return TRUE;
	return TRUE;	
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)