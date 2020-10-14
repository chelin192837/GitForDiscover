#include "../inc/fpdf_define.h"
#include "ifpdf_widget.h"
#include "fpwl_widget.h"
#include "fpwl_pushbutton.h"
#include "fpwl_checkbox.h"
#include "fpwl_radiobutton.h"
#include "fpwl_edit.h"
#include "fpwl_combobox.h"
#include "fpwl_listbox.h"
#include "fpwl_label.h"
#include "fpwl_image.h"
#include "fpwl_signature.h"
#include "./pdfwindow/include/fx_systemhandler.h"

CFSPDF_WidgetProperties::~CFSPDF_WidgetProperties() {
//     if (m_pSystemHandler) {
//         m_pSystemHandler->Release();
//         m_pSystemHandler = NULL;
//     }
}

void IFSPDF_Widget::Release()
{
	((CPWL_Widget*)this)->Release();
}

FPDFW_ERR IFSPDF_Widget::Initialize(FSAnnot*	pAnnot, CFSPDF_WidgetProperties* pProperties)
{
	return ((CPWL_Widget*)this)->Initialize(pAnnot, pProperties);
}

FPDFW_ERR IFSPDF_Widget::Finalize()
{
	return ((CPWL_Widget*)this)->Finalize();
}

FPDFW_ERR IFSPDF_Widget::GetWidgetRect(CFX_FloatRect &rect)
{
	return ((CPWL_Widget*)this)->GetWidgetRect(rect);
}

FPDFW_ERR IFSPDF_Widget::SetWidgetRect(const CFX_FloatRect &rect)
{
	return ((CPWL_Widget*)this)->SetWidgetRect(rect);
}

FPDFW_ERR IFSPDF_Widget::GetClientRect(CFX_FloatRect &rect)
{
	return ((CPWL_Widget*)this)->GetClientRect(rect);
}

FX_DWORD IFSPDF_Widget::GetStyles()
{
	return ((CPWL_Widget*)this)->GetStyles();
}

FPDFW_ERR IFSPDF_Widget::ModifyStyles(FX_DWORD dwStylesAdded, FX_DWORD dwStylesRemoved)
{
	return ((CPWL_Widget*)this)->ModifyStyles(dwStylesAdded, dwStylesRemoved);
}

FX_DWORD IFSPDF_Widget::GetStylesEx()
{
	return ((CPWL_Widget*)this)->GetStylesEx();
}
FPDFW_ERR IFSPDF_Widget::ModifyStylesEx(FX_DWORD dwStylesExAdded, FX_DWORD dwStylesExRemoved)
{
	return ((CPWL_Widget*)this)->ModifyStylesEx(dwStylesExAdded, dwStylesExRemoved);
}

FX_DWORD IFSPDF_Widget::GetStates()
{
	return ((CPWL_Widget*)this)->GetStates();
}

FPDFW_ERR IFSPDF_Widget::SetStates(FX_DWORD dwStates, FX_BOOL bSet)
{
	return ((CPWL_Widget*)this)->SetStates(dwStates, bSet);
}

FPDFW_ERR IFSPDF_Widget::SetText(const CFX_WideString &wsText)
{
	return ((CPWL_Widget*)this)->SetText(wsText);
}

FPDFW_ERR IFSPDF_Widget::Update()
{
	return ((CPWL_Widget*)this)->Update();
}

FPDFW_ERR IFSPDF_Widget::LockUpdate()
{
	return ((CPWL_Widget*)this)->LockUpdate();
}

FPDFW_ERR IFSPDF_Widget::UnlockUpdate()
{
	return ((CPWL_Widget*)this)->UnlockUpdate();
}

FX_DWORD IFSPDF_Widget::HitTest(FX_FLOAT fx, FX_FLOAT fy)
{
	return ((CPWL_Widget*)this)->HitTest(fx, fy);
}

FPDFW_ERR IFSPDF_Widget::DrawWidget(CFX_RenderDevice *pDevice, const CFX_Matrix *pMatrix)
{
	return ((CPWL_Widget*)this)->DrawWidget(pDevice, pMatrix);
}

FX_BOOL IFSPDF_Widget::OnKeyDown(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	return ((CPWL_Widget*)this)->OnKeyDown(nKeyCode, nFlags);
}

FX_BOOL IFSPDF_Widget::OnKeyUp(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	return ((CPWL_Widget*)this)->OnKeyUp(nKeyCode, nFlags);
}

FX_BOOL IFSPDF_Widget::OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	return ((CPWL_Widget*)this)->OnChar(nKeyCode, nFlags);
}

FX_BOOL IFSPDF_Widget::OnString(FX_WSTR string, FS_DWORD nFlags)
{
	return ((CPWL_Widget*)this)->OnString(string, nFlags);
}

FX_BOOL IFSPDF_Widget::OnLButtonDown(FS_DWORD nFlags,const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnLButtonDown(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnLButtonUp(FS_DWORD nFlags, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnLButtonUp(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnLButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnLButtonDblClk(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnMouseMove(FS_DWORD nFlags, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnMouseMove(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnRButtonDown(FS_DWORD nFlags, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnRButtonDown(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnRButtonUp(FS_DWORD nFlags, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnRButtonUp(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnRButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnRButtonDblClk(nFlags, point);
}

FX_BOOL IFSPDF_Widget::OnMouseWheel(FS_DWORD nFlags, short zDelta, const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnMouseWheel(nFlags, zDelta, point);
}

FX_BOOL IFSPDF_Widget::OnMouseHover(const CFX_PointF& point)
{
	return ((CPWL_Widget*)this)->OnMouseHover(point);
}

void IFSPDF_Widget::SetFocus()
{
	((CPWL_Widget*)this)->SetFocus();
}

void IFSPDF_Widget::KillFocus()
{
	((CPWL_Widget*)this)->KillFocus();
}

FPDFW_ERR IFSPDF_Widget::GetFocusRect(CFX_FloatRect &rect)
{
	return ((CPWL_Widget*)this)->GetFocusRect(rect);
}

CFX_ByteString IFSPDF_Widget::GetAppearanceStream()
{
	return ((CPWL_Widget*)this)->GetAppearanceStream();
}

FPDFW_ERR IFSPDF_Widget::ResetAppearance(CPDF_FormControl* pControl,FX_LPCWSTR sValue)
{
	return CPWL_Widget::ResetAppearance(pControl, sValue);
}

IFSPDF_PushButton* IFSPDF_PushButton::Create()
{
	return (IFSPDF_PushButton*)FX_NEW CFPWL_PushButton();
}

IFSPDF_CheckBox* IFSPDF_CheckBox::Create()
{
	return (IFSPDF_CheckBox*)FX_NEW CFPWL_CheckBox();
}

FX_INT32 IFSPDF_CheckBox::GetCheckState()
{
	return ((CFPWL_CheckBox*)this)->GetCheckState();
}

FPDFW_ERR IFSPDF_CheckBox::SetCheckState(FX_INT32 iCheck)
{
	return ((CFPWL_CheckBox*)this)->SetCheckState(iCheck);
}

IFSPDF_RadioButton* IFSPDF_RadioButton::Create()
{
	return (IFSPDF_RadioButton*)FX_NEW CFPWL_RadioButton();
}

FX_INT32 IFSPDF_RadioButton::GetCheckState()
{
	return ((CFPWL_RadioButton*)this)->GetCheckState();
}

FPDFW_ERR IFSPDF_RadioButton::SetCheckState(FX_INT32 iCheck)
{
	return ((CFPWL_RadioButton*)this)->SetCheckState(iCheck);
}

IFSPDF_Edit*	IFSPDF_Edit::Create()
{
	return (IFSPDF_Edit*)FX_NEW CFPWL_Edit();
}

FPDFW_ERR	IFSPDF_Edit::SetText(const CFX_WideString &wsText)
{
	return ((CFPWL_Edit*)this)->SetText(wsText);
}

FX_INT32	IFSPDF_Edit::GetTextLength() const
{
	return ((CFPWL_Edit*)this)->GetTextLength();
}

FPDFW_ERR	IFSPDF_Edit::GetText(CFX_WideString &wsText, FX_INT32 nStart, FX_INT32 nCount) const
{
	return ((CFPWL_Edit*)this)->GetText(wsText, nStart, nCount);
}

FPDFW_ERR	IFSPDF_Edit::ClearText()
{
	return ((CFPWL_Edit*)this)->ClearText();
}

FPDFW_ERR	IFSPDF_Edit::AddSelRange(FX_INT32 nStart, FX_INT32 nCount)
{
	return ((CFPWL_Edit*)this)->AddSelRange(nStart, nCount);
}

FX_INT32	IFSPDF_Edit::CountSelRanges()
{
	return ((CFPWL_Edit*)this)->CountSelRanges();
}

FX_INT32	IFSPDF_Edit::GetSelRange(FX_INT32& nStart, FX_INT32 &nEnd)
{
	return ((CFPWL_Edit*)this)->GetSelRange(nStart, nEnd);
}

FX_INT32	IFSPDF_Edit::SetSelRange(FX_INT32 nStart, FX_INT32 nEnd)
{
	return ((CFPWL_Edit*)this)->SetSelRange(nStart, nEnd);
}

FPDFW_ERR	IFSPDF_Edit::ClearSelections()
{
	return ((CFPWL_Edit*)this)->ClearSelections();
}

FPDFW_ERR	IFSPDF_Edit::SetCharArray(FX_INT32 nCharArray)
{
	return ((CFPWL_Edit*)this)->SetCharArray(nCharArray);
}

FX_INT32	IFSPDF_Edit::GetLimit()
{
	return ((CFPWL_Edit*)this)->GetLimit();
}

FPDFW_ERR	IFSPDF_Edit::SetLimit(FX_INT32 nLimit)
{
	return ((CFPWL_Edit*)this)->SetLimit(nLimit);
}

FPDFW_ERR	IFSPDF_Edit::SetAliasChar(FX_WCHAR wAlias)
{
	return ((CFPWL_Edit*)this)->SetAliasChar(wAlias);
}

FPDFW_ERR	IFSPDF_Edit::SetFormatString(const CFX_WideString &wsFormat)
{
	return ((CFPWL_Edit*)this)->SetFormatString(wsFormat);
}

FPDFW_ERR	IFSPDF_Edit::Insert(FX_INT32 nStart, FX_LPCWSTR lpText, FX_INT32 nLen)
{
	return ((CFPWL_Edit*)this)->Insert(nStart, lpText, nLen);
}

FPDFW_ERR	IFSPDF_Edit::DeleteSelections()
{
	return ((CFPWL_Edit*)this)->DeleteSelections();
}

FPDFW_ERR	IFSPDF_Edit::DeleteRange(FX_INT32 nStart, FX_INT32 nCount)
{
	return ((CFPWL_Edit*)this)->DeleteRange(nStart, nCount);
}

FPDFW_ERR	IFSPDF_Edit::ReplaceSelections(const CFX_WideStringC &wsReplace)
{
	return ((CFPWL_Edit*)this)->ReplaceSelections(wsReplace);
}

FPDFW_ERR	IFSPDF_Edit::Replace(FX_INT32 nStart, FX_INT32 nLen, const CFX_WideStringC &wsReplace)
{
	return ((CFPWL_Edit*)this)->Replace(nStart, nLen, wsReplace);
}

FPDFW_ERR	IFSPDF_Edit::DoClipboard(FX_INT32 iCmd)
{
	return ((CFPWL_Edit*)this)->DoClipboard(iCmd);
}

FX_BOOL		IFSPDF_Edit::Redo(FX_BSTR bsRecord)
{
	return ((CFPWL_Edit*)this)->Redo(bsRecord);
}

FX_BOOL		IFSPDF_Edit::Undo(FX_BSTR bsRecord)
{
	return ((CFPWL_Edit*)this)->Undo(bsRecord);
}

FPDFW_ERR	IFSPDF_Edit::SetHorzScale(FX_INT32 nHorzScale)
{
	return ((CFPWL_Edit*)this)->SetHorzScale(nHorzScale);
}

FPDFW_ERR	IFSPDF_Edit::SetCharSpace(FX_FLOAT fCharSpace)
{
	return ((CFPWL_Edit*)this)->SetCharSpace(fCharSpace);
}

FPDFW_ERR	IFSPDF_Edit::SetLineLeading(FX_FLOAT fLineLeading)
{
	return ((CFPWL_Edit*)this)->SetLineLeading(fLineLeading);
}

FPDFW_ERR	IFSPDF_Edit::GetContentRect(CFX_FloatRect &rect)
{
	return ((CFPWL_Edit*)this)->GetContentRect(rect);
}

CFX_ByteString	IFSPDF_Edit::GetTextAppearanceStream(const CPDF_Point & ptOffset)
{
	return ((CFPWL_Edit*)this)->GetTextAppearanceStream(ptOffset);
}

void	IFSPDF_Edit::SetFillerNotify(IFSPDF_Widget_Notify* pNotify)
{
	if (!pNotify) return;

	((CFPWL_Edit*)this)->SetFillerNotify(pNotify);
}

void  IFSPDF_Edit::SetTextMatrix(CPDF_Matrix& mtText)
{
	((CFPWL_Edit*)this)->SetTextMatrix(mtText);
}

void IFSPDF_Edit::SetRichText(FX_LPCWSTR xml, FX_LPCWSTR ds /* = NULL */, FX_BOOL bAutoSetAgin /* = TRUE */)
{
	((CFPWL_Edit*)this)->SetRichText(xml, ds, bAutoSetAgin);
}

IFSPDF_ListBox* IFSPDF_ListBox::Create()
{
	return (IFSPDF_ListBox*)FX_NEW CFPWL_ListBox();
}

FPDFW_HLISTITEM	IFSPDF_ListBox::AddString(FX_WSTR wsAdd, FX_BOOL bSelect)
{
	return ((CFPWL_ListBox*)this)->AddString(wsAdd, bSelect);
}

FX_BOOL			IFSPDF_ListBox::DeleteString(FPDFW_HLISTITEM hItem)
{
	return ((CFPWL_ListBox*)this)->DeleteString(hItem);
}

FX_BOOL			IFSPDF_ListBox::DeleteAll()
{
	return ((CFPWL_ListBox*)this)->DeleteAll();
}

FX_INT32		IFSPDF_ListBox::CountSelItems()
{
	return ((CFPWL_ListBox*)this)->CountSelItems();
}

FPDFW_HLISTITEM	IFSPDF_ListBox::GetSelItem(FX_INT32 nIndexSel)
{
	return ((CFPWL_ListBox*)this)->GetSelItem(nIndexSel);
}

FPDFW_ERR		IFSPDF_ListBox::SetSelItem(FPDFW_HLISTITEM hItem, FX_BOOL bSelect)
{
	return ((CFPWL_ListBox*)this)->SetSelItem(hItem, bSelect);
}

FPDFW_ERR		IFSPDF_ListBox::GetItemText(FPDFW_HLISTITEM hItem, CFX_WideString &wsText)
{
	return ((CFPWL_ListBox*)this)->GetItemText(hItem, wsText);
}

FPDFW_ERR		IFSPDF_ListBox::GetScrollPos(FX_FLOAT &fPos, FX_BOOL bVert)
{
	return ((CFPWL_ListBox*)this)->GetScrollPos(fPos);
}

FPDFW_ERR		IFSPDF_ListBox::SetItemHeight(FX_FLOAT fItemHeight)
{
	return ((CFPWL_ListBox*)this)->SetItemHeight(fItemHeight);
}

FPDFW_HLISTITEM	IFSPDF_ListBox::GetFocusItem()
{
	return ((CFPWL_ListBox*)this)->GetFocusItem();
}

FPDFW_ERR	IFSPDF_ListBox::SetFocusItem(FPDFW_HLISTITEM hItem)
{
	return ((CFPWL_ListBox*)this)->SetFocusItem(hItem);
}

FX_INT32		IFSPDF_ListBox::CountItems()
{
	return ((CFPWL_ListBox*)this)->CountItems();
}

FPDFW_HLISTITEM	IFSPDF_ListBox::GetItem(FX_INT32 nIndex)
{
	return ((CFPWL_ListBox*)this)->GetItem(nIndex);
}

FX_INT32 IFSPDF_ListBox::GetItemIndex(FPDFW_HLISTITEM hItem)
{
	return ((CFPWL_ListBox*)this)->GetItemIndex(hItem);
}

FPDFW_ERR		IFSPDF_ListBox::SetItemString(FPDFW_HLISTITEM hItem, FX_WSTR wsText)
{
	return ((CFPWL_ListBox*)this)->SetItemString(hItem, wsText);
}

FPDFW_ERR		IFSPDF_ListBox::GetItemString(FPDFW_HLISTITEM hItem, CFX_WideString &wsText)
{
	return ((CFPWL_ListBox*)this)->GetItemString(hItem, wsText);
}

FPDFW_ERR		IFSPDF_ListBox::SetItemData(FPDFW_HLISTITEM hItem, FX_LPVOID pData)
{
	return ((CFPWL_ListBox*)this)->SetItemData(hItem, pData);
}

FX_LPVOID		IFSPDF_ListBox::GetItemData(FPDFW_HLISTITEM hItem)
{
	return ((CFPWL_ListBox*)this)->GetItemData(hItem);
}

FPDFW_HLISTITEM	IFSPDF_ListBox::GetTopVisibleItem()
{
	return ((CFPWL_ListBox*)this)->GetTopVisibleItem();
}

FPDFW_ERR	IFSPDF_ListBox::SetTopVisibleItem(FPDFW_HLISTITEM hItem)
{
	return ((CFPWL_ListBox*)this)->SetTopVisibleItem(hItem);
}

FPDFW_HLISTITEM	IFSPDF_ListBox::GetItemAtPoint(FX_FLOAT fx, FX_FLOAT fy)
{
	return ((CFPWL_ListBox*)this)->GetItemAtPoint(fx, fy);
}

FX_DWORD		IFSPDF_ListBox::GetItemStates(FPDFW_HLISTITEM hItem)
{
	return ((CFPWL_ListBox*)this)->GetItemStates(hItem);
}

void	IFSPDF_ListBox::SetFillerNotify(IFSPDF_Widget_Notify* pNotify)
{
	((CFPWL_ListBox*)this)->SetFillerNotify(pNotify);
}

IFSPDF_ComboBox*	IFSPDF_ComboBox::Create()
{
	return (IFSPDF_ComboBox*)FX_NEW CFPWL_ComboBox();
}

FX_INT32	IFSPDF_ComboBox::AddString(FX_WSTR wsText)
{
	return ((CFPWL_ComboBox*)this)->AddString(wsText);
}

FX_INT32	IFSPDF_ComboBox::AddString(FX_WSTR wsText, CFX_DIBitmap *pIcon)
{
	return ((CFPWL_ComboBox*)this)->AddString(wsText, pIcon);
}

FX_INT32	IFSPDF_ComboBox::RemoveAt(FX_INT32 iIndex)
{
	return ((CFPWL_ComboBox*)this)->RemoveAt(iIndex);
}

FX_INT32    IFSPDF_ComboBox::RemoveAll()
{
	return ((CFPWL_ComboBox*)this)->RemoveAll();
}

FX_INT32	IFSPDF_ComboBox::CountItems()
{
	return ((CFPWL_ComboBox*)this)->CountItems();
}

FPDFW_ERR	IFSPDF_ComboBox::GetTextByIndex(FX_INT32 iIndex, CFX_WideString &wsText)
{
	return ((CFPWL_ComboBox*)this)->GetTextByIndex(iIndex, wsText);
}

FX_INT32	IFSPDF_ComboBox::GetCurSel()
{
	return ((CFPWL_ComboBox*)this)->GetCurSel();
}

FPDFW_ERR	IFSPDF_ComboBox::SetCurSel(FX_INT32 iSel)
{
	return ((CFPWL_ComboBox*)this)->SetCurSel(iSel);
}

FPDFW_ERR	IFSPDF_ComboBox::SetText(const CFX_WideString &wsText)
{
	return ((CFPWL_ComboBox*)this)->SetText(wsText);
}

FX_INT32	IFSPDF_ComboBox::GetEditTextLength() const
{
	return ((CFPWL_ComboBox*)this)->GetEditTextLength();
}
FPDFW_ERR	IFSPDF_ComboBox::GetEditText(CFX_WideString &wsText, FX_INT32 nStart, FX_INT32 nCount) const
{
	return ((CFPWL_ComboBox*)this)->GetEditText(wsText, nStart, nCount);
}

FPDFW_ERR	IFSPDF_ComboBox::AddEditSelRange(FX_INT32 nStart, FX_INT32 nCount)
{
	return ((CFPWL_ComboBox*)this)->AddEditSelRange(nStart, nCount);
}

FX_INT32	IFSPDF_ComboBox::GetEditSelRange(FX_INT32 nIndex, FX_INT32 &nStart)
{
	return ((CFPWL_ComboBox*)this)->GetEditSelRange(nIndex, nStart);
}

FX_INT32	IFSPDF_ComboBox::CountEditSelRanges()
{
	return ((CFPWL_ComboBox*)this)->CountEditSelRanges();
}

FPDFW_ERR	IFSPDF_ComboBox::ClearEditSelections()
{
	return ((CFPWL_ComboBox*)this)->ClearEditSelections();
}


FX_INT32	IFSPDF_ComboBox::GetEditLimit()
{
	return ((CFPWL_ComboBox*)this)->GetEditLimit();
}

FPDFW_ERR	IFSPDF_ComboBox::SetEditLimit(FX_INT32 nLimit)
{
	return ((CFPWL_ComboBox*)this)->SetEditLimit(nLimit);
}

FPDFW_ERR	IFSPDF_ComboBox::EditDoClipboard(FX_INT32 iCmd)
{
	return ((CFPWL_ComboBox*)this)->EditDoClipboard(iCmd);
}

FX_BOOL		IFSPDF_ComboBox::EditRedo(FX_BSTR bsRecord)
{
	return ((CFPWL_ComboBox*)this)->EditRedo(bsRecord);
}

FX_BOOL		IFSPDF_ComboBox::EditUndo(FX_BSTR bsRecord)
{
	return ((CFPWL_ComboBox*)this)->EditUndo(bsRecord);
}

FPDFW_ERR	IFSPDF_ComboBox::SetMaxListHeight(FX_FLOAT fMaxHeight)
{
	return ((CFPWL_ComboBox*)this)->SetMaxListHeight(fMaxHeight);
}

FPDFW_ERR	IFSPDF_ComboBox::SetItemData(FX_INT32 iIndex, FX_LPVOID pData)
{
	return ((CFPWL_ComboBox*)this)->SetItemData(iIndex, pData);
}

FX_LPVOID	IFSPDF_ComboBox::GetItemData(FX_INT32 iIndex)
{
	return ((CFPWL_ComboBox*)this)->GetItemData(iIndex);
}

void	IFSPDF_ComboBox::SetFillerNotify(IFSPDF_Widget_Notify* pNotify)
{
	if (!pNotify) return;

	((CFPWL_ComboBox*)this)->SetFillerNotify(pNotify);
}

FPDFW_ERR IFSPDF_ComboBox::SetSelRange(FX_INT32 nStart, FX_INT32 nEnd)
{
	return ((CFPWL_ComboBox*)this)->SetSelRange(nStart, nEnd);
}

FPDFW_ERR IFSPDF_ComboBox::ReplaceSel(const CFX_WideStringC &wsReplace)
{
	return ((CFPWL_ComboBox*)this)->ReplaceSel(wsReplace);
}

IFSPDF_Label* IFSPDF_Label::Create()
{
	return (IFSPDF_Label*)FX_NEW CFPWL_Label();
}

FPDFW_ERR IFSPDF_Label::SetText(const CFX_WideString wsText)
{
	return ((CFPWL_Label*)this)->SetText(wsText);
}

IFSPDF_Image* IFSPDF_Image::Create()
{
	return (IFSPDF_Image*)FX_NEW CFPWL_Image();
}

FPDFW_ERR IFSPDF_Image::SetImageStream(CPDF_Stream * pStream)
{
	return ((CFPWL_Image*)this)->SetImageStream(pStream);
}

IFSPDF_Signature* IFSPDF_Signature::Create()
{
	return (IFSPDF_Signature*)FX_NEW CFPWL_Signature();
}

FPDFW_ERR IFSPDF_Signature::SetShowText(FX_BOOL bShowText)
{
	return ((CFPWL_Signature*)this)->SetShowText(bShowText);
}

FPDFW_ERR IFSPDF_Signature::SetShowImage(FX_BOOL bShowImage)
{
	return ((CFPWL_Signature*)this)->SetShowImage(bShowImage);
}

FPDFW_ERR IFSPDF_Signature::SetText(const CFX_WideString text)
{
	return ((CFPWL_Signature*)this)->SetText(text);
}

FPDFW_ERR IFSPDF_Signature::SetImageStream(CPDF_Stream* pStream, FX_LPCSTR sImageAlias)
{
	return ((CFPWL_Signature*)this)->SetImageStream(pStream, sImageAlias);
}

FPDFW_ERR IFSPDF_Signature::SetDescription(const CFX_WideString description)
{
	return ((CFPWL_Signature*)this)->SetDescription(description);
}

