#ifndef _FPWL_RADIOBUTTON_IMP_H_
#define _FPWL_RADIOBUTTON_IMP_H_

#include "fpwl_widget.h"
#include "fpwl_pushbutton.h"
//*****************************************************************************
//* 
//*****************************************************************************
class CFPWL_RadioButton : public CFPWL_PushButton
{
public:
	virtual void				Release();

	virtual FPDFW_ERR			Initialize(FSAnnot* pAnnot,CFSPDF_WidgetProperties* pProperties = NULL);

	FX_INT32					GetCheckState();
	FPDFW_ERR					SetCheckState(FX_INT32 iCheck);

	CFPWL_RadioButton();
	virtual ~CFPWL_RadioButton();
};


#endif //_FPWL_RADIOBUTTON_IMP_H_


