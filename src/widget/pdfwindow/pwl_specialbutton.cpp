//#include "../../../../fs_precompilers.h"
//#if defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)

#include "include/pdfwindow/pdfwindow.h"
#include "include/pdfwindow/pwl_wnd.h"
#include "include/pdfwindow/pwl_button.h"
#include "include/pdfwindow/pwl_specialbutton.h"
#include "include/pdfwindow/pwl_utils.h"

/* --------------------------- CPWL_PushButton ---------------------------- */

CPWL_PushButton::CPWL_PushButton()
{
}

CPWL_PushButton::~CPWL_PushButton()
{
}

CFX_ByteString CPWL_PushButton::GetClassName() const
{
	return "CPWL_PushButton";
}

CPDF_Rect CPWL_PushButton::GetFocusRect() const
{
	return CPWL_Utils::DeflateRect(this->GetWindowRect(),(FX_FLOAT)GetBorderWidth());
}

/* --------------------------- CPWL_CheckBox ---------------------------- */

CPWL_CheckBox::CPWL_CheckBox() : m_bChecked(FALSE)
{
}

CPWL_CheckBox::~CPWL_CheckBox()
{
}

CFX_ByteString CPWL_CheckBox::GetClassName() const
{
	return "CPWL_CheckBox";
}

void CPWL_CheckBox::SetCheck(FX_BOOL bCheck)
{
	m_bChecked = bCheck;
}

FX_BOOL CPWL_CheckBox::IsChecked() const
{
	return m_bChecked;
}

FX_BOOL CPWL_CheckBox::OnLButtonUp(const CPDF_Point & point)
{
	if (IsReadOnly()) return FALSE;

	SetCheck(!IsChecked());
	return TRUE;
}

FX_BOOL CPWL_CheckBox::OnChar(FX_WORD nChar)
{
	SetCheck(!IsChecked());
	return TRUE;
}

/* --------------------------- CPWL_RadioButton ---------------------------- */

CPWL_RadioButton::CPWL_RadioButton() : m_bChecked(FALSE)
{
}

CPWL_RadioButton::~CPWL_RadioButton()
{
}

CFX_ByteString CPWL_RadioButton::GetClassName() const
{
	return "CPWL_RadioButton";
}

FX_BOOL	CPWL_RadioButton::OnLButtonUp(const CPDF_Point & point)
{
	if (IsReadOnly()) return FALSE;

	SetCheck(TRUE);
	return TRUE;
}

void CPWL_RadioButton::SetCheck(FX_BOOL bCheck)
{
	m_bChecked = bCheck;
}

FX_BOOL CPWL_RadioButton::IsChecked() const
{
	return m_bChecked;
}

FX_BOOL CPWL_RadioButton::OnChar(FX_WORD nChar)
{
	SetCheck(TRUE);
	return TRUE;
}

//#endif //defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)
