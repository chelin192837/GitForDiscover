#ifndef _FS_OFDIMAGEOBJECT_H_
#define _FS_OFDIMAGEOBJECT_H_

#include "./fs_ofdcontentobject.h"

class CFS_OFDContentObject;
class CFS_OFDImageObject;
class CFS_OFDLayer;

class CFS_OFDImageObject : public CFS_OFDContentObject
{
public:
	CFS_OFDImageObject();
	~CFS_OFDImageObject();

	FX_DWORD SetImageFromFile(const CFX_WideString& wsFilename);
	FX_DWORD SetImageFromBuf(FX_LPBYTE lpbDataBuf, FX_INT32 iLen, FX_INT32 imageFormat, FX_BOOL bTakeover = FALSE);
	void SetImageFromRes(FX_DWORD dwResID);
	void SetImageMask(FX_LPCBYTE imageBuf, FX_INT32 imageBufLen, FX_BOOL bTakeOver);

	FX_INT32 GetImageFormat();
	FX_BOOL GetImageData(FX_LPBYTE lpbData, FX_INT32 *piLen);

	void CopyImageObj(CFS_OFDImageObject* pSrcImageObj);
public:
	CFX_DIBitmap* LoadDIBitmap(IOFD_Document* pDoc, COFD_ImageObject* pImageObj);
	void SetDIBitmap(IOFD_WriteDocument* pWDoc, COFD_ImageObject* pImageObj, CFX_DIBitmap* pDIBitmap);

protected:
	virtual OFD_CONTENTTYPE GetWriteContentType();

};

#endif //_FS_OFDIMAGEOBJECT_H_
