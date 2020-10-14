#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_label.h"
#include "fpwl_label.h"

CFPWL_Label::CFPWL_Label()
{

}

CFPWL_Label::~CFPWL_Label()
{

}

void CFPWL_Label::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_Label::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_Label;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Widget::Initialize(pAnnot, pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Label::SetText(const CFX_WideString wsText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Label*)m_pWnd)->SetText((FX_LPCWSTR)(wsText));
	
	return FPDFW_ERR_Succeeded;
}
