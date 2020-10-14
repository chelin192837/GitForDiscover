#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "console.h"

/* ------------------------ console ------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Console)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Console)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Console)
	JS_STATIC_METHOD_ENTRY(clear, 0)
	JS_STATIC_METHOD_ENTRY(hide, 0)
	JS_STATIC_METHOD_ENTRY(println, 1)
	JS_STATIC_METHOD_ENTRY(show, 0)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Console,jconsole,console)

#define MAXCONSOLECONTENTS			10000

jconsole::jconsole(CFXJS_Object* pJSObject): CFXJS_EmbedObj(pJSObject)
{
}

jconsole::~jconsole()
{
}

FX_BOOL jconsole::clear(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	
// 	CReader_App* pApp = pContext->GetReaderApp();
// 	ASSERT(pApp != NULL);
// 
// 	CJS_Module* pModule = CJS_Module::GetModule(pApp);
// 	ASSERT(pModule != NULL);
// 
// 	pModule->ClearConsole();

	return TRUE;
}

FX_BOOL jconsole::hide(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	
// 	CReader_App* pApp = pContext->GetReaderApp();
// 	ASSERT(pApp != NULL);
// 
// 	CJS_Module* pModule = CJS_Module::GetModule(pApp);
// 	ASSERT(pModule != NULL);
// 
// 	pModule->HideConsole();

	return TRUE;
}

FX_BOOL jconsole::println(OBJ_METHOD_PARAMS)
{
	if (params.size() < 1)
	{
		return FALSE;
	}

// 	CJS_Context* pContext = (CJS_Context *)cc;
// 	ASSERT(pContext != NULL);
// 	
// 	CReader_App* pApp = pContext->GetReaderApp();
// 	ASSERT(pApp != NULL);
// 
// 	CJS_Module* pModule = CJS_Module::GetModule(pApp);
// 	ASSERT(pModule != NULL);
// 
// 	pModule->PrintLineConsole((FX_LPCWSTR)params[0]);
  
	return TRUE;
}

FX_BOOL jconsole::show(OBJ_METHOD_PARAMS)
{
// 	CJS_Context* pContext = (CJS_Context *)cc;
// 	ASSERT(pContext != NULL);
// 	
// 	CReader_App* pApp = pContext->GetReaderApp();
// 	ASSERT(pApp != NULL);
// 
// 	CJS_Module* pModule = CJS_Module::GetModule(pApp);
// 	ASSERT(pModule != NULL);
// 
// 	pModule->ShowConsole();

	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

