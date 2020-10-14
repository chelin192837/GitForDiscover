#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_icon.h"
#include "fpwl_image.h"

CFPWL_Image::CFPWL_Image()
{

}

CFPWL_Image::~CFPWL_Image()
{

}

void CFPWL_Image::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_Image::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_Image;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Widget::Initialize(pAnnot, pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_Image::SetImageStream(CPDF_Stream * pStream)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Image*)m_pWnd)->SetPDFStream(pStream);

	return FPDFW_ERR_Succeeded;
}
