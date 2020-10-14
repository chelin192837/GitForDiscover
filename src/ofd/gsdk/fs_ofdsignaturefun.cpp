#include "fxlib.h"
#include "include/fs_ofdsdkmgr.h"
#include "include/fs_ofdsignature.h"
#include "include/fs_ofdsignaturefun.h"
#include "include/fs_ofdsignaturemanage.h"
#include "include/fs_ofdsignaturefactory.h"

#include "include/fs_ofddocument.h"
#include "include/fs_ofdlayer.h"
#include "include/fs_ofdblockobject.h"
#include "include/fs_ofdannot.h"
#include "include/fs_ofdimageobject.h"

#include "gmsign/sm3.h"
#include "gmsign/fs_ofdsignaturegmhandler.h"
#include "../int/ofd_es.h"


#if _FX_OS_ == _FX_LINUX_DESKTOP_
#include "x509.h"
#include "pkcs12.h"
#include "pkcs7.h"
#endif
#include "openssl/x509.h"
#include "openssl/pkcs12.h"
#include "openssl/pkcs7.h"

#include "openssl/evp.h"
#include "openssl/ossl_typ.h"
//#include "x509.h"

#if _FX_PRODUCT_ == _FS_ESSDK_ || _FX_PRODUCT_ == _FS_EOSDK_ || _FX_PRODUCT_ == _FS_NONE_ || _FX_PRODUCT_ == _FS_EISDK_ || _FX_PRODUCT_ == _FS_ASDK_/*20190813*/ || _FX_PRODUCT_ == _FS_GSDK_

//
#define OFD_SG_SIGN_METHOD		"SGD_SM2_1"	//ǩ���㷨
#define OFD_SG_DIGEST_METHOD	"SGD_SM3"   //ժҪ�㷨����SM2���ʹ��

#define _OFD_SG_SIGN_GM_					//����ǩ���㷨

static CFS_OFDSignatureManage* pSignManage = NULL;
//
int FS_LoadSealModule(CFX_WideString& wsOESFile, CFX_ByteString bsPin)
{
	if(!pSignManage)
		pSignManage = new CFS_OFDSignatureManage;
	return pSignManage->LoadSealModule(wsOESFile, bsPin);
}
FX_DWORD FS_SignOFD_KTPOS(FX_LPVOID psealInfo, IOFD_WriteDocument* pWriteDoc, int* pPageArr, int nSize, int nSubRange, float x, float y, IFX_FileWrite* pFileWrite, FX_BOOL bLocked)
{
	if(pWriteDoc == NULL)
		return 10;

	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)psealInfo;

	IOFD_Document* doc = pWriteDoc->GetReadDoc();
	COFD_Signatures* sigs = doc->GetSignatures();
	COFD_WriteSignatures* wSigns = pWriteDoc->CreateSignatures();
	COFD_WriteSignature* wSeal = wSigns->InsertSignature(pWriteDoc, -1);

	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();

	//
	CFX_ByteString bsSignTime;
	if(!pOESInterface->IsEmptyGetSignDataTime())
	{
		CFX_WideString wsSignTime = FS_GetSignDataTime();
		wSeal->SetSignatureDateTime(wsSignTime);

		bsSignTime = wsSignTime.UTF8Encode();
	}
	wSeal->SetCompany(sealInfo->company);
	wSeal->SetProviderName(sealInfo->name);
	wSeal->SetVersion(sealInfo->version);
	if(!pOESInterface->IsEmptyGetSeal())
		wSeal->SetSealFile(pWriteDoc, sealInfo->sealStream);

	wSeal->SetSignLocked(bLocked);

	//
	//CFX_RectF rect;
	//FX_FLOAT left = x - sealInfo->width / 2;
	//FX_FLOAT top = y - sealInfo->height / 2;
	//rect.Set((FX_FLOAT)left, (FX_FLOAT)top, (FX_FLOAT)sealInfo->width, (FX_FLOAT)sealInfo->height);
	//COFD_WriteStampAnnot* pStampAnnot;// = wSeal->InsertStampAnnot(pWriteDoc, pageIndex, -1);
	//pStampAnnot->SetBoundary(rect);
/*
	// 发票签章，要求每一页都要做特定的位置显示章的图片
	if(posx != -1000 && posy != -1000)
	{
		CFX_RectF rect;
		rect.Set(posx, posy, (FX_FLOAT)sealInfo->width, (FX_FLOAT)sealInfo->height);
		FX_INT32 nCount = doc->CountPages();
		for(int i = 0; i < nCount; i++)
		{
			if(i == pageIndex)
				continue;

			COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, i, -1);

			pStampAnnot->SetBoundary(rect);
		}
	}
*/

	CFX_RectF rect;
	FX_FLOAT left = x - sealInfo->width / 2;
	FX_FLOAT top = y - sealInfo->height / 2;
	rect.Set((FX_FLOAT)left, (FX_FLOAT)top, (FX_FLOAT)sealInfo->width, (FX_FLOAT)sealInfo->height);

	if(pPageArr == NULL)
	{
		IOFD_Document* pOFDDoc = pWriteDoc->GetReadDoc();
		int nCount = pOFDDoc->CountPages();
		for(int i = 0; i < nCount; i++)
		{
			if((nSubRange == 1 && i % 2 != 0)		// 奇数页
			   || (nSubRange == 2 && i % 2 == 0))	// 偶数页
				continue;


			COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, i, -1);
            pStampAnnot->SetSignature(wSeal->GetReadSignature());
			pStampAnnot->SetBoundary(rect);
		}
	}
	else
	{
		for(int i = 0; i < nSize; i++)
		{
			if((nSubRange == 1 && i % 2 != 0)		// 奇数页
			   || (nSubRange == 2 && i % 2 == 0))	// 偶数页
				continue;

			COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, pPageArr[i], -1);
            pStampAnnot->SetSignature(wSeal->GetReadSignature());
			pStampAnnot->SetBoundary(rect);
		}
	}
	//
	if(!pOESInterface->IsEmptyGetSignMethod())
		wSeal->SetSignatureMethod(sealInfo->signMethod.UTF8Decode());
	wSeal->SetRefCheckMethod(sealInfo->digestMethod.UTF8Decode());
	//WriteLog_GSDK("FS_SignOFD sealInfo->signMethod : ", sealInfo->signMethod);
	//WriteLog_GSDK("FS_SignOFD sealInfo->digestMethod : ", sealInfo->digestMethod);
	IOFD_SignHandler* phSign = OFD_SignHandler_Create(pWriteDoc);
	phSign->StartSign(pFileWrite, wSeal);

	FX_INT32 refersCount = wSeal->CountReferences();
	//WriteLog_GSDK("FS_SignOFD ", "refersCount : ", refersCount);
	FX_LPBYTE pbTxts = NULL;
	unsigned int cbTxts = 0;
	FX_LPBYTE pbSign = NULL;
	int cbSign = 0;
	FX_DWORD offer = 0;

	// 摘要
	FX_LPBYTE lpDigestData = NULL;
	int cpDigestData = 0;
	for (int j = 0; j < refersCount; j++)
	{
		COFD_WriteSignReference* refer = wSeal->GetReference(j);
		IFX_FileRead* file = refer->GetFile();
		FX_DWORD fileSize = file->GetSize();
		FX_LPBYTE tempData = FX_Alloc(FX_BYTE, fileSize);
		memset(tempData, 0, fileSize);
		file->ReadBlock((FX_LPVOID)(tempData), 0, fileSize);
		//WriteLog_GSDK("FS_SignOFD ", "fileSize : ", fileSize);
		// 对每个refer进行Hash
		FX_DWORD dwRes = pOESInterface->Digest(tempData, fileSize, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
											   sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
		//WriteLog_GSDK("FS_SignOFD ", "dwRes : ", dwRes);
		if (OES_OK == dwRes)
		{
			lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
			memset(lpDigestData, 0, cpDigestData);
			dwRes = pOESInterface->Digest(tempData, fileSize, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
										  sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
		}
		//WriteLog_GSDK("FS_SignOFD ", "dwRes 2 : ", dwRes);
				FXSYS_assert(OES_OK == dwRes);

		CFX_WideString wsCheckMethod;
		CFX_Base64Encoder base64Encoder;
		base64Encoder.Encode(CFX_ByteString((unsigned char *)lpDigestData, cpDigestData), wsCheckMethod);
		refer->SetCheckValue(wsCheckMethod);
		FX_Free(tempData);
		FX_Free(lpDigestData);
	}

	lpDigestData = NULL;

	IFX_FileRead* fileSignature = phSign->GetSignatureFile();
	FX_DWORD dwSignfileSize = fileSignature->GetSize();
	//WriteLog_GSDK("FS_SignOFD ", "dwSignfileSize : ", dwSignfileSize);
	cbTxts = dwSignfileSize;
	pbTxts = FX_Alloc(FX_BYTE, cbTxts);
	memset((FX_LPVOID)pbTxts, 0, cbTxts);

	fileSignature->ReadBlock((FX_LPVOID)(pbTxts + offer), 0, dwSignfileSize);
	fileSignature->Release();

	// 对整个数据计算摘要
	FX_DWORD dwRes = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
										   sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
	//WriteLog_GSDK("FS_SignOFD ", "dwRes 3 : ", dwRes);
	if (OES_OK == dwRes)
	{
		lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
		memset(lpDigestData, 0, cpDigestData);
		dwRes = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
									  sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
	}
	//WriteLog_GSDK("FS_SignOFD ", "dwRes 4 : ", dwRes);
			FXSYS_assert(OES_OK == dwRes);

	// Sign
	CFX_ByteString bsSealID = sealInfo->sealId;
	CFX_WideString fileName = ((IOFD_FileStream *)fileSignature)->GetCurrentFileName(0);
	CFX_ByteString docProperty = CFX_ByteString::FromUnicode(fileName);
	if (docProperty[0] != '/')
		docProperty.Insert(0, '/');

	FX_DWORD dwSign = pOESInterface->Sign(
			(FX_LPBYTE)(FX_LPCBYTE)bsSealID,
			bsSealID.GetLength(),
			(FX_LPBYTE)(FX_LPCBYTE)docProperty,
			docProperty.GetLength(),
			(FX_LPBYTE)lpDigestData,
			cpDigestData,
			(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
			sealInfo->signMethod.GetLength(),
			(unsigned char*)(FX_LPCBYTE)bsSignTime,
			bsSignTime.GetLength(),
			NULL,
			cbSign);
	//WriteLog_GSDK("FS_SignOFD ", "dwSign 1 : ", dwSign);
	if (OES_OK == dwSign)
	{
		pbSign = FX_Alloc(FX_BYTE, cbSign);
		memset(pbSign, 0, cbSign);

		dwSign = pOESInterface->Sign(
				(FX_LPBYTE)(FX_LPCBYTE)bsSealID,
				bsSealID.GetLength(),
				(FX_LPBYTE)(FX_LPCBYTE)docProperty,
				docProperty.GetLength(),
				(FX_LPBYTE)lpDigestData,
				cpDigestData,
				(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
				sealInfo->signMethod.GetLength(),
				(unsigned char*)(FX_LPCBYTE)bsSignTime,
				bsSignTime.GetLength(),
				pbSign,
				cbSign);
	}
	//WriteLog_GSDK("FS_SignOFD ", "dwSign 2 : ", dwSign);
	CFX_ByteString signValue = CFX_ByteString(pbSign, cbSign);
	if (pbTxts) FX_Free(pbTxts);
	if (lpDigestData) FX_Free(lpDigestData);
	if (pbSign) FX_Free(pbSign);
	//WriteLog_GSDK("FS_SignOFD ", "dwSign 3 : ", dwSign);
	if (OES_OK != dwSign)
	{
		FX_INT32 index = ((COFD_Signatures *)wSigns)->CountSignature() - 1;
		//if (index > -1)
		//	wSigns->RemoveSignature(pWriteDoc, pageIndex, index);
	}
	else
		phSign->SetSignedValue(signValue);

	phSign->EndSign();
	phSign->Release();
	pFileWrite->Flush();
	//WriteLog_GSDK("FS_SignOFD ", "555 ...");
	return dwSign;
}
//
void FS_FreeSealModule()
{
	if(pSignManage) {
		pSignManage->FreeSealModule();
		pSignManage = NULL;
	}
}

CFX_ByteString FS_GetSealList() {
    unsigned int res = OES_OK;
    int nSealListLen = 0;

    //CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
    CFS_OESInterface *pOESInterface = pSignManage->GetOESInterface();
    res = pOESInterface->GetSealList(NULL, nSealListLen);
    if (res != OES_OK) {
        int ErrMessageLen = 0;
        pOESInterface->GetErrMessage(res, NULL, ErrMessageLen);

        FX_LPBYTE lpErrMessage = FX_Alloc(FX_BYTE, ErrMessageLen + 1);
        memset(lpErrMessage, 0, ErrMessageLen + 1);
        pOESInterface->GetErrMessage(res, lpErrMessage, ErrMessageLen);

        CFX_WideString wsTmp = CFX_WideString::FromUTF8((const char *) lpErrMessage, ErrMessageLen);
        FX_Free(lpErrMessage);
        lpErrMessage = NULL;
        return CFX_ByteString();
    }

    //���� ID1\0Name1\0ID2\0Name2\0\0
    FX_LPBYTE pbStampList = FX_Alloc(FX_BYTE, nSealListLen);
    memset(pbStampList, 0, nSealListLen);
    res = pOESInterface->GetSealList(pbStampList, nSealListLen);
    CFX_ByteString bsTemp(pbStampList, nSealListLen);
    return bsTemp;
}
//
static FX_BOOL FS_FindSeal(FX_BOOL bSealID, CFX_ByteString& bsSeal, CFX_ByteString& bsSealID)
{
	unsigned int res = OES_OK;
	int nSealListLen = 0;

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
	res = pOESInterface->GetSealList(NULL, nSealListLen);
	if (res != OES_OK)
	{
		int ErrMessageLen = 0;
		pOESInterface->GetErrMessage(res, NULL, ErrMessageLen);

		FX_LPBYTE lpErrMessage = FX_Alloc(FX_BYTE, ErrMessageLen + 1);
		memset(lpErrMessage, 0, ErrMessageLen + 1);
		pOESInterface->GetErrMessage(res, lpErrMessage, ErrMessageLen);

		CFX_WideString wsTmp = CFX_WideString::FromUTF8((const char*)lpErrMessage, ErrMessageLen);
		FX_Free(lpErrMessage);
		lpErrMessage = NULL;
		return FALSE;
	}

	//���� ID1\0Name1\0ID2\0Name2\0\0
	FX_LPBYTE pbStampList = FX_Alloc(FX_BYTE, nSealListLen);
	memset(pbStampList, 0, nSealListLen);
	res = pOESInterface->GetSealList(pbStampList, nSealListLen);

	FX_BOOL bFindSeal = FALSE;
	FX_BOOL bCurID = TRUE;
	CFX_ByteString bsTemp;
	for (int i = 0; i < nSealListLen; i++)
	{
		if (pbStampList[i] == '\0')
		{
			if (bCurID)
			{
				bsSealID = bsTemp;
			}

			if (bsSeal.IsEmpty())
			{
				bFindSeal = TRUE;
				break;
			}
			LOGD("ID:%s,search:%s,bID:%d", bsTemp.GetBuffer(10), bsSeal.GetBuffer(10), bSealID);
			if ((bsTemp == bsSeal) && bSealID)
			{
				bFindSeal = TRUE;
				break;
			}

			bSealID = !bSealID;
			bCurID = !bCurID;
			bsTemp.Empty();
		}
		else
			bsTemp += pbStampList[i];
	}
	FX_Free(pbStampList);
	pbStampList = NULL;

	if (!bFindSeal)
		bsSealID.Empty();

	return bFindSeal;
}

//
static int FS_LoadSealData(const CFX_ByteString& bsSealID, FS_SEAL_INFO* sealInfo)
{
	int res = OES_OK;
	FX_LPBYTE sealData = NULL;
	int sealDataLen = 0;

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();

	if(pOESInterface->IsEmptyGetSeal())
		return res;
	res = pOESInterface->GetSeal(bsSealID, NULL, sealDataLen);
	if (OES_OK == res)
	{
		sealData = FX_Alloc(FX_BYTE, sealDataLen + 1);
		memset(sealData, 0, sealDataLen+1);

		res = pOESInterface->GetSeal(bsSealID, sealData, sealDataLen);
	}

	if (OES_OK == res)
	{
		LOGD("FS_LoadSealData");
		sealInfo->sealData = CFX_ByteString(sealData, sealDataLen);
		sealInfo->sealStream = OFD_CreateMemoryStream(sealData, sealDataLen, TRUE);
	}

	return res;
}

void FS_GetSealImage_raw(FS_SEAL_INFO* sealInfo, FX_LPBYTE &pPictureData,FX_LPBYTE &pPictureType, FX_INT32 &pictureDataLen, FX_INT32 &pictureTypeLen) {
    FX_INT32 width = 0;
    FX_INT32 height = 0;
    int res = 0;
    pictureTypeLen = 0;
    //CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
    CFS_OESInterface *pOESInterface = pSignManage->GetOESInterface();
    LOGD("pOESInterface:%d", pOESInterface);
    res = pOESInterface->GetSealImage(sealInfo->sealData, OES_SEALIMAGE_FLAG_DISPLAY,
                                      sealInfo->sealId, NULL, pictureDataLen, NULL, pictureTypeLen,
                                      width, height);
    if (pictureDataLen < 1)
        return;
    pPictureData = FX_Alloc(FX_BYTE, pictureDataLen);
    LOGD("pPictureData:%d", pPictureData);
    FXSYS_memset(pPictureData, 0, pictureDataLen);
    if (pictureTypeLen != 0) {
        pPictureType = FX_Alloc(FX_BYTE, pictureTypeLen + 1);
        LOGD("pictureTypeLen:%d", pPictureType);
        FXSYS_memset(pPictureType, 0, pictureTypeLen + 1);
    }
	res = pOESInterface->GetSealImage(sealInfo->sealData, OES_SEALIMAGE_FLAG_DISPLAY, sealInfo->sealId, pPictureData, pictureDataLen, pPictureType, pictureTypeLen, width, height);
}

static void FS_GetSealImage(FS_SEAL_INFO* sealInfo)
{
	int res = 0;
	FX_LPBYTE pPictureData = NULL;
	FX_LPBYTE pPictureType = NULL;
	FX_INT32 pictureDataLen = 0;
	FX_INT32 pictureTypeLen = 0;
	FX_INT32 width = 0;
	FX_INT32 height = 0;

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
	res = pOESInterface->GetSealImage(sealInfo->sealData, OES_SEALIMAGE_FLAG_DISPLAY, sealInfo->sealId, NULL, pictureDataLen, NULL, pictureTypeLen, width, height);
	if (pictureDataLen < 1)
		return;
	pPictureData = FX_Alloc(FX_BYTE, pictureDataLen);
	FXSYS_memset(pPictureData, 0, pictureDataLen);
    if(pictureTypeLen != 0) {
        pPictureType = FX_Alloc(FX_BYTE, pictureTypeLen + 1);
        FXSYS_memset(pPictureType, 0, pictureTypeLen + 1);
    }
	sealInfo->width = width;
	sealInfo->height = height;
	
	res = pOESInterface->GetSealImage(sealInfo->sealData, OES_SEALIMAGE_FLAG_DISPLAY, sealInfo->sealId, pPictureData, pictureDataLen, pPictureType, pictureTypeLen, width, height);
	CFX_ByteString bsSealType((FX_LPCSTR)pPictureType);
	if (bsSealType.Compare("ofd") == 0) {
		sealInfo->sealImageType = OFD_SEAL_OFD;
		sealInfo->sealImageStream = OFD_CreateMemoryStream(pPictureData, pictureDataLen, TRUE);
	} else {
		//V1 no picture type,pPictureType is not null
		sealInfo->sealImageType = OFD_SEAL_BITMAP;
		//sealInfo->sealBmp = FS_CreateSealBitmap(pPictureData, pictureDataLen);
		// added by xhj for fa piao stemp annot
		//sealInfo->sealImageStream = OFD_CreateMemoryStream(pPictureData, pictureDataLen, TRUE);

		FX_Free(pPictureData);
		pPictureData = NULL;
	}

	FX_Free(pPictureType);
	pPictureType = NULL;
}

//��checkSeallist == 1���� seal list ��ȡӡ��ID���� bsSeal Ϊ�գ�bSealID Ϊ FALSE
FX_LPVOID FS_LoadSealInfo(CFX_ByteString& bsSeal, FX_BOOL bSealID, int bCheckSeallist, int* err)
{
    FS_SEAL_INFO* sealInfo = new FS_SEAL_INFO;
    // Sign Info
    FX_LPBYTE name  = NULL; int nameLen = 0;
    FX_LPBYTE version = NULL; int versionLen = 0;
    FX_LPBYTE company = NULL; int companyLen = 0;
    FX_LPBYTE extend = NULL; int extendLen = 0;

    CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();

    FX_DWORD dwRes = pOESInterface->GetProviderInfo(NULL, nameLen, NULL, companyLen, NULL, versionLen, NULL, extendLen);
    if (OES_OK == dwRes) {
        name = FX_Alloc(FX_BYTE, nameLen  + 1);// �����һЩ�ڴ棬��Ϊ�ײ�д���ݿ��������
        memset(name, 0, nameLen  + 1);
        company = FX_Alloc(FX_BYTE, companyLen  + 1);
        memset(company, 0, companyLen  + 1);
        version = FX_Alloc(FX_BYTE, versionLen  + 1);
        memset(version, 0, versionLen  + 1);
        extend = FX_Alloc(FX_BYTE, extendLen  + 1);
        memset(extend, 0, extendLen  + 1);

        dwRes = pOESInterface->GetProviderInfo(name, nameLen, company, companyLen, version, versionLen, extend, extendLen);
        if (OES_OK == dwRes) {
            sealInfo->version = CFX_WideString::FromUTF8((FX_LPCSTR)version, versionLen);
            sealInfo->name = CFX_WideString::FromUTF8((FX_LPCSTR)name, nameLen);
            sealInfo->company = CFX_WideString::FromUTF8((FX_LPCSTR)company, companyLen);
            LOGD("version:%s,name:%s,company:%s", version, name, company);
        }
        if (name) FX_Free(name);
        if (company) FX_Free(company);
        if (version) FX_Free(version);
        if (extend) FX_Free(extend);
    }
	CFX_ByteString bsSealID;
	if (bCheckSeallist)		// seal list ,get seal id
	{
		if (!FS_FindSeal(bSealID, bsSeal, bsSealID))
		{
			LOGD("OFD_ERRCODE_NoSuchSeal");
			if (err) *err = OFD_ERRCODE_NoSuchSeal;
			return NULL;
		}
	}
	else if (bSealID)		// bsSeal is seal id
	{
		bsSealID = bsSeal;
	}
	else
	{
		if (err) *err = OFD_ERRCODE_NoSuchSeal;
		return NULL;
	}
	{
		sealInfo->sealImageType = OFD_SEAL_BITMAP;
		sealInfo->sealBmp = NULL;
		sealInfo->sealImageStream = NULL;
		sealInfo->sealStream = NULL;
		sealInfo->width = 0;
		sealInfo->height = 0;
	}

	// Seal Info
	if ((dwRes = FS_LoadSealData(bsSealID, sealInfo)) != 0) {
		FS_UnLoadSealInfo(sealInfo);
		if (err) *err = dwRes;//OFD_ERRCODE_LoadSealData
		return NULL;
	}

	sealInfo->sealId = bsSealID;
	FS_GetSealImage(sealInfo);

	// Algorithm Info
	FX_LPBYTE digestMethod = NULL;
	int digestMethodLen = 0;
	if (OES_OK == dwRes)
		dwRes = pOESInterface->GetDigestMethod(NULL, digestMethodLen);
	if (OES_OK == dwRes) {
		digestMethod = FX_Alloc(FX_BYTE, digestMethodLen + 1);
		memset(digestMethod, 0, digestMethodLen + 1);
		dwRes = pOESInterface->GetDigestMethod(digestMethod, digestMethodLen);
		if (OES_OK == dwRes)
			sealInfo->digestMethod = CFX_ByteString((FX_LPCSTR)digestMethod, digestMethodLen);
		FX_Free(digestMethod);
	}
	if (OES_OK != dwRes)
		sealInfo->digestMethod = OFD_SG_DIGEST_METHOD;

	if(pOESInterface->IsEmptyGetSignMethod()){
		sealInfo->signMethod = OFD_SG_SIGN_METHOD;
	}else{
		FX_LPBYTE signMethod = NULL;
		int signMethodLen = 0;
		if (OES_OK == dwRes)
			dwRes = pOESInterface->GetSignMethod(NULL, signMethodLen);
		if (OES_OK == dwRes) {
			signMethod = FX_Alloc(FX_BYTE, signMethodLen + 1);
			memset(signMethod, 0, signMethodLen + 1);
			dwRes = pOESInterface->GetSignMethod(signMethod, signMethodLen);
			if (OES_OK == dwRes)
				sealInfo->signMethod = CFX_ByteString((FX_LPCSTR)signMethod, signMethodLen);
			FX_Free(signMethod);
		}
		if (OES_OK != dwRes)
			sealInfo->signMethod = OFD_SG_SIGN_METHOD;
	}

	if (err) *err = dwRes;
	LOGD("FS_LoadSealInfo:%d",sealInfo);
	return sealInfo;
}

//
void FS_UnLoadSealInfo(FX_LPVOID sealInfo)
{
	if (!sealInfo)
		return;
	FS_SEAL_INFO* pSealInfo = (FS_SEAL_INFO*)sealInfo;
	// pSealInfo->sealBmp Release By COFD_DocProvider.
	if (pSealInfo->sealBmp)
		delete pSealInfo->sealBmp;	
	pSealInfo->sealBmp = NULL;
	if(pSealInfo->sealImageStream)
		pSealInfo->sealImageStream->Release();
	pSealInfo->sealImageStream = NULL;
	if (pSealInfo->sealStream)
		pSealInfo->sealStream->Release();
	pSealInfo->sealStream = NULL;
	pSealInfo->width = 0;
	pSealInfo->height = 0;
	delete pSealInfo;
}

//
CFX_WideString FS_GetSignDataTime()
{
	CFX_WideString wsSignTime;
	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
	if(pOESInterface && !pOESInterface->IsEmptyGetSignDataTime())
	{
		FX_LPBYTE dataTimeBuf = NULL;
		int dataTimeBufLen = 0;
		FX_DWORD dwRes = pOESInterface->GetSignDataTime(NULL, dataTimeBufLen);
		if (OES_OK == dwRes) {
			dataTimeBuf = FX_Alloc(FX_BYTE, dataTimeBufLen + 1);
			memset(dataTimeBuf, 0, dataTimeBufLen + 1);
			dwRes = pOESInterface->GetSignDataTime(dataTimeBuf, dataTimeBufLen); 
			if (OES_OK == dwRes)
				wsSignTime = CFX_WideString::FromUTF8((FX_LPCSTR)dataTimeBuf, dataTimeBufLen);
			FX_Free(dataTimeBuf);
		}
	}
	return wsSignTime;
}

//
FX_DWORD FS_SignOFD(IOFD_Document* doc, FX_LPVOID psealInfo, IOFD_WriteDocument* pWriteDoc, int pageIndex, float x, float y, IFX_FileWrite* pFileWrite, FX_BOOL bLocked, /*fa piao*/int posx, /*fa piao*/int posy)
{
	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)psealInfo;

	COFD_Signatures* sigs = doc->GetSignatures();
	COFD_WriteSignatures* wSigns = pWriteDoc->CreateSignatures();
	COFD_WriteSignature* wSeal = wSigns->InsertSignature(pWriteDoc, -1);

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();

	//
	CFX_ByteString bsSignTime;
	if(!pOESInterface->IsEmptyGetSignDataTime())
	{
		CFX_WideString wsSignTime = FS_GetSignDataTime();
		wSeal->SetSignatureDateTime(wsSignTime);

		bsSignTime = wsSignTime.UTF8Encode();
	}
	wSeal->SetCompany(sealInfo->company);
	wSeal->SetProviderName(sealInfo->name);	
	wSeal->SetVersion(sealInfo->version);
	if(!pOESInterface->IsEmptyGetSeal())
		wSeal->SetSealFile(pWriteDoc, sealInfo->sealStream);

	wSeal->SetSignLocked(bLocked);

	//
	CFX_RectF rect;
	FX_FLOAT left = x - sealInfo->width / 2;
	FX_FLOAT top = y - sealInfo->height / 2;
	rect.Set((FX_FLOAT)left, (FX_FLOAT)top, (FX_FLOAT)sealInfo->width, (FX_FLOAT)sealInfo->height);
	COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, pageIndex, -1);
    pStampAnnot->SetSignature(wSeal->GetReadSignature());
	pStampAnnot->SetBoundary(rect);

	// ��Ʊǩ�£�Ҫ��ÿһҳ��Ҫ���ض���λ����ʾ�µ�ͼƬ
	if(posx != -1000 && posy != -1000)
	{
		CFX_RectF rect;
		rect.Set(posx, posy, (FX_FLOAT)sealInfo->width, (FX_FLOAT)sealInfo->height);
		FX_INT32 nCount = doc->CountPages();
		for(int i = 0; i < nCount; i++)
		{
			if(i == pageIndex)
				continue;

			COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, i, -1);
            pStampAnnot->SetSignature(wSeal->GetReadSignature());
			pStampAnnot->SetBoundary(rect);
		}
	}

	//
	if(!pOESInterface->IsEmptyGetSignMethod())
		wSeal->SetSignatureMethod(sealInfo->signMethod.UTF8Decode());
	wSeal->SetRefCheckMethod(sealInfo->digestMethod.UTF8Decode());
	//WriteLog_GSDK("FS_SignOFD sealInfo->signMethod : ", sealInfo->signMethod);
	//WriteLog_GSDK("FS_SignOFD sealInfo->digestMethod : ", sealInfo->digestMethod);
	IOFD_SignHandler* phSign = OFD_SignHandler_Create(pWriteDoc);
	phSign->StartSign(pFileWrite, wSeal);

	FX_INT32 refersCount = wSeal->CountReferences();
	//WriteLog_GSDK("FS_SignOFD ", "refersCount : ", refersCount);
	FX_LPBYTE pbTxts = NULL;
	unsigned int cbTxts = 0;
	FX_LPBYTE pbSign = NULL;
	int cbSign = 0;
	FX_DWORD offer = 0;

	// ժҪ
	FX_LPBYTE lpDigestData = NULL;
	int cpDigestData = 0;
	for (int j = 0; j < refersCount; j++)
	{
		COFD_WriteSignReference* refer = wSeal->GetReference(j);
		IFX_FileRead* file = refer->GetFile();
		FX_DWORD fileSize = file->GetSize();
		FX_LPBYTE tempData = FX_Alloc(FX_BYTE, fileSize);
		memset(tempData, 0, fileSize);
		file->ReadBlock((FX_LPVOID)(tempData), 0, fileSize);
		//WriteLog_GSDK("FS_SignOFD ", "fileSize : ", fileSize);
		// ��ÿ��refer����Hash
		FX_DWORD dwRes = pOESInterface->Digest(tempData, fileSize, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
			sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
		//WriteLog_GSDK("FS_SignOFD ", "dwRes : ", dwRes);
		if (OES_OK == dwRes)
		{
			lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
			memset(lpDigestData, 0, cpDigestData);
			dwRes = pOESInterface->Digest(tempData, fileSize, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
				sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
		}
		//WriteLog_GSDK("FS_SignOFD ", "dwRes 2 : ", dwRes);
		FXSYS_assert(OES_OK == dwRes);

		CFX_WideString wsCheckMethod;
		CFX_Base64Encoder base64Encoder;
		base64Encoder.Encode(CFX_ByteString((unsigned char *)lpDigestData, cpDigestData), wsCheckMethod);
		refer->SetCheckValue(wsCheckMethod);
		FX_Free(tempData);
		FX_Free(lpDigestData);
	}

	lpDigestData = NULL;

	IFX_FileRead* fileSignature = phSign->GetSignatureFile();
	FX_DWORD dwSignfileSize = fileSignature->GetSize();
	//WriteLog_GSDK("FS_SignOFD ", "dwSignfileSize : ", dwSignfileSize);
	cbTxts = dwSignfileSize;
	pbTxts = FX_Alloc(FX_BYTE, cbTxts);
	memset((FX_LPVOID)pbTxts, 0, cbTxts);

	fileSignature->ReadBlock((FX_LPVOID)(pbTxts + offer), 0, dwSignfileSize);
	fileSignature->Release();

	// ���������ݼ���ժҪ
	FX_DWORD dwRes = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
		sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
	//WriteLog_GSDK("FS_SignOFD ", "dwRes 3 : ", dwRes);
	if (OES_OK == dwRes)
	{
		lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
		memset(lpDigestData, 0, cpDigestData);
		dwRes = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
			sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
	}
	//WriteLog_GSDK("FS_SignOFD ", "dwRes 4 : ", dwRes);
	FXSYS_assert(OES_OK == dwRes);

	// Sign
	CFX_ByteString bsSealID = sealInfo->sealId;
	CFX_WideString fileName = ((IOFD_FileStream *)fileSignature)->GetCurrentFileName(0);
	CFX_ByteString docProperty = CFX_ByteString::FromUnicode(fileName);
	if (docProperty[0] != '/')
		docProperty.Insert(0, '/');

	FX_DWORD dwSign = pOESInterface->Sign(
		(FX_LPBYTE)(FX_LPCBYTE)bsSealID, 
		bsSealID.GetLength(),
		(FX_LPBYTE)(FX_LPCBYTE)docProperty,
		docProperty.GetLength(),
		(FX_LPBYTE)lpDigestData, 
		cpDigestData, 
		(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
		sealInfo->signMethod.GetLength(),
		(unsigned char*)(FX_LPCBYTE)bsSignTime,
		bsSignTime.GetLength(),
		NULL, 
		cbSign);
	//WriteLog_GSDK("FS_SignOFD ", "dwSign 1 : ", dwSign);
	if (OES_OK == dwSign)
	{
		pbSign = FX_Alloc(FX_BYTE, cbSign);
		memset(pbSign, 0, cbSign);

		dwSign = pOESInterface->Sign(
			(FX_LPBYTE)(FX_LPCBYTE)bsSealID, 
			bsSealID.GetLength(), 
			(FX_LPBYTE)(FX_LPCBYTE)docProperty,
			docProperty.GetLength(),
			(FX_LPBYTE)lpDigestData, 
			cpDigestData, 
			(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
			sealInfo->signMethod.GetLength(),
			(unsigned char*)(FX_LPCBYTE)bsSignTime,
			bsSignTime.GetLength(),
			pbSign,
			cbSign);
	}
	//WriteLog_GSDK("FS_SignOFD ", "dwSign 2 : ", dwSign);
	CFX_ByteString signValue = CFX_ByteString(pbSign, cbSign);
	if (pbTxts) FX_Free(pbTxts);
	if (lpDigestData) FX_Free(lpDigestData);
	if (pbSign) FX_Free(pbSign);
	//WriteLog_GSDK("FS_SignOFD ", "dwSign 3 : ", dwSign);
	if (OES_OK != dwSign)
	{
		FX_INT32 index = ((COFD_Signatures *)wSigns)->CountSignature() - 1;
		if (index > -1)
			wSigns->RemoveSignature(pWriteDoc, pageIndex, index);
	}
	else
		phSign->SetSignedValue(signValue);

	phSign->EndSign();
	phSign->Release();	
	pFileWrite->Flush();
	//WriteLog_GSDK("FS_SignOFD ", "555 ...");
	return dwSign;
}

void CStraddleSealPlot::StraddleSeal()
{
	if(m_sealInfo == NULL || m_wSeal == NULL || m_pWriteDoc == NULL || m_pReadDoc == NULL)
		return;

	//FS_WriteLog("StraddleSeal ", "m_nPart : ", m_nPart);

	switch(m_nPart)
	{
	case 0:
		LeftFirstPagePercent();
		LeftPart();
		break;
	case 1:
		TopFirstPagePercent();
		TopPart();
		break;
	case 2:
		RightFirstPagePercent();
		RightPart();
		break;
	case 3:
		BottomFirstPagePercent();
		BottomPart();
		break;
	}
}

void CStraddleSealPlot::LeftFirstPagePercent()
{
	//FS_WriteLog("LeftFirstPagePercent ", "beg ...");
	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	IOFD_Page *pPage = m_pReadDoc->GetPage(m_nFirstPageIndex);
	if(pPage == NULL)
		return;

	CFX_RectF pPageRect;
	CFX_RectF rtBoundaryRect;
	CFX_RectF rtClipRect;

	FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
	FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
	FX_FLOAT fLeft = 0.0 - width * (1 - m_fFirstPagePercent);

	FX_FLOAT fTop = 0.0;
	if(m_nProportion == 1)
		fTop = pPageRect.Height() / 5 - height / 2;
	else if(m_nProportion == 2)
		fTop = pPageRect.Height() * 2 / 5 - height / 2;
	else if(m_nProportion == 3)
		fTop = pPageRect.Height() * 3 / 5 - height / 2;
	else if(m_nProportion == 4)
		fTop = pPageRect.Height() * 4 / 5 - height / 2;

	//FX_FLOAT fClipWid = width * 1.0 / 5;
	//FX_FLOAT fClipLeft = pPageRect.right() - fClipWid;
	rtBoundaryRect.Set(fLeft, fTop, width, height);
	rtClipRect.Set(0, 0, width  * m_fFirstPagePercent, height  * m_fFirstPagePercent);

	COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, m_nFirstPageIndex, -1);
    pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
	pStampAnnot->SetBoundary(rtBoundaryRect);
	//pStampAnnot->SetClip(rtClipRect);
}
/*
void CStraddleSealPlot::TopFirstPagePercent()
{
	FS_WriteLog("TopFirstPagePercent ", "beg ...");

	if(m_pReadDoc == NULL)
		return;

	WriteLog_GSDKF("TopFirstPagePercent ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	IOFD_Page *pPage = m_pReadDoc->GetPage(m_nFirstPageIndex);
	if(pPage == NULL)
		return;

	CFX_RectF pPageRect;
	CFX_RectF rtBoundaryRect;
	CFX_RectF rtClipRect;

	FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
	FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
	FX_FLOAT fTop = 0;

	WriteLog_GSDKF("TopFirstPagePercent ", "fTop : ", fTop);
	FX_FLOAT fLeft = 0.0;
	if(m_nProportion == 1)
		fLeft = pPageRect.Width() / 5 - width / 2;
	else if(m_nProportion == 2)
		fLeft = pPageRect.Width() * 2 / 5 - width / 2;
	else if(m_nProportion == 3)
		fLeft = pPageRect.Width() * 3 / 5 - width / 2;
	else if(m_nProportion == 4)
		fLeft = pPageRect.Width() * 4 / 5 - width / 2;

	WriteLog_GSDKF("TopFirstPagePercent ", "fLeft : ", fLeft);
	//FX_FLOAT fClipWidth = width * 1.0 / 5;
	//WriteLog_GSDKF("TopFirstPagePercent ", "fClipWidth: ", fClipWidth);
     //   FX_FLOAT fClipLeft = pPageRect.right() - fClipWidth;
	//WriteLog_GSDKF("TopFirstPagePercent ", "fClipLeft : ", fClipLeft);
	rtBoundaryRect.Set(fLeft, fTop, width, height);
    rtClipRect.Set(0, 0, width, height  * m_fFirstPagePercent);

	COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, 0, -1);
	pStampAnnot->SetBoundary(rtBoundaryRect);
	pStampAnnot->SetClip(rtClipRect);
}
*/

void CStraddleSealPlot::TopFirstPagePercent()
{
	//FS_WriteLog("TopFirstPagePercent ", "beg ...");

	if(m_pReadDoc == NULL)
		return;

	//WriteLog_GSDKF("TopFirstPagePercent ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	IOFD_Page *pPage = m_pReadDoc->GetPage(m_nFirstPageIndex);
	if(pPage == NULL)
		return;

	CFX_RectF pPageRect;
	CFX_RectF rtBoundaryRect;
	CFX_RectF rtClipRect;

	FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
	FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
	FX_FLOAT fTop = 0 - height * (1 - m_fFirstPagePercent);

	//WriteLog_GSDKF("TopFirstPagePercent ", "fTop : ", fTop);
	FX_FLOAT fLeft = 0.0;
	if(m_nProportion == 1)
		fLeft = pPageRect.Width() / 5 - width / 2;
	else if(m_nProportion == 2)
		fLeft = pPageRect.Width() * 2 / 5 - width / 2;
	else if(m_nProportion == 3)
		fLeft = pPageRect.Width() * 3 / 5 - width / 2;
	else if(m_nProportion == 4)
		fLeft = pPageRect.Width() * 4 / 5 - width / 2;

	//WriteLog_GSDKF("TopFirstPagePercent ", "fLeft : ", fLeft);
	//FX_FLOAT fClipWidth = width * 1.0 / 5;
	//WriteLog_GSDKF("TopFirstPagePercent ", "fClipWidth: ", fClipWidth);
     //   FX_FLOAT fClipLeft = pPageRect.right() - fClipWidth;
	//WriteLog_GSDKF("TopFirstPagePercent ", "fClipLeft : ", fClipLeft);
	rtBoundaryRect.Set(fLeft, fTop, width, height);
    //    rtClipRect.Set(fLeft, fTop, width, height  * m_fFirstPagePercent);

	COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, m_nFirstPageIndex, -1);
    pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
	pStampAnnot->SetBoundary(rtBoundaryRect);
}


void CStraddleSealPlot::RightFirstPagePercent()
{

	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	IOFD_Page *pPage = m_pReadDoc->GetPage(m_nFirstPageIndex);
	if(pPage == NULL)
		return;

	CFX_RectF pPageRect;
	CFX_RectF rtBoundaryRect;
	CFX_RectF rtClipRect;

	FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
	FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
	FX_FLOAT fLeft = pPageRect.Width() - width * m_fFirstPagePercent;

	FX_FLOAT fTop = 0.0;
	if(m_nProportion == 1)
		fTop = pPageRect.Height() / 5 - height / 2;
	else if(m_nProportion == 2)
		fTop = pPageRect.Height() * 2 / 5 - height / 2;
	else if(m_nProportion == 3)
		fTop = pPageRect.Height() * 3 / 5 - height / 2;
	else if(m_nProportion == 4)
		fTop = pPageRect.Height() * 4 / 5 - height / 2;

	//FX_FLOAT fClipWid = width * 1.0 / 5;
	//FX_FLOAT fClipLeft = pPageRect.right() - fClipWid;
	rtBoundaryRect.Set(fLeft, fTop, width, height);
	rtClipRect.Set(0, 0, width  * m_fFirstPagePercent, height  * m_fFirstPagePercent);

	COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, m_nFirstPageIndex, -1);
    pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
	pStampAnnot->SetBoundary(rtBoundaryRect);
	//pStampAnnot->SetClip(rtClipRect);
}

void CStraddleSealPlot::BottomFirstPagePercent()
{
	//FS_WriteLog("BottomFirstPagePercent ", "beg ...");
	if(m_pReadDoc == NULL)
		return;

	//WriteLog_GSDKF("BottomFirstPagePercent ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	IOFD_Page *pPage = m_pReadDoc->GetPage(m_nFirstPageIndex);
	if(pPage == NULL)
		return;

	CFX_RectF pPageRect;
	CFX_RectF rtBoundaryRect;
	CFX_RectF rtClipRect;

	FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
	FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
	FX_FLOAT fTop = pPageRect.Height() - height * m_fFirstPagePercent;

	//WriteLog_GSDKF("BottomFirstPagePercent ", "fTop : ", fTop);
	FX_FLOAT fLeft = 0.0;
	if(m_nProportion == 1)
		fLeft = pPageRect.Width() / 5 - width / 2;
	else if(m_nProportion == 2)
		fLeft = pPageRect.Width() * 2 / 5 - width / 2;
	else if(m_nProportion == 3)
		fLeft = pPageRect.Width() * 3 / 5 - width / 2;
	else if(m_nProportion == 4)
		fLeft = pPageRect.Width() * 4 / 5 - width / 2;

	//WriteLog_GSDKF("BottomFirstPagePercent ", "fLeft : ", fLeft);
	FX_FLOAT fClipWidth = width * 1.0 / 5;
	//WriteLog_GSDKF("BottomFirstPagePercent ", "fClipWidth: ", fClipWidth);
        FX_FLOAT fClipLeft = pPageRect.right() - fClipWidth;
	//WriteLog_GSDKF("BottomFirstPagePercent ", "fClipLeft : ", fClipLeft);
	rtBoundaryRect.Set(fLeft, fTop, width, height);
        rtClipRect.Set(fLeft, fTop, width, height  * m_fFirstPagePercent);

	COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, m_nFirstPageIndex, -1);
    pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
	pStampAnnot->SetBoundary(rtBoundaryRect);
	//pStampAnnot->SetClip(rtClipRect);
}

void CStraddleSealPlot::LeftPart()
{
	//FS_WriteLog("LeftPart ", "beg ...");

	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	//WriteLog_GSDKF("LeftPart ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	float fLastPercent = 1 - m_fFirstPagePercent;
	int nLastCount = m_nCountPage - 1;
	//FS_WriteLog("LeftPart ", "m_nCountPage : ", m_nCountPage);
	//FS_WriteLog("LeftPart ", "nLastCount : ", nLastCount);
	float fPagePercent = fLastPercent / nLastCount;
	//WriteLog_GSDKF("LeftPart ", "fPagePercent : ", fPagePercent);

	//FS_WriteLog("LeftPart ", "m_nFirstPageIndex + 1 : ", m_nFirstPageIndex + 1);
	//FS_WriteLog("LeftPart ", "m_nFirstPageIndex + m_nCountPage : ", m_nFirstPageIndex + m_nCountPage);
	int j = 0;
	for(int i = m_nFirstPageIndex + 1; i < m_nFirstPageIndex + m_nCountPage; i++)
	{
		//FS_WriteLog("LeftPart ", "j : ", j);
		IOFD_Page *pPage = m_pReadDoc->GetPage(i);
		if(pPage == NULL)
			return;

		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
		FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
		pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
		FX_FLOAT fLeft = 0 - width * ( 1 - m_fFirstPagePercent - (j + 1) * fPagePercent);

		FX_FLOAT fClipLeft = width * (1 - m_fFirstPagePercent - (j + 1) * fPagePercent);
		FX_FLOAT fClipWidth = width * fPagePercent;

		FX_FLOAT fTop = 0.0;
		if(m_nProportion == 1)
			fTop = pPageRect.Height() / 5 - height / 2;
		else if(m_nProportion == 2)
			fTop = pPageRect.Height() * 2 / 5 - height / 2;
		else if(m_nProportion == 3)
			fTop = pPageRect.Height() * 3 / 5 - height / 2;
		else if(m_nProportion == 4)
			fTop = pPageRect.Height() * 4 / 5 - height / 2;

		rtBoundaryRect.Set(fLeft, fTop, width, height);
		rtClipRect.Set(fClipLeft, 0, fClipWidth, height);

		COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, i, -1);
        pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
		pStampAnnot->SetBoundary(rtBoundaryRect);
		pStampAnnot->SetClip(rtClipRect);

		j++;
	}
}
/*
void CStraddleSealPlot::TopPart()
{
	FS_WriteLog("TopPart ", "beg ...");

	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	WriteLog_GSDKF("TopPart ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	float fLastPercent = 1 - m_fFirstPagePercent;
	int nLastCount = m_nCountPage - 1;
	FS_WriteLog("TopPart ", "m_nCountPage : ", m_nCountPage);
	FS_WriteLog("TopPart ", "nLastCount : ", nLastCount);
	float fPagePercent = fLastPercent / nLastCount;
	WriteLog_GSDKF("TopPart ", "fPagePercent : ", fPagePercent);

	FS_WriteLog("TopPart ", "m_nFirstPageIndex + 1 : ", m_nFirstPageIndex + 1);
	FS_WriteLog("TopPart ", "m_nFirstPageIndex + m_nCountPage : ", m_nFirstPageIndex + m_nCountPage);
	int j = 0;
	for(int i = m_nFirstPageIndex + 1; i < m_nFirstPageIndex + m_nCountPage; i++)
	{
		FS_WriteLog("TopPart ", "j : ", j);
		IOFD_Page *pPage = m_pReadDoc->GetPage(i);
		if(pPage == NULL)
			return;

		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
		FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
		pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
		FX_FLOAT fTop = 0 - height * (m_fFirstPagePercent + (j + 1) * fPagePercent);

		FX_FLOAT fClipTop = height - height * (m_fFirstPagePercent + (j + 1) * fPagePercent);
		FX_FLOAT fClipHeight = height * fPagePercent;

		FX_FLOAT fLeft = 0.0;
		if(m_nProportion == 1)
			fLeft = pPageRect.Width() / 5 - width / 2;
		else if(m_nProportion == 2)
			fLeft = pPageRect.Width() * 2 / 5 - width / 2;
		else if(m_nProportion == 3)
			fLeft = pPageRect.Width() * 3 / 5 - width / 2;
		else if(m_nProportion == 4)
			fLeft = pPageRect.Width() * 4 / 5 - width / 2;

		rtBoundaryRect.Set(fLeft, fTop, width, height);
		rtClipRect.Set(0, fClipTop, width, fClipHeight);

		COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, i, -1);
		pStampAnnot->SetBoundary(rtBoundaryRect);
		pStampAnnot->SetClip(rtClipRect);

		j++;
	}
}
*/

void CStraddleSealPlot::TopPart()
{
	//FS_WriteLog("TopPart ", "beg ...");

	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	//WriteLog_GSDKF("BottomPart ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	float fLastPercent = 1 - m_fFirstPagePercent;
	int nLastCount = m_nCountPage - 1;
	//FS_WriteLog("BottomPart ", "m_nCountPage : ", m_nCountPage);
	//FS_WriteLog("BottomPart ", "nLastCount : ", nLastCount);
	float fPagePercent = fLastPercent / nLastCount;
	//WriteLog_GSDKF("BottomPart ", "fPagePercent : ", fPagePercent);

	//FS_WriteLog("BottomPart ", "m_nFirstPageIndex + 1 : ", m_nFirstPageIndex + 1);
	//FS_WriteLog("BottomPart ", "m_nFirstPageIndex + m_nCountPage : ", m_nFirstPageIndex + m_nCountPage);
	int j = 0;
	for(int i = m_nFirstPageIndex + 1; i < m_nFirstPageIndex + m_nCountPage; i++)
	{
		//FS_WriteLog("BottomPart ", "j : ", j);
		IOFD_Page *pPage = m_pReadDoc->GetPage(i);
		if(pPage == NULL)
			return;

		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
		FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
		pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
		FX_FLOAT fTop = 0 - height * (1 - m_fFirstPagePercent - (j + 1) * fPagePercent);

		FX_FLOAT fClipTop = height - height * (m_fFirstPagePercent + (j + 1) * fPagePercent);
		FX_FLOAT fClipHeight = height * fPagePercent;

		FX_FLOAT fLeft = 0.0;
		if(m_nProportion == 1)
			fLeft = pPageRect.Width() / 5 - width / 2;
		else if(m_nProportion == 2)
			fLeft = pPageRect.Width() * 2 / 5 - width / 2;
		else if(m_nProportion == 3)
			fLeft = pPageRect.Width() * 3 / 5 - width / 2;
		else if(m_nProportion == 4)
			fLeft = pPageRect.Width() * 4 / 5 - width / 2;

		rtBoundaryRect.Set(fLeft, fTop, width, height);
		rtClipRect.Set(0, fClipTop, width, fClipHeight);

		COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, i, -1);
        pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
		pStampAnnot->SetBoundary(rtBoundaryRect);
		pStampAnnot->SetClip(rtClipRect);

		j++;
	}
}


void CStraddleSealPlot::RightPart()
{
	//FS_WriteLog("RightPart ", "beg ...");

	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	//WriteLog_GSDKF("RightPart ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	float fLastPercent = 1 - m_fFirstPagePercent;
	int nLastCount = m_nCountPage - 1;
	//FS_WriteLog("RightPart ", "m_nCountPage : ", m_nCountPage);
	//FS_WriteLog("RightPart ", "nLastCount : ", nLastCount);
	float fPagePercent = fLastPercent / nLastCount;
	//WriteLog_GSDKF("RightPart ", "fPagePercent : ", fPagePercent);

	//FS_WriteLog("RightPart ", "m_nFirstPageIndex + 1 : ", m_nFirstPageIndex + 1);
	//FS_WriteLog("RightPart ", "m_nFirstPageIndex + m_nCountPage : ", m_nFirstPageIndex + m_nCountPage);
	int j = 0;
	for(int i = m_nFirstPageIndex + 1; i < m_nFirstPageIndex + m_nCountPage; i++)
	{
		//FS_WriteLog("RightPart ", "j : ", j);
		IOFD_Page *pPage = m_pReadDoc->GetPage(i);
		if(pPage == NULL)
			return;

		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
		FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
		pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
		FX_FLOAT fLeft = pPageRect.Width() - width * ( m_fFirstPagePercent + (j + 1) * fPagePercent);

		FX_FLOAT fClipLeft = width * (m_fFirstPagePercent + j * fPagePercent);
		FX_FLOAT fClipWidth = width * fPagePercent;

		FX_FLOAT fTop = 0.0;
		if(m_nProportion == 1)
			fTop = pPageRect.Height() / 5 - height / 2;
		else if(m_nProportion == 2)
			fTop = pPageRect.Height() * 2 / 5 - height / 2;
		else if(m_nProportion == 3)
			fTop = pPageRect.Height() * 3 / 5 - height / 2;
		else if(m_nProportion == 4)
			fTop = pPageRect.Height() * 4 / 5 - height / 2;

		rtBoundaryRect.Set(fLeft, fTop, width, height);
		rtClipRect.Set(fClipLeft, 0, fClipWidth, height);

		COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, i, -1);
        pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
		pStampAnnot->SetBoundary(rtBoundaryRect);
		pStampAnnot->SetClip(rtClipRect);

		j++;
	}
}

void CStraddleSealPlot::BottomPart()
{
	//FS_WriteLog("BottomPart ", "beg ...");
	if(m_pReadDoc == NULL)
		return;

	if(m_fFirstPagePercent < 0.05 || m_fFirstPagePercent > 0.95)
		m_fFirstPagePercent = 0.1;

	//WriteLog_GSDKF("BottomPart ", "m_fFirstPagePercent : ", m_fFirstPagePercent);
	float fLastPercent = 1 - m_fFirstPagePercent;
	int nLastCount = m_nCountPage - 1;
	//FS_WriteLog("BottomPart ", "m_nCountPage : ", m_nCountPage);
	//FS_WriteLog("BottomPart ", "nLastCount : ", nLastCount);
	float fPagePercent = fLastPercent / nLastCount;
	//WriteLog_GSDKF("BottomPart ", "fPagePercent : ", fPagePercent);

	//FS_WriteLog("BottomPart ", "m_nFirstPageIndex + 1 : ", m_nFirstPageIndex + 1);
	//FS_WriteLog("BottomPart ", "m_nFirstPageIndex + m_nCountPage : ", m_nFirstPageIndex + m_nCountPage);
	int j = 0;
	for(int i = m_nFirstPageIndex + 1; i < m_nFirstPageIndex + m_nCountPage; i++)
	{
		//FS_WriteLog("BottomPart ", "j : ", j);
		IOFD_Page *pPage = m_pReadDoc->GetPage(i);
		if(pPage == NULL)
			return;

		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = ((FS_SEAL_INFO*)m_sealInfo)->width;
		FX_INT32 height = ((FS_SEAL_INFO*)m_sealInfo)->height;
		pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
		FX_FLOAT fTop = pPageRect.Height() - height * (m_fFirstPagePercent + (j + 1) * fPagePercent);

		FX_FLOAT fClipTop = height * (m_fFirstPagePercent + j * fPagePercent);
		FX_FLOAT fClipHeight = height * fPagePercent;

		FX_FLOAT fLeft = 0.0;
		if(m_nProportion == 1)
			fLeft = pPageRect.Width() / 5 - width / 2;
		else if(m_nProportion == 2)
			fLeft = pPageRect.Width() * 2 / 5 - width / 2;
		else if(m_nProportion == 3)
			fLeft = pPageRect.Width() * 3 / 5 - width / 2;
		else if(m_nProportion == 4)
			fLeft = pPageRect.Width() * 4 / 5 - width / 2;

		rtBoundaryRect.Set(fLeft, fTop, width, height);
		rtClipRect.Set(0, fClipTop, width, fClipHeight);

		COFD_WriteStampAnnot* pStampAnnot = m_wSeal->InsertStampAnnot(m_pWriteDoc, i, -1);
        pStampAnnot->SetSignature(m_wSeal->GetReadSignature());
		pStampAnnot->SetBoundary(rtBoundaryRect);
		pStampAnnot->SetClip(rtClipRect);

		j++;
	}
}

//
FX_DWORD FS_SignOFDStraddle(FX_LPVOID psealInfo, IOFD_WriteDocument* pWriteDoc, IFX_FileWrite* pFileWrite, int nPart, int nProportion, float fFirstPagePercent, int nFirstPageIndex, int nCountPage)
{
	//FS_WriteLog("FS_SignOFDStraddle ", "beg ..");
	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)psealInfo;

	IOFD_Document* doc = pWriteDoc->GetReadDoc();
	COFD_Signatures* sigs = doc->GetSignatures();

	COFD_WriteSignatures* wSigns = pWriteDoc->CreateSignatures();
	COFD_WriteSignature* wSeal = wSigns->InsertSignature(pWriteDoc, -1);

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();

	//FS_WriteLog("FS_SignOFDStraddle ", "111 ..");
	//
	CFX_ByteString bsSignTime;
	if(!pOESInterface->IsEmptyGetSignDataTime())
	{
		CFX_WideString wsSignTime = FS_GetSignDataTime();
		wSeal->SetSignatureDateTime(wsSignTime);

		bsSignTime = wsSignTime.UTF8Encode();
	}
	//FS_WriteLog("FS_SignOFDStraddle ", "112 ..");
	wSeal->SetCompany(sealInfo->company);
	wSeal->SetProviderName(sealInfo->name);	
	wSeal->SetVersion(sealInfo->version);
	if(!pOESInterface->IsEmptyGetSeal())
		wSeal->SetSealFile(pWriteDoc, sealInfo->sealStream);

	//FS_WriteLog("FS_SignOFDStraddle ", "113 ..");
	//	Straddle seal sign beg ...
	FX_INT32 nIndex = 0;
	FX_INT32 nPageCount = doc->CountPages();
	//FS_WriteLog("FS_SignOFDStraddle ", "nPageCount : ", nPageCount);

	//FS_WriteLog("FS_SignOFDStraddle ", "nPart : ", nPart);
	//FS_WriteLog("FS_SignOFDStraddle ", "nProportion : ", nProportion);
	//WriteLog_GSDKF("FS_SignOFDStraddle ", "fFirstPagePercent : ", fFirstPagePercent);
	//FS_WriteLog("FS_SignOFDStraddle ", "nFirstPageIndex : ", nFirstPageIndex);
	CStraddleSealPlot sealPlot((void*)sealInfo, wSeal, pWriteDoc, doc, nPart, nProportion, fFirstPagePercent, nFirstPageIndex, nCountPage);
	sealPlot.StraddleSeal();
	//nIndex++;
/*
	if( nPageCount > 5 )
	{
		IOFD_Page *pPage = doc->GetPage(0);

		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = sealInfo->width;
		FX_INT32 height = sealInfo->height;

		pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
		FX_FLOAT fTop = (pPageRect.Height() - height) / 2;
		FX_FLOAT fBoundaryLeft = pPageRect.right() - 1.0 / 5 * width;

		FX_FLOAT fClipWid = width * 1.0 / 5;
        FX_FLOAT fClipLeft = pPageRect.right() - fClipWid;
		rtBoundaryRect.Set(fBoundaryLeft, fTop, width, height);
        rtClipRect.Set(fClipLeft, fTop, fClipWid, height);

		COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, 0, -1);
		pStampAnnot->SetBoundary(rtBoundaryRect);
        pStampAnnot->SetClip(rtClipRect);
        nIndex = 1;
	}
*/
/*
	nPageCount = nPageCount - nIndex;
	FS_WriteLog("FS_SignOFDStraddle ", "nPageCount 2 : ", nPageCount);
	for(int i = 0; i < nPageCount; i++)
    {
		CFX_RectF pPageRect;
		CFX_RectF rtBoundaryRect;
		CFX_RectF rtClipRect;

		FX_INT32 width = sealInfo->width;
		FX_INT32 height = sealInfo->height;

        IOFD_Page* pPage = doc->GetPage(i);
        pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, pPageRect);
        FX_FLOAT fTop = (pPageRect.Height() - height) / 2;
        //
        FX_FLOAT fBoundaryLeft = pPageRect.right() - (i + 1) * 1.0 / nPageCount * ((5 - nIndex) / 5.0 * width) - nIndex * width / 5.0;
        //
        FX_FLOAT fClipWid = ((5 - nIndex) / 5.0 * width) / nPageCount;
        FX_FLOAT fClipLeft = pPageRect.right() - fClipWid;
        rtBoundaryRect.Set(fBoundaryLeft, fTop, width, height);
        rtClipRect.Set(fClipLeft, fTop, fClipWid, height);
        COFD_WriteStampAnnot* pStampAnnot = wSeal->InsertStampAnnot(pWriteDoc, i + nIndex, -1);
        pStampAnnot->SetBoundary(rtBoundaryRect);
        pStampAnnot->SetClip(rtClipRect);
    }
*/
	//	Straddle seal sign end ...

	//FS_WriteLog("FS_SignOFDStraddle ", "114 ..");
	//
	if(!pOESInterface->IsEmptyGetSignMethod())
		wSeal->SetSignatureMethod(sealInfo->signMethod.UTF8Decode());
	wSeal->SetRefCheckMethod(sealInfo->digestMethod.UTF8Decode());

	IOFD_SignHandler* phSign = OFD_SignHandler_Create(pWriteDoc);

	phSign->StartSign(pFileWrite, wSeal);

	FX_INT32 refersCount = wSeal->CountReferences();

	//FS_WriteLog("FS_SignOFDStraddle ", "115 ..");
	FX_LPBYTE pbTxts = NULL;
	unsigned int cbTxts = 0;
	FX_LPBYTE pbSign = NULL;
	int cbSign = 0;
	FX_DWORD offer = 0;

	// ժҪ
	FX_LPBYTE lpDigestData = NULL;
	int cpDigestData = 0;
	for (int j = 0; j < refersCount; j++)
	{
		COFD_WriteSignReference* refer = wSeal->GetReference(j);
		IFX_FileRead* file = refer->GetFile();
		FX_DWORD fileSize = file->GetSize();
		FX_LPBYTE tempData = FX_Alloc(FX_BYTE, fileSize);
		memset(tempData, 0, fileSize);
		file->ReadBlock((FX_LPVOID)(tempData), 0, fileSize);

		// ��ÿ��refer����Hash
		FX_DWORD dwRes = pOESInterface->Digest(tempData, fileSize, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
			sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
		if (OES_OK == dwRes)
		{
			lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
			memset(lpDigestData, 0, cpDigestData);
			dwRes = pOESInterface->Digest(tempData, fileSize, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
				sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
		}
		FXSYS_assert(OES_OK == dwRes);

		CFX_WideString wsCheckMethod;
		CFX_Base64Encoder base64Encoder;
		base64Encoder.Encode(CFX_ByteString((unsigned char *)lpDigestData, cpDigestData), wsCheckMethod);
		refer->SetCheckValue(wsCheckMethod);
		FX_Free(tempData);
		FX_Free(lpDigestData);
	}

	lpDigestData = NULL;

	//FS_WriteLog("FS_SignOFDStraddle ", "116 ..");
	IFX_FileRead* fileSignature = phSign->GetSignatureFile();
	FX_DWORD dwSignfileSize = fileSignature->GetSize();
	cbTxts = dwSignfileSize;
	pbTxts = FX_Alloc(FX_BYTE, cbTxts);
	memset((FX_LPVOID)pbTxts, 0, cbTxts);

	fileSignature->ReadBlock((FX_LPVOID)(pbTxts + offer), 0, dwSignfileSize);
	fileSignature->Release();

	//FS_WriteLog("FS_SignOFDStraddle ", "117 ..");
	// ���������ݼ���ժҪ
	FX_DWORD dwRes = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
		sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
	//FS_WriteLog("FS_SignOFDStraddle ", "dwRes : ", dwRes);
	if (OES_OK == dwRes)
	{
		lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
		memset(lpDigestData, 0, cpDigestData);
		dwRes = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
			sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
	}
	FXSYS_assert(OES_OK == dwRes);

	// Sign
	CFX_ByteString bsSealID = sealInfo->sealId;
	CFX_WideString fileName = ((IOFD_FileStream *)fileSignature)->GetCurrentFileName(0);
	CFX_ByteString docProperty = CFX_ByteString::FromUnicode(fileName);
	if (docProperty[0] != '/')
		docProperty.Insert(0, '/');
	FX_DWORD dwSign = pOESInterface->Sign(
		(FX_LPBYTE)(FX_LPCBYTE)bsSealID, 
		bsSealID.GetLength(),
		(FX_LPBYTE)(FX_LPCBYTE)docProperty,
		docProperty.GetLength(),
		(FX_LPBYTE)lpDigestData, 
		cpDigestData, 
		(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
		sealInfo->signMethod.GetLength(),
		(unsigned char*)(FX_LPCBYTE)bsSignTime,
		bsSignTime.GetLength(),
		NULL, 
		cbSign);

	if (OES_OK == dwSign)
	{
		pbSign = FX_Alloc(FX_BYTE, cbSign);
		memset(pbSign, 0, cbSign);

		dwSign = pOESInterface->Sign(
			(FX_LPBYTE)(FX_LPCBYTE)bsSealID, 
			bsSealID.GetLength(), 
			(FX_LPBYTE)(FX_LPCBYTE)docProperty,
			docProperty.GetLength(),
			(FX_LPBYTE)lpDigestData, 
			cpDigestData, 
			(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
			sealInfo->signMethod.GetLength(),
			(unsigned char*)(FX_LPCBYTE)bsSignTime,
			bsSignTime.GetLength(),
			pbSign,
			cbSign);
	}

	CFX_ByteString signValue = CFX_ByteString(pbSign, cbSign);
	if (pbTxts) FX_Free(pbTxts);
	if (lpDigestData) FX_Free(lpDigestData);
	if (pbSign) FX_Free(pbSign);
	if (OES_OK != dwSign)
	{
		FX_INT32 index = ((COFD_Signatures *)wSigns)->CountSignature() - 1;
		if (index > -1)
			wSigns->RemoveSignature(pWriteDoc, 0, index);
	}
	else
		phSign->SetSignedValue(signValue);

	//FS_WriteLog("FS_SignOFDStraddle ", "dwSign : ", dwSign);
	phSign->EndSign();
	phSign->Release();	
	pFileWrite->Flush();

	return dwSign;
}

//
FX_DWORD FS_SignData(FX_LPVOID si, FX_LPBYTE pbTxts, unsigned int cbTxts, CFX_ByteString& bsSign, CFX_WideString& wsDateTime)
{
	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)si;
	FX_LPBYTE pbSign = NULL; int cbSign = 0;

	// ժҪ
	FX_LPBYTE lpDigestData = NULL;
	int cpDigestData = 0;

	// ���������ݼ���ժҪ
	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
	FX_DWORD dwRet = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
		sealInfo->digestMethod.GetLength(), NULL, cpDigestData);
	if (OES_OK == dwRet)
	{
		lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
		memset(lpDigestData, 0, cpDigestData);
		dwRet = pOESInterface->Digest(pbTxts, cbTxts, (unsigned char*)(FX_LPCBYTE)sealInfo->digestMethod,
			sealInfo->digestMethod.GetLength(), lpDigestData, cpDigestData);
	}
	else
		return dwRet;

	//////////////
	CFX_Base64Encoder base64Encoder;
	CFX_ByteString bsDigest;
	base64Encoder.Encode(CFX_ByteString((unsigned char *)lpDigestData, cpDigestData), bsDigest);
	//////////////

	// Sign
	CFX_ByteString bsSealID = sealInfo->sealId;
//	CFX_WideString fileName = ((IOFD_FileStream *)fileSignature)->GetCurrentFileName(0);
//	CFX_ByteString docProperty = CFX_ByteString::FromUnicode(fileName);
//	if (docProperty[0] != '/')
//		docProperty.Insert(0, '/');
	//CFX_WideString wsSignTime = FS_GetSignDataTime();
	CFX_ByteString docProperty = "/Doc_0/Signs/Sign_0/Signature.xml";
	CFX_ByteString bsSignTime = wsDateTime.UTF8Encode();

	dwRet = pOESInterface->Sign(
		(FX_LPBYTE)(FX_LPCBYTE)bsSealID, 
		bsSealID.GetLength(),
		(FX_LPBYTE)(FX_LPCBYTE)docProperty,
		docProperty.GetLength(),
		(FX_LPBYTE)lpDigestData, 
		cpDigestData, 
		(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
		sealInfo->signMethod.GetLength(),
		(unsigned char*)(FX_LPCBYTE)bsSignTime,
		bsSignTime.GetLength(),
		NULL, 
		cbSign);

	if (OES_OK == dwRet)
	{
		pbSign = FX_Alloc(FX_BYTE, cbSign);
		memset(pbSign, 0, cbSign);

		dwRet = pOESInterface->Sign(
			(FX_LPBYTE)(FX_LPCBYTE)bsSealID, 
			bsSealID.GetLength(), 
			(FX_LPBYTE)(FX_LPCBYTE)docProperty,
			docProperty.GetLength(),
			(FX_LPBYTE)lpDigestData, 
			cpDigestData, 
			(unsigned char*)(FX_LPCBYTE)sealInfo->signMethod,
			sealInfo->signMethod.GetLength(),
			(unsigned char*)(FX_LPCBYTE)bsSignTime,
			bsSignTime.GetLength(),
			pbSign,
			cbSign);

		CFX_Base64Encoder base64Encoder;
		base64Encoder.Encode(CFX_ByteString((unsigned char *)pbSign, cbSign), bsSign);
	}

	if (lpDigestData)
		FX_Free(lpDigestData);
	if (pbSign)
		FX_Free(pbSign);

	return dwRet;
}

FX_DWORD FS_VerifyData(FX_LPBYTE pbData, FX_DWORD dwLen, CFX_ByteString& bsSigData, CFX_ByteString& bsSealData, CFX_ByteString& bsSignMethod, CFX_ByteString& bsDigestMethod, CFX_ByteString& bsDateTime, int nOnLine)
{
	if( pbData == NULL || dwLen == 0 )
		return -1;

	FX_LPBYTE lpDigestData = NULL;
	int  cpDigestData = 0;

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
	FX_DWORD dwRet = pOESInterface->Digest(pbData, dwLen, (unsigned char*)(FX_LPCBYTE)bsDigestMethod,
		bsDigestMethod.GetLength(), NULL, cpDigestData);
	if(OES_OK != dwRet)
	{
		return dwRet;
	}

	lpDigestData = new FX_BYTE[cpDigestData+1];
	memset(lpDigestData, 0, cpDigestData+1);

	dwRet = pOESInterface->Digest(pbData, dwLen, (unsigned char*)(FX_LPCBYTE)bsDigestMethod,
		bsDigestMethod.GetLength(), lpDigestData, cpDigestData);
	if (OES_OK != dwRet)
	{
		delete[] lpDigestData;
		lpDigestData = NULL;

		return dwRet;
	}

	CFX_ByteString bsDigest(lpDigestData, cpDigestData);
	CFX_Base64Encoder base64en;

	CFX_ByteString bsDigest1;
	base64en.Encode(bsDigest, bsDigest1);

	CFX_ByteString bsDocProperty = "/Doc_0/Signs/Sign_0/Signature.xml";
	int nFlags = 0;
	if( nOnLine != 0 )
		nFlags = 1;

	CFX_Base64Decoder decoderSeal;
	CFX_ByteString bsDecodeSeal;
	decoderSeal.Decode(bsSealData, bsDecodeSeal);

	CFX_Base64Decoder decoderSign;
	CFX_ByteString bsDecodeSign;
	decoderSeal.Decode(bsSigData, bsDecodeSign);

	FX_DWORD res = pOESInterface->Verify(
		(FX_LPBYTE)bsDecodeSeal.GetBuffer(bsDecodeSeal.GetLength()),
		bsDecodeSeal.GetLength(),
		(FX_LPBYTE)(FX_LPCBYTE(bsDocProperty)),
		bsDocProperty.GetLength(),
		(FX_LPBYTE)lpDigestData, 
		cpDigestData,	
		(FX_LPBYTE)bsSignMethod.GetBuffer(bsSignMethod.GetLength()),
		bsSignMethod.GetLength(),
		(FX_LPBYTE)bsDateTime.GetBuffer(bsDateTime.GetLength()), 
		bsDateTime.GetLength(),
		(FX_LPBYTE)bsDecodeSign.GetBuffer(bsDecodeSign.GetLength()), 
		bsDecodeSign.GetLength(),
		nOnLine);

	////////////////////////////
	if(res != OES_OK)
	{
		int piErrMessageLen = 0;
		pOESInterface->GetErrMessage(res, NULL, piErrMessageLen);

		unsigned char* puchErrMessage = new unsigned char[piErrMessageLen+1];
		memset(puchErrMessage, 0, piErrMessageLen+1);

		pOESInterface->GetErrMessage(res, puchErrMessage, piErrMessageLen);

		CFX_WideString wsTmp = CFX_WideString::FromUTF8((const char*)puchErrMessage, piErrMessageLen);

		delete[] puchErrMessage;
		puchErrMessage = NULL;
	}
	////////////////////////////

	delete[] lpDigestData;
	lpDigestData = NULL;

	return res;
}

FX_DWORD FS_VerifyOFD(COFD_Signature* pSign, int index, int nOnLine)
{
	if( pSign == NULL || index < 0 )
		return -1;

	CFX_WideString signMethod = pSign->GetSignatureMethod();
	CFX_ByteString bsSignMethod = signMethod.UTF8Encode();
	CFX_ByteString bsSignValue = pSign->GetSignatureValue();

	FX_DWORD dwSealDataLen = 0;
	FX_LPBYTE pSealData = NULL;
	IFX_FileRead* pSealFile = pSign->GetSealFile();
	if(pSealFile)
	{
		dwSealDataLen = pSealFile->GetSize(); 
		pSealData = new FX_BYTE[dwSealDataLen+1];
		memset(pSealData, 0, dwSealDataLen+1);

		int ret = pSealFile->ReadBlock(pSealData, 0, dwSealDataLen);
		if(!ret) {
            LOGD("file read failed");
            return -1;
        }
	}

	CFX_WideString signDataTime = pSign->GetSignatureDateTimeString();

	FX_INT32 refersCount = pSign->CountReferences();
	FX_LPBYTE	pbTxts	= NULL;
	FX_DWORD    cbTxts = 0;
	FX_LPBYTE	pbSign = NULL;
	unsigned int cbSign = 0;

	FX_LPBYTE pDigestMethod = NULL;
	int nDigestMethodLen = 0;
	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();
	LOGD("interface:%d",pOESInterface);
	FX_DWORD dwRet = pOESInterface->GetDigestMethod(NULL, nDigestMethodLen);
	if(OES_OK != dwRet)
	{
		if(pSealData)
			delete[] pSealData;
		pSealData = NULL;

		return dwRet;
	}

	pDigestMethod = new FX_BYTE[nDigestMethodLen+1];
	memset(pDigestMethod, 0, nDigestMethodLen+1);

	dwRet = pOESInterface->GetDigestMethod(pDigestMethod, nDigestMethodLen);
	if (OES_OK != dwRet)
	{
		delete[] pDigestMethod;
		pDigestMethod = NULL;

		if(pSealData)
			delete[] pSealData;
		pSealData = NULL;
		
		return dwRet;
	}

	FX_DWORD offer = 0;

	FX_LPBYTE lpDigestData = NULL;
	int  cpDigestData = 0;

	for (FX_INT32 j = 0; j < refersCount; j++)
	{
		COFD_SignReference* refer = pSign->GetReference(j);
		IFX_FileRead* file = refer->GetFile();
		if (file != NULL)
		{
			cbTxts = file->GetSize();
			pbTxts = new FX_BYTE[cbTxts+1];
			memset(pbTxts, 0, cbTxts+1);

			file->ReadBlock((FX_LPVOID)(pbTxts), 0, cbTxts);
			dwRet = pOESInterface->Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, NULL, cpDigestData);
			if(OES_OK != dwRet)
			{
				delete[] pbTxts;
				pbTxts = NULL;

				delete[] pDigestMethod;
				pDigestMethod = NULL;

				if(pSealData)
					delete[] pSealData;
				pSealData = NULL;

				return dwRet;
			}

			lpDigestData = new FX_BYTE[cpDigestData+1];
			memset(lpDigestData, 0, cpDigestData+1);

			dwRet = pOESInterface->Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, lpDigestData, cpDigestData);
			if (OES_OK != dwRet)
			{
				delete[] lpDigestData;
				lpDigestData = NULL;

				delete[] pbTxts;
				pbTxts = NULL;

				delete[] pDigestMethod;
				pDigestMethod = NULL;

				if(pSealData)
					delete[] pSealData;
				pSealData = NULL;

				return dwRet;
			}

			CFX_ByteString bsDigest(lpDigestData, cpDigestData);
			CFX_Base64Encoder base64en;
			CFX_WideString wsNewHashData;
			base64en.Encode(bsDigest, wsNewHashData);

			delete[] lpDigestData;
			lpDigestData = NULL;

			delete[] pbTxts;
			pbTxts = NULL;

			CFX_WideString wcOldHashData = refer->GetCheckValue();

			if (wsNewHashData.Compare(wcOldHashData) != 0)
			{
				delete[] pDigestMethod;
				pDigestMethod = NULL;

				if(pSealData)
					delete[] pSealData;
				pSealData = NULL;

				return -1;
			}
		}
	}

	IFX_FileRead* fileSignature = pSign->GetFile();
	cbTxts = fileSignature->GetSize();
	pbTxts = new FX_BYTE[cbTxts+1];
	memset(pbTxts, 0, cbTxts+1);

	fileSignature->ReadBlock((FX_LPVOID)(pbTxts), 0, cbTxts);

	//��Signature.xml����ժҪ
	dwRet = pOESInterface->Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, NULL, cpDigestData);
	if(OES_OK != dwRet)
	{
		delete[] pbTxts;
		pbTxts = NULL;

		delete[] pDigestMethod;
		pDigestMethod = NULL;

		if(pSealData)
			delete[] pSealData;
		pSealData = NULL;

		return dwRet;
	}

	lpDigestData = new FX_BYTE[cpDigestData+1];
	memset(lpDigestData, 0, cpDigestData+1);

	dwRet = pOESInterface->Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, lpDigestData, cpDigestData);
	if (OES_OK != dwRet)
	{
		delete[] lpDigestData;
		lpDigestData = NULL;

		delete[] pbTxts;
		pbTxts = NULL;

		delete[] pDigestMethod;
		pDigestMethod = NULL;

		if(pSealData)
			delete[] pSealData;
		pSealData = NULL;

		return dwRet;
	}

	CFX_ByteString byteSignTime = CFX_ByteString::FromUnicode(signDataTime);

	CFX_WideString fileName = ((IOFD_FileStream *)fileSignature)->GetCurrentFileName(0);

	CFX_ByteString docProperty = CFX_ByteString::FromUnicode(fileName);

	FX_LPBYTE fxDocProperty = (FX_LPBYTE)docProperty.GetBuffer(docProperty.GetLength());
	if(fxDocProperty[0] != '/')
		docProperty.Insert(0,'/');

	int nFlags = 0;
	if( nOnLine != 0 )
		nFlags = 1;

	FX_DWORD res = pOESInterface->Verify(
		(FX_LPBYTE)pSealData,
		dwSealDataLen,
		(FX_LPBYTE)docProperty.GetBuffer(docProperty.GetLength()),
		docProperty.GetLength(),
		(FX_LPBYTE)lpDigestData, 
		cpDigestData,	
		(FX_LPBYTE)bsSignMethod.GetBuffer(bsSignMethod.GetLength()),
		bsSignMethod.GetLength(),
		(FX_LPBYTE)byteSignTime.GetBuffer(byteSignTime.GetLength()), 
		byteSignTime.GetLength(),
		(unsigned char *)bsSignValue.GetBuffer(bsSignValue.GetLength()), 
		bsSignValue.GetLength(),
		nFlags);
    LOGD("res:%d",res);
	////////////////////////////
	if(res != OES_OK)
	{
		int piErrMessageLen = 0;
		pOESInterface->GetErrMessage(res, NULL, piErrMessageLen);

		unsigned char* puchErrMessage = new unsigned char[piErrMessageLen+1];
		memset(puchErrMessage, 0, piErrMessageLen+1);

		pOESInterface->GetErrMessage(res, puchErrMessage, piErrMessageLen);
        LOGD("ERROR:%s", puchErrMessage);
		CFX_WideString wsTmp = CFX_WideString::FromUTF8((const char*)puchErrMessage, piErrMessageLen);

		delete[] puchErrMessage;
		puchErrMessage = NULL;
	}
	////////////////////////////

	//if (fileSignature) fileSignature->Release(); //ע�͵������IOFD_FilePackage::Release����������

	delete[] lpDigestData;
	lpDigestData = NULL;

	delete[] pbTxts;
	pbTxts = NULL;

	delete[] pDigestMethod;
	pDigestMethod = NULL;

	if(pSealData)
		delete[] pSealData;
	pSealData = NULL;

	return res;
}

void* FS_GetSealProperty(IFX_FileRead* pSealFile)
{
	if(pSealFile == NULL)
		return NULL;

	//CFS_OFDSignatureManage* pSignManage;// = CFS_OFDSDKMgr::Get()->GetSignatureManageHandler();
	CFS_OESInterface* pOESInterface = pSignManage->GetOESInterface();

	FX_DWORD dwSealDataLen = 0;
	FX_LPBYTE pSealData = NULL;

	if(pSealFile)
	{
		dwSealDataLen = pSealFile->GetSize(); 
		pSealData = new FX_BYTE[dwSealDataLen+1];
		memset(pSealData, 0, dwSealDataLen+1);

		pSealFile->ReadBlock(pSealData, 0, dwSealDataLen);
	}

	FX_BYTE* lpSealId = NULL;
	FX_INT32 iSealIdLen = 0;
	FX_BYTE* lpVersion = NULL;
	FX_INT32 iVersionLen = 0;
	FX_BYTE* lpVenderId = NULL;
	FX_INT32 iVenderIdLen = 0;
	FX_BYTE* lpSealType = NULL;
	FX_INT32 iSealTypeLen = 0;
	FX_BYTE* lpSealName = NULL;
	FX_INT32 iSealNameLen = 0;
	FX_BYTE* lpCertInfo = NULL;
	FX_INT32 iCertInfoLen = 0;
	FX_BYTE* lpValidStart = NULL;
	FX_INT32 iValidStartLen = 0;
	FX_BYTE* lpValidEnd = NULL;
	FX_INT32 iValidEndLen = 0;
	FX_BYTE* lpSignedDate = NULL;
	FX_INT32 iSignedDateLen = 0;
	FX_BYTE* lpSignerName = NULL;
	FX_INT32 iSignerNameLen = 0;
	FX_BYTE* lpSignMethod = NULL;
	FX_INT32 iSignMethodLen = 0;

	FX_DWORD dwRet = pOESInterface->GetSealInfo(pSealData, dwSealDataLen
												, NULL, iSealIdLen
												, NULL, iVersionLen
												, NULL, iVenderIdLen
												, NULL, iSealTypeLen
												, NULL, iSealNameLen
												, NULL, iCertInfoLen
												, NULL, iValidStartLen
												, NULL, iValidEndLen
												, NULL, iSignedDateLen
												, NULL, iSignerNameLen
												, NULL, iSignMethodLen
												);

	if(dwRet != OES_OK)
	{
		if(pSealData)
		delete[] pSealData;
		pSealData = NULL;

		return NULL;
	}

	lpSealId = new FX_BYTE[iSealIdLen + 1];
	memset(lpSealId, 0, iSealIdLen + 1);

	lpVersion = new FX_BYTE[iVersionLen + 1];
	memset(lpVersion, 0, iVersionLen + 1);

	lpVenderId = new FX_BYTE[iVenderIdLen + 1];
	memset(lpVenderId, 0, iVenderIdLen + 1);

	lpSealType = new FX_BYTE[iSealTypeLen + 1];
	memset(lpSealType, 0, iSealTypeLen + 1);

	lpSealName = new FX_BYTE[iSealNameLen + 1];
	memset(lpSealName, 0, iSealNameLen + 1);

	lpCertInfo = new FX_BYTE[iCertInfoLen + 1];
	memset(lpCertInfo, 0, iCertInfoLen + 1);

	lpValidStart = new FX_BYTE[iValidStartLen + 1];
	memset(lpValidStart, 0, iValidStartLen + 1);

	lpValidEnd = new FX_BYTE[iValidEndLen + 1];
	memset(lpValidEnd, 0, iValidEndLen + 1);

	lpSignedDate = new FX_BYTE[iSignedDateLen + 1];
	memset(lpSignedDate, 0, iSignedDateLen + 1);

	lpSignerName = new FX_BYTE[iSignerNameLen + 1];
	memset(lpSignerName, 0, iSignerNameLen + 1);

	lpSignMethod = new FX_BYTE[iSignMethodLen + 1];
	memset(lpSignMethod, 0, iSignMethodLen + 1);

	dwRet = pOESInterface->GetSealInfo(pSealData, dwSealDataLen
												, lpSealId, iSealIdLen
												, lpVersion, iVersionLen
												, lpVenderId, iVenderIdLen
												, lpSealType, iSealTypeLen
												, lpSealName, iSealNameLen
												, lpCertInfo, iCertInfoLen
												, lpValidStart, iValidStartLen
												, lpValidEnd, iValidEndLen
												, lpSignedDate, iSignedDateLen
												, lpSignerName, iSignerNameLen
												, lpSignMethod, iSignMethodLen
												);

	if(dwRet != OES_OK)
	{
		delete[] lpSealId;
		lpSealId = NULL;
		delete[] lpVersion;
		lpVersion = NULL;
		delete[] lpVenderId;
		lpVenderId = NULL;
		delete[] lpSealType;
		lpSealType = NULL;
		delete[] lpSealName;
		lpSealName = NULL;
		delete[] lpCertInfo;
		lpCertInfo = NULL;
		delete[] lpValidStart;
		lpValidStart = NULL;
		delete[] lpValidEnd;
		lpValidEnd = NULL;
		delete[] lpSignedDate;
		lpSignedDate = NULL;
		delete[] lpSignerName;
		lpSignerName = NULL;
		delete[] lpSignMethod;
		lpSignMethod = NULL;

		if(pSealData)
		delete[] pSealData;
		pSealData = NULL;

		return NULL;
	}

	CFX_MapPtrToPtr* pSealDataMap = FX_NEW CFX_MapPtrToPtr;
	
	if(lpSealId != NULL && iSealIdLen > 0)
	{
		CFX_WideString* pwsSealID = FX_NEW CFX_WideString;
		*pwsSealID = CFX_WideString::FromUTF8((const char*)lpSealId, iSealIdLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_ID), pwsSealID);
	}

	if(lpVersion != NULL && iVersionLen > 0)
	{
		CFX_WideString* pwsVersion = FX_NEW CFX_WideString;
		*pwsVersion = CFX_WideString::FromUTF8((const char*)lpVersion, iVersionLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_Version), pwsVersion);
	}

	if(lpVenderId != NULL && iVenderIdLen > 0)
	{
		CFX_WideString* pwsVenderId = FX_NEW CFX_WideString;
		*pwsVenderId = CFX_WideString::FromUTF8((const char*)lpVenderId, iVenderIdLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_VenderId), pwsVenderId);
	}

	if(lpSealType != NULL && iSealTypeLen > 0)
	{
		CFX_WideString* pwsSealType = FX_NEW CFX_WideString;
		*pwsSealType = CFX_WideString::FromUTF8((const char*)lpSealType, iSealTypeLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_Type), pwsSealType);
	}

	if(lpSealName != NULL && iSealNameLen > 0)
	{
		CFX_WideString* pwsSealName = FX_NEW CFX_WideString;
		*pwsSealName = CFX_WideString::FromUTF8((const char*)lpSealName, iSealNameLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_Name), pwsSealName);
	}

	if(lpCertInfo != NULL && iCertInfoLen > 0)
	{
		CFX_WideString* pwsCertInfo = FX_NEW CFX_WideString;
		*pwsCertInfo = CFX_WideString::FromUTF8((const char*)lpCertInfo, iCertInfoLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_CertInfo), pwsCertInfo);
	}

	if(lpValidStart != NULL && iValidStartLen > 0)
	{
		CFX_WideString* pwsValidStart = FX_NEW CFX_WideString;
		*pwsValidStart = CFX_WideString::FromUTF8((const char*)lpValidStart, iValidStartLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_ValidStart), pwsValidStart);
	}

	if(lpValidEnd != NULL && iValidEndLen > 0)
	{
		CFX_WideString* pwsValidEnd = FX_NEW CFX_WideString;
		*pwsValidEnd = CFX_WideString::FromUTF8((const char*)lpValidEnd, iValidEndLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_ValidEnd), pwsValidEnd);
	}

	if(lpSignedDate != NULL && iSignedDateLen > 0)
	{
		CFX_WideString* pwsSignedDate = FX_NEW CFX_WideString;
		*pwsSignedDate = CFX_WideString::FromUTF8((const char*)lpSignedDate, iSignedDateLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_SignedDate), pwsSignedDate);
	}

	if(lpSignerName != NULL && iSignerNameLen > 0)
	{
		CFX_WideString* pwsSignerName = FX_NEW CFX_WideString;
		*pwsSignerName = CFX_WideString::FromUTF8((const char*)lpSignerName, iSignerNameLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_SignerName), pwsSignerName);
	}

	if(lpSignMethod != NULL && iSignMethodLen > 0)
	{
		CFX_WideString* pwsSignMethod = FX_NEW CFX_WideString;
		*pwsSignMethod = CFX_WideString::FromUTF8((const char*)lpSignMethod, iSignMethodLen);
		pSealDataMap->SetAt((FX_LPVOID)(Seal_SignMethod), pwsSignMethod);
	}

	delete[] lpSealId;
	lpSealId = NULL;
	delete[] lpVersion;
	lpVersion = NULL;
	delete[] lpVenderId;
	lpVenderId = NULL;
	delete[] lpSealType;
	lpSealType = NULL;
	delete[] lpSealName;
	lpSealName = NULL;
	delete[] lpCertInfo;
	lpCertInfo = NULL;
	delete[] lpValidStart;
	lpValidStart = NULL;
	delete[] lpValidEnd;
	lpValidEnd = NULL;
	delete[] lpSignedDate;
	lpSignedDate = NULL;
	delete[] lpSignerName;
	lpSignerName = NULL;
	delete[] lpSignMethod;
	lpSignMethod = NULL;

	if(pSealData)
		delete[] pSealData;
	pSealData = NULL;

	return (void*)pSealDataMap;
}

#endif

#if _FX_PRODUCT_ == _FS_ESSDK_ || _FX_PRODUCT_ == _FS_EOSDK_ || _FX_PRODUCT_ == _FS_NONE_ || _FX_PRODUCT_ == _FS_ASDK_/*20190813*/ || _FX_PRODUCT_ == _FS_GSDK_

#define FS_SHA_DIGESTSIZE 32

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
#define UES_PFX

#ifdef UES_PFX
static FX_BOOL FS_CertSignData(const unsigned char* pbTxts[], FX_DWORD	cbTxts[], FX_DWORD cTxts, PCCERT_CONTEXT pSigndCert, 
	unsigned char*&	pbCrypted, FX_DWORD& cbCrypted)
{
	CRYPT_SIGN_MESSAGE_PARA  SigParams;
	::ZeroMemory(&SigParams, sizeof(CRYPT_SIGN_MESSAGE_PARA));
	SigParams.cbSize = sizeof(CRYPT_SIGN_MESSAGE_PARA);
	SigParams.dwMsgEncodingType =  X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
	SigParams.pSigningCert = pSigndCert;
	SigParams.HashAlgorithm.pszObjId = szOID_RSA_SHA256RSA;//szOID_OIWSEC_sha1
	//SigParams.HashAlgorithm.pszObjId = szOID_OIWSEC_sha1;
	SigParams.HashAlgorithm.Parameters.cbData = NULL;
	SigParams.cMsgCert = 1;
	SigParams.rgpMsgCert = &pSigndCert;
	SigParams.cAuthAttr = 0;
	SigParams.dwInnerContentType = 0;
	SigParams.cMsgCrl = 0;
	SigParams.cUnauthAttr = 0;
	SigParams.dwFlags = 0;
	SigParams.pvHashAuxInfo = NULL;
	SigParams.rgAuthAttr = NULL;
	if (!::CryptSignMessage(
		&SigParams,
		TRUE,
		cTxts,
		pbTxts,
		cbTxts,
		NULL,
		&cbCrypted) || cbCrypted == 0)
	{
		FX_DWORD err =  GetLastError();

		FXSYS_assert(err == 0);
		return FALSE;
	}
	pbCrypted = (unsigned char*)FX_Alloc(unsigned char, cbCrypted);
	::ZeroMemory((void*)pbCrypted, cbCrypted);
	if(!::CryptSignMessage(
		&SigParams,
		TRUE,
		cTxts,
		pbTxts,
		cbTxts,
		pbCrypted,
		&cbCrypted))
	{
		FX_DWORD dwError = GetLastError();
		FX_Free(pbCrypted);
		pbCrypted = NULL;
		return FALSE;
	}

	return TRUE;
}
#else

#include "pdfwindow/win32_crypt.h"

static FX_BOOL FS_CertSignData(CPSL_CryptLib& cryptLib, const unsigned char* pbTxts[], FX_DWORD	cbTxts[], FX_DWORD cTxts, PCCERT_CONTEXT pSigndCert, 
	unsigned char*&	pbCrypted, FX_DWORD& cbCrypted)
{
	CRYPT_SIGN_MESSAGE_PARA  SigParams;
	::ZeroMemory(&SigParams, sizeof(CRYPT_SIGN_MESSAGE_PARA));
	SigParams.cbSize = sizeof(CRYPT_SIGN_MESSAGE_PARA);
	SigParams.dwMsgEncodingType =  X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
	SigParams.pSigningCert = pSigndCert;
	SigParams.HashAlgorithm.pszObjId = szOID_OIWSEC_sha1;
	SigParams.HashAlgorithm.Parameters.cbData = NULL;
	SigParams.cMsgCert = 1;
	SigParams.rgpMsgCert = &pSigndCert;
	SigParams.cAuthAttr = 0;
	SigParams.dwInnerContentType = 0;
	SigParams.cMsgCrl = 0;
	SigParams.cUnauthAttr = 0;
	SigParams.dwFlags = 0;
	SigParams.pvHashAuxInfo = NULL;
	SigParams.rgAuthAttr = NULL;
	if (!cryptLib.CryptSignMessage(
		&SigParams,
		TRUE,
		cTxts,
		pbTxts,
		cbTxts,
		NULL,
		&cbCrypted) || cbCrypted == 0)
	{
		FX_DWORD err =  GetLastError();

		TCHAR szBuf[128];
            LPVOID lpMsgBuf;
		FormatMessage(
                   FORMAT_MESSAGE_ALLOCATE_BUFFER |
                   FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   err,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPTSTR) &lpMsgBuf,
                   0, NULL );
 
                wsprintf(szBuf,
                    (L"%s ������Ϣ (�����룽%d): %s"),
                    (L"CreateDirectory"), err, lpMsgBuf);
 
               LocalFree(lpMsgBuf);
 
               //�����ʾ��
              OutputDebugString(szBuf);

		FXSYS_assert(err == 0);
		return FALSE;
	}
	pbCrypted = (unsigned char*)FX_Alloc(unsigned char, cbCrypted);
	::ZeroMemory((void*)pbCrypted, cbCrypted);
	if(!cryptLib.CryptSignMessage(
		&SigParams,
		TRUE,
		cTxts,
		pbTxts,
		cbTxts,
		pbCrypted,
		&cbCrypted))
	{
		FX_DWORD dwError = GetLastError();
		FX_Free(pbCrypted);
		pbCrypted = NULL;
		return FALSE;
	}

	return TRUE;
}

#endif

static FX_BOOL IsPrivateHasUsage(PCCERT_CONTEXT pCertContext)
{
	PCERT_ENHKEY_USAGE pCrtUsage = NULL;
	FX_DWORD dwUsage;
	FX_BOOL bHasEnhance = FALSE;
	FX_BOOL bRet = ::CertGetEnhancedKeyUsage(
		pCertContext, 
		CERT_FIND_EXT_ONLY_ENHKEY_USAGE_FLAG, 
		NULL, 
		&dwUsage);
	if (bRet && dwUsage>0)
	{
		pCrtUsage = (PCERT_ENHKEY_USAGE)FX_Alloc(FX_BYTE, dwUsage);
		::CertGetEnhancedKeyUsage
			(pCertContext, 
			CERT_FIND_EXT_ONLY_ENHKEY_USAGE_FLAG, 
			pCrtUsage, 
			&dwUsage);
		if (pCrtUsage->rgpszUsageIdentifier)
			bHasEnhance = TRUE;
	}

	FX_BYTE usage;
	::CertGetIntendedKeyUsage(pCertContext->dwCertEncodingType, 
		pCertContext->pCertInfo,
		&usage, 
		1);
	FX_BOOL bIsPKC = FALSE;
	if (	( (usage & CERT_DIGITAL_SIGNATURE_KEY_USAGE) == CERT_DIGITAL_SIGNATURE_KEY_USAGE )
		||	( (usage == 0) /*&& !bHasEnhance */)) //û������key usage ��ֻenhanced key usge�ģ���������ǩ����Mantis ID 0048391
	{
		bIsPKC = TRUE;
	}
	if (pCrtUsage)
	{
		free(pCrtUsage);
	}

	return bIsPKC;
}

static FX_BOOL IsPrivateKeyCert(PCCERT_CONTEXT pCertContext)
{
	FX_BOOL bFree;
	FX_DWORD dwKeySpec = 0;
	HCRYPTPROV hCryptProv = NULL;
	FX_BOOL bHasPrivateKey = FALSE;

	if ( ::CryptAcquireCertificatePrivateKey(
		pCertContext,
		CRYPT_ACQUIRE_CACHE_FLAG | CRYPT_ACQUIRE_USE_PROV_INFO_FLAG | CRYPT_ACQUIRE_COMPARE_KEY_FLAG
		| 0x00000008 |CRYPT_ACQUIRE_PREFER_NCRYPT_KEY_FLAG,
		NULL,
		&hCryptProv,
		&dwKeySpec,
		&bFree) )
	{
		bHasPrivateKey = IsPrivateHasUsage(pCertContext);
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bHasPrivateKey;
}

#endif


#if !(_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
#define FREE_CERT_KEY	if(pkey)\
	EVP_PKEY_free(pkey);\
	if(x509)\
	X509_free(x509);\
	if(ca)\
	sk_X509_free(ca);\

static int parseP12File(const CFX_WideString& wsCertFile, const CFX_WideString& pwd, EVP_PKEY** pkey, X509** x509, STACK_OF(X509)** ca)
{
	//ȡPKCS12����
	CFX_ByteString bsPath = wsCertFile.UTF8Encode();
	FILE* fp = NULL; 
	if (!(fp = fopen(bsPath, "rb")))
	{
		return -1;
	}

	PKCS12* p12 = d2i_PKCS12_fp(fp, NULL); 
	fclose (fp);      
	if (!p12)
	{
		printf("Error reading PKCS#12 file\n");	
		return -2;
	}
	//ȡpkey����X509֤�顢֤����
	CFX_ByteString bsPwd = pwd.UTF8Encode();
	if (!PKCS12_parse(p12, bsPwd, pkey, x509, ca)) {  
		printf("Error parsing PKCS#12 file\n");
		return -3; 
	}   

	PKCS12_free(p12); 
	if (!pkey) {   
		return -4;
	}
	return 0;
}
#endif
/*
static FX_DWORD FS_CertSignData_GM(const CFX_WideString& wsCertFile, const CFX_WideString& pwd, FX_LPCBYTE pPlain, FX_DWORD len, CFX_ByteString& bsSignedData)
{
	CFS_SignatureHandlerAdbePkcs7DetachedGmssl* pGmPkcs7 = CFS_SignatureHandlerAdbePkcs7DetachedGmssl::Get();
	if(pGmPkcs7 == NULL)
		return -1;

	FX_INT32 nRet = pGmPkcs7->ParseP12File(wsCertFile, pwd);
	if(nRet != 0)
		return nRet;

	CFX_ByteString bsPlainData(pPlain, len);

	FX_BOOL bRet = pGmPkcs7->Sign(bsPlainData, bsSignedData);
	if(bRet)
		return 0;

	return -1;
}
*/
static FX_DWORD FS_CertSignData(const CFX_WideString& wsCertFile, const CFX_WideString& pwd, FX_LPCBYTE pPlain, FX_DWORD len,
	unsigned char*&	pbCrypted, FX_DWORD& cbCrypted)
{
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	IFX_FileRead* fpCert = FX_CreateFileRead(wsCertFile);
	if (!fpCert) return -1;

	long cbData = fpCert->GetSize();
	FX_BYTE* pbData = new FX_BYTE[cbData];
	fpCert->ReadBlock(pbData, cbData);
	fpCert->Release();

	//���ɽṹ��
	CRYPT_DATA_BLOB CDB;
	CDB.cbData = cbData;
	CDB.pbData = (BYTE*)pbData;

#ifdef UES_PFX
	HCERTSTORE hStore = NULL;
	//��֤�鵼����ʱ֤���
	if (!(hStore = PFXImportCertStore(&CDB, pwd, CRYPT_EXPORTABLE)))
	{
		return -1;
	}

	//ö����ʱ֤����е����֤��
	PCCERT_CONTEXT pCertContext = NULL;
	PCCERT_CONTEXT pSignCertContext = NULL;
	while (pCertContext = CertEnumCertificatesInStore(hStore, pCertContext))
	{
//		if (IsPrivateKeyCert(pCertContext))
		{
			pSignCertContext = pCertContext;
			break;
		}
	}
#else
	
	CPSL_CryptLib cryptLib;
	PCCERT_CONTEXT pSignCertContext = cryptLib.CertCreateCertificateContext(PSL_ENCODING_TYPE, pbData, cbData);
#endif

	delete pbData;

	FX_DWORD cbTxts[] = {len};
	FX_LPCBYTE	pbTxts[] = {pPlain};

#ifdef UES_PFX

	FX_BOOL bSigned = FS_CertSignData(pbTxts, cbTxts, sizeof(cbTxts)/sizeof(FX_DWORD), pSignCertContext, pbCrypted, cbCrypted);

	::CertCloseStore(hStore, 0);
	::CertFreeCertificateContext(pSignCertContext);
#else

	FX_BOOL bSigned = FS_CertSignData(cryptLib, pbTxts, cbTxts, sizeof(cbTxts)/sizeof(FX_DWORD), pSignCertContext, pbCrypted, cbCrypted);

	cryptLib.CertFreeCertificateContext(pSignCertContext);

#endif

	return bSigned ? 0 : -1;
#else
	int err = 0;
 
	PKCS7* p7 = NULL;
	EVP_PKEY* pkey = NULL;       
	X509* x509 = NULL;  
	STACK_OF(X509)* ca = NULL;
    
    OpenSSL_add_all_algorithms();
//    SSLeay_add_all_algorithms();

	if(parseP12File(wsCertFile, pwd, &pkey, &x509, &ca))
		return -1;

	p7 = PKCS7_new();
	PKCS7_set_type(p7, NID_pkcs7_signed);
	PKCS7_content_new(p7, NID_pkcs7_data);	

	//Application should not judge the sign algorithm with the content's length.
	//Here, just for convenient;
	PKCS7_ctrl(p7, PKCS7_OP_SET_DETACHED_SIGNATURE, 1, NULL);

	PKCS7_SIGNER_INFO* info = PKCS7_add_signature(p7, x509, pkey, EVP_sha1());
	PKCS7_add_certificate(p7, x509);
    
    
// 0928... x509
//    X509
//    x509
//	for (int i = 0; i< sk_num(CHECKED_STACK_OF(X509,ca)); i++)
//		PKCS7_add_certificate(p7, (X509*)sk_value(CHECKED_STACK_OF(X509,ca), i));
        
    
    
    
	//Set source data to BIO.
	BIO* p7bio = PKCS7_dataInit(p7, NULL);
	BIO_write(p7bio, pPlain, len);
	PKCS7_dataFinal(p7, p7bio); 

	FREE_CERT_KEY;
	BIO_free_all(p7bio);

	//Get Signed data.
	unsigned char* der = NULL;  
	unsigned char* derTmp = NULL;  
	unsigned long derlen = i2d_PKCS7(p7, NULL);  
	der = (unsigned char*)malloc(derlen);  
	memset(der, 0, derlen);
	derTmp = der;  

	i2d_PKCS7(p7, &derTmp);			 
	cbCrypted = derlen;
	pbCrypted = (unsigned char*)der;
	PKCS7_free(p7); 

	return 0;
#endif
}

FX_DWORD FX_CertVerifyData(FX_LPCBYTE pPlain, FX_DWORD len, CFX_ByteString bsSignValue)
{
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	const FX_BYTE* MessageArray[] = { pPlain };
	FX_DWORD dwBufLen = len;
	FX_DWORD MessageSizeArray[] = { dwBufLen };

	CRYPT_VERIFY_MESSAGE_PARA VerifyParams;
	VerifyParams.cbSize = sizeof(CRYPT_VERIFY_MESSAGE_PARA);
	VerifyParams.dwMsgAndCertEncodingType = PKCS_7_ASN_ENCODING| X509_ASN_ENCODING;
	VerifyParams.hCryptProv = 0;
	VerifyParams.pfnGetSignerCertificate = NULL;
	VerifyParams.pvGetArg = NULL;

	PCCERT_CONTEXT pCertContext = NULL ;
	if (!::CryptVerifyDetachedMessageSignature(
		&VerifyParams,				// Verify parameters.
		0,							// Signer index.
		bsSignValue,				// Pointer to signed BLOB.
		bsSignValue.GetLength(),	// Size of signed BLOB.
		1,							//  Number of array elements 
		MessageArray,				// Buffer for decoded message.
		MessageSizeArray,			// Size of buffer.
		&pCertContext))				// Pointer to signer certificate.
	{
		return -2;
	}
	else
	{
		return 0;
	}
#else
	FX_BOOL bSigAppr = FALSE;
	int length = 0;
	unsigned char *p = NULL;

    OpenSSL_add_all_algorithms();
//    SSLeay_add_all_algorithms();

	//Retain PKCS7 object from signed data. 
	BIO* vin = BIO_new_mem_buf(bsSignValue.GetBuffer(bsSignValue.GetLength()), bsSignValue.GetLength());
	PKCS7* p7 = d2i_PKCS7_bio(vin, NULL);  

	STACK_OF(PKCS7_SIGNER_INFO) *sk = PKCS7_get_signer_info(p7);
	int signCount = sk_PKCS7_SIGNER_INFO_num(sk);

	for(int i=0;i<signCount; i++)
	{
		PKCS7_SIGNER_INFO *signInfo = sk_PKCS7_SIGNER_INFO_value(sk,i);
		ASN1_TYPE *tstoken = PKCS7_get_attribute(signInfo, NID_id_smime_aa_timeStampToken);
		BIO *p7bio = BIO_new_mem_buf((void*)pPlain, len);;
		X509 *x509= PKCS7_cert_from_signer_info(p7,signInfo);
		int err = PKCS7_verify(p7, NULL, NULL,p7bio, NULL, PKCS7_NOVERIFY); 
		if(err == 1)	
			bSigAppr = TRUE;	
		BIO_free(p7bio);
	}

	PKCS7_free(p7);            
	BIO_free(vin);    

	if(bSigAppr)
		return 0;
	return -1;
#endif
}

#endif //_FX_PRODUCT_ == _FS_ESSDK_ || _FX_PRODUCT_ == _FS_NONE_
