#ifndef _FS_OFDVIDEOOBJECT_H_
#define _FS_OFDVIDEOOBJECT_H_

class CFS_OFDContentObject;
class CFS_OFDVideoObject;
class CFS_OFDLayer;

class CFS_OFDVideoObject : public CFS_OFDContentObject
{
public:
	CFS_OFDVideoObject();
	~CFS_OFDVideoObject();

	FX_BOOL GetVideoData(FX_LPBYTE lpbData, FX_INT32 *piLen);
	FX_INT32 GetResID();
	CFX_WideString GetType();
	CFX_WideString GetFormat();
	CFX_WideString GetTitle();
	FX_BOOL HasBorder();
	FX_FLOAT GetBorderLineWidth();
	FX_FLOAT GetBorderDashOffset();
	void GetBorderDashPattern(FX_FLOAT* fpDashPatterns, FX_INT32* iCount);
	void GetBorderCornerRadius(FX_FLOAT* fHCRadius, FX_FLOAT* fVCRadius);
	FX_ARGB GetBorderStrokeColor();

	void SetVideoFromFile(const CFX_WideString& wsFilename);
	void SetType(const CFX_WideString& wsType);
	void SetFormat(const CFX_WideString& wsFormat);
	void SetTitle(const CFX_WideString& wsTitle);
	void CreateBorder();
	void SetBorderLineWidth(FX_FLOAT fLineWidth);
	void SetBorderDashPattern(FX_FLOAT fDashOffset, FX_FLOAT* pfDashPatters, FX_INT32 iCount);
	void SetBorderCornerRadius(FX_FLOAT fHCRadius, FX_FLOAT fVCRadius);
	void SetBorderStrokeColor(FX_ARGB strokeColor);

protected:
	virtual OFD_CONTENTTYPE GetWriteContentType();
	void SetWriteVideoObject(CFS_OFDDocument* pDoc, IFX_FileRead* pVideoFile, 
		const CFX_WideString& wsPath);
	void SetWriteVideoObject(CFS_OFDDocument* pDoc, const CFX_WideString& videoFileName);

	COFD_VideoObject* GetVideoObject();
	COFD_Multimedia* GetMultimedia();
	COFD_Border* GetBorder();

	COFD_WriteMultimedia* m_pWriteMultimedia;
	CFX_WideString m_wsType;
	CFX_WideString m_wsFormat;

	COFD_WriteBorder* m_pWriteBorder;
};

#endif //_FS_OFDVIDEOOBJECT_H_
