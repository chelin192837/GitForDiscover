#include "../include/oes_dll.h"
#include "../include/oes.h"
#include "../include/OES_LibraryManage.h"
#include "../include/oes_error.h"

/**
* @brief 返回签章算法提供者信息 [Required]
* @param[out]     puchName      名称（UTF-8编码），当为NULL时，通过piNameLen给出长度
* @param[out/in]  piNameLen     名称长度(有效字符长度，不包括'\0')
* @param[out]     puchCompany   公司名称（UTF-8编码），当为NULL时，通过piCompanyLen给出长度
* @param[out/in]  piCompanyLen  公司名称长度(有效字符长度，不包括'\0')
* @param[out]     puchVersion   版本（UTF-8编码），当为NULL时，通过piVersionLen给出长度
* @param[out/in]  piVersionLen  版本长度(有效字符长度，不包括'\0')
* @param[out]     puchExtend    扩展信息（UTF-8编码），当为NULL时，通过piExtendLen给出长度
* @param[out/in]  piExtendLen   扩展信息长度(有效字符长度，不包括'\0')
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetProviderInfo(unsigned char* puchName, int* piNameLen,
    unsigned char* puchCompany, int* piCompanyLen,
    unsigned char* puchVersion, int* piVersionLen,
    unsigned char* puchExtend, int* piExtendLen)
{
    if (!piCompanyLen || !piVersionLen || !piExtendLen )
        return OES_ERROR_INVALID_PTR_NULL;

    //////////////////////////////////////////////////////////////////////////
    //Name
    std::string strProviderName = OES_PROVIDER_NAME;
    if (puchName)
    {
        int nSize = strProviderName.length();
        if(*piNameLen < nSize)
        {
            return OES_ERROR_BUFFER_NOT_ENUGH;
        }

        memset(puchName, 0, *piNameLen);
        memcpy(puchName, strProviderName.data(), strProviderName.length());
    }
    *piNameLen = strProviderName.length();

    //////////////////////////////////////////////////////////////////////////
    //Company
    std::string strProviderCompany = OES_PROVIDER_COMPANY;
    if (puchCompany)
    {
        int nSize = strProviderCompany.length();
        if(*piCompanyLen < nSize)
        {
            return OES_ERROR_BUFFER_NOT_ENUGH;
        }
        memcpy(puchCompany, strProviderCompany.data(), strProviderCompany.length());
    }
    *piCompanyLen = strProviderCompany.length();

    //////////////////////////////////////////////////////////////////////////
    //Version
    std::string strProviderVersion = OES_PROVIDER_VERSION;
    if (puchVersion)
    {
        int nSize = strProviderVersion.length();
        if(*piVersionLen < nSize)
        {
            return OES_ERROR_BUFFER_NOT_ENUGH;
        }
        memcpy(puchVersion, strProviderVersion.data(), strProviderVersion.length());
    }
    *piVersionLen = strProviderVersion.length();

    //////////////////////////////////////////////////////////////////////////
    //Extend Information
    if (!puchExtend)
    {
        piExtendLen = 0;
    }

    return OES_ERROR_OK;
}

/**
* @brief 获取用于签章列表，该函数可用来进行Name到ID的转换  [Required]
* @param[out]     puchSealListData            印章列表数据（UTF-8编码），当为NULL时，通过piSealListDataLen给出长度
*                                             形如 ID1\0Name1\0ID2\0Name2\0\0
* @param[out/in]  piSealListDataLen           印章列表数据长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetSealList(unsigned char* puchSealListData, int* piSealListDataLen)
{
    if (!piSealListDataLen) return OES_ERROR_INVALID_PTR_NULL;

    COES_LibraryManage manage;
    if (!manage.Load()) return OES_ERROR_SEALLIST_EMPTY;

    int nCount = manage.CountItem();
    if (nCount > 0)
    {
        int nMemSize = 0;
        FX_LPBYTE pMemBuffer = NULL;
        for (int i = 0; i < nCount; i++)
        {
            CXML_Element* pElem = manage.GetItem(i);
            if (!pElem) continue;

            CFX_WideString wsTagUID = OES_LIBRARY_SEAL_UID;
            CFX_WideString wsTagName = OES_LIBRARY_SEAL_NAME;

            CFX_WideString wsUID = pElem->GetAttrValue(wsTagUID.UTF8Encode());
            CFX_WideString wsName = pElem->GetAttrValue(wsTagName.UTF8Encode());

            CFX_ByteString uid = wsUID.UTF8Encode();
            CFX_ByteString name = wsName.UTF8Encode();

            int nMemNeed = (uid.GetLength() + 1) +
                           (name.GetLength() + 1);

            if (pMemBuffer == NULL)
            {
                pMemBuffer = FX_Alloc(FX_BYTE, nMemNeed);
            }
            else
            {
                pMemBuffer = FX_Realloc(FX_BYTE, pMemBuffer, nMemSize + nMemNeed);
            }

            FX_LPSTR pstrUID = uid.GetBuffer(uid.GetLength());
            FX_LPSTR pstrName = name.GetBuffer(name.GetLength());

            int nOffset = nMemSize;
            int nLenPart1 = uid.GetLength() + 1;
            int nLenPart2 = name.GetLength() + 1;

            memset(pMemBuffer + nOffset, '\0', nMemNeed);
            memcpy(pMemBuffer + nOffset, pstrUID, uid.GetLength());

            nOffset += nLenPart1;
            memcpy(pMemBuffer + nOffset, pstrName, name.GetLength());

            uid.ReleaseBuffer();
            name.ReleaseBuffer();

            nMemSize += nMemNeed;
        }

        if (puchSealListData)
        {
            if (*piSealListDataLen >= nMemSize)
            {
                memcpy(puchSealListData, pMemBuffer, nMemSize);
            }
            else
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
        }
        else
        {
            *piSealListDataLen = nMemSize;
        }

        if (pMemBuffer)
        {
            FX_Free(pMemBuffer);
            return OES_ERROR_OK;
        }

        return OES_ERROR_SEALLIST_EMPTY;
    }

    if (!puchSealListData)
    {
        piSealListDataLen = 0;
    }

    return OES_ERROR_SEALLIST_EMPTY;
}

/**
* @brief 获取用于签章的印模结构（含印模图像、校验证书和其他数据,符合国密标准）  [Required]
* @param[in]      puchSealId              印章标识或名称（字符串）
* @param[in]      iSealIdLen              印章标识或名称长度
* @param[out]     puchSealData            印章数据（符合国密标准），当为NULL时，通过piSealListDataLen给出长度
* @param[out/in]  piSealDataLen           印章数据长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetSeal(unsigned char* puchSealId,int iSealIdLen,
    unsigned char* puchSealData,int* piSealDataLen)
{
    if (!puchSealId ||
        !iSealIdLen ||
        !piSealDataLen)
    {
        return OES_ERROR_INVALID_PTR_NULL;
    }

    COES_LibraryManage manage;
    if (!manage.Load()) return OES_ERROR_SEALLIST_EMPTY;

    int nCount = manage.CountItem();
    if (nCount == 0) return OES_ERROR_SEALLIST_EMPTY;

    CFX_ByteString csSealID(puchSealId, iSealIdLen);

    for (int i = 0; i < nCount; i++)
    {
        CXML_Element* pElem = manage.GetItem(i);
        if (!pElem) continue;

        CFX_WideString wsTagUID = OES_LIBRARY_SEAL_UID;
        CFX_WideString wsTagName = OES_LIBRARY_SEAL_NAME;
        CFX_WideString wsTagPath = OES_LIBRARY_SEAL_PATH;

        CFX_WideString wsUID = pElem->GetAttrValue(wsTagUID.UTF8Encode());
        CFX_WideString wsName = pElem->GetAttrValue(wsTagName.UTF8Encode());

        CFX_ByteString id = wsUID.UTF8Encode();
        CFX_ByteString name = wsName.UTF8Encode();
        if (id.EqualNoCase(csSealID) || name.EqualNoCase(csSealID))
        {
            CFX_WideString wsDir = pElem->GetAttrValue(wsTagPath.UTF8Encode());

            CFX_WideString wsSealPath;
            wsSealPath.Format(_T("/.oes_lib/htfoxit/%s/esl.dat"), (FX_LPCWSTR)wsDir);

            IFX_FileRead* pFileRead = FX_CreateFileRead(wsSealPath);
            if (!pFileRead) return OES_ERROR_READ_DATA_DENIED;

            if (puchSealData)
            {
                if (*piSealDataLen >= pFileRead->GetSize())
                {
                    std::string strKey = COFD_Utils::GetSeedSHA256Reverse();
                    pFileRead->ReadBlock(puchSealData, pFileRead->GetSize());
                    CRYPT_ArcFourCryptBlock(puchSealData, pFileRead->GetSize(), (FX_LPCBYTE)strKey.data(), strKey.length());
                }
                else
                {
                    pFileRead->Release();
                    return OES_ERROR_BUFFER_NOT_ENUGH;
                }
            }
            else
            {
                *piSealDataLen = pFileRead->GetSize();
            }
            pFileRead->Release();

            return OES_ERROR_OK;
        }
    }

    if (!puchSealData)
    {
        piSealDataLen = 0;
    }

    return OES_ERROR_SEAL_INVALID_ID;
}

/**@brief 获取私钥证书，pfx内容.
* @param[in]      puchSealId              印章标识或名称（字符串）
* @param[in]      iSealIdLen              印章标识或名称长度
* @param[out]     puchKeyData            印章数据（符合国密标准），当为NULL时，通过piSealListDataLen给出长度
* @param[out/in]  piKeyDataLen           印章数据长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
static int OES_GetKey(unsigned char* puchSealId,int iSealIdLen,
    unsigned char* puchKeyData,int* piKeyDataLen)
{
    if (!puchSealId ||
        !iSealIdLen ||
        !piKeyDataLen)
    {
        return OES_ERROR_INVALID_PTR_NULL;
    }

    COES_LibraryManage manage;
    if (!manage.Load()) return OES_ERROR_SEALLIST_EMPTY;

    int nCount = manage.CountItem();
    if (nCount == 0) return OES_ERROR_SEALLIST_EMPTY;

    CFX_ByteString csSealID(puchSealId, iSealIdLen);

    for (int i = 0; i < nCount; i++)
    {
        CXML_Element* pElem = manage.GetItem(i);
        if (!pElem) continue;

        CFX_WideString wsTagUID = OES_LIBRARY_SEAL_UID;
        CFX_WideString wsTagName = OES_LIBRARY_SEAL_NAME;
        CFX_WideString wsTagPath = OES_LIBRARY_SEAL_PATH;

        CFX_WideString wsUID = pElem->GetAttrValue(wsTagUID.UTF8Encode());
        CFX_WideString wsName = pElem->GetAttrValue(wsTagName.UTF8Encode());

        CFX_ByteString id = wsUID.UTF8Encode();
        CFX_ByteString name = wsName.UTF8Encode();
        if (id.EqualNoCase(csSealID) || name.EqualNoCase(csSealID))
        {
            CFX_WideString wsDir = pElem->GetAttrValue(wsTagPath.UTF8Encode());

            CFX_WideString wsSealPath;
            wsSealPath.Format(_T("/.oes_lib/htfoxit/%s/pfx.dat"), (FX_LPCWSTR)wsDir);

            IFX_FileRead* pFileRead = FX_CreateFileRead(wsSealPath);
            if (!pFileRead) return OES_ERROR_READ_DATA_DENIED;

            if (puchKeyData)
            {
                if (*piKeyDataLen >= pFileRead->GetSize())
                {
                    std::string strKey = COFD_Utils::GetSeedSHA256Reverse();
                    pFileRead->ReadBlock(puchKeyData, pFileRead->GetSize());
                    CRYPT_ArcFourCryptBlock(puchKeyData, pFileRead->GetSize(), (FX_LPCBYTE)strKey.data(), strKey.length());
                }
                else
                {
                    pFileRead->Release();
                    return OES_ERROR_BUFFER_NOT_ENUGH;
                }
            }
            else
            {
                *piKeyDataLen = pFileRead->GetSize();
            }
            pFileRead->Release();

            return OES_ERROR_OK;
        }
    }

    return OES_ERROR_SEAL_INVALID_ID;
}

/**
* @brief 获取用于签章的印模结构（含印模图像、校验证书和其他数据,符合国密标准）  [Required]
* @param[in]	   puchSealData           印章数据（符合国密标准）
* @param[in]	   iSealDataLen           印章数据长度
* @param[out]     puchSealId             头信息-印章标识（UTF-8编码），当为NULL时，通过piSealIdLen给出长度
* @param[out/in]  piSealIdLen            头信息-印章标识长度
* @param[out]     puchVersion            头信息-版本（UTF-8编码），当为NULL时，通过piVersionLen给出长度
* @param[out/in]  piVersionLen           头信息-版本数据长度
* @param[out]     puchVenderId           头信息-厂商标识（UTF-8编码），当为NULL时，通过piVenderIdLen给出长度
* @param[out/in]  piVenderIdLen          头信息-厂商标识长度
* @param[out]     puchSealType           印章信息-印章类型（UTF-8编码），当为NULL时，通过piSealTypeLen给出长度
* @param[out/in]  piSealTypeLen          印章信息-印章类型长度
* @param[out]     puchSealName           印章信息-印章名称（UTF-8编码），当为NULL时，通过piSealNameLen给出长度
* @param[out/in]  piSealNameLen          印章信息-印章名称长度
* @param[out]     puchCertInfo           印章信息-证书列表信息（UTF-8编码），当为NULL时，通过piCertInfoLen给出长度
* @param[out/in]  piCertInfoLen          印章信息-证书列表信息长度
* @param[out]     puchValidStart         印章信息-有效起始时间（UTF-8编码，形如yyyy-MM-dd），当为NULL时，通过piValidStartLen给出长度
* @param[out/in]  piValidStartLen        印章信息-有效起始时间长度
* @param[out]     puchValidEnd           印章信息-有效结束时间（UTF-8编码，形如yyyy-MM-dd），当为NULL时，通过piValidEndLen给出长度
* @param[out/in]  piValidEndLen          印章信息-有效结束长度
* @param[out]     puchCreateDate         印章信息-制作日期（UTF-8编码，形如yyyy-MM-dd），当为NULL时，通过piCreateDateLen给出长度
* @param[out/in]  piCreateDateLen        印章信息-制作日期长度
* @param[out]     puchSignerName         签名信息-制章人（UTF-8编码），当为NULL时，通过piSignerNameLen给出长度
* @param[out/in]  piSignerNameLen        签名信息-制章人长度
* @param[out]     puchSignMethod         签名信息-制章签名方法（UTF-8编码），当为NULL时，通过piSignMethodLen给出长度
* @param[out/in]  piSignMethodLen        签名信息-制章签名方法长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetSealInfo(unsigned char* puchSealData,int iSealDataLen,
    unsigned char* puchSealId,int* piSealIdLen,
    unsigned char* puchVersion,int* piVersionLen,
    unsigned char* puchVenderId,int* piVenderIdLen,
    unsigned char* puchSealType,int* piSealTypeLen,
    unsigned char* puchSealName,int* piSealNameLen,
    unsigned char* puchCertInfo,int* piCertInfoLen,
    unsigned char* puchValidStart,int* piValidStartLen,
    unsigned char* puchValidEnd,int* piValidEndLen,
    unsigned char* puchCreateDate,int* piCreateDateLen,
    unsigned char* puchSignerName,int* piSignerNameLen,
    unsigned char* puchSignMethod,int* piSignMethodLen)
{
    if (!puchSealData	 ||
        !piSealIdLen     ||
        !piVersionLen	 ||
        !piVenderIdLen	 ||
        !piSealTypeLen	 ||
        !piSealNameLen	 ||
        !piCertInfoLen	 ||
        !piValidStartLen ||
        !piValidEndLen	 ||
        !piCreateDateLen ||
        !piSignerNameLen ||
        !piSignMethodLen)
    {
        return OES_ERROR_INVALID_PTR_NULL;
    }

    SESeal* pSeal = NULL;
    asn_dec_rval_t err = ber_decode(NULL, &asn_DEF_SESeal, (void**)&pSeal, puchSealData, iSealDataLen);

    if (err.code != RC_OK) return OES_ERROR_SEAL_INVALID_DATA;

    SES_Header_t& sealHead = pSeal->esealInfo.header;
    SES_ESPropertyInfo_t& sealProp = pSeal->esealInfo.property;
    SES_SignInfo_t& sealSignInfo = pSeal->signInfo;

    //////////////////////////////////////////////////////////////////////////
    //Head - ID
    {
        int nSize = sealHead.id.size;
        uint8_t* pBuffer = sealHead.id.buf;
        if (puchSealId)
        {
            if(*piSealIdLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchSealId, pBuffer, nSize);
        }
        *piSealIdLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Head - Version
    {
        char szVersion[32] = { 0 };
        int nVersion = sealHead.version;
        sprintf(szVersion, "%d", nVersion);
        std::string strVersion = szVersion;

        int nSize = strVersion.length();
        const char* pBuffer = strVersion.data();
        if (puchVersion)
        {
            if(*piVersionLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchVersion, pBuffer, nSize);
        }
        *piVersionLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Head - VenderID
    {
        int nSize = sealHead.vid.size;
        uint8_t* pBuffer = sealHead.vid.buf;
        if (puchVenderId)
        {
            if(*piVenderIdLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchVenderId, pBuffer, nSize);
        }
        *piVenderIdLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Property - Type
    {
        char szType[32] = { 0 };
        int nType = sealProp.type;
        sprintf(szType, "%d", nType);
        std::string strType = szType;

        int nSize = strType.length();
        const char* pBuffer = strType.data();

        if (puchSealType)
        {
            if(*piSealTypeLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchSealType, pBuffer, nSize);
        }
        *piSealTypeLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Property - Name
    {
        int nSize = sealProp.name.size;
        uint8_t* pBuffer = sealProp.name.buf;

        if (puchSealName)
        {
            if(*piSealNameLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchSealName, pBuffer, nSize);
        }
        *piSealNameLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Property - CertInfo
    {
        std::string strCertInfo = OES_ESEAL_PROPS_CERT_INFO;

        int nSize = strCertInfo.length();
        const char* pBuffer = strCertInfo.data();
        if (puchCertInfo)
        {
            if(*piCertInfoLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchCertInfo, pBuffer, nSize);
        }
        *piCertInfoLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Property - StartTime
    {
        std::string strStartTime;
        strStartTime.assign((char*)(sealProp.validStart.buf), sealProp.validStart.size);

        strStartTime = COFD_Utils::FormatLocalTimeString(strStartTime);

        int nSize = strStartTime.length();
        const char* pBuffer = strStartTime.data();
        if (puchValidStart)
        {
            if(*piValidStartLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchValidStart, pBuffer, nSize);
        }
        *piValidStartLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Property - EndTime
    {
        std::string strEndTime;
        strEndTime.assign((char*)(sealProp.validEnd.buf), sealProp.validEnd.size);

        strEndTime = COFD_Utils::FormatLocalTimeString(strEndTime);

        int nSize = strEndTime.length();
        const char* pBuffer = strEndTime.data();
        if (puchValidEnd)
        {
            if(*piValidEndLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchValidEnd, pBuffer, nSize);
        }
        *piValidEndLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //Property - EndTime
    {
        std::string strCreateTime;
        strCreateTime.assign((char*)(sealProp.createDate.buf), sealProp.validEnd.size);

        strCreateTime = COFD_Utils::FormatLocalTimeString(strCreateTime);

        int nSize = strCreateTime.length();
        const char* pBuffer = strCreateTime.data();
        if (puchCreateDate)
        {
            if(*piCreateDateLen < nSize)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchCreateDate, pBuffer, nSize);
        }
        *piCreateDateLen = nSize;
    }

    //////////////////////////////////////////////////////////////////////////
    //SignInfo - Signer name
    {
        std::string strCerFileData;
        strCerFileData.assign((char*)sealSignInfo.cert.buf, sealSignInfo.cert.size);
        std::string strIssue = COFD_Utils::GetCertIssue(strCerFileData);
        if(!strIssue.empty())
        {
            if (puchSignerName)
            {
                int nSize = strIssue.length();
                if(*piSignerNameLen < nSize)
                {
                    return OES_ERROR_BUFFER_NOT_ENUGH;
                }
                memcpy(puchSignerName, strIssue.data(), strIssue.length());
            }
            *piSignerNameLen = strIssue.length();
        }
        else
        {
            if (!puchSignerName)
            {
                *piSignerNameLen = 0;
            }

            return OES_ERROR_SEAL_INVALID_DATA;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //SignInfo - Sign method
    {
        if (puchSignMethod)
        {
            if(*piSignMethodLen < sealSignInfo.signatureAlgorighm.size)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
            memcpy(puchSignMethod, sealSignInfo.signatureAlgorighm.buf, sealSignInfo.signatureAlgorighm.size);
        }
        *piSignMethodLen = sealSignInfo.signatureAlgorighm.size;
    }

    return OES_ERROR_OK;
}

/**
* @brief 获取签名时间（时间戳或明文形式） [Required]
* @param[out]     puchSignDateTime          签名时间（字符时用UTF-8编码，形如yyyy-MM-dd hh:mm:ss,时间戳时二进制值），当为NULL时，通过piSignDateTimeLen给出长度
* @param[out/in]  piSignDateTimeLen         签名时间长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetSignDateTime(unsigned char* puchSignDateTime,int* piSignDateTimeLen)
{
    if (!piSignDateTimeLen) return OES_ERROR_INVALID_PTR_NULL;

    time_t t = time(NULL);
    tm* local_t = localtime(&t);

    char szTimeBuf[256] = { 0 };
    strftime(szTimeBuf, 256, "%Y-%m-%d %H:%M:%S", local_t);

    std::string strTime = szTimeBuf;
    if (puchSignDateTime)
    {
        memcpy(puchSignDateTime, strTime.data(),strTime.length());
    }
    else
    {
        *piSignDateTimeLen = strTime.length();
    }

    return OES_ERROR_OK;
}

/**
* @brief 获取签名算法标识 [Required]
* @param[out]     puchSignMethod            签名算法（UTF-8编码），当为NULL时，通过piSignMethodLen给出长度
* @param[out/in]  piSignMethodLen           签名算法长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetSignMethod(unsigned char* puchSignMethod,int* piSignMethodLen)
{
    if (!piSignMethodLen) return OES_ERROR_INVALID_PTR_NULL;

    std::string strSignMethod("1.2.840.113549.1.1.11"); /* = szOID_RSA_SHA256RSA*/
    if (puchSignMethod)
    {
        memcpy(puchSignMethod, strSignMethod.data(), strSignMethod.length());
    }
    else
    {
        *piSignMethodLen = strSignMethod.length();
    }

    return OES_ERROR_OK;
}

/**
* @brief 获取摘要算法标识 [Required]
* @param[out]     puchDigestMethod          摘要算法（UTF-8编码），当为NULL时，通过piDigestMethodLen给出长度
* @param[out/in]  piDigestMethodLen         摘要算法长度
* @return 调用成功返回OES_OK，否则是错误代码
*/
OES_API int OES_CALL    OES_GetDigestMethod(unsigned char* puchDigestMethod, int* piDigestMethodLen)
{
    if (!piDigestMethodLen) return OES_ERROR_INVALID_PTR_NULL;

    std::string strDigestMethod = OES_DIGEST_ALGORITHM_SHA256;

    if (puchDigestMethod)
    {
        memcpy(puchDigestMethod, strDigestMethod.data(), strDigestMethod.length());
    }
    else
    {
        *piDigestMethodLen = strDigestMethod.length();
    }

    return OES_ERROR_OK;
}

/**
* @brief 代理计算摘要  [Required]
* @param[in]      puchData                  待摘要的数据
* @param[in]      iDataLen                  待摘要的数据长度
* @param[in]      puchDigestMethod          摘要算法
* @param[in]      iDigestMethodLen          摘要算法长度
* @param[out]     puchDigestValue           摘要值，当为NULL时，通过piDigestValueLen给出长度
* @param[out/in]  piDigestValueLen          摘要值长度
* @return 调用成功返回OES_OK，否则是错误代码，可调用OES_GetErrMessage()获取详细信息。
*/
OES_API unsigned long OES_CALL    OES_Digest(unsigned char* puchData,int piDataLen,
    unsigned char* puchDigestMethod,int piDigestMethodLen,
    unsigned char* puchDigestValue,int* piDigestValueLen)
{
    if (!piDataLen ||
        !piDigestMethodLen ||
        !piDigestValueLen)
    {
        return OES_ERROR_INVALID_PTR_NULL;
    }

    std::string strCurDigest;
    strCurDigest.assign((char*)puchDigestMethod, piDigestMethodLen);
    if (strCurDigest.empty()) return OES_ERROR_DIGEST_ERROR;

    std::string strSupportDigest = OES_DIGEST_ALGORITHM_SHA256;
    if (strSupportDigest.compare(strCurDigest) != 0)
    {
        //The digest is invalid or not support.
        return OES_ERROR_DIGEST_NOTSUPPORT;
    }

    IHTFX_SignatureHandler* pSignatureHandler = IHTFX_SignatureHandler::Create();
    if (pSignatureHandler)
    {
        err_t ret = pSignatureHandler->Digest(puchData, piDataLen, puchDigestValue, piDigestValueLen);
        pSignatureHandler->Release();

        return ret;
    }

    return OES_ERROR_DIGEST_ERROR;
}

/**
* @brief 代理计算签名,如果计算前需要输入密码，插件实现者需要提供输入界面  [Required]
* @param[in]      puchSealId              印章标识
* @param[in]      iSealIdLen              印章标识长度
* @param[in]      puchDocProperty         文档信息，固定为Signature.xml的绝对路径
* @param[in]      iDocPropertyLen         文档信息长度
* @param[in]      puchDigestData          摘要数据
* @param[in]      iDigestDataLen          摘要数据长度
* @param[in]      puchSignMethod          签名算法
* @param[in]      iSignMethodLen          签名算法长度
* @param[in]      puchSignDateTime        签名时间
* @param[in]      iSignDateTimeLen        签名时间长度
* @param[out]     puchSignValue           签名值（符合国密标准），当为NULL时，通过piSignValueLen给出长度
* @param[out/in]  piSignValueLen          签名值长度
* @return 调用成功返回OES_OK，否则是错误代码，可调用OES_GetErrMessage()获取详细信息。
*/
OES_API unsigned long OES_CALL    OES_Sign(unsigned char* puchSealId,int iSealIdLen,
    unsigned char* puchDocProperty,int iDocPropertyLen,
    unsigned char* puchDigestData,int iDigestDataLen,
    unsigned char* puchSignMethod,int iSignMethodLen,
    unsigned char* puchSignDateTime,int iSignDateTimeLen,
    unsigned char* puchSignValue,int* piSignValueLen)
{
    std::string strPFXKey;
    std::string strSealData;
    const char* passwd = "123456";

    int nKeyLen = 0;
    if (OES_GetKey(puchSealId, iSealIdLen, NULL, &nKeyLen) == OES_ERROR_OK)
    {
        unsigned char* pKeyData = new unsigned char[nKeyLen];
        memset(pKeyData, 0, nKeyLen);

        if (OES_GetKey(puchSealId, iSealIdLen, pKeyData, &nKeyLen) == OES_ERROR_OK)
        {
            strPFXKey.assign((char*)pKeyData, nKeyLen);
        }
        delete[] pKeyData;
    }

    if (strPFXKey.empty()) return OES_ERROR_SIGN_ERROR_KEY;

    int nSealDataLen = 0;
    if (OES_GetSeal(puchSealId, iSealIdLen, NULL, &nSealDataLen) == OES_ERROR_OK)
    {
        unsigned char* pBuf = new unsigned char[nSealDataLen];
        memset(pBuf, 0, nSealDataLen);

        if (OES_GetSeal(puchSealId, iSealIdLen, pBuf, &nSealDataLen) == OES_ERROR_OK)
        {
            strSealData.assign((char*)pBuf, nSealDataLen);
        }
        delete[] pBuf;
    }

    if (strSealData.empty()) return OES_ERROR_SEAL_INVALID_DATA;

    IHTFX_SignatureHandler* pSignHandler = IHTFX_SignatureHandler::Create();
    if (!pSignHandler) return OES_ERROR_SIGN_BAD_PARAM;

    HTFX_SIGN_PARAM param;
    param.strPFXData = strPFXKey;
    param.strSealData = strSealData;
    param.strPassword.assign(passwd, strlen(passwd));
    param.strPropInfo.assign((char*)puchDocProperty, iDocPropertyLen);
    param.strDataHash.assign((char*)puchDigestData, iDigestDataLen);
    param.strTimeInfo.assign((char*)puchSignDateTime, iSignDateTimeLen);

    FX_DWORD dwSignValue = *piSignValueLen;
    err_t nRet = pSignHandler->Sign(param, puchSignValue, &dwSignValue);
    *piSignValueLen = dwSignValue;
    pSignHandler->Release();

    return nRet;
}

/**
* @brief 代理验签实现  [Required]
* @param[in]  puchSealData            印章数据
* @param[in]  iSealDataLen            印章数据长度
* @param[in]  puchDocProperty         文档信息，固定为Signature.xml的绝对路径
* @param[in]  iDocPropertyLen         文档信息长度
* @param[in]  puchDigestData		   摘要数据
* @param[in]  iDigestDataLen		   摘要数据长度
* @param[in]  puchSignMethod          签名算法
* @param[in]  iSignMethodLen          签名算法长度
* @param[in]  puchSignDateTime        签名时间
* @param[in]  piSignDateTimeLen       签名时间长度
* @param[in]  puchSignValue           签名值
* @param[in]  iSignValueLen           签名值长度
* @param[in]  iOnline                 是否在线验证(1：代表在线验证；0：代表离线验证）
* @return 调用成功返回OES_OK，否则是错误代码，返回值包括三段（印章本地验证结果，印章在线验证结果，文档验证结果），
*         在发生错误时，详细的错误信息需要调用OES_GetErrMessage()。
*/
OES_API unsigned long OES_CALL    OES_Verify(unsigned char* puchSealData,int iSealDataLen,
    unsigned char* puchDocProperty,int iDocPropertyLen,
    unsigned char* puchDigestData,int iDigestDataLen,
    unsigned char* puchSignMethod,int iSignMethodLen,
    unsigned char* puchSignDateTime,int iSignDateTimeLen,
    unsigned char* puchSignValue,int iSignValueLen,
    int iOnline)
{
    if (!iDocPropertyLen ||
        !iDigestDataLen ||
        !iSignMethodLen ||
        !iSignDateTimeLen ||
        !iSignValueLen)
    {
        return OES_ERROR_INVALID_PTR_NULL;
    }

    IHTFX_SignatureHandler* pSigHandler = IHTFX_SignatureHandler::Create();

    HTFX_VERIFY_PARAM verifyParam;
    verifyParam.strSealData.assign((char*)puchSealData, iSealDataLen);
    verifyParam.strPropInfo.assign((char*)puchDocProperty, iDocPropertyLen);
    verifyParam.strDataHash.assign((char*)puchDigestData, iDigestDataLen);

    verifyParam.strTimeInfo.assign((char*)puchSignDateTime, iSignDateTimeLen);

    FX_BOOL bRel = pSigHandler->Verify(verifyParam, puchSignValue, iSignValueLen);
    pSigHandler->Release();
    return bRel;
}

/**
* @brief 取得错误信息
* @param[in]      errCode              错误代码，获得于OES_Verify()或者OES_Sign()、OES_Digest()
* @param[out]     puchErrMessage       错误信息（UTF-8编码），应使用UTF-8编码，当为NULL时，通过piErrMessageLen给出长度
* @param[out/in]  piErrMessageLen      错误信息长度
*/
OES_API int OES_CALL OES_GetErrMessage(unsigned long errCode,unsigned char* puchErrMessage,int* piErrMessageLen)
{
    std::string strErrMsg = GetErrMsg(errCode);

    if (puchErrMessage)
    {
        memset(puchErrMessage, 0, *piErrMessageLen);
        memcpy(puchErrMessage, strErrMsg.data(), strErrMsg.length());
        *piErrMessageLen = strErrMsg.length();
    }
    else
    {
        *piErrMessageLen = strErrMsg.length() + 1;
    }

    return OES_ERROR_OK;
}
