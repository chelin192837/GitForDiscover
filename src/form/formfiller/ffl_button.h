#ifndef _FFL_BUTTON_H_
#define _FFL_BUTTON_H_

#define CFFL_BUTTON_MOUSESTATE_DOWN 1
#define CFFL_BUTTON_MOUSESTATE_IN 2

class CFFL_ButtonCtrl : public CFFL_Widget
{
public:
	CFFL_ButtonCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_ButtonCtrl();

public: //repaint events
	virtual void	OnDraw			(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags);
	virtual void	OnDrawInactive	(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags);

public: //mouse events
	virtual FX_BOOL	OnLButtonDown	(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL	OnLButtonUp		(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);

	virtual void	OnMouseEnter	(FSPDFPage *pPageView);
	virtual void	OnMouseExit		(FSPDFPage *pPageView);

protected:
	FX_DWORD	m_nMouseState;
};


#endif // _FFL_BUTTON_H_
