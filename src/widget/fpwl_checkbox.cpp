#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_specialbutton.h"
#include "fpwl_checkbox.h"

CFPWL_CheckBox::CFPWL_CheckBox() 
{
	
}

CFPWL_CheckBox::~CFPWL_CheckBox()
{

}

void CFPWL_CheckBox::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_CheckBox::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_CheckBox();
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Widget::Initialize(pAnnot ,pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_CheckBox::SetCheckState(FX_INT32 iCheck)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_CheckBox*)m_pWnd)->SetCheck(iCheck);

	return FPDFW_ERR_Succeeded;
}

FX_INT32 CFPWL_CheckBox::GetCheckState()
{
	if (!m_pWnd) return 0;

	return ((CPWL_CheckBox*)m_pWnd)->IsChecked();
}
