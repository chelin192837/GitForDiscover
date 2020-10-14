#ifndef _FS_OFDOUTLINE_H_
#define _FS_OFDOUTLINE_H_

class CFS_OFDOutline;

class CFS_OFDOutline : public CFX_Object
{
public:
	CFS_OFDOutline();
	~CFS_OFDOutline();

	FX_BOOL Create(CFS_OFDDocument* pDocument, COFD_Outline* pOutline = NULL);

	FX_INT32 CountSubOutline();
	CFS_OFDOutline* GetSubOutline(FX_INT32 index);
	CFX_WideString GetTitle();
	FX_INT32 GetActionType();
	const COFD_Action* GetAction(FX_INT32 index);
	void GetDest(FX_INT32& iPage, FX_FLOAT& x, FX_FLOAT& y);
	CFX_WideString GetURI();

	FX_INT32 InsertSubOutline(CFS_OFDOutline* pSubOutline, FX_INT32 nIndex = -1);
	void SetExpanded(FX_BOOL bSet);
	void SetTitle(const CFX_WideString& wsTitle);
	void SetDest(int iPage, FX_FLOAT x, FX_FLOAT y);
	void SetURI(const CFX_WideString& wsUri);

	CFS_OFDDocument* GetDocument();
	COFD_WriteOutline* GetWriteOutline();

protected:
	CFS_OFDDocument* m_pDocument;
	COFD_WriteOutline* m_pWriteOutline;
};

#endif //_FS_OFDOUTLINE_H_
