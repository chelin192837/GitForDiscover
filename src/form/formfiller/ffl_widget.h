#ifndef _FFL_WIDGET_H_
#define _FFL_WIDGET_H_
#include "../../widget/ifpdf_widget.h"
#include "../../form/fpdf_stactionhandler.h"
#include "../../widget/fpwl_widget.h"

class CFSPDF_WidgetAnnotHandler;

typedef	CFX_MapPtrTemplate<FSPDFPage*, IFSPDF_Widget*> CFFL_PageView2Widget;

class CFFL_Widget : public CFX_Object, protected IFSPDF_Widget_Notify
{
public:
	CFFL_Widget(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_Widget();
	
	FSFormControl*	GetControl() const {return m_pControl;}
	
	IFSPDF_Widget*		GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfInexist = TRUE);
	void				ReleaseWidget(FSPDFPage* pPageView);

protected:
	virtual IFSPDF_Widget* InitWidget() = 0; //implement this in specific child class.
	virtual void		FillCreateParam	(CFSPDF_WidgetProperties& prop); //override this in specific child class.

public: //creation & deletion events
	virtual void		OnCreate();
	virtual void		OnLoad	();
	virtual void		OnRelease();

public: //mouse events
	virtual void		OnMouseEnter	(FSPDFPage *pPageView);
	virtual void		OnMouseExit		(FSPDFPage *pPageView);

public: //mouse events
	virtual FX_BOOL		OnLButtonDown	(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL		OnLButtonUp		(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL		OnLButtonDblClk	(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL		OnMouseMove		(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL		OnMouseWheel	(FSPDFPage *pPageView, FS_DWORD nFlags, short zDelta, const CFX_PointF& point);
	virtual FX_BOOL		OnRButtonDown	(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL		OnRButtonUp		(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL		OnRButtonDblClk	(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	
public: //kbd events
	virtual FX_BOOL		OnKeyDown	(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL		OnKeyUp		(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL		OnChar		(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL		OnString	(FX_WSTR string,	FS_DWORD nFlags);


public: //repaint events
	virtual void		OnDraw			(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags);
	virtual void		OnDrawInactive	(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags);
	
public:	// focus events
	virtual FX_BOOL				OnSetFocus	(FS_DWORD nFlag);
	virtual FX_BOOL				OnKillFocus	(FS_DWORD nFlag);

public: // selection events
	virtual void				OnSelected	();
	virtual void				OnDeSelected();


protected:  // implement IFSPDF_Widget_Notify
	virtual void	QueryWherePopup(void* pPrivateData, FX_FLOAT fPopupMin,FX_FLOAT fPopupMax, FX_INT32 & nRet, FX_FLOAT & fPopupRet);
	virtual void	OnBeforeKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode, CFX_WideString & strChange, 
		const CFX_WideString& strChangeEx, int nSelStart, int nSelEnd,FX_BOOL bKeyDown, FX_BOOL & bRC, FX_BOOL & bExit, FX_DWORD nFlag);
	virtual void	OnAfterKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_BOOL& bExit, FX_DWORD nFlag);
	virtual void	OnPopupPreOpen(void* pPrivateData, FX_BOOL& bExit);
	virtual void	OnPopupPostOpen(void* pPrivateData, FX_BOOL& bExit);

public: //access external objects.
	FSPDFPage*				GetCurrentPageView() const;
	CFSPDF_WidgetAnnotHandler*	GetWidgetHandler() const;
protected:
	CFX_PointF					PDFPointToWidget(FSPDFPage* pPageView, CFX_PointF point);
	static void					CallControlDrawAppearance(FSFormControl* pControl, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, CPDF_Annot::AppearanceMode mode, CPDF_Annot::AppearanceMode fallBackMode, const CPDF_RenderOptions* pOptions);

public: //save, validate & commit, etc..
	FX_BOOL				CommitData		(FSPDFPage* pPageView, FS_DWORD nFlags);
	virtual FX_BOOL		IsDataChanged	(FSPDFPage* pPageView);
	virtual void		SaveData		(FSPDFPage* pPageView);

public: //get control properties 
	CPDF_Matrix			GetCurrentMatrix();
	CPDF_Rect			GetViewBBoxF	(FSPDFPage *pPageView);
	FX_RECT				GetViewBBox		(FSPDFPage *pPageView);
	CPDF_Rect			GetFocusBox		(FSPDFPage* pPageView);

public: // coordinate transformation
	static CPDF_Rect	FFLRectToWidget(const CPDF_Matrix& mt, const CPDF_Rect& rect);
	static CPDF_Point	FFLPointToWidget(const CPDF_Matrix& mt, const CPDF_Point& point);
	static CPDF_Rect	WidgetRectToFFL(const CPDF_Matrix& mt, const CPDF_Rect& rect);
	static CPDF_Point	WidgetPointtoFFL(const CPDF_Matrix& mt, const CPDF_Point & point);

public:
	void				ExitFiller(FSPDFPage* pPageView, FX_BOOL bReleaseWidget);

public:		// widget activation
	FX_BOOL				IsActivated() const {return m_bActivated;}
	void				Activate()		{m_bActivated = TRUE;}
	void				DeActivate()	{m_bActivated = FALSE;}

public:		// action data
	virtual void		GetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& fa);
	virtual void		SetActionData(FSPDFPage* pPageView, CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& fa);
	virtual FX_BOOL		IsActionDataChanged(CPDF_AAction::AActionType type, const FSPDF_FieldActionParam& faOld, const FSPDF_FieldActionParam& faNew);

public:		// display update
	void				InvalidateRect(FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom);
	virtual void		FormatAndResetAppearance();
	void				Invalidate(FSPDFPage* pPageView = NULL); 
	void				UpdateField(CPDF_FormField* pFormField = NULL,FX_LPCWSTR value = NULL, FX_BOOL doResetAppearance = TRUE, FX_BOOL doInvalidateRect = TRUE);


	void				SetChangeMark();

public:		// state keeping
	//Note: SaveState() stores data into member variables, while SaveData() stores data into pControl.  --CrLF
	virtual void				SaveState(FSPDFPage* pPageView);  
	virtual void				RestoreState(FSPDFPage* pPageView);

protected:
	CFSPDF_WidgetAnnotHandler*	m_pWidgetHandler;
	FSFormControl*		m_pControl;
	CFFL_PageView2Widget		m_Map;

protected:
	FX_BOOL				m_bActivated;

public:
	virtual IFSPDF_Widget*		ResetPDFWindow(FSPDFPage* pPageView, FX_BOOL bRestoreValue)  {return NULL;}
/*


	  virtual	void				OnDeSelected(CPDFSDK_Annot* pAnnot);
	  virtual	void				OnSelected(CPDFSDK_Annot* pAnnot);

	  virtual FX_BOOL				CanCopy(CPDFSDK_Document* pDocument); 
	  virtual FX_BOOL				CanCut(CPDFSDK_Document* pDocument); 
	  virtual FX_BOOL				CanPaste(CPDFSDK_Document* pDocument); 

	  virtual void				DoCopy(CPDFSDK_Document* pDocument); 
	  virtual void				DoCut(CPDFSDK_Document* pDocument); 
	  virtual void				DoPaste(CPDFSDK_Document* pDocument); 

public: //CPWL_TimerHandler
	virtual void				TimerProc();
	virtual IFX_SystemHandler*	GetSystemHandler() const;

public:
	virtual CPDF_Matrix			GetWindowMatrix(void* pAttachedData);
	virtual CFX_WideString		LoadPopupMenuString(int nIndex);



	virtual CPWL_Wnd* 			ResetPDFWindow(FSPDFPage* pPageView, FX_BOOL bRestoreValue);

	virtual void				OnKeyStroke(FX_BOOL bKeyDown);

	CPDF_Point					WndtoPWL(FSPDFPage* pPageView, const CPDF_Point& pt);
	CPDF_Rect					FFLtoWnd(FSPDFPage* pPageView, const CPDF_Rect& rect);

	void						SetWindowRect(FSPDFPage* pPageView, const CPDF_Rect& rcWindow);
	CPDF_Rect					GetWindowRect(FSPDFPage* pPageView);

	static void					FFL_FreeData(void* pData);

	virtual void				GetKeyStrokeData(FSPDFPage* pPageView, FFL_KeyStrokeData& data);

public:
	void						EscapeFiller(FSPDFPage* pPageView, FX_BOOL bDestroyPDFWindow);

	CPDF_Rect					GetPDFWindowRect() const;

*/

};


#endif // _FFL_WIDGET_H_
