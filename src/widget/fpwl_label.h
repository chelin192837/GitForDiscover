#ifndef _FPWL_LABEL_IMP_H_
#define _FPWL_LABEL_IMP_H_

class CFPWL_Label : public CPWL_Widget
{
public:
	virtual	void		Release();
	virtual FPDFW_ERR	Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FPDFW_ERR	SetText(const CFX_WideString wsText);

	CFPWL_Label();
	virtual ~CFPWL_Label();
};


#endif // _FPWL_LABEL_IMP_H_