#ifndef _FPWL_IMAGE_IMP_H_
#define _FPWL_IMAGE_IMP_H_

class CFPWL_Image : public CPWL_Widget
{
public:
	virtual	void		Release();
	virtual FPDFW_ERR	Initialize(FSAnnot* pAnnot, CFSPDF_WidgetProperties* pProperties = NULL);

	FPDFW_ERR	SetImageStream(CPDF_Stream * pStream);

	CFPWL_Image();
	virtual ~CFPWL_Image();
};

#endif //_FPWL_IMAGE_IMP_H_