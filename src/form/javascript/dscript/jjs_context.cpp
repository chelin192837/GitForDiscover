#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

CFSCRT_JS_ActionHandler::CFSCRT_JS_ActionHandler()
{
}

FS_RESULT   CFSCRT_JS_ActionHandler::Release()
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::InvalidateRect(FSPDFDoc* document, FS_INT32 pageIndex, FSRectF* pdfRect)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::GetCurrentPageIndex(FSPDFDoc* document, FS_INT32* pageIndex)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::SetCurrentPageIndex(FSPDFDoc* document, FS_INT32 pageIndex)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::GetRotation(FSPDFDoc* document, FS_INT32 pageIndex, FS_INT32* rotation)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::ExecuteNamedAction(FSPDFDoc* document, const char* namedAction)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::SetChangeMark(FSPDFDoc* document, FS_BOOL changeMark)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::GetChangeMark(FSPDFDoc* document, FS_BOOL* changeMark)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::GetOpenedDocs(FSPDFDoc** docs, FS_INT32* count)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::Alert(const char* msg, const char* title, FS_INT32 type, FS_INT32 icon, FS_INT32* retCode)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::Beep(FS_INT32 type)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::Response(const char* question, const char* title, const char* defaultValue, 
	const char* label, FS_BOOL isPassword, char* response)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::GetFilePath(FSPDFDoc* document, char* filePath)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::Mail(FSPDFDoc* document, FX_LPBYTE data, FS_DWORD len, FS_BOOL bUI, const char* To, 
	const char* Subject, const char* CC, const char* BCC, const char* Msg, FS_INT32 nMailType)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::Print(FSPDFDoc* document, FS_BOOL isUI, FS_INT32 start, FS_INT32 end, FS_BOOL isSilent ,
	FS_BOOL isShrinkToFit, FS_BOOL isPrintAsImage, FS_BOOL isReverse, FS_BOOL isAnnotations)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::SubmitForm(FSPDFDoc* document, FS_LPVOID formData, FS_DWORD length, const char* URL)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::LaunchURL(const char* URL)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::BrowseFile(char* filePath)
{
	return 0;
}

FS_RESULT   CFSCRT_JS_ActionHandler::GetLanguage(FS_INT32* language)
{
	return 0;
}

//FS_RESULT   GetIdentityProperties(FSPDF_IDENTITYPROPERTIES* identityProperties);

//FS_RESULT   PopupMenu(const FSPDF_MENULIST* menus, FS_INT32 count, char* seletedMenu);

FS_RESULT   CFSCRT_JS_ActionHandler::GetAppInfo(FS_INT32 type, char* info)
{
	return 0;
}


/* -------------------------- CJS_Context -------------------------- */

CFXJS_Context::CFXJS_Context(CFXJS_Runtime* pRuntime) : 	
	m_pRuntime(pRuntime),
	m_bBusy(FALSE),
	m_bMsgBoxEnable(TRUE)
{
	m_pEventHandler = new CFXJS_EventHandler(this);
}

CFXJS_Context::~CFXJS_Context(void)
{
	if (m_pEventHandler)
	{
		delete m_pEventHandler;
		m_pEventHandler = NULL;
	}
}

FSPDFDoc* CFXJS_Context::GetReaderDocument()
{
	ASSERT(m_pRuntime != NULL);

	return m_pRuntime->GetReaderDocument();
}

CFSCRT_LTAppHandler* CFXJS_Context::GetReaderApp()
{
	ASSERT(m_pRuntime != NULL);
	
	return m_pRuntime->GetReaderApp();
}

FX_BOOL CFXJS_Context::DoJob(int nMode, const CFX_WideString& script, CFX_WideString& info, Value*& pRet, Dobject* pVariables)
{
	FSPDFDocImp* pLTDoc = (FSPDFDocImp*)m_pRuntime->GetReaderDocument();
	FSFormFillerImp* pFormFiller = pLTDoc->GetForm()->GetFormFiller();

	if (m_bBusy)
	{		
		info = JSGetStringFromID(this, IDS_STRING_JSBUSY);
		return FALSE;
	}	

	m_bBusy = TRUE;

	ASSERT(m_pRuntime != NULL);
	ASSERT(m_pEventHandler != NULL);
	ASSERT(m_pEventHandler->IsValid());

	if (!m_pRuntime->AddEventToLoop(m_pEventHandler->TargetName(), m_pEventHandler->EventType()))
	{
		info = JSGetStringFromID(this, IDS_STRING_JSEVENT);

// 		CFX_ByteString cbMessage = CFX_ByteString::FromUnicode(info);
// 		FSCRT_BSTR message = {NULL,0};
// 		FSCRT_ST_FXBStrToFSStr(cbMessage, &message);
// 		pFormFiller->m_pJSActionHandle->OutputJSMessage(&message);
// 		FSCRT_BStr_Clear(&message);
		return FALSE;
	}

	DSErr error;
	int nRet = 0;	
#ifndef _NOEXCEPTION_
	try
#endif
	{	
		if (script.GetLength() > 0)
		{
			if (nMode == 0)
			{		
				nRet = DS_Execute(m_pRuntime, this, script, script.GetLength(), pRet, &error, pVariables);
			}
			else
			{
				nRet = DS_Parse(m_pRuntime, this, script, script.GetLength(), &error);
			}
		}

		if (nRet < 0)
		{
			CFX_WideString sLine;
			if(error.srcline != NULL)
			{
				sLine.Format((FX_LPCWSTR)L"[ Line: %05d { %s } ] : %s",error.linnum-1,error.srcline,error.message);
				info += sLine;
			}

// 			CFX_ByteString cbMessage = CFX_ByteString::FromUnicode(info);
// 			FSCRT_BSTR message = {NULL, 0};
// 			FSCRT_ST_FXBStrToFSStr(cbMessage, &message);
// 			pFormFiller->m_pJSActionHandle->OutputJSMessage(&message);
// 			FSCRT_BStr_Clear(&message);
		}
		else
		{
			info = JSGetStringFromID(this, IDS_STRING_RUN);
		}		

		//为提高JS执行效率
 		if (m_pEventHandler->EventType() != JET_FIELD_CALCULATE)
			DS_FullCollect();
	}
// 	catch (CException *e)
// 	{
// 		e->Delete();
// 		//info = JSGetStringFromID(this, IDS_STRING_UNHANDLED);
// 		info = L"An unhandled error!";
// 		nRet = -1;
// 	}
#ifndef _NOEXCEPTION_
	catch (...)
	{
		info = JSGetStringFromID(this, IDS_STRING_UNHANDLED);
		nRet = -1;
	}
#endif
	m_pRuntime->RemoveEventInLoop(m_pEventHandler->TargetName(), m_pEventHandler->EventType());

	m_pEventHandler->Destroy();
	m_bBusy = FALSE;	

	return nRet >= 0;
}

FX_BOOL CFXJS_Context::RunScript(const CFX_WideString& script, CFX_WideString& info)
{
	Value* pValue = NULL;
	return DoJob(0, script, info, pValue, NULL);
}

FX_BOOL CFXJS_Context::Compile(const CFX_WideString& script, CFX_WideString& info)
{
	Value* pValue = NULL;
	return DoJob(1, script, info, pValue, NULL);
}

void CFXJS_Context::OnApp_Init()
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnApp_Init();
}

void CFXJS_Context::OnDoc_Open(FSPDFDoc* pDoc, const CFX_WideString &strTargetName)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnDoc_Open(pDoc,strTargetName);
}

void CFXJS_Context::OnDoc_WillPrint(FSPDFDoc* pDoc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnDoc_WillPrint(pDoc);
}

void CFXJS_Context::OnDoc_DidPrint(FSPDFDoc* pDoc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnDoc_DidPrint(pDoc);
}

void CFXJS_Context::OnDoc_WillSave(FSPDFDoc* pDoc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnDoc_WillSave(pDoc);
}

void CFXJS_Context::OnDoc_DidSave(FSPDFDoc* pDoc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnDoc_DidSave(pDoc);
}

void CFXJS_Context::OnDoc_WillClose(FSPDFDoc* pDoc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnDoc_WillClose(pDoc);
}

void CFXJS_Context::OnPage_Open(FSPDFDoc* pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnPage_Open(pTarget);
}

void CFXJS_Context::OnPage_Close(FSPDFDoc* pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnPage_Close(pTarget);
}

void CFXJS_Context::OnPage_InView(FSPDFDoc* pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnPage_InView(pTarget);
}

void CFXJS_Context::OnPage_OutView(FSPDFDoc* pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnPage_OutView(pTarget);
}

void CFXJS_Context::OnField_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_MouseDown(bModifier, bShift, pTarget);
}

void CFXJS_Context::OnField_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_MouseEnter(bModifier, bShift, pTarget);
}

void CFXJS_Context::OnField_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_MouseExit(bModifier, bShift, pTarget);
}

void CFXJS_Context::OnField_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_MouseUp(bModifier, bShift, pTarget);
}

void CFXJS_Context::OnField_Focus(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, const CFX_WideString& Value)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_Focus(bModifier, bShift, pTarget, Value);
}

void CFXJS_Context::OnField_Blur(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, const CFX_WideString& Value)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_Blur(bModifier, bShift, pTarget, Value);
}

void CFXJS_Context::OnField_Calculate(CPDF_FormField* pSource, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL& bRc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_Calculate(pSource, pTarget, Value, bRc);
}

void CFXJS_Context::OnField_Format(int nCommitKey, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL bWillCommit)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_Format(nCommitKey, pTarget, Value, bWillCommit);
}


void CFXJS_Context::OnField_Keystroke(int nCommitKey, CFX_WideString& strChange, const CFX_WideString& strChangeEx,
									FX_BOOL bKeyDown, FX_BOOL bModifier, int &nSelEnd,int &nSelStart,
									FX_BOOL bShift, CPDF_FormField* pTarget, CFX_WideString& Value,
									FX_BOOL bWillCommit, FX_BOOL bFieldFull, FX_BOOL& bRc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_Keystroke(nCommitKey, strChange, strChangeEx, bKeyDown,
		bModifier, nSelEnd, nSelStart, bShift, pTarget, Value, bWillCommit, bFieldFull, bRc);
}

void CFXJS_Context::OnField_Validate(CFX_WideString& strChange,const CFX_WideString& strChangeEx,
								   FX_BOOL bKeyDown, FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget,
								   CFX_WideString& Value, FX_BOOL& bRc)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnField_Validate(strChange, strChangeEx, bKeyDown, bModifier, bShift, pTarget, Value, bRc);
}

void CFXJS_Context::OnScreen_Focus(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_Focus(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_Blur(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_Blur(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_Open(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_Open(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_Close(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_Close(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_MouseDown(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_MouseUp(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_MouseEnter(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_MouseExit(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_InView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_InView(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnScreen_OutView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnScreen_OutView(bModifier, bShift, pScreen);
}

void CFXJS_Context::OnBookmark_MouseUp(CPDF_Bookmark* pBookMark)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnBookmark_MouseUp(pBookMark);
}

void CFXJS_Context::OnLink_MouseUp(FSPDFDoc* pTarget)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnLink_MouseUp(pTarget);
}

void CFXJS_Context::OnConsole_Exec()
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnConsole_Exec();
}

void CFXJS_Context::OnExternal_Exec()
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnExternal_Exec();
}

void CFXJS_Context::OnBatchExec(FSPDFDoc* pTarget)
{
	ASSERT(m_pEventHandler != NULL);	
	m_pEventHandler->OnBatchExec(pTarget);
}

void CFXJS_Context::OnMenu_Exec(FSPDFDoc* pTarget,const CFX_WideString& strTargetName)
{
	ASSERT(m_pEventHandler != NULL);
	m_pEventHandler->OnMenu_Exec(pTarget, strTargetName);
}

// FS_RESULT outputMessage(CFX_ByteString info, CFSCRT_JS_ActionHandler* handle)
// {
// 	//	CFX_ByteString cbMessage = CFX_ByteString::FromUnicode(info);
// 	FSCRT_BSTR message = {NULL, 0};
// 	FSCRT_ST_FXBStrToFSStr(info, &message);
// 	FS_RESULT ret = handle->OutputJSMessage(&message);
// 	FSCRT_BStr_Clear(&message);
// 
// 	return ret;
// }

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)