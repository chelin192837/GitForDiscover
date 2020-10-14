#ifndef _FSPDF_ANNOTHANDLER_H_
#define _FSPDF_ANNOTHANDLER_H_

class FSPDFPage;

class IFSPDF_AnnotHandler
{
public:
	virtual void				Release() = 0;
	//annot subtype
	virtual FX_BSTR				GetType() = 0;
	
	virtual FS_RESULT			GetViewBBox		(FSPDFPage *pPageView, FSAnnot* pAnnot, FX_RECT& rect) = 0;
	virtual FS_RESULT			OnDraw			(FSPDFPage *pPageView, FSRenderer* renderer, FSAnnot* pAnnot, 
													CPDF_Matrix* pUser2Device, const FSRectI& rcWindow, FS_DWORD dwFlags) = 0;

	virtual FS_RESULT			OnCreate		(FSAnnot* pAnnot) = 0;
	virtual FS_RESULT			OnLoad			(FSAnnot* pAnnot) = 0;
	virtual FS_RESULT			OnRelease		(FSAnnot* pAnnot) = 0;

	virtual FS_RESULT			OnMouseEnter	(FSAnnot* pAnnot, FSPDFPage *pPageView) = 0;
	virtual FS_RESULT			OnMouseExit		(FSAnnot* pAnnot, FSPDFPage *pPageView) = 0;
	
	virtual FS_RESULT			OnLButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnLButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnLButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnMouseMove		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnMouseWheel	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, FS_INT32 deltaX, FS_INT32 deltaY, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnRButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnRButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnRButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnMButtonDown	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnMButtonUp		(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;
	virtual FS_RESULT			OnMButtonDblClk	(FSAnnot* pAnnot, FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point) = 0;

	virtual FS_RESULT			OnKeyDown		(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags) = 0;
	virtual FS_RESULT			OnKeyUp			(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags) = 0;
	virtual FS_RESULT			OnChar			(FSAnnot* pAnnot, FS_DWORD nKeyCode, FS_DWORD nFlags) = 0;
	virtual FS_RESULT			OnString		(FSAnnot* pAnnot, FX_WSTR string,	FS_DWORD nFlags) = 0;

	virtual	FS_RESULT			OnDeSelected	(FSAnnot* pAnnot) = 0;
	virtual	FS_RESULT			OnSelected		(FSAnnot* pAnnot) = 0;

	virtual FS_RESULT			OnSetFocus		(FSAnnot* pAnnot) = 0;
	virtual FS_RESULT			OnKillFocus		(FSAnnot* pAnnot) = 0;

	virtual FS_RESULT			SetFormFiller (FSFormFillerImp* formFiller) = 0;
	virtual FS_RESULT			GetFormFiller (FSFormFillerImp** formFiller) = 0;
	//virtual FS_RESULT			SetViewContainer (CFSPDF_ViewContainer* viewContainer) = 0;
	//virtual FS_RESULT			GetViewContainer (CFSPDF_ViewContainer** viewContainer) = 0;
	virtual FS_RESULT			useWindowless (FS_BOOL* bUseWindowless) = 0;
protected:
	virtual ~IFSPDF_AnnotHandler() {}
};

#endif //_FSPDF_ANNOTHANDLER_H_
