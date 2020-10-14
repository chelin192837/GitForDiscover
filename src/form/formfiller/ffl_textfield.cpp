#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"
#include "ffl_textfield.h"

#define FIELDFLAG_MULTILINE					(1<<12)
#define FIELDFLAG_PASSWORD					(1<<13)
#define FIELDFLAG_FILESELECT				(1<<20)
#define FIELDFLAG_DONOTSCROLL				(1<<23)
#define FIELDFLAG_COMB						(1<<24)
#define FIELDFLAG_RICHTEXT					(1<<25)

#define BF_ALIGN_LEFT			0
#define BF_ALIGN_MIDDLE			1
#define BF_ALIGN_RIGHT			2


CFFL_TextFieldCtrl::CFFL_TextFieldCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_Widget(pControl, pWidgetHandler)
{

}

CFFL_TextFieldCtrl::~CFFL_TextFieldCtrl()
{

}

IFSPDF_Widget* CFFL_TextFieldCtrl::InitWidget()
{
	IFSPDF_Edit* widget = IFSPDF_Edit::Create();
	if(!widget) return NULL;

	CFSPDF_WidgetProperties properties;
	FillCreateParam(properties);
	widget->Initialize(m_pControl, &properties);
	widget->Update();
	widget->SetFillerNotify((IFSPDF_Widget_Notify*)this);

	FXSYS_assert(m_pControl);
	CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	FXSYS_assert(pField);

	FX_INT32 nMaxLen = pField->GetMaxLen();
	CFX_WideString swValue = pField->GetValue();
	if (nMaxLen > 0)
	{
		if (pField->GetFieldFlags() & FPDFW_STYLEEXT_EDT_CombText)
		{
			/* TODO: implement this*/
			widget->SetCharArray(nMaxLen);
//			pWnd->SetAlignFormatV(PEAV_CENTER);
			
		}
		else
		{
			widget->SetLimit(nMaxLen);
		}
	}

	widget->SetText(swValue);

	return widget;
}

void CFFL_TextFieldCtrl::FillCreateParam(CFSPDF_WidgetProperties& prop)
{
	CFFL_Widget::FillCreateParam(prop);
	//Need to clear the background color to opaque.
	if(prop.m_sBackgroundColor == 0)
		prop.m_sBackgroundColor = 0xFFFFFFFF;

	CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	FXSYS_assert(pField);
	FX_DWORD nFlags = pField->GetFieldFlags();
	if (nFlags & FIELDFLAG_PASSWORD)
	{
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_Password;
	}
	//no spell check
	if (nFlags & FIELDFLAG_MULTILINE)
	{		
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_MultiLine | FPDFW_STYLEEXT_EDT_WantReturn | FPDFW_STYLEEXT_EDT_VNear;
	
		if (!(nFlags & FIELDFLAG_DONOTSCROLL))
		{
			prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_AutoHScroll | FPDFW_STYLEEXT_EDT_AutoVScroll;
		}
	}
	else
	{
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_VCenter;
	
		if (!(nFlags & FIELDFLAG_DONOTSCROLL))
		{
			prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_AutoHScroll;
		}
	}

	if (nFlags & FIELDFLAG_COMB)
	{		
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_CombText;
	}
	
	if (nFlags & FIELDFLAG_RICHTEXT)
	{
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_RichEdit;
	}
	//No NoUndoRedo means support it.
	switch (((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetControlAlignment())
	{
	default:
	case BF_ALIGN_LEFT:
	 	prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_HNear;
	 	break;
	case BF_ALIGN_MIDDLE:
	 	prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_HCenter;
	 	break;
	case BF_ALIGN_RIGHT:
	 	prop.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_HFar;
	 	break;
	}

	/* TODO: see if this is necessary
	//	cp.pFontMap = m_pFontMap;
	//	cp.pFocusHandler = this;
	*/
}

FX_BOOL CFFL_TextFieldCtrl::OnChar(FS_DWORD nChar, FS_DWORD nFlags)
{
	FXSYS_assert(m_pControl);
	CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	switch (nChar)
	{
	case e_vkeyReturn:
		if (!(pField->GetFieldFlags() & FIELDFLAG_MULTILINE))
		{
			FSPDFPage* pPageView = GetCurrentPageView();
			FXSYS_assert(pPageView);

 			if (!m_bActivated)
 			{
				Activate();
 				if (IFSPDF_Widget* widget = GetWidget(pPageView, TRUE))
 					widget->SetFocus();
 			}
			else
			{
				DeActivate();
				if (CommitData(pPageView, nFlags))
				{
					FSFormFillerImp* pFormFiller = NULL;
					m_pWidgetHandler->GetFormFiller(&pFormFiller);
					if(pFormFiller) pFormFiller->SetFocusAnnot(NULL);

					ReleaseWidget(pPageView);
					return TRUE;
				}
				return FALSE;
			}
		}
		break;
	case e_vkeyEscape:
		{
			FSPDFPage* pPageView = GetCurrentPageView();
			FXSYS_assert(pPageView != NULL);
			ExitFiller(pPageView, TRUE);
			return TRUE;
		}
	}

	return CFFL_Widget::OnChar(nChar, nFlags);
}

CFX_WideString CFFL_TextFieldCtrl::GetRichTextContent(CFX_WideString wsRichText)
{
	CFX_WideString wsContent = wsRichText;
	CFX_ByteString sValue = CFX_ByteString::FromUnicode(wsRichText);

	if (CXML_Element * pXML = CXML_Element::Parse((FX_LPCSTR)sValue,sValue.GetLength()))
	{
		FX_INT32 nCount = pXML->CountChildren();
		FX_BOOL bFirst = TRUE;
		wsContent.Empty();
		for (FX_INT32 i=0; i<nCount; i++)
		{
			if (CXML_Element * pSubElement = pXML->GetElement(i))
			{
				CFX_ByteString tag=pSubElement->GetTagName();
				if (tag.EqualNoCase("p"))
				{
					int nChild = pSubElement->CountChildren();
					CFX_WideString swSection;
					for(FX_INT32 j=0; j<nChild; j++)
					{
						swSection += pSubElement->GetContent(j);
					}

					if (bFirst)bFirst = FALSE;
					else
						wsContent += e_vkeyReturn;
					wsContent += swSection;
				}
			}
		}
		delete pXML;
	}
	return wsContent;
}

FX_BOOL CFFL_TextFieldCtrl::IsDataChanged( FSPDFPage* pPageView )
{
	FXSYS_assert(m_pControl);

	CFX_WideString wsValue;

	CPDF_FormField *pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	FXSYS_assert(pField);

	if(pField->GetFieldFlags() & FIELDFLAG_RICHTEXT)
		wsValue = GetRichTextContent(pField->GetValue());
	else
		wsValue = pField->GetValue();

	if (IFSPDF_Edit * widget = GetWidget(pPageView, FALSE))
	{
		CFX_WideString wsWidgetValue;
		widget->GetText(wsWidgetValue);
		return wsWidgetValue != wsValue;
	}

	return FALSE;
}


FX_BOOL	CFFL_TextFieldCtrl::OnSetFocus(FS_DWORD nFlag)
{
	//	IFX_InputMethod* pInputMethod = m_pApp->GetCurrentInputMethod();
	//	ASSERT(pInputMethod != NULL);

	//int pageIndex = -1;//((CFSCRT_LTPDFPage*)m_pControl->GetPage())->m_nPageIndex;
	//FSPDFDoc* pDoc = NULL;//(CFSCRT_LTPDFDocument*)m_pControl->GetPage()->GetDocument();
	FSPDFPage* pPageView = m_pControl->getPage();//pDoc->getPage(pageIndex);
	//pDoc->GetPageView(pageIndex, &pPageView);

	if (IFSPDF_Edit * pEdit = GetWidget(pPageView, TRUE))
	{
		CFX_WideString wsText;
		pEdit->GetText(wsText);
		CFX_ByteString bsUTFText = wsText.UTF8Encode();
		//unsigned short* pBuffer = (unsigned short*)(FX_LPCSTR)bsUTFText;
		//m_pApp->FFI_OnSetFieldInputFocus(m_pWidget->GetFormField(), pBuffer, nCharacters);

		FSFormFillerImp* pFormFiller = NULL;
		m_pWidgetHandler->GetFormFiller(&pFormFiller);

		//const FSCRT_BSTR value = {(FX_LPSTR)(FX_LPCSTR)bsUTFText, (FS_DWORD)bsUTFText.GetLength()};
		//pFormFiller->m_formInterWindowless.GotFocusOnControl((FSPDF_FORMCONTROL)m_pControl, &value);
		FSFormFillerAssist* pFormFillAssist = pFormFiller->GetFormFillerAssist();
		if(pFormFillAssist)
			pFormFillAssist->focusGotOnControl(m_pControl, (FX_LPCSTR)bsUTFText);

		//pEdit->SetEditNotify(this);
		//pUndo->BeginEdit(pDocument);
	}
	return CFFL_Widget::OnSetFocus(nFlag);
}

FX_BOOL	CFFL_TextFieldCtrl::OnKillFocus(FS_DWORD nFlag)
{
	//	IFX_InputMethod* pInputMethod = m_pApp->GetCurrentInputMethod();
	//	ASSERT(pInputMethod != NULL);

	//int pageIndex = -1;// ((CFSCRT_LTPDFPage*)m_pControl->GetPage())->m_nPageIndex;
	//FSPDFDoc* pDoc = NULL; // (CFSCRT_LTPDFDocument*)m_pControl->GetPage()->GetDocument();
	FSPDFPage* pPageView = m_pControl->getPage();
	//pDoc->GetPageView(pageIndex, &pPageView);

	if (IFSPDF_Edit * pEdit = GetWidget(pPageView, FALSE))
	{
		CFX_WideString wsText;
		pEdit->GetText(wsText);
		CFX_ByteString bsUTFText = wsText.UTF8Encode();
		//unsigned short* pBuffer = (unsigned short*)(FX_LPCSTR)bsUTFText;
		//m_pApp->FFI_OnSetFieldInputFocus(m_pWidget->GetFormField(), pBuffer, nCharacters);

		FSFormFillerImp* pFormFiller = NULL;
		m_pWidgetHandler->GetFormFiller(&pFormFiller);
        FSFormFillerAssist* pFormFillAssist = pFormFiller->GetFormFillerAssist();
        if(pFormFillAssist)
            pFormFillAssist->focusLostFromControl(m_pControl, (FX_LPCSTR)bsUTFText);

		//const FSCRT_BSTR value = {(FX_LPSTR)(FX_LPCSTR)bsUTFText, (FS_DWORD)bsUTFText.GetLength()};
		//pFormFiller->m_formInterWindowless.LostFocusOnControl((FSPDF_FORMCONTROL)m_pControl, &value);
		
		//pEdit->SetEditNotify(this);
		//pUndo->BeginEdit(pDocument);
	}
	return CFFL_Widget::OnKillFocus(nFlag);
}

void CFFL_TextFieldCtrl::SaveData(FSPDFPage* pPageView)
{
	FXSYS_assert(m_pControl);
	if (IFSPDF_Edit* widget = GetWidget(pPageView, FALSE))
	{
		CPDF_FormField *pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		/* Without Undo
		CFX_WideString sOldValue = pField->GetValue(); */
		CFX_WideString sNewValue; widget->GetText(sNewValue);

		pField->SetValue(sNewValue, FALSE);
		
        //Don't need do this, update Field will do.
        //m_pControl->resetAppearanceStream();

		//IFSPDF_Widget::ResetAppearance(m_pControl->GetCPDFFormControl(), sNewValue);
		
		UpdateField(pField, sNewValue);
		SetChangeMark();
	}
}

void CFFL_TextFieldCtrl::GetActionData( FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa )
{
	switch (type)
	{
	case CPDF_AAction::KeyStroke:
		if (IFSPDF_Edit* pWnd = GetWidget(pPageView, FALSE))
		{
			//fa.bFieldFull = pWnd->IsTextFull();	
			 pWnd->GetText(fa.sValue);

			if (fa.bFieldFull)
			{
				fa.sChange = L"";
				fa.sChangeEx = L"";
			}
		}
		break;
	case CPDF_AAction::Validate:
		if (IFSPDF_Edit* pWnd = GetWidget(pPageView, FALSE))
		{
			pWnd->GetText(fa.sValue);
		}
		break;
	case CPDF_AAction::LoseFocus:
	case CPDF_AAction::GetFocus:
		FXSYS_assert(m_pControl != NULL);
		fa.sValue = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField()->GetValue();
		break;
	}
}

void CFFL_TextFieldCtrl::SetActionData( FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa )
{
	switch (type)
	{
	case CPDF_AAction::KeyStroke:
		if (IFSPDF_Edit * pEdit = GetWidget(pPageView, FALSE))
		{
			pEdit->SetFocus();
			pEdit->AddSelRange(fa.nSelStart, fa.nSelEnd - fa.nSelStart); //SetSel(fa.nSelStart, fa.nSelEnd) ==AddSelRange(fa.nSelStart, fa.nSelEnd - fa.nSelStart) ??
			pEdit->AddSelRange(fa.nSelStart, fa.nSelEnd - fa.nSelStart);
			pEdit->ReplaceSelections(fa.sChange);
		}
		break;
	}
}

FX_BOOL CFFL_TextFieldCtrl::IsActionDataChanged( CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& faOld, const FSPDF_FieldActionParam& faNew )
{
	switch (type)
	{
	case CPDF_AAction::KeyStroke:
		return !faOld.bFieldFull && faOld.nSelEnd != faNew.nSelEnd || faOld.nSelStart != faNew.nSelStart ||
			faOld.sChange != faNew.sChange;
	}

	return FALSE;
}

void CFFL_TextFieldCtrl::SaveState( FSPDFPage* pPageView )
{
	FXSYS_assert(pPageView != NULL);

	if (IFSPDF_Edit* pWnd = GetWidget(pPageView, FALSE))
	{
		pWnd->GetSelRange(m_State.nStart, m_State.nEnd); ///  GetSel ===== GetSelRange ?????
		 pWnd->GetText(m_State.sValue);
	}
}

void CFFL_TextFieldCtrl::RestoreState( FSPDFPage* pPageView )
{
	FXSYS_assert(pPageView != NULL);

	if (IFSPDF_Edit* pWnd = GetWidget(pPageView, TRUE))
	{
		pWnd->SetText(m_State.sValue);
		pWnd->SetSelRange(m_State.nStart, m_State.nEnd);
	}
}

IFSPDF_Widget* CFFL_TextFieldCtrl::ResetPDFWindow( FSPDFPage* pPageView, FX_BOOL bRestoreValue ) //CPWL_Widget ----> IFSPDF_Widget
{
	if (bRestoreValue)
		SaveState(pPageView);
	//DestroyPDFWindow(pPageView);
	ReleaseWidget(pPageView);

	IFSPDF_Widget* pRet = NULL;

	if (bRestoreValue)
	{
		RestoreState(pPageView);
		pRet = this->GetWidget(pPageView, FALSE);
	}
	else
		pRet = this->GetWidget(pPageView, TRUE);

	UpdateField(NULL, NULL, FALSE, TRUE);
	//m_pWidget->UpdateField();

	return pRet;
}

// TODO: JJJ's code
//FX_BOOL CFFL_TextField::OnSetText(FSFormControl* pControl, const CFX_WideString &wText, int nFlag)
//{
//	if(pControl)
//	{
//		FSPDFPage* pPageView;  //????????????????????? //CPDFSDK_PageView* pPageView = pAnnot->GetPageView();
//		if (IFSPDF_Edit* pWnd = GetWidget(pPageView, FALSE))
//		{
//			pWnd->SetText(wText);
//			return TRUE;
//		}
//	}
//	return FALSE;
//}

/* SDK code:

//CFFL_EditUndoItem::CFFL_EditUndoItem(CPWL_Edit* pEdit) : m_pEdit(pEdit)
//{
//}
//
//CFFL_EditUndoItem::~CFFL_EditUndoItem()
//{
//}
//
//void CFFL_EditUndoItem::Undo()
//{
////	m_pEdit->Undo();
//}
//
//void CFFL_EditUndoItem::Redo()
//{
////	m_pEdit->Redo();
//}
//
//CFX_WideString CFFL_EditUndoItem::GetDescr()
//{
//	return L"Input";
//}
//
//void CFFL_EditUndoItem::Release()
//{
//	delete this;
//}
//
///* ------------------------------- CFFL_TextField ------------------------------- */
//
//
//
//FX_BOOL	CFFL_TextField::OnSetText(CPDFSDK_Annot* pAnnot, FSCRT_LPCWSTR wText, int nTextLen, int nFlag)
//{
//	if(pAnnot)
//	{
//		CPDFSDK_PageView* pPageView = pAnnot->GetPageView();
//		if (CPWL_Edit* pWnd = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//		{
//			CFX_WideString wsValue = CFX_WideString::FromUTF16LE(wText, nTextLen);
//			pWnd->SetText(wsValue);
//			return TRUE;
//		}
//	}
//	return FALSE;
//}
//
//

//
//void CFFL_TextField::GetActionData(CPDFSDK_PageView* pPageView, CPDF_AAction::AActionType type,
//									PDFSDK_FieldAction& fa)
//{
//	switch (type)
//	{
//	case CPDF_AAction::KeyStroke:
//		if (CPWL_Edit* pWnd = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//		{
//			fa.bFieldFull = pWnd->IsTextFull();	
//			/*
//			int nSelStart = 0;
//			int nSelEnd = 0;
//			pWnd->GetSel(nSelStart, nSelEnd);
//			fa.nSelEnd = nSelEnd;
//			fa.nSelStart = nSelStart;
//			*/
//			fa.sValue = pWnd->GetText();
//			
//			if (fa.bFieldFull)
//			{
//				fa.sChange = L"";
//				fa.sChangeEx = L"";
//			}
//		}
//		break;
//	case CPDF_AAction::Validate:
//		if (CPWL_Edit* pWnd = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//		{
//			fa.sValue = pWnd->GetText();
//		}
//		break;
//	case CPDF_AAction::LoseFocus:
//	case CPDF_AAction::GetFocus:
//		ASSERT(m_pWidget != NULL);
//		fa.sValue = m_pWidget->GetValue();
//		break;
//	}
//}
//
//void CFFL_TextField::SetActionData(CPDFSDK_PageView* pPageView, CPDF_AAction::AActionType type, 
// 									const PDFSDK_FieldAction& fa)
//{
//	switch (type)
//	{
//	case CPDF_AAction::KeyStroke:
//		if (CPWL_Edit * pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//		{
//			pEdit->SetFocus();
//			pEdit->SetSel(fa.nSelStart, fa.nSelEnd);
//			pEdit->ReplaceSel(fa.sChange);
//		}
//		break;
//	}
//}
//
//
//FX_BOOL	CFFL_TextField::IsActionDataChanged(CPDF_AAction::AActionType type, const PDFSDK_FieldAction& faOld, 
//									const PDFSDK_FieldAction& faNew)
//{
//	switch (type)
//	{
//	case CPDF_AAction::KeyStroke:
//		return !faOld.bFieldFull && faOld.nSelEnd != faNew.nSelEnd || faOld.nSelStart != faNew.nSelStart ||
//			faOld.sChange != faNew.sChange;
//	}
//
//	return FALSE;
//}
//
//void CFFL_TextField::SaveState(CPDFSDK_PageView* pPageView)
//{
//	ASSERT(pPageView != NULL);
//
//	if (CPWL_Edit* pWnd = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//	{
//		pWnd->GetSel(m_State.nStart, m_State.nEnd);
//		m_State.sValue = pWnd->GetText();
//	}
//}
//
//void CFFL_TextField::RestoreState(CPDFSDK_PageView* pPageView)
//{
//	ASSERT(pPageView != NULL);
//
//	if (CPWL_Edit* pWnd = (CPWL_Edit*)GetPDFWindow(pPageView, TRUE))
//	{
//		pWnd->SetText(m_State.sValue);
//		pWnd->SetSel(m_State.nStart, m_State.nEnd);
//	}
//}
//
//CPWL_Wnd* CFFL_TextField::ResetPDFWindow(CPDFSDK_PageView* pPageView, FX_BOOL bRestoreValue)
//{
//	if (bRestoreValue)
//		SaveState(pPageView);
//
//	DestroyPDFWindow(pPageView);
//
//	CPWL_Wnd* pRet = NULL;
//
//	if (bRestoreValue)
//	{
//		RestoreState(pPageView);
//		pRet = this->GetPDFWindow(pPageView, FALSE);
//	}
//	else
//		pRet = this->GetPDFWindow(pPageView, TRUE);
//
//	m_pWidget->UpdateField();
//	
//	return pRet;
//}
//
//
//FX_BOOL	CFFL_TextField::CanCopy(CPDFSDK_Document* pDocument)
//{
//// 	ASSERT(pDocument != NULL);
//// 
//// 	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//// 	{
//// 		if (CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView())
//// 		{
//// 			if (CPWL_Edit* pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//// 			{
//// 				return pEdit->CanCopy();
//// 			}
//// 		}
//// 	}
//
//	return FALSE;
//}
//
//FX_BOOL CFFL_TextField::CanCut(CPDFSDK_Document* pDocument)
//{
//// 	ASSERT(pDocument != NULL);
//// 
//// 	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//// 	{
//// 		if (CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView())
//// 		{
//// 			if (CPWL_Edit* pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//// 			{
//// 				return pEdit->CanCut();
//// 			}
//// 		}
//// 	}
//
//	return FALSE;
//}
//
//FX_BOOL	CFFL_TextField::CanPaste(CPDFSDK_Document* pDocument)
//{
//// 	ASSERT(pDocument != NULL);
//// 
//// 	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//// 	{
//// 		if (CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView())
//// 		{
//// 			if (CPWL_Edit* pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//// 			{
//// 				return pEdit->CanPaste();
//// 			}
//// 		}
//// 	}
//
//	return FALSE;
//}
//
//void CFFL_TextField::DoCopy(CPDFSDK_Document* pDocument)
//{
//// 	ASSERT(pDocument != NULL);
//// 
//// 	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//// 	{
//// 		if (CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView())
//// 		{
//// 			if (CPWL_Edit* pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//// 			{
//// 				pEdit->CopyText();
//// 			}
//// 		}
//// 	}
//}
//
//void CFFL_TextField::DoCut(CPDFSDK_Document* pDocument)
//{
//// 	ASSERT(pDocument != NULL);
//// 
//// 	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//// 	{
//// 		if (CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView())
//// 		{
//// 			if (CPWL_Edit* pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//// 			{
//// 				pEdit->CutText();
//// 			}
//// 		}
//// 	}
//}
//
//void CFFL_TextField::DoPaste(CPDFSDK_Document* pDocument)
//{
//// 	ASSERT(pDocument != NULL);
//// 
//// 	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//// 	{
//// 		if (CPDFSDK_PageView* pPageView = pDocView->GetCurrentPageView())
//// 		{
//// 			if (CPWL_Edit* pEdit = (CPWL_Edit*)GetPDFWindow(pPageView, FALSE))
//// 			{
//// 				pEdit->PasteText();
//// 			}
//// 		}
//// 	}
//}
//
//void CFFL_TextField::OnAddUndo(CPWL_Edit* pEdit)
//{
//// 	ASSERT(pEdit != NULL);
//// 
//// 	IUndo* pUndo = IUndo::GetUndo(m_pApp);
//// 	ASSERT(pUndo != NULL);
//// 
//// 	CFFL_EditUndoItem* pItem = new CFFL_EditUndoItem(pEdit);
//// 
//// 	CReader_Page* pPage = m_pWidget->GetPage();
//// 	ASSERT(pPage != NULL);
//// 
//// 	CPDFSDK_Document* pDocument = pPage->GetDocument();
//// 	ASSERT(pDocument != NULL);
//// 
//// 	pUndo->AddUndoItem(pDocument, pItem, TRUE);
//}
