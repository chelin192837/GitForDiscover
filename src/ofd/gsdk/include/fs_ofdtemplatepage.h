#ifndef _FS_OFDTEMPLATEPAGE_H_
#define _FS_OFDTEMPLATEPAGE_H_

class CFS_OFDPage;
class CFS_OFDTemplatePage;
class CFS_OFDDocument;

class CFS_OFDTemplatePage : public CFS_OFDPage
{
public:
	CFS_OFDTemplatePage();
	virtual ~CFS_OFDTemplatePage();

	FX_BOOL Create(CFS_OFDDocument* pDocument, FX_INT32 nIndex, IOFD_WritePage* pWritePage = NULL);

	IOFD_WriteTemplatePage* GetWriteTemplatePage();
	FX_INT32 GetPageIndex();
	void SetPageName(const CFX_WideString& wsName);
};

#endif //_FS_OFDTEMPLATEPAGE_H_
