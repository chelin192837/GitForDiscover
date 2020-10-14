#ifndef _FS_OFDPATHOBJECT_H_
#define _FS_OFDPATHOBJECT_H_

class CFS_OFDContentObject;
class CFS_OFDPathObject;
class CFS_OFDLayer;

class CFS_OFDPathObject : public CFS_OFDContentObject
{
public:
	CFS_OFDPathObject();
	virtual ~CFS_OFDPathObject();

	void SetFillMode(FX_INT32 nFillMode);
	void SetFill(FX_BOOL bSet);
	void SetStroke(FX_BOOL bSet);
	void SetAbbreviteData(FX_LPCSTR lpszAbbreviteData, int iLen);
	void SetPath(COFD_Path* path);

	const COFD_Path* GetPath();
	FX_BOOL IsStroke();
	FX_BOOL IsFill();
	FX_INT32 GetFillRule();
	void GetAbbreviteData(FX_LPSTR lpszAbbreviteData, int* piLen);

	void CopyPathObj(CFS_OFDPathObject* pSrcPathObj);
protected:
	virtual OFD_CONTENTTYPE GetWriteContentType();
	void ParseAbbreviteData(COFD_Path *pPath, const CFX_WideString& wsStr);
	void BuildAbbreviteData(COFD_Path *pPath, CFX_WideString& wsStr);

public:
	static CFX_WideString BuildAbbreviteData(COFD_Path *pPath);
};

#endif //_FS_OFDPATHOBJECT_H_
