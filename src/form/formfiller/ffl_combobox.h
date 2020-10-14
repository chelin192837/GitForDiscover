#ifndef _FFL_COMBOBOX_H_
#define _FFL_COMBOBOX_H_

struct CFFL_ComboBoxState
{
	FX_INT32 nSelIndex;

	FX_INT32 nEditSelStart;
	FX_INT32 nEditSelCount;
	CFX_WideString sValue;
};

class CFFL_ComboBoxCtrl : public CFFL_Widget
{
public:
	CFFL_ComboBoxCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_ComboBoxCtrl();

public:
	virtual IFSPDF_Widget*	InitWidget();
	virtual void			FillCreateParam(CFSPDF_WidgetProperties& prop);

	virtual FX_BOOL			OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL			OnSetFocus(FS_DWORD nFlags);
	virtual FX_BOOL			OnKillFocus(FS_DWORD nFlag);
	virtual void			OnKeyStroke(FX_BOOL bKeyDown, FS_DWORD nFlag);

	virtual FX_BOOL			IsDataChanged(FSPDFPage *pPageView);
	virtual void			SaveData(FSPDFPage* pPageView);
	virtual	void			SaveState(FSPDFPage* pPageView);
	virtual void			RestoreState(FSPDFPage* pPageView);

	virtual void			QueryWherePopup(void* pPrivateData, FX_FLOAT fPopupMin,FX_FLOAT fPopupMax, FX_INT32 & nRet, FX_FLOAT & fPopupRet);

public:
	IFSPDF_ComboBox*	GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfInexist = TRUE) {return (IFSPDF_ComboBox*)CFFL_Widget::GetWidget(pPageView, bNewIfInexist);}

// private:
// 	CFX_WideString				GetSelectExportText();
public:
	//virtual void			GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa);
	virtual void			SetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa);
	//virtual FX_BOOL			IsActionDataChanged(CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& faOld, const FSPDF_FieldActionParam& faNew);

	//virtual IFSPDF_Widget*	ResetPDFWindow(FSPDFPage* pPageView, FX_BOOL bRestoreValue);

	//virtual void				OnAddUndo(CPWL_Edit* pEdit);
	//virtual void				DoPaste(CPDFSDK_Document* pDocument); 
	//virtual void				DoCut(CPDFSDK_Document* pDocument); 
	//virtual void				DoCopy(CPDFSDK_Document* pDocument); 
	//virtual FX_BOOL			CanPaste(CPDFSDK_Document* pDocument); 
	//virtual FX_BOOL			CanCut(CPDFSDK_Document* pDocument); 
	//virtual FX_BOOL			CanCopy(CPDFSDK_Document* pDocument);  
	//virtual void				OnKillFocus(CPWL_Wnd* pWnd);

protected:
	CFFL_ComboBoxState			m_State;


};

#endif // _FFL_COMBOBOX_H_
