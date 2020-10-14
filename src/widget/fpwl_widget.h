#ifndef _FPWL_WIDGET_IMP_H_
#define _FPWL_WIDGET_IMP_H_

class FSAnnot;

class CPWL_Wnd;
class CPWL_FontMap;
class FX_SystemHandlerImp;
//*****************************************************************************
//* 
//*****************************************************************************
class CPWL_Widget : public CFX_Object
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);
	virtual FPDFW_ERR			Finalize();
	
	virtual FPDFW_ERR			GetWidgetRect(CFX_FloatRect &rect);
	virtual FPDFW_ERR			SetWidgetRect(const CFX_FloatRect &rect);
	virtual	FPDFW_ERR			GetClientRect(CFX_FloatRect &rect);
	virtual FX_DWORD			GetStyles();
	virtual FPDFW_ERR			ModifyStyles(FX_DWORD dwStylesAdded, FX_DWORD dwStylesRemoved);
	virtual FX_DWORD			GetStylesEx();
	virtual FPDFW_ERR			ModifyStylesEx(FX_DWORD dwStylesExAdded, FX_DWORD dwStylesExRemoved);
	virtual FX_DWORD			GetStates();
	virtual FPDFW_ERR			SetStates(FX_DWORD dwStates, FX_BOOL bSet = TRUE);
	virtual FPDFW_ERR			SetText(const CFX_WideString &wsText);

	virtual	FPDFW_ERR			Update();
	virtual FPDFW_ERR			LockUpdate();
	virtual FPDFW_ERR			UnlockUpdate();
		
	virtual FX_DWORD			HitTest(FX_FLOAT fx, FX_FLOAT fy);
	virtual FPDFW_ERR			DrawWidget(CFX_RenderDevice *pDevice, const CFX_Matrix *pMatrix = NULL);

	virtual FX_BOOL				OnKeyDown(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL				OnKeyUp(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL				OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL				OnString(FX_WSTR string, FS_DWORD nFlags);

	virtual FX_BOOL				OnLButtonDown(FS_DWORD nFlags,const CFX_PointF& point);
	virtual FX_BOOL				OnLButtonUp(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnLButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnMouseMove(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnRButtonDown(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnRButtonUp(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnRButtonDblClk(FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL				OnMouseWheel(FS_DWORD nFlags, short zDelta, const CFX_PointF& point);
	virtual FX_BOOL				OnMouseHover(const CFX_PointF& point);

	virtual void				SetFocus();
	virtual void				KillFocus();

	virtual FPDFW_ERR			GetFocusRect(CFX_FloatRect &rect);

	virtual CFX_ByteString		GetAppearanceStream();
	FS_INT32					GetDefaultFontItalicAngle();
	CPWL_Wnd*					m_pWnd;

	static FPDFW_ERR			ResetAppearance(CPDF_FormControl* pControl, FX_LPCWSTR sValue);


protected:
	CPWL_Widget();
	virtual ~CPWL_Widget();
	CFSPDF_WidgetProperties* m_pProperties;
	FSAnnot*			m_pAnnot;
	IFX_Edit_FontMap*		m_pFontMap;
};


#endif //_FPWL_WIDGET_IMP_H_


