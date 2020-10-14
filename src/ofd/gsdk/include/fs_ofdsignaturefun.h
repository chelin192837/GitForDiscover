#ifndef _FS_OFDSIGNATUREFUN_H_
#define _FS_OFDSIGNATUREFUN_H_

#include "fxcore/include/fxcrt/fx_system.h"

//#include "ofd/include/fx_time.h"

#include "../fxlib.h"
#include "fxcore/include/fxcrt/fx_stream.h"


//
int FS_LoadSealModule(CFX_WideString& wsOESFile, CFX_ByteString bsPin);
void FS_FreeSealModule();
FX_LPVOID FS_LoadSealInfo(CFX_ByteString& bsSeal, FX_BOOL bSealID, int bCheckSeallist, int* err);
void FS_UnLoadSealInfo(FX_LPVOID sealInfo);

CFX_WideString FS_GetSignDataTime();

FX_DWORD FS_SignOFD(IOFD_Document* doc, FX_LPVOID sealInfo, IOFD_WriteDocument* pWriteDoc, int pageIndex, float x, float y, IFX_FileWrite* pFileWrite, FX_BOOL bLocked = FALSE, /*fa piao*/int posx = -1000, /*fa piao*/int posy = -1000);

class CStraddleSealPlot
{
public:
	CStraddleSealPlot(void* sealInfo
			, COFD_WriteSignature* wSeal
			, IOFD_WriteDocument* pWriteDoc
			, IOFD_Document* pReadDoc
			, int nPart
			, int nProportion
			, float	fFirstPagePercent
			, int nFirstPageIndex
			, int nCountPage
			)
	{
		m_sealInfo = sealInfo;
		m_wSeal = wSeal;
		m_pWriteDoc = pWriteDoc;
		m_pReadDoc = pReadDoc;
		m_nPart =nPart;
		m_nProportion = nProportion;
		m_fFirstPagePercent = fFirstPagePercent;
		m_nFirstPageIndex = nFirstPageIndex;
		m_nCountPage = nCountPage;
	}

	~CStraddleSealPlot()
	{
	}

	void StraddleSeal();
	void LeftFirstPagePercent();
	void TopFirstPagePercent();
	void RightFirstPagePercent();
	void BottomFirstPagePercent();

	void LeftPart();
	void TopPart();
	void RightPart();
	void BottomPart();

private:
	void*				m_sealInfo;
	COFD_WriteSignature*		m_wSeal;
	IOFD_WriteDocument*		m_pWriteDoc;
	IOFD_Document*			m_pReadDoc;
	int				m_nPart;	// 0 left 1 top 2 right 3bottom
	int				m_nProportion;
	float				m_fFirstPagePercent;
	int 				m_nFirstPageIndex;
	int 				m_nCountPage;
};

FX_DWORD FS_SignOFDStraddle(FX_LPVOID sealInfo, IOFD_WriteDocument* pWriteDoc, IFX_FileWrite* pFileWrite, int nPart, int nProportion, float fFirstPagePercent, int nFirstPageIndex, int nCountPage);
FX_DWORD FS_SignData(FX_LPVOID sealInfo, FX_LPBYTE pbTxts, unsigned int cbTxts, CFX_ByteString& bsSign, CFX_WideString& wsDateTime);

FX_DWORD FS_VerifyOFD(COFD_Signature* pSign, int index, int nOnLine);
FX_DWORD FS_VerifyData(FX_LPBYTE pbData, FX_DWORD dwLen, CFX_ByteString& bsSigData, CFX_ByteString& bsSealData, CFX_ByteString& bsSignMethod, CFX_ByteString& bsDigestMethod, CFX_ByteString& bsDateTime, int nOnLine);
CFX_ByteString FS_GetSealList();
void* FS_GetSealProperty(IFX_FileRead* pSealFile);

FX_DWORD FS_SignPDFFile(FX_LPVOID sealInfo, IFX_FileRead* file, IFX_MemoryStream *fw, int pageIndex, float x, float y);
FX_DWORD FS_VerifyPDF(IFX_FileRead* pFileRead, int index, int nOnLine);
FX_DWORD FS_GetPDFSigCount(IFX_FileRead* pFileRead);

void HexStrToByte(char* pHexStr, int iHexLen, FX_BYTE* pStr);

//FX_BOOL FS_GetSealImage(IOFD_FileStream* pSealFile, FX_BOOL bFlag, OFD_SEALTYPE& eType, CFX_DIBitmap*& pDIB, IOFD_FileStream*& pImageStream);

// Cert for OFD.
FX_BOOL FS_CertSignInit();
void FS_CertSignRelease();
FX_DWORD FS_CertSignDoc(IFX_FileWrite* pFileWrite, IOFD_WriteDocument* pWriteDoc, const CFX_WideString& wsCertFile, const CFX_WideString& pwd);
FX_DWORD FX_CertVerifyOFD(COFD_Signature* pSign);
FX_DWORD FX_CertGetInfo(COFD_Signature* pSign); // ��ȡ֤�����ԣ������֤ʧ�ܣ�����û����ȡ֤���ʱ��ͷ����ˣ��ʴ����������������
CFX_ByteString FX_CertGetProperty(FX_INT32 nIndex);
FX_DWORD FS_SignOFD_KTPOS(FX_LPVOID psealInfo, IOFD_WriteDocument* pWriteDoc, int* pPageArr, int nSize, int nSubRange, float x, float y, IFX_FileWrite* pFileWrite, FX_BOOL bLocked = FALSE);
//
struct FS_SEAL_INFO
{
	// Seal
	CFX_ByteString	sealId;
	CFX_WideString	sealName;
	OFD_SEALTYPE	sealImageType;
	CFX_DIBitmap*	sealBmp;
	IOFD_FileStream* sealImageStream;
	FX_INT32		width;
	FX_INT32		height;
	CFX_ByteString	sealData;
	IOFD_FileStream* sealStream;

	// Sign
	CFX_WideString	version;
	CFX_WideString	name;
	CFX_WideString	company;

	// Algorithm
	CFX_ByteString	signMethod;
	CFX_ByteString	digestMethod;
};

void FS_GetSealImage_raw(FS_SEAL_INFO* sealInfo, FX_LPBYTE &pPictureData,FX_LPBYTE &pPictureType, FX_INT32 &pictureDataLen, FX_INT32 &pictureTypeLen);

void OFD_Sign_SetStamp(CFS_OFDPage* pOFDPage, IFX_FileRead* pImage, int image_type, float& x, float& y, float& w, float& h, int rotate);

#endif //_FS_OFDSIGNATUREFUN_H_
