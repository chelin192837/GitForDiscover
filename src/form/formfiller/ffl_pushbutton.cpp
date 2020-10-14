#include "../../inc/fpdf_define.h"
#include "ffl_widget.h"
#include "ffl_button.h"
#include "ffl_pushbutton.h"

CFFL_PushButtonCtrl::CFFL_PushButtonCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler)
	: CFFL_ButtonCtrl(pControl, pWidgetHandler)
{

}

CFFL_PushButtonCtrl::~CFFL_PushButtonCtrl()
{

}

IFSPDF_Widget* CFFL_PushButtonCtrl::InitWidget()
{
	IFSPDF_PushButton* widget = IFSPDF_PushButton::Create();
	if(!widget) return NULL;

	CFSPDF_WidgetProperties properties;
	FillCreateParam(properties);
	widget->Initialize(m_pControl, &properties);
	widget->Update();
	return widget;
}

FX_BOOL	CFFL_PushButtonCtrl::OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	IFSPDF_Widget* widget = GetWidget(GetCurrentPageView(), FALSE);
	if(nKeyCode == e_vkeyReturn && widget)
	{
		/* TODO: Phantom Action Code, pending for implement.
		FX_BOOL bExit = FALSE;
		FX_BOOL bReset = FALSE;
		pIFormFiller->OnButtonUp(this->m_pWidget, this->GetCurPageView(), bReset, bExit);

		if (bExit)
		{
			return FALSE;
		}*/
		return TRUE;
	}

	return CFFL_ButtonCtrl::OnChar(nKeyCode, nFlags);
}

void CFFL_PushButtonCtrl::OnDraw(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags)
{
	CFFL_ButtonCtrl::OnDraw(pPageView, pDevice, pUser2Device, dwFlags);

	/* TODO: Phantom code, working on it.
	if (m_bMouseDown)
	{
		ASSERT(pPageView != NULL);
		ASSERT(pAnnot != NULL);

		CBF_Widget* pWidget = (CBF_Widget*)pAnnot;

		CPDF_FormControl* pCtrl = pWidget->GetFormControl();
		ASSERT(pCtrl != NULL);

		CPDF_FormControl::HighlightingMode eHM = pCtrl->GetHighlightingMode();

		CPDF_Rect rcAnnot = pWidget->GetRect();
		CPDF_Rect rcAnnotIn = CFFL_Utils::DeflateRect(rcAnnot,(FX_FLOAT)pWidget->GetBorderWidth());

		CRect rcNew = this->FFLtoWnd(pPageView, rcAnnot);
		CRect rcNewIn = this->FFLtoWnd(pPageView, rcAnnotIn);

		switch (eHM)
		{
		case CPDF_FormControl::Invert://invert
			{					
				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = RGB(255,204,142);

				HBRUSH hBrush = CreateBrushIndirect(&logBrush);
				void* hOldBrush = ::SelectObject(hDC,hBrush);

				::PatBlt(hDC,rcNew.left,
					rcNew.top,
					rcNew.Width(),
					rcNew.Height(),
					PATINVERT);
				::SelectObject(hDC,hOldBrush);
				::DeleteObject(hBrush);
			}
			break;
		case CPDF_FormControl::Outline://outline
			{
				HRGN hRgnOut = ::CreateRectRgnIndirect(&rcNew);
				HRGN hRgnIn = ::CreateRectRgnIndirect(&rcNewIn);
				HRGN hRgnResult = ::CreateRectRgn( 0, 0, 0, 0 );

				::CombineRgn(hRgnResult,hRgnOut,hRgnIn,RGN_XOR);

				::InvertRgn(hDC,hRgnResult);

				::DeleteObject(hRgnOut);
				::DeleteObject(hRgnIn);
				::DeleteObject(hRgnResult);
			}
			break;
		}
	}*/
}