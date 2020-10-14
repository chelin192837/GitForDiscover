#ifndef _OFD_ES_H_
#define _OFD_ES_H_

#include "../include/ofd_base_r.h"

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
void* OFD_SEAL_Load(OFD_LPCSTR lpszSeal, int sealType/*0表示lpszSeal为印章ID，1表示lpszSeal为印章名称*/, int bCheckSeallist/*0表示不Get SealList，1表示Get SealList*/, OFD_LPCSTR lpszPin, int* err);
void OFD_SEAL_UnLoad(void* hSeal);
int OFD_SEAL_Sign(void* hSeal, OFD_LPCWSTR lpwszFileName, OFD_LPCWSTR lpwszSignFile, int page, float x, float y, OFD_BOOL bLocked = FALSE);
int OFD_SEAL_Sign(void* hSeal, IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite, int page, float x, float y, OFD_BOOL bLocked = FALSE);
int OFD_SEAL_Sign(void* hSeal, OFD_PACKAGE hPackage, IFX_FileWrite* pFileWrite, int page, float x, float y, OFD_BOOL bLocked = FALSE);
int OFD_SEAL_SignStraddle(void* hSeal, OFD_LPCWSTR lpwszFileName, OFD_LPCWSTR lpwszSignFile, int nPart, int nProportion, float fFirstPagePercent, int nFirstPageIndex, int nCountPage, OFD_BOOL bLocked = FALSE);
int OFD_SEAL_SignStraddle(void* hSeal, IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite, int nPart, int nProportion, float fFirstPagePercent, int nFirstPageIndex, int nCountPage, OFD_BOOL bLocked = FALSE);

////////////// fa piao ///////////////////////
int OFD_SEAL_SignFP(void* hSeal, OFD_LPCWSTR lpwszFileName, OFD_LPCWSTR lpwszSignFile, int page, float x, float y, float posx, float posy, OFD_BOOL bLocked = FALSE);
int OFD_SEAL_SignFP(void* hSeal, IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite, int page, float x, float y, float posx, float posy, OFD_BOOL bLocked = FALSE);
//////////////////////////////////////////////

int OFD_SEAL_GetSigCount(OFD_LPCWSTR lpwszFileName);
int OFD_SEAL_GetSigCount(IFX_FileRead* pFileRead);
int OFD_SEAL_Verify(OFD_LPCWSTR lpwszFileName, int index, int nOnLine);
int OFD_SEAL_Verify(IFX_FileRead* pFileRead, int index, int nOnLine);


enum SEAL_SIGN_PROPERTY
{
	type,
	ProviderName,
	Company,
	Version,
	SignDateTime,
	SignMethod,
	CheckMethod
};
//获取 seal 签章的属性值
int OFD_SEAL_GetSigProperty(OFD_LPCWSTR lpwszFileName, int index, SEAL_SIGN_PROPERTY property, OFD_WSTR* data);
int OFD_SEAL_GetSigProperty(IFX_FileRead* pFileRead, int index, SEAL_SIGN_PROPERTY property, OFD_WSTR* data);

#define FOREACH_PROPERTY(CMD) \
	CMD(Seal_ID) \
	CMD(Seal_Version) \
	CMD(Seal_VenderId) \
	CMD(Seal_Type) \
	CMD(Seal_Name) \
	CMD(Seal_CertInfo) \
	CMD(Seal_ValidStart) \
	CMD(Seal_ValidEnd) \
	CMD(Seal_SignedDate) \
	CMD(Seal_SignerName) \
	CMD(Seal_SignMethod)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum SEAL_PROPERTY {
	FOREACH_PROPERTY(GENERATE_ENUM)
	};


static const char *PROPERTY_STRING[] = {
		FOREACH_PROPERTY(GENERATE_STRING)
};

//获取 seal 印章的属性值
void* OFD_SEAL_GetPropertyInit(OFD_LPCWSTR lpwszFileName, int index);
void* OFD_SEAL_GetPropertyInit(IFX_FileRead* pFileRead, int index);
int OFD_SEAL_GetProperty(void* handler, SEAL_PROPERTY property, OFD_WSTR* data);
void OFD_SEAL_GetPropertyRelease(void* handler);

void* OFD_Seal_Begin(IFX_FileRead* pFileRead);
int OFD_Seal_GetPageCount(void* handler);
int OFD_Seal_SearchKeyWord(void* handler, OFD_LPCWSTR lpszKeyWord);
OFD_BOOL OFD_Seal_IncludeKeyWord(void* handler, int nPageIndex);
float OFD_Seal_GetFirstKeyWordPos(void* handler, int type);
void OFD_Seal_End(void* handler);
// Sign
int OFD_CERT_SignInit();
void OFD_CERT_SignRelease();
int OFD_Document_Sign(IFX_FileRead* pFileRead, OFD_LPCSTR lpwszOFDPwd, IFX_FileWrite* pFileWrite, OFD_LPCWSTR lpwszCertFile, OFD_LPCWSTR lpwszCertPwd);
int OFD_Document_GetSigCount(IFX_FileRead* pFileRead, OFD_LPCSTR lpwszOFDPwd);
int OFD_Document_Verify(IFX_FileRead* pFileRead, int index, OFD_LPCSTR lpwszOFDPwd);
//获取 cert 签章的属性值
int OFD_CERT_GetSigProperty(OFD_LPCWSTR lpwszFileName, int index, SEAL_SIGN_PROPERTY property, OFD_WSTR* data);
int OFD_CERT_GetSigProperty(IFX_FileRead* pFileRead, int index, SEAL_SIGN_PROPERTY property, OFD_WSTR* data);
int OFD_Cert_GetCertProperty(IFX_FileRead* pFileRead, int index, OFD_LPCSTR lpwszOFDPwd, int propertyindex, OFD_WSTR* data);

// PDF
int PDF_Document_Sign(void* hSeal, OFD_LPCWSTR lpwszFileName, OFD_LPCWSTR lpwszSignFile, int pageIndex, float x, float y);
int PDF_Document_Sign(void* hSeal, IFX_FileRead* pFileRead, IFX_MemoryStream* pFileWrite, int page, float x, float y);
int PDF_Document_GetSigCount(OFD_LPCWSTR lpwszFileName);
int PDF_Document_GetSigCount(IFX_FileRead* pFileRead);
int PDF_Document_Verify(OFD_LPCWSTR lpwszFileName, int index, int nOnLine);
int PDF_Document_Verify(IFX_FileRead* pFileRead, int index, int nOnLine);

// Sign2
int OFD_SIGITAL_Init(OFD_LPCWSTR lpszODS);
int OFD_SIGITAL_Init(OFD_LPCWSTR lpszODS, void* hJNIEnv, void* hClassObj);
void OFD_SIGITAL_Destory();
int OFD_SIGITAL_Sign(OFD_LPCSTR lpszSignID, IFX_FileRead* pFileRead, OFD_LPCSTR lpwszOFDPwd, IFX_FileWrite* pFileWrite, 
	IFX_FileRead* pImage, int image_type, int page, float x, float y, float w, float h, int rotate);
int OFD_SIGITAL_GetSigCount(IFX_FileRead* pFileRead, OFD_LPCSTR lpwszOFDPwd);
int OFD_SIGITAL_Verify(IFX_FileRead* pFileRead, int index, OFD_LPCSTR lpwszOFDPwd);

// Seal Merge
void* OFD_Merge_Begin(OFD_LPCWSTR lpwszFileName);
void OFD_Merge_Add(void* mergeHandler, OFD_LPCWSTR signDataFile);
int OFD_Merge_End(void* mergeHandler, OFD_LPCWSTR lpwszFileName);

void* OFD_Merge_Begin2(IFX_FileRead* pFileRead);
void OFD_Merge_Add2(void* mergeHandler, IFX_FileRead* signDataFile);
int OFD_Merge_End2(void* mergeHandler, IFX_FileWrite* pFileWrite);

// Seal Remove
void OFD_Seal_RemoveAll(IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite);

//
void* OFD_Sign_Begin(IFX_FileRead* pFileRead);
int OFD_Sign_GetPageCount(void* handler);
void OFD_Sign_SetProviderInfo(void* handler, CFX_WideString& providerName, CFX_WideString& company, CFX_WideString& version, CFX_WideString& digestMethod, CFX_WideString& signMethod);
void OFD_Sign_SetDateTime(void* handler, CFX_WideString& signTime);
void OFD_Sign_SetSealFile(void* handler, IFX_MemoryStream* pSealFile);

int OFD_Sign_SearchKeyWord(void* handler, OFD_LPCWSTR lpszKeyWord);
OFD_BOOL OFD_Sign_IncludeKeyWord(void* handler, int nPageIndex);
float OFD_Sign_GetFirstKeyWordPos(void* handler, int type);

int OFD_Sign_AddSealImage(void* handler, int page, float x, float y, float w, float h);
int OFD_Sign_AddSealImageByKeyWord(void* handler, OFD_LPCWSTR lpszKeyWord, float width, float height);
int OFD_Sign_AddSealImageByKeyWord2(void* handler, int nPageIndex, float width, float height);

int OFD_Sign_AddSignImage(void* handler, IFX_FileRead* pPic, int iImgType, int page, float x, float y, float w, float h, int rotate);
int OFD_Sign_AddSignImageByKeyWord(void* handler, OFD_LPCWSTR lpszKeyWord, IFX_FileRead* pPic, int iImgType, float w, float h, int rotate);
int OFD_Sign_AddSignImageByKeyWord2(void* handler, int nPageIndex, IFX_FileRead* pPic, int iImgType, float w, float h, int rotate);

int OFD_Sign_CountRefers(void* handler);
IFX_MemoryStream* OFD_Sign_GetReferData(void* handler, int ref_index);
void OFD_Sign_SetReferDigest(void* handler, int ref_index, FX_LPBYTE buf, int buf_len);
IFX_MemoryStream* OFD_Sign_GetSignData(void* handler);
void OFD_Sign_SetSignValue(void* handler, FX_LPBYTE buf, int buf_len);
IFX_MemoryStream* OFD_Sign_GetSignFile(void* handler);
void OFD_Sign_End(void* handler);
void* OFD_SignVerify_Begin(IFX_FileRead* pFileRead);
int OFD_SignVerify_Count(void* handler);
void OFD_SignVerify_Load(void* handler, int sign_index);
CFX_WideString OFD_SignVerify_GetProviderInfo(void* handler);
CFX_WideString OFD_SignVerify_GetDateTime(void* handler);
int OFD_SignVerify_CountRefers(void* handler);
IFX_MemoryStream* OFD_SignVerify_GetReferData(void* handler, int ref_index);
IFX_MemoryStream* OFD_SignVerify_GetReferDigest(void* handler, int ref_index);
IFX_MemoryStream* OFD_SignVerify_GetSignData(void* handler);
IFX_MemoryStream* OFD_SignVerify_GetSignValue(void* handler);
void OFD_SignVerify_End(void* handler);

#endif //_OFD_ES_H_
