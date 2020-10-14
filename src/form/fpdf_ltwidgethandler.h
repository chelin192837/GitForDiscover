
#ifndef _FSPDF_WIDGETHANDLER_H_
#define _FSPDF_WIDGETHANDLER_H_
 
class CFFL_Widget;
class CFSPDF_ViewContainer;
class IFX_SystemHandler;

class CFSPDF_WidgetAnnotHandler : public IFSPDF_AnnotHandler, public CFX_Object
{

public:  //previously cffl_iwidget.h  //:/* public IBA_AnnotFiller, *///public IPWL_Filler_Notify//, 
										//	public IUndo_EventHandler, public IClipboard_Handler
public:
	CFSPDF_WidgetAnnotHandler();
	virtual	~CFSPDF_WidgetAnnotHandler();

	//Interface methods
	virtual FX_BSTR		GetType();

	virtual void		Release();


	virtual FS_RESULT	GetViewBBox		(FSPDFPage *pPageView, FSAnnot* pAnnot, FX_RECT& rect);
	virtual FS_RESULT	OnDraw			(FSPDFPage *pPageView, FSRenderer* renderer, FSAnnot* pAnnot, 
											CPDF_Matrix* pUser2Device, const FSRectI& rcWindow, FS_DWORD dwFlags);

	virtual FS_RESULT	OnCreate		(FSAnnot* pAnnot);
	virtual FS_RESULT	OnLoad			(FSAnnot* pAnnot);
	virtual FS_RESULT	OnRelease		(FSAnnot* pAnnot);

	virtual FS_RESULT	OnMouseEnter	(FSAnnot* pAnnot, FSPDFPage *pPageView);
	virtual FS_RESULT	OnMouseExit		(FSAnnot* pAnnot, FSPDFPage *pPageView);

	virtual FS_RESULT	OnLButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnLButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnLButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnMouseMove		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnMouseWheel	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, FS_INT32 deltaX, FX_INT32 deltaY, const CFX_PointF& point);
	virtual FS_RESULT	OnRButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnRButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnRButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnMButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnMButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FS_RESULT	OnMButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);

	virtual FS_RESULT	OnKeyDown		(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FS_RESULT	OnKeyUp			(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FS_RESULT	OnChar			(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FS_RESULT	OnString		(FSAnnot* pAnnot, FX_WSTR string,	FS_DWORD nFlags);

	virtual	FS_RESULT	OnDeSelected	(FSAnnot* pAnnot);
	virtual	FS_RESULT	OnSelected		(FSAnnot* pAnnot);

	virtual FS_RESULT	OnSetFocus		(FSAnnot* pAnnot);
	virtual FS_RESULT	OnKillFocus		(FSAnnot* pAnnot);

	//virtual FS_RESULT	SetViewContainer (CFSPDF_ViewContainer* viewContainer);
	//virtual FS_RESULT	GetViewContainer (CFSPDF_ViewContainer** viewContainer);
	virtual FS_RESULT			SetFormFiller (FSFormFillerImp* formFiller);
	virtual FS_RESULT			GetFormFiller (FSFormFillerImp** formFiller);
	virtual FS_RESULT			useWindowless (FS_BOOL* bUseWindowless);
public:
	static void			DrawAppearance(FSFormControl* pControl, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, CPDF_Annot::AppearanceMode mode, const CPDF_RenderOptions* pOptions);
	static void			DrawFocus(const CPDF_Rect& rcFocus, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, FSPDFPage* pPageView);
	static void			DrawShadow(FSFormControl *pControl, CFX_RenderDevice* pDevice, const CPDF_Matrix* pUser2Device, FSPDFPage* pPageView);
	static FX_BOOL		IsWidgetAppearanceValid(FSFormControl* pControl, CPDF_Annot::AppearanceMode mode);

	CFFL_Widget*		GetWidget(FSFormControl* pControl, FX_BOOL bCreateIfNotExist = TRUE);
	void				ReleaseWidget(FSFormControl* pControl);

	FSFormControl*	GetFocusControl();
	void					SetFocusControl(FSFormControl* pControl);

	IFX_SystemHandler*		GetSystemHandler();

	static FX_BOOL		WidgetDirectionIsRightToLeft(CPDF_Dictionary* pAnnotDict, FX_BOOL& bRTL);//copy from phantom, used to draw the drop button of combo box
public:
	void				OnButtonUp(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bReset, FX_BOOL& bExit,FS_DWORD nFlag);
	/*


	virtual CFX_ByteString		GetName()  {return CFX_ByteString("WidgetHandler");}
	virtual FX_BOOL				CanAnswer(FSAnnot* pAnnot);

	virtual CPDF_Rect			GetViewBBox(FSPDFPage *pPageView, FSAnnot* pAnnot);

	virtual FX_BOOL				HitTest(FSPDFPage *pPageView, FSAnnot* pAnnot, const CPDF_Point& point);

private:

	FX_BOOL	IsAppearanceValid(FSAnnot* pAnnot);
	// liang*/

public:
	//
	//
	//	virtual FX_BOOL				Annot_HitTest(FSPDFPage* pPageView,FSAnnot* pAnnot, CPDF_Point point);
	//	virtual void				OnDraw(FSPDFPage *pPageView, /*HDC hDC,*/ FSAnnot* pAnnot, 
	//									CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device,
	//									/*const CRect& rcWindow,*/ FX_DWORD dwFlags);
	//
	//public:
	//	virtual void				QueryWherePopup(void* pPrivateData, FX_FLOAT fPopupMin,FX_FLOAT fPopupMax, FX_INT32 & nRet, FX_FLOAT & fPopupRet);
	//	virtual void				OnBeforeKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode,
	//										CFX_WideString & strChange, const CFX_WideString& strChangeEx, 
	//										int nSelStart, int nSelEnd,
	//										FX_BOOL bKeyDown, FX_BOOL & bRC, FX_BOOL & bExit, FX_DWORD nFlag);
	//	virtual void				OnAfterKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_BOOL & bExit, FX_DWORD nFlag) ;
	//
	//	virtual void				OnSetWindowRect(void* pPrivateData, const CPDF_Rect & rcWindow);
	//	virtual void				OnKeyStroke(FX_BOOL bEditOrList, void* pPrivateData, FX_INT32 nKeyCode, CFX_WideString & strChange, 
	//									const CFX_WideString& strChangeEx, FX_BOOL bKeyDown, FX_BOOL & bRC, FX_BOOL & bExit);
	//
	//public:
	//	virtual void				BeforeUndo();
	//	virtual void				BeforeRedo();
	//	virtual void				AfterUndo();
	//	virtual void				AfterRedo();
	//
	//public:
	//	
	//
	//public:
	//
	// 	static FX_BOOL				IsValidAnnot(FSPDFPage* pPageView, FSAnnot* pAnnot);
	//
	//	void						OnKeyStrokeCommit(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bRC, FX_BOOL& bExit, FX_DWORD nFlag);
	//	void						OnValidate(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bRC, FX_BOOL& bExit, FX_DWORD nFlag);
	//
	//	void						OnCalculate(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bExit, FX_DWORD nFlag);
	//	void						OnFormat(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bExit, FX_DWORD nFlag);
	//	void						OnButtonUp(FSFormControl* pWidget, FSPDFPage* pPageView, FX_BOOL& bReset, FX_BOOL& bExit,FS_DWORD nFlag);
	//	static int					GetCommitKey();
	//	static FX_BOOL				GetKeyDown();
	//
	//
	//public:
	//
	//private:
	//	void						SetFocusAnnotTab(FSFormControl* pWidget, FX_BOOL bSameField, FX_BOOL bNext);

private:
	typedef CFX_MapPtrTemplate<FSFormControl*, CFFL_Widget*> CFFL_Control2Widget;

	CFFL_Control2Widget			m_Map;
	FX_BOOL						m_bNotifyBlocked; // used during notify processing
	//CFSPDF_ViewContainer*		m_pViewContainer;
	FSFormFillerImp*		m_pFormFiller;
	IFX_SystemHandler*			m_pSystemHandler;
public:
	FSCRT_DEFINELOCK(m_lock)

protected:
	FX_BOOL						m_bNotifying;
};


class CFFL_PrivateData
{
public:
	FSFormControl*	m_pControl;
	FSPDFPage*			m_pPageView;
	int						m_nWidgetAge;
	int						m_nValueAge;
};

#endif //_FSPDF_WIDGETHANDLER_H_

