#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"
#include "ffl_button.h"

CFFL_ButtonCtrl::CFFL_ButtonCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_Widget(pControl, pWidgetHandler)
	, m_nMouseState(0)
{

}
CFFL_ButtonCtrl::~CFFL_ButtonCtrl()
{

}

void CFFL_ButtonCtrl::OnDraw(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags)
{
	FXSYS_assert(m_pControl);

	CPDF_FormControl* pCtrl = ((FSFormControlImp*)m_pControl)->GetCPDFFormControl();
	FXSYS_assert(pCtrl);

	CPDF_FormControl::HighlightingMode eHM = pCtrl->GetHighlightingMode();

	if (eHM == CPDF_FormControl::Push && m_nMouseState)
	{
		if(m_nMouseState & CFFL_BUTTON_MOUSESTATE_DOWN)
			CallControlDrawAppearance(m_pControl, pDevice, pUser2Device, CPDF_Annot::Down, CPDF_Annot::Normal, NULL);
		else
			CallControlDrawAppearance(m_pControl, pDevice, pUser2Device, CPDF_Annot::Rollover, CPDF_Annot::Normal, NULL);
	}
	else
		CallControlDrawAppearance(m_pControl, pDevice, pUser2Device, CPDF_Annot::Normal, CPDF_Annot::Normal, NULL);
}


void CFFL_ButtonCtrl::OnDrawInactive(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags)
{
	OnDraw(pPageView, pDevice, pUser2Device, dwFlags);
}

void CFFL_ButtonCtrl::OnMouseEnter(FSPDFPage *pPageView)
{
	m_nMouseState |= CFFL_BUTTON_MOUSESTATE_IN;
	Invalidate(pPageView);
}

void CFFL_ButtonCtrl::OnMouseExit(FSPDFPage *pPageView)
{
	m_nMouseState &= ~CFFL_BUTTON_MOUSESTATE_IN;

	//TODO: EndTimer();
	//TODO: //m_pWidget->HideHint();
	if(GetWidgetHandler()->GetFocusControl() != m_pControl)
	{
		DeActivate();
	}
	Invalidate(pPageView);
}

FX_BOOL CFFL_ButtonCtrl::OnLButtonDown(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	m_nMouseState |= CFFL_BUTTON_MOUSESTATE_DOWN;
	Activate();
	Invalidate(pPageView);
	return TRUE;
}

FX_BOOL CFFL_ButtonCtrl::OnLButtonUp(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point)
{
	m_nMouseState &= ~CFFL_BUTTON_MOUSESTATE_DOWN;
	Invalidate(pPageView);
	return TRUE;
}
