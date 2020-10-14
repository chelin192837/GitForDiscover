#include "../include/stdafx.h"

//////////////////////////////////////////////////////////////////////////
COFD_OESVerifyHandler::COFD_OESVerifyHandler():
m_cbData(0)
{
    m_pbData = NULL;
    m_strDigestMethod.clear();
}

COFD_OESVerifyHandler::~COFD_OESVerifyHandler()
{
    SAFE_DELETE_BLOCK(m_pbData);
}

COFD_OESVerifyHandler* COFD_OESVerifyHandler::Create(char* pbPFXData, FX_DWORD cbPFXData, std::string PFX_PWD, std::string strDigestMetohd)
{
      if (!pbPFXData || cbPFXData == 0 || strDigestMetohd.empty()) return NULL;

    COFD_OESVerifyHandler* pHandler = NULL;

    COFD_Utils::InitOpenSSL();

    BIO* bo = BIO_new_mem_buf(pbPFXData, cbPFXData);
    if (!bo) return NULL;

    //Extracting client certificate & private key from .pfx or .p12 file
    PKCS12 *p12 = d2i_PKCS12_bio(bo, NULL);
    BIO_free(bo);

    if (!p12)
    {
        printf("Extracting client certificate & private key failed.");
        return NULL;
    }

    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;

    STACK_OF(X509) *ca = NULL;
    int nLen = 0;
    char* pCertData = NULL;
    int result = PKCS12_parse(p12, PFX_PWD.data(), &pkey, &x509, &ca);
    if (result)
    {
        nLen = i2d_X509(x509, NULL);
        pCertData = new char[nLen];
        memset(pCertData, 0, nLen);

        unsigned char* pOutPut = (unsigned char*)pCertData;
        nLen = i2d_X509(x509, &pOutPut);
    }
    X509_free(x509);
    PKCS12_free(p12);
    EVP_PKEY_free(pkey);
    sk_X509_free(ca);

    EVP_cleanup();

    if (pCertData && nLen > 0)
    {
        FX_DWORD dwCertDataLen = nLen;
        pHandler = COFD_OESVerifyHandler::Create(pCertData, dwCertDataLen, strDigestMetohd);
    }

    SAFE_DELETE_BLOCK(pCertData);

	return pHandler;
}

COFD_OESVerifyHandler* COFD_OESVerifyHandler::Create(char* pbCerData, FX_DWORD cbCerData, std::string strDigestMethod)
{
    if (!pbCerData || cbCerData == 0 || strDigestMethod.empty()) return NULL;

    COFD_OESVerifyHandler* pHandler = new COFD_OESVerifyHandler;
    if (!pHandler->Initialize(pbCerData, cbCerData, strDigestMethod))
    {
        SAFE_DELETE(pHandler);
    }

    return pHandler;
}

bool COFD_OESVerifyHandler::Initialize(char* pbCerData, FX_DWORD cbCerData, std::string strDigestMethod)
{
    COFD_Utils::InitOpenSSL();

    /**@brief hint
     * d2i_X509 会使传入的指针发生偏移（移动至内存尾部）。
     * 这里使用副本传入，以防止pbCerData地址被更改。
    */
    const char* pbCerData_x = pbCerData;
    X509* x509 = d2i_X509(NULL, (const unsigned char**)&pbCerData_x, cbCerData);
    if (!x509)
        return false;
    else
    {
        m_cbData = cbCerData;
        m_pbData = new char[cbCerData];
        memset(m_pbData, 0, cbCerData);
        memcpy(m_pbData, pbCerData, cbCerData);

        m_strDigestMethod = strDigestMethod;
    }
    X509_free(x509);

    return true;
}

bool	COFD_OESVerifyHandler::Verify(unsigned char* pbContent, FX_DWORD cbContent,
                                                  unsigned char* pbSignedData, FX_DWORD cbSignedData)
{
    COFD_Utils::InitOpenSSL();

    /**@brief hint
     * d2i_X509 会使传入的指针发生偏移（移动至内存尾部）。
     * 这里使用副本传入，以防止m_pbData地址被更改。
    */
    const char* pbCerData_x = m_pbData;
    X509* x509 = d2i_X509(NULL, (const unsigned char**)&pbCerData_x, m_cbData);
    if (!x509) return false;

    EVP_PKEY* pPubKey = X509_get_pubkey(x509);
    if (!pPubKey)
    {
        X509_free(x509);
        return false;
    }

    const EVP_MD* md = EVP_get_digestbyname(m_strDigestMethod.data());

    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    EVP_MD_CTX_init(md_ctx);
    
    bool bRet = true;
    if (bRet) bRet = EVP_VerifyInit_ex(md_ctx, md, NULL);
    if (bRet) bRet = EVP_VerifyUpdate(md_ctx, pbContent, cbContent);
    if (bRet) bRet = EVP_VerifyFinal(md_ctx, pbSignedData, cbSignedData, pPubKey);
    
    EVP_MD_CTX_free(md_ctx);

    X509_free(x509);

    CRYPTO_cleanup_all_ex_data();

    ERR_free_strings();
    //ERR_remove_state(0); // comment by gm, ld error in android

    EVP_PKEY_free(pPubKey);
    EVP_cleanup();

    return bRet;
}

