#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_specialbutton.h"
#include "fpwl_radiobutton.h"

CFPWL_RadioButton::CFPWL_RadioButton() 
{
	
}

CFPWL_RadioButton::~CFPWL_RadioButton()
{

}

void CFPWL_RadioButton::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_RadioButton::Initialize(FSAnnot* pAnnot,CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_RadioButton;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Widget::Initialize(pAnnot, pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_RadioButton::SetCheckState(FX_INT32 iCheck)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_RadioButton*)m_pWnd)->SetCheck(iCheck);

	return FPDFW_ERR_Succeeded;
}

FX_INT32 CFPWL_RadioButton::GetCheckState()
{
	if (!m_pWnd) return 0;

	return ((CPWL_RadioButton*)m_pWnd)->IsChecked();
}
