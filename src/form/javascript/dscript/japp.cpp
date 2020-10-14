
#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "app.h"
#include "js_eventhandler.h"
#include "document.h"
#include "fullscreen.h"
#include "publicmethods.h"

class FSPDF_MENULIST;
/* ---------------------------- TimerObj ---------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_TimerObj)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_TimerObj)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_TimerObj)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_TimerObj, JTimerObj,TimerObj)

JTimerObj::JTimerObj(CFXJS_Object* pJSObject)
: CFXJS_EmbedObj(pJSObject),
m_pTimer(NULL)
{
	
}

JTimerObj::~JTimerObj()
{
}

void JTimerObj::SetTimer(CFXJS_Timer* pTimer)
{
	m_pTimer = pTimer;
}

CFXJS_Timer* JTimerObj::GetTimer() const
{
	return m_pTimer;
}

#define JS_STR_VIEWERTYPE_READER		L"Reader"
#define JS_STR_VIEWERTYPE_STANDARD		L"Exchange"
#define JS_STR_VIEWERVARIATION			L"Full"

#define JS_STR_LANGUANGE_UNK			L"UNKNOWN"
#define JS_STR_LANGUANGE_CHS			L"CHS"
#define JS_STR_LANGUANGE_CHT			L"CHT"
#define JS_STR_LANGUANGE_DAN			L"DAN"
#define JS_STR_LANGUANGE_DEU			L"DEU"
#define JS_STR_LANGUANGE_ENU			L"ENU"
#define JS_STR_LANGUANGE_ESP			L"ESP"
#define JS_STR_LANGUANGE_FRA			L"FRA"
#define JS_STR_LANGUANGE_ITA			L"ITA"
#define JS_STR_LANGUANGE_KOR			L"KOR"
#define JS_STR_LANGUANGE_JPN			L"JPN"
#define JS_STR_LANGUANGE_NLD			L"NLD"
#define JS_STR_LANGUANGE_NOR			L"NOR"
#define JS_STR_LANGUANGE_PTB			L"PTB"
#define JS_STR_LANGUANGE_SUO			L"SUO"
#define JS_STR_LANGUANGE_SVE			L"SVE"

#if _FX_OS_ == _FX_WIN32_MOBILE_ || _FX_OS_ == _FX_WIN64_ || _FX_OS_ == _FX_WIN32_DESKTOP_
#define JS_STR_PLATFORM					L"WIN"
#elif _FX_OS_ == _FX_SOLARIS_ || _FX_OS_ == _FX_LINUX_EMBEDDED_ || _FX_OS_ == _FX_LINUX_DESKTOP_
#define JS_STR_PLATFORM					L"LINUX"
#elif _FX_OS_ == _FX_IOS_
#define JS_STR_PLATFORM					L"IOS"
#elif _FX_OS_ == _FX_MACOSX_
#define JS_STR_PLATFORM					L"MAC"
#elif _FX_OS_ == _FX_ANDROID_
#define JS_STR_PLATFORM					L"ANDROID"
#else
#define JS_STR_PLATFORM					L"UNKNOWN"
#endif

#define JS_STR_VIEWERVERSION			8
#define JS_NUM_FORMSVERSION				7

#define JS_FILEPATH_MAXLEN				2000

/* ---------------------------- app ---------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_App)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_App)
	JS_STATIC_PROP_ENTRY(activeDocs)
	JS_STATIC_PROP_ENTRY(calculate)
	JS_STATIC_PROP_ENTRY(formsVersion)
	JS_STATIC_PROP_ENTRY(fs)
	JS_STATIC_PROP_ENTRY(fullscreen)
	JS_STATIC_PROP_ENTRY(language)
	JS_STATIC_PROP_ENTRY(media)
	JS_STATIC_PROP_ENTRY(platform)
	JS_STATIC_PROP_ENTRY(runtimeHighlight)
	JS_STATIC_PROP_ENTRY(viewerType)
	JS_STATIC_PROP_ENTRY(viewerVariation)
	JS_STATIC_PROP_ENTRY(viewerVersion)	
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_App)
	JS_STATIC_METHOD_ENTRY(alert, 6)
	JS_STATIC_METHOD_ENTRY(beep, 1)
	JS_STATIC_METHOD_ENTRY(browseForDoc, 0)
	JS_STATIC_METHOD_ENTRY(clearInterval, 1)
	JS_STATIC_METHOD_ENTRY(clearTimeOut, 1)
	JS_STATIC_METHOD_ENTRY(execDialog, 3)
	JS_STATIC_METHOD_ENTRY(execMenuItem,  1)
	JS_STATIC_METHOD_ENTRY(findComponent, 1)
	JS_STATIC_METHOD_ENTRY(goBack, 0)
	JS_STATIC_METHOD_ENTRY(goForward, 0)
	JS_STATIC_METHOD_ENTRY(launchURL, 0)
	JS_STATIC_METHOD_ENTRY(mailMsg, 0)	
	JS_STATIC_METHOD_ENTRY(newFDF, 0)
	JS_STATIC_METHOD_ENTRY(newDoc, 0)
	JS_STATIC_METHOD_ENTRY(openDoc, 0)
	JS_STATIC_METHOD_ENTRY(openFDF, 5)
	JS_STATIC_METHOD_ENTRY(popUpMenuEx, 0)
	JS_STATIC_METHOD_ENTRY(popUpMenu, 1)
	JS_STATIC_METHOD_ENTRY(response, 0)
	JS_STATIC_METHOD_ENTRY(setInterval, 2)
	JS_STATIC_METHOD_ENTRY(setTimeOut, 2)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_App,japp,app)

japp::japp(CFXJS_Object * pJSObject) : CFXJS_EmbedObj(pJSObject) ,
	m_bCalculate(true),
	m_pRuntime(NULL),
	m_bRuntimeHighLight(false)
{
}

japp::~japp(void)
{
	for (int i=0,sz=m_aTimer.GetSize(); i<sz; i++)
		delete m_aTimer[i];

	m_aTimer.RemoveAll();
}

FX_BOOL japp::activeDocs(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		vp.SetNull();
		//CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
		//if (!pApp) return TRUE;

		if (IsSafeMode(cc)) return TRUE;
 
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return TRUE;
		FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
		FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();
// 		FSCRT_VAR params = {FSCRT_VT_LPVOID, 0, 0};
// 		FSCRT_Var_SetVoidPtrValue(params, NULL);
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0);
// 
// 		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETOPENEDDOCS, &params, 1, &retVal);
//		if (ret != FSCRT_ERRCODE_SUCCESS || retVal.var.int32Var < 1) return TRUE;
		FS_INT32 count = 0;
		pFormFiller->m_pJSActionHandle->GetOpenedDocs(NULL, &count);

		FX_LPVOID* pDocs = FX_Alloc(FX_LPVOID, count);
		if (!pDocs) return TRUE;
// 		params.var.voidPtrVar = pDocs;
// 		ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETOPENEDDOCS, &params, 1, &retVal);
		FS_RESULT ret = pFormFiller->m_pJSActionHandle->GetOpenedDocs((FSPDFDoc**)pDocs, &count);
		if (ret == FSCRT_ERRCODE_SUCCESS)
		{
			CFXJS_Context* pContext = (CFXJS_Context*)cc;
			ASSERT(pContext != NULL);

			CFXJS_Array aDocs;
			FX_INT32 iNumDocs = count;
			CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
			if(!pRuntime) return TRUE;
			for(int iIndex = 0; iIndex<iNumDocs; iIndex++)
			{
				FSPDFDoc* pDoc = (FSPDFDoc*)pDocs[iIndex];
				if (!pDoc)continue;	

				CFXJS_Document * pJSDocument = NULL;
				{
					JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Document"));
					pJSDocument = (CFXJS_Document*)DS_GetPrivate(pObj);
					JDocument* pDocument = (JDocument*)pJSDocument->GetEmbedObject();
					pDocument->AttachDoc(pDoc);
				}

				aDocs.SetElement(iIndex, CFXJS_Value(pJSDocument));
			}

			if (aDocs.GetLength() > 0)
				vp << aDocs;
			else
				vp.SetNull();
			FX_Free(pDocs);
			return TRUE;
		}
		FX_Free(pDocs);
	}
	return FALSE;
}

FX_BOOL japp::calculate(OBJ_PROP_PARAMS)
{
	if (vp.IsSetting())
	{
		bool bVP;
		vp >> bVP;
		m_bCalculate = (FX_BOOL)bVP;

// 		CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 		if (!pApp) return TRUE;
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return TRUE;
		FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
		FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();

// 		FSCRT_VAR params = {FSCRT_VT_LPVOID, 0, 0};
// 		FSCRT_Var_SetVoidPtrValue(params, NULL);
// 		FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(retVal, 0);
// 
// 		FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETOPENEDDOCS, &params, 1, &retVal);
		FS_INT32 count = 0;
//		FSCRT_StartCallBackState();
		FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
		ret = pFormFiller->m_pJSActionHandle->GetOpenedDocs(NULL, &count);
		if (ret != FSCRT_ERRCODE_SUCCESS || count < 1) return TRUE;

		FX_LPVOID* pDocs = FX_Alloc(FX_LPVOID, count);
		if (!pDocs) return TRUE;
		ret = pFormFiller->m_pJSActionHandle->GetOpenedDocs((FSPDFDoc**)pDocs, &count);
// 		params.var.voidPtrVar = pDocs;
// 		ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_GETOPENEDDOCS, &params, 1, &retVal);
		if (ret != FSCRT_ERRCODE_SUCCESS)
		{
			CFXJS_Context* pContext = (CFXJS_Context*)cc;
			ASSERT(pContext != NULL);

			CFXJS_Array aDocs;
			FX_INT32 iNumDocs = count;
			CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
			if(!pRuntime) return TRUE;
			for(int iIndex = 0; iIndex<iNumDocs; iIndex++)
			{
				FSPDFDoc* pDoc = (FSPDFDoc*)pDocs[iIndex];
				if (!pDoc)continue;	

				CFXJS_Document * pJSDocument = NULL;
				{
					JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"Document"));
					pJSDocument = (CFXJS_Document*)DS_GetPrivate(pObj);
					JDocument* pDocument = (JDocument*)pJSDocument->GetEmbedObject();
					
					FSFormImp* pForm = ((FSPDFDocImp*)pDoc)->GetForm();
					ASSERT(pForm != NULL);
					pForm->EnableCalculate((FX_BOOL)m_bCalculate);
				}
			}
		}
		FX_Free(pDocs);
	}
	else
	{
		vp << (bool)m_bCalculate;
	}
	
	return TRUE;
}

void japp::GetAppInfo(FX_INT32 type, CFX_WideString& info, CFXJS_Context* cc)
{
#if 0
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return;
	FSPDFDoc* pDocument = pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();


	FSCRT_BSTR bstrInfo;
	FSCRT_BStr_Init(&bstrInfo);
	pFormFiller->m_pJSActionHandle->GetAppInfo(type, &bstrInfo);
	if (bstrInfo.len > 0 && bstrInfo.str != NULL)
	{
		FSCRT_ST_FSUTF8ToFXWStr(&bstrInfo, info);
		FSCRT_BStr_Clear(&bstrInfo);
	}
#endif
}

FX_BOOL japp::formsVersion(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFX_WideString info;
		GetAppInfo(FSPDF_APPINFOTYPE_FORMSVERSION, info,(CFXJS_Context*)cc);
		FX_DOUBLE version = (FX_DOUBLE)JS_NUM_FORMSVERSION;
		if (!info.IsEmpty())
			version = CFXJS_PublicMethods::ParseStringToNumber(info);
		vp << version;
		return TRUE;
	}
	
	return FALSE;
}

FX_BOOL japp::viewerType(OBJ_PROP_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

	if (vp.IsGetting())
	{
		CFX_WideString info = (FX_LPCWSTR)L"FX SDK";
		GetAppInfo(FSPDF_APPINFOTYPE_VIEWERTYPE, info, pContext);
		vp << info;
		return TRUE;
	}
	
	return FALSE;
}

FX_BOOL japp::viewerVariation(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFX_WideString info = (FX_LPCWSTR)JS_STR_VIEWERVARIATION;
		GetAppInfo(FSPDF_APPINFOTYPE_VIEWERVARIATION, info, (CFXJS_Context*)cc);
		vp << info;
		return TRUE;
	}
	
	return FALSE;
}

FX_BOOL japp::viewerVersion(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFX_WideString info;
		GetAppInfo(FSPDF_APPINFOTYPE_VIEWERVERSION, info,(CFXJS_Context*)cc);
		FX_DOUBLE version = JS_STR_VIEWERVERSION;
		FS_INT32 skip = 0;
		if (!info.IsEmpty())
			version = CFXJS_PublicMethods::ParseStringToNumber(info);
		vp << version;
		return TRUE;
	}
	
	return FALSE;
}

FX_BOOL japp::platform(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		vp << (FX_LPCWSTR)JS_STR_PLATFORM;
		return TRUE;
	}
	
	return FALSE;
}

FX_BOOL japp::language(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return TRUE;
		FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
		FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();

		FS_INT32 language = 0;
		pFormFiller->m_pJSActionHandle->GetLanguage(&language);

		switch(language) {
			case FSCRT_LANGUAGE_CHS:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_CHS;
				break;
			case FSCRT_LANGUAGE_CHT:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_CHT;
				break;
			case FSCRT_LANGUAGE_DAN:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_DAN;
				break;
			case FSCRT_LANGUAGE_DEU:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_DEU;
				break;
			case FSCRT_LANGUAGE_ENU:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_ENU;
				break;
			case FSCRT_LANGUAGE_ESP:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_ESP;
				break;
			case FSCRT_LANGUAGE_FRA:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_FRA;
				break;
			case FSCRT_LANGUAGE_ITA:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_ITA;
				break;
			case FSCRT_LANGUAGE_KOR:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_KOR;
				break;
			case FSCRT_LANGUAGE_JPN:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_JPN;
				break;
			case FSCRT_LANGUAGE_NLD:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_NLD;
				break;
			case FSCRT_LANGUAGE_NOR:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_NOR;
				break;
			case FSCRT_LANGUAGE_PTB:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_PTB;
				break;
			case FSCRT_LANGUAGE_SUO:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_SUO;
				break;
			case FSCRT_LANGUAGE_SVE:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_SVE;
				break;
			default:
				vp << (FX_LPCWSTR)JS_STR_LANGUANGE_UNK;
				break;
		}
		return TRUE;
	}
	
	return FALSE;
}

//creates a new fdf object that contains no data
//comment: need reader support
//note:
//CFDF_Document * CPDFDoc_Environment::NewFDF();
FX_BOOL japp::newFDF(OBJ_METHOD_PARAMS)
{
	return TRUE;
}
//opens a specified pdf document and returns its document object
//comment:need reader support
//note: as defined in js reference, the proto of this function's fourth parmeters, how old an fdf document while do not show it.
//CFDF_Document * CPDFDoc_Environment::OpenFDF(string strPath,bool bUserConv);
FX_BOOL japp::openFDF(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

FX_BOOL japp::alert(OBJ_METHOD_PARAMS)
{
	int iSize = params.size();
	if (iSize < 1) return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;

	CFX_WideString swMsg = L"";
	CFX_WideString swTitle = L"";
	FX_INT32 iIcon = 0;
	FX_INT32 iType = 0;

	if (iSize == 1)
	{
		if (params[0].GetType() == VT_object)
		{
			if (JSObject* pObj = (JSObject *)params[0])
			{
				if (Value* pValue = DS_GetObjectElement(pObj, L"cMsg"))
					swMsg = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

				if (Value* pValue = DS_GetObjectElement(pObj,L"cTitle"))
					swTitle = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

				if (Value* pValue = DS_GetObjectElement(pObj,L"nIcon"))
					iIcon = (int)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

				if (Value* pValue = DS_GetObjectElement(pObj,L"nType"))
					iType = (int)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
			}

			if (swMsg == L"")
			{
				CFXJS_Array carray;
				if (params[0].ConvertToArray(carray))
				{
					int iLenth = carray.GetLength();
					CFXJS_Value* pValue = NULL;
					if (iLenth == 1)
						pValue = new CFXJS_Value;
					else if (iLenth > 1)
						pValue = new CFXJS_Value[iLenth];

					for(int i = 0; i < iLenth; i++)
					{
						carray.GetElement(i, pValue[i]);
						swMsg += (FX_LPWSTR)pValue[i];
						if (i < iLenth - 1)
							swMsg += L",  ";
					}

					if ((iLenth > 1) && pValue)
					{
						delete[]pValue;
						pValue = NULL;
					}
					else if ((iLenth == 1) && pValue)
					{
						delete pValue;
						pValue = NULL;
					}
				}
			}

			if (swTitle == L"")
				swTitle = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSALERT);
		}
		else if (params[0].GetType() == VT_boolean)
		{
			FX_BOOL bGet = (FX_BOOL)params[0];
			if (bGet)
				swMsg = L"true";
			else
				swMsg = L"false";

			swTitle = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSALERT);
		}
		else
		{
			swMsg = params[0];
			swTitle = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSALERT);
		}
	}
	else
	{
		if (params[0].GetType() == VT_boolean)
		{
			FX_BOOL bGet = (FX_BOOL)params[0];
			if (bGet)
				swMsg = L"true";
			else
				swMsg = L"false";
		}
		else
		{
			swMsg = params[0];
		}
		swTitle = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSALERT);
		
		for(int i = 1;i<iSize;i++)
		{
			if (i == 1)
				iIcon = int(params[i]);
			if (i == 2)
				iType = int(params[i]);
			if (i == 3)
				swTitle = params[i];			
		}
	}

	pRuntime->BeginBlock();
	CFX_ByteString utf8Msg = swMsg.UTF8Encode();
	CFX_ByteString utf8Title = swTitle.UTF8Encode();
	CFS_Library* pLib = CFS_Library::get();
	if (!pLib) return FALSE;
	FSActionHandler* pActionHandler = pLib->getActionHandler();
	if (!pActionHandler) return FALSE;
	vRet = pActionHandler->alert((FX_LPSTR)(FX_LPCSTR)utf8Msg, (FX_LPSTR)(FX_LPCSTR)utf8Title, iType, iIcon);
	pRuntime->EndBlock();

	return TRUE;
}


FX_BOOL japp::beep(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();

	if (params.size() == 1)
	{
		//CJS_Context* pContext = (CJS_Context*)cc;
		//CJS_Runtime* pRuntime = pContext->GetJSRuntime();
		//pEnv->JS_appBeep((int)params[0]);
		
		pFormFiller->m_pJSActionHandle->Beep(0);
// 		FSCRT_VAR inParams = {FSCRT_VT_INT32, 0, 0};
// 		FSCRT_Var_SetInt32Value(inParams, params[0]);
// 		FSCRT_VAR retVal = {FSCRT_VT_EMPTY, 0, 0};
// 		vRet = pApp->CallFunction(NULL, FSCRT_APPFUNC_BEEP, &inParams, 1, &retVal);	
// 		switch((int)params[0])
// 		{
// 		case 0:
// 			
// 			MessageBeep(MB_ICONEXCLAMATION);
// 			break;
// 		case 1:
// 			MessageBeep(MB_ICONHAND);
// 			break;
// 		case 2:
// 			MessageBeep(MB_ICONQUESTION);
// 			break;
// 		case 3:
// 			MessageBeep(MB_ICONASTERISK);
// 			break;
// 		case 4:
// 			MessageBeep(MB_OK);
// 			break;
// 		default:
// 			MessageBeep(-1);
// 			break;
// 		}

		return TRUE;
	}
	else
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}
}

FX_BOOL japp::findComponent(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

FX_BOOL japp::popUpMenuEx(OBJ_METHOD_PARAMS)
{	
	return FALSE;
}

FX_BOOL japp::fs(OBJ_PROP_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	if (vp.IsGetting())
	{
#if 0
		//Do not support  "FullScreen" Currently add by zhh 2014/7/30
		JSFXObject* pObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"FullScreen"));
		ASSERT(pObj != NULL);
		
		CJS_FullScreen* pJS_FS = (CJS_FullScreen*)DS_GetPrivate(pObj);
		ASSERT(pJS_FS != NULL);

		FullScreen* pFS = (FullScreen*)pJS_FS->GetEmbedObject();
		ASSERT(pFS != NULL);

	//	pFS->SetApp(pContext->GetReaderApp());
		
		vp << pJS_FS;
#endif	
		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

FX_BOOL japp::setInterval(OBJ_METHOD_PARAMS)
{
	if (params.size() > 2 || params.size() == 0) 
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPARAMERROR);	
		return FALSE;
	}
	
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	CFX_WideString script = params.size() > 0 ?  ((FX_WCHAR *)params[0]) : (FX_LPCWSTR)L"";
	if (script.IsEmpty()) 
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSAFNUMBER_KEYSTROKE);
		return TRUE;
	}

	FX_DWORD dwInterval = params.size() > 1 ? (int)params[1] : 1000;
	
	CFSCRT_LTAppHandler* pApp = pRuntime->GetReaderApp();
	//ASSERT(pApp);
	CFXJS_Timer* pTimer = new CFXJS_Timer(this, pApp);
	m_aTimer.Add(pTimer);

	pTimer->SetType(0);
	pTimer->SetRuntime(pRuntime);
	pTimer->SetJScript(script);
	pTimer->SetTimeOut(0);
//	pTimer->SetStartTime(GetTickCount());
	pTimer->SetJSTimer(dwInterval);
	
	JSFXObject* pRetObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"TimerObj"));
	ASSERT(pRetObj != NULL);
	
	CFXJS_TimerObj* pJS_TimerObj = (CFXJS_TimerObj*)DS_GetPrivate(pRetObj);
	ASSERT(pJS_TimerObj != NULL);
	
	JTimerObj* pTimerObj = (JTimerObj*)pJS_TimerObj->GetEmbedObject();
	ASSERT(pTimerObj != NULL);
	
	pTimerObj->SetTimer(pTimer); 
	
	vRet = pRetObj;
	
	return TRUE;
}

FX_BOOL japp::setTimeOut(OBJ_METHOD_PARAMS)
{
	if (params.size() > 2 || params.size() == 0)
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPARAMERROR);	
		return FALSE;
	}
	
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	
	CFX_WideString script = params.size() > 0 ?  ((FX_WCHAR *)params[0]) : (FX_LPCWSTR)L"";
	if (script.IsEmpty()) 
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSAFNUMBER_KEYSTROKE);
		return TRUE;
	}
	
	FX_DWORD dwTimeOut = params.size() > 1 ? (int)params[1] : 1000;
	
	CFSCRT_LTAppHandler* pApp = pRuntime->GetReaderApp();
	//ASSERT(pApp);
	CFXJS_Timer* pTimer = new CFXJS_Timer(this, pApp);
	m_aTimer.Add(pTimer);
	
	pTimer->SetType(1);
	pTimer->SetRuntime(pRuntime);
	pTimer->SetJScript(script);
	pTimer->SetTimeOut(dwTimeOut);
//	pTimer->SetStartTime(GetTickCount());
//	pTimer->SetJSTimer(1000);
	pTimer->SetJSTimer(dwTimeOut);
	
	JSFXObject* pRetObj = DS_NewFxDynamicObj(pRuntime, pContext, DS_GetObjDefnID(pRuntime, L"TimerObj"));
	ASSERT(pRetObj != NULL);
	
	CFXJS_TimerObj* pJS_TimerObj = (CFXJS_TimerObj*)DS_GetPrivate(pRetObj);
	ASSERT(pJS_TimerObj != NULL);
	
	JTimerObj* pTimerObj = (JTimerObj*)pJS_TimerObj->GetEmbedObject();
	ASSERT(pTimerObj != NULL);
	
	pTimerObj->SetTimer(pTimer); 
	
	vRet = pRetObj;
	
	return TRUE;
}

FX_BOOL japp::clearTimeOut(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	
	if (params.size() != 1)
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPARAMERROR);	
		return FALSE;
	}
	
	if (params[0].GetType() == VT_fxobject)
	{
		if (JSFXObject* pObj = (JSFXObject*)params[0])
		{
			if (DS_GetObjDefnID(pObj) == DS_GetObjDefnID(pRuntime, L"TimerObj"))
			{
				if (CFXJS_Object* pJSObj = (CFXJS_Object*)params[0])
				{
					if (JTimerObj* pTimerObj = (JTimerObj*)pJSObj->GetEmbedObject())
					{
						if (CFXJS_Timer* pTimer = pTimerObj->GetTimer())
						{
							pTimer->KillJSTimer();
							
							for (int i=0,sz=m_aTimer.GetSize(); i<sz; i++)
							{
								if (m_aTimer[i] == pTimer)
								{
									m_aTimer.RemoveAt(i);
									break;
								}
							}
							
							delete pTimer;
							pTimerObj->SetTimer(NULL);
						}
					}
				}
			}
		}
	}
	
	return TRUE;
}

FX_BOOL japp::clearInterval(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	if (params.size() != 1)
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPARAMERROR);	
		return FALSE;
	}
	
	if (params[0].GetType() == VT_fxobject)
	{
		if (JSFXObject* pObj = (JSFXObject*)params[0])
		{
			if (DS_GetObjDefnID(pObj) == DS_GetObjDefnID(pRuntime, L"TimerObj"))
			{
				if (CFXJS_Object* pJSObj = (CFXJS_Object*)params[0])
				{
					if (JTimerObj* pTimerObj = (JTimerObj*)pJSObj->GetEmbedObject())
					{
						if (CFXJS_Timer* pTimer = pTimerObj->GetTimer())
						{
							pTimer->KillJSTimer();
							
							for (int i=0,sz=m_aTimer.GetSize(); i<sz; i++)
							{
								if (m_aTimer[i] == pTimer)
								{
									m_aTimer.RemoveAt(i);
									break;
								}
							}
							
							delete pTimer;
							pTimerObj->SetTimer(NULL);
						}
					}
				}
			}
		}
	}
	
	return TRUE;
}

FX_BOOL japp::execMenuItem(OBJ_METHOD_PARAMS)
{	
	return FALSE;
}

void japp::TimerProc(CFXJS_Timer* pTimer)
{
	ASSERT(pTimer != NULL);

	switch (pTimer->GetType())
	{
	case 0: //interval
		RunJsScript(pTimer->GetRuntime(), pTimer->GetJScript());
		break;
	case 1:
		if (pTimer->GetTimeOut() > 0)
		{
			RunJsScript(pTimer->GetRuntime(), pTimer->GetJScript());
			pTimer->KillJSTimer();
		}
		break;
	}
	
}

void japp::RunJsScript(CFXJS_Runtime* pRuntime,const CFX_WideString& wsScript)
{
	ASSERT(pRuntime != NULL);

	if (!pRuntime->IsBlocking())
	{
		IJS_Context* pContext = pRuntime->NewContext();
		ASSERT(pContext != NULL);
		pContext->OnExternal_Exec();
		CFX_WideString wtInfo;
		pContext->RunScript(wsScript,wtInfo);
		pRuntime->ReleaseContext(pContext);
	}
}

FX_BOOL japp::goBack(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	
/*	int iCurrentPage = pDocument->GetCurrentPageView()->GetPageIndex();
	if (iCurrentPage >= 1)
		pDocument->GotoPageView()
*/
	return TRUE;
}

FX_BOOL japp::goForward(OBJ_METHOD_PARAMS)
{	
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

/*	int iCurrentPage = pDocument->GetCurrentPageView()->GetPageIndex();
	if (iCurrentPage < pDocument->GetPageCount())
		pDocument->GotoPageView(iCurrentPage + 1);
*/
	return TRUE;
}

FX_BOOL japp::mailMsg(OBJ_METHOD_PARAMS)
{
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return FALSE;
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();

	FX_BOOL bUI = TRUE;
	CFX_WideString cTo = L"";
	CFX_WideString cCc = L"";
	CFX_WideString cBcc = L"";
	CFX_WideString cSubject = L"";
	CFX_WideString cMsg = L"";

	if(params.size() < 1)
		return FALSE;

	bUI = params.size()>=1?(int)params[0]:TRUE;
	cTo = params.size()>=2?(const wchar_t*)(FX_LPCWSTR)params[1]:L"";
	cCc = params.size()>=3?(const wchar_t*)(FX_LPCWSTR)params[2]:L"";
	cBcc = params.size()>=4?(const wchar_t*)(FX_LPCWSTR)params[3]:L"";
	cSubject = params.size()>=5?(const wchar_t*)(FX_LPCWSTR)params[4]:L"";
	cMsg = params.size()>=6?(const wchar_t*)(FX_LPCWSTR)params[5]:L"";		


	if (params[0].GetType() == VT_object)
	{
		JSObject* pObj = (JSObject*)params[0];

		if (Value* pValue = DS_GetObjectElement(pObj, L"bUI"))
			bUI = (int)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj, L"cTo"))
			cTo = (FX_LPWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj, L"cCc"))
			cCc = (FX_LPWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj, L"cBcc"))
			cBcc = (FX_LPWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj, L"cSubject"))
			cSubject = (FX_LPWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj, L"cMsg"))
			cMsg = (FX_LPWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
	}
#if 0
	pRuntime->BeginBlock();
	//pApp->JS_docmailForm(NULL, 0, bUI, (FX_LPCWSTR)cTo, (FX_LPCWSTR)cSubject, (FX_LPCWSTR)cCc, (FX_LPCWSTR)cBcc, (FX_LPCWSTR)cMsg, CPDFDoc_Environment::MailType_Msg);
	
	CFX_ByteString utfTo = cTo.UTF8Encode();
	CFX_ByteString utfCc = cCc.UTF8Encode();
	CFX_ByteString utfBcc = cBcc.UTF8Encode();
	CFX_ByteString utfSubject = cSubject.UTF8Encode();
	CFX_ByteString utfMsg = cMsg.UTF8Encode();
	
	FSCRT_BSTR sTo = {(FS_LPSTR)(FX_LPCSTR)(utfTo),(FS_DWORD)utfTo.GetLength()};
	FSCRT_BSTR sCc = {(FS_LPSTR)(FX_LPCSTR)(utfCc), (FS_DWORD)utfCc.GetLength()};
	FSCRT_BSTR sBcc = {(FS_LPSTR)(FX_LPCSTR)(utfBcc), (FS_DWORD)utfBcc.GetLength()};
	FSCRT_BSTR sSubject = {(FS_LPSTR)(FX_LPCSTR)(utfSubject), (FS_DWORD)utfSubject.GetLength()};
	FSCRT_BSTR sMsg ={(FS_LPSTR)(FX_LPCSTR)(utfMsg), (FS_DWORD)utfMsg.GetLength()};

	pFormFiller->m_pJSActionHandle->Mail((FSPDFDoc)pDocument, NULL, 0,bUI, &sTo, &sSubject, &sCc, &sBcc, &sMsg, FSPDF_MAILTYPE_MSG);
// 	FSCRT_VAR inParams[8] = {{FSCRT_VT_LPVOID, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, NULL},
// 						{FSCRT_VT_DWORD, 0, 0}, {FSCRT_VT_BOOL, 0, bUI}};
// 	FSCRT_Var_SetVoidPtrValue(inParams[0], &sTo);
// 	FSCRT_Var_SetVoidPtrValue(inParams[1], &sCc);
// 	FSCRT_Var_SetVoidPtrValue(inParams[2], &sBcc);
// 	FSCRT_Var_SetVoidPtrValue(inParams[3], &sSubject);
// 	FSCRT_Var_SetVoidPtrValue(inParams[4], &sMsg);
// 	FSCRT_Var_SetBooleanValue(inParams[7], bUI);
// 	FSCRT_VAR retVal = {FSCRT_VT_EMPTY, 0, 0};
// 	//void (*FSCRT_LPAPPFunc_Mail)(const FSCRT_BSTR* to, const FSCRT_BSTR* cc, const FSCRT_BSTR* bcc, const FSCRT_BSTR* subject, const FSCRT_BSTR* msg, FS_LPCVOID attachData, FS_DWORD attachLen, FS_BOOL displayUI);
// 	vRet = pApp->CallFunction(NULL, FSCRT_APPFUNC_MAIL, inParams, 8, &retVal);
	pRuntime->EndBlock();

#endif
	return FALSE;
}

FX_BOOL japp::launchURL(OBJ_METHOD_PARAMS)
{
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;
	FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();

	if (IsSafeMode(cc)) return TRUE;
	CFX_WideString swURL = (FX_LPWSTR)params[0];

#if 0
	pRuntime->BeginBlock();
	//FX_BOOL bRet = pApp->OpenURL(swURL);
	//typedef FS_INT32	(*FSCRT_LPAPPFunc_OpenURL)(const FSCRT_BSTR* url);
	CFX_ByteString utfURL = swURL.UTF8Encode();
	FSCRT_BSTR sURL = {(FS_LPSTR)(FX_LPCSTR)utfURL, (FS_DWORD)utfURL.GetLength()};
// 	FSCRT_VAR inParam = {FSCRT_VT_LPVOID, 0, 0};
// 	FSCRT_Var_SetVoidPtrValue(inParam, &sURL)
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0)
// 	pApp->CallFunction(NULL, FSCRT_APPFUNC_OPENURL, &inParam, 1, &retVal);
	pFormFiller->m_pJSActionHandle->LaunchURL(&sURL);

	pRuntime->EndBlock();
#endif

	return FALSE;
}

FX_BOOL japp::runtimeHighlight(OBJ_PROP_PARAMS)
{
	if (vp.IsSetting())
	{
		vp>>m_bRuntimeHighLight;
	}
	else
	{
		vp<<m_bRuntimeHighLight;
	}

	return TRUE;
}

FX_BOOL japp::fullscreen(OBJ_PROP_PARAMS)
{
	return FALSE;
}

FS_RESULT getMenuTitlesArray(CFXJS_Array& menuArray, FS_INT32 menuLevel, FSPDF_MENULIST *menuList, FS_INT32* count)
{
#if 0
	FS_INT32 j = 0;
	FS_RESULT ret = 0;
	FS_BOOL isFirstNonArrayChild = TRUE;
	while (j < menuArray.GetLength())
	{
		CFXJS_Value subMenuTitle;
		menuArray.GetElement(j, subMenuTitle);
		if (subMenuTitle.IsArrayObject())
		{
			CFXJS_Array subMenuArray;
			subMenuTitle.ConvertToArray(subMenuArray);
			ret = getMenuTitlesArray(subMenuArray, menuLevel+1, menuList, count);
			if (ret != FSCRT_ERRCODE_SUCCESS)
				return ret;
		}
		else //if (subMenuTitle.IsDateObject())
		{
			if (menuList)
			{
				FS_INT32 level = isFirstNonArrayChild?menuLevel-1:menuLevel;
				if (level < 0)level = 0;
				menuList[*count].level = level;
				isFirstNonArrayChild = FALSE;

				CFX_WideString wsMenuTitle= (FX_LPWSTR)subMenuTitle;
				FSCRT_BStr_Init(&(menuList[*count].name));
				FSCRT_ST_FXWStrToFSUTF8(wsMenuTitle, &(menuList[*count].name));
			}	
			(*count)++;
		}
	j++;
	}
#endif
	return FSCRT_ERRCODE_SUCCESS;
}

FX_BOOL japp::popUpMenu(OBJ_METHOD_PARAMS)
{
#if 0
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	FSPDFDoc* pDocument = pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();

	int i = 0;
	CFXJS_Array arJSMenu;
	while(i<params.size())
	{
		CFXJS_Value value = params.GetAt(i);
		arJSMenu.SetElement(i, value);
		i ++;
	}

	FS_INT32 count = 0;
	getMenuTitlesArray(arJSMenu, 0, NULL, &count);

	FSPDF_MENULIST* menuList = NULL;
	FS_RESULT ret = FSCRT_Memory_Alloc(sizeof(FSPDF_MENULIST)*count, (FS_LPVOID*)&menuList);
	if (ret != FSCRT_ERRCODE_SUCCESS)
		return FALSE;

	FS_INT32 index = 0;
	getMenuTitlesArray(arJSMenu, 0, menuList, &index);
	ASSERT(index == count);

	FSCRT_BSTR selectedMenu;
	FSCRT_BStr_Init(&selectedMenu);
	pFormFiller->m_pJSActionHandle->PopupMenu(menuList, count, &selectedMenu);
	CFX_WideString wsSeletedMenu;
	if (selectedMenu.len > 0 && selectedMenu.str){
		FSCRT_ST_FSUTF8ToFXWStr(&selectedMenu, wsSeletedMenu);
		FSCRT_BStr_Clear(&selectedMenu);
	}
	vRet = wsSeletedMenu;

	for(index = 0; index < count; index ++)
	{
		ret = FSCRT_BStr_Clear(&menuList->name);
		if (ret != FSCRT_ERRCODE_SUCCESS)
			return FALSE;
	}
	FSCRT_Memory_Free(menuList);
#endif
	return TRUE;
}

FX_BOOL japp::browseForDoc(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	FSPDFDoc* pDocument = pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = ((FSPDFDocImp*)pDocument)->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	//This method may trigger a "file save" dialog,while enable user to save contents of the document.
	//Such action is considered to be unsafe.
	if (IsSafeMode(cc)) return TRUE;

	bool bSave = false;
	CFX_ByteString cFilenameInit = CFX_ByteString();
	CFX_ByteString cFSInit = CFX_ByteString();

	if(params.size()>0 && (params[0].GetType() == VT_object))
	{
		JSObject* pObj = (JSObject *)params[0];
		if (!pObj) return FALSE;

 		if (Value* pValue = DS_GetObjectElement(pObj,L"bSave"))
 			bSave = (bool)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
 		
		if (Value* pValue = DS_GetObjectElement(pObj,L"cFilenameInit"))
		{
			CFXJS_Value t = CFXJS_Value(pValue, GET_VALUE_TYPE(pValue));
 			cFilenameInit = t.operator CFX_ByteString();
		}
	}
	else
	{
		if(params.size() >= 1)
		{
			bSave = (bool)params[0];
		}
		if(params.size() >= 2)
		{
			CFXJS_Value t = params[1];
			cFilenameInit = t.operator CFX_ByteString();
		}
		if(params.size() >= 3)
		{
			CFXJS_Value t = params[2];
			cFSInit = t.operator CFX_ByteString();
		}
	}
#if 0
	FSCRT_BSTR sPath;
	FSCRT_BStr_Init(&sPath);
//	FSCRT_VAR inParams[3] = {{FSCRT_VT_BOOL, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0}};
// 	FSCRT_Var_SetBooleanValue(inParams[0], bSave)
// 	FSCRT_Var_SetVoidPtrValue(inParams[1], &sFilenameInit)
// 	FSCRT_Var_SetVoidPtrValue(inParams[2], &sPath)
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	FSCRT_Var_SetInt32Value(retVal, 0)
// 	//typedef FS_INT32	(*FSCRT_LPAPPFunc_BrowseFile)(FS_BOOL bSave, const FSCRT_BSTR* initFileName, FSCRT_BSTR* path);
// 	pApp->CallFunction(NULL, FSCRT_APPFUNC_BROWSEFILE, inParams, 3, &retVal);

	pFormFiller->m_pJSActionHandle->BrowseFile(&sPath);

	JSFXObject* pRetObj = DS_NewFxDynamicObj(pRuntime, pContext, -1);
	CFX_WideString wsFilePath;
	FSCRT_ST_FSUTF8ToFXWStr(&sPath, wsFilePath);
	DS_PutObjectString(pRetObj, L"cPath", SysPathToPDFPath(wsFilePath));

	FSCRT_BStr_Clear(&sPath);

	vRet =  pRetObj;
#endif
	return TRUE;
}

CFX_WideString japp::PDFPathToSysPath(const CFX_WideString& sOldPath)
{
	//strLPath = "D:\temporay.fdf";
	CFX_WideString strOPath = sOldPath;
	strOPath.TrimLeft();
	strOPath.TrimRight();

	if (strOPath.GetAt(0) == L'/' && strOPath.GetAt(2) == L'/')
	{
		wchar_t c_Drive = strOPath.GetAt(1);
		if ((c_Drive >= L'a' && c_Drive <= L'z' )||( c_Drive >= L'A' && c_Drive <= L'Z'))
		{
			strOPath.Replace((FX_LPCWSTR)L"/",(FX_LPCWSTR)L"\\");
			//strOPath.SetAt(0,'');
			strOPath.Insert(2,':');
			strOPath.Delete(0);
		}
	}

	return strOPath;
}

CFX_WideString japp::SysPathToPDFPath(const CFX_WideString& sOldPath)
{
	CFX_WideString sRet = L"/";
	
	for (int i=0,sz=sOldPath.GetLength(); i<sz; i++)
	{
		wchar_t c = sOldPath.GetAt(i);
		if (c == L':')
		{
		}
		else
		{
			if (c == L'\\')
			{
				sRet += L"/";
			}
			else
			{
				sRet += c;
			}
		}
	}
	
	return sRet;
}

FX_BOOL japp::newDoc(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL japp::openDoc(OBJ_METHOD_PARAMS)
{
	return FALSE;
}

FX_BOOL japp::response(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);

	CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
	if(!pRuntime) return TRUE;

	FSPDFDocImp* pDocument = (FSPDFDocImp*)pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();
// 	CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 	if (!pApp) return TRUE;

	vRet.SetNull();

	CFX_WideString swQuestion = L"";
	CFX_WideString swLabel = L"";
//#ifndef FOXIT_CHROME_BUILD
	CFX_WideString swTitle = L"Foxit";
// #else
// 	CFX_WideString swTitle = L"PDF";
// #endif
	CFX_WideString swDefault = L"";
	CFX_WideString swResponse = L"";
	bool bPassWord = false;
	
	int iLength = params.size();	
	if (iLength > 0 && params[0].GetType() == VT_object)
	{
		JSObject* pObj = (JSObject *)params[0];
		if (Value* pValue = DS_GetObjectElement(pObj,L"cQuestion"))
			swQuestion = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj,L"cTitle"))
			swTitle = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj,L"cDefault"))
			swDefault = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj,L"cLabel"))
			swLabel = (FX_LPCWSTR)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));

		if (Value* pValue = DS_GetObjectElement(pObj,L"bPassword"))
			bPassWord = (bool)CFXJS_Value(pValue,GET_VALUE_TYPE(pValue));
	}
	else
	{
		switch(iLength)
		{
		case 1:
			swQuestion = params[0];
			break;
		case 2:
			swQuestion = params[0];
			swTitle = params[1];
			break;
		case 3:
			swQuestion = params[0];
			swTitle = params[1];
			swDefault = params[2];
			break;
		case 4:
			swQuestion = params[0];
			swTitle = params[1];
			swDefault = params[2];
			bPassWord = params[3];
			break;
		case 5:
			swQuestion = params[0];
			swTitle = params[1];
			swDefault = params[2];
			bPassWord = params[3];
			swLabel = params[4];
			break;
		default:
			break;
		}
	}


#if 0
	pRuntime->BeginBlock();
	CFX_ByteString utfQuestion = swQuestion.UTF8Encode();
	CFX_ByteString utfTitle = swTitle.UTF8Encode();
	CFX_ByteString utfDefault = swDefault.UTF8Encode();
	CFX_ByteString utfLabel = swLabel.UTF8Encode();
	CFX_ByteString utfResponse = swResponse.UTF8Encode();

	FSCRT_BSTR sQuestion = {(FS_LPSTR)(FX_LPCSTR)(utfQuestion), (FS_DWORD)utfQuestion.GetLength()};
	FSCRT_BSTR sTitle = {(FS_LPSTR)(FX_LPCSTR)(utfTitle), (FS_DWORD)utfTitle.GetLength()};
	FSCRT_BSTR sDefault ={(FS_LPSTR)(FX_LPCSTR)(utfDefault), (FS_DWORD)utfDefault.GetLength()};
	FSCRT_BSTR sLabel = {(FS_LPSTR)(FX_LPCSTR)(utfLabel), (FS_DWORD)utfLabel.GetLength()};
	FSCRT_BSTR sResponse ={NULL, 0};
// 	FSCRT_VAR inParams[6] = {{FSCRT_VT_LPVOID, 0, 0}, {FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_LPVOID, 0, 0},{FSCRT_VT_BOOL, 0, 0},{FSCRT_VT_LPVOID, 0, NULL}};
// 	FSCRT_Var_SetVoidPtrValue(inParams[0], &sQuestion);
// 	FSCRT_Var_SetVoidPtrValue(inParams[1], &sTitle);
// 	FSCRT_Var_SetVoidPtrValue(inParams[2], &sDefault);
// 	FSCRT_Var_SetVoidPtrValue(inParams[3], &sLabel);
// 	FSCRT_Var_SetBooleanValue(inParams[4], bPassWord);
// 	FSCRT_Var_SetVoidPtrValue(inParams[5], &sResponse);
// 	FSCRT_VAR retVal = {FSCRT_VT_INT32, 0, 0};
// 	//typedef FS_INT32	(*FSCRT_LPAPPFunc_Response)(const FSCRT_BSTR* question, const FSCRT_BSTR* title, const FSCRT_BSTR* defaultValue, const FSCRT_BSTR* label, FS_BOOL password, FSCRT_BSTR* response);
// 	FS_RESULT ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_RESPONSE, inParams, 6, &retVal);
//	FSCRT_StartCallBackState();
	FS_RESULT ret = pFormFiller->m_pJSActionHandle->Response(&sQuestion, &sTitle, &sDefault, &sLabel, bPassWord, &sResponse);
	if (FSCRT_ERRCODE_SUCCESS != ret || sResponse.len <= 0) return FALSE;

// 	FX_INT32 nLength = retVal.var.int32Var;
// 	char* pBuff = FX_Alloc(char, nLength + 2);
// 	if(!pBuff) return FALSE;
// 	FXSYS_memset(pBuff, 0, nLength + 2);
// 	sResponse.len = nLength;
// 	sResponse.str = pBuff;
// 	ret = pApp->CallFunction(NULL, FSCRT_APPFUNC_RESPONSE, inParams, 6, &retVal);
// 	if (FSCRT_ERRCODE_SUCCESS != ret) return FALSE;

	if (sResponse.len > 0)
 	{
		swResponse = CFX_WideString::FromUTF8(sResponse.str, sResponse.len);
 		vRet = swResponse;
 	}
 	else
 	{
 		vRet.SetNull();
 	}

	//FX_Free(pBuff);
	//pBuff = NULL;

	pRuntime->EndBlock();
#endif
	return TRUE;
}

FX_BOOL japp::media(OBJ_PROP_PARAMS)
{
	return FALSE;
}

FX_BOOL japp::execDialog(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)