#ifndef _FPWL_PUSHBUTTON_IMP_H_
#define _FPWL_PUSHBUTTON_IMP_H_

#include "fpwl_widget.h"
//*****************************************************************************
//* 
//*****************************************************************************
class CFPWL_PushButton : public CPWL_Widget
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot,CFSPDF_WidgetProperties* pProperties = NULL);

	CFPWL_PushButton();
	virtual ~CFPWL_PushButton();
};


#endif //_FPWL_PUSHBUTTON_IMP_H_


