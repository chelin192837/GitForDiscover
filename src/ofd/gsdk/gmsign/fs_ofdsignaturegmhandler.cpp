#include "../fxlib.h"
#include "../include/fs_ofdsdkmgr.h"
#include "../include/fs_ofdsignature.h"
#include "../include/fs_ofdsignaturefun.h"
#include "../include/fs_ofdsignaturemanage.h"
#include "../include/fs_ofdsignaturefactory.h"
#include "fs_ofdsignaturegmsslfunc.h"
#include "fs_ofdsignaturegmhandler.h"

#include <time.h>

#include <iostream>
#include <memory>
using namespace std;

#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN32_MOBILE_ || _FX_OS_ == _FX_WIN64_)
	#if _FX_OS_ != _FX_WIN32_MOBILE_
		#include <winsock.h>
	#else
		#include <winsock2.h>
	#endif
	#ifdef ParseURL
		#undef ParseURL
	#endif
#else //linux
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <netdb.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
#endif

#if _FX_PRODUCT_ == _FS_ESSDK_ || _FX_PRODUCT_ == _FS_EOSDK_ || _FX_PRODUCT_ == _FS_NONE_ || _FX_PRODUCT_ == _FS_EISDK_ || _FX_PRODUCT_ == _FS_ASDK_/*20190813*/ || _FX_PRODUCT_ == _FS_GSDK_

#define CHECKED_STACK_OF(type, p) \
	((_STACK*) (1 ? p : (STACK_OF(type)*)0))

struct deleterPKCS7 {
	inline void operator()(PKCS7* p7) const { if (p7) PKCS7_free(p7); }
};

struct deleterBIO {
	inline void operator()(BIO *pbio) const { if (pbio) BIO_free(pbio); }
};

GmsslFunctions *g_GmsslFunctions;

FX_BOOL LoadGmssl()
{
	if (NULL == g_GmsslFunctions)
		g_GmsslFunctions = FX_NEW GmsslFunctions();

	if (g_GmsslFunctions && !g_GmsslFunctions->IsValid())
		return g_GmsslFunctions->LoadAllFunctions();

	return TRUE;
}

void UnLoadGmssl()
{
	if (g_GmsslFunctions)
		delete g_GmsslFunctions;
	g_GmsslFunctions = NULL;
}

void initialize_openssl()
{	
	SSLeay_add_all_algorithms();
	ERR_load_ERR_strings();
	ERR_load_crypto_strings();
}

static void HexCryptBuffer(const FX_BYTE* pOriginBuf, FX_DWORD nOriginLen, FX_BYTE* pCryptedBuf)
{
	FX_BYTE* pCB = pCryptedBuf;
	for (FX_DWORD i = 0; i<nOriginLen; i++)
	{
		FX_INT32 b = (*pOriginBuf & 0xF0) >> 4;
		*pCB++ = (b <= 9) ? b + '0' : (b - 10) + 'A';
		b = *pOriginBuf & 0x0F;
		*pCB++ = (b <= 9) ? b + '0' : (b - 10) + 'A';
		pOriginBuf++;
	}
}

static CFS_SignatureHandlerAdbePkcs7DetachedGmssl* g_pGmPkcs7 = NULL;

FX_BOOL	CFS_SignatureHandlerAdbePkcs7DetachedGmssl::Create()
{
	if (g_pGmPkcs7)
		return TRUE;

	g_pGmPkcs7 = FX_NEW CFS_SignatureHandlerAdbePkcs7DetachedGmssl;

	return TRUE;
}

void CFS_SignatureHandlerAdbePkcs7DetachedGmssl::Destory()
{
	if (g_pGmPkcs7)
		delete g_pGmPkcs7;
	g_pGmPkcs7 = NULL;
}

CFS_SignatureHandlerAdbePkcs7DetachedGmssl*	CFS_SignatureHandlerAdbePkcs7DetachedGmssl::Get()
{
	return g_pGmPkcs7;
}

FX_BOOL CFS_SignatureHandlerAdbePkcs7DetachedGmssl::IsValid()
{
	if(g_GmsslFunctions == NULL)
		return FALSE;

	return g_GmsslFunctions->IsValid();
}

CFS_SignatureHandlerAdbePkcs7DetachedGmssl::CFS_SignatureHandlerAdbePkcs7DetachedGmssl()
	:m_pSig(NULL), m_pFile(NULL), m_pkey(NULL), m_x509(NULL), m_ca(NULL) 
{
	//initialize_openssl();
	LoadGmssl();

	m_bParsePfxFile = FALSE;
}

CFS_SignatureHandlerAdbePkcs7DetachedGmssl::~CFS_SignatureHandlerAdbePkcs7DetachedGmssl()
{
	if(m_pkey)
		EVP_PKEY_free(m_pkey);
	m_pkey = NULL;

	if(m_x509)
		X509_free(m_x509);
	m_x509 = NULL;

	if (m_ca)
		sk_X509_free(m_ca);
	m_ca = NULL;

	UnLoadGmssl();
}

static FX_BOOL GetTextFromFile(IFX_FileRead* pFile, const CFX_DWordArray& byteRangeArray, CFX_ByteString& bsDigest)
{
	if (!pFile) return false;

	FX_DWORD offset0 = byteRangeArray.GetAt(0);
	FX_DWORD dwBlock0 = byteRangeArray.GetAt(1);

	FX_DWORD offset1 = byteRangeArray.GetAt(2);
	FX_DWORD dwBlock1 = byteRangeArray.GetAt(3);

	FX_LPBYTE pBuffer = FX_Alloc(FX_BYTE, dwBlock0 + dwBlock1);
	if (!pBuffer) return false;
	FXSYS_memset32(pBuffer, 0, dwBlock0 + dwBlock1);

	FX_BOOL bRet = pFile->ReadBlock((void*)pBuffer, offset0, dwBlock0);
	if (!bRet) {
		FX_Free(pBuffer);
		return false;
	}
	bRet = pFile->ReadBlock((void*)(pBuffer + dwBlock0), offset1, dwBlock1);
	if (!bRet) {
		FX_Free(pBuffer);
		return false;
	}

	bsDigest = CFX_ByteString(pBuffer, dwBlock0 + dwBlock1);
	FX_Free(pBuffer);

	return true;
}

static FX_INT32 GetCertInfo(X509* pX509Cert)
{
	if (!pX509Cert)
		return -1;

	//usage
	ASN1_BIT_STRING* lASN1UsageStr = (ASN1_BIT_STRING*)X509_get_ext_d2i(pX509Cert, NID_key_usage, NULL, NULL);
	if (!lASN1UsageStr)
		return -1;

	unsigned short usage = lASN1UsageStr->data[0];
	if(lASN1UsageStr->length > 1)
	{
		usage |= lASN1UsageStr->data[1] << 8;
	}
		
	if ((usage & KU_DIGITAL_SIGNATURE) == 0 && (usage & KU_KEY_CERT_SIGN) == 0 && (usage & KU_CRL_SIGN) == 0)
	{
		return -1;
	}

	//time
	ASN1_TIME* pNotBefore = X509_get_notBefore(pX509Cert);
	ASN1_TIME* pNotAfter = X509_get_notAfter(pX509Cert);

	ASN1_UTCTIME* pNotBeforeUTC = ASN1_STRING_dup(pNotBefore);
	ASN1_UTCTIME* pNotAfterUTC = ASN1_STRING_dup(pNotAfter);

	time_t currentTime;
	time(&currentTime);
	
	if(ASN1_UTCTIME_cmp_time_t(pNotBeforeUTC, currentTime) >= 0 || ASN1_UTCTIME_cmp_time_t(pNotAfterUTC, currentTime) <= 0)
	{
		ASN1_STRING_free(pNotBeforeUTC);
		ASN1_STRING_free(pNotAfterUTC);
		return -2;
	}

	ASN1_STRING_free(pNotBeforeUTC);
	ASN1_STRING_free(pNotAfterUTC);

	return 0;
}

//CPDF_Signature
static FX_INT32 ParseP12File_pdf(const /*CPDF_Signature*/void* pSig, EVP_PKEY** pKey, X509** pSignCert, STACK_OF(X509)** pCerts)
{
	if (!pSig) return -1;

	CFX_WideString wsCert;
	CFX_WideString wsPwd;
	//pSig->GetSignerCertPath(wsCert, wsPwd);
	FILE* fp = fopen((FX_LPCSTR)CFX_ByteString::FromUnicode(wsCert), "rb");
	if (!fp)
		return -11;

	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();

	PKCS12* p12 = d2i_PKCS12_fp(fp, NULL);
	fclose (fp);
	if (!p12)
		return -12;

	if (!PKCS12_parse(p12, (FX_LPCSTR)wsPwd.UTF8Encode(), pKey, pSignCert, pCerts))
		return -13;

	PKCS12_free(p12);
	if (!pKey)
		return -1;

	return 0;
}

static FX_INT32 ParseP12File_Ofd(CFX_WideString wsCertFile, CFX_WideString wsPwd, EVP_PKEY** pKey, X509** pSignCert, STACK_OF(X509)** pCerts)
{
	if(wsCertFile.IsEmpty())
		return -1;

	FX_BOOL bRet = FS_IsFileExist(wsCertFile);
	if(!bRet)	return -1;

	//WriteLog_GSDK("ParseP12File_Ofd wsCertFile : ", CFX_ByteString::FromUnicode(wsCertFile));
	//WriteLog_GSDK("ParseP12File_Ofd wsPwd : ", CFX_ByteString::FromUnicode(wsPwd));
	FILE* fp = fopen((FX_LPCSTR)CFX_ByteString::FromUnicode(wsCertFile), "rb");
	if (!fp)
		return -11;

	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();

	PKCS12* p12 = d2i_PKCS12_fp(fp, NULL);
	//WriteLog_GSDK("ParseP12File_Ofd ", "p12 : ", (long)p12);
	fclose (fp);
	if (!p12)
		return -12;

	if (!PKCS12_parse(p12, (FX_LPCSTR)wsPwd.UTF8Encode(), pKey, pSignCert, pCerts))
		return -13;

	PKCS12_free(p12);
	if (!pKey)
		return -1;

	return 0;
}

int GetSignAlgoritham(X509 *x509)
{
	char oid[128] =	{ 0 };
	ASN1_OBJECT* salg = NULL;
	salg = x509->sig_alg->algorithm;
	OBJ_obj2txt(oid, 128, salg, 1);
	return OBJ_txt2nid(oid);
}

//CPDF_Signature
void InsertSMIDtoDict(void* pSig)
{
/*
	FX_LPBYTE pSMID = pSig->GetSMID();
	if(pSMID)
	{
		pSig->SetKeyValue(FX_BSTRC("SMID"), CFX_ByteString(pSMID, 16));
	}
*/
}

FX_BOOL IsSm2sm3Sign(X509* x509)
{
	return GetSignAlgoritham(x509) == NID_sm2sign_with_sm3;
}

//CPDF_Signature
FX_INT32 CFS_SignatureHandlerAdbePkcs7DetachedGmssl::GetCertificateInfo(void* pSig, FX_LPVOID pClientData/* = NULL*/)
{
	if (!pSig) return -1;
	ASSERT(g_GmsslFunctions->IsValid());

	FX_INT32 nRet = ParseP12File_pdf(pSig, &m_pkey, &m_x509, &m_ca);
	//if (nRet != CPDF_SIGNATURE_CERT_VALID)
	if (nRet != 0)
		return nRet;

	int ret = GetCertInfo(m_x509);
	//if(ret == CPDF_SIGNATURE_CERT_VALID && IsSm2sm3Sign(m_x509))
	if(ret == 0 && IsSm2sm3Sign(m_x509))
	{
		InsertSMIDtoDict(pSig);
	}
	return ret;
}

FX_BOOL	CFS_SignatureHandlerAdbePkcs7DetachedGmssl::IsSM2SM3PFXCert(CFX_WideString wsCertFile, CFX_WideString wsPwd)
{
	WriteLog_GSDK("IsSM2SM3PFXCert ", "m_bParsePfxFile : ", m_bParsePfxFile);
	if(m_bParsePfxFile)
		return FALSE;

	WriteLog_GSDK("IsSM2SM3PFXCert ", "g_GmsslFunctions : ", (long)g_GmsslFunctions);
	if(g_GmsslFunctions == NULL || !g_GmsslFunctions->IsValid() || wsCertFile.IsEmpty())
		return FALSE;

	FX_BOOL bRet = FS_IsFileExist(wsCertFile);
	WriteLog_GSDK("IsSM2SM3PFXCert ", "bRet : ", bRet);
	if(!bRet)	return FALSE;

	FX_INT32 nRet = ParseP12File_Ofd(wsCertFile, wsPwd, &m_pkey, &m_x509, &m_ca);
	WriteLog_GSDK("IsSM2SM3PFXCert ", "nRet : ", nRet);
	if (nRet != 0)
		return FALSE;

	int ret = GetCertInfo(m_x509);
	WriteLog_GSDK("IsSM2SM3PFXCert ", "ret : ", ret);
	if(ret == 0 && IsSm2sm3Sign(m_x509))
	{
		m_bParsePfxFile = TRUE;
		WriteLog_GSDK("IsSM2SM3PFXCert ", "m_bParsePfxFile : ", m_bParsePfxFile);
		return TRUE;
	}

	return FALSE;
}

FX_INT32 CFS_SignatureHandlerAdbePkcs7DetachedGmssl::ParseP12File(CFX_WideString wsCertFile, CFX_WideString wsPwd)
{
	WriteLog_GSDK("ParseP12File ", "m_bParsePfxFile : ", m_bParsePfxFile);
	if(m_bParsePfxFile)
		return 0;

	//WriteLog_GSDK("ParseP12File ", "g_GmsslFunctions : ", (long)g_GmsslFunctions);
	if(g_GmsslFunctions == NULL || wsCertFile.IsEmpty())
		return -1;

	FX_BOOL bRet = FS_IsFileExist(wsCertFile);
	if(!bRet)	return -1;

	FX_INT32 nRet = ParseP12File_Ofd(wsCertFile, wsPwd, &m_pkey, &m_x509, &m_ca);
	//WriteLog_GSDK("ParseP12File ", "nRet : ", nRet);
	if (nRet != 0)
		return nRet;

	int ret = GetCertInfo(m_x509);
	//WriteLog_GSDK("ParseP12File ", "ret : ", ret);

	if(ret == 0)
		m_bParsePfxFile = TRUE;

	return ret;
}

FX_BOOL CFS_SignatureHandlerAdbePkcs7DetachedGmssl::StartCalcDigest_PDF(IFX_FileRead* pFile, const CFX_DWordArray& byteArray, /*CPDF_Signature*/void* pSig, FX_LPVOID pClientData/* = NULL*/)
{
	if (!pFile || !pSig) return false;

	if (byteArray.GetSize() != 4 || byteArray.GetAt(0) != 0) return false;

	m_pSig = pSig;
	m_pFile = pFile;
	m_dwByteRangeArray = byteArray;

	return true;
}

FX_INT32 CFS_SignatureHandlerAdbePkcs7DetachedGmssl::ContinueCalcDigest_PDF(IFX_Pause* pPause /* = NULL */, FX_LPVOID pClientData/* = NULL*/)
{
	ASSERT(g_GmsslFunctions->IsValid());
	if (!GetTextFromFile(m_pFile, m_dwByteRangeArray, m_bsDigest))
		return Failed;

	return Done;
}

void CFS_SignatureHandlerAdbePkcs7DetachedGmssl::CRYPT_SM3Generate(FX_LPCBYTE data, FX_DWORD size, FX_BYTE digest[32])
{

}

CFX_ByteString CFS_SignatureHandlerAdbePkcs7DetachedGmssl::GetDigest(FX_LPVOID pClientData/* = NULL*/) const
{
	return m_bsDigest;
}

FX_BOOL CFS_SignatureHandlerAdbePkcs7DetachedGmssl::PKCS7_Sign(const CFX_ByteString& bsPlainText, CFX_ByteString& bsSignedData)
{
	WriteLog_GSDK("Gmssl::PKCS7_Sign ", "g_GmsslFunctions : ", (long)g_GmsslFunctions);
	ASSERT(g_GmsslFunctions->IsValid());
	PKCS7* p7 = PKCS7_new();

	PKCS7_set_type(p7, NID_pkcs7_signed);
	PKCS7_content_new(p7, NID_pkcs7_data);
	//PKCS7_set_detached(p7, 1);
	//Application should not judge the sign algorithm with the content's length.
	//Here, just for convenient;
	WriteLog_GSDK("Gmssl::PKCS7_Sign ", "bsPlainText.GetLength : ", bsPlainText.GetLength());
	//if (bsPlainText.GetLength() > 32)
		PKCS7_ctrl(p7, PKCS7_OP_SET_DETACHED_SIGNATURE, 1, NULL);


	PKCS7_SIGNER_INFO* info = NULL;

	if (IsSm2sm3Sign(m_x509))
	{
		WriteLog_GSDK("Gmssl::PKCS7_Sign ", "IsSm2sm3Sign ...");
		info = PKCS7_add_signature(p7, m_x509, m_pkey, EVP_sm3());
		/* The last parameter is the amount to add/subtract from the current
		* time (in seconds) */
		ASN1_UTCTIME *sign_time = X509_gmtime_adj(NULL, 0);
		PKCS7_add_signed_attribute(info, NID_pkcs9_signingTime, V_ASN1_UTCTIME, (char *)sign_time);
		//FX_LPBYTE pSMID = m_pSig->GetSMID();
		FX_LPBYTE pSMID = NULL;//m_pSig->GetSMID();
		unsigned char sm2sm3ID[17] = "1234567812345678";
		if(pSMID != NULL)
		{
			FXSYS_memcpy32(sm2sm3ID, pSMID, 16);
		}
		Setsm2sm3ID(sm2sm3ID, 16);	
		SetX509(m_x509);
	}
	else
	{
		WriteLog_GSDK("Gmssl::PKCS7_Sign ", "Is not Sm2sm3Sign ...");
		info = PKCS7_add_signature(p7, m_x509, m_pkey, EVP_sha1());
	}

	PKCS7_add_certificate(p7, m_x509);

	for (FX_INT32 i = 0; i< sk_num(CHECKED_STACK_OF(X509, m_ca)); i++)
		PKCS7_add_certificate(p7, (X509*)sk_value(CHECKED_STACK_OF(X509, m_ca), i));

	//Set source data to BIO.
	BIO* p7bio = PKCS7_dataInit(p7, NULL);
	BIO_write(p7bio, (FX_LPCSTR)bsPlainText, bsPlainText.GetLength());
	PKCS7_dataFinal(p7, p7bio); 
#if 0
	CPDF_TimeStampServerMgr* pServerMgr = CPDF_TimeStampServerMgr::GetTimeStampServerMgr();
	CPDF_TimeStampServer* pServer = NULL;
	if (pServerMgr)
		pServer = pServerMgr->GetDefaultServer();
	if (pServer)
	{
		//append the time stamp token to pkcs7 structure.
		unsigned char md256[32] = "";
		SHA256(info->enc_digest->data, info->enc_digest->length, md256);

		unsigned char *pTSRep = NULL;
		long lTSRepLength = Get_TS_REP(md256, 32, pServer, &pTSRep);
		if (lTSRepLength != 0)
			append_tsp_token(info, pTSRep, lTSRepLength);
		FX_Free(pTSRep);
	}
#endif
	BIO_free_all(p7bio);

	//Get Signed data.
	unsigned char* der = NULL;
	unsigned char* derTmp = NULL;
	unsigned long derlen = i2d_PKCS7(p7, NULL);
	der = (unsigned char*)FX_Alloc(unsigned char*, derlen);
	FXSYS_memset(der, 0, derlen);
	derTmp = der;
	i2d_PKCS7(p7, &derTmp);
	PKCS7_free(p7);
	//WriteLog_GSDK("PKCS7_Sign ", "derlen : ", derlen);
	bsSignedData = CFX_ByteString(der, derlen);
	FX_Free(der);
	
	return true;
}

FX_BOOL	CFS_SignatureHandlerAdbePkcs7DetachedGmssl::Sign(const CFX_ByteString& bsDigest, CFX_ByteString& bsSignedData, FX_LPVOID pClientData/* = NULL*/)
{
//	initialize_openssl();
	ASSERT(g_GmsslFunctions->IsValid());
	return PKCS7_Sign(bsDigest, bsSignedData);
}

FX_BOOL CFS_SignatureHandlerAdbePkcs7DetachedGmssl::PKCS7_VerifySig(const CFX_ByteString& bsSignedData, const CFX_ByteString& bsPlainText, FX_DWORD& dwVerRes, FX_CERT_INFO *signCertInfo /*= NULL*/)
{
	WriteLog_GSDK("PKCS7_VerifySig ", "111 ...");
	ASSERT(g_GmsslFunctions->IsValid());
	FX_BOOL bRet = false;
	WriteLog_GSDK("PKCS7_VerifySig bsSignedData : ", bsSignedData);
	//Retain PKCS7 object from signed data. 
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	std::unique_ptr<BIO, deleterBIO> vin(BIO_new_mem_buf((void*)(FX_LPCSTR)bsSignedData, bsSignedData.GetLength()));
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
	BIO* vin = BIO_new_mem_buf((void*)(FX_LPCSTR)bsSignedData, bsSignedData.GetLength());
#endif
	WriteLog_GSDK("PKCS7_VerifySig ", "113 ...");
	if (!vin)
		return false;

	WriteLog_GSDK("PKCS7_VerifySig ", "114 ...");
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	std::unique_ptr<PKCS7, deleterPKCS7> p7(d2i_PKCS7_bio(vin.get(), NULL));
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
	PKCS7* p7 = d2i_PKCS7_bio(vin, NULL);
#endif
	WriteLog_GSDK("PKCS7_VerifySig ", "115 ...");
	if (!p7)
	{
		WriteLog_GSDK("PKCS7_VerifySig ", "!p7 ...");
#if _FX_OS_ == _FX_LINUX_DESKTOP_
		deleterBIO()(vin);
#endif
		return false;
	}

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	STACK_OF(PKCS7_SIGNER_INFO) *sk = PKCS7_get_signer_info(p7.get());
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
	STACK_OF(PKCS7_SIGNER_INFO) *sk = PKCS7_get_signer_info(p7);
#endif
	WriteLog_GSDK("PKCS7_VerifySig ", "116 ...");
	if (!sk)
	{
		WriteLog_GSDK("PKCS7_VerifySig ", "!sk ...");
#if _FX_OS_ == _FX_LINUX_DESKTOP_
		deleterPKCS7()(p7);
		deleterBIO()(vin);
#endif
		return false;
	}

	int signCount = sk_PKCS7_SIGNER_INFO_num(sk);
	WriteLog_GSDK("PKCS7_VerifySig ", "signCount : ", signCount);
	if (signCount != 1)
	{
#if _FX_OS_ == _FX_LINUX_DESKTOP_
		deleterPKCS7()(p7);
		deleterBIO()(vin);
#endif
		return false;
	}

	for(int i = 0;i < signCount; i++)
	{
		PKCS7_SIGNER_INFO *signInfo = sk_PKCS7_SIGNER_INFO_value(sk,i);

		ASN1_TYPE *tstoken = PKCS7_get_attribute(signInfo, NID_id_smime_aa_timeStampToken);
		if (tstoken)//verify the time stamp token.
		{
			FX_BOOL bTSAvail = false;
			ASN1_STRING *set = (ASN1_STRING *)(tstoken->value.set);
			int length = ASN1_STRING_length(set);
			unsigned char* tstokeng = ASN1_STRING_data(set);
			
			unsigned char *p = tstokeng;
			PKCS7* tsp7 = d2i_PKCS7(NULL, (const unsigned char**)&p, length);
			
			BIO* p7bio = PKCS7_dataDecode(tsp7, 0, 0, 0);
			unsigned char src[1024] = "";
			length =BIO_read(p7bio, src, 1024);
			
			p = src;
			TS_TST_INFO *req = d2i_TS_TST_INFO(NULL,(const unsigned char **)&p, length);
			//if time stamp is correct, time string is in req->time->data.
			// TESTDOC: Bug#CORE-6384 - 36172_Pielikums_2.pdf
			// check req's value whether or not null.
			if(req)
			{
				if (req->msg_imprint->hashed_msg->length == 20)
				{
					unsigned char md[20];
					SHA1(signInfo->enc_digest->data,signInfo->enc_digest->length,md);
					if (!FXSYS_memcmp(md, req->msg_imprint->hashed_msg->data, 20))
						bTSAvail = true;
				}
				else if (req->msg_imprint->hashed_msg->length == 32)
				{
					unsigned char md[32];
					SHA256(signInfo->enc_digest->data,signInfo->enc_digest->length,md);
					if (!FXSYS_memcmp(md, req->msg_imprint->hashed_msg->data, 32))
						bTSAvail = true;
				}
			}
		}

		BIO *p7bio = BIO_new_mem_buf((void*)(FX_LPCSTR)bsPlainText, bsPlainText.GetLength());
#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
		X509 *x509= PKCS7_cert_from_signer_info(p7.get(),signInfo);
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
		X509 *x509= PKCS7_cert_from_signer_info(p7,signInfo);
#endif
		if ((i == signCount - 1) && signCertInfo) {
			ASN1_INTEGER *serial = X509_get_serialNumber(x509);
			FX_BYTE pHexData[256] = {0};
			HexCryptBuffer((FX_BYTE*)serial->data, serial->length, pHexData);
			signCertInfo->certSerialNum = CFX_ByteString(pHexData, serial->length * 2);

			char csbuf[256] = {0};
			X509_NAME *issuerName = X509_get_issuer_name(x509);
			X509_NAME_get_text_by_NID(issuerName, NID_commonName, csbuf, 256);
			signCertInfo->certPublisher = CFX_ByteString(csbuf, (FX_STRSIZE)strlen(csbuf));

			X509_NAME *subjectName = X509_get_subject_name(x509);
			X509_NAME_ENTRY *subjectEntry = X509_NAME_get_entry(subjectName, 0);
			FXSYS_memset(csbuf, 256 * sizeof(char), 0);
			X509_NAME_get_text_by_NID(subjectName, NID_pkcs9_emailAddress, csbuf, 256);
			signCertInfo->certEmailInfo = CFX_ByteString(csbuf, (FX_STRSIZE)strlen(csbuf));

			/*ASN1_TIME *startTime = X509_get_notBefore(x509);
			time_t timeStart = getTimeFromASN1(startTime);
			struct tm tm1;
			localtime_r(&timeStart, &tm1);
			signCertInfo->certStartDate.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
			tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
			tm1.tm_hour, tm1.tm_min,tm1.tm_sec);

			ASN1_TIME *endTime = X509_get_notAfter(x509);
			time_t timeEnd = getTimeFromASN1(endTime);
			localtime_r(&timeEnd, &tm1);
			signCertInfo->certEndDate.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
			tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
			tm1.tm_hour, tm1.tm_min,tm1.tm_sec);*/
		}
		if (IsSm2sm3Sign(x509))
		{
			unsigned char sm2sm3ID[17] = "1234567812345678";
			CFX_ByteString bsSMID;
			//CPDF_Signature
			//m_pSig->GetKeyValue(FX_BSTRC("SMID"), bsSMID);
			if(bsSMID.GetLength() != 16 && bsSMID.GetLength() != 0)
			{
				dwVerRes = -21;
				WriteLog_GSDK("PKCS7_VerifySig", " dwVerRes 1 : ", dwVerRes);
				BIO_free(p7bio);
				WriteLog_GSDK("PKCS7_VerifySig", " bRet 1 : ", bRet);
				return bRet;
			}
			if(bsSMID.GetLength() == 16)
			{
				FXSYS_memcpy32(sm2sm3ID, bsSMID.GetBuffer(16), 16);
			}
			Setsm2sm3ID(sm2sm3ID, 16);
		}

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
		int err = PKCS7_verify(p7.get(), NULL, NULL, p7bio, NULL, PKCS7_NOVERIFY); 
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
		int err = PKCS7_verify(p7, NULL, NULL, p7bio, NULL, PKCS7_NOVERIFY); 
#endif
		WriteLog_GSDK("PKCS7_VerifySig ", "err : ", err);
		if(err == 1)
		{
			bRet = true;
		}
		else
		{
			dwVerRes = -22;
			WriteLog_GSDK("PKCS7_VerifySig", " dwVerRes 2 : ", dwVerRes);
		}
		BIO_free(p7bio);
	}

#if _FX_OS_ == _FX_LINUX_DESKTOP_
		deleterPKCS7()(p7);
		deleterBIO()(vin);
#endif
		WriteLog_GSDK("PKCS7_VerifySig", " bRet 2 : ", bRet);
	return bRet;
}

FX_BOOL CFS_SignatureHandlerAdbePkcs7DetachedGmssl::Verify(const CFX_ByteString& bsDigest, const CFX_ByteString& bsSignedData, FX_DWORD& dwVerRes, FX_LPVOID pClientData/* = NULL*/)
{
	WriteLog_GSDK("Pkcs7DetachedGmssl::Verify ", "beg ...");
	//initialize_openssl();
	ASSERT(g_GmsslFunctions->IsValid());
	dwVerRes = -31;

	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();
	WriteLog_GSDK("Pkcs7DetachedGmssl::Verify ", "111 ...");
	//FX_CERT_INFO certInfo;
	if (!PKCS7_VerifySig(bsSignedData, bsDigest, dwVerRes, &m_certInfo)) {
		dwVerRes = -32;
		WriteLog_GSDK("Gmssl::Verify", "dwVerRes 1 : ", dwVerRes);
		return false;
	}
	WriteLog_GSDK("Pkcs7DetachedGmssl::Verify ", "112 ...");
#if 0
	FX_BOOL bRet = CPDF_DMDetector::LeakDetect();
	if (bRet) {
		dwVerRes |= CPDF_SIGNATURE_STATE_VERIFY_INCREDIBLE;
	}
	else {
		if (dwVerRes & CPDF_SIGNATURE_STATE_VERIFY_VALID) {//LeckDetect
			FX_DWORD dwRet = CPDF_DMDetector::VerifyModiDetect();
			if (dwRet & CPDF_SIGNATURE_STATE_VERIFY_INVALID)
				dwVerRes &= ~ CPDF_SIGNATURE_STATE_VERIFY_VALID;

			dwVerRes |= dwRet;
		}
	}
#endif	
//	VerifyIssuer();
	dwVerRes = 0;
	WriteLog_GSDK("Gmssl::Verify", "dwVerRes 2 : ", dwVerRes);
	return true;
}

CFX_ByteString CFS_SignatureHandlerAdbePkcs7DetachedGmssl::GetCertProperty(FX_INT32 nIndex)
{
	//WriteLog_GSDK("Gmssl::GetCertProperty ", "nIndex : ", nIndex);
	if(nIndex < 0 || nIndex > 4)
		return "";

	CFX_ByteString bsProperty;

	//WriteLog_GSDK("Gmssl::GetCertProperty m_certInfo.certSerialNum : ", m_certInfo.certSerialNum);
	//WriteLog_GSDK("Gmssl::GetCertProperty m_certInfo.certPublisher : ", m_certInfo.certPublisher);
	//WriteLog_GSDK("Gmssl::GetCertProperty m_certInfo.certStartDate : ", m_certInfo.certStartDate);
	//WriteLog_GSDK("Gmssl::GetCertProperty m_certInfo.certEndDate : ", m_certInfo.certEndDate);
	//WriteLog_GSDK("Gmssl::GetCertProperty m_certInfo.certEmailInfo : ", m_certInfo.certEmailInfo);

	switch (nIndex)
	{
	case 0:
		bsProperty = m_certInfo.certSerialNum;
		break;
	case 1:
		bsProperty = m_certInfo.certPublisher;
		break;
	case 2:
		bsProperty = m_certInfo.certStartDate;
		break;
	case 3:
		bsProperty = m_certInfo.certEndDate;
		break;
	case 4:
		bsProperty = m_certInfo.certEmailInfo;
		break;
	}

	return bsProperty;
}

#endif //_FX_PRODUCT_ == _FS_ESSDK_ || _FX_PRODUCT_ == _FS_NONE_