#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/pdfwindow/pwl_combobox.h"
#include "fpwl_combobox.h"

CFPWL_ComboBox::CFPWL_ComboBox() 
{
	
}

CFPWL_ComboBox::~CFPWL_ComboBox()
{

}

void	CFPWL_ComboBox::Release()
{
	delete this;
}

FPDFW_ERR	CFPWL_ComboBox::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_ComboBox;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ComboBox*)m_pWnd)->AttachFFLData(pAnnot);
	CPWL_Widget::Initialize(pAnnot, pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_ComboBox::AddString(FX_WSTR wsText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ComboBox*)m_pWnd)->AddString(wsText.GetPtr());

	return FPDFW_ERR_Succeeded;
}

//
FX_INT32	CFPWL_ComboBox::AddString(FX_WSTR wsText, CFX_DIBitmap *pIcon)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ComboBox*)m_pWnd)->AddString(wsText.GetPtr());

	return 0;
}

FX_INT32	CFPWL_ComboBox::RemoveAt(FX_INT32 iIndex)
{
	return 0;
}

FX_INT32    CFPWL_ComboBox::RemoveAll()
{
	if (!m_pWnd) return 0;

	((CPWL_ListBox*)*((CPWL_ComboBox*)m_pWnd))->ResetContent();

	return 0;
}

FX_INT32	CFPWL_ComboBox::CountItems()
{
	if (!m_pWnd) return 0;

	return ((CPWL_ListBox*)*((CPWL_ComboBox*)m_pWnd))->GetCount();
}

FPDFW_ERR CFPWL_ComboBox::GetTextByIndex( FX_INT32 iIndex, CFX_WideString &wsText )
{
	return FPDFW_ERR_Succeeded;
}

FX_INT32 CFPWL_ComboBox::GetCurSel()
{
	if (!m_pWnd) return -1;
	
	return ((CPWL_ComboBox*)m_pWnd)->GetSelect();
}

FPDFW_ERR CFPWL_ComboBox::SetCurSel( FX_INT32 iSel )
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	((CPWL_ComboBox*)m_pWnd)->SetSelect(iSel);

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_ComboBox::SetText(const CFX_WideString &wsText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ComboBox*)m_pWnd)->SetText((FX_LPCWSTR)(wsText));

	return FPDFW_ERR_Succeeded;
}

FX_INT32	CFPWL_ComboBox::GetEditTextLength() const
{
	if (!m_pWnd) return 0;

	return ((CPWL_ComboBox*)m_pWnd)->GetText().GetLength();
}

FPDFW_ERR	CFPWL_ComboBox::GetEditText(CFX_WideString &wsText, FX_INT32 nStart, FX_INT32 nCount) const
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	wsText = ((CPWL_ComboBox*)m_pWnd)->GetText();

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_ComboBox::AddEditSelRange(FX_INT32 nStart, FX_INT32 nCount)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ComboBox*)m_pWnd)->SetEditSel(nStart, nStart + nCount);

	return FPDFW_ERR_Succeeded;
}

FX_INT32	CFPWL_ComboBox::GetEditSelRange(FX_INT32 nIndex, FX_INT32 &nStart)
{
	FX_INT32 nEndChar;

	if(CountEditSelRanges() && nIndex == 0)
	{
		((CPWL_ComboBox*)m_pWnd)->GetEditSel(nStart, nEndChar);
		return nEndChar - nStart;
	}
	nStart = -1; //TODO: Check if this is correct
	return 0;
}

FX_INT32	CFPWL_ComboBox::CountEditSelRanges()
{
	FX_INT32 nStartChar, nEndChar;
	
	((CPWL_ComboBox*)m_pWnd)->GetEditSel(nStartChar, nEndChar);
	if(nEndChar <= nStartChar || nStartChar < 0) //TODO: which way does the text engine use? Use one way only.
	{
		return 0;
	}
	return 1;
}

FPDFW_ERR	CFPWL_ComboBox::ClearEditSelections()
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ComboBox*)m_pWnd)->SetEditSel(-1, 0);

	return FPDFW_ERR_Succeeded;
}


FX_INT32	CFPWL_ComboBox::GetEditLimit()
{
	return 0;
}

FPDFW_ERR	CFPWL_ComboBox::SetEditLimit(FX_INT32 nLimit)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Edit* pEdit = (CPWL_Edit*)*((CPWL_ComboBox*)m_pWnd);
	pEdit->SetLimitChar(nLimit);

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_ComboBox::EditDoClipboard(FX_INT32 iCmd)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	CPWL_Edit* pEdit = (CPWL_Edit*)*((CPWL_ComboBox*)m_pWnd);
	switch(iCmd)
	{
	case 1:
		pEdit->CopyText();
		break;
	case 2:
		pEdit->CutText();
		break;
	case 3:
		pEdit->PasteText();
		break;
	default:
		return FPDFW_ERR_Indefinite;
	}

	return FPDFW_ERR_Succeeded;
}

FX_BOOL		CFPWL_ComboBox::EditRedo(FX_BSTR bsRecord)
{
	if (!m_pWnd) return FALSE;
	
	CPWL_Edit* pEdit = (CPWL_Edit*)*((CPWL_ComboBox*)m_pWnd);
	if (pEdit->CanRedo())
		pEdit->Redo();

	return TRUE;

}

FX_BOOL		CFPWL_ComboBox::EditUndo(FX_BSTR bsRecord)
{
	if (!m_pWnd) return FALSE;

	CPWL_Edit* pEdit = (CPWL_Edit*)*((CPWL_ComboBox*)m_pWnd);
	if (pEdit->CanUndo())
		pEdit->Undo();

	return TRUE;
}

FPDFW_ERR	CFPWL_ComboBox::SetMaxListHeight(FX_FLOAT fMaxHeight)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR	CFPWL_ComboBox::SetItemData(FX_INT32 iIndex, FX_LPVOID pData)
{
	return FPDFW_ERR_Succeeded;
}

FX_LPVOID	CFPWL_ComboBox::GetItemData(FX_INT32 iIndex)
{
	return NULL;
}

void		CFPWL_ComboBox::SetFillerNotify(IFSPDF_Widget_Notify* pNotify)
{
	if (!m_pWnd) return ;

	((CPWL_ComboBox*)m_pWnd)->SetFillerNotify((IPWL_Filler_Notify*)pNotify);
}

FPDFW_ERR CFPWL_ComboBox::SetSelRange(FX_INT32 nStart, FX_INT32 nEnd)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	((CPWL_ComboBox*)m_pWnd)->GetEdit()->SetSel(nStart, nEnd);
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_ComboBox::ReplaceSel(const CFX_WideStringC &wsReplace)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
	((CPWL_ComboBox*)m_pWnd)->GetEdit()->ReplaceSel(wsReplace.GetPtr());
	return FPDFW_ERR_Succeeded;
}