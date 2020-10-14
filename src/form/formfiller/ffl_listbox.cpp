#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"
#include "ffl_listbox.h"


#ifndef FIELDFLAG_MULTISELECT
#define FIELDFLAG_MULTISELECT				(1<<21)
#endif
#ifndef FIELDFLAG_COMMITONSELCHANGE
#define FIELDFLAG_COMMITONSELCHANGE			(1<<26)
#endif

CFFL_ListBoxCtrl::CFFL_ListBoxCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_Widget(pControl, pWidgetHandler)
{

}
CFFL_ListBoxCtrl::~CFFL_ListBoxCtrl()
{

}

IFSPDF_Widget* CFFL_ListBoxCtrl::InitWidget()
{
	IFSPDF_ListBox* widget = IFSPDF_ListBox::Create();
	if(!widget) return NULL;

	CFSPDF_WidgetProperties properties;
	FillCreateParam(properties);
	widget->Initialize(m_pControl, &properties);

	/*TODO: phantom
	pWnd->SetFillerNotify(CFFL_Module::GetFormFiller(m_pApp));
	*/

	widget->Update();

	CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
	FXSYS_assert(pField);

	for (FX_INT32 i=0, count = pField->CountOptions(); i < count; i++)
		widget->AddString(pField->GetOptionLabel(i));

	FX_BOOL bMultiSelect = pField->GetFieldFlags() & FIELDFLAG_MULTISELECT;

	m_OriginSelections.RemoveAll();

	FX_BOOL bSetCaret = FALSE;  //will put caret on first selected item.

	for (FX_INT32 i = 0, count = pField->CountSelectedItems(); i < count; i++)
	{
		FX_INT32 index = pField->GetSelectedIndex(i);
		if (!bSetCaret && bMultiSelect)
		{
			widget->SetFocusItem(widget->GetItem(index));
			bSetCaret = TRUE;
		}
		widget->SetSelItem(widget->GetItem(i), 1);
		m_OriginSelections.SetAt(i, NULL);
		if(!bMultiSelect)
			break;
	}

	widget->SetTopVisibleItem(widget->GetItem(pField->GetTopVisibleIndex()));
	
	return widget;
}

void CFFL_ListBoxCtrl::FillCreateParam(CFSPDF_WidgetProperties& prop)
{
	CFFL_Widget::FillCreateParam(prop);

	FX_DWORD dwFieldFlag = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField()->GetFieldFlags();

	if (dwFieldFlag & FIELDFLAG_MULTISELECT)
	{
		prop.m_dwStyleExes |= FPDFW_STYLEEXT_LTB_MultiSelection;
	}

	if (dwFieldFlag & FIELDFLAG_COMMITONSELCHANGE)
	{
		/* TODO: phantom code
		cp.dwFlags |= PLBS_COMMITSELECTEDVALUE;
		*/
	}
	prop.m_dwStyles |= FPDFW_WGTSTYLE_VScroll;

	/* phantom code

	if (cp.dwFlags & PWS_AUTOFONTSIZE)
		cp.fFontSize = FFL_DEFAULTLISTBOXFONTSIZE;

	if (!m_pFontMap)
	{
	ASSERT(this->m_pApp != NULL);
	m_pFontMap = new CBA_FontMap(m_pWidget, ISystemHandle::GetSystemHandler(m_pApp));
	m_pFontMap->Initial();
	}
	cp.pFontMap = m_pFontMap;
	*/
}

FX_BOOL CFFL_ListBoxCtrl::OnChar(FS_DWORD nChar, FS_DWORD nFlags)
{
	return CFFL_Widget::OnChar(nChar, nFlags);
}

FX_BOOL CFFL_ListBoxCtrl::IsDataChanged( FSPDFPage* pPageView )
{
	FXSYS_assert(m_pControl != NULL);

	if (IFSPDF_ListBox* widget = this->GetWidget(pPageView, FALSE))
	{
		CPDF_FormField* pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		if (pField->GetFieldFlags() & FIELDFLAG_MULTISELECT)
		{
			if (widget->CountSelItems() != m_OriginSelections.GetCount())
				return TRUE;

			for (FX_INT32 i = 0, count = widget->CountSelItems(); i < count; i++)
			{
				FX_INT32 index = widget->GetItemIndex(widget->GetSelItem(i));
				void* ptrfoo = NULL;
				if (!m_OriginSelections.Lookup(index, ptrfoo))
					return TRUE;
			}
		}
		else
		{
			FX_INT32 widgetselindex = widget->CountSelItems() > 0 ? widget->GetItemIndex(widget->GetSelItem(0)) : -1;
			FX_INT32 originalselindex = pField->CountSelectedItems() > 0 ? pField->GetSelectedIndex(0) : -1;

			return widgetselindex != originalselindex;
		}
	}

	return FALSE;
}

void CFFL_ListBoxCtrl::SaveData( FSPDFPage* pPageView )
{
	FXSYS_assert(m_pControl != NULL);

	if (IFSPDF_ListBox* widget = this->GetWidget(pPageView, FALSE))
	{
		CFX_Int32Array aNewSelect;

		CPDF_FormField *pField = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField();
		FXSYS_assert(pField);

		/*TODO: remove undo code 
		CFX_Int32Array aOldSelect;

		for (int i = 0, count = pField->CountOptions(); i < count; i++)
		{
			if (pField->IsOptionSelected(i))
				aOldSelect.Add(i);
		}

		FX_INT32 nOldTopIndex = pField->GetTopVisibleIndex();
		*/
		FX_INT32 nNewTopIndex = widget->GetItemIndex(widget->GetTopVisibleItem());

		if (pField->GetFieldFlags() & FIELDFLAG_MULTISELECT)
		{
			pField->ClearSelection(FALSE);

			for (FX_INT32 i = 0, count = widget->CountSelItems(); i < count; i++)
			{
				FX_INT32 index = widget->GetItemIndex(widget->GetSelItem(i));

				pField->SetItemSelection(index, TRUE, FALSE);
				aNewSelect.Add(i);
			}
		}
		else
		{
//			pField->ClearSelection(FALSE);

			FX_INT32 index = widget->GetItemIndex(widget->GetSelItem(0));

			pField->SetItemSelection(index, TRUE, FALSE);
		}

		pField->SetTopVisibleIndex(nNewTopIndex);
		m_pControl->resetAppearanceStream();
		IFSPDF_Widget::ResetAppearance(((FSFormControlImp*)m_pControl)->GetCPDFFormControl(), NULL);

		UpdateField(((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField());
		SetChangeMark();
		//TODO: undo code
	}
}

void CFFL_ListBoxCtrl::SaveState( FSPDFPage* pPageView )
{
	FXSYS_assert(pPageView != NULL);

	m_State.RemoveAll();
	if (IFSPDF_ListBox* widget = this->GetWidget(pPageView, FALSE))
	{
		for (FX_INT32 i = 0, count = widget->CountSelItems(); i < count; i++)
			m_State.Add(widget->GetItemIndex(widget->GetSelItem(i)));
	}
}

void CFFL_ListBoxCtrl::RestoreState( FSPDFPage* pPageView )
{
	if (IFSPDF_ListBox* widget = this->GetWidget(pPageView, FALSE))
	{
		{
			for (FX_INT32 i = 0, count = widget->CountSelItems(); i < count; i++)
				widget->SetSelItem(widget->GetSelItem(i), 0);
		}
		{
	 		for (FX_INT32 i = 0, count = m_State.GetSize(); i < count; i++)
				widget->SetSelItem(widget->GetItem(m_State[i]), 1);
		}
	}
}

void CFFL_ListBoxCtrl::OnKeyStroke(FX_BOOL bKeyDown, FX_DWORD nFlag)
{
	FXSYS_assert(m_pControl != NULL);

	int nFlags = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->GetField()->GetFieldFlags();
	if (nFlags & FIELDFLAG_COMMITONSELCHANGE)
	{
		if (IsActivated())
		{
			FSPDFPage* pPageView = GetCurrentPageView();
			FXSYS_assert(pPageView != NULL);

			if (CommitData(pPageView, nFlag))
			{
				ExitFiller(pPageView, TRUE);
			}
		}
	}
}

FX_BOOL CFFL_ListBoxCtrl::OnSetFocus(FS_DWORD nFlags)
{
    if (IFSPDF_ListBox* widget = GetWidget(GetCurrentPageView(), TRUE))
    {
        CFX_WideString wsText;
        FPDFW_HLISTITEM item = widget->GetFocusItem();
        if(item)
            widget->GetItemText(item, wsText);

        CFX_ByteString bsUTFText = wsText.UTF8Encode();
        
        FSFormFillerImp* pFormFiller = NULL;
        m_pWidgetHandler->GetFormFiller(&pFormFiller);
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

FX_BOOL CFFL_ListBoxCtrl::OnKillFocus(FS_DWORD nFlag)
{
    if (IFSPDF_ListBox* widget = GetWidget(GetCurrentPageView(), FALSE))
    {
        CFX_WideString wsText;
        FPDFW_HLISTITEM item = widget->GetFocusItem();
        if(item)
            widget->GetItemText(item, wsText);
        
        CFX_ByteString bsUTFText = wsText.UTF8Encode();
        
        FSFormFillerImp* pFormFiller = NULL;
        m_pWidgetHandler->GetFormFiller(&pFormFiller);
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

/* JJJ's code. not verified.
 
 IFSPDF_Widget* CFFL_ListBoxCtrl::ResetPDFWindow( FSPDFPage* pPageView, FX_BOOL bRestoreValue )
{
if (bRestoreValue)
SaveState(pPageView);

//DestroyPDFWindow(pPageView);

IFSPDF_Widget* pRet = NULL;

if (bRestoreValue)
{
RestoreState(pPageView);
pRet = this->GetWidget(pPageView, FALSE);
}
else
pRet = this->GetWidget(pPageView, TRUE);

//m_pWidget->UpdateField();

return pRet;
}

void CFFL_ListBoxCtrl::GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa)
{
CPDF_FormField* pField = m_pControl->GetCPDFFormControl()->GetField();
switch (type)
{
case CPDF_AAction::Validate:
if (pField->GetFieldFlags() & FIELDFLAG_MULTISELECT)
{
fa.sValue = L"";
}
else
{
if (IFSPDF_ListBox* widget = this->GetWidget(pPageView, FALSE))
{
FXSYS_assert(m_pControl != NULL);
//FX_INT32 nCurSel = pListBox->GetCurSel();
//if (nCurSel >= 0)
//fa.sValue = m_pWidget->GetOptionLabel(nCurSel);
}
}
break;
case CPDF_AAction::LoseFocus:
case CPDF_AAction::GetFocus:
if (pField->GetFlags() & FIELDFLAG_MULTISELECT)
{
fa.sValue = L"";
}
else
{
FXSYS_assert(m_pControl != NULL);
FX_INT32 nCurSel = m_pControl->GetCPDFFormControl()->GetField()->GetSelectedIndex(0);
if (nCurSel >= 0)
fa.sValue = m_pControl->GetCPDFFormControl()->GetField()->GetOptionLabel(nCurSel);
}
break;
}
}

void CFFL_ListBoxCtrl::SetActionData( FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa )
{

}



*/
