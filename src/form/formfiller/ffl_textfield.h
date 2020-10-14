#ifndef _FFL_TEXTFIELD_H_
#define _FFL_TEXTFIELD_H_

struct FFL_TextFieldState
{
	int nStart;
	int nEnd;
	CFX_WideString sValue;
};

class CFFL_TextFieldCtrl : public CFFL_Widget
{
public:
	CFFL_TextFieldCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_TextFieldCtrl();

public:
	virtual IFSPDF_Widget*		InitWidget();
	virtual void				FillCreateParam(CFSPDF_WidgetProperties& prop);

	virtual FX_BOOL				IsDataChanged(FSPDFPage* pPageView);
	virtual	void				SaveData(FSPDFPage* pPageView);

	static  CFX_WideString		GetRichTextContent(CFX_WideString wsRichText);


	virtual FX_BOOL				OnChar(FS_DWORD nChar, FS_DWORD nFlags);
	virtual void				GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa);
	virtual void				SetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa);
	virtual FX_BOOL				IsActionDataChanged(CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& faOld, const FSPDF_FieldActionParam& faNew);
	virtual	void				SaveState(FSPDFPage* pPageView);
	virtual void				RestoreState(FSPDFPage* pPageView);
	virtual IFSPDF_Widget*		ResetPDFWindow(FSPDFPage* pPageView, FX_BOOL bRestoreValue); 

	//virtual	FX_BOOL				OnSetText(FSFormControl* pControl, const CFX_WideString &wText, int nFlag);

	virtual FX_BOOL				OnSetFocus	(FS_DWORD nFlag);
	virtual FX_BOOL				OnKillFocus	(FS_DWORD nFlag);
	//virtual void				OnAddUndo(CPWL_Edit* pEdit);
	//virtual void				DoPaste(CPDFSDK_Document* pDocument); 
	//virtual void				DoCut(CPDFSDK_Document* pDocument); 
	//virtual void				DoCopy(CPDFSDK_Document* pDocument); 
	//virtual FX_BOOL			CanPaste(CPDFSDK_Document* pDocument); 
	//virtual FX_BOOL			CanCut(CPDFSDK_Document* pDocument); 
	//virtual FX_BOOL			CanCopy(CPDFSDK_Document* pDocument); 
	//virtual void				OnKillFocus(CPWL_Wnd* pWnd);


public:
	IFSPDF_Edit*	GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfInexist = TRUE) {return (IFSPDF_Edit*)CFFL_Widget::GetWidget(pPageView, bNewIfInexist);}

private:
	FFL_TextFieldState	m_State;
	/*TODO: 
	CFFL_IM_BOX					m_IMBox;
	*/

/*TODO: 

virtual	PWL_CREATEPARAM		GetCreateParam();
virtual CPWL_Wnd*			NewPDFWindow(const PWL_CREATEPARAM& cp, CReader_PageView* pPageView);

virtual FX_BOOL				OnLButtonUp(CReader_PageView *pPageView, CReader_Annot* pAnnot, UINT nFlags, const CPoint& point);
virtual FX_BOOL				OnChar(CReader_Annot* pAnnot, UINT nChar, UINT nFlags);
virtual FX_BOOL				OnSetFocus(CReader_Annot* pAnnot);
virtual FX_BOOL				OnKillFocus(CReader_Annot* pAnnot);

virtual FX_BOOL				IsDataChanged(CReader_PageView* pPageView);
virtual void				SaveData(CReader_PageView* pPageView);

virtual void				GetActionData(CReader_PageView* pPageView, CPDF_AAction::AActionType type,
BA_FieldAction& fa);
virtual void				SetActionData(CReader_PageView* pPageView, CPDF_AAction::AActionType type, 
const BA_FieldAction& fa);
virtual void				SaveState(CReader_PageView* pPageView);
virtual void				RestoreState(CReader_PageView* pPageView);

virtual CPWL_Wnd*			ResetPDFWindow(CReader_PageView* pPageView, FX_BOOL bRestoreValue);

virtual FX_BOOL				IsFieldFull(CReader_PageView* pPageView);

public:
virtual void				OnSetFocus(CPWL_Wnd* pWnd);
virtual void				OnKillFocus(CPWL_Wnd* pWnd);

public:
virtual void				OnAddUndo(CPWL_Edit* pEdit);

public:
virtual FX_BOOL				CanSelectAll(CReader_Document* pDocument); 
virtual FX_BOOL				CanDelete(CReader_Document* pDocument);
virtual FX_BOOL				CanCopy(CReader_Document* pDocument); 
virtual FX_BOOL				CanCut(CReader_Document* pDocument); 
virtual FX_BOOL				CanPaste(CReader_Document* pDocument); 

virtual void				DoSelectAll(CReader_Document* pDocument);
virtual void				DoDelete(CReader_Document* pDocument);
virtual void				DoCopy(CReader_Document* pDocument); 
virtual void				DoCut(CReader_Document* pDocument); 
virtual void				DoPaste(CReader_Document* pDocument);  
virtual void				DoDeselect(CReader_Document* pDocument); 

public:
static void					DrawScrollMark(CReader_PageView* pPageView, HDC hDC, CBF_Widget* pWidget,
CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device,
const CRect& rcWindow, DWORD dwFlags);

private:
static FX_BOOL				IsTextOverflow(CReader_PageView* pPageView, CBF_Widget* pWidget);
*/

};


#endif // _FFL_TEXTFIELD_H_
