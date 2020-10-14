#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_button.h"
#include "fpwl_pushbutton.h"

CFPWL_PushButton::CFPWL_PushButton() 
{
	
}

CFPWL_PushButton::~CFPWL_PushButton()
{

}

void CFPWL_PushButton::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_PushButton::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_Button();
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Widget::Initialize(pAnnot, pProperties);
	return FPDFW_ERR_Succeeded;
}

