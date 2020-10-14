#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"
#include "ffl_button.h"
#include "ffl_radiobutton.h"

CFFL_RadioButtonCtrl::CFFL_RadioButtonCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_ButtonCtrl(pControl, pWidgetHandler)
{

}

CFFL_RadioButtonCtrl::~CFFL_RadioButtonCtrl()
{

}

IFSPDF_Widget* CFFL_RadioButtonCtrl::InitWidget()
{
	IFSPDF_RadioButton* widget = IFSPDF_RadioButton::Create();
	if(!widget) return NULL;

	CFSPDF_WidgetProperties properties;
	FillCreateParam(properties);
	widget->Initialize(m_pControl, &properties);
	widget->Update();
	widget->SetCheckState(((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->IsChecked());

	return widget;
}

FX_BOOL CFFL_RadioButtonCtrl::OnLButtonUp (FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	if(!(m_nMouseState & CFFL_BUTTON_MOUSESTATE_DOWN))
		return FALSE;

	CFFL_ButtonCtrl::OnLButtonUp(pPageView, nFlags, point);

	if (IsActivated())
	{
		if (IFSPDF_RadioButton* pWnd = GetWidget(pPageView, TRUE))
		{
			pWnd->SetCheckState(TRUE);
		}

		if(m_pControl)
			IFSPDF_Widget::ResetAppearance(((FSFormControlImp*)m_pControl)->GetCPDFFormControl(),NULL);
		if (!CommitData(pPageView, nFlags)) return FALSE;
	}

	return TRUE;
}

FX_BOOL CFFL_RadioButtonCtrl::OnKeyDown (FS_DWORD nKeyCode, FS_DWORD nFlags)
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

FX_BOOL CFFL_RadioButtonCtrl::OnChar (FS_DWORD nKeyCode, FS_DWORD nFlags)
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

			if (IFSPDF_RadioButton* pWnd = GetWidget(pPageView, TRUE))
			{
				pWnd->SetCheckState(TRUE); 
			}

			CommitData(pPageView, nFlags);
			return TRUE;
		}
	default:
		return CFFL_ButtonCtrl::OnChar(nKeyCode, nFlags);
	}
}

FX_BOOL CFFL_RadioButtonCtrl::IsDataChanged (FSPDFPage* pPageView)
{
	FXSYS_assert(m_pControl);
	if (IFSPDF_RadioButton* pWnd = GetWidget(pPageView, FALSE))
	{
		return ((FSFormControlImp*)m_pControl)->GetCPDFFormControl()->IsChecked() != pWnd->GetCheckState();
	}
	return FALSE;
}

void CFFL_RadioButtonCtrl::SaveData (FSPDFPage* pPageView)
{
	FXSYS_assert(m_pControl);
	if (IFSPDF_RadioButton* pWnd = GetWidget(pPageView, TRUE))
	{
		((FSFormControlImp*)m_pControl)->SetChecked(pWnd->GetCheckState(), FALSE);
		UpdateField();

		SetChangeMark();
	}
}
