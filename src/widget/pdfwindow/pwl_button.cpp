//#include "../../../../fs_precompilers.h"
//#if defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)

#include "include/pdfwindow/pdfwindow.h"
#include "include/pdfwindow/pwl_wnd.h"
#include "include/pdfwindow/pwl_button.h"
#include "include/pdfwindow/pwl_utils.h"

/* ------------------------------- CPWL_Button ---------------------------------- */

CPWL_Button::CPWL_Button() :
	m_bMouseDown(FALSE)
{
}

CPWL_Button::~CPWL_Button()
{
//	PWL_TRACE("~CPWL_Button\n");
}

CFX_ByteString CPWL_Button::GetClassName() const
{
	return "CPWL_Button";
}

void CPWL_Button::OnCreate(PWL_CREATEPARAM & cp)
{
	cp.eCursorType = FXCT_HAND;
}

FX_BOOL CPWL_Button::OnLButtonDown(const CPDF_Point & point, FX_DWORD nFlag)
{
	CPWL_Wnd::OnLButtonDown(point, nFlag);

	m_bMouseDown = TRUE;
	SetCapture();
	
	return TRUE;
}

FX_BOOL CPWL_Button::OnLButtonUp(const CPDF_Point & point, FX_DWORD nFlag)
{
	CPWL_Wnd::OnLButtonUp(point, nFlag);

	ReleaseCapture();
	m_bMouseDown = FALSE;

	return TRUE;
}

//#endif //defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)
