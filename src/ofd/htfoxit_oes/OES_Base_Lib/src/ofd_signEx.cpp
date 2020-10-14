#include "../include/stdafx.h"
#include "time.h"

static int OutputSequence(const void *buffer, size_t size, void *app_key)
{
	if (!app_key) return -1;

	std::string* pstrData = (std::string*)app_key;
    unsigned long oldSize = pstrData->length();

    unsigned long dataSize = size + oldSize;
	char* pBuf = new char[dataSize + 1];
    memset(pBuf, 0, dataSize + 1);

    memcpy(pBuf, pstrData->data(), oldSize);
    memcpy(pBuf + oldSize, buffer, size);

	pstrData->assign(pBuf, dataSize);
	
    SAFE_DELETE_BLOCK(pBuf);

	return 0;

}

static void	CopySealInfo(SESeal_t* dest, SESeal_t* src)
{
	if (!dest || !src) return;

	//印章拷贝

	SES_SealInfo& sealInfo = dest->esealInfo;
	SES_SignInfo& signInfo = dest->signInfo;
	//印章信息
	{
        SES_Header_t& header = sealInfo.header;
        IA5String_t& esID = sealInfo.esID;
        SES_ESPictureInfo_t& picture = sealInfo.picture;
        SES_ESPropertyInfo_t& props = sealInfo.property;
		//印章头信息
		{
			header.version = OES_ESEAL_HEADER_VERSION;
			header.version = src->esealInfo.header.version;
			membufcpy(header.id, src->esealInfo.header.id);
			membufcpy(header.vid, src->esealInfo.header.vid);
		}
		//印章标识编码
		{
			membufcpy(esID, src->esealInfo.esID);
		}
		//印章的图片信息
		{
			picture.height = src->esealInfo.picture.height;
			picture.with = src->esealInfo.picture.with;
			membufcpy(picture.type, src->esealInfo.picture.type);
			membufcpy(picture.data, src->esealInfo.picture.data);
		}
		//印章的属性信息
		{
            long& type = props.type;
            UTF8String_t& name = props.name;
            CERT_LIST& list = props.certList;
            UTCTime_t& tmCreate = props.createDate;
            UTCTime_t& tmStart = props.validStart;
            UTCTime_t& tmEnd = props.validEnd;

			type = src->esealInfo.property.type;
			membufcpy(name, src->esealInfo.property.name);

			list.list.count = 0;
			list.list.size = 1;

			list.list.array = (OCTET_STRING_t**)malloc(sizeof(OCTET_STRING_t *));
			OCTET_STRING_t* pValue = (OCTET_STRING_t*)malloc(sizeof(OCTET_STRING_t));
            OCTET_STRING& srcValue = *(src->esealInfo.property.certList.list.array[0]);
			membufcpy(*pValue, srcValue);
			ASN_SEQUENCE_ADD(&(list.list), pValue);

			membufcpy(tmCreate, src->esealInfo.property.createDate);
			membufcpy(tmStart, src->esealInfo.property.validStart);
			membufcpy(tmEnd, src->esealInfo.property.validEnd);
		}
	}

	//印章的签名信息
	{
        OCTET_STRING_t& certData = signInfo.cert;
        OBJECT_IDENTIFIER_t& algorighm = signInfo.signatureAlgorighm;
        BIT_STRING_t& signData = signInfo.signData;

		membufcpy(certData, src->signInfo.cert);
		membufcpy(algorighm, src->signInfo.signatureAlgorighm);
		membufcpy(signData, src->signInfo.signData);
	}
}

std::string oes_foxit::GetCertData(CFX_WideString strFilePath, CFX_ByteString strPasswd)
{
    std::string strCertData;
    IFX_FileRead* pFileRead = FX_CreateFileRead(strFilePath);
    if (pFileRead)
    {
        int nSize = pFileRead->GetSize();
        char* buf = new char[nSize + 1];
        memset(buf, 0, nSize + 1);
        pFileRead->ReadBlock(buf, nSize);
        pFileRead->Release();

        std::string strPWD;
        strPWD.assign(strPasswd.GetBuffer(strPasswd.GetLength()), strPasswd.GetLength());
        strPasswd.ReleaseBuffer();

        COFD_OESSignHandler* pHandler = COFD_OESSignHandler::Create(buf, nSize, strPWD);
        if(pHandler)
        {
            pHandler->GetSignCertData(strCertData);
            pHandler->Release();
        }

        delete[] buf;
    }

    return strCertData;
}

std::string          oes_foxit::Sign(CFX_WideString strFilePath, CFX_ByteString strPasswd, std::string digest, std::string digestAlg)
{
    std::string strCertData;
    IFX_FileRead* pFileRead = FX_CreateFileRead(strFilePath);
    if (pFileRead)
    {
        int nSize = pFileRead->GetSize();
        char* buf = new char[nSize + 1];
        memset(buf, 0, nSize + 1);
        pFileRead->ReadBlock(buf, nSize);
        pFileRead->Release();
        
        std::string strPWD;
        strPWD.assign(strPasswd.GetBuffer(strPasswd.GetLength()), strPasswd.GetLength());
        strPasswd.ReleaseBuffer();
        
        COFD_OESSignHandler* pHandler = COFD_OESSignHandler::Create(buf, nSize, strPWD);
        if(pHandler)
        {
            FX_DWORD dataSize = 0;
            if (pHandler->Sign((unsigned char*)digest.data(), digest.size(), NULL, &dataSize)) {
                char* dataBuf = new char[dataSize+1];
                memset(dataBuf, 0, dataSize+1);
                if (pHandler->Sign((unsigned char*)digest.data(), digest.size(), (unsigned char*)dataBuf, &dataSize)) {
                    strCertData.assign(dataBuf, dataSize);
                }
                delete[] dataBuf;
            }
            pHandler->Release();
        }
        
        delete[] buf;
    }
    
    return strCertData;
}

//////////////////////////////////////////////////////////////////////////
IHTFX_Eseal* IHTFX_Eseal::Create(HTFX_ESEAL_PARAM& param)
{
    if (IHTFX_Eseal* pEseal = CHTFX_Eseal::Create(param))
		return pEseal;

    return NULL;
}

IHTFX_SignatureHandler* IHTFX_SignatureHandler::Create()
{
	return CHTFX_SignatureHandler::Create();
}

//////////////////////////////////////////////////////////////////////////
//CHTFX_Eseal
CHTFX_Eseal::CHTFX_Eseal():
m_pSeal(NULL)
{

}

CHTFX_Eseal::~CHTFX_Eseal()
{
    releaseSealPtr(m_pSeal);
}

bool CHTFX_Eseal::Initialize(HTFX_ESEAL_PARAM& param)
{
	if (m_pSeal)
	{
        releaseSealPtr(m_pSeal);
	}

    bool bResult = false;
	m_pSeal = new SESeal_t;
    memset(m_pSeal, 0, sizeof(SESeal_t));

	//印章初始化
	{
		SES_SealInfo& sealInfo = m_pSeal->esealInfo;
		SES_SignInfo& signInfo = m_pSeal->signInfo;
		//印章信息
		{
            SES_Header_t& header = sealInfo.header;
            IA5String_t& esID = sealInfo.esID;
            SES_ESPictureInfo_t& picture = sealInfo.picture;
            SES_ESPropertyInfo_t& props = sealInfo.property;
			//印章头信息
			{
				std::string strID, strVID;
				strID.append(OES_ESEAL_HEADER_ID);
				strVID.append(OES_ESEAL_HEADER_VID);
				header.version = OES_ESEAL_HEADER_VERSION;
				str2IA5(strID, header.id);
				str2IA5(strVID, header.vid);
			}
			//印章的标识编码
			{
				str2IA5(param.esID, esID);
			}
			//印章的图片信息
			{
				picture.height = param.lnHeight;
				picture.with = param.lnWidth;
				str2IA5(param.strPicType, picture.type);
				str2OES(param.strPicData, picture.data);
			}
			//印章的属性信息
			{
                long& type = props.type;
                UTF8String_t& name = props.name;
                CERT_LIST& list = props.certList;
                UTCTime_t& tmCreate = props.createDate;
                UTCTime_t& tmStart = props.validStart;
                UTCTime_t& tmEnd = props.validEnd;

				//印章类型
				{
					type = OES_ESEAL_PROPS_TYPE_PERSONAL;
				}
				//印章名称,UTF8String required
				{
					str2OES(param.strName, name);
				}
				//证书列表
				{
					list.list.count = 0;
					list.list.size = 1;
					
					list.list.array = (OCTET_STRING_t**)malloc(sizeof(OCTET_STRING_t *));
					OCTET_STRING_t* pValue = (OCTET_STRING_t*)malloc(sizeof(OCTET_STRING_t));
					str2OES(param.strCertData, *pValue);
					ASN_SEQUENCE_ADD(&(list.list), pValue);
				}
				//印章时间信息
				{
					str2UTCTime(param.strCreateDate, tmCreate);
					str2UTCTime(param.strStartDate, tmStart);
					str2UTCTime(param.strEndDate, tmEnd);
				}
			}
            sealInfo.extDatas.list.array = NULL;
			sealInfo.extDatas.list.size = 0;
			sealInfo.extDatas.list.count = 0;
		}

		//印章签名信息
		{
            COFD_InternalHandler* pInternalHandler = COFD_InternalHandler::Create();
            if (!pInternalHandler) return bResult;
			
            auto_ptr_release<COFD_InternalHandler> unique_ptr_internal_handler(pInternalHandler);

            COFD_OESSignHandler* pSignHandler = pInternalHandler->GetSignHandler();
            if (!pSignHandler)
            {
                return bResult;
            }

			std::string strSealInfo;
			if (OutputSealInfo(&strSealInfo))
			{
				unsigned char* pData = (unsigned char*)strSealInfo.data();
                FX_DWORD dwSealSize = strSealInfo.length();

                FX_DWORD cbSignData = 0;
                if (pSignHandler->Sign(pData, dwSealSize, NULL, &cbSignData))
				{
					unsigned char* pbSignData = new unsigned char[cbSignData];
                    memset(pbSignData, 0, cbSignData);

                    if (pSignHandler->Sign(pData, dwSealSize, pbSignData, &cbSignData))
					{
						std::string strSignValue;
						strSignValue.assign((char*)pbSignData, cbSignData);

						std::string strCertData, strAlgorithm;
						pSignHandler->GetSignCertData(strCertData);
						pSignHandler->GetSignAlgorithm(strAlgorithm);

						str2OES(strCertData, signInfo.cert);
						str2ID(strAlgorithm, signInfo.signatureAlgorighm);
						str2BIT(strSignValue, signInfo.signData);
						
                        bResult = true;
					}
                    SAFE_DELETE_BLOCK(pbSignData);
				}
            }
		}
	}

	return bResult;
}

IHTFX_Eseal* CHTFX_Eseal::Create(HTFX_ESEAL_PARAM& param)
{
	CHTFX_Eseal* pSeal = new CHTFX_Eseal();
    if (!pSeal) return NULL;

	if (pSeal->Initialize(param))
		return pSeal;
	else
		delete pSeal;

    return NULL;
}

bool CHTFX_Eseal::SetSignInfo(std::string strCertData,
								 std::string strSignAlgorithm,
								 std::string strSignValue)
{
    if (!m_pSeal) return false;

	SES_SignInfo& signInfo = m_pSeal->signInfo;
	//印章的签名信息
	{
        OCTET_STRING_t& certData = signInfo.cert;
        OBJECT_IDENTIFIER_t& algorighm = signInfo.signatureAlgorighm;
        BIT_STRING_t& signData = signInfo.signData;

		str2OES(strCertData, certData);
		str2ID(strSignAlgorithm, algorighm);
		str2BIT(strSignValue, signData);
	}

    return true;
}

bool CHTFX_Eseal::OutputSealInfo(std::string* pstrData)
{
    SES_SealInfo_t& sealInfo = m_pSeal->esealInfo;
    asn_enc_rval_t ret = der_encode(&asn_DEF_SES_SealInfo, &sealInfo, OutputSequence, pstrData);
	if (ret.encoded == -1)
	{
        return false;
	}

    return true;
}

bool CHTFX_Eseal::OutputSealData(std::string& pstrData)
{
    asn_enc_rval_t ret = der_encode(&asn_DEF_SESeal, m_pSeal, OutputSequence, &pstrData);
	if (ret.encoded == -1)
	{
        return false;
	}

    return true;
}

void CHTFX_Eseal::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////////////
CHTFX_SignatureHandler::CHTFX_SignatureHandler()
{
    m_ExtHandler = NULL;
}

CHTFX_SignatureHandler::~CHTFX_SignatureHandler()
{

}

CHTFX_SignatureHandler* CHTFX_SignatureHandler::Create()
{
    CHTFX_SignatureHandler* pSign = new CHTFX_SignatureHandler;
	return pSign;
}

void CHTFX_SignatureHandler::Init(IHTFX_SignatureCallback* handler)
{
    m_ExtHandler = handler;
}

err_t CHTFX_SignatureHandler::Digest(unsigned char* pbToBeHash, FX_DWORD cbToBeHash,
									   unsigned char* pbHashedData, int* pcbHashedData)
{
    const char* pHashMethod = "sha256";
    
    // add by gm 20170818
    if (m_ExtHandler) {
        std::string method = m_ExtHandler->getDigestMethod();
        if (method == OES_DIGEST_ALGORITHM_SHA1) {
            pHashMethod = "sha1";
        }
    }
    // add end 20170818
    return COFD_BaseHandler::Digest(pbToBeHash, cbToBeHash, pbHashedData, pcbHashedData, pHashMethod);
}

err_t	CHTFX_SignatureHandler::Sign(HTFX_SIGN_PARAM& param, unsigned char* pbSignedValue, FX_DWORD* pcbSignedValue)
{
    std::string strPFXData = param.strPFXData;
    // modify by gm 20170818
    if (m_ExtHandler) {
        // do nothing
    } else if (strPFXData.empty()) {
        return OES_ERROR_SIGN_ERROR_KEY;
    } else {
        bool bRet = COFD_Utils::IsCertHasSignUsage(strPFXData, param.strPassword);
        //    printf("1234123412341234\n");
        if (!bRet) {
            return OES_ERROR_SIGN_ERROR_KEY;
        }
    }
    // moidfy end 20170818

	//////////////////////////////////////////////////////////////////////////
	//验证印章的有效性(验证印章的签名)
	if (!VerifySeal(param.strSealData)) return OES_ERROR_SEAL_NOT_OFFICIAL;

	//////////////////////////////////////////////////////////////////////////
	//Begin 解析印章，对比印章与签章证书的一致性。

    SESeal* pSealParam = NULL;
    FX_DWORD  dwSize = param.strSealData.length();
	const char* pBuffer = param.strSealData.data();
    asn_dec_rval_t rval = ber_decode(0, &asn_DEF_SESeal, (void **)&pSealParam, pBuffer, dwSize);

    auto_seal_ptr_releaser(pSealParam);

	//印章数据错误
	if (rval.code != RC_OK) return OES_ERROR_SEAL_INVALID_DATA;

    CERT_LIST& certList = pSealParam->esealInfo.property.certList;
    OCTET_STRING_t** & pCertListAry = certList.list.array;

	//印章证书数据缺失
	if (!pCertListAry) return OES_ERROR_SEAL_INVALID_DATA;

	//印章内无证书
    OCTET_STRING_t* pCertItem = certList.list.array[0];
	if (!pCertItem) return OES_ERROR_SEAL_INVALID_DATA;

    std::string strSealCertData;
    strSealCertData.assign((char*)pCertItem->buf, pCertItem->size);

    COFD_OESSignHandlerEx* pSignHandler = NULL;
    if (m_ExtHandler) {
        pSignHandler = COFD_OESSignHandlerEx::Create(m_ExtHandler, param.strPassword);
    } else {
        pSignHandler = COFD_OESSignHandlerEx::Create((char*)strPFXData.data(), strPFXData.length(), param.strPassword);
    }
    if (!pSignHandler) return OES_ERROR_SIGN_ERROR_KEY;

    auto_ptr_release<COFD_OESSignHandler> unique_tag_handler(pSignHandler);

    std::string strPFXCertData;
    pSignHandler->GetSignCertData(strPFXCertData);

    bool bSame = strSealCertData.compare(strPFXCertData) == 0;

	//印章证书与签章证书不匹配
    if (!bSame)
    {
        return OES_ERROR_SIGN_ERROR_KEY;
    }

	//End.
	//////////////////////////////////////////////////////////////////////////

    SES_Signature_t sign;
    memset(&sign, 0, sizeof(sign));

    auto_sign_tag_releaser(sign);

	//签章数据
    TBS_Sign_t& signData = sign.toSign;
	
    long& version = signData.version;
    SESeal_t& seal = signData.esealInfo;
    BIT_STRING_t& timeInfo = signData.timeInfo;
    BIT_STRING_t& dataHash = signData.dataHash;
    IA5String_t& propInfo = signData.propertyInfo;
    OCTET_STRING_t& cert = signData.cert;
    OBJECT_IDENTIFIER_t& signAlgorithm = signData.signatureAlgorithm;
	{
		version = OES_SIGN_TOSIGN_VERSION;
		
		CopySealInfo(&seal, pSealParam);

        std::string strCert = strPFXCertData;
		str2OES(strCert, cert);

		str2BIT(param.strTimeInfo, timeInfo);
		str2BIT(param.strDataHash, dataHash);
		str2IA5(param.strPropInfo, propInfo);

        std::string strAlgorithm;
        pSignHandler->GetSignAlgorithm(strAlgorithm);
		str2ID(strAlgorithm, signAlgorithm);
	}
	
	std::string strToBeSignData;
    asn_enc_rval_t xRet = der_encode(&asn_DEF_TBS_Sign, &(sign.toSign), OutputSequence, &strToBeSignData);
	if (xRet.encoded == -1)
	{
		return OES_ERROR_SIGN_TOBESIGN_DATA;
	}

    FX_DWORD dwToBeSignedDataSize = strToBeSignData.length();
    unsigned char* pToBeSignedData = (unsigned char*)strToBeSignData.data();

    FX_DWORD dwSignDataSize = 0;
    if (pSignHandler->Sign(pToBeSignedData, dwToBeSignedDataSize, NULL, &dwSignDataSize))
    {
        sign.signature.size = dwSignDataSize;
        sign.signature.buf = new uint8_t[dwSignDataSize];

        uint8_t*& pSignData = sign.signature.buf;
        if (pSignHandler->Sign(pToBeSignedData, dwToBeSignedDataSize, pSignData, &dwSignDataSize))
        {
            std::string strData;
            asn_enc_rval_t ret = der_encode(&asn_DEF_SES_Signature, &sign, OutputSequence, &strData);

            if (ret.encoded == -1)
            {
                return OES_ERROR_SIGN_BAD_PARAM;
            }

            //内存不足
            FX_DWORD dwFileSize = strData.length();
            if (pbSignedValue && dwFileSize > *pcbSignedValue)
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }

            if (pbSignedValue)
            {
                memcpy(pbSignedValue, strData.data(), strData.length());
            }

            *pcbSignedValue = strData.length();

            return OES_ERROR_OK;
        }
    }

    return OES_ERROR_SIGN_BAD_PARAM;
}

#include <algorithm>

bool CHTFX_SignatureHandler::VerifySeal(std::string& strSealData)
{
    if (strSealData.length() == 0) return false;
	
    SESeal* pSeal = NULL;
    asn_dec_rval_t rval = ber_decode(0, &asn_DEF_SESeal, (void **)&pSeal,
		strSealData.data(), strSealData.length());

    if (rval.code != RC_OK) return false;

    auto_seal_ptr_releaser(pSeal);

	std::string strSealInfo;
    asn_enc_rval_t ret = der_encode(&asn_DEF_SES_SealInfo, &(pSeal->esealInfo), OutputSequence, &strSealInfo);
    if (ret.encoded == -1)
    {
        return false;
    }

    char* pbData = (char*)pSeal->signInfo.cert.buf;
    FX_DWORD cbData = pSeal->signInfo.cert.size;
    std::string strDigestMethod("sha256");
    if (m_ExtHandler) {
        std::string method = m_ExtHandler->getDigestMethod();
        if (method == OES_DIGEST_ALGORITHM_SHA1) {
            strDigestMethod = "sha1";
        }
    }
    COFD_OESVerifyHandler* pVerifyHandler = COFD_OESVerifyHandler::Create(pbData, cbData, strDigestMethod);
    if (!pVerifyHandler)
    {
        return false;
    }

    //
    std::string strSignData;
    BIT_STRING_t& signData = pSeal->signInfo.signData;
    strSignData.assign((char*)signData.buf, signData.size);

    unsigned char* pbContent = (unsigned char*)strSealInfo.data();
    FX_DWORD  cbContent = strSealInfo.length();

    bool bVerifyResult = pVerifyHandler->Verify(pbContent, cbContent, (unsigned char*)strSignData.data(), strSignData.length());

    pVerifyHandler->Release();

	return bVerifyResult;
}

err_t CHTFX_SignatureHandler::Verify(HTFX_VERIFY_PARAM& param, unsigned char* pbSignedValue, FX_DWORD cbSignedValue)
{
	//////////////////////////////////////////////////////////////////////////
	//校验签章数据是否被篡改。

    SES_Signature* pSign = NULL;
    asn_dec_rval_t rval = ber_decode(0, &asn_DEF_SES_Signature, (void **)&pSign,
		pbSignedValue, cbSignedValue);

    if (rval.code != RC_OK)
    {
        return OES_ERROR_VERIFY_SIGNDATA;
    }

    auto_sign_ptr_releaser(pSign);

    BIT_STRING_t& dataHash = pSign->toSign.dataHash;
    IA5String_t& propInfo = pSign->toSign.propertyInfo;
    BIT_STRING_t& timeInfo = pSign->toSign.timeInfo;
    SESeal_t& sealInfo = pSign->toSign.esealInfo;

    std::string strSignMethodOID;
    OBJECT_IDENTIFIER_t& algorithm = pSign->toSign.signatureAlgorithm;
    strSignMethodOID.assign((char*)algorithm.buf, algorithm.size);

    if (!obj_equalto_str(dataHash, param.strDataHash)) return OES_ERROR_VERIFY_SIGNDATA;
    if (!obj_equalto_str(propInfo, param.strPropInfo)) return OES_ERROR_VERIFY_SIGNDATA;
    if (!obj_equalto_str(timeInfo, param.strTimeInfo)) return OES_ERROR_VERIFY_SIGNDATA;
	{
		//校验印章数据与签章中的印章数据是否相同。
		std::string strSealDataInSignValue;
        asn_enc_rval_t ret = der_encode(&asn_DEF_SESeal, &sealInfo, OutputSequence, &strSealDataInSignValue);
        if (ret.encoded == -1) return OES_ERROR_VERIFY_SIGNDATA;

        bool bSame = param.strSealData.compare(strSealDataInSignValue) == 0;

        if (!bSame)
        {
            return OES_ERROR_VERIFY_SIGNDATA;
        }
	}

	//////////////////////////////////////////////////////////////////////////
	//校验印章签名
    if (!VerifySeal(param.strSealData))
    {
        return OES_ERROR_VERIFY_SIGNDATA;
    }

	//////////////////////////////////////////////////////////////////////////
	//校验签章

    std::string strToBeSign;
    asn_enc_rval_t xRet = der_encode(&asn_DEF_TBS_Sign, &(pSign->toSign), OutputSequence, &strToBeSign);
    if (xRet.encoded == -1)
    {
        return OES_ERROR_VERIFY_SIGNDATA;
    }

    std::string strDigestMethod("sha256");
    OCTET_STRING_t& pCertData = pSign->toSign.cert;
    COFD_OESVerifyHandler* pVerifyhandler = COFD_OESVerifyHandler::Create((char*)pCertData.buf, pCertData.size, strDigestMethod);
    if (!pVerifyhandler)
    {
        return OES_ERROR_VERIFY_SIGNDATA;
    }

    unsigned char* pbToBeSigned = (unsigned char*)strToBeSign.data();
    FX_DWORD cbToBeSigned = strToBeSign.length();

	//Decode signed data to verify later.
    FX_DWORD cbSignedData = pSign->signature.size;
	unsigned char* pbSignedData = pSign->signature.buf;

	//////////////////////////////////////////////////////////////////////////
	//verifying...
    bool bResult = pVerifyhandler->Verify(pbToBeSigned, cbToBeSigned, pbSignedData, cbSignedData);

    pVerifyhandler->Release();

	return bResult ? OES_ERROR_OK : OES_ERROR_VERIFY_FAILED;
}

//签署纯数字签名
err_t CHTFX_SignatureHandler::DigitalSign(HTFX_DIGITAL_SIGN_PARAM& param, unsigned char* pbSignedValue, FX_DWORD* cbSignedValue)
{
    std::string strPFXData = param.strPFXData;
    if (strPFXData.empty()) return OES_ERROR_SIGN_ERROR_KEY;

    bool bRet = COFD_Utils::IsCertHasSignUsage(strPFXData, param.strPassword);
    if (!bRet) return OES_ERROR_SIGN_ERROR_KEY;

    COFD_DigitalSignHandler* pSignHandler = COFD_DigitalSignHandler::Create((char*)strPFXData.data(),
                                                                                                               strPFXData.length(),
                                                                                                               param.strPassword,
                                                                                                               std::string("sha256RSA"));
    if (!pSignHandler) return OES_ERROR_SIGN_ERROR_KEY;

    auto_ptr_release<COFD_DigitalSignHandler> tmpSmartPtr(pSignHandler);

    FX_DWORD dwToBeSignedDataSize = param.strDataHash.length();
    unsigned char* pToBeSignedData = (unsigned char*)param.strDataHash.data();

    FX_DWORD dwSignDataSize = 0;
    if (pSignHandler->Sign(pToBeSignedData, dwToBeSignedDataSize, NULL, &dwSignDataSize))
    {
        *cbSignedValue = dwSignDataSize;
        if (pbSignedValue)
        {
            if (!pSignHandler->Sign(pToBeSignedData, dwToBeSignedDataSize, pbSignedValue, cbSignedValue))
            {
                return OES_ERROR_BUFFER_NOT_ENUGH;
            }
        }
        return OES_ERROR_OK;
    }

    return OES_ERROR_SIGN_BAD_PARAM;
}

//验证纯数字签名
err_t CHTFX_SignatureHandler::DigitalVerify(HTFX_DIGITAL_VERIFY_PARAM& param, unsigned char* pbSignedValue, FX_DWORD cbSignedValue)
{
    COFD_DigitalVerifyHandler* pVerifyHandler = COFD_DigitalVerifyHandler::Create();
    if (!pVerifyHandler) return OES_ERROR_VERIFY_FAILED;

    auto_ptr_release<COFD_DigitalVerifyHandler> tmpSmartPtr(pVerifyHandler);

    FX_DWORD dwToBeSignedDataSize = param.strDataHash.length();
    unsigned char* pToBeSignedData = (unsigned char*)param.strDataHash.data();

    FX_DWORD dwSignDataSize = 0;
    if (pVerifyHandler->Verify(pToBeSignedData, dwToBeSignedDataSize, pbSignedValue, dwSignDataSize))
    {
        return OES_ERROR_OK;
    }

    return OES_ERROR_VERIFY_FAILED;
}

//获取签名身份
err_t CHTFX_SignatureHandler::DigitalGetSigner(unsigned char* pbSignedValue, FX_DWORD cbSignedValue, char* pbSigner, FX_DWORD* cbSigner)
{
    COFD_DigitalVerifyHandler* pVerifyHandler = COFD_DigitalVerifyHandler::Create();
    if (!pVerifyHandler) return OES_ERROR_READ_DATA_DENIED;

    if (pVerifyHandler->GetSigner(pbSignedValue, cbSignedValue, pbSigner, cbSigner))
    {
        return OES_ERROR_OK;
    }

    return OES_ERROR_READ_DATA_DENIED;
}

void CHTFX_SignatureHandler::Release()
{
	delete this;
}
