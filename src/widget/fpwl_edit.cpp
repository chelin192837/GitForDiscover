#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_edit.h"
#include "fpwl_edit.h"

CFPWL_Edit::CFPWL_Edit() 
{
	
}

CFPWL_Edit::~CFPWL_Edit()
{

}

void CFPWL_Edit::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_Edit::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_Edit;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->AttachFFLData(pAnnot);
	CPWL_Widget::Initialize(pAnnot ,pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetText(const CFX_WideString &wsText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetText((FX_LPCWSTR)(wsText));
	
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetTextMatrix(const CPDF_Matrix& mtText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetTextMartix(mtText);

	return FPDFW_ERR_Succeeded;
}

FX_INT32	CFPWL_Edit::GetTextLength()const
{
	if (!m_pWnd) return 0;

	return ((CPWL_Edit*)m_pWnd)->GetText().GetLength();
}

FPDFW_ERR	CFPWL_Edit::GetText(CFX_WideString &wsText, FX_INT32 nStart, FX_INT32 nCount) const 
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	if (nCount < 0 ) nCount = ((CPWL_Edit*)m_pWnd)->GetText().GetLength();
	FX_INT32 nEndChar = nStart + nCount;
	wsText = (((CPWL_Edit*)m_pWnd))->GetText(nStart, nEndChar);

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::ClearText()
{
	return FPDFW_ERR_Succeeded;
}

//heyali, there is  problems. What is range?
FPDFW_ERR	CFPWL_Edit::AddSelRange(FX_INT32 nStart, FX_INT32 nCount)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetSel(nStart, nStart + nCount);

	return FPDFW_ERR_Succeeded;
}

//
FX_INT32	CFPWL_Edit::CountSelRanges()
{
	return 0;
}

//heyali, range is a selected word.
FX_INT32	CFPWL_Edit::GetSelRange(FX_INT32& nStart, FX_INT32 &nEnd)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->GetEdit()->GetSel(nStart, nEnd);

	return FPDFW_ERR_Succeeded;
}

FX_INT32	CFPWL_Edit::SetSelRange(FX_INT32 nStart, FX_INT32 nEnd)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->GetEdit()->SetSel(nStart, nEnd);

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::ClearSelections()
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->GetEdit()->SelectNone();

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetCharArray(FX_INT32 nCharArray)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetCharArray(nCharArray);

	return FPDFW_ERR_Succeeded;
}

FX_INT32	CFPWL_Edit::GetLimit()
{
	return 0;
}

FPDFW_ERR	CFPWL_Edit::SetLimit(FX_INT32 nLimit)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetLimitChar(nLimit);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetAliasChar(FX_WCHAR wAlias)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetFormatString(const CFX_WideString &wsFormat)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::Insert(FX_INT32 nStart, FX_LPCWSTR lpText, FX_INT32 nLen)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::DeleteSelections()
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->GetEdit()->Delete();

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::DeleteRange(FX_INT32 nStart, FX_INT32 nCount)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::ReplaceSelections(const CFX_WideStringC &wsReplace)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->ReplaceSel(wsReplace.GetPtr());
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::Replace(FX_INT32 nStart, FX_INT32 nLen, const CFX_WideStringC &wsReplace)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::DoClipboard(FX_INT32 iCmd)
{
	switch(iCmd)
	{
	case 1:
		((CPWL_Edit*)m_pWnd)->CopyText();
		break;
	case 2:
		((CPWL_Edit*)m_pWnd)->CutText();
		break;
	case 3:
		((CPWL_Edit*)m_pWnd)->PasteText();
		break;
	default:
		return FPDFW_ERR_Indefinite;
	}

	return FPDFW_ERR_Succeeded;
}

FX_BOOL		CFPWL_Edit::Redo(FX_BSTR bsRecord)
{
	if (!m_pWnd) return FALSE;

	if (((CPWL_Edit*)m_pWnd)->CanRedo())
		((CPWL_Edit*)m_pWnd)->Redo();

	return TRUE;
}

FX_BOOL		CFPWL_Edit::Undo(FX_BSTR bsRecord)
{
	if (!m_pWnd) return FALSE;

	if (((CPWL_Edit*)m_pWnd)->CanUndo())
		((CPWL_Edit*)m_pWnd)->Undo();

	return TRUE;
}

FPDFW_ERR	CFPWL_Edit::SetHorzScale(FX_INT32 nHorzScale)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	
	((CPWL_Edit*)m_pWnd)->SetHorzScale(nHorzScale);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetCharSpace(FX_FLOAT fCharSpace)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetCharSpace(fCharSpace);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::SetLineLeading(FX_FLOAT fLineLeading)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_Edit*)m_pWnd)->SetLineLeading(fLineLeading);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_Edit::GetContentRect(CFX_FloatRect &rect)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	rect = ((CPWL_Edit*)m_pWnd)->GetContentRect();

	FS_INT32 italicAngle = GetDefaultFontItalicAngle();
	FS_FLOAT fontSize = 0;
	if(FSAnnot::e_annotFreeText == m_pAnnot->getType()){
		CFX_ByteString fontName; FS_FLOAT fontSize;
		((FSPDFFreeTextImp*)(FSFreeText*)m_pAnnot)->getFontInfo(fontName, fontSize);
		CFX_Matrix matrix = ((FSPDFFreeTextImp*)(FSFreeText*)m_pAnnot)->getMatrix("Matrix");
		fontSize = matrix.TransformDistance(fontSize);
	}else 
		fontSize = rect.Height();
	rect.right = rect.left + rect.Width()+ fontSize * tan( FX_FLOAT(italicAngle) * 3.14159265358979f / 180.0f);
	return FPDFW_ERR_Succeeded;
}

CFX_ByteString	CFPWL_Edit::GetTextAppearanceStream(const CPDF_Point & ptOffset)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	return ((CPWL_Edit*)m_pWnd)->GetTextAppearanceStream(ptOffset);
}

void CFPWL_Edit::SetRichText(FX_LPCWSTR xml, FX_LPCWSTR ds /* = NULL */, FX_BOOL bAutoSetAgin /* = TRUE */)
{
	if (!m_pWnd) return ;

	((CPWL_Edit*)m_pWnd)->SetRichText(xml, ds, bAutoSetAgin);
}

void CFPWL_Edit::SetFillerNotify(IFSPDF_Widget_Notify* pNotify)
{
	if (!m_pWnd) return ;

	((CPWL_Edit*)m_pWnd)->SetFillerNotify((IPWL_Filler_Notify*)pNotify);
}
