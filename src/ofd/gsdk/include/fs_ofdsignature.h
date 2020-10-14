#ifndef _FS_OFDSIGNATURE_H_
#define _FS_OFDSIGNATURE_H_

#define _FS_OFD_NEWSIG_LIB
#ifdef _FS_OFD_NEWSIG_LIB

#include "../../../rdkcommon/rdk/fxlib/ofd/include/ofd.h"

class COFD_DocProvider : public CFX_Object, public IOFD_DocProvider
{
public:
	COFD_DocProvider();
	~COFD_DocProvider();

	void Init();
	void Init(CFX_DIBitmap* pSeal);
	void Init(IOFD_FileStream* pVectorSeal);
		CFX_DIBitmap*	GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile);
		CFX_DIBitmap*   GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile, COFD_StampAnnot* pStampAnnot);
        CFX_DIBitmap*   GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile, OFD_SEALBITMAPTYPE type, COFD_StampAnnot* pStampAnnot);
        IOFD_FileStream* GetSealFile(IOFD_Document* pDoc, IOFD_FileStream* pSealFile);
        OFD_SEALTYPE    GetSealType(IOFD_FileStream* pSealFile) const;
        void            SetSignVersion(OFD_SIGNVERSION eVersion);
        OFD_OESVERSION  GetOesVersion(COFD_Signature* pSignature)	{ return OFD_OES_FIRST; }
        OFD_SEALBITMAPTYPE GetSealBitmapType()	{ return OFD_SEALBITMAP_NORMAL; }
        FX_BOOL                 IsOesLibExist()	{ return TRUE; }

protected:
	CFX_PtrArray m_pRenderSeals;
	CFX_PtrArray m_pRenderVectorSeals;

	CFX_DIBitmap* m_pSignSeal;
	IOFD_FileStream* m_pSignVectorSeal;
	OFD_SEALTYPE m_type;
	OFD_SIGNVERSION m_SignVersion;

	FX_BOOL m_bSign;
};
#else
class COFD_DocProvider : public CFX_Object, public IOFD_DocProvider
{
public:
	COFD_DocProvider();
	~COFD_DocProvider();

	void Init();
	void Init(CFX_DIBitmap* pSeal);
	void Init(IOFD_FileStream* pVectorSeal);
	CFX_DIBitmap* GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile);
	IOFD_FileStream*GetSealFile(IOFD_Document* pDoc, IOFD_FileStream* pSealFile);
	OFD_SEALTYPE	GetSealType(IOFD_FileStream* pSealFile) const;
	void            SetSignVersion(OFD_SIGNVERSION eVersion){ m_SignVersion = eVersion; };

protected:
	CFX_PtrArray m_pRenderSeals;
	CFX_PtrArray m_pRenderVectorSeals;

	CFX_DIBitmap* m_pSignSeal;
	IOFD_FileStream* m_pSignVectorSeal;
	OFD_SEALTYPE m_type;
	OFD_SIGNVERSION m_SignVersion;

	FX_BOOL m_bSign;
};
#endif	//_FS_OFD_NEWSIG_LIB

CFX_DIBitmap* FS_CreateSealBitmap(FX_LPBYTE pdata, FX_INT32 len);
void  FS_ParseSealData(FX_LPBYTE psealData/*in*/ ,FX_INT32 psealDataLen /*in*/,FX_INT32 _startpos /*in*/,FX_INT32 & _inoutPutLen/*in out*/,FX_INT32 &psealDataOffset/*in out*/,FX_LPBYTE pSealType/*out*/,FX_INT32& sealTypeLen/*out*/,FX_INT32 nCount);
FX_BOOL FS_Decode_Sealdata(const unsigned char *buffer, const int& buf_size, FX_LPBYTE sealPic, FX_INT32& sealPicLen,FX_LPBYTE sealType, FX_INT32& sealTypeLen);
FX_BOOL FS_Ber_Decode_PicSize(const unsigned char *buffer, const int& buf_size, long& width, long& height);
FX_BOOL FS_GetSealImage(OFD_SIGNVERSION eSignVersion, IOFD_FileStream* pSealFile, FX_BOOL bFlag, COFD_StampAnnot* pStampAnnot, OFD_SEALTYPE& eType, CFX_DIBitmap*& pDIB, IOFD_FileStream*& pImageStream);
// Added by xhj ,20200408
FX_BOOL FS_GetSealImage_(void* pOESInterface_, FX_LPBYTE buffer, FX_STRSIZE size, FX_BOOL bFlag, OFD_SEALTYPE& eType, CFX_DIBitmap*& pDIB, IOFD_FileStream*& pImageStream);
CFX_DIBitmap*   RenderVetorBitmap(FX_LPBYTE pdata, FX_INT32 len,int width,int height);

CFX_DIBitmap* GetSignedValueBitmap(const unsigned char *pSignBuf, int nLen);

#endif //_FS_OFDSIGNATURE_H_
