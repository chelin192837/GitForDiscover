#include "../include/stdafx.h"

err_t COFD_BaseHandler::Digest(unsigned char* pbToBeHash, FX_DWORD cbToBeHash,
                                                      unsigned char* pbHashedData, int* pcbHashedData,
                                                      const char* pDigestMethod)
{
    COFD_Utils::InitOpenSSL();

    const EVP_MD* md = EVP_get_digestbyname(pDigestMethod);

    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    EVP_MD_CTX_init(md_ctx);
    
    unsigned int s = 0;
    unsigned char szDigestResult[EVP_MAX_MD_SIZE] = { 0 };
    
    bool success = false;
    do {
        if (!EVP_DigestInit(md_ctx, md)) break;;
        if (!EVP_DigestUpdate(md_ctx, pbToBeHash, cbToBeHash)) break;
        if (!EVP_DigestFinal(md_ctx, szDigestResult, &s))  break;
        success = true;
    } while (0);
    
    EVP_MD_CTX_free(md_ctx);
    
    if (!success) {
        return OES_ERROR_DIGEST_ERROR;
    }

    if (pbHashedData)
    {
        if (*pcbHashedData >= int(s))
        {
            memcpy(pbHashedData, szDigestResult, s);
        }
        else
        {
            return OES_ERROR_BUFFER_NOT_ENUGH;
        }
    }

    *pcbHashedData = s;

    return OES_OK;
}


