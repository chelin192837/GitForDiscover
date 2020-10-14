#ifndef _FPWL_SIGNATURE_IMP_H_
#define _FPWL_SIGNATURE_IMP_H_

class CFPWL_Signature : public CPWL_Widget
{
public:
	virtual	void		Release();
	virtual FPDFW_ERR	Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FPDFW_ERR		SetShowText(FX_BOOL bShowText);
	FPDFW_ERR		SetShowImage(FX_BOOL bShowImage);
	
	FPDFW_ERR		SetText(const CFX_WideString text);
	FPDFW_ERR		SetImageStream(CPDF_Stream* pStream, FX_LPCSTR sImageAlias);
	FPDFW_ERR		SetDescription(const CFX_WideString description);

	
	CFPWL_Signature();
	~CFPWL_Signature();
};

#endif //_FPWL_SIGNATURE_IMP_H_