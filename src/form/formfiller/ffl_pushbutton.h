#ifndef _FFL_PUSHBUTTON_H_
#define _FFL_PUSHBUTTON_H_

class CFFL_PushButtonCtrl : public CFFL_ButtonCtrl
{
public:
	CFFL_PushButtonCtrl(FSFormControl* pControl, CFSPDF_WidgetAnnotHandler* pWidgetHandler);
	virtual ~CFFL_PushButtonCtrl();

public:
	virtual IFSPDF_Widget* InitWidget();

public:
	IFSPDF_PushButton*	GetWidget(FSPDFPage* pPageView, FX_BOOL bNewIfInexist = TRUE) {return (IFSPDF_PushButton*)CFFL_Widget::GetWidget(pPageView, bNewIfInexist);}

public:
	virtual FX_BOOL	OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags);
	virtual void	OnDraw(FSPDFPage *pPageView, CFX_RenderDevice* pDevice, CPDF_Matrix* pUser2Device, FX_DWORD dwFlags);

};

#endif // _FFL_PUSHBUTTON_H_
