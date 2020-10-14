#ifndef _FPWL_COMBOBOX_IMP_H_
#define _FPWL_COMBOBOX_IMP_H_

#include "fpwl_widget.h"
//*****************************************************************************
//* 
//*****************************************************************************
class CFPWL_ComboBox : public CPWL_Widget
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FX_INT32	AddString(FX_WSTR wsText);
	FX_INT32	AddString(FX_WSTR wsText, CFX_DIBitmap *pIcon);
	FX_INT32	RemoveAt(FX_INT32 iIndex);
	FX_INT32    RemoveAll();
	FX_INT32	CountItems();
	FPDFW_ERR	GetTextByIndex(FX_INT32 iIndex, CFX_WideString &wsText);

	FX_INT32	GetCurSel();
	FPDFW_ERR	SetCurSel(FX_INT32 iSel);

	FPDFW_ERR	SetText(const CFX_WideString &wsText);
	FX_INT32	GetEditTextLength() const;
	FPDFW_ERR	GetEditText(CFX_WideString &wsText, FX_INT32 nStart = 0, FX_INT32 nCount = -1) const;

	FPDFW_ERR	AddEditSelRange(FX_INT32 nStart, FX_INT32 nCount = -1);
	FX_INT32	CountEditSelRanges();
	FX_INT32	GetEditSelRange(FX_INT32 nIndex, FX_INT32 &nStart);
	FPDFW_ERR	ClearEditSelections();

	FX_INT32	GetEditLimit();
	FPDFW_ERR	SetEditLimit(FX_INT32 nLimit);
	FPDFW_ERR	EditDoClipboard(FX_INT32 iCmd);/* 1 Copy; 2 Cut; 3 Paste*/
	FX_BOOL		EditRedo(FX_BSTR bsRecord);
	FX_BOOL		EditUndo(FX_BSTR bsRecord);
	FPDFW_ERR	SetMaxListHeight(FX_FLOAT fMaxHeight);

	FPDFW_ERR	SetItemData(FX_INT32 iIndex, FX_LPVOID pData);
	FX_LPVOID	GetItemData(FX_INT32 iIndex);

	void		SetFillerNotify(IFSPDF_Widget_Notify* pNotify);

	FPDFW_ERR	SetSelRange(FX_INT32 nStart, FX_INT32 nEnd);
	FPDFW_ERR	ReplaceSel(const CFX_WideStringC &wsReplace);

	CFPWL_ComboBox();
	virtual ~CFPWL_ComboBox();
};


#endif //_FPWL_COMBOBOX_IMP_H_


