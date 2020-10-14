#ifndef _FFL_LISTBOX_H_
#define _FFL_LISTBOX_H_

class CFFL_ListBoxCtrl : public CFFL_Widget
{
public:
	CFFL_ListBoxCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_ListBoxCtrl();

public:
	virtual IFSPDF_Widget*	InitWidget();

	virtual void			FillCreateParam(CFSPDF_WidgetProperties& prop);

	virtual FX_BOOL			OnChar(FS_DWORD nChar, FS_DWORD nFlags);
	
	virtual FX_BOOL			IsDataChanged(FSPDFPage* pPageView);
	virtual void			SaveData(FSPDFPage* pPageView);
	
	virtual void			SaveState(FSPDFPage* pPageView);
	virtual void			RestoreState(FSPDFPage* pPageView);
	
	virtual void			OnKeyStroke(FX_BOOL bKeyDown, FX_DWORD nFlag);
    virtual FX_BOOL			OnSetFocus(FS_DWORD nFlags);
    virtual FX_BOOL			OnKillFocus(FS_DWORD nFlag);

//	virtual IFSPDF_Widget*	ResetPDFWindow(FSPDFPage* pPageView, FX_BOOL bRestoreValue);

//	virtual void			GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa);
//	virtual void			SetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa);

public:
	IFSPDF_ListBox*	GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfInexist = TRUE) {return (IFSPDF_ListBox*)CFFL_Widget::GetWidget(pPageView, bNewIfInexist);}

protected:
	CFX_MapPtrTemplate<int, void*>	m_OriginSelections;
	CFX_ArrayTemplate<int>			m_State;
};

#endif // _FFL_LISTBOX_H_
