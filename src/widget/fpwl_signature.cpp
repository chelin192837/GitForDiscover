#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_signature.h"
#include "fpwl_signature.h"

CFPWL_Signature::CFPWL_Signature()
{

}

CFPWL_Signature::~CFPWL_Signature()
{

}

void CFPWL_Signature::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_Signature::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_Signature;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	CPWL_Widget::Initialize(pAnnot, pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Signature::SetShowText(FX_BOOL bShowText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Signature*)m_pWnd)->SetTextFlag(bShowText);
	
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Signature::SetShowImage(FX_BOOL bShowImage)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Signature*)m_pWnd)->SetImageFlag(bShowImage);
	
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Signature::SetText(const CFX_WideString text)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Signature*)m_pWnd)->SetText((FX_LPCWSTR)text);
	
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Signature::SetImageStream(CPDF_Stream* pStream, FX_LPCSTR sImageAlias)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Signature*)m_pWnd)->SetImageStream(pStream, sImageAlias);
	
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Signature::SetDescription(const CFX_WideString description)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Signature*)m_pWnd)->SetDescription((FX_LPCWSTR)description);
	
	return FPDFW_ERR_Succeeded;
}
