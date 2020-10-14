#include "fxlib.h"
#include "seal.h"
#include "include/fs_ofdsignature.h"
#include "../include/ofd_search_r.h"
#include "../../../../../../rdkcommon/rdk/src/fxcore.h"
#include "include/fs_ofdsignaturefun.h"
// test
int OFD_SEAL_Sign_KT(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, OFD_LPCWSTR pcwOESName
				, OFD_LPCSTR pPassWD
				, float fXPos
				, float fYPos
				, OFD_LPCSTR pRange
				, OFD_LPCSTR pSubRange
				, OFD_LPCWSTR pcwKeyWord
				, int nMatchCase
				, int nMatchWord
				, int nKeyIndex
				, int nSealSignType
				, int nPageSealType
				, int nStartPage
				, int nEndPage
				, int nDoublePrint
				, int nFirstPercent
				, int nPageSealDirection
				, int nPageSealCoordinates
				, OFD_LPCWSTR pcwSignFile		//B64
				)
{
	if(hSeal == NULL || pcwFileName == NULL || pcwOESName == NULL)
		return -1;

	int ret = -1;
	IFX_FileRead* pFileRead = FX_CreateFileRead((FX_LPCWSTR)pcwFileName);
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

	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, NULL);
	if(pOFDParser == NULL)
	{
		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;
		return -1;
	}

	IOFD_Document* pOFDDoc = pOFDParser->GetDocument(0);
	if(pOFDDoc == NULL)
	{
		pOFDPackage->Release();
		pOFDPackage = NULL;

		pFileRead->Release();
		pFileRead = NULL;

		return -1;
	}

	pOFDPackage->Release();
	pOFDPackage = NULL;

	pFileRead->Release();
	pFileRead = NULL;

	return 0;
}

int OFD_SEAL_Sign_KT_Pos(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, float fXPos
				, float fYPos
				, OFD_LPCSTR pRange
				, OFD_LPCSTR pSubRange
				, OFD_LPCWSTR pcwSignFile		//B64
				)
{
	if(hSeal == NULL || pcwFileName == NULL || pRange == NULL)
		return -1;

	CFX_ByteString bsRange(pRange);
	CFX_ByteString bsSubRange;

	CFX_Int32Array	pageArray;

	if(pSubRange != NULL)
		bsSubRange = pSubRange;

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

	FS_SignOFD(NULL, hSeal, pWriteDocument, 0, fXPos, fYPos, pFileWrite, 0);

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
#include "include/fs_ofdsearch.h"
int OFD_SEAL_Sign_KT_Key(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, OFD_LPCSTR pRange
				, OFD_LPCSTR pSubRange
				, OFD_LPCWSTR pcwKeyWord
				, int nMatchCase
				, int nMatchWord
				, int nKeyIndex
				, int nSealSignType
				, OFD_LPCWSTR pcwSignFile		//B64
				)
{
	if(hSeal == NULL || pcwFileName == NULL || pRange == NULL || pcwKeyWord == NULL)
		return -1;

	CFX_ByteString bsRange(pRange);
	CFX_ByteString bsSubRange;

	CFX_Int32Array	pageArray;

	if(pSubRange != NULL)
		bsSubRange = pSubRange;

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
		int pageID = OFD_Search_GetProperty((OFD_SEARCH)pSearch, i, 3); //ҳ������
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

	FS_SignOFD(NULL, hSeal, pWriteDocument, 0, rect.x, rect.y, pFileWrite, 0);

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

int OFD_SEAL_Sign_KT_Straddle(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, int nPageSealType
				, int nStartPage
				, int nEndPage
				, int nDuplexPrint
				, int nFirstPagePercent
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

	FS_SignOFDStraddle(hSeal, pWriteDocument, pFileWrite, nPageSealDirection, 1, nFirstPagePercent, nStartPage, nEndPage - nStartPage);

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
