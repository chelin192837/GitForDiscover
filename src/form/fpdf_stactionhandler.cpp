#include "../inc/fs_precompilers.h"

#if defined(_FSDK_ALL_) || defined(_PDFFORMFILLER_) /*|| defined(_FSDK_PDFFORM_R_)*/
#include "../inc/fpdf_define.h"
#include "fpdf_stactionhandler.h"

FX_BOOL	CPDFSDK_FormActionHandler::DoAction_Hide(const CPDF_Action& action, FSPDFDoc* pDocument)
{
	ASSERT(pDocument != NULL);

	FSPDFDocImp* pDoc = (FSPDFDocImp*)pDocument;
	FSForm* pInterForm = pDoc->getForm();
	ASSERT(pInterForm != NULL);

// 	if (pInterForm->DoAction_Hide(action))
// 	{
// 		pDocument->SetChangeMark();
// 		return TRUE;
// 	}

	return FALSE;
}

FX_BOOL	CPDFSDK_FormActionHandler::DoAction_SubmitForm(const CPDF_Action& action, FSPDFDoc* pDocument)
{
	ASSERT(pDocument != NULL);

	FSPDFDocImp* pDoc = (FSPDFDocImp*)pDocument;
	FSFormImp* pInterForm = (FSFormImp*)pDoc->getForm();
	ASSERT(pInterForm != NULL);

	return pInterForm->ST_GetSTInterForm()->DoAction_SubmitForm(action);

	return FALSE;
}

FX_BOOL	CPDFSDK_FormActionHandler::DoAction_ResetForm(const CPDF_Action& action, FSPDFDoc* pDocument)
{
	ASSERT(pDocument != NULL);

	FSFormImp* pInterForm = (FSFormImp*)((FSPDFDocImp*)pDocument)->getForm();
	ASSERT(pInterForm != NULL);

 	if (pInterForm->ST_GetSTInterForm()->DoAction_ResetForm(action))
 	{
// 		// 		pDocument->SetChangeMark();
// 		// 		CReader_App* pApp = pDocument->GetApp();
// 		// 		ASSERT(pApp != NULL);
// 		// 		IUndo* pUndo = IUndo::GetUndo(pApp);
// 		// 		ASSERT(pUndo != NULL);
// 		// 		pUndo->Reset(pDocument);
// 
// 		return TRUE;
 	}

	return FALSE;
}

FX_BOOL	CPDFSDK_FormActionHandler::DoAction_ImportData(const CPDF_Action& action, FSPDFDoc* pDocument)
{
	ASSERT(pDocument != NULL);

	FSFormImp* pInterForm = (FSFormImp*)((FSPDFDocImp*)pDocument)->getForm();
	ASSERT(pInterForm != NULL);

// 	if (pInterForm->DoAction_ImportData(action))
// 	{
// 		pDocument->SetChangeMark();
// 		// 		CReader_App* pApp = pDocument->GetApp();
// 		// 		ASSERT(pApp != NULL);
// 		// 		IUndo* pUndo = IUndo::GetUndo(pApp);
// 		// 		ASSERT(pUndo != NULL);
// 		// 		pUndo->Reset(pDocument);
// 
// 		return TRUE;
// 	}

	return FALSE;
}

CFSCRT_STPDFActionHandler::CFSCRT_STPDFActionHandler()
{
	m_pFormActionHandler = new CPDFSDK_FormActionHandler;
	m_pMediaActionHandler = NULL;
}

CFSCRT_STPDFActionHandler::~CFSCRT_STPDFActionHandler()
{
	if(m_pFormActionHandler)
		delete m_pFormActionHandler;
	m_pFormActionHandler = NULL;
}

// void CFSCRT_STPDFActionHandler::SetFormActionHandler(CPDFSDK_FormActionHandler* pHandler)
// {
// 	ASSERT(pHandler != NULL);
// 	ASSERT(m_pFormActionHandler == NULL);
// 	m_pFormActionHandler = pHandler;
// }
// 
// void CFSCRT_STPDFActionHandler::SetMediaActionHandler(CPDFSDK_MediaActionHandler* pHandler)
// {
// 	ASSERT(pHandler != NULL);
// 	ASSERT(m_pMediaActionHandler == NULL);
// 	m_pMediaActionHandler = pHandler;
// }

void CFSCRT_STPDFActionHandler::Destroy()
{
	delete this;
}

//document open
FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_DocOpen(const CPDF_Action& action, FSPDFDoc* pDocument
	/*CReader_Document* pDocument, CReader_DocView *pDocView*/)
{
	CFX_PtrList list;
	return ExecuteDocumentOpenAction(action, pDocument, /*pDocView, */list);
}

//document open
FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_JavaScript(const CPDF_Action& JsAction,CFX_WideString csJSName,
	FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/)
{
	if (JsAction.GetType() == CPDF_Action::JavaScript)
	{
		CFX_WideString swJS = JsAction.GetJavaScript();
		if (!swJS.IsEmpty())
		{
			RunDocumentOpenJavaScript(pDocument, csJSName, swJS);
			return TRUE;
		}
	}

	return FALSE;
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_FieldJavaScript(const CPDF_Action& JsAction, CPDF_AAction::AActionType type, 
	FSPDFDoc* pDocument, CPDF_FormField* pFormField, 
	FSPDF_FieldActionParam& data)
{
	if (JsAction.GetType() == CPDF_Action::JavaScript)
	{
		CFX_WideString swJS = JsAction.GetJavaScript();
		if (!swJS.IsEmpty())
		{
			RunFieldJavaScript(pDocument, pFormField, type, data, swJS);
			return TRUE;
		}
	}
	return FALSE;
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_Page(const CPDF_Action& action, enum CPDF_AAction::AActionType eType,
	FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/)
{
	CFX_PtrList list;
	return ExecuteDocumentPageAction(action, eType, pDocument,/* pDocView,*/ list);
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_Document(const CPDF_Action& action, enum CPDF_AAction::AActionType eType,
	FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/)
{
	CFX_PtrList list;
	return ExecuteDocumentPageAction(action, eType, pDocument,/* pDocView,*/ list);
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_BookMark(CPDF_Bookmark *pBookMark, const CPDF_Action& action, CPDF_AAction::AActionType type, 
	FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/)
{
	CFX_PtrList list;
	return this->ExecuteBookMark(action, pDocument,/* pDocView,*/ pBookMark, list);
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_Screen(const CPDF_Action& action, CPDF_AAction::AActionType type, 
	FSPDFDoc* pDocument,/* CReader_DocView *pDocView,*/ CFSCRT_LTPDFAnnot* pScreen)
{
	CFX_PtrList list;
	return this->ExecuteScreenAction(action, type, pDocument,/* pDocView,*/ pScreen, list);
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_Link(const CPDF_Action& action, 
	FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/)
{
	CFX_PtrList list;
	return ExecuteLinkAction(action, pDocument,/* pDocView,*/ list);
}

FX_BOOL	CFSCRT_STPDFActionHandler::DoAction_Field(const CPDF_Action& action, CPDF_AAction::AActionType type, 
	FSPDFDoc* pDocument,/* CReader_DocView *pDocView,*/ 
	CPDF_FormField* pFormField, FSPDF_FieldActionParam& data)
{
	CFX_PtrList list;
	return ExecuteFieldAction(action, type, pDocument,/* pDocView,*/ pFormField, data, list);
}

FX_BOOL	CFSCRT_STPDFActionHandler::ExecuteDocumentOpenAction(const CPDF_Action& action, FSPDFDoc* pDocument,
	/*CReader_DocView *pDocView,*/ CFX_PtrList& list)
{
	ASSERT(pDocument != NULL);

	if (list.Find((CPDF_Dictionary*)action))
		return FALSE;
	list.AddTail((CPDF_Dictionary*)action);

	if (action.GetType() == CPDF_Action::JavaScript)
	{

		CFX_WideString swJS = action.GetJavaScript();
		if (!swJS.IsEmpty())
		{
			RunDocumentOpenJavaScript(pDocument, L"", swJS);
		}

	}
	else
	{
		DoAction_NoJs(action, pDocument/*, pDocView*/);
	}

	// 	if (!IsValidDocView(pDocument, pDocView))
	// 		return FALSE;

	for (FX_INT32 i=0,sz=action.GetSubActionsCount(); i<sz; i++)
	{
		CPDF_Action subaction = action.GetSubAction(i);
		if (!ExecuteDocumentOpenAction(subaction, pDocument,/* pDocView,*/ list)) return FALSE;
	}

	return TRUE;
}

FX_BOOL CFSCRT_STPDFActionHandler::ExecuteLinkAction(const CPDF_Action& action,	FSPDFDoc* pDocument,
	/*CReader_DocView* pDocView,*/ CFX_PtrList& list)
{
	ASSERT(pDocument != NULL);
	FSPDFDocImp* pDoc = (FSPDFDocImp*)pDocument;

	if (list.Find((CPDF_Dictionary*)action))
		return FALSE;
	list.AddTail((CPDF_Dictionary*)action);

	if (action.GetType() == CPDF_Action::JavaScript)
	{
		CFX_WideString swJS = action.GetJavaScript();
		if (!swJS.IsEmpty())
		{

			IJS_Runtime* pRuntime = pDoc->GetJsRuntime(); //????
			if(!pRuntime) return FALSE;

			pRuntime->SetReaderDocument(pDocument);

			IJS_Context* pContext = pRuntime->NewContext();
			ASSERT(pContext != NULL);

			pContext->OnLink_MouseUp(pDocument);

			CFX_WideString csInfo;
			FX_BOOL bRet = pContext->RunScript(swJS, csInfo);
			if (!bRet)
			{
				//CBCL_FormNotify::MsgBoxJSError(pPageView->GetPageViewWnd(), csInfo);
			}

			pRuntime->ReleaseContext(pContext);

		}
	}
	else
	{
		DoAction_NoJs(action, pDocument/*, pDocView*/);
	}

	// 	if (!IsValidDocView(pDocument, pDocView))
	// 		return FALSE;

	for (FX_INT32 i=0,sz=action.GetSubActionsCount(); i<sz; i++)
	{
		CPDF_Action subaction = action.GetSubAction(i);
		if (!ExecuteLinkAction(subaction, pDocument,/* pDocView,*/ list)) return FALSE;
	}

	return TRUE;
}

FX_BOOL	CFSCRT_STPDFActionHandler::ExecuteDocumentPageAction(const CPDF_Action& action, CPDF_AAction::AActionType type,
	FSPDFDoc* pDocument,/* CReader_DocView* pDocView,*/ CFX_PtrList& list)
{
	ASSERT(pDocument != NULL);

	if (list.Find((CPDF_Dictionary*)action))
		return FALSE;
	list.AddTail((CPDF_Dictionary*)action);

	if (action.GetType() == CPDF_Action::JavaScript)
	{
		CFX_WideString swJS = action.GetJavaScript();
		if (!swJS.IsEmpty())
		{
			RunDocumentPageJavaScript(pDocument, type, swJS);
		}
	}
	else
	{
		DoAction_NoJs(action, pDocument/*, pDocView*/);
	}

	if (!IsValidDocView(pDocument/*, pDocView*/))
		return FALSE;

	for (FX_INT32 i=0,sz=action.GetSubActionsCount(); i<sz; i++)
	{
		CPDF_Action subaction = action.GetSubAction(i);
		if (!ExecuteDocumentPageAction(subaction, type, pDocument,/* pDocView,*/ list)) return FALSE;
	}

	return TRUE;
}

FX_BOOL	CFSCRT_STPDFActionHandler::IsValidField(FSPDFDoc* pDocument, CPDF_Dictionary* pFieldDict)
{
	ASSERT(pDocument != NULL);
	ASSERT(pFieldDict != NULL);

	if (1/*m_pApp->IsValidDocument(pDocument)*/)
	{
		FSFormImp* pInterForm = (FSFormImp*)((FSPDFDocImp*)pDocument)->getForm();
		ASSERT(pInterForm != NULL);

		CPDF_InterForm* pPDFInterForm = pInterForm->ST_GetSTInterForm()->m_pInterForm;
		ASSERT(pPDFInterForm != NULL);

		return pPDFInterForm->GetFieldByDict(pFieldDict) != NULL;
	}

	return FALSE;
}

FX_BOOL	CFSCRT_STPDFActionHandler::ExecuteFieldAction(const CPDF_Action& action, CPDF_AAction::AActionType type, 
	FSPDFDoc* pDocument,/* CReader_DocView* pDocView,*/ CPDF_FormField* pFormField, 
	FSPDF_FieldActionParam& data, CFX_PtrList& list)
{
	ASSERT(pDocument != NULL);

	if (list.Find((CPDF_Dictionary*)action))
		return FALSE;
	list.AddTail((CPDF_Dictionary*)action);

	if (action.GetType() == CPDF_Action::JavaScript)
	{
		CFX_WideString swJS = action.GetJavaScript();
		if (!swJS.IsEmpty())
		{
			RunFieldJavaScript(pDocument, pFormField, type, data, swJS);
			if (!IsValidField(pDocument, pFormField->GetFieldDict()))
				return FALSE;
		}
	}
	else
	{
		DoAction_NoJs(action, pDocument/*, pDocView*/);
		// 		if (!IsValidDocView(pDocument, pDocView))
		// 			return FALSE;
	}

	for (FX_INT32 i=0,sz=action.GetSubActionsCount(); i<sz; i++)
	{
		CPDF_Action subaction = action.GetSubAction(i);
		if (!ExecuteFieldAction(subaction, type, pDocument,/* pDocView,*/ pFormField, data, list)) return FALSE;
	}

	return TRUE;
}

FX_BOOL CFSCRT_STPDFActionHandler::ExecuteScreenAction(const CPDF_Action& action, CPDF_AAction::AActionType type, 
	FSPDFDoc* pDocument,/* CReader_DocView* pDocView,*/ CFSCRT_LTPDFAnnot* pScreen, CFX_PtrList& list)
{
	ASSERT(pDocument != NULL);

	if (list.Find((CPDF_Dictionary*)action))
		return FALSE;
	list.AddTail((CPDF_Dictionary*)action);

	if (action.GetType() == CPDF_Action::JavaScript)
	{
#if 0
			CFX_WideString swJS = action.GetJavaScript();
			if (!swJS.IsEmpty())
			{
				IJS_Runtime* pRuntime = pDocument->GetJsRuntime();
				ASSERT(pRuntime != NULL);

				pRuntime->SetReaderDocument(pDocument);

				IJS_Context* pContext = pRuntime->NewContext();
				ASSERT(pContext != NULL);

				// 			switch (type)
				// 			{
				// 			case CPDF_AAction::CursorEnter:
				// 				pContext->OnScreen_MouseEnter(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::CursorExit:
				// 				pContext->OnScreen_MouseExit(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::ButtonDown:
				// 				pContext->OnScreen_MouseDown(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::ButtonUp:
				// 				pContext->OnScreen_MouseUp(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::GetFocus:
				// 				pContext->OnScreen_Focus(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::LoseFocus:
				// 				pContext->OnScreen_Blur(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::PageOpen:
				// 				pContext->OnScreen_Open(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::PageClose:
				// 				pContext->OnScreen_Close(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::PageVisible:
				// 				pContext->OnScreen_InView(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			case CPDF_AAction::PageInvisible:
				// 				pContext->OnScreen_OutView(IsCTRLpressed(), IsSHIFTpressed(), pScreen);
				// 				break;
				// 			default:
				// 				ASSERT(FALSE);
				// 				break;
				// 			}

				CFX_WideString csInfo;
				FX_BOOL bRet = pContext->RunScript(swJS, csInfo);
				if (!bRet)
				{
					//CBCL_FormNotify::MsgBoxJSError(pPageView->GetPageViewWnd(), csInfo);
				}

				pRuntime->ReleaseContext(pContext);
			}
#endif
	}
	else
	{
		DoAction_NoJs(action, pDocument/*, pDocView*/);
	}

	// 	if (!IsValidDocView(pDocument, pDocView))
	// 		return FALSE;

	for (FX_INT32 i=0,sz=action.GetSubActionsCount(); i<sz; i++)
	{
		CPDF_Action subaction = action.GetSubAction(i);
		if (!ExecuteScreenAction(subaction, type, pDocument,/* pDocView,*/ pScreen, list)) return FALSE;
	}

	return TRUE;
}

FX_BOOL	CFSCRT_STPDFActionHandler::ExecuteBookMark(const CPDF_Action& action, FSPDFDoc* pDocument, 
	/*CReader_DocView* pDocView,*/ CPDF_Bookmark* pBookmark, CFX_PtrList& list)
{
	ASSERT(pDocument != NULL);

	if (list.Find((CPDF_Dictionary*)action))
		return FALSE;
	list.AddTail((CPDF_Dictionary*)action);

	if (action.GetType() == CPDF_Action::JavaScript)
	{
#if 0
			CFX_WideString swJS = action.GetJavaScript();
			if (!swJS.IsEmpty())
			{
				IJS_Runtime* pRuntime = pDocument->GetJsRuntime();
				ASSERT(pRuntime != NULL);

				pRuntime->SetReaderDocument(pDocument);

				IJS_Context* pContext = pRuntime->NewContext();
				ASSERT(pContext != NULL);

				pContext->OnBookmark_MouseUp(pBookmark);

				CFX_WideString csInfo;
				FX_BOOL bRet = pContext->RunScript(swJS, csInfo);
				if (!bRet)
				{
					//CBCL_FormNotify::MsgBoxJSError(pPageView->GetPageViewWnd(), csInfo);
				}

				pRuntime->ReleaseContext(pContext);
			}
#endif
	}
	else
	{
		DoAction_NoJs(action, pDocument/*, pDocView*/);
	}

	// 	if (!IsValidDocView(pDocument, pDocView))
	// 		return FALSE;

	for (FX_INT32 i=0,sz=action.GetSubActionsCount(); i<sz; i++)
	{
		CPDF_Action subaction = action.GetSubAction(i);
		if (!ExecuteBookMark(subaction, pDocument,/* pDocView,*/ pBookmark, list)) return FALSE;
	}

	return TRUE;
}

void CFSCRT_STPDFActionHandler::DoAction_NoJs(const CPDF_Action& action, FSPDFDoc* pDocument/*, CReader_DocView* pDocView*/)
{
	ASSERT(pDocument != NULL);

	switch (action.GetType())
	{
	case CPDF_Action::GoTo:
		DoAction_GoTo(pDocument,/* pDocView,*/ action);
		break;
	case CPDF_Action::GoToR:
		DoAction_GoToR(pDocument, action);
		break;
	case CPDF_Action::GoToE:
		break;
	case CPDF_Action::Launch:
		DoAction_Launch(pDocument, action);
		break;
	case CPDF_Action::Thread:
		break;
	case CPDF_Action::URI:
		DoAction_URI(pDocument, action);
		break;
#if 0
	case CPDF_Action::Sound:
		if (m_pMediaActionHandler)
		{
			m_pMediaActionHandler->DoAction_Sound(action, pDocument/*, pDocView*/);
		}
		break;
	case CPDF_Action::Movie:
		if (m_pMediaActionHandler)
		{
			m_pMediaActionHandler->DoAction_Movie(action, pDocument/*, pDocView*/);
		}
		break;
#endif
	case CPDF_Action::Hide:
		if (m_pFormActionHandler)
		{
			m_pFormActionHandler->DoAction_Hide(action, pDocument);
		}
		break;
	case CPDF_Action::Named:
		DoAction_Named(pDocument, action);
		break;
	case CPDF_Action::SubmitForm:
		if (m_pFormActionHandler)
		{
			m_pFormActionHandler->DoAction_SubmitForm(action, pDocument/*, pDocView*/);
		}
		break;
	case CPDF_Action::ResetForm:
		if (m_pFormActionHandler)
		{
			m_pFormActionHandler->DoAction_ResetForm(action, pDocument);
		}
		break;
	case CPDF_Action::ImportData:
		if (m_pFormActionHandler)
		{
			m_pFormActionHandler->DoAction_ImportData(action, pDocument/*, pDocView*/);
		}
		break;
#if 0
	case CPDF_Action::JavaScript:
		ASSERT(FALSE);
		break;
	case CPDF_Action::SetOCGState:
		DoAction_SetOCGState(pDocument, /*pDocView,*/ action);
		break;
	case CPDF_Action::Rendition:
		if (m_pMediaActionHandler)
		{
			m_pMediaActionHandler->DoAction_Rendition(action, pDocument/*, pDocView*/);
		}
		break;
#endif
	case CPDF_Action::Trans:
		break;
	case CPDF_Action::GoTo3DView:
		break;
	}
}

FX_BOOL	CFSCRT_STPDFActionHandler::IsValidDocView(FSPDFDoc* pDocument/*, CReader_DocView* pDocView*/)
{
	ASSERT(pDocument != NULL);
	//ASSERT(pDocView != NULL);

	//return pDocument->IsValidDocView(pDocView);
	return TRUE;
}

void CFSCRT_STPDFActionHandler::DoAction_GoTo(FSPDFDoc* pDocument, /*CReader_DocView* pDocView,*/
	const CPDF_Action& action)
{
	ASSERT(pDocument != NULL);
	//	ASSERT(pDocView != NULL);
	ASSERT(action != NULL);

	CPDF_Document* pPDFDocument = ((FSPDFDocImp*)pDocument)->getPDFDocument();
	ASSERT(pPDFDocument != NULL);

	CPDF_Dest MyDest = action.GetDest(pPDFDocument);
	int nPageIndex = MyDest.GetPageIndex(pPDFDocument);
	int nFitType = MyDest.GetZoomMode();
	const CPDF_Array * pMyArray = (CPDF_Array*)MyDest.m_pObj;
	CFX_ArrayTemplate<float> temparray;

#ifndef _FS_PDFIUM_BUILD_
	if (pMyArray != NULL)
	{
		for (int i = 2; i < (int)pMyArray->GetCount(); i++)
			temparray.Add((float)pMyArray->GetFloat(i));
	}
#else
	if (pMyArray != NULL)
	{
		int length = (int)pMyArray->GetCount() - 2;
		float *fArray = (float*)malloc(length*sizeof(float));
		for (int i = 2; i < length+2; i++)
			fArray[i-2] = (float)pMyArray->GetFloat(i);
		if (m_pFormFiller->m_pJSActionHandle->m_pFFI->FFI_DoGoToAction)
			m_pFormFiller->m_pJSActionHandle->m_pFFI->FFI_DoGoToAction(m_pFormFiller->m_pJSActionHandle->m_pFFI, nPageIndex, nFitType, fArray, length);
	}
#endif

	//pDocView->GotoPageView(nPageIndex, nFitType, temparray);
	temparray.RemoveAll();
}

void CFSCRT_STPDFActionHandler::DoAction_GoToR(FSPDFDoc* pDocument, const CPDF_Action& action)
{
	// 	ASSERT(pDocument != NULL);
	// 	ASSERT(action != NULL);
	// 
	// 	CPDF_Document* pPDFDocument = pDocument->GetDocument();
	// 	ASSERT(pPDFDocument != NULL);
	// 
	// 	CPDF_Dest MyDest = action.GetDest(pPDFDocument);
	// 	int nPageIndex = MyDest.GetPageIndex(pPDFDocument);
	// 	int nFitType = MyDest.GetZoomMode();
	// 	FX_BOOL bNewWindow = action.GetNewWindow();
	// 	CFX_WideString csFilePath = action.GetFilePath();
	// 	
	// 	CFX_ArrayTemplate<float> temparray;
	// 	CFX_ByteString strName = MyDest.GetRemoteName();
	// 	if (strName.IsEmpty())
	// 	{
	// 		const CPDF_Array* pMyArray = (CPDF_Array*)MyDest.m_pObj;
	// 		if (pMyArray != NULL)
	// 		{
	// 			for(int i=2; i<(int)pMyArray->GetCount(); i++)
	// 				temparray.Add((float)pMyArray->GetFloat(i));
	// 		}
	// 	}		
	// 
	// 	ASSERT(m_pApp != NULL);
	// 	
	// 	FSPDFDoc* pNewDocument = NULL;
	// 	CFX_WideString csExt = csFilePath.Right(4);
	// 	CFX_WideString sFileFullName = csFilePath;
	// 	if (csExt.CompareNoCase(L".pdf") == 0)
	// 	{
	// 		CFX_WideString csDocName = pDocument->GetPath();
	// 		if (PathIsURL(sFileFullName))
	// 		{
	// 			m_pApp->OpenURL(sFileFullName);
	// 			return;
	// 		}
	// 		else if (PathIsRelative(sFileFullName))			
	// 		{		
	// 			
	// 			wchar_t drive[_MAX_DRIVE];
	// 			wchar_t dir[_MAX_DIR];
	// 			wchar_t fname[_MAX_FNAME];
	// 			wchar_t ext[_MAX_EXT];
	// 			
	// 			
	// 			_wsplitpath(csDocName,drive,dir,fname,ext);
	// 			CFX_WideString csDocPath = drive;
	// 			csDocPath += dir;
	// 
	// 			sFileFullName = csDocPath + csFilePath;
	// 			/************************************************************************/
	// 			/* 到这里sFileFullName可能是个URL(Lee.w.zi 2009-06-08 for bug #8179) */
	// 			/************************************************************************/
	// 			if (PathIsURL(sFileFullName))
	// 			{
	// 				m_pApp->OpenURL(sFileFullName);
	// 				return;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			csDocName.Replace(L"/", L"\\");
	// 			sFileFullName.Replace(L"/", L"\\");
	// 		}
	//		
	// 		int iDocCount = m_pApp->CountDocuments();
	// 		for (int i = 0; i < iDocCount; i ++)
	// 		{
	// 			CReader_Document *pDoc = m_pApp->GetDocument(i);
	// 			CFX_WideString csDocPath = pDoc->GetPath();
	// 			if (0 == csDocPath.CompareNoCase(sFileFullName))
	// 			{
	// 				pNewDocument = pDoc;
	// 				m_pApp->SetCurrentDocument(pDoc);
	// 				break;
	// 			}
	// 		}
	// 	}
	// 	if (!pNewDocument)
	// 		pNewDocument = m_pApp->OpenDocument(sFileFullName);
	// 	
	// 	if (!strName.IsEmpty() && pNewDocument)
	// 	{
	// 		CPDF_NameTree nametree(pNewDocument->GetDocument(), FX_BSTRC("Dests"));
	// 
	// 		CPDF_Dest pDest = nametree.LookupNamedDest(pNewDocument->GetDocument(),strName);
	// 		
	// 		const CPDF_Array * pMyArray = (CPDF_Array*)pDest.m_pObj;					
	// 		if (pMyArray)
	// 		{
	// 			for (int i=2;i<(int)pMyArray->GetCount();i++)
	// 				temparray.Add((float)pMyArray->GetFloat(i));
	// 		}
	// 		nPageIndex = pDest.GetPageIndex(pNewDocument->GetDocument());
	// 		nFitType = pDest.GetZoomMode();
	// 	}
	// 	
	// 	if (pNewDocument)
	// 	{
	// 		if (CReader_DocView* pDocView = pNewDocument->GetCurrentDocView())
	// 			pDocView->GotoPageView(nPageIndex, nFitType, temparray);
	// 	}
}

void CFSCRT_STPDFActionHandler::DoAction_Launch(FSPDFDoc* pDocument, const CPDF_Action& action)
{
	// 	ASSERT(pDocument != NULL);
	// 	ASSERT(action != NULL);
	// 	ASSERT(m_pApp != NULL);
	// 
	// 	CFX_WideString csFilePath = action.GetFilePath();
	// 	FX_BOOL bNewWindow = action.GetNewWindow();
	// 	CPDF_LWinParam WinParam = action.GetWinParam();
	// 	CFX_ByteString csFile = WinParam.GetFileName();
	// 	if (!csFile.IsEmpty()) csFilePath = CFX_WideString::FromLocal(csFile);
	// 	CFX_ByteString csParam = WinParam.GetParameter();
	// 	CFX_ByteString csDefDir = WinParam.GetDefaultDirectory();
	// 	CFX_ByteString sOpr = WinParam.GetOperation();
	// 	FX_BOOL bOpenOrPrint = TRUE;
	// 	if (sOpr.EqualNoCase("Print")) bOpenOrPrint = FALSE;
	// 
	// 	FX_BOOL bSuccess = FALSE;
	// 	if (!csFilePath.IsEmpty())
	// 	{
	// 		CFX_WideString csDestFile;
	// 		CFX_WideString wcsFilePath = csFilePath;
	// 		FX_BOOL bRet = FALSE;
	// 		if (PathIsURL(wcsFilePath))
	// 		{
	// 			m_pApp->OpenURL(wcsFilePath);
	// 			return;
	// 		}
	// 		else if (PathIsRelative(wcsFilePath))					
	// 		{		
	// 			LPWSTR pSrcName = NULL;
	// 			CFX_WideString csDocName = pDocument->GetPath();
	// 			wchar_t drive[_MAX_DRIVE];
	// 			wchar_t dir[_MAX_DIR];
	// 			wchar_t fname[_MAX_FNAME];
	// 			wchar_t ext[_MAX_EXT];
	// 
	// 			_wsplitpath(csDocName,drive,dir,fname,ext);
	// 
	// 			CFX_WideString  csDocPath;	
	// 			csDocPath = drive;
	// 			csDocPath += dir;					
	// 			csDestFile = csDocPath + wcsFilePath;
	// 			
	// 			bRet = TRUE;
	// 		
	// 		}
	// 		else
	// 		{
	// 			csDestFile = wcsFilePath;
	// 			bRet = TRUE;
	// 		}
	//
	// 		if (bRet)
	// 		{
	// 			CFX_WideString csExt = csFilePath.Right(4);
	// 			if (csExt.CompareNoCase(L".pdf") == 0)
	// 			{
	// 				int iDocCount = m_pApp->CountDocuments();
	// 				for (int i = 0; i < iDocCount; i ++)
	// 				{
	// 					CReader_Document *pDoc = m_pApp->GetDocument(i);
	// 					if (pDoc == NULL) continue;
	// 					CFX_WideString csDocName = pDoc->GetPath();
	// 					if (0 == csDocName.CompareNoCase(csDestFile) )
	// 					{
	// 						m_pApp->SetCurrentDocument(pDoc);
	// 						return;
	// 					}
	// 				}
	//             
	// 				bSuccess = m_pApp->OpenDocument(csDestFile) != NULL;
	// 			}
	// 			else
	// 			{
	// 				bSuccess = m_pApp->Launch(csDestFile, CFX_WideString::FromLocal(csParam), CFX_WideString::FromLocal(csDefDir), bOpenOrPrint);
	// 			}
	// 
	// 			if (!bSuccess)
	// 			{
	// 				CFileFind finder;
	// 				CFX_WideString csClipFileName = csDestFile;
	// 				if(csDestFile.GetLength() >= _MAX_PATH)
	// 					csClipFileName = csDestFile.Left(_MAX_PATH);
	// 				FX_BOOL bFind = finder.FindFile(csClipFileName);
	// 				finder.Close();
	// 				if(!bFind)//[file is not exist]
	// 				{
	// 					/*
	// 					CFX_WideString csError, csTitle;
	// 					csError = CReader_Language::LoadString(IDS_LAUNCH_ERROR);
	// 					csTitle = pApp->GetAppTitle();
	// 					::MessageBox(hWnd, csError, csTitle, MB_OK | MB_ICONERROR);
	// 					*/
	// 				}
	// 			}
	// 		}
	//	}
}

void CFSCRT_STPDFActionHandler::DoAction_URI(FSPDFDoc* pDocument, const CPDF_Action& action)
{
	  	ASSERT(pDocument != NULL);
	 	ASSERT(action != NULL);
		FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
		FSFormFillerImp* pFormFiller = ((FSFormImp*)((FSPDFDocImp*)pDocument)->getForm())->GetFormFiller();
		//CFX_ByteString sURI = action.GetURI(pDocument->m_pSTDoc);
		//const FSCRT_BSTR bstrURL = {(FX_LPSTR)(FX_LPCSTR)sURI, (FS_DWORD)sURI.GetLength()};

		//pFormFiller->m_pJSActionHandle->LaunchURL(&bstrURL);
}

void CFSCRT_STPDFActionHandler::DoAction_Named(FSPDFDoc* pDocument, const CPDF_Action& action)
{
	ASSERT(pDocument != NULL);
	ASSERT(action != NULL);

	CFX_ByteString csName = action.GetNamedAction();
	//pDocument->GetEnv()->FFI_ExecuteNamedAction(csName);
	//const FSCRT_BSTR bsName = {(FS_LPSTR)(FX_LPCSTR)csName, (FX_DWORD)csName.GetLength()};

	//m_pFormFiller->m_pJSActionHandle->ExecuteNamedAction((FSCRT_DOCUMENT)pDocument, &bsName);
}

//只对当前DocView有效
void CFSCRT_STPDFActionHandler::DoAction_SetOCGState(FSPDFDoc* pDocument,/* CReader_DocView* pDocView,*/ const CPDF_Action& action)
{
	// 	CPDF_OCProperties *pOCP = pDocument->GetOCProperties();
	// 	if (pOCP == NULL) return;
	// 
	// 	CPDF_OCContext *pOCC = pDocView->GetOCContext();
	// 	CFX_MapPtrToPtr ocgStateMap;
	// 	FX_INT32 iCount = action.CountOCGStates();
	// 	for (FX_INT32 i = 0; i < iCount; i++)
	// 	{
	// 		CPDF_Action::OCGState eState = CPDF_Action::ON;
	// 		CFX_PtrArray ocgs;
	// 		FX_BOOL bRet = action.GetOCGStates(i, eState, ocgs);
	// 		FX_INT32 iNum = ocgs.GetSize();
	// 		for (FX_INT32 j = 0; j < iNum; j++)
	// 		{
	// 			if (eState == CPDF_Action::ON || eState == CPDF_Action::OFF)
	// 				ocgStateMap.SetAt(ocgs[j], (FX_LPVOID)(eState == CPDF_Action::ON));
	// 			else
	// 			{
	// 				FX_BOOL bState = FALSE;
	// 				if (!ocgStateMap.Lookup(ocgs[j], (FX_LPVOID&)bState))
	// 					bState = pOCC->CheckOCGVisible((const CPDF_Dictionary*)ocgs[j]);
	// 				ocgStateMap.SetAt(ocgs[j], (FX_LPVOID)!bState);
	// 			}
	// 		}
	// 	}
	// 	CFX_MapPtrToPtr ocgPageMap;
	// 	iCount = pDocView->CountVisiblePages();
	// 	for (i = 0; i < iCount; i++)
	// 		ocgPageMap.SetAt(pDocView->GetVisiblePage(i), NULL);
	// 	FX_POSITION pos1 = ocgStateMap.GetStartPosition();
	// 	while (pos1 != NULL)
	// 	{
	// 		CPDF_Dictionary *pOCGDict = NULL;
	// 		FX_BOOL bState = FALSE;
	// 		ocgStateMap.GetNextAssoc(pos1, (FX_LPVOID&)pOCGDict, (FX_LPVOID&)bState);
	// 		FX_BOOL bOldState = pOCC->CheckOCGVisible(pOCGDict);
	// 		if (bOldState != bState)
	// 		{
	// 			pOCC->SetOCGState(pOCGDict, bState, TRUE);
	// 			FX_POSITION pos2 = ocgPageMap.GetStartPosition();
	// 			while (pos2 != NULL)
	// 			{
	// 				CReader_PageView *pPageView = NULL;
	// 				FX_DWORD dwCount = 0;
	// 				ocgPageMap.GetNextAssoc(pos2, (FX_LPVOID&)pPageView, (FX_LPVOID&)dwCount);
	// 				if (pOCP->IsOCGInPage(pPageView->GetReaderPage()->GetPage()->m_pFormDict, pOCGDict))
	// 				{
	// 					dwCount ++;
	// 					ocgPageMap.SetAt(pPageView, (FX_LPVOID)dwCount);
	// 				}
	// 			}
	// 		}
	// 	}
	// 	pos1 = ocgPageMap.GetStartPosition();
	// 	while (pos1 != NULL)
	// 	{
	// 		CReader_PageView *pPageView = NULL;
	// 		FX_DWORD dwCount = 0;
	// 		ocgPageMap.GetNextAssoc(pos1, (FX_LPVOID&)pPageView, (FX_LPVOID&)dwCount);
	// 		if (dwCount > 0)
	// 		{
	// 			CRect pageRect = pPageView->GetPageRect();
	// 			
	// 			CReader_DocView* pDocView = pPageView->GetDocView();
	// 			if (pDocView)
	// 				pDocView->SetRenderDataChange();
	// 			::InvalidateRect(pPageView->GetPageViewWnd(), pageRect, TRUE);
	// 		}
	// 	}
}

void CFSCRT_STPDFActionHandler::RunFieldJavaScript(FSPDFDoc* pDocument, CPDF_FormField* pFormField, CPDF_AAction::AActionType type,
	FSPDF_FieldActionParam& data, const CFX_WideString& script)
{

	ASSERT(type != CPDF_AAction::Calculate);
	ASSERT(type != CPDF_AAction::Format);

	ASSERT(pDocument != NULL);

	IJS_Runtime* pRuntime = ((FSPDFDocImp*)pDocument)->GetJsRuntime();
	if(!pRuntime) return ;

	pRuntime->SetReaderDocument(pDocument);

	IJS_Context* pContext = pRuntime->NewContext();
	ASSERT(pContext != NULL);

	switch (type)
	{
	case CPDF_AAction::CursorEnter:
		pContext->OnField_MouseEnter(data.bModifier, data.bShift, pFormField);
		break;
	case CPDF_AAction::CursorExit:
		pContext->OnField_MouseExit(data.bModifier, data.bShift, pFormField);
		break;
	case CPDF_AAction::ButtonDown:
		pContext->OnField_MouseDown(data.bModifier, data.bShift, pFormField);
		break;
	case CPDF_AAction::ButtonUp:
		pContext->OnField_MouseUp(data.bModifier, data.bShift, pFormField);
		break;
	case CPDF_AAction::GetFocus:
		pContext->OnField_Focus(data.bModifier, data.bShift, pFormField, data.sValue);
		break;
	case CPDF_AAction::LoseFocus:
		pContext->OnField_Blur(data.bModifier, data.bShift, pFormField, data.sValue);
		break;
	case CPDF_AAction::KeyStroke:
		pContext->OnField_Keystroke(data.nCommitKey, data.sChange, data.sChangeEx, data.bKeyDown, 
			data.bModifier, data.nSelEnd, data.nSelStart, data.bShift, pFormField, data.sValue,
			data.bWillCommit, data.bFieldFull, data.bRC);
		break;
	case CPDF_AAction::Validate:
		pContext->OnField_Validate(data.sChange, data.sChangeEx, data.bKeyDown, data.bModifier,
			data.bShift, pFormField, data.sValue, data.bRC);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	CFX_WideString csInfo;
	FX_BOOL bRet = pContext->RunScript(script, csInfo);
	if (!bRet)
	{
		//CBCL_FormNotify::MsgBoxJSError(pPageView->GetPageViewWnd(), csInfo);
	}

	pRuntime->ReleaseContext(pContext);

}

void CFSCRT_STPDFActionHandler::RunDocumentOpenJavaScript(FSPDFDoc* pDocument, const CFX_WideString& sScriptName, const CFX_WideString& script)
{

	ASSERT(pDocument != NULL);

	IJS_Runtime* pRuntime = ((FSPDFDocImp*)pDocument)->GetJsRuntime();
	if(!pRuntime) return ;

	pRuntime->SetReaderDocument(pDocument);

	IJS_Context* pContext = pRuntime->NewContext();
	ASSERT(pContext != NULL);

	pContext->OnDoc_Open(pDocument, sScriptName);

	CFX_WideString csInfo;
	FX_BOOL bRet = pContext->RunScript(script, csInfo);
	if (!bRet)
	{
		//CBCL_FormNotify::MsgBoxJSError(pPageView->GetPageViewWnd(), csInfo);
	}

	pRuntime->ReleaseContext(pContext);

}

void CFSCRT_STPDFActionHandler::RunDocumentPageJavaScript(FSPDFDoc* pDocument, CPDF_AAction::AActionType type, const CFX_WideString& script)
{

	ASSERT(pDocument != NULL);

	IJS_Runtime* pRuntime = ((FSPDFDocImp*)pDocument)->GetJsRuntime();
	if(!pRuntime) return;

	pRuntime->SetReaderDocument(pDocument);

	IJS_Context* pContext = pRuntime->NewContext();
	ASSERT(pContext != NULL);

	switch (type)
	{	
	case CPDF_AAction::OpenPage:
		pContext->OnPage_Open(pDocument);
		break;
	case CPDF_AAction::ClosePage:
		pContext->OnPage_Close(pDocument);
		break;
	case CPDF_AAction::CloseDocument:
		pContext->OnDoc_WillClose(pDocument);
		break;
	case CPDF_AAction::SaveDocument:
		pContext->OnDoc_WillSave(pDocument);
		break;
	case CPDF_AAction::DocumentSaved:
		pContext->OnDoc_DidSave(pDocument);
		break;
	case CPDF_AAction::PrintDocument:
		pContext->OnDoc_WillPrint(pDocument);
		break;
	case CPDF_AAction::DocumentPrinted:
		pContext->OnDoc_DidPrint(pDocument);
		break;
	case CPDF_AAction::PageVisible:
		pContext->OnPage_InView(pDocument);
		break;
	case CPDF_AAction::PageInvisible:
		pContext->OnPage_OutView(pDocument);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	CFX_WideString csInfo;
	FX_BOOL bRet = pContext->RunScript(script, csInfo);
	if (!bRet)
	{
		//CBCL_FormNotify::MsgBoxJSError(pPageView->GetPageViewWnd(), csInfo);
	}

	pRuntime->ReleaseContext(pContext);

}


#endif


