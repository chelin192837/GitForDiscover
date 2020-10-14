#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

/* ---------------------------------  CJS_EmbedObj --------------------------------- */

CFXJS_EmbedObj::CFXJS_EmbedObj(CFXJS_Object* pJSObject) : 
	m_pJSObject(pJSObject)
{
}

CFXJS_EmbedObj::~CFXJS_EmbedObj()
{
	m_pJSObject = NULL;

}

void CFXJS_EmbedObj::Alert(CFXJS_Context* pContext, FX_LPCWSTR swMsg)
{
	CFXJS_Object::Alert(pContext, swMsg);
}

CFXJS_Timer* CFXJS_EmbedObj::BeginTimer(CFSCRT_LTAppHandler* pApp,FX_DWORD nElapse)
{
	CFXJS_Timer* pTimer = new CFXJS_Timer(this, pApp);
	pTimer->SetJSTimer(nElapse);
	
	return pTimer;
}

void CFXJS_EmbedObj::EndTimer(CFXJS_Timer* pTimer)
{
	ASSERT(pTimer != NULL);
	pTimer->KillJSTimer();
	delete pTimer;
}

FX_BOOL	CFXJS_EmbedObj::IsSafeMode(IDS_Context* cc)
{
	return FALSE;
// 	ASSERT(cc != NULL);
// 
// 	CJS_Context* pContext = (CJS_Context*)cc;
// 
// 	CPDFDoc_Environment* pApp = pContext->GetReaderApp();
// 	ASSERT(pApp != NULL);
// 	return pApp->IsEnableSafeMode();

// 	if (pApp->IsReadSafeMode())
// 	{
// 		if (FSCRT_DOCUMENT pDoc = pContext->GetReaderDocument())
// 		{
// 			if (pDoc->GetClientData(pContext->GetJSRuntime()) == NULL)
// 			{	
// 				Alert(pContext, JS_LoadString(pApp, IDS_STRING_SAFEMODEL));
// 
// 				pDoc->SetClientData(pContext->GetJSRuntime(), (void*)1, NULL);
// 			}
// 		}
// 
// 		return TRUE;
// 	}

//	return TRUE;
}

/* ---------------------------------  CJS_Object --------------------------------- */

CFXJS_Object::CFXJS_Object(JSFXObject* pObject) : m_pObject(pObject),m_pEmbedObj(NULL)
{
};

CFXJS_Object::~CFXJS_Object(void)
{
	delete m_pEmbedObj;
	m_pEmbedObj = NULL;
	m_pObject = NULL;
};

void CFXJS_Object::Alert(CFXJS_Context* pContext, FX_LPCWSTR swMsg)
{
	ASSERT(pContext != NULL);

	if (pContext->IsMsgBoxEnabled())
	{
// 		CPDFSDK_PageView* pPageView = JSGetPageView(pContext);
// 		ASSERT(pPageView != NULL);
// 
// 		FSCRT_DOCUMENT pDocument = pContext->GetReaderDocument();
// 		ASSERT(pDocument != NULL);
// 
// 		CPDFDoc_Environment* pApp = pDocument->GetApp();
// 		ASSERT(pApp != NULL);
// 
// 		::MessageBox(pPageView->GetPageViewWnd(), swMsg, pApp->GetAppTitle(), MB_OK | MB_ICONEXCLAMATION);
// 		CPDFDoc_Environment* pApp = pContext->GetReaderApp();
// 		if(pApp)
// 			pApp->JS_appAlert(swMsg, NULL, 0, 3);
		FSPDFDocImp* pDocument = (FSPDFDocImp*)pContext->GetReaderDocument();
		FSFormFillerImp* pFormFiller = pDocument->GetForm()->GetFormFiller();
// 		CFSCRT_LTAppHandler* pApp = FSCRT_GetLTAppHandler();
// 		if (!pApp) return;
		ASSERT(pContext != NULL);
		CFXJS_Runtime* pRuntime = pContext->GetJSRuntime();
		if(!pRuntime) return ;
		pRuntime->BeginBlock();
		CFX_WideString swMessage(swMsg);
		CFX_WideString swTitle;

		CFX_ByteString utf8Msg = swMessage.UTF8Encode();
		CFX_ByteString utf8Title = swTitle.UTF8Encode();
		CFS_Library* pLib = CFS_Library::get();
		if (!pLib) return;
		FSActionHandler* pActionHandler = pLib->getActionHandler();
		if (!pActionHandler) return;
		pActionHandler->alert((FX_LPSTR)(FX_LPCSTR)utf8Msg, (FX_LPSTR)(FX_LPCSTR)utf8Title, 0, 0);
		pRuntime->EndBlock();

	}
}

FX_DWORD CFXJS_Timer::SetJSTimer(FX_DWORD nElapse)
{	
	if (m_nTimerID)KillJSTimer();

	//typedef FS_INT32	(*FSCRT_LPAPPFunc_SetTimer)(FS_INT32 uElapse, FSCRT_CALLBACK_TIMER callbackTimer);
	FSPDFDocImp* pDoc = (FSPDFDocImp*)m_pRuntime->GetReaderDocument();
	FSFormImp* pForm = pDoc->GetForm();
	if(!pForm) return 0;
	FSFormFillerImp* pFormFiller = pForm->GetFormFiller();

	FS_INT32 timerID = 0;
	pFormFiller->m_formInterWindowless.SetTimer(500, (FSCRT_CALLBACK_TIMER)TimerProc, &timerID);

	m_nTimerID = timerID;
	if (timerID != 0){
		m_sTimeMap.SetAt(m_nTimerID,this);
		m_dwElapse = nElapse;
	}
	return m_nTimerID;
};

void CFXJS_Timer::KillJSTimer()
{
	if (m_nTimerID)
	{
		//typedef void		(*FSCRT_LPAPPFunc_KillTimer)(FS_INT32 timer);

		FSPDFDocImp* pDoc = (FSPDFDocImp*)m_pRuntime->GetReaderDocument();
		FSFormImp* pForm = pDoc->GetForm();
		if(!pForm) return;
		FSFormFillerImp* pFormFiller = pForm->GetFormFiller();

		FS_INT32 timerID = (FS_INT32)m_nTimerID;
		pFormFiller->m_formInterWindowless.KillTimer(timerID);

		m_sTimeMap.RemoveAt(m_nTimerID);
		m_nTimerID = 0;
	}
};

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)