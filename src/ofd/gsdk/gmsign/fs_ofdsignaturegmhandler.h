#ifndef _FS_OFDSIGNATUREGMHANDLER_H_
#define _FS_OFDSIGNATUREGMHANDLER_H_

#include "openssl/ossl_typ.h"
#include "openssl/safestack.h"

typedef struct SIG_CertInfo : CFX_Object
{
	CFX_ByteString			certSerialNum;
	CFX_ByteString			certPublisher;
	CFX_ByteString			certStartDate;
	CFX_ByteString			certEndDate;
	CFX_ByteString			certEmailInfo;
	//	FXCRT_DATETIMEZONE		signDate;
	//	const CFX_DWordArray	byteRangeArray;
}FX_CERT_INFO;


class CFS_SignatureHandlerAdbePkcs7DetachedGmssl : public CFX_Object
{
public:
	static FX_BOOL				Create();
	static void					Destory();
	static CFS_SignatureHandlerAdbePkcs7DetachedGmssl*		Get();

	FX_BOOL						IsValid();

	FX_INT32					GetCertificateInfo(/*CPDF_Signature*/void* pSig, FX_LPVOID pClientData = NULL);

	FX_BOOL						IsSM2SM3PFXCert(CFX_WideString wsCertFile, CFX_WideString wsPwd);

	FX_INT32					ParseP12File(CFX_WideString wsCertFile, CFX_WideString wsPwd);

	virtual FX_BOOL				StartCalcDigest_PDF(IFX_FileRead* pFile, const CFX_DWordArray& byteArray, /*CPDF_Signature*/void* pSig, FX_LPVOID pClientData = NULL);
	virtual FX_INT32			ContinueCalcDigest_PDF(IFX_Pause* pPause = NULL, FX_LPVOID pClientData = NULL);

	void						CRYPT_SM3Generate(FX_LPCBYTE data, FX_DWORD size, FX_BYTE digest[32]);

	virtual CFX_ByteString		GetDigest(FX_LPVOID pClientData = NULL) const;

	virtual FX_BOOL				Sign(const CFX_ByteString& bsDigest, CFX_ByteString& bsSignedData, FX_LPVOID pClientData = NULL);
	virtual FX_BOOL				Verify(const CFX_ByteString& bsDigest, const CFX_ByteString& bsSignedData, FX_DWORD& dwVerResult, FX_LPVOID pClientData = NULL);
	CFX_ByteString				GetCertProperty(FX_INT32 nIndex);
protected:
	CFS_SignatureHandlerAdbePkcs7DetachedGmssl();
	virtual ~CFS_SignatureHandlerAdbePkcs7DetachedGmssl();

private:
	FX_BOOL						PKCS7_Sign(const CFX_ByteString& bsPlainText, CFX_ByteString& bsSignedData);
	FX_BOOL						PKCS7_VerifySig(const CFX_ByteString& bsSignedData, const CFX_ByteString& bsPlainText, FX_DWORD& dwVerRes, FX_CERT_INFO *signCertInfo = NULL);

	/*CPDF_Signature*/void*			m_pSig;
	IFX_FileRead*			m_pFile;
	CFX_DWordArray			m_dwByteRangeArray;
	CFX_ByteString			m_bsDigest;

	EVP_PKEY*				m_pkey;
	X509*					m_x509;
	STACK_OF(X509)*			m_ca;
	FX_CERT_INFO			m_certInfo;
	FX_BOOL					m_bParsePfxFile;
};


#endif //_FS_OFDSIGNATUREGMHANDLER_H_

