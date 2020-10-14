#include "../../inc/fpdf_define.h"
#include "../fpdf_stactionhandler.h"
#include "ffl_widget.h"
#include "ffl_combobox.h"

#define FIELDFLAG_EDIT						(1<<18)
#define FIELDFLAG_COMMITONSELCHANGE			(1<<26)

CFFL_ComboBoxCtrl::CFFL_ComboBoxCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_Widget(pControl, pWidgetHandler)
{

}

CFFL_ComboBoxCtrl::~CFFL_ComboBoxCtrl()
{
	//	for (int i=0,sz=m_IMBox.GetSize(); i<sz; i++)
	//	{
	//		delete m_IMBox.GetAt(i);
	//	}
	//
	//	m_IMBox.RemoveAll();
}

IFSPDF_Widget* CFFL_ComboBoxCtrl::InitWidget()
{
	IFSPDF_ComboBox* widget = IFSPDF_ComboBox::Create();
	if (!widget) return NULL;

	CFSPDF_WidgetProperties properties;
	FillCreateParam(properties);
	widget->Initialize(m_pControl, &properties);

	CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	FXSYS_assert(pField);

	//TODO: 
	//	pWnd->SetFillerNotify(CFFL_Module::GetFormFiller(m_pApp));

	widget->Update();
	widget->SetFillerNotify(this);

	FX_INT32 nCurSel = pField->CountSelectedItems() > 0 ? pField->GetSelectedIndex(0) : -1;
	
	CFX_WideString swText;
	if (nCurSel < 0)
		swText = pField->GetValue();
	else
		swText = pField->GetOptionLabel(nCurSel);

	for (FX_INT32 i = 0, count = pField->CountOptions(); i < count; i++)
	{
		widget->AddString(pField->GetOptionLabel(i));
	}

	widget->SetCurSel(nCurSel);
	widget->SetText(swText);

	return widget;
}

void CFFL_ComboBoxCtrl::FillCreateParam( CFSPDF_WidgetProperties& prop )
{
	CFFL_Widget::FillCreateParam(prop);

	CPDF_FormField *pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	FXSYS_assert(pField);

	if (pField->GetFieldFlags() & FIELDFLAG_EDIT)
	{	
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_CMB_DropDown;
	}


}

FX_BOOL CFFL_ComboBoxCtrl::OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	switch (nKeyCode)
	{
	case e_vkeyReturn:
		{
			FSPDFPage* pPageView = GetCurrentPageView();
			FXSYS_assert(pPageView);
			if(IsActivated()) DeActivate(); else Activate();

			Invalidate();
			if(IsActivated())
			{
				if(IFSPDF_ComboBox* widget = GetWidget(pPageView, TRUE))
					widget->SetFocus();
			}
			else
			{
				if (CommitData(pPageView, 0))
				{
					ReleaseWidget(pPageView);
					return TRUE;
				}
				return FALSE;
			}
		}
		break;
	case e_vkeySpace:
		{
			FSPDFPage* pPageView = GetCurrentPageView();
			FXSYS_assert(pPageView);
			ExitFiller(pPageView, TRUE);
			return TRUE;
		}
	}

	return CFFL_Widget::OnChar(nKeyCode, nFlags);
}


static void CFFL_ComboBox_SimpleGetEditSelRange(IFSPDF_ComboBox* widget, FX_INT32& start, FX_INT32& count)
{
	if(widget->CountEditSelRanges())
		count = widget->GetEditSelRange(0, start);
	else
		start = -1; count = 0;
}

static void CFFL_ComboBox_SimpleSetEditSelRange(IFSPDF_ComboBox* widget, FX_INT32 start, FX_INT32 count = -1)
{
	widget->ClearEditSelections();
	if(start >= 0)
		widget->AddEditSelRange(start, count);
}



FX_BOOL CFFL_ComboBoxCtrl::OnSetFocus(FS_DWORD nFlags)
{
	if (IFSPDF_ComboBox* widget = GetWidget(GetCurrentPageView(), TRUE))
	{
		CFFL_ComboBox_SimpleSetEditSelRange(widget, 0);

		CFX_WideString wsText;
		widget->GetEditText(wsText);
		CFX_ByteString bsUTFText = wsText.UTF8Encode();

		FSFormFillerImp* pFormFiller = NULL;
		m_pWidgetHandler->GetFormFiller(&pFormFiller);

		//const FSCRT_BSTR value = {(FX_LPSTR)(FX_LPCSTR)bsUTFText, (FS_DWORD)bsUTFText.GetLength()};
		//pFormFiller->m_formInterWindowless.GotFocusOnControl((FSPDF_FORMCONTROL)m_pControl, &value);
		FSFormFillerAssist* pFormFillAssist = pFormFiller->GetFormFillerAssist();
		if(pFormFillAssist)
		{
			CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
			if(pField)
				pFormFillAssist->focusGotOnControl(m_pControl, (FX_LPCSTR)bsUTFText);
		}
	}

	return CFFL_Widget::OnSetFocus(nFlags);
}

FX_BOOL CFFL_ComboBoxCtrl::OnKillFocus(FS_DWORD nFlag)
{
	if (IFSPDF_ComboBox* widget = GetWidget(GetCurrentPageView(), FALSE))
	{
		CFFL_ComboBox_SimpleSetEditSelRange(widget, 0);

		CFX_WideString wsText;
		widget->GetEditText(wsText);
		CFX_ByteString bsUTFText = wsText.UTF8Encode();

		FSFormFillerImp* pFormFiller = NULL;
		m_pWidgetHandler->GetFormFiller(&pFormFiller);

		//const FSCRT_BSTR value = {(FX_LPSTR)(FX_LPCSTR)bsUTFText, (FS_DWORD)bsUTFText.GetLength()};
		//pFormFiller->m_formInterWindowless.LostFocusOnControl((FSPDF_FORMCONTROL)m_pControl, &value);
        FSFormFillerAssist* pFormFillAssist = pFormFiller->GetFormFillerAssist();
		if(pFormFillAssist)
		{
			CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
			if(pField)
				pFormFillAssist->focusLostFromControl(m_pControl, (FX_LPCSTR)bsUTFText);
		}
	}

	return CFFL_Widget::OnKillFocus(nFlag);
}


FX_BOOL CFFL_ComboBoxCtrl::IsDataChanged(FSPDFPage *pPageView)
{
	if (IFSPDF_ComboBox* widget = GetWidget(pPageView, FALSE))
	{
		FX_INT32 nCurSel = widget->GetCurSel();

		FXSYS_assert(m_pControl);

		CPDF_FormField *pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		FX_INT32 nFieldSel = pField->CountSelectedItems() > 0 ? pField->GetSelectedIndex(0) : -1;

		if(nCurSel != nFieldSel) return TRUE;

		if ((pField->GetFieldFlags() & FIELDFLAG_EDIT) && nCurSel < 0)
		{
			CFX_WideString wsText;
			widget->GetEditText(wsText);
			return wsText != pField->GetValue();
		}
	}
	return FALSE;
}

void CFFL_ComboBoxCtrl::SaveData(FSPDFPage* pPageView)
{
	if (IFSPDF_ComboBox* widget = GetWidget(pPageView, FALSE))
	{
		CFX_WideString swText; widget->GetEditText(swText);
		FX_INT32 nCurSel = widget->GetCurSel();

		FXSYS_assert(m_pControl);

		CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		FX_BOOL bSetValue = (pField->GetFieldFlags() & FIELDFLAG_EDIT) && ( nCurSel == -1 ? TRUE : swText != pField->GetOptionLabel(nCurSel));

		/* TODO: Remove these undo code 
		CFX_WideString sOldValue;
		FX_INT32 nOldSelect = -1;

		if (bSetValue)
		{
			sOldValue = pField->GetValue();
		}
		else
		{
			nOldSelect = pField->GetSelectedIndex(0);
		}*/

		if (bSetValue)
		{
			pField->SetValue(swText, FALSE);
		}
		else
		{
			pField->SetItemSelection(nCurSel, TRUE, FALSE);
		}

		m_pControl->resetAppearanceStream();	//TODO:	replace with m_pWidget->ResetFieldAppearance(TRUE);
		UpdateField(((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField());
		SetChangeMark();

	}
}

//TODO: Investigate on this
//FX_BOOL	CFFL_ComboBox::IsFieldFull(CReader_PageView* pPageView)
//{
//	if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//	{
//		if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//		{
//			return pEdit->IsTextFull();
//		}
//	}
//
//	return FALSE;
//}

void CFFL_ComboBoxCtrl::SaveState(FSPDFPage* pPageView)
{
	FXSYS_assert(pPageView);

	if (IFSPDF_ComboBox* widget = GetWidget(pPageView, FALSE))
	{
		m_State.nSelIndex = widget->GetCurSel();

		CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		if(pField->GetFieldFlags() & FIELDFLAG_EDIT)
		{
			CFFL_ComboBox_SimpleGetEditSelRange(widget, m_State.nEditSelStart, m_State.nEditSelCount);
			widget->GetEditText(m_State.sValue);
		}
	}
}

void CFFL_ComboBoxCtrl::RestoreState(FSPDFPage* pPageView)
{
	FXSYS_assert(pPageView != NULL);

	if (IFSPDF_ComboBox* widget = GetWidget(pPageView, TRUE))
	{
		CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		if (m_State.nSelIndex >= 0)
			widget->SetCurSel(m_State.nSelIndex);
		else if (pField->GetFieldFlags() & FIELDFLAG_EDIT)
		{
			widget->SetText(m_State.sValue);
			CFFL_ComboBox_SimpleSetEditSelRange(widget, m_State.nEditSelStart, m_State.nEditSelCount);
		}
	}
}

//Implement IFSPDF_Widget_Notify
#define FFL_MAXLISTBOXHEIGHT		140.0f

void CFFL_ComboBoxCtrl::QueryWherePopup(void* pPrivateData, FX_FLOAT fPopupMin,FX_FLOAT fPopupMax, FX_INT32 & nRet, FX_FLOAT & fPopupRet)
{
	FSPDFPage* pPageView = GetCurrentPageView();
	FXSYS_assert(pPageView);

	CPDF_Rect rcPageView(0,0,0,0);
	CPDF_Page* pPDFPage =  ((FSPDFPageImp*)pPageView)->getPage();
	rcPageView = pPDFPage->GetPageBBox();
// 	FX_RECT fflrcPageView_VisiblePage;	pPageView->GetVisibleRect(fflrcPageView_VisiblePage);
// 	CFX_Rect fflrcPageView_VisiblePageDevice;  fflrcPageView_VisiblePageDevice.Set(fflrcPageView_VisiblePage.left, fflrcPageView_VisiblePage.top, fflrcPageView_VisiblePage.Width(), fflrcPageView_VisiblePage.Height());
// 	CPDF_Rect rcPageView;
// 	pPageView->DeviceToPageRect(fflrcPageView_VisiblePageDevice, rcPageView);

	FXSYS_assert(m_pControl);
	CPDF_Rect rcAnnot =	((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetRect();
	 
	FX_FLOAT fTop = 0.0f;
	FX_FLOAT fBottom = 0.0f;

	FS_INT32 nRotation = 0;
	((FSFormControlImp*)m_pControl)->GetMKRotation(&nRotation);
	switch (nRotation / 90)
	{
	default:
	case 0:
		fTop = rcPageView.top - rcAnnot.top;
		fBottom = rcAnnot.bottom - rcPageView.bottom;
		break;
	case 1:
		fTop = rcAnnot.left - rcPageView.left;
		fBottom = rcPageView.right - rcAnnot.right;
		break;
	case 2:
		fTop = rcAnnot.bottom - rcPageView.bottom;
		fBottom = rcPageView.top - rcAnnot.top;
		break;
	case 3:
		fTop = rcPageView.right - rcAnnot.right;
		fBottom = rcAnnot.left - rcPageView.left;
		break;
	}

	FX_FLOAT fFactHeight = 0;
	FX_BOOL bBottom = TRUE;
	FX_FLOAT fMaxListBoxHeight = 0;

	fMaxListBoxHeight = FX_MIN(fPopupMax, FX_MAX(fPopupMin, FFL_MAXLISTBOXHEIGHT));

	if(fBottom > fMaxListBoxHeight)
	{
		fFactHeight = fMaxListBoxHeight;
		bBottom = TRUE;
	}
	else if (fTop > fMaxListBoxHeight)
	{
		fFactHeight = fMaxListBoxHeight;
		bBottom = FALSE;
	}
	else if (fTop > fBottom)
	{
		fFactHeight = fTop;
		bBottom = FALSE;
	}
	else
	{
		fFactHeight = fBottom;
		bBottom = TRUE;
	}

	nRet = bBottom ? 0 : 1;
	fPopupRet = fFactHeight;
}


void CFFL_ComboBoxCtrl::OnKeyStroke(FX_BOOL bKeyDown, FS_DWORD nFlag)
{
	FXSYS_assert(m_pControl != NULL );

	int nFlags;
	((FSFormControlImp*)m_pControl)->ST_GetFlags(&nFlags);

	if (nFlags)
	{
		if (m_bActivated)
		{
			FSPDFPage* pPageView = this->GetCurrentPageView(); 
			FXSYS_assert(pPageView != NULL);

			if (CommitData(pPageView, nFlag))
			{
				//DestroyPDFWindow(pPageView);
				m_bActivated = FALSE;
			}
		}
	}
}

// JJJ:
// CFX_WideString CFFL_ComboBoxCtrl::GetSelectExportText()
// {
// 	CFX_WideString swRet;
// 
// 	int nExport = -1;
// 	FSPDFPage *pPageView = GetCurrentPageView();
// 	if (IFSPDF_ComboBox * pComboBox = GetWidget(pPageView, FALSE))
// 	{
// 		nExport = pComboBox->GetCurSel();
// 	}
// 	if (nExport >= 0)
// 	{
// 		if (CPDF_FormField * pFormField = m_pControl->GetCPDFFormControl()->GetField())
// 		{
// 			swRet = pFormField->GetOptionValue(nExport);
// 			if (swRet.IsEmpty())
// 				swRet = pFormField->GetOptionLabel(nExport);
// 		}
// 	}
// 	return swRet;
// }

// IFSPDF_Widget* CFFL_ComboBoxCtrl::ResetPDFWindow( FSPDFPage* pPageView, FX_BOOL bRestoreValue )
// {
// 	if (bRestoreValue)
// 		SaveState(pPageView);
// 
// 	//DestroyPDFWindow(pPageView);
// 
// 	IFSPDF_Widget* pRet = NULL;
// 
// 	if (bRestoreValue)
// 	{
// 		RestoreState(pPageView);
// 		pRet = this->GetWidget(pPageView, FALSE);
// 	}
// 	else
// 		pRet = this->GetWidget(pPageView, TRUE);
// 	//m_pControl->UpdateField();
// 	return pRet;
// }

// void CFFL_ComboBoxCtrl::GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa)
// {
// 	switch (type)
// 	{
// 	case CPDF_AAction::KeyStroke:
// 		if (IFSPDF_ComboBox* pComboBox = GetWidget(pPageView, FALSE))
// 		{
// 			if (IFSPDF_Edit* pEdit = (IFSPDF_Edit*)pComboBox)
// 			{
// 				//fa.bFieldFull = pEdit->IsTextFull();	
// 				int nSelStart = 0;
// 				int nSelEnd = 0;
// 				pEdit->GetSelRange(nSelStart, nSelEnd);
// 				fa.nSelEnd = nSelEnd;
// 				fa.nSelStart = nSelStart;
// 				pEdit->GetText(fa.sValue);
// 				fa.sChangeEx = GetSelectExportText();
// 
// 				if (fa.bFieldFull)
// 				{
// 					fa.sChange = L"";
// 					fa.sChangeEx = L"";
// 				}
// 			}
// 		}
// 		break;
// 	case CPDF_AAction::Validate:
// 		if (IFSPDF_ComboBox* pComboBox = GetWidget(pPageView, FALSE))
// 		{
// 			if (IFSPDF_Edit* pEdit = (IFSPDF_Edit*)pComboBox)
// 			{
// 				pEdit->GetText(fa.sValue);
// 			}
// 		}
// 		break;
// 	case CPDF_AAction::LoseFocus:
// 	case CPDF_AAction::GetFocus:
// 		FXSYS_assert(m_pControl != NULL);
// 		fa.sValue = m_pControl->GetCPDFFormControl()->GetField()->GetValue();
// 		break;
// 	}
// }

void CFFL_ComboBoxCtrl::SetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa)
{
	switch (type)
	{
	case CPDF_AAction::KeyStroke:
		if (IFSPDF_ComboBox* pComboBox = GetWidget(pPageView, FALSE))
		{
			pComboBox->SetSelRange(fa.nSelStart, fa.nSelEnd);
			pComboBox->ReplaceSel(fa.sChange);
		}
		break;
	}
}

// FX_BOOL CFFL_ComboBoxCtrl::IsActionDataChanged( CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& faOld, const FSPDF_FieldActionParam& faNew )
// {
// 	switch (type)
// 	{
// 	case CPDF_AAction::KeyStroke:
// 		return !faOld.bFieldFull && faOld.nSelEnd != faNew.nSelEnd || faOld.nSelStart != faNew.nSelStart ||
// 			faOld.sChange != faNew.sChange;
// 	}
// 
// 	return FALSE;
// }




//GSDK code:
//
//void CFFL_ComboBox::GetActionData(CReader_PageView* pPageView, CPDF_AAction::AActionType type,
//						BA_FieldAction& fa)
//{
//	switch (type)
//	{
//	case CPDF_AAction::KeyStroke:
//		if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//		{
//			if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//			{
//				fa.bFieldFull = pEdit->IsTextFull();
//				/*
//				int nSelStart = 0;
//				int nSelEnd = 0;
//				pEdit->GetSel(nSelStart, nSelEnd);
//				fa.nSelEnd = nSelEnd;
//				fa.nSelStart = nSelStart;
//				*/
//				fa.sValue = pEdit->GetText();
//
//				if (fa.bFieldFull)
//				{
//					fa.sChange = L"";
//					fa.sChangeEx = L"";
//				}
//			}
//
//			if (!fa.bFieldFull) // mantis bug id 0036031
//			{
//				FX_INT32 nLastSel = pComboBox->GetLastSelect();
//				if (nLastSel >= 0)
//				{
//					fa.sValue = m_pWidget->GetOptionLabel(nLastSel);
//				}
//
//				FX_INT32 nCurSel = pComboBox->GetSelect();
//				if (nCurSel >= 0)
//				{
//					fa.sChangeEx = m_pWidget->GetOptionValue(nCurSel);
//				}
//			}
//		}
//		break;
//	case CPDF_AAction::Validate:
//		if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//		{
//			if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//			{
//				fa.sValue = pEdit->GetText();
//			}
//		}
//		break;
//	case CPDF_AAction::LoseFocus:
//	case CPDF_AAction::GetFocus:
//		ASSERT(m_pWidget != NULL);
//		fa.sValue = m_pWidget->GetValue(FALSE);
//		break;
//	}
//}
//
//void CFFL_ComboBox::SetActionData(CReader_PageView* pPageView, CPDF_AAction::AActionType type, 
//									const BA_FieldAction& fa)
//{
//	switch (type)
//	{
//	case CPDF_AAction::KeyStroke:
//		if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//		{
//			if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//			{
//				pEdit->SetFocus();
//				pEdit->SetSel(fa.nSelStart, fa.nSelEnd);
//				pEdit->ReplaceSel(fa.sChange);
//			}
//		}
//		break;
//	}
//}
//
//
//CPWL_Wnd* CFFL_ComboBox::ResetPDFWindow(CReader_PageView* pPageView, FX_BOOL bRestoreValue)
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
//void CFFL_ComboBox::OnKeyStroke(FX_BOOL& bExit)
//{
//	ASSERT(m_pWidget != NULL);
//
//	int nFlags = m_pWidget->GetFieldFlags();
//	
//	if (nFlags & FIELDFLAG_COMMITONSELCHANGE)
//	{
//		if (m_bValid)
//		{
//			CReader_PageView* pPageView = this->GetCurPageView();
//			ASSERT(pPageView != NULL);
//
//			if (CommitData(pPageView))
//			{
//				EscapeFiller(pPageView, TRUE);
//				bExit = TRUE;
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::OnSetFocus(CPWL_Wnd* pWnd)
//{
//	ASSERT(pWnd != NULL);
//
//	if (pWnd->GetClassName() == PWL_CLASSNAME_EDIT)
//	{
//		CPWL_Edit* pEdit = (CPWL_Edit*)pWnd;
//
//		CBA_EditInput* pHandler = new CBA_EditInput(pEdit);
//		
//		ASSERT(m_pApp != NULL);
//		if (IFX_InputMethod* pInputMethod = m_pApp->GetCurrentInputMethod())
//		{
//			pInputMethod->RegisterInputHandler(pHandler);
//			pInputMethod->EnterInputStatus();
//		}
//
//		m_IMBox.Add(pHandler);
//
//		IUndo* pUndo = IUndo::GetUndo(m_pApp);
//		ASSERT(pUndo != NULL);
//
//		CReader_Page* pPage = m_pWidget->GetPage();
//		ASSERT(pPage != NULL);
//
//		CReader_Document* pDocument = pPage->GetDocument();
//		ASSERT(pDocument != NULL);
//
//		pEdit->SetEditNotify(this);
//		pUndo->BeginEdit(pDocument, CFFL_FormFiller::ExitEdit, pDocument);
//	}
//}
//
//void CFFL_ComboBox::OnKillFocus(CPWL_Wnd* pWnd)
//{
//	ASSERT(pWnd != NULL);
//
//	if (pWnd->GetClassName() == PWL_CLASSNAME_EDIT)
//	{
//		CPWL_Edit* pEdit = (CPWL_Edit*)pWnd;
//
//		ASSERT(m_pApp != NULL);
//		if (IFX_InputMethod* pInputMethod = m_pApp->GetCurrentInputMethod())
//		{
//			pInputMethod->ExitInputStatus();
//			pInputMethod->UnRegisterInputHandler();	
//		}
//
//		for (int i=m_IMBox.GetSize()-1; i>=0; i--)
//		{
//			CBA_EditInput* pHandler = m_IMBox.GetAt(i);
//			ASSERT(pHandler != NULL);
//
//			if (pHandler->GetEdit() == pEdit)
//			{
//				delete pHandler;
//				m_IMBox.RemoveAt(i);
//			}
//		}
//
//		IUndo* pUndo = IUndo::GetUndo(m_pApp);
//		ASSERT(pUndo != NULL);
//
//		CReader_Page* pPage = m_pWidget->GetPage();
//		ASSERT(pPage != NULL);
//
//		CReader_Document* pDocument = pPage->GetDocument();
//		ASSERT(pDocument != NULL);
//
//		pUndo->EndEdit(pDocument);
//		pEdit->SetEditNotify(NULL);
//	}
//}
//
//FX_BOOL CFFL_ComboBox::CanSelectAll(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					return pEdit->CanSelectAll();
//				}
//			}
//		}
//	}
//
//	return FALSE;
//}
//
//FX_BOOL	CFFL_ComboBox::CanDelete(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					return pEdit->CanClear();
//				}
//			}
//		}
//	}
//
//	return FALSE;
//}
//
//FX_BOOL	CFFL_ComboBox::CanCopy(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					return pEdit->CanCopy();
//				}
//			}
//		}
//	}
//
//	return FALSE;
//}
//
//FX_BOOL CFFL_ComboBox::CanCut(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					return pEdit->CanCut();
//				}
//			}
//		}
//	}
//
//	return FALSE;
//}
//
//FX_BOOL	CFFL_ComboBox::CanPaste(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					return pEdit->CanPaste();
//				}
//			}
//		}
//	}
//
//	return FALSE;
//}
//
//void CFFL_ComboBox::DoSelectAll(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					pEdit->SelectAll();
//				}
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::DoDelete(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					pEdit->Clear();
//				}
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::DoCopy(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					pEdit->CopyText();
//				}
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::DoCut(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					pEdit->CutText();
//				}
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::DoPaste(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					pEdit->PasteText();
//				}
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::DoDeselect(CReader_Document* pDocument)
//{
//	ASSERT(pDocument != NULL);
//
//	if (CReader_DocView* pDocView = pDocument->GetCurrentDocView())
//	{
//		if (CReader_PageView* pPageView = pDocView->GetCurrentPageView())
//		{
//			if (CPWL_ComboBox* pComboBox = (CPWL_ComboBox*)GetPDFWindow(pPageView, FALSE))
//			{
//				if (CPWL_Edit* pEdit = (CPWL_Edit*)*pComboBox)
//				{
//					pEdit->SetSel(-1, -1);
//				}
//			}
//		}
//	}
//}
//
//void CFFL_ComboBox::OnAddUndo(CPWL_Edit* pEdit)
//{
//	ASSERT(pEdit != NULL);
//
//	IUndo* pUndo = IUndo::GetUndo(m_pApp);
//	ASSERT(pUndo != NULL);
//
//	CFX_WideString sDesc = FFL_LoadString(m_pApp , IDS_UNDO_INPUT);
//	CFFL_EditUndoItem* pItem = new CFFL_EditUndoItem(pEdit , sDesc);
//
//	CReader_Page* pPage = m_pWidget->GetPage();
//	ASSERT(pPage != NULL);
//
//	CReader_Document* pDocument = pPage->GetDocument();
//	ASSERT(pDocument != NULL);
//
//	pUndo->AddUndoItem(pDocument, pItem, TRUE);
//}
