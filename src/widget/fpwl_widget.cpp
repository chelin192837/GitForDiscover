#ifndef _FPWL_WIDGET_H_
#define _FPWL_WIDGET_H_

#include "../inc/fpdf_define.h"

#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/fxedit/fx_edit.h"
#include "pdfwindow/include/pdfwindow/pwl_wnd.h"
#include "pdfwindow/include/pdfwindow/pwl_fontmap.h"
#include "pdfwindow/include/fx_systemhandlerimp.h"
#include "pdfwindow/include/fpdf_resetappearance.h"
#include "fpwl_fontmap.h"
//TODO: wait for /form
//#include "../form/formfiller/ffl_widget.h"

CPWL_Widget::CPWL_Widget()
	: m_pProperties(NULL)
	, m_pWnd(NULL)
	, m_pAnnot(NULL)
	, m_pFontMap(NULL)
{
	
}

CPWL_Widget::~CPWL_Widget()
{
	if (m_pProperties)
	{
		delete m_pProperties;
		m_pProperties = NULL;
	}

	if (m_pWnd)
	{
		m_pWnd->Destroy();
		delete m_pWnd;
		m_pWnd = NULL;
	}

	if (m_pFontMap)
	{
		m_pFontMap->Release();
		m_pFontMap = NULL;
	}
}

void CPWL_Widget::Release()
{
	delete this;
		
}

FPDFW_ERR CPWL_Widget::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pAnnot = pAnnot;
	m_pProperties = FX_NEW CFSPDF_WidgetProperties;
	if (!m_pProperties) return FPDFW_ERR_Indefinite;

	if (!m_pWnd)
	{	
		m_pWnd = FX_NEW CPWL_Wnd();
		if (!m_pWnd) return FPDFW_ERR_Indefinite;
	}

	if (pProperties) *m_pProperties = *pProperties;

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CPWL_Widget::Finalize()
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CPWL_Widget::GetWidgetRect(CFX_FloatRect &rect)
{
//	rect = m_pProperties->m_rtWidget;

	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	rect = m_pWnd->GetWindowRect();
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CPWL_Widget::SetWidgetRect(const CFX_FloatRect &rect)
{
	m_pProperties->m_rtWidget = rect;
	return FPDFW_ERR_Succeeded;
}

FX_BOOL CPWL_Widget::GetClientRect(CFX_FloatRect &rect)
{
	if (!m_pWnd) return FALSE;

	rect = m_pWnd->GetClientRect();
	return TRUE;
}

FX_DWORD CPWL_Widget::GetStyles()
{
	return m_pProperties->m_dwStyles;
}

FPDFW_ERR CPWL_Widget::ModifyStyles(FX_DWORD dwStylesAdded, FX_DWORD dwStylesRemoved)
{
	m_pProperties->m_dwStyles = (m_pProperties->m_dwStyles & ~dwStylesRemoved) | dwStylesAdded;
	return FPDFW_ERR_Succeeded;
}

FX_DWORD CPWL_Widget::GetStylesEx()
{
	return m_pProperties->m_dwStyleExes;
}

FPDFW_ERR CPWL_Widget::ModifyStylesEx(FX_DWORD dwStylesExAdded, FX_DWORD dwStylesExRemoved)
{
	m_pProperties->m_dwStyleExes = (m_pProperties->m_dwStyles & ~dwStylesExRemoved) | dwStylesExAdded;
	return FPDFW_ERR_Succeeded;
}

FX_DWORD CPWL_Widget::GetStates()
{
	return m_pProperties->m_dwStates;
}

FPDFW_ERR CPWL_Widget::SetStates(FX_DWORD dwStates, FX_BOOL bSet)
{
	bSet ? (m_pProperties->m_dwStates |= dwStates) : (m_pProperties->m_dwStates &= ~dwStates);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CPWL_Widget::SetText(const CFX_WideString &wsText)
{
	return FPDFW_ERR_Succeeded;
}

void FXARGBToCPWLColor(FX_ARGB argb, CPWL_Color& color)
{
	color.nColorType = COLORTYPE_TRANSPARENT;
	if (FXARGB_A(argb) > 0)
		color.nColorType = COLORTYPE_RGB;
	
	color.fColor1 = FXARGB_R(argb)/255.0f;
	color.fColor2 = FXARGB_G(argb)/255.0f;
	color.fColor3 = FXARGB_B(argb)/255.0f;
}

FX_BOOL CPWL_Widget::Update()
{
	if (!m_pWnd) return FALSE;

	PWL_CREATEPARAM cp;
	cp.rcRectWnd = m_pProperties->m_rtWidget;

	cp.nBorderStyle = m_pProperties->m_nBorderStyle;
	cp.dwBorderWidth = m_pProperties->m_dwBorderWidth;

	FXARGBToCPWLColor(m_pProperties->m_sBackgroundColor, cp.sBackgroundColor);
	FXARGBToCPWLColor(m_pProperties->m_sBorderColor, cp.sBorderColor);
	FXARGBToCPWLColor(m_pProperties->m_sTextColor, cp.sTextColor);
	FXARGBToCPWLColor(m_pProperties->m_sTextStrokeColor, cp.sTextStrokeColor);
	cp.nTransparency = m_pProperties->m_nTransparency;
	cp.fFontSize = m_pProperties->m_fFontSize;
	cp.sDash.nDash = m_pProperties->m_nDash;
	cp.sDash.nGap = m_pProperties->m_nDashGap;
	cp.sDash.nPhase = m_pProperties->m_nDashPhase;
	cp.pAttachedData = m_pProperties->m_pAttachedData;
	cp.dwFlags |=  PWS_BACKGROUND;

	if (cp.fFontSize <= 0)
		cp.dwFlags |= PWS_AUTOFONTSIZE;

	if (m_pProperties->m_dwStyles & FPDFW_WGTSTYLE_Border)
		cp.dwFlags |= PWS_BORDER;

	if (m_pProperties->m_dwStyles & FPDFW_WGTSTYLE_VScroll)
		cp.dwFlags |= PWS_VSCROLL;

	cp.dwFlags |= PWS_VISIBLE;
	if (m_pProperties->m_dwStates & FPDFW_WGTSTATE_Disabled)
		cp.dwFlags |= PWS_BORDER;

	if (m_pWnd->GetClassName() == PWL_CLASSNAME_EDIT)
	{
		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_Password)
			cp.dwFlags |= PES_PASSWORD;

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_MultiLine)
			cp.dwFlags |= PES_MULTILINE;

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_WantReturn)
			cp.dwFlags |= PES_AUTORETURN;

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_AutoHScroll)
			cp.dwFlags |= PES_AUTOSCROLL;

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_AutoVScroll)
			cp.dwFlags |= PWS_VSCROLL;

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_RichEdit)
			cp.dwFlags |= PES_RICH;

		cp.dwFlags |= PES_UNDO;

		switch(m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_HAlignMask)
		{
		case FPDFW_STYLEEXT_EDT_HNear:
			cp.dwFlags |= PES_LEFT;
			break;
		case FPDFW_STYLEEXT_EDT_HCenter:
			cp.dwFlags |= PES_MIDDLE;
			break;
		case FPDFW_STYLEEXT_EDT_HFar:
			cp.dwFlags |= PES_RIGHT;
			break;
		}

		switch(m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_VAlignMask)
		{
		case FPDFW_STYLEEXT_EDT_VNear:
			cp.dwFlags |= PES_TOP;
			break;
		case FPDFW_STYLEEXT_EDT_VCenter:
			cp.dwFlags |= PES_CENTER;
			break;
		case FPDFW_STYLEEXT_EDT_VFar:
			cp.dwFlags |= PES_BOTTOM;
			break;
		}

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_EDT_CombText)
		{
			cp.dwFlags |= PES_CHARARRAY | PES_CENTER;
		}
	}

	if (FX_BSTRC("CPWL_ListBox") == m_pWnd->GetClassName())
	{
		if (cp.dwFlags & PWS_AUTOFONTSIZE)
			cp.fFontSize = 12.0f;

		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_LTB_MultiSelection)
			cp.dwFlags |= PLBS_MULTIPLESEL;

		if (m_pProperties->m_dwStyles & FPDFW_WGTSTYLE_VScroll)
			cp.dwFlags |= PWS_VSCROLL;
	}

	if (FX_BSTRC("CPWL_ComboBox") == m_pWnd->GetClassName())
	{
		if (m_pProperties->m_dwStyleExes & FPDFW_STYLEEXT_CMB_DropDown)
			cp.dwFlags |= PCBS_ALLOWCUSTOMTEXT;
	}

	cp.hAttachedWnd = m_pProperties->m_pWidget;
	cp.pSystemHandler = m_pProperties->m_pSystemHandler;

	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)m_pAnnot->getPage()->getDocument())->getPDFDocument();
	m_pFontMap = FX_NEW CBA_FontMap(m_pAnnot, cp.pSystemHandler, pPDFDoc/*((CFSCRT_LTPDFDocument*)m_pProperties->m_pWidget->GetControl()->GetPage()->GetDocument())->m_pSTDoc*/);
	if (!m_pFontMap) return FALSE;

	((CBA_FontMap*)m_pFontMap)->Initial(NULL);
	cp.pFontMap = m_pFontMap;

	m_pWnd->Create(cp);
	
	return TRUE;
}

FPDFW_ERR CPWL_Widget::LockUpdate()
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CPWL_Widget::UnlockUpdate()
{
	return FPDFW_ERR_Succeeded;
}

FX_DWORD CPWL_Widget::HitTest(FX_FLOAT fx, FX_FLOAT fy)
{
	if (!m_pWnd) return FPDFW_WGTHITTEST_Unknown;

	CPDF_Point point(fx, fy);

	return (FX_DWORD)m_pWnd->WndHitTest(point);
}

FPDFW_ERR CPWL_Widget::DrawWidget(CFX_RenderDevice *pDevice, const CFX_Matrix *pMatrix)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPDF_Matrix pUser2Device;
	pUser2Device.Copy(*pMatrix);

	m_pWnd->DrawAppearance(pDevice, &pUser2Device);
	return FPDFW_ERR_Succeeded;
}

FX_BOOL CPWL_Widget::OnKeyDown(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	if (!m_pWnd) return FALSE;

	return m_pWnd->OnKeyDown(nKeyCode, nFlags);
}

FX_BOOL CPWL_Widget::OnKeyUp(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	if (!m_pWnd) return FALSE;

	return m_pWnd->OnKeyUp(nKeyCode, nFlags);
}

FX_BOOL CPWL_Widget::OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	if (!m_pWnd) return FALSE;

	return m_pWnd->OnChar(nKeyCode, nFlags);
}

FX_BOOL CPWL_Widget::OnString(FX_WSTR string, FS_DWORD nFlags)
{
	return TRUE;
}

FX_BOOL CPWL_Widget::OnLButtonDown(FS_DWORD nFlags,const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnLButtonDown(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnLButtonUp(FS_DWORD nFlags, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnLButtonUp(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnLButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnLButtonDblClk(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnMouseMove(FS_DWORD nFlags, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnMouseMove(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnRButtonDown(FS_DWORD nFlags, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnRButtonDown(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnRButtonUp(FS_DWORD nFlags, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnRButtonUp(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnRButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnRButtonDblClk(pt, nFlags);
}

FX_BOOL CPWL_Widget::OnMouseWheel(FS_DWORD nFlags, short zDelta, const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	CPDF_Point pt(point.x , point.y);
	return m_pWnd->OnMouseWheel(zDelta, pt, nFlags);
}

FX_BOOL CPWL_Widget::OnMouseHover(const CFX_PointF& point)
{
	if (!m_pWnd) return FALSE;

	//CPDF_Point pt(point.x , point.y);
	//return m_pWnd->OnMouseHover(pt);

	return TRUE;
}

void CPWL_Widget::SetFocus()
{
	if (!m_pWnd) return;

	m_pWnd->SetFocus();
}

void	CPWL_Widget::KillFocus()
{
	if (!m_pWnd) return;

	m_pWnd->KillFocus();
}

FPDFW_ERR	CPWL_Widget::GetFocusRect(CFX_FloatRect &rect)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	rect = m_pWnd->GetFocusRect();
	return FPDFW_ERR_Succeeded;
}

CFX_ByteString	CPWL_Widget::GetAppearanceStream()
{
	if (!m_pWnd) return "";

	CFX_ByteString csAPStr;
	m_pWnd->GetAppearanceStream(csAPStr);
	return csAPStr;
}

FS_INT32 CPWL_Widget::GetDefaultFontItalicAngle()
{
	FS_INT32 nItalicAngle = 0;
	FS_BOOL bItalic;
	if (! ((CBA_FontMap*)m_pFontMap)->GetDefaultFont())
		return nItalicAngle;
	nItalicAngle = ((CBA_FontMap*)m_pFontMap)->GetDefaultFont()->GetItalicAngle();

	if (nItalicAngle == 0)
	{
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)m_pAnnot->getDict();
		CFX_ByteString sDA = pAnnotDict->GetString("DA");
	
		if (!sDA.IsEmpty())
		{
			CPDF_DefaultAppearance da(sDA);
			CFX_ByteString sFont;
			FX_FLOAT fFontSize = 0.0f;
	
			da.GetFont(sFont, fFontSize);
			bItalic = (sFont.Right(6) == "Italic" || sFont.Right(7) == "Oblique");
	
			sFont = ((CBA_FontMap*)m_pFontMap)->GetDefaultFont()->GetBaseFont();
			bItalic |= (sFont.Right(6) == "Italic" || sFont.Right(7) == "Oblique");

			if (bItalic)
				nItalicAngle = 12;
		}
	}

	return nItalicAngle;
}

FPDFW_ERR	CPWL_Widget::ResetAppearance(CPDF_FormControl* pControl,FX_LPCWSTR sValue)
{
	CFSCRT_STPDFResetApperance apperance(pControl);
	CPDF_FormField::Type type = pControl->GetType();
	
	FX_SystemHandlerImp* handler = FX_NEW FX_SystemHandlerImp(NULL);

	FSAnnot* pAnnot = (FSAnnot*)FX_NEW FSPDFAnnotImp(pControl->GetWidget(), NULL);
	if (!pAnnot) return FSCRT_ERRCODE_OUTOFMEMORY;
	CBA_FontMap* pFontmap = FX_NEW CBA_FontMap(pAnnot, handler, (pControl->GetInterForm()->GetDocument()));
	pFontmap->Initial(NULL);//Default to set font to /Resource <</Font >> in "N" dictionary.
							//If /Resource in "N" dictionary doesn't exist,this function will create a new one.
	
	switch(type)
	{
	case CPDF_FormField::PushButton:
		apperance.ST_ResetAppearance_PushButton(pFontmap);
		break;
	case CPDF_FormField::RadioButton:
		apperance.ST_ResetAppearance_RadioButton();
		break;
	case CPDF_FormField::CheckBox:
		apperance.ST_ResetAppearance_CheckBox();
		break;
	case CPDF_FormField::ListBox:
		apperance.ST_ResetAppearance_ListBox(pFontmap);
		break;
	case CPDF_FormField::ComboBox:
		apperance.ST_ResetAppearance_ComboBox(pFontmap, sValue);
		break;
	case CPDF_FormField::Text:
	case CPDF_FormField::RichText:
	case CPDF_FormField::File:
		apperance.ST_ResetAppearance_TextField(pFontmap, sValue);
		break;
	}
	
	if(handler) delete handler;
	if(pAnnot) delete pAnnot;
	if(pFontmap) delete pFontmap;
	return FSCRT_ERRCODE_SUCCESS;
}



#endif //_FPWL_WIDGET_H_
