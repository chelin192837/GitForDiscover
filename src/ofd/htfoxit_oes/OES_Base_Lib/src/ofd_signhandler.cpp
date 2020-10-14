#include "../include/stdafx.h"

extern const char* szPFX;
//////////////////////////////////////////////////////////////////////////
COFD_OESSignHandler* COFD_OESSignHandler::Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strDigestMetohd)
{
    COFD_OESSignHandler* pInstance = new COFD_OESSignHandler;
    if (!pInstance->Initialize(pbData, cbData, PFX_PWD, strDigestMetohd))
	{
        SAFE_DELETE(pInstance);
	}

	return pInstance;
}

COFD_OESSignHandler::COFD_OESSignHandler():
m_bValid(FALSE),
m_cbData(0)
{
    m_pbData = NULL;
    m_strPasswd.clear();
    m_strDigestMethod.clear();
}

bool COFD_OESSignHandler::Initialize(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strDigestMetohd)
{
    if (!pbData || cbData == 0 || strDigestMetohd.empty()) return false;

    COFD_Utils::InitOpenSSL();

    BIO* bo = BIO_new_mem_buf(pbData, cbData);
    if (!bo) return false;

    //Extracting client certificate & private key from .pfx or .p12 file
    PKCS12 *p12 = d2i_PKCS12_bio(bo, NULL);
    BIO_free(bo);

    if (!p12)
    {
        printf("Extracting client certificate & private key failed.");
        return false;
    }

    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;

    STACK_OF(X509) *ca = NULL;
    int result = PKCS12_parse(p12, PFX_PWD.data(), &pkey, &x509, &ca);
    if (result)
    {
        if (COFD_Utils::IsCertHasSignUsage(x509))
        {
            m_cbData = cbData;
            m_pbData = new char[m_cbData];
            memset(m_pbData, 0, m_cbData);
            memcpy(m_pbData, pbData, m_cbData);

            m_strDigestMethod = strDigestMetohd;
            m_strPasswd = PFX_PWD;

            m_bValid = true;
        }
    }

    X509_free(x509);
    PKCS12_free(p12);
    EVP_PKEY_free(pkey);
    sk_X509_free(ca);

    EVP_cleanup();

	return m_bValid;
}

COFD_OESSignHandler::~COFD_OESSignHandler()
{
    SAFE_DELETE_BLOCK(m_pbData);
}

bool COFD_OESSignHandler::Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                           unsigned char* pbSignedData, FX_DWORD* pcbSignedData)
{
    COFD_Utils::InitOpenSSL();

    BIO* bo = BIO_new_mem_buf(m_pbData, m_cbData);
    if (!bo) return false;

    //Extracting client certificate & private key from .pfx or .p12 file
    PKCS12 *p12 = d2i_PKCS12_bio(bo, NULL);
    BIO_free(bo);

    if (!p12)
    {
        printf("Extracting client certificate & private key failed.");
        return false;
    }

    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;

    bool bRet = false;
    STACK_OF(X509) *ca = NULL;

    int result = PKCS12_parse(p12, m_strPasswd.data(), &pkey, &x509, &ca);
    if (result)
    {
        const EVP_MD* md = EVP_get_digestbyname(m_strDigestMethod.data());

        EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
        EVP_MD_CTX_init(md_ctx);
        
        unsigned int nLen = 0;
        unsigned char value[1024] = { 0 };
        
        bRet = true;
        if (bRet) bRet = EVP_SignInit_ex(md_ctx, md, NULL);
        if (bRet) bRet = EVP_SignUpdate(md_ctx, pbToBeSign, cbToBeSign);
        if (bRet) bRet = EVP_SignFinal(md_ctx, value, &nLen, pkey);
        
        EVP_MD_CTX_free(md_ctx);

        if (bRet)
        {
            if (pbSignedData)
            {
                if (*pcbSignedData >= nLen)
                {
                    memcpy(pbSignedData, value, nLen);
                }
                else
                {
                    bRet = false;
                }
            }
            *pcbSignedData = nLen;
        }
    }

    X509_free(x509);
    PKCS12_free(p12);

    CRYPTO_cleanup_all_ex_data();

    ERR_free_strings();
    //ERR_remove_state(0);	// comment by gm, ld error in android

    EVP_PKEY_free(pkey);
    sk_X509_free(ca);
    EVP_cleanup();

    return bRet;
}

void COFD_OESSignHandler::GetSignCertData(std::string& csCertData)
{
    COFD_Utils::InitOpenSSL();

    if (!IsValid()) return;

    BIO* bo = BIO_new_mem_buf(m_pbData, m_cbData);
    if (!bo) return;

    //Extracting client certificate & private key from .pfx or .p12 file
    PKCS12 *p12 = d2i_PKCS12_bio(bo, NULL);
    BIO_free(bo);

    if (!p12)
    {
        printf("Extracting client certificate & private key failed.");
        return;
    }

    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;

    STACK_OF(X509) *ca = NULL;
    int result = PKCS12_parse(p12, m_strPasswd.data(), &pkey, &x509, &ca);
    if (result)
    {
        int nLen = i2d_X509(x509, NULL);
        unsigned char* pCertData = new unsigned char[nLen];
        memset(pCertData, 0, nLen);

        unsigned char* pOutPut = pCertData;
        nLen = i2d_X509(x509, &pOutPut);

        csCertData.assign((char*)pCertData, nLen);

        delete[] pCertData;
    }

    X509_free(x509);
    PKCS12_free(p12);
    EVP_PKEY_free(pkey);
    sk_X509_free(ca);

    EVP_cleanup();
}

void COFD_OESSignHandler::GetSignAlgorithm(std::string& algorithm)
{
    // XCN_OID_NIST_sha256 (2.16.840.1.101.3.4.2.1)
    // XCN_OID_RSA_SHA1RSA (1.2.840.113549.1.1.5)
    // XCN_OID_RSA_SHA256RSA (1.2.840.113549.1.1.11)

    algorithm.assign(OES_SIGN_ALGORITHM_SHA256RSA);
}

//class COFD_OESSignHandlerEx : public COFD_OESSignHandler
COFD_OESSignHandlerEx* COFD_OESSignHandlerEx::Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strDigestMetohd)
{
    COFD_OESSignHandlerEx* pInstance = new COFD_OESSignHandlerEx;
    if (!pInstance->Initialize(pbData, cbData, PFX_PWD, strDigestMetohd))
    {
        SAFE_DELETE(pInstance);
    }
    
    return pInstance;
}

COFD_OESSignHandlerEx* COFD_OESSignHandlerEx::Create(IHTFX_SignatureCallback* handler, std::string strDigestMetohd)
{
    COFD_OESSignHandlerEx* pInstance = new COFD_OESSignHandlerEx;
    if (!pInstance->InitializeEx(handler, strDigestMetohd)) {
        SAFE_DELETE(pInstance);
    }
    return pInstance;
}

COFD_OESSignHandlerEx::COFD_OESSignHandlerEx()
: COFD_OESSignHandler()
, m_ExtHandler(NULL)
{
    
}

bool COFD_OESSignHandlerEx::InitializeEx(IHTFX_SignatureCallback* handler, std::string strDigestMetohd)
{
    m_ExtHandler = handler;
    return true;
}

bool						COFD_OESSignHandlerEx::Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
                                     unsigned char* pbSignedData, FX_DWORD* pcbSignedData)
{
    if (m_ExtHandler) {
        std::string digest((const char*)pbToBeSign, cbToBeSign);
        std::string result = m_ExtHandler->sign(digest);
        if (pbSignedData) {
            if (!pcbSignedData || *pcbSignedData<result.size()) {
                return false;
            } else {
                memcpy(pbSignedData, result.data(), result.size());
            }
        }
        if (pcbSignedData) {
            *pcbSignedData = result.size();
        } else {
            return false;
        }

        return true;
    } else {
        return COFD_OESSignHandler::Sign(pbToBeSign, cbToBeSign, pbSignedData, pcbSignedData);
    }
}
    
void						COFD_OESSignHandlerEx::GetSignCertData(std::string& csCertData)
{
    if (m_ExtHandler) {
        csCertData = m_ExtHandler->getCertData();
    } else {
        COFD_OESSignHandler::GetSignCertData(csCertData);
    }
}

void						COFD_OESSignHandlerEx::GetSignAlgorithm(std::string& csCertData)
{
    if (m_ExtHandler) {
        csCertData = m_ExtHandler->getSignMethod();
    } else {
        COFD_OESSignHandler::GetSignAlgorithm(csCertData);
    }
}

