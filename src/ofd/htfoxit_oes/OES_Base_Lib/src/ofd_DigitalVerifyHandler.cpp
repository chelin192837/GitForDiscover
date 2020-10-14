#include "../include/stdafx.h"

//////////////////////////////////////////////////////////////////////////
COFD_DigitalVerifyHandler::COFD_DigitalVerifyHandler()
{

}

COFD_DigitalVerifyHandler::~COFD_DigitalVerifyHandler()
{

}

COFD_DigitalVerifyHandler* COFD_DigitalVerifyHandler::Create()
{
    return new COFD_DigitalVerifyHandler;
}

bool COFD_DigitalVerifyHandler::GetSigner(unsigned char* pbSignedValue, FX_DWORD cbSignedValue,
                                                            char* pbSigner, FX_DWORD* cbSigner)
{
    COFD_Utils::InitOpenSSL();

    BIO* SignValue_bio = BIO_new_mem_buf(pbSignedValue, cbSignedValue);
    PKCS7* p7 = d2i_PKCS7_bio(SignValue_bio, NULL);
    BIO_free(SignValue_bio);

    if (!p7) return false;

    if (!PKCS7_type_is_signed(p7))
    {
        PKCS7err(PKCS7_F_PKCS7_VERIFY, PKCS7_R_WRONG_CONTENT_TYPE);

        PKCS7_free(p7);
        return false;
    }

    /*p7结构中应不含明文信息(被签名数据)*/
    if (!PKCS7_get_detached(p7))
    {
        PKCS7err(PKCS7_F_PKCS7_VERIFY, PKCS7_R_NO_CONTENT);

        PKCS7_free(p7);
        return false;
    }

    std::string strSigner;
    strSigner.assign("DN: ");
    STACK_OF(PKCS7_SIGNER_INFO)* sinfos = PKCS7_get_signer_info(p7);
    for (int i = 0; i < sk_PKCS7_SIGNER_INFO_num(sinfos); i++)
    {
        PKCS7_SIGNER_INFO* si = sk_PKCS7_SIGNER_INFO_value(sinfos, i);
        X509* signer = PKCS7_cert_from_signer_info(p7, si);

        if (i != 0)
        {
            strSigner.append(";");
        }

        std::string strSubject = COFD_Utils::GetSubjectName(signer);
        strSigner.append(strSubject);
    }

    PKCS7_free(p7);

    FX_DWORD dwBuffSize = *cbSigner;
    FX_DWORD dwLen = strSigner.length();

    *cbSigner = dwLen;

    if (!pbSigner) return true;
    if (dwBuffSize < dwLen) return false;

    memcpy(pbSigner, strSigner.data(), dwLen);

    return true;
}

bool	COFD_DigitalVerifyHandler::Verify(unsigned char* pbContent, FX_DWORD cbContent,
                                                  unsigned char* pbSignedData, FX_DWORD cbSignedData)
{
    COFD_Utils::InitOpenSSL();

    BIO* SignValue_bio = BIO_new_mem_buf(pbSignedData, cbSignedData);
    PKCS7* p7 = d2i_PKCS7_bio(SignValue_bio, NULL);
    BIO_free(SignValue_bio);

    if (!p7) return false;

    if (!PKCS7_type_is_signed(p7))
    {
        PKCS7err(PKCS7_F_PKCS7_VERIFY, PKCS7_R_WRONG_CONTENT_TYPE);

        PKCS7_free(p7);
        return false;
    }

    /*p7结构中应不含明文信息(被签名数据)*/
    if (!PKCS7_get_detached(p7))
    {
        PKCS7err(PKCS7_F_PKCS7_VERIFY, PKCS7_R_NO_CONTENT);

        PKCS7_free(p7);
        return false;
    }

    STACK_OF(PKCS7_SIGNER_INFO)* sinfos = PKCS7_get_signer_info(p7);

    if (!sinfos || !sk_PKCS7_SIGNER_INFO_num(sinfos))
    {
        PKCS7err(PKCS7_F_PKCS7_VERIFY, PKCS7_R_NO_SIGNATURES_ON_DATA);

        PKCS7_free(p7);
        return false;
    }

    BIO* indata = BIO_new_mem_buf(pbContent, cbContent);
    if (!indata) return false;

    BIO *p7bio = PKCS7_dataInit(p7, indata);
    if (!p7bio) return false;

    /* 这里必须将IO设备的内容读取一遍，才能保证验证正常。*/
    while(true)
    {
        char buf[4096] = { 0 };
        int i = BIO_read(p7bio, buf, sizeof(buf));
        if (i <= 0) break;
    }

    /* PKCS7支持多个身份的签署，这里逐一验证。 */
    bool ret = true;
    for (int i = 0; i < sk_PKCS7_SIGNER_INFO_num(sinfos); i++)
    {
        PKCS7_SIGNER_INFO* si = sk_PKCS7_SIGNER_INFO_value(sinfos, i);
        X509* signer = PKCS7_cert_from_signer_info(p7, si);
        int ret = PKCS7_signatureVerify(p7bio, p7, si, signer);
        if (ret <= 0)
        {
            PKCS7err(PKCS7_F_PKCS7_VERIFY, PKCS7_R_SIGNATURE_FAILURE);
            ret = false;
            break;
        }
    }

    BIO_free_all(p7bio);

    return ret;
}

