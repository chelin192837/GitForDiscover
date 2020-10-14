#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "pdfwindow/include/fxedit/fx_edit.h"
#include "pdfwindow/include/pdfwindow/pwl_wnd.h"
#include "pdfwindow/include/pdfwindow/pwl_listbox.h"
#include "fpwl_listbox.h"

CFPWL_ListBox::CFPWL_ListBox()
{

}

CFPWL_ListBox::~CFPWL_ListBox()
{
	
}

void CFPWL_ListBox::Release()
{
	delete this;
}

FPDFW_ERR CFPWL_ListBox::Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	m_pWnd = FX_NEW CPWL_ListBox;
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	((CPWL_ListBox*)m_pWnd)->AttachFFLData(pAnnot);
	CPWL_Widget::Initialize(pAnnot ,pProperties);
	return FPDFW_ERR_Succeeded;
}

FPDFW_HLISTITEM CFPWL_ListBox::AddString(FX_WSTR wsAdd, FX_BOOL bSelect)
{
	if (!m_pWnd) return NULL;

	((CPWL_ListBox*)m_pWnd)->AddString(wsAdd.GetPtr());

	return NULL;
}

FX_BOOL CFPWL_ListBox::DeleteString(FPDFW_HLISTITEM hItem)
{
	if (!m_pWnd) return NULL;

	return TRUE;
}

FX_BOOL CFPWL_ListBox::DeleteAll()
{
	if (!m_pWnd) return FALSE;

	((CPWL_ListBox*)m_pWnd)->ResetContent();
	return TRUE;
}

FX_INT32 CFPWL_ListBox::CountSelItems()
{
	if (!m_pWnd) return 0;

	FX_INT32 count = 0;
	CPWL_ListBox* pListBox = (CPWL_ListBox*)m_pWnd;
	if (!pListBox->IsMultipleSel())
	{
		if (pListBox->GetCurSel() > -1)
		{
			count++;
		}
		return 	count ;
	}

	count = pListBox->GetCount();
	for (FX_INT32 i = 0; i < count; i++)
	{
		if (pListBox->IsItemSelected(i))
			++count;
	}

	return count;		
}

FPDFW_HLISTITEM CFPWL_ListBox::GetSelItem(FX_INT32 nIndexSel)
{
	if (!m_pWnd) return NULL;

	CPWL_ListBox* pListBox = (CPWL_ListBox*)m_pWnd;
	if (!pListBox->IsMultipleSel())
	{
		FX_INT32 curSel = pListBox->GetCurSel();
		if (curSel < 0) return NULL;
		return 	(FPDFW_HLISTITEM)(curSel + 1);
	}
	FX_INT32 indexSel = 0;
	for (FX_INT32 i = 0; i < pListBox->GetCount(); i++)
	{
		if (pListBox->IsItemSelected(i))
		{
			if (nIndexSel == indexSel)
				return FPDFW_HLISTITEM(i + 1);

			++indexSel;
		}
	}

	return NULL;
}

FPDFW_ERR CFPWL_ListBox::SetSelItem(FPDFW_HLISTITEM hItem, FX_BOOL bSelect)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_ListBox::GetItemText(FPDFW_HLISTITEM hItem, CFX_WideString &wsText)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_ListBox::GetScrollPos(FX_FLOAT &fPos, FX_BOOL bVert)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_ListBox::SetItemHeight(FX_FLOAT fItemHeight)
{
	return FPDFW_ERR_Succeeded;
}

FPDFW_HLISTITEM CFPWL_ListBox::GetFocusItem()
{
	if (!m_pWnd) return NULL;

	return (FPDFW_HLISTITEM)m_focusItem;
}

FPDFW_ERR CFPWL_ListBox::SetFocusItem(FPDFW_HLISTITEM hItem)
{
	if (!m_pWnd || !hItem) return FPDFW_ERR_Indefinite;

	m_focusItem = (FS_INT32)(FX_UINTPTR)hItem;

	((CPWL_ListBox*)m_pWnd)->SetCaret(m_focusItem - 1);
	return FPDFW_ERR_Succeeded;
}

FX_INT32 CFPWL_ListBox::CountItems()
{
	if (!m_pWnd) return 0;

	return ((CPWL_ListBox*)m_pWnd)->GetCount();
}

FPDFW_HLISTITEM CFPWL_ListBox::GetItem(FX_INT32 nIndex)
{
	if (!m_pWnd || nIndex < 0) return NULL;

	return (FPDFW_HLISTITEM)(nIndex + 1);
}

FX_INT32	CFPWL_ListBox::GetItemIndex(FPDFW_HLISTITEM hItem)
{
	if (!m_pWnd) return -1;

	return (FX_INT32)(FX_UINTPTR)hItem - 1;
}

FPDFW_ERR CFPWL_ListBox::SetItemString(FPDFW_HLISTITEM hItem, FX_WSTR wsText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;
//	((CPWL_ListBox*)m_pWnd)->SetText(wsText.GetPtr());

	return FPDFW_ERR_Succeeded;
}

FPDFW_ERR CFPWL_ListBox::GetItemString(FPDFW_HLISTITEM hItem, CFX_WideString &wsText)
{
	if (!m_pWnd) return FPDFW_ERR_Indefinite;

	return FPDFW_ERR_Succeeded; 
}

FPDFW_ERR CFPWL_ListBox::SetItemData(FPDFW_HLISTITEM hItem, FX_LPVOID pData)
{
	return FPDFW_ERR_Succeeded;
}

FX_LPVOID CFPWL_ListBox::GetItemData(FPDFW_HLISTITEM hItem)
{
	return NULL;
}

FPDFW_HLISTITEM	CFPWL_ListBox::GetTopVisibleItem()
{
	if (!m_pWnd) return NULL;
	FX_INT32 index = ((CPWL_ListBox*)m_pWnd)->GetTopVisibleIndex();

	return (FPDFW_HLISTITEM)(index + 1);
}

FPDFW_ERR	CFPWL_ListBox::SetTopVisibleItem(FPDFW_HLISTITEM hItem)
{
	if (!m_pWnd || !hItem) return FPDFW_ERR_Indefinite;

	((CPWL_ListBox*)m_pWnd)->SetTopVisibleIndex((FX_INT32)(FX_UINTPTR)hItem - 1);
	return FPDFW_ERR_Succeeded;
}

FPDFW_HLISTITEM CFPWL_ListBox::GetItemAtPoint(FX_FLOAT fx, FX_FLOAT fy)
{
	return NULL;
}

FX_DWORD CFPWL_ListBox::GetItemStates(FPDFW_HLISTITEM hItem)
{
	return 0;
}

void	CFPWL_ListBox::SetFillerNotify(IFSPDF_Widget_Notify* pNotify)
{
	if (!m_pWnd) return ;

	((CPWL_ListBox*)m_pWnd)->SetFillerNotify((IPWL_Filler_Notify*)pNotify);
}
