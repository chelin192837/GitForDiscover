
//#include <jni.h>

#include <stdlib.h>
#include <sstream>
#include <fstream>

#include "../include/ofd.h"

#include "fxlib.h"
#include "include/fs_ofdfilepackage.h"
#include "include/fs_ofddocument.h"
#include "include/fs_ofdpage.h"
#include "include/fs_ofdsignature.h"
#include "include/fs_ofdsignaturefun.h"
#include "../int/ofd_es.h"

#include "strutil.h"

#include "./oes.h"

#include "SESeal.h"
#include "SES_Signature.h"
#include "SESeal.h"
#include "SES_Header.h"
#include "../../rdk/src/ofd/htfoxit_oes/OES_DLL/include/oes_dll.h"
#include <SES_ESPropertyInfo.h>


#if _FX_PRODUCT_ == _FS_ESSDK_ || _FX_PRODUCT_ == _FS_NONE_ || _FX_PRODUCT_ == _FS_EOSDK_ || _FX_PRODUCT_ == _FS_EISDK_ || _FX_PRODUCT_ == _FS_ASDK_/*20190813*/ || _FX_PRODUCT_ == _FS_GSDK_

// ESSDK

struct sealinfo;
int OFD_SEAL_Init(OFD_LPCSTR lpszOESDir, OFD_LPCSTR lpszPin)
{
	if (!lpszOESDir)
		return -1;

	CFX_WideString wsOESFile = CFX_WideString::FromUTF8(lpszOESDir);

	//CFX_WideString wsOESFile;
	//FS_GetSealModuleFileName(wsOESDir, wsOESFile);

	//if (!FS_IsFileExist(wsOESFile))
	//	return OFD_ERRCODE_NoFoundDLL;
	CFX_ByteString bsPin(lpszPin);
	int iLoad = FS_LoadSealModule(wsOESFile, bsPin);

	return iLoad;
}

void OFD_SEAL_Destory()
{
	FS_FreeSealModule();
}

//��checkSeallist == 1���� seal list ��ȡӡ��ID���� lpszSeal Ϊ��
//checkSeallist == 1 ʱ ������ȵ��� OES �� GetSealList �ӿڻ�ȡ SealID
void* OFD_SEAL_Load(OFD_LPCSTR lpszSeal, int sealType/*0��ʾӡ��ID��1��ʾӡ������*/, int bCheckSeallist, OFD_LPCSTR lpszPin, int* err)
{
	CFX_ByteString bsSeal(lpszSeal);
	return FS_LoadSealInfo(bsSeal, sealType != 0, bCheckSeallist, err);
}

//
void OFD_SEAL_UnLoad(void* hSeal)
{
	if (!hSeal)
		return;

	FS_UnLoadSealInfo(hSeal);
}


int OFD_SEAL_Sign_KT_Pos(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, float fXPos
				, float fYPos
				, int* pPageArr
				, int nSize
				, int nSubRange
				, OFD_LPCWSTR pcwSignFile		//B64
				)
{
	if(hSeal == NULL || pcwFileName == NULL)
		return -1;

	// �ļ��� Base64 ����
	CFX_WideString wsFileName(pcwFileName);
	CFX_Base64Decoder base64Decoder;
	CFX_ByteString bsFileName = wsFileName.UTF8Encode();
	//base64Decoder.Decode(wsFileName, bsFileName);
	LOGD("in Filename:%s", wcharTochar(pcwFileName));
	int ret = -1;
	IFX_FileRead* pFileRead = FX_CreateFileRead(bsFileName);
	if(pFileRead == NULL)
		return -1;

	IOFD_FilePackage*	pOFDPackage = OFD_FilePackage_Create(pFileRead);
	if(pOFDPackage == NULL)
	{
		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	COFD_DocProvider* pDocProvider = FX_NEW COFD_DocProvider;
	pDocProvider->Init();

	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, pDocProvider);
	if(pOFDParser == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)hSeal;
	if(OFD_SEAL_OFD == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealImageStream);
	}else if(OFD_SEAL_BITMAP == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealBmp);
	}

	IOFD_Document* pOFDDoc = pOFDParser->GetDocument(0);
	if(pOFDDoc == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;

		return -1;
	}

	IOFD_WriteDocument* pWriteDocument = OFD_WriteDoc_Create(pOFDDoc);

	// �ļ��� Base64 ����
	CFX_WideString wsSignFileName(pcwSignFile);
	CFX_Base64Decoder base64Decoder2;
	CFX_ByteString bsSignFileName;
	base64Decoder2.Decode(wsSignFileName, bsSignFileName);

	IFX_FileWrite* pFileWrite = FX_CreateFileWrite(wcharTochar(pcwSignFile));

	FS_SignOFD_KTPOS(hSeal, pWriteDocument, pPageArr, nSize, nSubRange,  fXPos, fYPos, pFileWrite, 0);

	if (pFileWrite)
		pFileWrite->Release();
	pFileWrite = NULL;

	delete pDocProvider;
	pDocProvider = NULL;

	pOFDPackage->Release();
	pOFDPackage = NULL;

	pFileRead->Release();
	pFileRead = NULL;

	return 0;
}

int OFD_SEAL_Sign_KT_Pos2(IOFD_WriteDocument* pWriteDocument
		, void* hSeal
		, float fXPos
		, float fYPos
		, int* pPageArr
		, int nSize
		, int nSubRange
		, OFD_LPCWSTR pcwSignFile
)
{
	if(hSeal == NULL || pWriteDocument == NULL)
		return -1;

	COFD_DocProvider* pDocProvider = FX_NEW COFD_DocProvider;
	pDocProvider->Init();

	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)hSeal;
	if(OFD_SEAL_OFD == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealImageStream);
	}else if(OFD_SEAL_BITMAP == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealBmp);
	}

	CFX_WideString wsSignFileName(pcwSignFile);
	IFX_FileWrite* pFileWrite = FX_CreateFileWrite(wsSignFileName);
    if(pFileWrite == NULL) {
		LOGD("open signed file failed:%ls", wsSignFileName.GetBuffer(wsSignFileName.GetLength()));
		return -1;
	}
	//IFX_MemoryStream* pMemStream = FX_CreateMemoryStream();
	FX_DWORD dwRet = FS_SignOFD_KTPOS(hSeal, pWriteDocument, pPageArr, nSize, nSubRange, fXPos, fYPos, pFileWrite, 0);

	if (pFileWrite)
		pFileWrite->Release();
	pFileWrite = NULL;

	delete pDocProvider;
	pDocProvider = NULL;

	return dwRet;
}

#include "include/fs_ofdsearch.h"
#include "oes.h"

int OFD_SEAL_Sign_KT_Key(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, OFD_LPCWSTR pcwKeyWord
				, int nMatchCase
				, int nMatchWord
				, int nKeyIndex
				, int nSealSignType
				, OFD_LPCWSTR pcwSignFile		//B64
				)
{
	if(hSeal == NULL || pcwFileName == NULL || pcwKeyWord == NULL)
		return -1;

	CFX_WideString wsKey(pcwKeyWord);

	// �ļ��� Base64 ����
	CFX_WideString wsFileName(pcwFileName);
	CFX_Base64Decoder base64Decoder;
	CFX_ByteString bsFileName;
	base64Decoder.Decode(wsFileName, bsFileName);

	int ret = -1;
	IFX_FileRead* pFileRead = FX_CreateFileRead(bsFileName);
	if(pFileRead == NULL)
		return -1;

	IOFD_FilePackage*	pOFDPackage = OFD_FilePackage_Create(pFileRead);
	if(pOFDPackage == NULL)
	{
		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	COFD_DocProvider* pDocProvider = FX_NEW COFD_DocProvider;
	pDocProvider->Init();

	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, pDocProvider);
	if(pOFDParser == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	IOFD_Document* pOFDDoc = pOFDParser->GetDocument(0);
	FX_INT32 nCounts = pOFDDoc->CountPages();
	CFS_OFDSearch* pSearch = new CFS_OFDSearch;
	for(int i = 0; i < nCounts; i++)
	{
		IOFD_Page* pOFDPage = pOFDDoc->GetPage(i);
		IOFD_TextPageFind* pFind = IOFD_TextPageFind::CreateTextFind(pOFDPage);
		FX_INT32 nFind = pFind->FindFirstEx(wsKey, 0);
		while ((nFind = pFind->FindNextEx()) != -1)
		{
			CFS_OFDSearchResult* pResult = new CFS_OFDSearchResult;

			FX_BOOL bRet = pFind->GetRectArray(pResult->m_ArrRects);
			pSearch->AddResult(pResult);

			pResult->nPageID = pOFDPage->GetID();
			pResult->nPageIndex = pOFDDoc->GetPageIndex(pOFDPage);
			pResult->nDocIndex = 0;
		}
	}

	int nKeyCount = OFD_Search_Count((OFD_SEARCH)pSearch);
	
	for(int i = 0; i < nKeyCount; i++)
	{
		//int pageID = OFD_Search_GetProperty((OFD_SEARCH)pSearch, i, 3); //ҳ������
	}

	if(nKeyIndex > 0)
		nKeyIndex = nKeyIndex - 1;
	else if(nKeyIndex < 0)
		nKeyIndex = nKeyCount + nKeyIndex;

	int nPageIndex = OFD_Search_GetProperty((OFD_SEARCH)pSearch, nKeyIndex, 3); //ҳ������

	OFD_RECT rect = OFD_Search_GetRect((OFD_SEARCH)pSearch, nKeyIndex, 0);

	int nRectCount = OFD_Search_GetProperty((OFD_SEARCH)pSearch, nKeyIndex, 2); //ArrRects
	for(FX_INT32 j = 1; j < nRectCount; j++)
	{
		OFD_RECT tmpRect = OFD_Search_GetRect((OFD_SEARCH)pSearch, nKeyIndex, j);
		rect.width += tmpRect.width;
		rect.height += tmpRect.height;
	}

	rect.x += rect.width / 2;

	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)hSeal;
	if(OFD_SEAL_OFD == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealImageStream);
	}else if(OFD_SEAL_BITMAP == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealBmp);
	}

	
	if(pOFDDoc == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;

		return -1;
	}

	IOFD_WriteDocument* pWriteDocument = OFD_WriteDoc_Create(pOFDDoc);

	// �ļ��� Base64 ����
	CFX_WideString wsSignFileName(pcwSignFile);
	CFX_Base64Decoder base64Decoder2;
	CFX_ByteString bsSignFileName;
	base64Decoder2.Decode(wsSignFileName, bsSignFileName);

	IFX_FileWrite* pFileWrite = FX_CreateFileWrite(bsSignFileName);

	FS_SignOFD(NULL, hSeal, pWriteDocument, nPageIndex, rect.x, rect.y, pFileWrite, 0);

	if (pFileWrite)
		pFileWrite->Release();
	pFileWrite = NULL;

	delete pDocProvider;
	pDocProvider = NULL;

	pOFDPackage->Release();
	pOFDPackage = NULL;

	pFileRead->Release();
	pFileRead = NULL;

	return 0;
}

int OFD_SEAL_Sign_KT_Key2(IOFD_WriteDocument* pWriteDocument
		, void* hSeal
		, OFD_LPCWSTR pcwKeyWord
		, int nMatchCase
		, int nMatchWord
		, int nKeyIndex
		, int nSealSignType
		, OFD_LPCWSTR pcwSignFile		//B64
)
{
	if(hSeal == NULL || pWriteDocument == NULL || pcwKeyWord == NULL)
		return -1;

	CFX_WideString wsKey(pcwKeyWord);

	COFD_DocProvider* pDocProvider = FX_NEW COFD_DocProvider;
	pDocProvider->Init();

	IOFD_Document* pOFDDoc = pWriteDocument->GetReadDoc();
	FX_INT32 nCounts = pOFDDoc->CountPages();
	CFS_OFDSearch* pSearch = new CFS_OFDSearch;
	for(int i = 0; i < nCounts; i++)
	{
		IOFD_Page* pOFDPage = pOFDDoc->GetPage(i);
		IOFD_TextPageFind* pFind = IOFD_TextPageFind::CreateTextFind(pOFDPage);
		FX_INT32 nFind = pFind->FindFirstEx(wsKey, 0);
		while ((nFind = pFind->FindNextEx()) != -1)
		{
			CFS_OFDSearchResult* pResult = new CFS_OFDSearchResult;

			FX_BOOL bRet = pFind->GetRectArray(pResult->m_ArrRects);
			pSearch->AddResult(pResult);

			pResult->nPageID = pOFDPage->GetID();
			pResult->nPageIndex = pOFDDoc->GetPageIndex(pOFDPage);
			pResult->nDocIndex = 0;
		}
	}

	int nKeyCount = OFD_Search_Count((OFD_SEARCH)pSearch);

	FX_DWORD dwRet;
	//如果nKeyIndex 为0 所有关键词都要加签章。其他情况只会选一个
	if(nKeyIndex > 0) {//
		nKeyIndex = nKeyIndex - 1;
		nKeyCount = nKeyIndex + 1;//保证下面的循环只会执行一次
	}
	else if(nKeyIndex < 0) {
		nKeyIndex = nKeyCount + nKeyIndex;
		nKeyCount = nKeyIndex + 1;
	}

	for(int i = nKeyIndex; i < nKeyCount; i++)
	{
		//int pageID = OFD_Search_GetProperty((OFD_SEARCH)pSearch, i, 3); //页面索引
		nKeyIndex = i;

		int nPageIndex = OFD_Search_GetProperty((OFD_SEARCH)pSearch, nKeyIndex, 3); //页面索引

		OFD_RECT rect = OFD_Search_GetRect((OFD_SEARCH)pSearch, nKeyIndex, 0);

		int nRectCount = OFD_Search_GetProperty((OFD_SEARCH)pSearch, nKeyIndex, 2); //ArrRects
		for(FX_INT32 j = 1; j < nRectCount; j++)
		{
			OFD_RECT tmpRect = OFD_Search_GetRect((OFD_SEARCH)pSearch, nKeyIndex, j);
			rect.width += tmpRect.width;
			rect.height += tmpRect.height;
		}

		rect.x += rect.width / 2;

		FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)hSeal;
		if(OFD_SEAL_OFD == sealInfo->sealImageType)
		{
			pDocProvider->Init(sealInfo->sealImageStream);
		}else if(OFD_SEAL_BITMAP == sealInfo->sealImageType)
		{
			pDocProvider->Init(sealInfo->sealBmp);
		}


		if(pOFDDoc == NULL)
		{
			delete pSearch;
			pSearch = NULL;

			delete pDocProvider;
			pDocProvider = NULL;

			return -1;
		}

		CFX_WideString wsSignFileName(pcwSignFile);

		IFX_FileWrite* pFileWrite = FX_CreateFileWrite(wsSignFileName);

		dwRet = FS_SignOFD(pOFDDoc, hSeal, pWriteDocument, nPageIndex, rect.x, rect.y, pFileWrite, 0);

		if (pFileWrite)
			pFileWrite->Release();
		pFileWrite = NULL;
	}

	delete pSearch;
	pSearch = NULL;

	delete pDocProvider;
	pDocProvider = NULL;

	return dwRet;
}


int OFD_SEAL_Sign_KT_Straddle(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, int nPageSealType
				, int nStartPage
				, int nEndPage
				, int nDuplexPrint
				, float nFirstPagePercent
				, int nPageSealDirection
				, int nPageSealCoordinates
				, int nSealSignType
				, OFD_LPCWSTR pcwSignFile		//B64
				)
{
	if(hSeal == NULL || pcwFileName == NULL)
		return -1;

	// �ļ��� Base64 ����
	CFX_WideString wsFileName(pcwFileName);
	CFX_Base64Decoder base64Decoder;
	CFX_ByteString bsFileName;
	base64Decoder.Decode(wsFileName, bsFileName);

	int ret = -1;
	IFX_FileRead* pFileRead = FX_CreateFileRead(bsFileName);
	if(pFileRead == NULL)
		return -1;

	IFX_MemoryStream* pMemStream = FX_CreateMemoryStream();

	IOFD_FilePackage*	pOFDPackage = OFD_FilePackage_Create(pFileRead);
	if(pOFDPackage == NULL)
	{
		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	COFD_DocProvider* pDocProvider = FX_NEW COFD_DocProvider;
	pDocProvider->Init();

	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, pDocProvider);
	if(pOFDParser == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)hSeal;
	if(OFD_SEAL_OFD == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealImageStream);
	}else if(OFD_SEAL_BITMAP == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealBmp);
	}

	IOFD_Document* pOFDDoc = pOFDParser->GetDocument(0);
	if(pOFDDoc == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;

		return -1;
	}

	IOFD_WriteDocument* pWriteDocument = OFD_WriteDoc_Create(pOFDDoc);

	// �ļ��� Base64 ����
	CFX_WideString wsSignFileName(pcwSignFile);
	CFX_Base64Decoder base64Decoder2;
	CFX_ByteString bsSignFileName;
	base64Decoder2.Decode(wsSignFileName, bsSignFileName);

	IFX_FileWrite* pFileWrite = FX_CreateFileWrite(bsSignFileName);

	FS_SignOFDStraddle(hSeal, pWriteDocument, pFileWrite, nPageSealDirection, nPageSealCoordinates, nFirstPagePercent, nStartPage, nEndPage - nStartPage);

	if (pFileWrite)
		pFileWrite->Release();
	pFileWrite = NULL;

	delete pDocProvider;
	pDocProvider = NULL;

	pOFDPackage->Release();
	pOFDPackage = NULL;

	pFileRead->Release();
	pFileRead = NULL;

	return 0;
}

int OFD_SEAL_Sign_KT_Straddle2(IOFD_WriteDocument* pWriteDocument
		, void* hSeal
		, int nPageSealType
		, int nStartPage
		, int nEndPage
		, int nDuplexPrint
		, float nFirstPagePercent
		, int nPageSealDirection
		, int nPageSealCoordinates
		, int nSealSignType
		, OFD_LPCWSTR pcwSignFile		//B64
)
{
	if(hSeal == NULL || pWriteDocument == NULL)
		return -1;

	COFD_DocProvider* pDocProvider = FX_NEW COFD_DocProvider;
	pDocProvider->Init();

	FS_SEAL_INFO* sealInfo = (FS_SEAL_INFO*)hSeal;
	if(OFD_SEAL_OFD == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealImageStream);
	}else if(OFD_SEAL_BITMAP == sealInfo->sealImageType)
	{
		pDocProvider->Init(sealInfo->sealBmp);
	}

	IOFD_Document* pOFDDoc = pWriteDocument->GetReadDoc();
	if(pOFDDoc == NULL)
	{
		delete pDocProvider;
		pDocProvider = NULL;

		return -1;
	}

	CFX_WideString wsSignFileName(pcwSignFile);

	IFX_FileWrite* pFileWrite = FX_CreateFileWrite(wsSignFileName);

	FX_DWORD dwRet = FS_SignOFDStraddle(hSeal, pWriteDocument, pFileWrite, nPageSealDirection, nPageSealCoordinates, nFirstPagePercent, nStartPage, nEndPage - nStartPage);

	if (pFileWrite)
		pFileWrite->Release();
	pFileWrite = NULL;

	delete pDocProvider;
	pDocProvider = NULL;

	return dwRet;
}

IFX_FileRead* OFD_SEAL_Get_Seal_KT(IOFD_Document* pOFDDoc, OFD_LPCWSTR pcwSealID)
{
	if(pOFDDoc == NULL)
		return NULL;

	COFD_Signatures* pSignes = pOFDDoc->GetSignatures();
	if (!pSignes)
		return NULL;

	wchar_t * pEnd;
	int SealID = wcstol (pcwSealID,&pEnd,10);
	int nSignCount = pSignes->CountSignature();
	if(SealID > nSignCount || SealID <= 0 ) {
		LOGD("SealID must bigger than 0 and smaller than Sign number of doc Sealid:$d",SealID);
		return NULL;
	}

	COFD_Signature* pSign = pSignes->GetSignature(SealID - 1);

	return pSign->GetSealFile();
}


string Base64Encoder(const char * chrawdata, size_t idatasize)
{
	string strSealProperty_Base64;
	CFX_Base64Encoder base64Encoder;
	char * base64SealData = NULL;
	int size = base64Encoder.Encode((unsigned char*)chrawdata, idatasize, base64SealData);
	LOGD("size of base64 str:%d", size);
	base64SealData = new char[size + 1];
	memset(base64SealData, 0,  size + 1);
	base64Encoder.Encode((unsigned char*)chrawdata, idatasize, base64SealData);
	string ret(base64SealData,size);
	delete[] base64SealData;
	return ret;
}

/*
struct sealinfo OES_GetInfo_FromSign(unsigned char* puchSignData,int iSignDataLen)
{
    struct sealinfo info;
    SES_Signature* pSign = NULL;

    LOGD("%d,%d", puchSignData, iSignDataLen);
    asn_dec_rval_t err = ber_decode(NULL, &asn_DEF_SES_Signature, (void**)&pSign, puchSignData, iSignDataLen);
    if (err.code != RC_OK){
        LOGD("签章解析失败:%d", err.code);
        //return info;
    }
    LOGD("pSign:%d", pSign);
    info.SealCertBase64.assign((char*)pSign->toSign.esealInfo.signInfo.cert.buf, pSign->toSign.esealInfo.signInfo.cert.size);
    info.SignCertBase64.assign((char*)pSign->toSign.cert.buf, pSign->toSign.cert.size);
    for(int i = 0; i < pSign->toSign.esealInfo.esealInfo.property.certList.list.count; i++ ){
        info.SignCertList.push_back(string((char*)pSign->toSign.esealInfo.esealInfo.property.certList.list.array[i]->buf,
                                           pSign->toSign.esealInfo.esealInfo.property.certList.list.array[i]->size));
    }
    info.SealID.assign((char*)pSign->toSign.esealInfo.esealInfo.header.id.buf,
                       pSign->toSign.esealInfo.esealInfo.header.id.size);
    info.SealMakeTime.assign((char*)pSign->toSign.esealInfo.esealInfo.property.createDate.buf,
                             pSign->toSign.esealInfo.esealInfo.property.createDate.size);
    info.SealName.assign((char*)pSign->toSign.esealInfo.esealInfo.property.name.buf,
                         pSign->toSign.esealInfo.esealInfo.property.name.size);
    info.SealSignMethod.assign((char*)pSign->toSign.esealInfo.signInfo.signatureAlgorighm.buf,
                               pSign->toSign.esealInfo.signInfo.signatureAlgorighm.size);
    info.SealType = pSign->toSign.esealInfo.esealInfo.property.type;
    info.SealValidEnd.assign((char*)pSign->toSign.esealInfo.esealInfo.property.validEnd.buf,
                             pSign->toSign.esealInfo.esealInfo.property.validEnd.size);
    info.SealValidStart.assign((char*)pSign->toSign.esealInfo.esealInfo.property.validStart.buf,
                               pSign->toSign.esealInfo.esealInfo.property.validStart.size);
    info.SealVersion = pSign->toSign.esealInfo.esealInfo.header.version;
    info.SignTime.assign((char*)pSign->toSign.timeInfo.buf,
                         pSign->toSign.timeInfo.size);
    //pSign->
    info.SignVersion = pSign->toSign.version;
    info.SignatureAlg.assign((char*)pSign->toSign.signatureAlgorithm.buf,
                             pSign->toSign.signatureAlgorithm.size);

    info.VenderId.assign((char*)pSign->toSign.esealInfo.esealInfo.header.vid.buf,
                         pSign->toSign.esealInfo.esealInfo.header.vid.size);
    LOGD("获取SealInfo成功");
    return info;
    LOGD("获取SealInfo失败");
    return info;
}
*/

string OFD_SEAL_Verify_KT(IOFD_Document* pOFDDoc
		, OFD_LPCWSTR pcwSealID
		, bool bverify
		, int nOnLine
)
{
	if(pOFDDoc == NULL)
		return string();

	CFX_WideString wsSealID_(pcwSealID);

	COFD_Signatures* pSignes = pOFDDoc->GetSignatures();
	if (!pSignes)
		return string();

	int nSignCount = pSignes->CountSignature();

    wchar_t * pEnd;
	int SealID =  wcstol (wsSealID_.GetBuffer(wsSealID_.GetLength()),&pEnd,10);
    LOGD("SealID %d", SealID);
	string ret;
	bool verified_before = false;
	for (int i = 0; i < nSignCount; i ++)
	{
		stringstream strSealProperty;
		COFD_Signature* pSign = pSignes->GetSignature(i);
		CFX_WideString wsSealID = pSign->GetSealID();
		CFX_WideString wsType = pSign->GetType();
        CFX_ByteString pSealFile = pSign->GetSignatureValue();

        LOGD("type:%s,seal in signed:%s, seal to verify:%s", wcharTochar(wsType.GetBuffer(20)), wcharTochar(wsSealID.GetBuffer(20)), wcharTochar(wsSealID_.GetBuffer(20)));
		if (wsType.Equal(FX_WSTRC(L"Seal")) && (SealID == 0 || (SealID - 1) == i ) )//if SealID == 0 verify all.
		{
			if( !bverify || (FS_VerifyOFD(pSign, i, nOnLine) == 0) )
			{
				LOGD("verify success");
				CFX_ByteString pSealFile = pSign->GetSignatureValue();
				FX_DWORD dwSealDataLen = 0;
				FX_LPBYTE pSealData = NULL;
				LOGD("pSIGNFile:%d", pSealFile.GetLength());
				if(pSealFile.GetLength()){
					dwSealDataLen = pSealFile.GetLength();
					pSealData = (FX_LPBYTE)pSealFile.GetBuffer(dwSealDataLen);
					LOGD("SealDataLen:%d", dwSealDataLen);
				}
//				struct sealinfo info = OES_GetInfo_FromSign(pSealData, dwSealDataLen);
//				if(info.VenderId != ""){
//					strSealProperty << "SealCertBase64:" << Base64Encoder(info.SealCertBase64.c_str(), info.SealCertBase64.size()) << "\n";
//					strSealProperty << "SignCertBase64:" << Base64Encoder(info.SignCertBase64.c_str(), info.SignCertBase64.size()) << "\n";
//					strSealProperty << "SealVersion:" << info.SealVersion << "\n";
//					strSealProperty << "SealType:" << info.SealType << "\n";
//					strSealProperty << "SealMakeTime:" << info.SealMakeTime << "\n";
//					strSealProperty << "SealName:" << info.SealName << "\n";
//					strSealProperty << "SealID:" << info.SealID << "\n";
//					for(int j = 0; j < info.SignCertList.size(); j++) {
//						strSealProperty << "SignCertList-" << j << ":" << Base64Encoder(info.SignCertList[j].c_str(), info.SignCertList[j].size()) << "\n";
//					}
//					strSealProperty << "SignTime:" << info.SignTime << "\n";
//					strSealProperty << "SealSignMethod:" << info.SealSignMethod << "\n";
//					strSealProperty << "SealValidEnd:" << info.SealValidEnd << "\n";
//					strSealProperty << "SealValidStart:" << info.SealValidStart << "\n";
//					strSealProperty << "SignatureAlg:" << info.SignatureAlg << "\n";
//					strSealProperty << "SignType:" << info.SignType << "\n";
//					strSealProperty << "SignVersion:" << info.SignVersion << "\n";
//					strSealProperty << "VenderId:" << info.VenderId << "\n";
//				}

//				LOGD("str:%s", strSealProperty.str().c_str());
				string base64SealData = Base64Encoder((char*)pSealData, dwSealDataLen);
				if(verified_before)
					ret += ",";

				ret += "{\"b64SealSign\":\"" + string(base64SealData) + "\"}";
				verified_before = true;
			}
			if(SealID)
				break;
		}
	}
	if(ret.size() !=0)
		ret = "[" + ret + "]";
	return ret;
}

#endif
