#ifndef _JS_DEFINE_H_
#define _JS_DEFINE_H_

typedef Value			JSValue;
typedef Dobject			JSObject;
typedef DFxObj			JSFXObject;
typedef unsigned		JSBool;

struct JSConstSpec
{
	const wchar_t* pName;
	double number;
	const wchar_t* string;
	FX_BYTE t; //0:double 1:str
};

struct JSPropertySpec
{
	const wchar_t* pName;
	LP_PROP_GET pPropGet;
	LP_PROP_PUT pPropPut;
};

struct JSMethodSpec
{
	const wchar_t* pName;
	LP_METHOD_CALL pMethodCall;
	unsigned nParamNum;
};

typedef CFX_WideString	JS_ErrorString;

#define JS_TRUE			(unsigned)1
#define JS_FALSE		(unsigned)0


#define CJS_PointsArray		CFX_ArrayTemplate<float>
#define CJS_IntArray		CFX_ArrayTemplate<int>

/* ====================================== PUBLIC DEFINE SPEC ============================================== */

#define OBJ_PROP_PARAMS			IDS_Context* cc, CFXJS_PropValue& vp, JS_ErrorString& sError
#define OBJ_METHOD_PARAMS		IDS_Context* cc, const CJS_Parameters& params, CFXJS_Value& vRet, JS_ErrorString& sError
#ifndef __GNUC__
#define BEGIN_JS_STATIC_CONST(js_class_name) JSConstSpec js_class_name::JS_Class_Consts[] = {
#define JS_STATIC_CONST_ENTRY_NUMBER(const_name, pValue) {L#const_name, pValue, L"", 0},
#define JS_STATIC_CONST_ENTRY_STRING(const_name, pValue) {L#const_name, 0, L#pValue, 1},
#define END_JS_STATIC_CONST() {0, 0, 0, 0}};

#define BEGIN_JS_STATIC_PROP(js_class_name) JSPropertySpec js_class_name::JS_Class_Properties[] = {
#define JS_STATIC_PROP_ENTRY(prop_name) {L#prop_name, get_##prop_name##_static, set_##prop_name##_static},
#define END_JS_STATIC_PROP() {0, 0, 0}};

#define BEGIN_JS_STATIC_METHOD(js_class_name) JSMethodSpec js_class_name::JS_Class_Methods[] = {
#define JS_STATIC_METHOD_ENTRY(method_name, nargs) {L#method_name, method_name##_static, nargs},
#define END_JS_STATIC_METHOD() {0, 0, 0}};
#else
#define BEGIN_JS_STATIC_CONST(js_class_name) JSConstSpec js_class_name::JS_Class_Consts[] = {
#define JS_STATIC_CONST_ENTRY_NUMBER(const_name, pValue) {L""#const_name, pValue, L"", 0},
#define JS_STATIC_CONST_ENTRY_STRING(const_name, pValue) {L""#const_name, 0, L""#pValue, 1},
#define END_JS_STATIC_CONST() {0, 0, 0, 0}};

#define BEGIN_JS_STATIC_PROP(js_class_name) JSPropertySpec js_class_name::JS_Class_Properties[] = {
#define JS_STATIC_PROP_ENTRY(prop_name) {L""#prop_name, get_##prop_name##_static, set_##prop_name##_static},
#define END_JS_STATIC_PROP() {0, 0, 0}};

#define BEGIN_JS_STATIC_METHOD(js_class_name) JSMethodSpec js_class_name::JS_Class_Methods[] = {
#define JS_STATIC_METHOD_ENTRY(method_name, nargs) {L""#method_name, method_name##_static, nargs},
#define END_JS_STATIC_METHOD() {0, 0, 0}};
#endif
#define MEMLEAKCHECK_1() ((void)0)
#define MEMLEAKCHECK_2(main_name, sub_name) ((void)0)


/*
#ifdef _DEBUG
#define MEMLEAKCHECK_1() \
	_CrtMemState state1;\
	_CrtMemCheckpoint(&state1);

#define MEMLEAKCHECK_2(main_name,sub_name) \
	_CrtMemState state2;\
	_CrtMemCheckpoint(&state2);\
	_CrtMemState diff;\
	_CrtMemDifference(&diff,&state1,&state2);\
	if (diff.lSizes[_NORMAL_BLOCK] > 0)\
	{\
		TRACE("Detected normal block memory leaks in JS Module! [%s.%s]\n",#main_name,#sub_name);\
		_CrtMemDumpStatistics(&diff);\
	}
#else
	#define MEMLEAKCHECK_1() ((void)0)
	#define MEMLEAKCHECK_2(main_name,sub_name) ((void)0)
#endif
*/

/* ======================================== PROP CALLBACK ============================================ */
#ifdef _NOEXCEPTION_
#define JS_STATIC_PROP_GET(prop_name, class_name)\
	static JSBool get_##prop_name##_static(DS_PROPGET_ARGS)\
{\
	CFXJS_PropValue value(CFXJS_Value(get, VT_unknown));\
	value.StartGetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_name* pObj = (class_name*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
{\
	MEMLEAKCHECK_1();\
	bRet = pObj->prop_name(cc, value, sError);\
	MEMLEAKCHECK_2(class_name, prop_name);\
}\
	if (bRet)\
{\
	return 1;\
}\
	else\
{\
	if(error)\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, #prop_name);\
		DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
	}\
	return 0;\
	}\
}

#define JS_STATIC_PROP_SET(prop_name, class_name)\
	static JSBool set_##prop_name##_static(DS_PROPPUT_ARGS)\
{\
	CFXJS_PropValue value(PARAM_TO_JSVALUE(put));\
	value.StartSetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_name* pObj = (class_name*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
	MEMLEAKCHECK_1();\
	bRet = pObj->prop_name(cc, value, sError);\
	MEMLEAKCHECK_2(class_name, prop_name);\
	}\
	if (bRet)\
	{\
	return 1;\
	}\
	else\
	{\
	if (error)\
	{\
	CFX_ByteString cbName;\
	cbName.Format("%s.%s", #class_name, #prop_name);\
	DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
	}\
	return 0;\
	}\
}
#else
#define JS_STATIC_PROP_GET(prop_name, class_name)\
	static JSBool get_##prop_name##_static(DS_PROPGET_ARGS)\
{\
	CFXJS_PropValue value(CFXJS_Value(get, VT_unknown));\
	value.StartGetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_name* pObj = (class_name*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->prop_name(cc, value, sError);\
		MEMLEAKCHECK_2(class_name, prop_name);\
	}\
	catch (...)\
	{\
		if(error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #prop_name);\
			DS_Error(error,CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if(error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #prop_name);\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
}

#define JS_STATIC_PROP_SET(prop_name, class_name)\
	static JSBool set_##prop_name##_static(DS_PROPPUT_ARGS)\
{\
	CFXJS_PropValue value(PARAM_TO_JSVALUE(put));\
	value.StartSetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_name* pObj = (class_name*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->prop_name(cc, value, sError);\
		MEMLEAKCHECK_2(class_name, prop_name);\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #prop_name);\
			DS_Error(error,CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #prop_name);\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
}
#endif
#define JS_STATIC_PROP(prop_name, class_name)\
JS_STATIC_PROP_GET(prop_name, class_name);\
JS_STATIC_PROP_SET(prop_name, class_name)

/* ========================================= METHOD CALLBACK =========================================== */
#ifdef _NOEXCEPTION_
#define JS_STATIC_METHOD(method_name, class_name)\
	static JSBool method_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
    {\
	CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
	parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval,VT_unknown);\
	CFXJS_Object* pJSObj = (CFXJS_Object *)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_name* pObj = (class_name*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
	MEMLEAKCHECK_1();\
	bRet = pObj->method_name(cc, parameters, valueRes, sError);\
	MEMLEAKCHECK_2(class_name, method_name);\
	}\
	if (bRet)\
	{\
	return 1;\
	}\
	else\
	{\
	if(error)\
	{\
	CFX_ByteString cbName;\
	cbName.Format("%s.%s", #class_name, #method_name);\
	DS_Error(error, CFX_WideString::FromLocal(cbName), sError);\
	}\
	return 0;\
	}\
}
#else
#define JS_STATIC_METHOD(method_name, class_name)\
	static JSBool method_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
    {\
	    CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
		parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval,VT_unknown);\
	CFXJS_Object* pJSObj = (CFXJS_Object *)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_name* pObj = (class_name*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->method_name(cc, parameters, valueRes, sError);\
		MEMLEAKCHECK_2(class_name, method_name);\
	}\
	catch (...)\
	{\
		if(error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #method_name);\
			DS_Error(error, CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #method_name);\
			DS_Error(error, CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
}
#endif

/* ===================================== JS CLASS =============================================== */

#define DECLARE_JS_CLASS(js_class_name) \
	static JSBool JSConstructor(IDS_Context* cc, JSFXObject* obj,JSFXObject *global);\
	static JSBool JSDestructor(JSFXObject *obj, void* pGlobalData);\
	static int Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType);\
	static void GetConsts(JSConstSpec*& pConsts, int& nSize);\
	static void GetProperties(JSPropertySpec*& pProperties, int& nSize);\
	static void GetMethods(JSMethodSpec*& pMethods, int& nSize);\
	static JSConstSpec JS_Class_Consts[];\
	static JSPropertySpec JS_Class_Properties[];\
	static JSMethodSpec	JS_Class_Methods[];\
	static const wchar_t* m_pClassName
#ifndef __GNUC__
#define IMPLEMENT_JS_CLASS_RICH(js_class_name, class_alternate, class_name) \
const wchar_t* js_class_name::m_pClassName = L#class_name;\
JSBool js_class_name::JSConstructor(IDS_Context* cc, JSFXObject* obj, JSFXObject* global)\
{\
	CFXJS_Object* pObj = FX_NEW js_class_name(obj);\
	pObj->SetEmbedObject(FX_NEW class_alternate(pObj));\
	DS_SetPrivate(obj, (void*)pObj); \
	pObj->InitInstance(cc);\
	return JS_TRUE;\
}\
\
JSBool js_class_name::JSDestructor(JSFXObject* obj, void* pGlobalData) \
{\
	js_class_name* pObj = (js_class_name*)DS_GetPrivate(obj);\
	ASSERT(pObj != NULL);\
	pObj->ExitInstance();\
	delete pObj;\
	return JS_TRUE;\
}\
\
int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, JSConstructor, JSDestructor, 0);\
	if (nObjDefnID >= 0)\
	{\
		for (int j=0, szj=sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1; j<szj; j++)\
		{\
			if (DS_DefineObjProperty(pRuntime, nObjDefnID, JS_Class_Properties[j].pName, JS_Class_Properties[j].pPropGet, JS_Class_Properties[j].pPropPut) < 0) return -1;\
		}\
		for (int k=0, szk=sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1; k<szk; k++)\
		{\
			if (DS_DefineObjMethod(pRuntime, nObjDefnID,JS_Class_Methods[k].pName, JS_Class_Methods[k].pMethodCall, JS_Class_Methods[k].nParamNum) < 0) return -1;\
		}\
		return nObjDefnID;\
	}\
	return -1;\
}\
void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts) / sizeof(JSConstSpec) - 1;\
}\
void js_class_name::GetProperties(JSPropertySpec*& pProperties, int& nSize)\
{\
	pProperties = JS_Class_Properties;\
	nSize = sizeof(JS_Class_Properties) / sizeof(JSPropertySpec) - 1;\
}\
void js_class_name::GetMethods(JSMethodSpec*& pMethods, int& nSize)\
{\
	pMethods = JS_Class_Methods;\
	nSize = sizeof(JS_Class_Methods) / sizeof(JSMethodSpec) - 1;\
}
#else
#define IMPLEMENT_JS_CLASS_RICH(js_class_name, class_alternate, class_name) \
	const wchar_t* js_class_name::m_pClassName = L""#class_name;\
	JSBool js_class_name::JSConstructor(IDS_Context* cc, JSFXObject* obj, JSFXObject* global)\
{\
	CFXJS_Object* pObj = FX_NEW js_class_name(obj);\
	pObj->SetEmbedObject(FX_NEW class_alternate(pObj));\
	DS_SetPrivate(obj, (void*)pObj); \
	pObj->InitInstance(cc);\
	return JS_TRUE;\
}\
	\
	JSBool js_class_name::JSDestructor(JSFXObject* obj, void* pGlobalData) \
{\
	js_class_name* pObj = (js_class_name*)DS_GetPrivate(obj);\
	ASSERT(pObj != NULL);\
	pObj->ExitInstance();\
	delete pObj;\
	return JS_TRUE;\
}\
	\
	int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, JSConstructor, JSDestructor, 0);\
	if (nObjDefnID >= 0)\
	{\
	for (int j=0, szj=sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1; j<szj; j++)\
		{\
		if (DS_DefineObjProperty(pRuntime, nObjDefnID, JS_Class_Properties[j].pName, JS_Class_Properties[j].pPropGet, JS_Class_Properties[j].pPropPut) < 0) return -1;\
		}\
		for (int k=0, szk=sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1; k<szk; k++)\
		{\
		if (DS_DefineObjMethod(pRuntime, nObjDefnID,JS_Class_Methods[k].pName, JS_Class_Methods[k].pMethodCall, JS_Class_Methods[k].nParamNum) < 0) return -1;\
		}\
		return nObjDefnID;\
	}\
	return -1;\
}\
	void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts) / sizeof(JSConstSpec) - 1;\
}\
	void js_class_name::GetProperties(JSPropertySpec*& pProperties, int& nSize)\
{\
	pProperties = JS_Class_Properties;\
	nSize = sizeof(JS_Class_Properties) / sizeof(JSPropertySpec) - 1;\
}\
	void js_class_name::GetMethods(JSMethodSpec*& pMethods, int& nSize)\
{\
	pMethods = JS_Class_Methods;\
	nSize = sizeof(JS_Class_Methods) / sizeof(JSMethodSpec) - 1;\
}
#endif

#define IMPLEMENT_JS_CLASS(js_class_name, class_alternate, class_name) IMPLEMENT_JS_CLASS_RICH(js_class_name, class_alternate, class_name)

/* ======================================== CONST CLASS ============================================ */

#define DECLARE_JS_CLASS_CONST() \
	static int Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType);\
	static void GetConsts(JSConstSpec*& pConsts, int& nSize);\
	static JSConstSpec JS_Class_Consts[];\
	static const wchar_t* m_pClassName
#ifndef __GNUC__
#define IMPLEMENT_JS_CLASS_CONST(js_class_name, class_name) \
const wchar_t* js_class_name::m_pClassName = L#class_name;\
int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, NULL, NULL, 0);\
	if (nObjDefnID >=0)\
	{\
		for (int i=0, sz=sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1; i<sz; i++)\
		{\
			if (JS_Class_Consts[i].t == 0)\
			{\
				if (DS_DefineObjConst(pRuntime, nObjDefnID, JS_Class_Consts[i].pName, DS_NewNumber(JS_Class_Consts[i].number)) < 0) return -1;\
			}\
			else\
			{\
			if (DS_DefineObjConst(pRuntime, nObjDefnID, JS_Class_Consts[i].pName, DS_NewString(JS_Class_Consts[i].string)) < 0) return -1;\
			}\
		}\
		return nObjDefnID;\
	}\
	return -1;\
}\
void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1;\
}
#else
#define IMPLEMENT_JS_CLASS_CONST(js_class_name, class_name) \
	const wchar_t* js_class_name::m_pClassName = L""#class_name;\
	int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, NULL, NULL, 0);\
	if (nObjDefnID >=0)\
	{\
	for (int i=0, sz=sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1; i<sz; i++)\
		{\
		if (JS_Class_Consts[i].t == 0)\
			{\
			if (DS_DefineObjConst(pRuntime, nObjDefnID, JS_Class_Consts[i].pName, DS_NewNumber(JS_Class_Consts[i].number)) < 0) return -1;\
			}\
			else\
			{\
			if (DS_DefineObjConst(pRuntime, nObjDefnID, JS_Class_Consts[i].pName, DS_NewString(JS_Class_Consts[i].string)) < 0) return -1;\
			}\
		}\
		return nObjDefnID;\
	}\
	return -1;\
}\
	void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1;\
}
#endif

/* ===================================== SPECIAL JS CLASS =============================================== */

#define DECLARE_SPECIAL_JS_CLASS(js_class_name) \
	static JSBool JSConstructor(IDS_Context* cc, JSFXObject* obj, JSFXObject* global);\
	static JSBool JSDestructor(JSFXObject* obj, void* pGlobalData);\
	static void GetConsts(JSConstSpec*& pConsts, int& nSize);\
	static void GetProperties(JSPropertySpec*& pProperties, int& nSize);\
	static void GetMethods(JSMethodSpec*& pMethods, int& nSize);\
	static JSConstSpec JS_Class_Consts[];\
	static JSPropertySpec JS_Class_Properties[];\
	static JSMethodSpec	JS_Class_Methods[];\
	static int Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType);\
	static const wchar_t* m_pClassName;\
	static JSBool queryprop_##js_class_name##_static(DS_PROPQUERY_ARGS);\
	static JSBool getprop_##js_class_name##_static(DS_PROPGET_ARGS);\
	static JSBool putprop_##js_class_name##_static(DS_PROPPUT_ARGS);\
	static JSBool delprop_##js_class_name##_static(DS_PROPDEL_ARGS)
#ifndef __GNUC__
#ifdef _NOEXCEPTION_
#define IMPLEMENT_SPECIAL_JS_CLASS(js_class_name, class_alternate, class_name) \
const wchar_t * js_class_name::m_pClassName = L#class_name;\
	JSBool js_class_name::queryprop_##js_class_name##_static(DS_PROPQUERY_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->QueryProperty((FX_LPCWSTR)propname);\
		MEMLEAKCHECK_2(class_name, (FX_LPCWSTR)prop_name);\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		return 0;\
	}\
	return 0;\
}\
	JSBool js_class_name::getprop_##js_class_name##_static(DS_PROPGET_ARGS)\
{\
	CFXJS_PropValue value(CFXJS_Value(get, VT_unknown));\
	value.StartGetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name, L"GetProperty");\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, L"GetProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"GetProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::putprop_##js_class_name##_static(DS_PROPPUT_ARGS)\
{\
	CFXJS_PropValue value(PARAM_TO_JSVALUE(put));\
	value.StartSetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name,L"PutProperty");\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, "PutProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"PutProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::delprop_##js_class_name##_static(DS_PROPDEL_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DelProperty(cc, (FX_LPCWSTR)propname, sError);\
		MEMLEAKCHECK_2(class_name,L"DelProperty");\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, "DelProperty");\
		return 0;\
	}\
	return 0;\
}\
JSBool js_class_name::JSConstructor(IDS_Context* cc, JSFXObject * obj,JSFXObject * global)\
{\
	CFXJS_Object* pObj = FX_NEW js_class_name(obj);\
	pObj->SetEmbedObject(FX_NEW class_alternate(pObj));\
	DS_SetPrivate(obj, (void*)pObj); \
	pObj->InitInstance(cc);\
	return JS_TRUE;\
}\
\
JSBool js_class_name::JSDestructor(JSFXObject *obj, void* pGlobalData) \
{\
	js_class_name* pObj = (js_class_name*)DS_GetPrivate(obj);\
	ASSERT(pObj != NULL);\
	pObj->ExitInstance();\
	delete pObj;\
	return JS_TRUE;\
}\
\
int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, JSConstructor, JSDestructor, 0);\
\
	if (nObjDefnID >= 0)\
	{\
		for (int j=0, szj=sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1; j<szj; j++)\
		{\
			if (DS_DefineObjProperty(pRuntime, nObjDefnID, JS_Class_Properties[j].pName, JS_Class_Properties[j].pPropGet,JS_Class_Properties[j].pPropPut)<0)return -1;\
		}\
\
		for (int k=0, szk=sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1; k<szk; k++)\
		{\
			if (DS_DefineObjMethod(pRuntime, nObjDefnID,JS_Class_Methods[k].pName,JS_Class_Methods[k].pMethodCall,JS_Class_Methods[k].nParamNum)<0)return -1;\
		}\
		if (DS_DefineObjAllProperties(pRuntime, nObjDefnID, js_class_name::queryprop_##js_class_name##_static, js_class_name::getprop_##js_class_name##_static,js_class_name::putprop_##js_class_name##_static,js_class_name::delprop_##js_class_name##_static)<0) return -1;\
\
		return nObjDefnID;\
	}\
\
	return -1;\
}\
void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1;\
}\
void js_class_name::GetProperties(JSPropertySpec*& pProperties, int& nSize)\
{\
	pProperties = JS_Class_Properties;\
	nSize = sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1;\
}\
void js_class_name::GetMethods(JSMethodSpec*& pMethods, int& nSize)\
{\
	pMethods = JS_Class_Methods;\
	nSize = sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1;\
}
#else
#define IMPLEMENT_SPECIAL_JS_CLASS(js_class_name, class_alternate, class_name) \
const wchar_t * js_class_name::m_pClassName = L#class_name;\
	JSBool js_class_name::queryprop_##js_class_name##_static(DS_PROPQUERY_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->QueryProperty((FX_LPCWSTR)propname);\
		MEMLEAKCHECK_2(class_name, (FX_LPCWSTR)prop_name);\
	}\
	catch (...)\
	{\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		return 0;\
	}\
	return 0;\
}\
	JSBool js_class_name::getprop_##js_class_name##_static(DS_PROPGET_ARGS)\
{\
	CFXJS_PropValue value(CFXJS_Value(get, VT_unknown));\
	value.StartGetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name, L"GetProperty");\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, L"GetProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, L"GetProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"GetProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::putprop_##js_class_name##_static(DS_PROPPUT_ARGS)\
{\
	CFXJS_PropValue value(PARAM_TO_JSVALUE(put));\
	value.StartSetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name,L"PutProperty");\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, "PutProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, "PutProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"PutProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::delprop_##js_class_name##_static(DS_PROPDEL_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DelProperty(cc, (FX_LPCWSTR)propname, sError);\
		MEMLEAKCHECK_2(class_name,L"DelProperty");\
	}\
	catch (...)\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, "DelProperty");\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, "DelProperty");\
		return 0;\
	}\
	return 0;\
}\
JSBool js_class_name::JSConstructor(IDS_Context* cc, JSFXObject * obj,JSFXObject * global)\
{\
	CFXJS_Object* pObj = FX_NEW js_class_name(obj);\
	pObj->SetEmbedObject(FX_NEW class_alternate(pObj));\
	DS_SetPrivate(obj, (void*)pObj); \
	pObj->InitInstance(cc);\
	return JS_TRUE;\
}\
\
JSBool js_class_name::JSDestructor(JSFXObject *obj, void* pGlobalData) \
{\
	js_class_name* pObj = (js_class_name*)DS_GetPrivate(obj);\
	ASSERT(pObj != NULL);\
	pObj->ExitInstance();\
	delete pObj;\
	return JS_TRUE;\
}\
\
int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, JSConstructor, JSDestructor, 0);\
\
	if (nObjDefnID >= 0)\
	{\
		for (int j=0, szj=sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1; j<szj; j++)\
		{\
			if (DS_DefineObjProperty(pRuntime, nObjDefnID, JS_Class_Properties[j].pName, JS_Class_Properties[j].pPropGet,JS_Class_Properties[j].pPropPut)<0)return -1;\
		}\
\
		for (int k=0, szk=sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1; k<szk; k++)\
		{\
			if (DS_DefineObjMethod(pRuntime, nObjDefnID,JS_Class_Methods[k].pName,JS_Class_Methods[k].pMethodCall,JS_Class_Methods[k].nParamNum)<0)return -1;\
		}\
		if (DS_DefineObjAllProperties(pRuntime, nObjDefnID, js_class_name::queryprop_##js_class_name##_static, js_class_name::getprop_##js_class_name##_static,js_class_name::putprop_##js_class_name##_static,js_class_name::delprop_##js_class_name##_static)<0) return -1;\
\
		return nObjDefnID;\
	}\
\
	return -1;\
}\
void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1;\
}\
void js_class_name::GetProperties(JSPropertySpec*& pProperties, int& nSize)\
{\
	pProperties = JS_Class_Properties;\
	nSize = sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1;\
}\
void js_class_name::GetMethods(JSMethodSpec*& pMethods, int& nSize)\
{\
	pMethods = JS_Class_Methods;\
	nSize = sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1;\
}
#endif
#else
#ifdef _NOEXCEPTION_
#define IMPLEMENT_SPECIAL_JS_CLASS(js_class_name, class_alternate, class_name) \
const wchar_t * js_class_name::m_pClassName = L""#class_name;\
	JSBool js_class_name::queryprop_##js_class_name##_static(DS_PROPQUERY_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->QueryProperty(propname);\
		MEMLEAKCHECK_2(class_name, prop_name);\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		return 0;\
	}\
	return 0;\
}\
	JSBool js_class_name::getprop_##js_class_name##_static(DS_PROPGET_ARGS)\
{\
	CFXJS_PropValue value(CFXJS_Value(get, VT_unknown));\
	value.StartGetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name, L"GetProperty");\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, L"GetProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"GetProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::putprop_##js_class_name##_static(DS_PROPPUT_ARGS)\
{\
	CFXJS_PropValue value(PARAM_TO_JSVALUE(put));\
	value.StartSetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name,L"PutProperty");\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, "PutProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"PutProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::delprop_##js_class_name##_static(DS_PROPDEL_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DelProperty(cc, (FX_LPCWSTR)propname, sError);\
		MEMLEAKCHECK_2(class_name,L"DelProperty");\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, "DelProperty");\
		return 0;\
	}\
	return 0;\
}\
JSBool js_class_name::JSConstructor(IDS_Context* cc, JSFXObject * obj,JSFXObject * global)\
{\
	CFXJS_Object* pObj = FX_NEW js_class_name(obj);\
	pObj->SetEmbedObject(FX_NEW class_alternate(pObj));\
	DS_SetPrivate(obj, (void*)pObj); \
	pObj->InitInstance(cc);\
	return JS_TRUE;\
}\
\
JSBool js_class_name::JSDestructor(JSFXObject *obj, void* pGlobalData) \
{\
	js_class_name* pObj = (js_class_name*)DS_GetPrivate(obj);\
	ASSERT(pObj != NULL);\
	pObj->ExitInstance();\
	delete pObj;\
	return JS_TRUE;\
}\
\
int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, JSConstructor, JSDestructor, 0);\
\
	if (nObjDefnID >= 0)\
	{\
		for (int j=0, szj=sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1; j<szj; j++)\
		{\
			if (DS_DefineObjProperty(pRuntime, nObjDefnID, JS_Class_Properties[j].pName, JS_Class_Properties[j].pPropGet,JS_Class_Properties[j].pPropPut)<0)return -1;\
		}\
\
		for (int k=0, szk=sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1; k<szk; k++)\
		{\
			if (DS_DefineObjMethod(pRuntime, nObjDefnID,JS_Class_Methods[k].pName,JS_Class_Methods[k].pMethodCall,JS_Class_Methods[k].nParamNum)<0)return -1;\
		}\
		if (DS_DefineObjAllProperties(pRuntime, nObjDefnID, js_class_name::queryprop_##js_class_name##_static, js_class_name::getprop_##js_class_name##_static,js_class_name::putprop_##js_class_name##_static,js_class_name::delprop_##js_class_name##_static)<0) return -1;\
\
		return nObjDefnID;\
	}\
\
	return -1;\
}\
void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1;\
}\
void js_class_name::GetProperties(JSPropertySpec*& pProperties, int& nSize)\
{\
	pProperties = JS_Class_Properties;\
	nSize = sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1;\
}\
void js_class_name::GetMethods(JSMethodSpec*& pMethods, int& nSize)\
{\
	pMethods = JS_Class_Methods;\
	nSize = sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1;\
}
#else
#define IMPLEMENT_SPECIAL_JS_CLASS(js_class_name, class_alternate, class_name) \
const wchar_t * js_class_name::m_pClassName = L""#class_name;\
	JSBool js_class_name::queryprop_##js_class_name##_static(DS_PROPQUERY_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->QueryProperty(propname);\
		MEMLEAKCHECK_2(class_name, prop_name);\
	}\
	catch (...)\
	{\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		return 0;\
	}\
	return 0;\
}\
	JSBool js_class_name::getprop_##js_class_name##_static(DS_PROPGET_ARGS)\
{\
	CFXJS_PropValue value(CFXJS_Value(get, VT_unknown));\
	value.StartGetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name, L"GetProperty");\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, L"GetProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, L"GetProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"GetProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::putprop_##js_class_name##_static(DS_PROPPUT_ARGS)\
{\
	CFXJS_PropValue value(PARAM_TO_JSVALUE(put));\
	value.StartSetting();\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DoProperty(cc, (FX_LPCWSTR)propname, value, sError);\
		MEMLEAKCHECK_2(class_name,L"PutProperty");\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, "PutProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, "PutProperty");\
			DS_Error(error,CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error,L"PutProperty", L"Embeded object not found!");\
	}\
	return 0;\
}\
	JSBool js_class_name::delprop_##js_class_name##_static(DS_PROPDEL_ARGS)\
{\
	CFXJS_Object* pJSObj = (CFXJS_Object*)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->DelProperty(cc, (FX_LPCWSTR)propname, sError);\
		MEMLEAKCHECK_2(class_name,L"DelProperty");\
	}\
	catch (...)\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, "DelProperty");\
		return 0;\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, "DelProperty");\
		return 0;\
	}\
	return 0;\
}\
JSBool js_class_name::JSConstructor(IDS_Context* cc, JSFXObject * obj,JSFXObject * global)\
{\
	CFXJS_Object* pObj = FX_NEW js_class_name(obj);\
	pObj->SetEmbedObject(FX_NEW class_alternate(pObj));\
	DS_SetPrivate(obj, (void*)pObj); \
	pObj->InitInstance(cc);\
	return JS_TRUE;\
}\
\
JSBool js_class_name::JSDestructor(JSFXObject *obj, void* pGlobalData) \
{\
	js_class_name* pObj = (js_class_name*)DS_GetPrivate(obj);\
	ASSERT(pObj != NULL);\
	pObj->ExitInstance();\
	delete pObj;\
	return JS_TRUE;\
}\
\
int js_class_name::Init(IDS_Runtime* pRuntime, DSOBJTYPE eObjType)\
{\
\
	int nObjDefnID = DS_DefineObj(pRuntime, js_class_name::m_pClassName, eObjType, JSConstructor, JSDestructor, 0);\
\
	if (nObjDefnID >= 0)\
	{\
		for (int j=0, szj=sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1; j<szj; j++)\
		{\
			if (DS_DefineObjProperty(pRuntime, nObjDefnID, JS_Class_Properties[j].pName, JS_Class_Properties[j].pPropGet,JS_Class_Properties[j].pPropPut)<0)return -1;\
		}\
\
		for (int k=0, szk=sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1; k<szk; k++)\
		{\
			if (DS_DefineObjMethod(pRuntime, nObjDefnID,JS_Class_Methods[k].pName,JS_Class_Methods[k].pMethodCall,JS_Class_Methods[k].nParamNum)<0)return -1;\
		}\
		if (DS_DefineObjAllProperties(pRuntime, nObjDefnID, js_class_name::queryprop_##js_class_name##_static, js_class_name::getprop_##js_class_name##_static,js_class_name::putprop_##js_class_name##_static,js_class_name::delprop_##js_class_name##_static)<0) return -1;\
\
		return nObjDefnID;\
	}\
\
	return -1;\
}\
void js_class_name::GetConsts(JSConstSpec*& pConsts, int& nSize)\
{\
	pConsts = JS_Class_Consts;\
	nSize = sizeof(JS_Class_Consts)/sizeof(JSConstSpec)-1;\
}\
void js_class_name::GetProperties(JSPropertySpec*& pProperties, int& nSize)\
{\
	pProperties = JS_Class_Properties;\
	nSize = sizeof(JS_Class_Properties)/sizeof(JSPropertySpec)-1;\
}\
void js_class_name::GetMethods(JSMethodSpec*& pMethods, int& nSize)\
{\
	pMethods = JS_Class_Methods;\
	nSize = sizeof(JS_Class_Methods)/sizeof(JSMethodSpec)-1;\
}
#endif
#endif
#ifndef __GNUC__
#ifdef _NOEXCEPTION_
#define JS_SPECIAL_STATIC_METHOD(method_name, class_alternate, class_name)\
	static JSBool method_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
    {\
	CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
	parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval, VT_unknown);\
	CFXJS_Object* pJSObj = (CFXJS_Object *)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
	MEMLEAKCHECK_1();\
	bRet = pObj->method_name(cc, parameters, valueRes, sError);\
	MEMLEAKCHECK_2(class_name, method_name);\
	}\
	if (bRet)\
	{\
	return 1;\
	}\
	else\
	{\
	if (error)\
	{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, #method_name);\
		DS_Error(error, CFX_WideString::FromLocal(cbName), sError);\
	}\
	return 0;\
	}\
	if (error)\
	{\
		DS_Error(error, L#method_name, L"Embeded object not found!");\
	}\
    return 0;\
}
#else
#define JS_SPECIAL_STATIC_METHOD(method_name, class_alternate, class_name)\
	static JSBool method_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
    {\
	    CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
		parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval, VT_unknown);\
	CFXJS_Object* pJSObj = (CFXJS_Object *)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
		MEMLEAKCHECK_1();\
		bRet = pObj->method_name(cc, parameters, valueRes, sError);\
		MEMLEAKCHECK_2(class_name, method_name);\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #method_name);\
			DS_Error(error, CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
			return 0;\
		}\
	}\
	if (bRet)\
	{\
		return 1;\
	}\
	else\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #method_name);\
			DS_Error(error, CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error, L#method_name, L"Embeded object not found!");\
	}\
    return 0;\
}
#endif
#else
#ifdef _NOEXCEPTION_
#define JS_SPECIAL_STATIC_METHOD(method_name, class_alternate, class_name)\
	static JSBool method_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
    {\
	CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
	parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval, VT_unknown);\
	CFXJS_Object* pJSObj = (CFXJS_Object *)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	{\
	MEMLEAKCHECK_1();\
	bRet = pObj->method_name(cc, parameters, valueRes, sError);\
	MEMLEAKCHECK_2(class_name, method_name);\
	}\
	if (bRet)\
	{\
	return 1;\
	}\
	else\
	{\
		if(error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #method_name);\
			DS_Error(error, CFX_WideString::FromLocal(cbName), sError);\
		}\
		return 0;\
	}\
	if(error)\
	{\
		DS_Error(error, L""#method_name, L"Embeded object not found!");\
	}\
    return 0;\
}
#else
#define JS_SPECIAL_STATIC_METHOD(method_name, class_alternate, class_name)\
	static JSBool method_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
    {\
	CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
	parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval, VT_unknown);\
	CFXJS_Object* pJSObj = (CFXJS_Object *)DS_GetPrivate(obj);\
	ASSERT(pJSObj != NULL);\
	class_alternate* pObj = (class_alternate*)pJSObj->GetEmbedObject();\
	ASSERT(pObj != NULL);\
	JS_ErrorString sError;\
	FX_BOOL bRet = FALSE;\
	try\
	{\
	MEMLEAKCHECK_1();\
	bRet = pObj->method_name(cc, parameters, valueRes, sError);\
	MEMLEAKCHECK_2(class_name, method_name);\
	}\
	catch (...)\
	{\
		if (error)\
		{\
			CFX_ByteString cbName;\
			cbName.Format("%s.%s", #class_name, #method_name);\
			DS_Error(error, CFX_WideString::FromLocal(cbName), L"Unknown error is catched!");\
		}\
		return 0;\
	}\
	if (bRet)\
	{\
	return 1;\
	}\
	else\
	{\
		if (error)\
		{\
		CFX_ByteString cbName;\
		cbName.Format("%s.%s", #class_name, #method_name);\
		DS_Error(error, CFX_WideString::FromLocal(cbName), sError);\
		}\	
		return 0;\
	}\
	if (error)\
	{\
		DS_Error(error, L""#method_name, L"Embeded object not found!");\
	}\
    return 0;\
}
#endif
#endif
/* ======================================== GLOBAL METHODS ============================================ */
#ifndef __GNUC__
#define JS_STATIC_GLOBAL_FUN(fun_name) \
static JSBool fun_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
	{\
		CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
		parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval, VT_unknown);\
	JS_ErrorString sError;\
	if (!fun_name(cc, parameters, valueRes, sError))\
	{\
		if(error)\
		{\
			DS_Error(error, L#fun_name, sError);\
		}\
		return 0;\
	}\
	return 1;\
}
#else
#define JS_STATIC_GLOBAL_FUN(fun_name) \
	static JSBool fun_name##_static(DS_METHOD_ARGS)\
{\
	CJS_Parameters parameters;\
	for (unsigned int i = 0; i<argc; i++)\
	{\
	CFXJS_Value value = PARAM_TO_JSVALUE(DS_GetListValue(arglist, i));\
	parameters.push_back(value);\
	}\
	CFXJS_Value valueRes(rval, VT_unknown);\
	JS_ErrorString sError;\
	if (!fun_name(cc, parameters, valueRes, sError))\
	{\
		if(error)\
		{\
			DS_Error(error, L""#fun_name, sError);\
		}\
		return 0;\
	}\
	return 1;\
}
#endif

#define JS_STATIC_DECLARE_GLOBAL_FUN() \
static JSMethodSpec	global_methods[]; \
static int Init(IDS_Runtime* pRuntime)

#define BEGIN_JS_STATIC_GLOBAL_FUN(js_class_name) \
JSMethodSpec js_class_name::global_methods[] = {

#define JS_STATIC_GLOBAL_FUN_ENTRY(method_name,nargs) JS_STATIC_METHOD_ENTRY(method_name,nargs)

#define END_JS_STATIC_GLOBAL_FUN() END_JS_STATIC_METHOD()

#define IMPLEMENT_JS_STATIC_GLOBAL_FUN(js_class_name) \
int js_class_name::Init(IDS_Runtime* pRuntime)\
{\
	for (int i=0, sz=sizeof(js_class_name::global_methods)/sizeof(JSMethodSpec)-1; i<sz; i++)\
	{\
		if (DS_DefineGlobalMethod(pRuntime,\
				js_class_name::global_methods[i].pName,\
				js_class_name::global_methods[i].pMethodCall,\
				js_class_name::global_methods[i].nParamNum\
				) < 0\
			)return -1;\
	}\
	return 0;\
}

/* ======================================== GLOBAL CONSTS ============================================ */
#ifndef __GNUC__
#define DEFINE_GLOBAL_CONST(pRuntime, const_name , const_value)\
if (DS_DefineGlobalConst(pRuntime, L#const_name,DS_NewString(L#const_value))) return -1

/* ======================================== GLOBAL ARRAYS ============================================ */

#define DEFINE_GLOBAL_ARRAY(pRuntime)\
int size = sizeof(ArrayContent) / sizeof(FX_LPCWSTR);\
\
CFXJS_Array array;\
for (int i=0; i<size; i++) array.SetElement(i,(FX_LPCWSTR)ArrayContent[i]);\
\
CFXJS_PropValue prop;\
prop << array;\
if (DS_DefineGlobalConst(pRuntime, (const wchar_t*)ArrayName, prop.ToJSValue()) < 0)\
	return -1
#else
#define DEFINE_GLOBAL_CONST(pRuntime, const_name , const_value)\
if (DS_DefineGlobalConst(pRuntime, L""#const_name,DS_NewString(L""#const_value))) return -1

/* ======================================== GLOBAL ARRAYS ============================================ */

#define DEFINE_GLOBAL_ARRAY(pRuntime)\
	int size = sizeof(ArrayContent) / sizeof(FX_LPCWSTR);\
	\
	CFXJS_Array array;\
	for (int i=0; i<size; i++) array.SetElement(i,(FX_LPCWSTR)ArrayContent[i]);\
	\
	CFXJS_PropValue prop;\
	prop << array;\
	if (DS_DefineGlobalConst(pRuntime, ArrayName, prop.ToJSValue()) < 0)\
	return -1
#endif

/* ============================================================ */

#define VALUE_NAME_STRING		L"string"
#define VALUE_NAME_NUMBER		L"number"
#define VALUE_NAME_BOOLEAN		L"boolean"
#define VALUE_NAME_DATE			L"date"
#define VALUE_NAME_OBJECT		L"object"
#define VALUE_NAME_FXOBJ		L"fxobj"
#define VALUE_NAME_NULL			L"null"
#define VALUE_NAME_UNDEFINED	L"undefined"

#define CLASSNAME_ARRAY			L"Array"
#define CLASSNAME_DATE			L"Date"
#define CLASSNAME_STRING		L"String"

const unsigned int JSCONST_nStringHash = DS_CalcHash(VALUE_NAME_STRING,FXSYS_wcslen(VALUE_NAME_STRING));
const unsigned int JSCONST_nNumberHash = DS_CalcHash(VALUE_NAME_NUMBER,FXSYS_wcslen(VALUE_NAME_NUMBER));
const unsigned int JSCONST_nBoolHash = DS_CalcHash(VALUE_NAME_BOOLEAN,FXSYS_wcslen(VALUE_NAME_BOOLEAN));
const unsigned int JSCONST_nDateHash = DS_CalcHash(VALUE_NAME_DATE,FXSYS_wcslen(VALUE_NAME_DATE));
const unsigned int JSCONST_nObjectHash = DS_CalcHash(VALUE_NAME_OBJECT,FXSYS_wcslen(VALUE_NAME_OBJECT));
const unsigned int JSCONST_nFXobjHash = DS_CalcHash(VALUE_NAME_FXOBJ,FXSYS_wcslen(VALUE_NAME_FXOBJ));
const unsigned int JSCONST_nNullHash = DS_CalcHash(VALUE_NAME_NULL,FXSYS_wcslen(VALUE_NAME_NULL));
const unsigned int JSCONST_nUndefHash = DS_CalcHash(VALUE_NAME_UNDEFINED,FXSYS_wcslen(VALUE_NAME_UNDEFINED));

static VALUETYPE GET_VALUE_TYPE(Value * p)
{
	if (p)
	{
		const unsigned int nHash = DS_CalcHash(DS_GetTypeof(p));

		if (nHash == JSCONST_nUndefHash)
			return VT_undefined;
		else if (nHash == JSCONST_nNullHash)
			return VT_null;
		else if (nHash == JSCONST_nStringHash)
			return VT_string;
		else if (nHash == JSCONST_nNumberHash)
			return VT_number;
		else if (nHash == JSCONST_nBoolHash)
			return VT_boolean;
		else if (nHash == JSCONST_nDateHash)
			return VT_date;
		else if (nHash == JSCONST_nObjectHash)
			return VT_object;		
		else if (nHash == JSCONST_nFXobjHash)
			return VT_fxobject;

		/*
		const char * sType = p->getTypeof()->toDchars();
		if (strcmp(sType,VALUE_NAME_STRING) == 0)
			return VT_string;
		else if (strcmp(sType,VALUE_NAME_NUMBER) == 0)
			return VT_number;
		else if (strcmp(sType,VALUE_NAME_BOOLEAN) == 0)
			return VT_boolean;
		else if (strcmp(sType,VALUE_NAME_DATE) == 0)
			return VT_date;
		else if (strcmp(sType,VALUE_NAME_OBJECT) == 0)
			return VT_object;
		else if (strcmp(sType,VALUE_NAME_FXOBJ) == 0)
			return VT_object;
		else if (strcmp(sType,VALUE_NAME_NULL) == 0)
			return VT_null;
		else if (strcmp(sType,VALUE_NAME_UNDEFINED) == 0)
			return VT_undefined;
			*/
	}

	return VT_unknown;
}

#endif //_JS_DEFINE_H_
