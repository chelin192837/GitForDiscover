#include "../include/stdafx.h"


void releaseSealTag(SESeal_t seal)
{
    memdelete(seal.esealInfo.esID);

    memdelete(seal.esealInfo.header.id);
    memdelete(seal.esealInfo.header.vid);

    memdelete(seal.esealInfo.picture.type);
    memdelete(seal.esealInfo.picture.data);

    memdelete(seal.esealInfo.property.name);
    asn_set_empty(&(seal.esealInfo.property.certList.list));
    memdelete(seal.esealInfo.property.createDate);
    memdelete(seal.esealInfo.property.validStart);
    memdelete(seal.esealInfo.property.validEnd);

    memdelete(seal.signInfo.cert);
    memdelete(seal.signInfo.signatureAlgorighm);
    memdelete(seal.signInfo.signData);
}

void	releaseToSignTag(TBS_Sign_t toSign)
{
    releaseSealTag(toSign.esealInfo);

    memdelete(toSign.cert);
    memdelete(toSign.dataHash);
    memdelete(toSign.propertyInfo);
    memdelete(toSign.signatureAlgorithm);
    memdelete(toSign.timeInfo);
}

void	releaseSignatureTag(SES_Signature_t signature)
{
    releaseToSignTag(signature.toSign);
    memdelete(signature.signature);
}

void releaseSealPtr(SESeal_t* pSeal)
{
    if (!pSeal) return;

    releaseSealTag(*pSeal);

    SAFE_DELETE(pSeal);
}

void	releaseToSignPtr(TBS_Sign_t* pToSign)
{
    if (!pToSign) return;

    releaseToSignTag(*pToSign);

    SAFE_DELETE(pToSign);
}

void	releaseSignaturePtr(SES_Signature_t* pSignature)
{
    if (!pSignature) return;

    releaseSignatureTag(*pSignature);

    SAFE_DELETE(pSignature);
}


std::string COFD_Utils::GetSeedSHA256()
{
    int nLen = 32;
    unsigned char szSHA256[32] = { 0 };

    const char* pHashMethod = "sha256";
    COFD_BaseHandler::Digest((unsigned char*)FX_ENCRYPT_ARCFOUR_KEY, strlen(FX_ENCRYPT_ARCFOUR_KEY), szSHA256, &nLen, pHashMethod);

    std::string str;
    str.assign((char*)szSHA256, nLen);

    return str;
}

std::string COFD_Utils::GetSeedSHA256Reverse()
{
    int nLen = 32;
    unsigned char szSHA256[32] = { 0 };

    const char* pHashMethod = "sha256";
    COFD_BaseHandler::Digest((unsigned char*)FX_ENCRYPT_ARCFOUR_KEY, strlen(FX_ENCRYPT_ARCFOUR_KEY), szSHA256, &nLen, pHashMethod);

    std::string str;
    str.assign((char*)szSHA256, nLen);

    std::string strReverse;
    strReverse.assign(str.rbegin(), str.rend());

    return strReverse;
}

bool COFD_Utils::IsCertHasSignUsage(X509* x509)
{
    if (!x509)
    {
        return false;
    }

    InitOpenSSL();

    //KU_DIGITAL_SIGNATURE    签名目的。
    //KU_DATA_ENCIPHERMENT  加密目的。

    ASN1_BIT_STRING* pUsageStr = (ASN1_BIT_STRING*)X509_get_ext_d2i(x509, NID_key_usage, NULL, NULL);
    if (pUsageStr)
    {
        unsigned short usage = pUsageStr->data[0];
        if(pUsageStr->length > 1)
        {
            usage |= pUsageStr->data[1] << 8;
        }
        if (usage & KU_DIGITAL_SIGNATURE)
        {
           //Digital Signature
            return true;
        }
        return false;
        /*
        if (usage & KU_NON_REPUDIATION)
        {
            //Non-Repudiation
        }
        if (usage & KU_KEY_ENCIPHERMENT)
        {
            //Key Encipherment
        }
        if (usage & KU_DATA_ENCIPHERMENT)
        {
            //Data  Encipherment
        }
        if (usage & KU_KEY_AGREEMENT)
        {
            //Key  Agreement
        }
        if (usage & KU_KEY_CERT_SIGN)
        {
            //Certificate Signature
        }
        if (usage & KU_CRL_SIGN)
        {
            //CRL Signature
        }
        */
    }

    return false;
}

bool COFD_Utils::IsCertHasSignUsage(std::string strPFXData, std::string strPasswd)
{
    InitOpenSSL();

    BIO* bo = BIO_new_mem_buf((char*)strPFXData.data(), strPFXData.length());
    if (!bo) return false;

    //Extracting client certificate & private key from .pfx or .p12 file
    PKCS12 *p12 = d2i_PKCS12_bio(bo, NULL);
    BIO_free(bo);

    if (!p12)
    {
        printf("Extracting client certificate & private key failed.");
        return false;
    }

    bool bRet = false;
    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;

    STACK_OF(X509) *ca = NULL;
    int result = PKCS12_parse(p12, strPasswd.data(), &pkey, &x509, &ca);
    if (result)
    {
        bRet = IsCertHasSignUsage(x509);
    }

    X509_free(x509);
    PKCS12_free(p12);
    EVP_PKEY_free(pkey);
    sk_X509_free(ca);
    EVP_cleanup();

    return bRet;
}

//初始化OpenSSL
void COFD_Utils::InitOpenSSL()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();

#if (OPENSSL_VERSION_NUMBER<0x10100000L)
    CRYPTO_malloc_init();
#endif
    ERR_load_crypto_strings();
}

void COFD_Utils::HexEncode(const char* pOriginBuf, FX_DWORD nOriginLen, char* pCryptedBuf)
{
    char* pCB = pCryptedBuf;

    for (FX_DWORD i = 0; i < nOriginLen; i++)
    {
        int b = (*pOriginBuf & 0xF0) >> 4;
        *pCB++ = (b <= 9) ? b + '0' : (b - 10) + 'A';
        b = *pOriginBuf & 0x0F;
        *pCB++ = (b <= 9) ? b + '0' : (b - 10) + 'A';
        pOriginBuf++;
    }
}

void COFD_Utils::HexDecode(const char* pOriginBuf, FX_DWORD nOriginLen, char* pDeCryptedBuf)
{
    char* pDCB = pDeCryptedBuf;

    for (FX_DWORD i = 0; i < nOriginLen; i += 2)
    {
        char c = 0;
        for (FX_DWORD j = 0; j < 2; j++)
        {
            const char* p = pOriginBuf + i + j;

            if ('0' <= *p && *p <= '9')
                c = c * 16 + (*p - '0');
            else if ('a' <= *p && *p <= 'f')
                c = c * 16 + (*p - 'a' + 10);
            else if ('A' <= *p && *p <= 'F')
                c = c * 16 + (*p - 'A' + 10);
            else return;
        }

        *pDCB++ = c;
    }
}

void	COFD_Utils::Base64_Encode(const unsigned char* Data, int dataLen, unsigned char* pOutData, int* piOutDataLen)
{
    const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

    int nOutLen = 0;
    int LineLength=0;
    unsigned char Tmp[4] = { 0 };

    std::string strEncode;

    for(int i = 0; i < (int)(dataLen / 3); i++)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        Tmp[3] = *Data++;

        strEncode += EncodeTable[Tmp[1] >> 2];
        strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode += EncodeTable[Tmp[3] & 0x3F];
        LineLength += 4;
    }

    int Mod = dataLen % 3;
    if(Mod == 1)
    {
        Tmp[1] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
        strEncode += "==";
    }
    else if(Mod == 2)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
        strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
        strEncode += "=";
    }

    nOutLen = strEncode.length();
    if (pOutData)
    {
        memcpy(pOutData, strEncode.data(), nOutLen);
    }
    else
    {
        *piOutDataLen = nOutLen;
    }
}

void	COFD_Utils::Base64_Decode(const unsigned char* Data, int dataLen, unsigned char* pOutData, int* nOutDataLen)
{
    const unsigned char DecodeTable[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        62, // '+'
        0, 0, 0,
        63, // '/'
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
        0, 0, 0, 0, 0, 0,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };

    int i= 0;
    int nOutLen = 0;
    int nValue = '\0';
    std::string strDecode;
    while (i < dataLen)
    {
        if ( (*Data>='A' && *Data<='Z')
             || (*Data>='a' && *Data<='z')
             || (*Data>='0' && *Data<='9')
             || (*Data=='+')
             || (*Data=='/'))
        {
            nValue = DecodeTable[*Data++] << 18;
            nValue += DecodeTable[*Data++] << 12;
            strDecode += (nValue & 0x00FF0000) >> 16;
            nOutLen++;
            if (*Data != '=')
            {
                nValue += DecodeTable[*Data++] << 6;
                strDecode+=(nValue & 0x0000FF00) >> 8;
                nOutLen++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[*Data++];
                    strDecode+=nValue & 0x000000FF;
                    nOutLen++;
                }
            }
            i += 4;
        }
        else
        {
            Data++;
            i++;
        }
    }

    if (pOutData)
    {
        memcpy(pOutData, strDecode.data(), nOutLen);
    }
    else
    {
        *nOutDataLen = nOutLen;
    }
}

bool COFD_Utils::GetTime_t(std::string strUTCTime, time_t* t)
{
    //时间转换
    struct tm tmp_time;
    strptime(strUTCTime.data(),"%y%m%d%H%M%Sz", &tmp_time);
    *t = mktime(&tmp_time);

    //时区处理
    struct timezone tz;
    gettimeofday(NULL, &tz);
    *t -= tz.tz_minuteswest * 60;

    return TRUE;
}

std::string	COFD_Utils::FormatLocalTimeString(std::string strUTCTime)
{
    std::string strTime;

    time_t t;
    if (GetTime_t(strUTCTime, &t))
    {
        strTime = FormatLocalTimeString(&t);
    }

    return strTime;
}

std::string COFD_Utils::FormatLocalTimeString(time_t* t)
{
    std::string strResult;
    tm* t0 = localtime(t);
    {
        char szbuf[256] = { 0 };
        int nLen = strftime(szbuf, 256, "%Y-%m-%d %H:%M:%S", t0);

        strResult.assign(szbuf, nLen);
    }

    return strResult;
}

/**@brief utf8 encode */
std::string	COFD_Utils::GetCertIssue(std::string strCerFileData)
{
    InitOpenSSL();

    /**@brief hint
     * d2i_X509 会使传入的指针发生偏移（移动至内存尾部）。
     * 这里使用副本传入，以防止strCerFileData.data()地址被更改。
    */
    std::string strCertIssue;

    int cbCerData = strCerFileData.length();
    const char* pbCerData_x = strCerFileData.data();
    X509* x509 = d2i_X509(NULL, (const unsigned char**)&pbCerData_x, cbCerData);
    if (x509)
    {
        X509_NAME* pIsseurName = X509_get_issuer_name(x509);
        if (pIsseurName)
        {
            char szIssuerName[512] = { 0 };
            int nNameLen = X509_NAME_get_text_by_NID(pIsseurName, NID_commonName, szIssuerName, 512);
            if (-1 == nNameLen) return strCertIssue;

            strCertIssue.assign(szIssuerName, nNameLen);
        }
    }

    X509_free(x509);

    return strCertIssue;
}

std::string COFD_Utils::GetSubjectName(X509* cert)
{
    InitOpenSSL();

    std::string str;
    if (!cert) return str;

    X509_NAME *pSubName = X509_get_subject_name(cert);
    if (!pSubName) return str;

    struct subj_step_param
    {
        const char*  csPrefix;
        int               nid;
    };

    subj_step_param lstNID[] = {
        {SN_commonName, NID_commonName},
        { SN_organizationName, NID_organizationName},
        {SN_localityName, NID_localityName},
        { SN_organizationalUnitName, NID_organizationalUnitName},
        { SN_countryName, NID_countryName},
    };

    int nCount = (sizeof(lstNID) / sizeof(subj_step_param));
    for (int i = 0; i < nCount; i++)
    {
        subj_step_param& param = lstNID[i];

        char szBuf[256] = {0};
        int nLen = X509_NAME_get_text_by_NID(pSubName, param.nid, szBuf, 256);
        if (nLen > 0)
        {
            if (!str.empty())
            {
                str.append(",");
            }
            str.append(param.csPrefix);
            str.append("=");
            str.append(szBuf);
        }
    }

    return str;
}
