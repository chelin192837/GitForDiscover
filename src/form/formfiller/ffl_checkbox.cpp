#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"
#include "ffl_button.h"
#include "ffl_checkbox.h"

CFFL_CheckBoxCtrl::CFFL_CheckBoxCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_ButtonCtrl(pControl, pWidgetHandler)
{

}

CFFL_CheckBoxCtrl::~CFFL_CheckBoxCtrl()
{

}

IFSPDF_Widget* CFFL_CheckBoxCtrl::InitWidget()
{
	IFSPDF_CheckBox* widget = IFSPDF_CheckBox::Create();
	if(!widget) return NULL;

	CFSPDF_WidgetProperties properties;
	FillCreateParam(properties);

	widget->Initialize(m_pControl, &properties);
	widget->Update();
	widget->SetCheckState(((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->IsChecked());

	return widget;
}


FX_BOOL CFFL_CheckBoxCtrl::OnLButtonUp (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	if(!(m_nMouseState & CFFL_BUTTON_MOUSESTATE_DOWN)) //phantom does this on chkbox. we add it to chkbox & radio
		return FALSE;

	CFFL_ButtonCtrl::OnLButtonUp(pPageView, nFlags, point);

	if (IsActivated())
	{
		if (IFSPDF_CheckBox* pWnd = GetWidget(pPageView, TRUE))
			pWnd->SetCheckState(!pWnd->GetCheckState());
		if (!CommitData(pPageView, nFlags)) return FALSE;
	}

	return TRUE;
}

FX_BOOL CFFL_CheckBoxCtrl::OnKeyDown (FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	switch(nKeyCode)
	{
	case e_vkeyReturn:
	case e_vkeySpace:
		return TRUE;
	default:
		return CFFL_ButtonCtrl::OnKeyDown(nKeyCode, nFlags);
	}
}

FX_BOOL CFFL_CheckBoxCtrl::OnChar (FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	switch(nKeyCode)
	{
	case e_vkeyReturn:
	case e_vkeySpace:
		{
			CFSPDF_WidgetAnnotHandler* pIWidget = GetWidgetHandler();
			if(!pIWidget) return FALSE;

			FSPDFPage* pPageView = GetCurrentPageView();
			if(!pPageView) return FALSE;

/*  TODO: Verify this code...
			FX_BOOL bReset = FALSE;
			FX_BOOL bExit = FALSE;

			pIFormFiller->OnButtonUp(m_pWidget, pPageView, bReset, bExit,nFlags);

			if (bReset) return TRUE;
			if (bExit) return TRUE;*/

			CFFL_ButtonCtrl::OnChar(nKeyCode, nFlags);

			if (IFSPDF_CheckBox* pWnd = GetWidget(pPageView, TRUE))
			{
				pWnd->SetCheckState(!pWnd->GetCheckState()); 
			}

			CommitData(pPageView, nFlags);
			return TRUE;
		}
	default:
		return CFFL_ButtonCtrl::OnChar(nKeyCode, nFlags);
	}
}

FX_BOOL CFFL_CheckBoxCtrl::IsDataChanged (FSPDFPage* pPageView)
{
	FXSYS_assert(m_pControl);
	if (IFSPDF_CheckBox* pWnd = GetWidget(pPageView, FALSE))
		return ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->IsChecked() != pWnd->GetCheckState();
	return FALSE;
}

void CFFL_CheckBoxCtrl::SaveData (FSPDFPage* pPageView)
{
	if (IFSPDF_CheckBox* pWnd = GetWidget(pPageView, TRUE))
	{
		FX_BOOL bNewChecked = pWnd->GetCheckState() ? 1 : 0;

		((FSFormControlImp*)m_pControl)->SetChecked(bNewChecked, FALSE);
		UpdateField();

		SetChangeMark();
	}
}
