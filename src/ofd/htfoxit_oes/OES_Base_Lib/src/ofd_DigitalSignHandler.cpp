#include "../include/stdafx.h"

extern const char* szPFX;
//////////////////////////////////////////////////////////////////////////
COFD_DigitalSignHandler* COFD_DigitalSignHandler::Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD, std::string strFilter)
{
    COFD_DigitalSignHandler* pHandler = NULL;
    if (strFilter.compare("sha256RSA") == 0)
    {
        pHandler = COFD_SHA256DigitalSignHandler::Create(pbData, cbData, PFX_PWD);
    }

    return pHandler;
}

///////////////////////////////////////////////////////////////////////////
COFD_SHA256DigitalSignHandler* COFD_SHA256DigitalSignHandler::Create(char* pbData, FX_DWORD cbData, std::string PFX_PWD)
{
    COFD_SHA256DigitalSignHandler* pHandler = new COFD_SHA256DigitalSignHandler;
    if (!pHandler->Initialize(pbData, cbData, PFX_PWD))
    {
        SAFE_DELETE(pHandler);
    }

    return pHandler;
}

COFD_SHA256DigitalSignHandler::COFD_SHA256DigitalSignHandler():
m_bValid(FALSE),
m_cbData(0)
{
    m_pbData = NULL;
    m_strPasswd.clear();
}

bool COFD_SHA256DigitalSignHandler::Initialize(char* pbData, FX_DWORD cbData, std::string PFX_PWD)
{
    if (!pbData || cbData == 0) return false;

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

COFD_SHA256DigitalSignHandler::~COFD_SHA256DigitalSignHandler()
{
    SAFE_DELETE_BLOCK(m_pbData);
}

bool COFD_SHA256DigitalSignHandler::Sign(unsigned char* pbToBeSign, FX_DWORD cbToBeSign,
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

    EVP_PKEY* pKey = NULL;
    X509* pCert = NULL;

    bool bRet = false;
    STACK_OF(X509) *ca = NULL;
    int result = PKCS12_parse(p12, m_strPasswd.data(), &pKey, &pCert, &ca);
    if (result)
    {
        PKCS7 *p7 = PKCS7_new();

        PKCS7_set_type(p7, NID_pkcs7_signed);
        PKCS7_content_new(p7, NID_pkcs7_data);
        PKCS7_sign_add_signer(p7, pCert, pKey, EVP_sha256(), PKCS7_DETACHED);

        for (int i = 0; i < sk_X509_num(ca); i++)
        {
            PKCS7_add_certificate(p7, sk_X509_value(ca, i));
        }

        PKCS7_set_detached(p7, 1);

        BIO* bio = BIO_new_mem_buf(pbToBeSign, cbToBeSign);

        if (PKCS7_final(p7, bio, PKCS7_DETACHED | PKCS7_BINARY))
        {
            bRet = true;
            FX_DWORD dwLen = i2d_PKCS7(p7, NULL);

            if (pbSignedData)
            {
                if (*pcbSignedData >= dwLen)
                {
                    unsigned char* pOutput = (unsigned char*)pbSignedData;
                    i2d_PKCS7(p7, &pOutput);
                }
                else
                {
                    bRet = false;
                }
            }
            *pcbSignedData = dwLen;
        }
        PKCS7_free(p7);
        BIO_free(bio);
    }

    X509_free(pCert);
    PKCS12_free(p12);
    EVP_PKEY_free(pKey);
    EVP_cleanup();

    return bRet;
}

void COFD_SHA256DigitalSignHandler::GetSignAlgorithm(std::string& algorithm)
{
    // XCN_OID_NIST_sha256 (2.16.840.1.101.3.4.2.1)
    // XCN_OID_RSA_SHA1RSA (1.2.840.113549.1.1.5)
    // XCN_OID_RSA_SHA256RSA (1.2.840.113549.1.1.11)

    algorithm.assign(OES_SIGN_ALGORITHM_SHA256RSA);
}
