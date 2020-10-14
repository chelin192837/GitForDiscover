#ifndef _FPWL_EDIT_IMP_H_
#define _FPWL_EDIT_IMP_H_

#include "fpwl_widget.h"
#include "fpwl_pushbutton.h"
//*****************************************************************************
//* 
//*****************************************************************************
class CFPWL_Edit : public CPWL_Widget
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FPDFW_ERR		SetText(const CFX_WideString &wsText);
	FPDFW_ERR		SetTextMatrix(const CPDF_Matrix& mtText);
	FX_INT32		GetTextLength() const;
	FPDFW_ERR		GetText(CFX_WideString &wsText, FX_INT32 nStart = 0, FX_INT32 nCount = -1) const;
	FPDFW_ERR		ClearText();

	FPDFW_ERR		AddSelRange(FX_INT32 nStart, FX_INT32 nCount = -1);
	FX_INT32		CountSelRanges();
	FX_INT32		GetSelRange(FX_INT32& nStart, FX_INT32 &nEnd);
	FX_INT32		SetSelRange(FX_INT32 nStart, FX_INT32 nEnd);
	FPDFW_ERR		ClearSelections();

	FPDFW_ERR		SetCharArray(FX_INT32 nCharArray);
	FX_INT32		GetLimit();
	FPDFW_ERR		SetLimit(FX_INT32 nLimit);
	FPDFW_ERR		SetAliasChar(FX_WCHAR wAlias); 
	FPDFW_ERR		SetFormatString(const CFX_WideString &wsFormat);

	FPDFW_ERR		Insert(FX_INT32 nStart, FX_LPCWSTR lpText, FX_INT32 nLen);
	FPDFW_ERR		DeleteSelections();
	FPDFW_ERR		DeleteRange(FX_INT32 nStart, FX_INT32 nCount = -1);
	
	FPDFW_ERR		ReplaceSelections(const CFX_WideStringC &wsReplace);
	FPDFW_ERR		Replace(FX_INT32 nStart, FX_INT32 nLen, const CFX_WideStringC &wsReplace);
	FPDFW_ERR		DoClipboard(FX_INT32 iCmd);/* 1 Copy; 2 Cut; 3 Paste*/

	FX_BOOL		Redo(FX_BSTR bsRecord);
	FX_BOOL		Undo(FX_BSTR bsRecord);

	FPDFW_ERR		SetHorzScale(FX_INT32 nHorzScale);
	FPDFW_ERR		SetCharSpace(FX_FLOAT fCharSpace);
	FPDFW_ERR		SetLineLeading(FX_FLOAT fLineLeading);

	FPDFW_ERR		GetContentRect(CFX_FloatRect &rect);

	CFX_ByteString	GetTextAppearanceStream(const CPDF_Point & ptOffset);

	virtual	void		SetFillerNotify(IFSPDF_Widget_Notify* pNotify);

	void	SetRichText(FX_LPCWSTR xml, FX_LPCWSTR ds = NULL, FX_BOOL bAutoSetAgin = TRUE);

	CFPWL_Edit();
	virtual ~CFPWL_Edit();
};


#endif //_FPWL_EDIT_IMP_H_


