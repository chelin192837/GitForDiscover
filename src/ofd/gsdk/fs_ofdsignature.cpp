#include "fxlib.h"
#include "include/fs_ofdsdkmgr.h"
#include "include/fs_ofdsignaturemanage.h"
#include "include/fs_ofdsignaturefactory.h"
#include "include/fs_ofdsignature.h"

//
COFD_DocProvider::COFD_DocProvider()
{
	m_pSignSeal = NULL;
	m_pSignVectorSeal = NULL;
	m_bSign = TRUE;
}

//
COFD_DocProvider::~COFD_DocProvider()
{
	int count = m_pRenderSeals.GetSize();
	for (int i = 0; i < count; i ++)
	{
		CFX_DIBitmap* pDIB = (CFX_DIBitmap*)m_pRenderSeals.GetAt(i);
		if (pDIB)
			delete pDIB;
	}
	m_pRenderSeals.RemoveAll();

	count = m_pRenderVectorSeals.GetSize();
	for (int j = 0; j < count; j ++)
	{
		IOFD_FileStream* pImageStream = (IOFD_FileStream*)m_pRenderVectorSeals.GetAt(j);
		if (pImageStream)
			pImageStream->Release();
	}
	m_pRenderVectorSeals.RemoveAll();
}

//
void COFD_DocProvider::Init()
{
	//WriteLog_GSDK("COFD_DocProvider::Init1 ", "m_bSign = FALSE ...");
	m_bSign = FALSE;
}

//
void COFD_DocProvider::Init(CFX_DIBitmap* pSeal)
{
	//WriteLog_GSDK("COFD_DocProvider::Init2 ", "OFD_SEAL_BITMAP ...");
	m_bSign = TRUE;
	m_pSignSeal = pSeal;
	//WriteLog_GSDK("COFD_DocProvider::Init2 ", "m_pSignSeal : ", (long)m_pSignSeal);
	m_type = OFD_SEAL_BITMAP;
}

//
void COFD_DocProvider::Init(IOFD_FileStream* pVectorSeal)
{
	m_bSign = TRUE;
	m_pSignVectorSeal = pVectorSeal;

	m_type =  OFD_SEAL_OFD;
}

#ifdef _FS_OFD_NEWSIG_LIB
CFX_DIBitmap* COFD_DocProvider::GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile, COFD_StampAnnot* pStampAnnot)
{
	if (m_bSign)
		return m_pSignSeal;
	else
	{
		COFD_Signature* pSignature;// = pStampAnnot->GetSignature();
		if(pSignature == NULL)
			return NULL;

		CFX_BinaryBuf signValue;
		pSignature->GetSignatureValue(signValue);

		FX_LPBYTE pbBuf = signValue.GetBuffer();
		FX_STRSIZE size = signValue.GetSize();
		if(pbBuf == NULL || size <= 0)
			return NULL;

		CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
		CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
		//WriteLog_GSDK("COFD_DocProvider::GetSealBitmap1 ", "pOESInterface : ", (long)pOESInterface);
		if(pOESInterface != NULL)
		{
			FX_INT32 nVersion = pOESInterface->GetOESVerion();
			//WriteLog_GSDK("COFD_DocProvider::GetSealBitmap1 ", "nVersion : ", nVersion);
			if(nVersion == 3)
			{
				CFX_DIBitmap* pDIB = NULL;
				IOFD_FileStream* pImageStream = NULL;
				OFD_SEALTYPE eType;
				FX_BOOL bRet = FS_GetSealImage_((void*)pOESInterface, pbBuf, size, TRUE, eType, pDIB, pImageStream);
				//WriteLog_GSDK("COFD_DocProvider::GetSealBitmap1 ", "eType : ", eType);
				if(eType == OFD_SEAL_BITMAP)
					m_pRenderSeals.Add(pDIB);
				else if(eType == OFD_SEAL_OFD)
				{
					int nPicLen = pImageStream->GetSize();
					FX_LPBYTE pPicBuf = FX_Alloc(FX_BYTE, nPicLen);
					pImageStream->ReadBlock(pPicBuf, nPicLen);

					pDIB = RenderVetorBitmap(pPicBuf, nPicLen, 245, 245);

					FX_Free(pPicBuf);
				}
				return pDIB;
			}
		}

		CFX_DIBitmap* pDIB = GetSignedValueBitmap((const unsigned char *)pbBuf, size);
/*
		CFX_DIBitmap* pDIB = NULL;
		IOFD_FileStream* pImageStream = NULL;
		OFD_SEALTYPE eType;
		FS_GetSealImage(m_SignVersion, pSealFile, TRUE, pStampAnnot, eType, pDIB, pImageStream);
		WriteLog_GSDK("COFD_DocProvider::GetSealBitmap1 ", "eType : ", eType);
		WriteLog_GSDK("COFD_DocProvider::GetSealBitmap1 ", "pDIB : ", (long)pDIB);
		WriteLog_GSDK("COFD_DocProvider::GetSealBitmap1 ", "pImageStream : ", (long)pImageStream);
		if (!pDIB) return NULL;
*/
		m_pRenderSeals.Add(pDIB);
		return pDIB;
	}
}

//
CFX_DIBitmap*  COFD_DocProvider::GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile, OFD_SEALBITMAPTYPE type, COFD_StampAnnot* pStampAnnot)
{
        if (m_bSign)
                return m_pSignSeal;
        else
        {
                CFX_DIBitmap* pDIB = NULL;
                IOFD_FileStream* pImageStream = NULL;
                OFD_SEALTYPE eType;
				//FS_GetSealImage(m_SignVersion, pSealFile, TRUE, pStampAnnot,eType, pDIB, pImageStream);

                if (!pDIB) return NULL;

                m_pRenderSeals.Add(pDIB);
                return pDIB;
        }
}

#else

//

#endif //_FS_OFD_NEWSIG_LIB

CFX_DIBitmap* COFD_DocProvider::GetSealBitmap(IOFD_Document* pDoc, IOFD_FileStream* pSealFile)
{
	if (m_bSign)
		return m_pSignSeal;
	else
	{
		CFX_DIBitmap* pDIB = NULL;
		IOFD_FileStream* pImageStream = NULL;
		OFD_SEALTYPE eType;
		//FS_GetSealImage(m_SignVersion, pSealFile, TRUE, pStampAnnot, eType, pDIB, pImageStream);
		if (!pDIB) return NULL;

		m_pRenderSeals.Add(pDIB);
		return pDIB;
	}
}

//
IOFD_FileStream* COFD_DocProvider::GetSealFile(IOFD_Document* pDoc, IOFD_FileStream* pSealFile)
{
	if (m_bSign)
		return m_pSignVectorSeal;
	else
	{
		CFX_DIBitmap* pDIB = NULL;
		IOFD_FileStream* pImageStream = NULL;
		OFD_SEALTYPE eType;
		//FS_GetSealImage(m_SignVersion, pSealFile, TRUE, eType, pDIB, pImageStream);

		m_pRenderVectorSeals.Add(pImageStream);
		return pImageStream;
	}
}

//
OFD_SEALTYPE COFD_DocProvider::GetSealType(IOFD_FileStream* pSealFile) const 
{
	if (m_bSign)
		return m_type;
	else
	{
		CFX_DIBitmap* pDIB = NULL;
		IOFD_FileStream* pImageStream = NULL;
		OFD_SEALTYPE eType = OFD_SEAL_BITMAP;
		//FS_GetSealImage(m_SignVersion, pSealFile, FALSE, eType, pDIB, pImageStream);

		return eType;
	}
}

//
void COFD_DocProvider::SetSignVersion(OFD_SIGNVERSION eVersion)
{
	m_SignVersion = eVersion;
}

//
void FS_FormatSignBMP(CFX_DIBitmap* pBmp){
	pBmp->ConvertFormat(FXDIB_Argb);
	FX_INT32 pitch = pBmp->GetPitch();
	for (int Row = 0; Row < pBmp->GetHeight(); ++Row)
	{
		FX_LPBYTE pBuf = (pBmp->GetBuffer() + Row * pitch);
		for (FX_INT32 Column = 0;Column < pBmp->GetWidth();++Column)
		{
			if (pBuf[Column*4]==255 &&
				pBuf[Column*4+1]==255 &&
				pBuf[Column*4+2]==255)
			{
				pBuf[Column * 4 + 3] = 0;
			}
		}
	}
}
CFX_DIBitmap* FS_LoadImage(IFX_FileRead* pFileRead)
{
	IFX_Image* pImage = FX_Image_Create();
	FX_HIMAGE hImage = pImage->LoadImage(pFileRead);
	if (!hImage) return NULL;
	pImage->LoadFrame(hImage, 0);

	//CFX_DIBitmap* pTemp = pImage->GetFrameImage(hImage);
	CFX_DIBitmap* pTemp = pImage->GetFrameImageWithTransparency(hImage);
	CFX_DIBitmap* pDIBitmap = pTemp->Clone();

	pImage->FreeImage(hImage);
	pImage->Release();

	return pDIBitmap;
}

CFX_DIBitmap* FS_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize)
{
	IFX_MemoryStream* pMemoryStream = FX_CreateMemoryStream((FX_LPBYTE)pBuf, nSize);
	CFX_DIBitmap* pDIBitmap = FS_LoadImage(pMemoryStream);
	pMemoryStream->Release();

	return pDIBitmap;
}

//
CFX_DIBitmap* FS_CreateSealBitmap(FX_LPBYTE pdata, FX_INT32 len)
{
	CFX_DIBitmap* pBitmap = FS_LoadImage(pdata, len);
	if (!pBitmap) return NULL;

	FS_FormatSignBMP(pBitmap);
	return pBitmap;
}

//
void  FS_ParseSealData(FX_LPBYTE psealData/*in*/ ,FX_INT32 psealDataLen /*in*/,FX_INT32 _startpos /*in*/,FX_INT32 & _inoutPutLen/*in out*/,FX_INT32 &psealDataOffset/*in out*/,FX_LPBYTE pSealType/*out*/,FX_INT32& sealTypeLen/*out*/,FX_INT32 nCount)
{
	//psealData is begin after seal version,in asn view is the third red point
	if(!psealData || psealDataLen <= 0 ||_startpos < 0||_startpos >= psealDataLen)
		return ;
	FX_INT32 _Offset = 0;
	FX_INT32 _DataLenFlags = 0;
	FX_BYTE   _Datatype  =  psealData[_startpos];
	if (!_Datatype)
		return;
	if (!psealData[_startpos +1])
		return;

	//Parse Picture
	if (5 == nCount && _Datatype == 0x30 ) //never use picture type to parse  picture ,instead ,using the convention of seal data
	{
		switch (psealData[_startpos +1]) 
		{
		case 0x81:
			_inoutPutLen = psealData[_startpos+2]&0xff;
			_DataLenFlags = 2;
			break;
		case 0x82:
			_inoutPutLen =  ( psealData[_startpos+3] & 0xff );
			_inoutPutLen += (( psealData[_startpos+2]& 0xff)<<8);
			_DataLenFlags = 3;
			break;
		case 0x83:
			_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<16;
			_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<8;
			_inoutPutLen +=  (psealData[_startpos+4]& 0xff);
			_DataLenFlags = 4;
			break;
		case 0x84:
			_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<24;
			_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<16;
			_inoutPutLen +=  (psealData[_startpos+4]& 0xff)<<8;
			_inoutPutLen +=  (psealData[_startpos+5]& 0xff);
			_DataLenFlags = 5;
			break;
		default:
			_inoutPutLen = (psealData[_startpos +1 ]& 0xff);
			_DataLenFlags = 1;

		}
		_Offset = _DataLenFlags + 1; // the start position included
		psealDataOffset  = _Offset +_startpos; //the length from 0
		_startpos = psealDataOffset;

		if (psealData[psealDataOffset] == 0x16 ) // picture type descriptor 
		{
			switch (psealData[_startpos +1])
			{
			case 0x81:
				_inoutPutLen = psealData[_startpos+2]&0xff;
				_DataLenFlags = 2;
				break;
			case 0x82:
				_inoutPutLen =  ( psealData[_startpos+3] & 0xff );
				_inoutPutLen += ((psealData[_startpos+2]& 0xff)<<8);
				_DataLenFlags = 3;
				break;
			case 0x83:
				_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<16;
				_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<8;
				_inoutPutLen +=  (psealData[_startpos+4]& 0xff);
				_DataLenFlags = 4;
				break;
			case 0x84:
				_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<24;
				_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<16;
				_inoutPutLen +=  (psealData[_startpos+4]& 0xff)<<8;
				_inoutPutLen +=  (psealData[_startpos+5]& 0xff);
				_DataLenFlags = 5;
				break;
			default:
				_inoutPutLen = (psealData[_startpos +1 ]& 0xff);
				_DataLenFlags = 1;
			}

			_Offset = 0;
			_Offset = _DataLenFlags +1;

			sealTypeLen = _inoutPutLen;
			//attention : this type memory is NULL default
			if (pSealType)
			{
				FX_LPBYTE sealType = pSealType;
				for (int i = 0; i < sealTypeLen; ++i)
				{
					*sealType++ = psealData[_startpos + _Offset + i];
				}
			}
			psealDataOffset = _startpos + _Offset + _inoutPutLen; //the end  of picture type descriptor
			_startpos = psealDataOffset;
		}
		if (psealData[_startpos] == 0x04) //parse picture data
		{
			switch (psealData[_startpos +1])
			{
			case 0x81:
				_inoutPutLen = psealData[_startpos+2]&0xff;
				_DataLenFlags = 2;
				break;
			case 0x82:
				_inoutPutLen =  ( psealData[_startpos+3] & 0xff );
				_inoutPutLen += ((psealData[_startpos+2]& 0xff)<<8);
				_DataLenFlags = 3;
				break;
			case 0x83:
				_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<16;
				_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<8;
				_inoutPutLen +=  (psealData[_startpos+4]& 0xff);
				_DataLenFlags = 4;
				break;
			case 0x84:
				_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<24;
				_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<16;
				_inoutPutLen +=  (psealData[_startpos+4]& 0xff)<<8;
				_inoutPutLen +=  (psealData[_startpos+5]& 0xff);
				_DataLenFlags = 5;
				break;
			default:
				_inoutPutLen = (psealData[_startpos +1 ]& 0xff);
				_DataLenFlags = 1;
			}

			_Offset = 0;
			_Offset = _DataLenFlags +1;
			psealDataOffset = _startpos + _Offset; //the beginning  of picture data descriptor
		}

		return;
	}
	else
	{
		switch (psealData[_startpos +1]) 
		{
		case 0x81:
			_inoutPutLen = psealData[_startpos+2]&0xff;
			_DataLenFlags = 2;
			break;
		case 0x82:
			_inoutPutLen =  ( psealData[_startpos+3] & 0xff );
			_inoutPutLen += (( psealData[_startpos+2]& 0xff)<<8);
			_DataLenFlags = 3;
			break;
		case 0x83:
			_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<16;
			_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<8;
			_inoutPutLen +=  (psealData[_startpos+4]& 0xff);
			_DataLenFlags = 4;
			break;
		case 0x84:
			_inoutPutLen  =  (psealData[_startpos+2]& 0xff)<<24;
			_inoutPutLen +=  (psealData[_startpos+3]& 0xff)<<16;
			_inoutPutLen +=  (psealData[_startpos+4]& 0xff)<<8;
			_inoutPutLen +=  (psealData[_startpos+5]& 0xff);
			_DataLenFlags = 5;
			break;
		default:
			_inoutPutLen = (psealData[_startpos +1 ]& 0xff);
			_DataLenFlags = 1;

		}
		_Offset = _DataLenFlags + 1; // the start position included
		psealDataOffset  = _Offset +_startpos; 
		if (_Datatype == 0x30)
			FS_ParseSealData(psealData , psealDataLen , _startpos + _Offset, _inoutPutLen,psealDataOffset,pSealType,sealTypeLen,nCount +1);
		else
			FS_ParseSealData( psealData, psealDataLen,_startpos + _Offset +_inoutPutLen ,_inoutPutLen,psealDataOffset,pSealType,sealTypeLen,nCount);

	}
}

//
FX_BOOL FS_Decode_Sealdata(const unsigned char *buffer, const int& buf_size, FX_LPBYTE sealPic, FX_INT32& sealPicLen,FX_LPBYTE sealType, FX_INT32& sealTypeLen)
{
	FX_INT32 picDataOffset = 0;
	FS_ParseSealData((FX_LPBYTE)buffer,buf_size,0,sealPicLen,picDataOffset,sealType,sealTypeLen,0);

	if (sealPic)
	{
		buffer += picDataOffset;
		memcpy(FX_LPVOID(sealPic),FX_LPVOID(buffer),sealPicLen);
	}
	return TRUE;
}

//
FX_BOOL FS_Ber_Decode_PicSize(const unsigned char *buffer, const int& buf_size, long& width, long& height)
{
	int picDataLen = 0;
	int picDataOffset = 0;
	int picTypeLen = 0;
	width = 0; height = 0;
	int nCount = 0;
	FS_ParseSealData((FX_LPBYTE)buffer, buf_size, 0, picDataLen, picDataOffset, NULL, picTypeLen, nCount);
	int _PicWidthDataLen = 0;
	int _PicHeightDataLen = 0;
	_PicWidthDataLen = buffer[picDataOffset + picDataLen + 1] & 0xff;
	_PicHeightDataLen = buffer[picDataOffset + picDataLen + 1 + _PicWidthDataLen+2] & 0xff;

	for (int i = 0;i < _PicWidthDataLen;i++)
	{
		width += ((buffer[picDataLen + picDataOffset + 2+ i]&0xff)<<(8 *(_PicWidthDataLen -1 -i )));
	}
	for (int j =0; j < _PicHeightDataLen; j++)
	{
		height += ((buffer[picDataOffset + picDataLen + 4 + _PicWidthDataLen +j] & 0xff) << (8 * (_PicHeightDataLen - 1 - j)));
	}
	return TRUE;
}

//
FX_BOOL FS_GetSealImage(OFD_SIGNVERSION eSignVersion, IOFD_FileStream* pSealFile, FX_BOOL bFlag, OFD_SEALTYPE& eType, COFD_StampAnnot* pStampAnnot, CFX_DIBitmap*& pDIB, IOFD_FileStream*& pImageStream)
{
	int res = 0;
	FX_LPBYTE pPictureData = NULL;
	FX_LPBYTE pPictureType = NULL;
	int pictureDataLen = 0;
	int pictureTypeLen = 0;
	FX_INT32 size = pSealFile->GetSize();
	void *buffer = FX_Alloc(FX_BYTE, size);
	FXSYS_memset((FX_LPBYTE*)buffer, 0, size);

	pSealFile->ReadBlock(buffer, (size_t)size);
	//CFS_OFDSignatureManage* pSignManage = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface;// = pSignManage->GetOESInterface(eSignVersion);

	res = pOESInterface->GetSignImage((OFD_BYTE*)buffer, size, NULL, pictureDataLen, NULL, pictureTypeLen);
	if((res != OES_OK) || (pictureDataLen < 1))
	{
		FX_Free(buffer);
		return FALSE;
	}
	pPictureData = FX_Alloc(FX_BYTE, pictureDataLen);
	FXSYS_memset(pPictureData, 0, pictureDataLen);
	pPictureType = FX_Alloc(FX_BYTE, pictureTypeLen + 1);
	FXSYS_memset(pPictureType, 0, pictureTypeLen + 1);

	res = pOESInterface->GetSignImage((OFD_BYTE*)buffer, size, pPictureData, pictureDataLen, pPictureType, pictureTypeLen);
	if (res != OES_OK) 
	{
		FX_Free(buffer);
		FX_Free(pPictureData);
		FX_Free(pPictureType);
		return FALSE;
	}

	FX_Free(buffer);

	CFX_ByteString bsSealType((FX_LPCSTR)pPictureType);
	if (bsSealType.Compare("ofd") == 0) {

		eType = OFD_SEAL_OFD;
		if (bFlag) {
			pImageStream = OFD_CreateMemoryStream(pPictureData, pictureDataLen, TRUE);
		}

	} else {

		eType = OFD_SEAL_BITMAP;
		if (bFlag)
			pDIB = FS_CreateSealBitmap(pPictureData, pictureDataLen);
	}

	if (eType == OFD_SEAL_BITMAP)
		FX_Free(pPictureData);
	FX_Free(pPictureType);

	return TRUE;
}

//
FX_BOOL FS_GetSealImage_(void* pOESInterface_, FX_LPBYTE buffer, FX_STRSIZE size, FX_BOOL bFlag, OFD_SEALTYPE& eType, CFX_DIBitmap*& pDIB, IOFD_FileStream*& pImageStream)
{
	if(pOESInterface_ == NULL || buffer == NULL || size <= 0)
		return FALSE;

	CFS_OESInterface* pOESInterface = (CFS_OESInterface*)pOESInterface_;
	int res = 0;
	FX_LPBYTE pPictureData = NULL;
	FX_LPBYTE pPictureType = NULL;
	int pictureDataLen = 0;
	int pictureTypeLen = 0;

	res = pOESInterface->GetSignImage((OFD_BYTE*)buffer, size, NULL, pictureDataLen, NULL, pictureTypeLen);
	if((res != OES_OK) || (pictureDataLen < 1))
	{
		FX_Free(buffer);
		return FALSE;
	}
	pPictureData = FX_Alloc(FX_BYTE, pictureDataLen);
	FXSYS_memset(pPictureData, 0, pictureDataLen);
	pPictureType = FX_Alloc(FX_BYTE, pictureTypeLen + 1);
	FXSYS_memset(pPictureType, 0, pictureTypeLen + 1);

	res = pOESInterface->GetSignImage((OFD_BYTE*)buffer, size, pPictureData, pictureDataLen, pPictureType, pictureTypeLen);
	if (res != OES_OK) 
	{
		FX_Free(buffer);
		FX_Free(pPictureData);
		FX_Free(pPictureType);
		return FALSE;
	}

	FX_Free(buffer);

	CFX_ByteString bsSealType((FX_LPCSTR)pPictureType);
	if (bsSealType.Compare("ofd") == 0) {

		eType = OFD_SEAL_OFD;
		if (bFlag) {
			pImageStream = OFD_CreateMemoryStream(pPictureData, pictureDataLen, TRUE);
		}

	} else {

		eType = OFD_SEAL_BITMAP;
		if (bFlag)
			pDIB = FS_CreateSealBitmap(pPictureData, pictureDataLen);
	}

	if (eType == OFD_SEAL_BITMAP)
		FX_Free(pPictureData);
	FX_Free(pPictureType);

	return TRUE;
}

#include <string>
using namespace std;
/*
//
CFX_DIBitmap* FS_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize)
{
	IFX_MemoryStream* pMemoryStream = FX_CreateMemoryStream((FX_LPBYTE)pBuf, nSize);
	CFX_DIBitmap* pDIBitmap = FS_LoadImage(pMemoryStream);
	pMemoryStream->Release();

	return pDIBitmap;
}
*/
bool ParseSignedvalueImage(const unsigned char *pBuf, const int nBufSize, unsigned char *&pImageDataBuf, int &nImageSize2Res, int *pImgSubType)
{
	std::string strError = "";
	bool bRes = false;

	{
		if (pBuf == NULL || nBufSize <= 0){
			strError = "parameter error";
			goto end;
		}

		int nOffset = 0;
		int nOffsetType = 0;
		
		while (nOffset < nBufSize){

			nOffsetType = nOffset + 1;
			if (nOffsetType >= nBufSize){
				bRes = false;
				goto end;
			}

			//SEQUENCE Tag = 0x30
			//�����롾>128�� 0x81 : length 1 byte , 0x82 : length 2 byte, 0x83 : length 3 byte, 0x84 : length 4 byte.
			//4 byte, 32λ �� 2147483647 , 21�ڣ���ǰ�涨�����ֵ��
			//ͼƬ���ݣ�ע���ǳ�����
			int nType = 0;
			if (pBuf[nOffset] == 0x30){
				if (pBuf[nOffsetType] == 0x81 ||
					pBuf[nOffsetType] == 0x82 ||
					pBuf[nOffsetType] == 0x83 ||
					pBuf[nOffsetType] == 0x84)
				{
#ifdef _DEBUG
					if (nOffset >= 730){
						int nTemp = 0;
						//Debug
					}
#endif
					switch (pBuf[nOffsetType])
					{
					case 0x81:
						{
							nType = nOffsetType + 1;
						}
						break;
					case 0x82:
						{
							nType = nOffsetType + 2;
						}
						break;
					case 0x83:
						{
							nType = nOffsetType + 3;
						}
						break;
					case 0x84:
						{
							nType = nOffsetType + 4;
						}
						break;
					}
					nType += 1;
					if (nType >= nBufSize){
						bRes = false;
						goto end;
					}
#ifdef _DEBUG
					if (nOffset >= 730){
						int nTemp = 0;
						//Debug
					}
#endif
					if (pBuf[nType] == 0x16){//IA5String , ASN�ı�����0x16
						int nTypeLen = pBuf[nType + 1];	//ͼƬ��ʽ���Ͳ��᳤

						int nTempOffset = nType + 2 + nTypeLen;

						{
							//ͼƬ��ʽ������
							char imgType[260];
							memset(imgType, 0, 260);
							memcpy(imgType, pBuf + nType + 2, nTypeLen);
							std::string strSubType = imgType;
							if (strSubType == "ofd"){
								*pImgSubType = OFD_SEAL_OFD;
							}
						}

						if (nTempOffset >= nBufSize){
							bRes = false;
							goto end;
						}
						
						if (pBuf[nTempOffset] == 0x04){//�������ȷ��������ͼƬ���ݡ�
							int nPicDataLenOC = nTempOffset + 1;	//OCTET STRING �� ASN�ı�����0x04
							if (nPicDataLenOC >= nBufSize){
								bRes = false;
								goto end;
							}
							int nPicLenOffset = 0;
							switch (pBuf[nPicDataLenOC])
							{
							case 0x81:
								nPicLenOffset = 1;
								break;
							case 0x82:
								nPicLenOffset = 2;
								break;
							case 0x83:
								nPicLenOffset = 3;
								break;
							case 0x84:
								nPicLenOffset = 4;
								break;
							}
							if (nPicDataLenOC + nPicLenOffset  + 1 > nBufSize){
								bRes = false;
								goto end;
							}
							//����ͼƬ���ݳ���
							//���� ���� ͼƬ�ĳ��ȱ�Ȼ ���� 128�� ���ڳ�����
							int nPicDataLen = 0;
							switch (nPicLenOffset)
							{
							case 1:
								nPicDataLen = pBuf[nPicDataLenOC + 1] & 0xff;
								break;
							case 2:
								nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 8) | (pBuf[nPicDataLenOC + 2] & 0xff);
								break;
							case 3:
								nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 16) | ((pBuf[nPicDataLenOC + 2] & 0xff) << 8) |
									(pBuf[nPicDataLenOC + 3] & 0xff);
								break;
							case 4:
								nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 24) | ((pBuf[nPicDataLenOC + 2] & 0xff) << 16) |
									((pBuf[nPicDataLenOC + 3] & 0xff) << 8) | (pBuf[nPicDataLenOC + 4] & 0xff);
								break;
							}

							int nPicDataOffset = nPicDataLenOC + nPicLenOffset + 1;
							if ((nPicDataOffset + nPicDataLen) >= nBufSize){
								bRes = false;
								goto end;
							}
							nImageSize2Res = nPicDataLen;
							pImageDataBuf = new unsigned char[nPicDataLen];
							memset(pImageDataBuf, 0, nPicDataLen);
							memcpy(pImageDataBuf, (pBuf + nPicDataOffset), nPicDataLen);
							bRes = true;
							goto end;
						}
					}
				}
			}
			nOffset++;
		}

	}

end:

	return bRes;
}

//
CFX_DIBitmap * CreateVectorBitmap(IOFD_FileStream* pOFDFileStream,int width ,int height)
{
	if (!pOFDFileStream) return NULL;
	IOFD_FilePackage* pOFDPackage = OFD_FilePackage_Create(pOFDFileStream);
	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage);
	if (!pOFDParser) 
	{
		pOFDPackage->Release();
		pOFDFileStream->Release();
		return NULL;
	}
	if (pOFDParser->CountDocuments() == 0)
	{	
		pOFDParser->Release();
		pOFDPackage->Release();
		pOFDFileStream->Release();
		return NULL;
	}
	IOFD_Document* pDoc = pOFDParser->GetDocument(0);
	if (!pDoc){
		pOFDParser->Release();
		pOFDPackage->Release();
		pOFDFileStream->Release();
		return NULL;
	}

	IOFD_Page *pPage = pDoc->GetPage(0);
	pPage->LoadPage();
	CFX_AffineMatrix matrix;
	CFX_RectF pPageRect,physicalPageRect;
    FX_INT32 nRotate = (pPage->GetPageArea().GetRotate() / 90) % 4;
    pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_CropBox,pPageRect);
    pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox,physicalPageRect);
	CFX_Rect  deviceRect;
        
    CFX_AffineMatrix mtOffset;
    mtOffset.Translate(physicalPageRect.left - pPageRect.left, physicalPageRect.top - pPageRect.top);

	deviceRect.Set(0,0,width, height);
	OFD_GetPageMatrix(matrix, pPageRect, deviceRect, nRotate);
    matrix.Concat(mtOffset, TRUE);

	IOFD_RenderContext * pContentext = IOFD_RenderContext::Create();
	pContentext->AddPage(pPage,&matrix);

	CFX_DIBitmap *pDiBitMap = new CFX_DIBitmap;
	pDiBitMap->Create(FXSYS_round(width),FXSYS_round(height),FXDIB_Argb);
	pDiBitMap->Clear(0xFFFFFFFF);

	CFX_FxgeDevice * pFXgeDevice = new CFX_FxgeDevice;
	pFXgeDevice->Attach(pDiBitMap);

	COFD_RenderDevice  OFDRenderDevice;
	OFDRenderDevice.Create(pFXgeDevice);

	IOFD_ProgressiveRenderer * pProgressRender = IOFD_ProgressiveRenderer::Create();

	pProgressRender->StartRender(&OFDRenderDevice,pContentext,NULL,&matrix);

	pProgressRender->DoRender(NULL);
	pProgressRender->StopRender();
	delete pFXgeDevice;
	pContentext->Release();
	pProgressRender->Release();
	pPage->UnloadPage();
	pDoc->Release();
	pOFDParser->Release();
	pOFDPackage->Release();
	pOFDFileStream->Release();
	return  pDiBitMap;
}

//
CFX_DIBitmap*   RenderVetorBitmap(FX_LPBYTE pdata, FX_INT32 len,int width,int height)
{
	if (!pdata || len <= 0)
		return NULL;
// 	CRect rect;
// 	m_wndPic.GetClientRect(&rect);
// 	if (rect.IsRectEmpty())
// 		return NULL;

	IOFD_FileStream * pOFDFileStream = OFD_CreateMemoryStream(pdata,len);
	if (!pOFDFileStream)  return NULL;

	//return CreateVectorBitmap(pOFDFileStream,width,height/*rect.Height(),rect.Height()*/);//otherwise picture will stretch
	return CreateVectorBitmap(pOFDFileStream,245,245);//client height
}

//
CFX_DIBitmap* GetSignedValueBitmap(const unsigned char *pSignBuf, int nLen)
{
	unsigned char *pPicBuf = NULL;
	int nPicLen = 0;
	CFX_DIBitmap *pStampBMP = NULL;
	int nImgSubType = -1;
	std::string strErr = "";

	{
		if (pSignBuf == NULL || nLen <= 0){
			strErr = "parameter error";
			goto end;
		}

		bool bParseImage = ParseSignedvalueImage(pSignBuf, nLen, pPicBuf, nPicLen, &nImgSubType);
		if (!bParseImage || (pPicBuf == NULL) || (nPicLen <= 0)){
			strErr = "parse failed";
			goto end;
		}

		//pStampBMP = CreateSealBitmap(pPicBuf, nPicLen);
		if(nImgSubType == OFD_SEAL_OFD)
		{
			pStampBMP = RenderVetorBitmap(pPicBuf, nPicLen, 245, 245);
		}
		else
		{
			pStampBMP = FS_CreateSealBitmap(pPicBuf, nPicLen);
		}

		if (pStampBMP == NULL)
			strErr = "create bitmap failed";
	}

end:

	if (pPicBuf != NULL){
		delete []pPicBuf;
		pPicBuf = NULL;
	}
	return pStampBMP;
}
