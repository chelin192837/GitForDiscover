#ifndef _FFL_CHECKBOX_H_
#define _FFL_CHECKBOX_H_

class CFFL_CheckBoxCtrl : public CFFL_ButtonCtrl
{
public:
	CFFL_CheckBoxCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_CheckBoxCtrl();

public:
	virtual IFSPDF_Widget* InitWidget();

public:
	IFSPDF_CheckBox*	GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfInexist = TRUE) {return (IFSPDF_CheckBox*)CFFL_Widget::GetWidget(pPageView, bNewIfInexist);}

public:
	virtual FX_BOOL	OnLButtonUp		(FSPDFPage *pPageView, FS_DWORD nFlags, const CFX_PointF& point);
	virtual FX_BOOL	OnKeyDown		(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual FX_BOOL	OnChar			(FS_DWORD nKeyCode, FS_DWORD nFlags);

	virtual FX_BOOL	IsDataChanged	(FSPDFPage* pPageView);
	virtual void	SaveData		(FSPDFPage* pPageView);


};

#endif // _FFL_CHECKBOX_H_
