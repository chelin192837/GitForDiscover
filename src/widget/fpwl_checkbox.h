#ifndef _FPWL_CHECKBOX_IMP_H_
#define _FPWL_CHECKBOX_IMP_H_

#include "fpwl_pushbutton.h"
//*****************************************************************************
//* 
//*****************************************************************************
class CFPWL_CheckBox : public CFPWL_PushButton
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FPDFW_ERR			SetCheckState(FX_INT32 iCheck);
	FX_INT32			GetCheckState();

	CFPWL_CheckBox();
	virtual ~CFPWL_CheckBox();
};


#endif //_FPWL_CHECKBOX_IMP_H_


