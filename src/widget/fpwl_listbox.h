#ifndef _FPWL_LISTBOX_IMP_H_
#define _FPWL_LISTBOX_IMP_H_

#include "fpwl_widget.h"

//*****************************************************************************
//* 
//*****************************************************************************
class CFPWL_ListBox : public CPWL_Widget
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FPDFW_HLISTITEM				AddString(FX_WSTR wsAdd, FX_BOOL bSelect = FALSE);
	FX_BOOL						DeleteString(FPDFW_HLISTITEM hItem);
	FX_BOOL						DeleteAll();

	FX_INT32					CountSelItems();
	FPDFW_HLISTITEM				GetSelItem(FX_INT32 nIndexSel);
	FPDFW_ERR					SetSelItem(FPDFW_HLISTITEM hItem, FX_BOOL bSelect = TRUE);
	FPDFW_ERR					GetItemText(FPDFW_HLISTITEM hItem, CFX_WideString &wsText);
	FPDFW_ERR					GetScrollPos(FX_FLOAT &fPos, FX_BOOL bVert = TRUE);
	FPDFW_ERR					SetItemHeight(FX_FLOAT fItemHeight);

	FPDFW_HLISTITEM				GetFocusItem();
	FPDFW_ERR					SetFocusItem(FPDFW_HLISTITEM hItem);	

	FX_INT32					CountItems();
	FPDFW_HLISTITEM				GetItem(FX_INT32 nIndex);
	FX_INT32					GetItemIndex(FPDFW_HLISTITEM hItem);
	FPDFW_ERR					SetItemString(FPDFW_HLISTITEM hItem, FX_WSTR wsText);
	FPDFW_ERR					GetItemString(FPDFW_HLISTITEM hItem, CFX_WideString &wsText);

	FPDFW_ERR					SetItemData(FPDFW_HLISTITEM hItem, FX_LPVOID pData);
	FX_LPVOID					GetItemData(FPDFW_HLISTITEM hItem);
		
	FPDFW_HLISTITEM				GetTopVisibleItem();
	FPDFW_ERR					SetTopVisibleItem(FPDFW_HLISTITEM hItem);

	FPDFW_HLISTITEM				GetItemAtPoint(FX_FLOAT fx, FX_FLOAT fy);
	FX_DWORD					GetItemStates(FPDFW_HLISTITEM hItem);

	void						SetFillerNotify(IFSPDF_Widget_Notify* pNotify);

	FX_INT32					m_focusItem;

	

	CFPWL_ListBox();
	virtual ~CFPWL_ListBox();
};


#endif //_FPWL_LISTBOX_IMP_H_


