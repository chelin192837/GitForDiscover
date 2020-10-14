#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "document.h"
#include "field.h"

/* ---------------------------- CJS_EventHandler ---------------------------- */

CFXJS_EventHandler::CFXJS_EventHandler(CFXJS_Context * pContext)	 :
	m_pJSContext(pContext),
	m_eEventType(JET_UNKNOWN),
	m_bValid(FALSE),
	m_pWideStrChange(NULL),
	m_nCommitKey(-1),
	m_bKeyDown(FALSE),
	m_bModifier(FALSE),
	m_bShift(FALSE),
	m_pISelEnd(NULL),
	m_nSelEndDu(0),
	m_pISelStart(NULL),
	m_nSelStartDu(0),
	m_bWillCommit(FALSE),
	m_pValue(NULL),
	m_bFieldFull(FALSE),
	m_pbRc(NULL),
	m_bRcDu(FALSE),
	m_pSourceDoc(NULL),
	m_pTargetBookMark(NULL),
	m_pTargetDoc(NULL),
	m_pTargetAnnot(NULL)
{
}

CFXJS_EventHandler::~CFXJS_EventHandler()
{
}

void CFXJS_EventHandler::OnApp_Init()
{
	Initial(JET_APP_INIT);
}

void CFXJS_EventHandler::OnDoc_Open(FSPDFDoc* pDoc, const CFX_WideString& strTargetName)
{
	Initial(JET_DOC_OPEN);

	m_pTargetDoc = pDoc;
	m_strTargetName = strTargetName;	
}

void CFXJS_EventHandler::OnDoc_WillPrint(FSPDFDoc* pDoc)
{
	Initial(JET_DOC_WILLPRINT);

	m_pTargetDoc = pDoc;	
}

void CFXJS_EventHandler::OnDoc_DidPrint(FSPDFDoc* pDoc)
{
	Initial(JET_DOC_DIDPRINT);
	
	m_pTargetDoc = pDoc;		
}

void CFXJS_EventHandler::OnDoc_WillSave(FSPDFDoc* pDoc)
{
	Initial(JET_DOC_WILLSAVE);
	m_pTargetDoc = pDoc;		
}

void CFXJS_EventHandler::OnDoc_DidSave(FSPDFDoc* pDoc)
{
	Initial(JET_DOC_DIDSAVE);	
	
	m_pTargetDoc = pDoc;	
}

void CFXJS_EventHandler::OnDoc_WillClose(FSPDFDoc* pDoc)
{
	Initial(JET_DOC_WILLCLOSE);

	m_pTargetDoc = pDoc;	
}

void CFXJS_EventHandler::OnPage_Open(FSPDFDoc* pDoc)
{
	Initial(JET_PAGE_OPEN);

	m_pTargetDoc = pDoc;
}

void CFXJS_EventHandler::OnPage_Close(FSPDFDoc* pDoc)
{
	Initial(JET_PAGE_CLOSE);
	
	m_pTargetDoc = pDoc;
}

void CFXJS_EventHandler::OnPage_InView(FSPDFDoc* pDoc)
{
	Initial(JET_PAGE_INVIEW);
	
	m_pTargetDoc = pDoc;
}

void CFXJS_EventHandler::OnPage_OutView(FSPDFDoc* pDoc)
{
	Initial(JET_PAGE_OUTVIEW);
	
	m_pTargetDoc = pDoc;
}

void CFXJS_EventHandler::OnField_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget)
{
	Initial(JET_FIELD_MOUSEENTER);

	m_bModifier = bModifier;
	m_bShift = bShift;

	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
}

void CFXJS_EventHandler::OnField_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget)
{
	Initial(JET_FIELD_MOUSEEXIT);

	m_bModifier = bModifier;
	m_bShift = bShift;
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();	
}

void CFXJS_EventHandler::OnField_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget)
{
	Initial(JET_FIELD_MOUSEDOWN);
	m_eEventType = JET_FIELD_MOUSEDOWN;
	
	m_bModifier = bModifier;
	m_bShift = bShift;	
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
}

void CFXJS_EventHandler::OnField_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget)
{
	Initial(JET_FIELD_MOUSEUP);

	m_bModifier = bModifier;
	m_bShift = bShift;
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
}

void CFXJS_EventHandler::OnField_Focus(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, 
									 const CFX_WideString& Value)
{
	Initial(JET_FIELD_FOCUS);

	m_bModifier = bModifier;
	m_bShift = bShift;  
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
	m_pValue = (CFX_WideString*)&Value;
}

void CFXJS_EventHandler::OnField_Blur(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget,
									const CFX_WideString& Value)
{
	Initial(JET_FIELD_BLUR);

	m_bModifier = bModifier;
	m_bShift = bShift;
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
	m_pValue = (CFX_WideString*)&Value;	
}

void CFXJS_EventHandler::OnField_Keystroke(int nCommitKey, CFX_WideString &strChange,
										 const CFX_WideString& strChangeEx, FX_BOOL KeyDown,
										 FX_BOOL bModifier, int& nSelEnd, int& nSelStart,
										 FX_BOOL bShift, CPDF_FormField* pTarget,
										 CFX_WideString& Value, FX_BOOL bWillCommit,
										  FX_BOOL bFieldFull, FX_BOOL& bRc)
{
	Initial(JET_FIELD_KEYSTROKE);
	
	m_nCommitKey = nCommitKey;
	m_pWideStrChange = &strChange;
	m_WideStrChangeEx = strChangeEx;
	m_bKeyDown = KeyDown;
	m_bModifier = bModifier;
	m_pISelEnd = &nSelEnd;
	m_pISelStart = &nSelStart;
	m_bShift = bShift;	
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
	m_pValue = &Value;
	m_bWillCommit = bWillCommit;	
	m_pbRc = &bRc;
	m_bFieldFull = bFieldFull;
}

void CFXJS_EventHandler::OnField_Validate(CFX_WideString& strChange, const CFX_WideString& strChangeEx,
										FX_BOOL bKeyDown, FX_BOOL bModifier, FX_BOOL bShift,
										CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL& bRc)
{
	Initial(JET_FIELD_VALIDATE);
	
	m_pWideStrChange = &strChange;
	m_WideStrChangeEx = strChangeEx;	
	m_bKeyDown = bKeyDown;
	m_bModifier = bModifier;
	m_bShift = bShift;
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
	m_pValue = &Value;	
	m_pbRc = &bRc;	
}

void CFXJS_EventHandler::OnField_Calculate(CPDF_FormField* pSource, CPDF_FormField* pTarget, 
										 CFX_WideString& Value, FX_BOOL& bRc)
{
	Initial(JET_FIELD_CALCULATE);

	if (pSource)
		m_strSourceName = pSource->GetFullName();
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
    m_pValue = &Value;
	m_pbRc = &bRc;
}

void CFXJS_EventHandler::OnField_Format(int nCommitKey, CPDF_FormField* pTarget,
									  CFX_WideString& Value, FX_BOOL bWillCommit)
{
	Initial(JET_FIELD_FORMAT);
	
	m_nCommitKey = nCommitKey;	  
	ASSERT(pTarget != NULL);
	m_strTargetName = pTarget->GetFullName();
	m_pValue = &Value;
	m_bWillCommit = bWillCommit;
}

void CFXJS_EventHandler::OnScreen_Focus(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_FOCUS);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_Blur(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_BLUR);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_Open(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_OPEN);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_Close(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_CLOSE);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_MOUSEDOWN);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_MOUSEUP);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_MOUSEENTER);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_MOUSEEXIT);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_InView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_INVIEW);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnScreen_OutView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen)
{
	Initial(JET_SCREEN_OUTVIEW);

	m_bModifier = bModifier;
	m_bShift = bShift;
	m_pTargetAnnot = pScreen;
}

void CFXJS_EventHandler::OnLink_MouseUp(FSPDFDoc* pTarget)
{
	Initial(JET_LINK_MOUSEUP);
	
	m_pTargetDoc = pTarget;	
}

void CFXJS_EventHandler::OnBookmark_MouseUp(CPDF_Bookmark* pBookMark)
{
	Initial(JET_BOOKMARK_MOUSEUP);

	m_pTargetBookMark = pBookMark;	
}

void CFXJS_EventHandler::OnMenu_Exec(FSPDFDoc* pTarget, const CFX_WideString& strTargetName)
{
	Initial(JET_MENU_EXEC);

	m_pTargetDoc = pTarget;
	m_strTargetName = strTargetName;
}

void CFXJS_EventHandler::OnExternal_Exec()
{
	Initial(JET_EXTERNAL_EXEC);
}

void CFXJS_EventHandler::OnBatchExec(FSPDFDoc* pTarget)
{
	Initial(JET_BATCH_EXEC);

	m_pTargetDoc = pTarget;	
}

void CFXJS_EventHandler::OnConsole_Exec()
{
	Initial(JET_CONSOLE_EXEC);
}


void CFXJS_EventHandler::Initial(JS_EVENT_T type)
{
	m_eEventType = type;

	m_strTargetName = L"";
	m_strSourceName = L"";
	m_pWideStrChange = NULL;
	m_WideStrChangeDu = L"";
	m_WideStrChangeEx = L"";
	m_nCommitKey = -1;
	m_bKeyDown = FALSE;
	m_bModifier = FALSE;
	m_bShift = FALSE;
	m_pISelEnd = NULL;
	m_nSelEndDu = 0;
	m_pISelStart = NULL;
	m_nSelStartDu = 0;
	m_bWillCommit = FALSE;
	m_pValue = NULL;
	m_bFieldFull = FALSE;
	m_pbRc = NULL;
	m_bRcDu = FALSE;

	m_pSourceDoc = NULL;
	m_pTargetBookMark = NULL;
	m_pTargetDoc = NULL;
	m_pTargetAnnot = NULL;

	m_bValid = TRUE;
}

void CFXJS_EventHandler::Destroy()
{
	m_bValid = FALSE;
}

FX_BOOL CFXJS_EventHandler::IsValid()
{
	return m_bValid;
}

CFX_WideString & CFXJS_EventHandler::Change()
{
	if (m_pWideStrChange != NULL)
		return *m_pWideStrChange;
	else
	{
		return m_WideStrChangeDu;
	}
}

CFX_WideString CFXJS_EventHandler::ChangeEx()
{
	return m_WideStrChangeEx;
}

int CFXJS_EventHandler::CommitKey()
{
	return m_nCommitKey;
}

FX_BOOL CFXJS_EventHandler::FieldFull()
{
	return m_bFieldFull;
}

FX_BOOL CFXJS_EventHandler::KeyDown()
{
	return m_bKeyDown;
}

FX_BOOL CFXJS_EventHandler::Modifier()
{
	return m_bModifier;
}

FX_LPCWSTR CFXJS_EventHandler::Name()
{
	switch (m_eEventType)
	{
	case JET_APP_INIT:			return (FX_LPCWSTR)L"Init";
	case JET_BATCH_EXEC:		return (FX_LPCWSTR)L"Exec";
	case JET_BOOKMARK_MOUSEUP:	return (FX_LPCWSTR)L"Mouse Up";
	case JET_CONSOLE_EXEC:		return (FX_LPCWSTR)L"Exec";
	case JET_DOC_DIDPRINT:		return (FX_LPCWSTR)L"DidPrint";
	case JET_DOC_DIDSAVE:		return (FX_LPCWSTR)L"DidSave";
	case JET_DOC_OPEN:			return (FX_LPCWSTR)L"Open";
	case JET_DOC_WILLCLOSE:		return (FX_LPCWSTR)L"WillClose";
	case JET_DOC_WILLPRINT:		return (FX_LPCWSTR)L"WillPrint";
	case JET_DOC_WILLSAVE:		return (FX_LPCWSTR)L"WillSave";
	case JET_EXTERNAL_EXEC:		return (FX_LPCWSTR)L"Exec";
	case JET_FIELD_FOCUS:		
	case JET_SCREEN_FOCUS:		return (FX_LPCWSTR)L"Focus";
	case JET_FIELD_BLUR:		
	case JET_SCREEN_BLUR:		return (FX_LPCWSTR)L"Blur";
	case JET_FIELD_MOUSEDOWN:
	case JET_SCREEN_MOUSEDOWN:	return (FX_LPCWSTR)L"Mouse Down";
	case JET_FIELD_MOUSEUP:		
	case JET_SCREEN_MOUSEUP:	return (FX_LPCWSTR)L"Mouse Up";
	case JET_FIELD_MOUSEENTER:
	case JET_SCREEN_MOUSEENTER:	return (FX_LPCWSTR)L"Mouse Enter";
	case JET_FIELD_MOUSEEXIT:
	case JET_SCREEN_MOUSEEXIT:	return (FX_LPCWSTR)L"Mouse Exit";
	case JET_FIELD_CALCULATE:	return (FX_LPCWSTR)L"Calculate";
	case JET_FIELD_FORMAT:		return (FX_LPCWSTR)L"Format";
	case JET_FIELD_KEYSTROKE:	return (FX_LPCWSTR)L"Keystroke";
	case JET_FIELD_VALIDATE:	return (FX_LPCWSTR)L"Validate";
	case JET_LINK_MOUSEUP:		return (FX_LPCWSTR)L"Mouse Up";
	case JET_MENU_EXEC:			return (FX_LPCWSTR)L"Exec";
	case JET_PAGE_OPEN:		
	case JET_SCREEN_OPEN:		return (FX_LPCWSTR)L"Open";
	case JET_PAGE_CLOSE:
	case JET_SCREEN_CLOSE:		return (FX_LPCWSTR)L"Close";
	case JET_SCREEN_INVIEW:	
	case JET_PAGE_INVIEW:		return (FX_LPCWSTR)L"InView";
	case JET_PAGE_OUTVIEW:
	case JET_SCREEN_OUTVIEW:	return (FX_LPCWSTR)L"OutView";
	}

	return (FX_LPCWSTR)L"";
}

FX_LPCWSTR CFXJS_EventHandler::Type()
{
	switch (m_eEventType)
	{
	case JET_APP_INIT:			return (FX_LPCWSTR)L"App";
	case JET_BATCH_EXEC:		return (FX_LPCWSTR)L"Batch";
	case JET_BOOKMARK_MOUSEUP:	return (FX_LPCWSTR)L"BookMark";	
	case JET_CONSOLE_EXEC:		return (FX_LPCWSTR)L"Console";
	case JET_DOC_DIDPRINT:
	case JET_DOC_DIDSAVE:
	case JET_DOC_OPEN:
	case JET_DOC_WILLCLOSE:
	case JET_DOC_WILLPRINT:
	case JET_DOC_WILLSAVE:		return (FX_LPCWSTR)L"Doc";
	case JET_EXTERNAL_EXEC:		return (FX_LPCWSTR)L"External";
	case JET_FIELD_BLUR:
	case JET_FIELD_FOCUS:
	case JET_FIELD_MOUSEDOWN:
	case JET_FIELD_MOUSEENTER:
	case JET_FIELD_MOUSEEXIT:
	case JET_FIELD_MOUSEUP:
	case JET_FIELD_CALCULATE:
	case JET_FIELD_FORMAT:
	case JET_FIELD_KEYSTROKE:
	case JET_FIELD_VALIDATE:	return (FX_LPCWSTR)L"Field";
	case JET_SCREEN_FOCUS:
	case JET_SCREEN_BLUR:
	case JET_SCREEN_OPEN:
	case JET_SCREEN_CLOSE:
	case JET_SCREEN_MOUSEDOWN:
	case JET_SCREEN_MOUSEUP:
	case JET_SCREEN_MOUSEENTER:
	case JET_SCREEN_MOUSEEXIT:
	case JET_SCREEN_INVIEW:
	case JET_SCREEN_OUTVIEW:	return (FX_LPCWSTR)L"Screen";
	case JET_LINK_MOUSEUP:		return (FX_LPCWSTR)L"Link";	
	case JET_MENU_EXEC:			return (FX_LPCWSTR)L"Menu";
	case JET_PAGE_OPEN:
	case JET_PAGE_CLOSE:
	case JET_PAGE_INVIEW:
	case JET_PAGE_OUTVIEW:return (FX_LPCWSTR)L"Page";
	}

	return (FX_LPCWSTR)L"";
}

FX_BOOL& CFXJS_EventHandler::Rc()
{
	if (m_pbRc != NULL)
		return *m_pbRc;
	else
	{	    
		return m_bRcDu;
	}
}

int & CFXJS_EventHandler::SelEnd()
{
	if (m_pISelEnd != NULL)
	{
		return *m_pISelEnd;
	}
	else
	{
		return m_nSelEndDu;
	}
}

int & CFXJS_EventHandler::SelStart()
{
	if (m_pISelStart != NULL)
		return * m_pISelStart;
	else
	{
		return m_nSelStartDu;
	}
}

FX_BOOL CFXJS_EventHandler::Shift()
{
	return m_bShift;
}

JField* CFXJS_EventHandler::Source()
{
	ASSERT(m_pJSContext != NULL);

	CFXJS_Runtime* pRuntime = m_pJSContext->GetJSRuntime();
	if(!pRuntime) return NULL;
	JSFXObject * pDocObj = DS_NewFxDynamicObj(pRuntime, m_pJSContext, DS_GetObjDefnID(pRuntime, L"Document"));
	ASSERT(pDocObj != NULL);
	CFXJS_Document* pJSDocument = (CFXJS_Document*)DS_GetPrivate(pDocObj);
	ASSERT(pJSDocument != NULL);
	JDocument* pDocument = (JDocument*)pJSDocument->GetEmbedObject();
	ASSERT(pDocument != NULL);
 	if (m_pTargetDoc != NULL)
 		pDocument->AttachDoc(m_pTargetDoc);
 	else
 		pDocument->AttachDoc(m_pJSContext->GetReaderDocument());
	
	//if (m_pSourceField == NULL)
	//	return NULL;
	//CRAO_Widget *pWidget = IBCL_Widget::GetWidget(m_pSourceField);
	//CPDF_FormField* pFormField = pWidget->GetFormField();
	//ASSERT(pFormField);
	//CFX_WideString csFieldName = pFormField->GetFullName();

	JSFXObject * pFieldObj = DS_NewFxDynamicObj(pRuntime, m_pJSContext, DS_GetObjDefnID(pRuntime, L"Field"));
	ASSERT(pFieldObj != NULL);
	CFXJS_Field * pJSField = (CFXJS_Field*)DS_GetPrivate(pFieldObj);
	ASSERT(pJSField != NULL);
	JField * pField = (JField *)pJSField->GetEmbedObject(); 
	ASSERT(pField != NULL);
	pField->AttachField(pDocument, m_strSourceName);
	return pField;	
}

JField* CFXJS_EventHandler::Target_Field()
{
	ASSERT(m_pJSContext != NULL);

	CFXJS_Runtime* pRuntime = m_pJSContext->GetJSRuntime();
	if(!pRuntime) return NULL;
	JSFXObject* pDocObj = DS_NewFxDynamicObj(pRuntime, m_pJSContext, DS_GetObjDefnID(pRuntime, L"Document"));
	ASSERT(pDocObj != NULL);
	CFXJS_Document* pJSDocument = (CFXJS_Document*)DS_GetPrivate(pDocObj);
	ASSERT(pJSDocument != NULL);
	JDocument* pDocument = (JDocument*)pJSDocument->GetEmbedObject();
 	ASSERT(pDocument != NULL);
 	if (m_pTargetDoc != NULL)
 		pDocument->AttachDoc(m_pTargetDoc);
 	else
 		pDocument->AttachDoc(m_pJSContext->GetReaderDocument());
	
	JSFXObject* pFieldObj = DS_NewFxDynamicObj(pRuntime, m_pJSContext, DS_GetObjDefnID(pRuntime, L"Field"));
	ASSERT(pFieldObj != NULL);

	CFXJS_Field* pJSField = (CFXJS_Field*)DS_GetPrivate(pFieldObj);
	ASSERT(pJSField != NULL);

	JField* pField = (JField *)pJSField->GetEmbedObject(); 
	ASSERT(pField != NULL);

	pField->AttachField(pDocument, m_strTargetName);
	return pField;	
}

CFX_WideString& CFXJS_EventHandler::Value()
{
	return *m_pValue;
}

FX_BOOL CFXJS_EventHandler::WillCommit()
{
	return m_bWillCommit;
}

CFX_WideString CFXJS_EventHandler::TargetName()
{
	return m_strTargetName;
}


#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)