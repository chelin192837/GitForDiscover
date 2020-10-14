#ifndef _OFD_H_
#define _OFD_H_

#include "../include/ofd_base_r.h"
#include <vector>
#include <string>

// External classes
class IFX_FileRead;
class IFX_FileWrite;
class IFX_MemoryStream;

// Internal interface
#define OFD_ERRCODE_NoFoundDLL		501
#define OFD_ERRCODE_LoadDLL			502
#define OFD_ERRCODE_NoSymbol		503
#define OFD_ERRCODE_NoSuchSeal		504
#define OFD_ERRCODE_LoadSealData	505
#define OFD_ERRCODE_NoLoadDLL		506

// ESSDK
// Seal
int OFD_SEAL_Init(OFD_LPCSTR lpszOES, OFD_LPCSTR lpszPin);
void OFD_SEAL_Destory();
void* OFD_SEAL_Load(OFD_LPCSTR lpszSeal, int sealType/*0��ʾlpszSealΪӡ��ID��1��ʾlpszSealΪӡ������*/, int bCheckSeallist/*0��ʾ��Get SealList��1��ʾGet SealList*/, OFD_LPCSTR lpszPin, int* err);
void OFD_SEAL_UnLoad(void* hSeal);

struct sealinfo{
	std::string SealCertBase64;
	std::string SignCertBase64;
	std::vector<std::string> SignCertList;
	std::string SealID;
	std::string SealMakeTime;
	std::string SealName;
	std::string SealSignMethod;
	int SealType;
	std::string SealValidEnd;
	std::string SealValidStart;
	int SealVersion;
	std::string SignTime;
	std::string SignType;
	int SignVersion;
	std::string SignatureAlg;
	std::string VenderId;
};

//��һ�������x��y����
int OFD_SEAL_Sign_KT_Pos(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, float fXPos
				, float fYPos
				, int* pPageArr
				, int nSize
				, int nSubRange
				, OFD_LPCWSTR pcwSignFile		//B64
				);

int OFD_SEAL_Sign_KT_Pos2(IOFD_WriteDocument* pWriteDocument
        , void* hSeal
        , float fXPos
        , float fYPos
        , int* pPageArr					// NULL, 所有页面
        , int nSize
        , int nSubRange					// -1 不起作用，1 奇数页	2 偶数页
        , OFD_LPCWSTR pcwSignFile
);

//�ڶ���������ؼ��֣�������£�
int OFD_SEAL_Sign_KT_Key(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, OFD_LPCWSTR pcwKeyWord
				, int nMatchCase
				, int nMatchWord
				, int nKeyIndex
				, int nSealSignType
				, OFD_LPCWSTR pcwSignFile		//B64
				);

int OFD_SEAL_Sign_KT_Key2(IOFD_WriteDocument* pWriteDocument
        , void* hSeal
        , OFD_LPCWSTR pcwKeyWord
        , int nMatchCase
        , int nMatchWord
        , int nKeyIndex
        , int nSealSignType
        , OFD_LPCWSTR pcwSignFile
);

//����������������
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
				);

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
        , OFD_LPCWSTR pcwSignFile
);

std::string OFD_SEAL_Verify_KT(IOFD_Document* pOFDDoc
		, OFD_LPCWSTR pcwSealID
		, bool bverify
		, int nOnLine
);

IFX_FileRead* OFD_SEAL_Get_Seal_KT(IOFD_Document* pOFDDoc, OFD_LPCWSTR pcwSealID);
#endif
