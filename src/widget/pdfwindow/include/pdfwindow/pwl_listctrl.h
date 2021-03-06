#ifndef _PWL_LISTCTRL_H_
#define _PWL_LISTCTRL_H_

class CPWL_ListCtrl;

class CPWL_ListCtrl : public CPWL_Wnd
{
public:
	CPWL_ListCtrl();
	virtual ~CPWL_ListCtrl();

public:
	void								SetScrollPos(const CPDF_Point& point);
	CPDF_Point							GetScrollPos() const;
	CPDF_Rect							GetScrollArea() const;
	
	void								SetItemSpace(FX_FLOAT fSpace);
	void								SetTopSpace(FX_FLOAT fSpace);
	void								SetBottomSpace(FX_FLOAT fSpace);
	void								ResetFace();
	void								ResetContent(FX_INT32 nStart);
	FX_INT32							GetItemIndex(CPWL_Wnd* pItem);
	FX_FLOAT							GetContentsHeight(FX_FLOAT fLimitWidth);

protected:
	virtual void						RePosChildWnd();
	virtual void						DrawChildAppearance(CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device);

public:
	CPDF_Point							InToOut(const CPDF_Point& point) const;
	CPDF_Point							OutToIn(const CPDF_Point& point) const;
	CPDF_Rect							InToOut(const CPDF_Rect& rect) const;
	CPDF_Rect							OutToIn(const CPDF_Rect& rect) const;

private:
	void								ResetAll(FX_BOOL bMove,FX_INT32 nStart);

private:
	CPDF_Rect							m_rcContent;
	CPDF_Point							m_ptScroll;
	FX_FLOAT							m_fItemSpace;
	FX_FLOAT							m_fTopSpace;
	FX_FLOAT							m_fBottomSpace;
};

#endif

