#include "fr_include.h"
#include "../fs_util.h"
#if !defined(_WIN32) && !defined(WIN32)
#include "fr_ofdsignaturehandler.h"
#define ENABLE_EXT_SIGN_CALLBACK
#endif
#include "fr_ofdpageImp.h"
#include "fr_ofddocImp.h"
#include "fr_ofdsignatureImp.h"
#include "fr_ofdutil.h"

#if 1
/** @brief	Unknown signature. */
#define CPDF_SIGNATURE_STATE_UNKNOWN								0x00000000//
/** @brief	Unsigned signature. */
#define CPDF_SIGNATURE_STATE_UNSIGNED								0x00000001
/** @brief	Signed signature. */
#define CPDF_SIGNATURE_STATE_SIGNED									0x00000002

/** @brief Verify signature state is valid. */
#define CPDF_SIGNATURE_STATE_VERIFY_VALID							0x00000004
/** @brief Verify signature state is invalid. */
#define CPDF_SIGNATURE_STATE_VERIFY_INVALID							0x00000008
/** @brief Signature data is destroyed (the signature data cannot be parsed properly). */
#define CPDF_SIGNATURE_STATE_VERIFY_ERRORDATA						0x00000010
/** @brief Unsupported signature. */
#define CPDF_SIGNATURE_STATE_VERIFY_NOSUPPORTWAY					0x00000020
/** @brief Non expected byte range. */
#define CPDF_SIGNATURE_STATE_VERIFY_ERRORBYTERANGE					0x00000040
/** @brief The document has changed within the scope of the signature. */
#define CPDF_SIGNATURE_STATE_VERIFY_CHANGE							0x00000080
/** @brief Signature cannot be trusted (containing aggression). */
#define CPDF_SIGNATURE_STATE_VERIFY_INCREDIBLE						0x00000100

/** @brief Verify the issuer is valid. */
#define CPDF_SIGNATURE_STATE_VERIFY_ISSUER_VALID					0x00001000
/** @brief Verify the issuer is unknown. */
#define CPDF_SIGNATURE_STATE_VERIFY_ISSUER_UNKNOW					0x00002000
/** @brief Verify the certificate is revoked. */
#define CPDF_SIGNATURE_STATE_VERIFY_ISSUER_REVOKE					0x00004000
/** @brief Verify the certificate is expired. */
#define CPDF_SIGNATURE_STATE_VERIFY_ISSUER_EXPIRE					0x00008000
/** @brief Don't check the issuer . */
#define CPDF_SIGNATURE_STATE_VERIFY_ISSUER_UNCHECK					0x00010000
/** @brief Verify the issuer is current issuer. */
#define CPDF_SIGNATURE_STATE_VERIFY_ISSUER_CURRENT					0x00020000

/** @brief Uncheck timestamp. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_NONE					0x00040000
/** @brief Signature is timestamp signature. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_DOC					0x00080000
/** @brief Verify the timestamp is valid. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_VALID					0x00100000
/** @brief Verify the timestamp is invalid. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_INVALID				0x00200000
/** @brief Verify the timestamp is expired. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_EXPIRE				0x00400000
/** @brief Verify the timestamp issuer is unknown. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_ISSUER_ISUNKNOW		0x00800000
/** @brief Verify the timestamp issuer is valid. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_ISSUER_ISVALID		0x01000000
/** @brief Verify the timestamp time is valid. */
#define CPDF_SIGNATURE_STATE_VERIFY_TIMESTAMP_TIMEBEFORE			0x02000000

/*@}*/
#endif

#if (_FX_OS_==_FX_WIN32_DESKTOP_)
#pragma comment(lib, "Crypt32.lib")
#endif

//#define __T(x)      L ## x
//#define _T(x)       __T(x)
//#define _T(str)   L##str
#define AUTO_RELEASE_PTR(type, var, lamda) AutoReleasePtr<type> var(lamda)
#define OES_OK						0x00000000	//成功
//#define AUTO_RELEASE_PTR(type, var, lamda) std::unique_ptr<##type, ptr_releaser> ##var(##lamda)

void DM_LogOut(const char* format, ...)
{
#if (_FX_OS_==_FX_ANDROID_)
#ifdef DM_DEBUG
	va_list argptr;
	va_start(argptr, format);
	__android_log_vprint(4, "XXXXXX", format, argptr);
	va_end(argptr);
#endif
#endif
}


#ifdef ENABLE_EXT_SIGN_CALLBACK
std::string _getCertData(std::string pfxPath, std::string password)
{
    return oes_foxit::GetCertData(CFX_WideString::FromUTF8(pfxPath.c_str()),password.c_str());
}

std::string _signWithCert(std::string pfxPath, std::string password, std::string digest, std::string digestAlg)
{
    return oes_foxit::Sign(CFX_WideString::FromUTF8(pfxPath.c_str()),password.c_str(), digest, digestAlg);
}
#endif

//FSAnnot
FSPDFPage* FR_OFDSignatureImp::getPage() const 
{
	return m_pPage;
}

FS_BOOL FR_OFDSignatureImp::isMarkup() const
{
	return FALSE;
}

FSM_ANNOTTYPE FR_OFDSignatureImp::getType() const
{
	return FSAnnot::e_annotWidget;
}

FS_INT32 FR_OFDSignatureImp::getIndex() const
{
	// need implement
	return 0;
}

FSString FR_OFDSignatureImp::getContent() const
{
	// need implement
	return	NULL;    
}

void FR_OFDSignatureImp::setContent(const char* content)
{

}

FSDateTime FR_OFDSignatureImp::getModifiedDateTime() const
{
	// need implement
	FSDateTime retDate;
	return retDate;
}

void FR_OFDSignatureImp::setModifiedDateTime(FSDateTime dataTime)
{
	// need implement
}

FS_DWORD FR_OFDSignatureImp::getFlags() const
{
	// need implement
	return 0;
}
void FR_OFDSignatureImp::setFlags(FS_DWORD flags)
{
	// need implement
}

FSString FR_OFDSignatureImp::getUniqueID() const
{
	// need implement
	return NULL;
}

void FR_OFDSignatureImp::setUniqueID(const char* uniqueID)
{
	
}

FSRectF FR_OFDSignatureImp::getRect() const
{
	// need implement
	FSRectF retcF;
	// m_Rect to FSRectF
	return retcF;
}

FS_BOOL FR_OFDSignatureImp::move(FSRectF rect)
{
	// need implement
	return FALSE;
}

FSBorderInfo FR_OFDSignatureImp::getBorderInfo() const
{
	FSBorderInfo info;
	return info;
}

void FR_OFDSignatureImp::setBorderInfo(FSBorderInfo border)
{

}

FS_ARGB FR_OFDSignatureImp::getBorderColor() const
{
	return 0;
}

void FR_OFDSignatureImp::setBorderColor(FS_ARGB color)
{

}

FS_BOOL FR_OFDSignatureImp::resetAppearanceStream()
{
	return FALSE;
}

FSRectI	FR_OFDSignatureImp::getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix)
{
	FSRectI rectI;
	return rectI;
}

FSPDFDictionary* FR_OFDSignatureImp::getDict() const 
{
	return NULL;
}

FS_BOOL	FR_OFDSignatureImp::removeProperty(FSM_ANNOTPROPERTY property) 
{
	return FALSE;
}

//FSFormControl
FSFormField* FR_OFDSignatureImp::getField()
{
	return NULL;
}

//FSSignature
FS_BOOL	 FR_OFDSignatureImp::isSigned()
{
	LogOut("test_verify-------------->isSigned true");
	return getState() == CPDF_SIGNATURE_STATE_SIGNED;
}

typedef CFX_ByteString		(*GetDigestValue)(IHTFX_SignatureHandler* pHTFX_SignHandler, IFX_FileRead* pFileRead);

CFX_ByteString GetDigestValueImpl(IHTFX_SignatureHandler* pHTFX_SignHandler, IFX_FileRead* pFileRead)
{
	CFX_ByteString csValue;
	FX_INT32 nFileSize = pFileRead->GetSize();
	FX_LPBYTE pBuf = new FX_BYTE[nFileSize];
	ZeroMemory(pBuf, nFileSize);

	if (pFileRead->ReadBlock(pBuf, 0, nFileSize))
	{
		int nDigestSize = 0;
		if (pHTFX_SignHandler->Digest(pBuf, nFileSize, NULL, &nDigestSize) == OES_OK)
		{
			FX_LPBYTE pDigestData = new FX_BYTE[nDigestSize];
			ZeroMemory(pDigestData, nDigestSize);

			if (pHTFX_SignHandler->Digest(pBuf, nFileSize, pDigestData, &nDigestSize) == OES_OK)
			{
				csValue = CFX_ByteString((char*)pDigestData, nDigestSize);
			}

			delete[] pDigestData;
			pDigestData = NULL;
		}
	}

	delete[] pBuf;
	pBuf = NULL;


	return csValue;
};

FS_PROGRESSSTATE FR_OFDSignatureImp::startSign(const char* savePath, const char* certPath, const char password[], FS_INT32 passwordLen,
	FS_DIGEST_ALGORITHM digestAlgorithm, FSPause* pause, void* clientData)
{
	if (m_pSignature || !getPage() || !getPage()->getDocument() 
		|| isSigned() || FSCRT_Util_IsStringEmpty(certPath)
		|| digestAlgorithm != FSSignatureDefines::e_digestSHA256
		|| (passwordLen>0 && !password)
		) {
		return FSCommonDefines::e_progressError;
	}
	LogOut("FR_OFDSignatureImp::startSign, begin");

	//FSCRT_LOCKOBJECT(lock, &m_lock);
    if (certPath) {
        m_CertPath = certPath;
    }
    if (NULL != password && passwordLen>0) {
        m_CertPassword.assign(password, passwordLen);
    }
    //string filter = m_Value[FSSignatureDefines::e_signatureKeyNameFilter];
    //string subfilter = m_Value[FSSignatureDefines::e_signatureKeyNameSubFilter];
    if (m_pSignHandler) {
#ifdef ENABLE_EXT_SIGN_CALLBACK
        m_pSignHandler->m_signClientData.pClientData = clientData;
        m_pSignHandler->m_signClientData.pfxpath = m_CertPath;
        m_pSignHandler->m_signClientData.password = m_CertPassword;
        m_pSignHandler->m_signClientData.pFSSig = this;
        m_pSignHandler->m_signClientData.m_algorithm = digestAlgorithm;
#endif
    }

	//AUTO_RELEASE_PTR(IFX_FileStream, pOutputStream,FX_CreateFileStream(savePath, FX_FILEMODE_Write | FX_FILEMODE_Truncate));
    AutoReleasePtr<IFX_FileStream> pOutputStream(FX_CreateFileStream(savePath, FX_FILEMODE_Write | FX_FILEMODE_Truncate));

    time_t tmNow = time(NULL);
    CFX_WideString wsTimeNow = Time2UTCString(tmNow);
    if (!GenerateSeal(tmNow)) {
		m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
		LogOut("FR_OFDSignatureImp::startSign, end 1");
        return FSCommonDefines::e_progressError;
    }

	//Get Sign Method Handler.
	AUTO_RELEASE_PTR(IHTFX_SignatureHandler, pHTFX_SignHandler, IHTFX_SignatureHandler::Create());
#ifdef ENABLE_EXT_SIGN_CALLBACK
    pHTFX_SignHandler->Init(m_pSignHandler);
#endif
	
	//Create a sign.
	FSPDFDoc* doc = m_pPage->getDocument();
//	FR_OFDDocImp* docImp = dynamic_cast<FR_OFDDocImp*>(doc);
	FR_OFDDocImp* docImp = (FR_OFDDocImp*)(doc);
	IOFD_WriteDocument* pWriteDoc = docImp->getOFDWriteDocument();
	COFD_WriteSignatures* pSignatures = pWriteDoc->CreateSignatures();
	COFD_WriteSignature* pSignature = pSignatures->InsertSignature(pWriteDoc, -1);
	COFD_WriteStampAnnot* pSigStampAnnot = pSignature->InsertStampAnnot(pWriteDoc, m_pPage->getIndex(), -1);
	
	//CFX_RectF rtAnnot;
	//m_pLDPageView->ClientPointToOFDPoint_EX(0, rtAnnot, m_rtDrag);
	pSigStampAnnot->SetBoundary(m_Rect);

	CFX_WideString sg_Company = L"htfoxit";
	CFX_WideString sg_ProviderName = L"htfoxit_eseal";
	CFX_WideString sg_RefCheckMethod = CFX_WideString::FromLocal(szOID_NIST_sha256);
	CFX_WideString sg_SignatureMethod = CFX_WideString::FromLocal(szOID_RSA_SHA256RSA);
	CFX_WideString sg_Version = L"1";
    
    FSString provider = this->getKeyValue(e_signatureKeyNameProvider);
    FSString company = this->getKeyValue(e_signatureKeyNameCompany);
    FSString version = this->getKeyValue(e_signatureKeyNameVersion);
    if (!provider.isEmpty()) {
        provider.toUTF8();
        sg_ProviderName = CFX_WideString::FromUTF8(provider.getBuffer());
    }
    if (!company.isEmpty()) {
        company.toUTF8();
        sg_Company = CFX_WideString::FromUTF8(company.getBuffer());
    }
    if (!version.isEmpty()) {
        version.toUTF8();
        sg_Version = CFX_WideString::FromUTF8(version.getBuffer());
    }
    
    if (m_pSignHandler) {
#ifdef ENABLE_EXT_SIGN_CALLBACK
        sg_RefCheckMethod = CFX_WideString::FromLocal(m_pSignHandler->getDigestMethod().c_str());
        sg_SignatureMethod = CFX_WideString::FromLocal(m_pSignHandler->getSignMethod().c_str());
#endif
    }

	pSignature->SetCompany(sg_Company);
	pSignature->SetProviderName(sg_ProviderName);
	pSignature->SetRefCheckMethod(sg_RefCheckMethod);
	pSignature->SetSignatureMethod(sg_SignatureMethod);
	pSignature->SetSignatureDateTime(wsTimeNow);
	pSignature->SetVersion(sg_Version);

	pSignature->SetSealFile(pWriteDoc, m_pSealStream);

	//Sign Begin.
	AUTO_RELEASE_PTR(IOFD_SignHandler, pOFD_SignHandler, OFD_SignHandler_Create(pWriteDoc));
	pOFD_SignHandler->StartSign(pOutputStream.get(), pSignature);

	//Digest Data.
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	auto lamdaGetDigestValue = [&pHTFX_SignHandler](IFX_FileRead* pFileRead)->CFX_ByteString
	{
		CFX_ByteString csValue;
		FX_INT32 nFileSize = pFileRead->GetSize();
		FX_LPBYTE pBuf = new FX_BYTE[nFileSize];
		ZeroMemory(pBuf, nFileSize);

		if (pFileRead->ReadBlock(pBuf, 0, nFileSize))
		{
			int nDigestSize = 0;
			if (pHTFX_SignHandler->Digest(pBuf, nFileSize, NULL, &nDigestSize) == OES_OK)
			{
				FX_LPBYTE pDigestData = new FX_BYTE[nDigestSize];
				ZeroMemory(pDigestData, nDigestSize);

				if (pHTFX_SignHandler->Digest(pBuf, nFileSize, pDigestData, &nDigestSize) == OES_OK)
				{
					csValue = CFX_ByteString((char*)pDigestData, nDigestSize);
				}

				delete[] pDigestData;
				pDigestData = NULL;
			}
		}

		delete[] pBuf;
		pBuf = NULL;


		return csValue;
	};
#else
	GetDigestValue lamdaGetDigestValue = GetDigestValueImpl;
#endif

	int nCount = pSignature->CountReferences();
	for (int i = 0; i < nCount; i++)
	{
		COFD_WriteSignReference* pItem = pSignature->GetReference(i);
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
		CFX_ByteString csValue = lamdaGetDigestValue(pItem->GetFile());
#else
		CFX_ByteString csValue = lamdaGetDigestValue(pHTFX_SignHandler, pItem->GetFile());
#endif

		CFX_ByteString csBase64Value = \
		CR_Base64_Encode(\
		(unsigned char*)csValue.GetBuffer(csValue.GetLength()), csValue.GetLength());

		CFX_WideString wsBase64Value = CFX_WideString::FromLocal(csBase64Value);
		pItem->SetCheckValue(wsBase64Value);
	}
	
	//Sign Digest result.
	IOFD_FileStream* pSignatureFile = (IOFD_FileStream*)pOFD_SignHandler->GetSignatureFile();
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	CFX_ByteString csValue = lamdaGetDigestValue(pSignatureFile);
//	pHTFX_SignHandler
#else
	CFX_ByteString csValue = lamdaGetDigestValue(pHTFX_SignHandler, pSignatureFile);
#endif
	CFX_ByteString csPropInfo = CFX_ByteString::FromUnicode(pSignatureFile->GetCurrentFileName());
    if (csPropInfo[0] != '/') {
		csPropInfo.Insert(0,'/');
    }

    pSignatureFile->Release();

	FX_DWORD dwSize = m_pSealStream->GetSize();
	FX_LPBYTE pSealData = new FX_BYTE[dwSize];
	ZeroMemory(pSealData, dwSize);

	m_pSealStream->ReadBlock(pSealData, 0, dwSize);


	HTFX_SIGN_PARAM signParam;
    if (m_pSignHandler) {
        // do nothing
    } else {
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	signParam.pCertContext = m_pCertData;
#else
	// note by gm, need use file stream;
    CFX_WideString wsFilePath = CFX_WideString::FromUTF8(m_CertPath.c_str());
    AUTO_RELEASE_PTR(IFX_FileRead, pPFXFileRead, FX_CreateFileRead(wsFilePath));
	signParam.strPFXData = GetFileString(pPFXFileRead.get());//m_CretData;// = COFD_OESSignHandler::GetFileString(pPFXFileRead.get());
	signParam.strPassword = m_CertPassword;
#endif
    }
	signParam.strDataHash.append(csValue);
	signParam.strPropInfo.append(csPropInfo);
	signParam.strTimeInfo.append(wsTimeNow.UTF8Encode());
	signParam.strSealData.assign((char*)pSealData, dwSize);

	FX_DWORD dwSignSize= 0;
	if (pHTFX_SignHandler->Sign(signParam, NULL, &dwSignSize) == OES_OK)
	{
		FX_LPBYTE pByte = new FX_BYTE[dwSignSize];
		ZeroMemory(pByte, dwSignSize);

		if (pHTFX_SignHandler->Sign(signParam, pByte, &dwSignSize) == OES_OK)
		{
			pOFD_SignHandler->SetSignedValue(pByte, dwSignSize);
        } else {
            delete[] pByte;
            m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
            LogOut("FR_OFDSignatureImp::startSign, end 2");
            return FSCommonDefines::e_progressError;
        }

		delete[] pByte;
    } else {
        m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
        LogOut("FR_OFDSignatureImp::startSign, end 3");
        return FSCommonDefines::e_progressError;
    }

	pOFD_SignHandler->ContinueSign();
	pOFD_SignHandler->EndSign();

	pOutputStream->Flush();

	m_State = CPDF_SIGNATURE_STATE_SIGNED;
	LogOut("FR_OFDSignatureImp::startSign, end 0");
	return FSCommonDefines::e_progressFinished;
}

CFX_WideString FR_OFDSignatureImp::Time2UTCString(time_t t)
{
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	tm t0;
	if (gmtime_s(&t0, &t) == 0)
	{
#else
	tm* t1 = gmtime(&t);
	if (t1)
	{
		tm& t0 = *t1;
#endif

		CFX_ByteString str;
		FX_LPSTR pBuffer = str.GetBuffer(256);

		strftime(pBuffer, 256, "%y%m%d%H%M%Sz", &t0);
		str.ReleaseBuffer();

		return CFX_WideString::FromLocal(str);
	}

	return L"";
}

	

CFX_ByteString FR_OFDSignatureImp::CR_Base64_Encode(const unsigned char* Data, int DataByte)
{
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

	CFX_ByteString strEncode;
	unsigned char Tmp[4]={0};
	int LineLength=0;
	for(int i=0;i<(int)(DataByte / 3);i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;

		strEncode+= EncodeTable[Tmp[1] >> 2];
		strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode+= EncodeTable[Tmp[3] & 0x3F];
		LineLength+=4;
	}

	int Mod=DataByte % 3;
	if(Mod==1)
	{
		Tmp[1] = *Data++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode+= "==";
	}
	else if(Mod==2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode+= "=";
	}

	return strEncode;
}



CFX_ByteString FR_OFDSignatureImp::CR_Base64_Decode(const char* Data, int DataByte, int& OutByte)
{

	const char DecodeTable[] =
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

	CFX_ByteString strDecode;
	int nValue;
	int i= 0;
	while (i < DataByte)
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
			OutByte++;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode+=(nValue & 0x0000FF00) >> 8;
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode+=nValue & 0x000000FF;
					OutByte++;
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
	return strDecode;

}

FS_PROGRESSSTATE FR_OFDSignatureImp::continueSign()
{
	if (m_State == CPDF_SIGNATURE_STATE_SIGNED) {
		return FSCommonDefines::e_progressFinished;
	} else if (m_State == CPDF_SIGNATURE_STATE_UNSIGNED) {
		return FSCommonDefines::e_progressError;
	} else if (m_State == CPDF_SIGNATURE_STATE_UNKNOWN) {
		return FSCommonDefines::e_progressError;
	}
	return FSCommonDefines::e_progressError;
}

FS_PROGRESSSTATE FR_OFDSignatureImp::startVerify(FSPause* pause, void* clientData)
{
#if (_FX_OS_ == _FX_ANDROID_) || (_FX_OS_ == _FX_IOS_)
	LogOut("Test_C--->startVerify begin:");
	bool isOnline = false;
	FX_BOOL nflags = isOnline ? TRUE : FALSE;
	COFD_Signature* pSeal =	m_pSignature;
	LogOut("Test_C--->startVerify pSeal:%p",pSeal);

    CFX_WideString checkMethod = pSeal->GetRefCheckMethod();
	CFX_WideString signMethod = pSeal->GetSignatureMethod();
	CFX_ByteString bsSignMethod = (FX_LPCSTR)signMethod.UTF8Encode();
	CFX_ByteString bsSignValue = pSeal->GetSignatureValue();
	IFX_FileRead* pSealFile = pSeal->GetSealFile();
	FX_DWORD dwSealDataLen = pSealFile->GetSize();

	FX_LPBYTE pSealData = FX_Alloc(FX_BYTE, dwSealDataLen+1 );
    AutoFxMemory autuSealData(pSealData);
	::ZeroMemory(pSealData, dwSealDataLen);
	pSealFile->ReadBlock(pSealData, 0, dwSealDataLen);
	CFX_WideString signDataTime = pSeal->GetSignatureDateTimeString();

	FX_INT32 refersCount = pSeal->CountReferences();
	LogOut("Test_C--->startVerify refersCount=%d doing_1:",refersCount);

	FX_LPBYTE	pbTxts	= NULL;
	FX_DWORD    cbTxts = 0;
	FX_LPBYTE	pbSign = NULL;
	unsigned int cbSign = 0;

	FX_LPBYTE pDigestMethod = NULL;
    AutoFxMemory autoDigestMethod;
	int nDigestMethodLen = 0;
    int& dwRes = m_OESRet;
    dwRes = OES_GetDigestMethod(NULL, &nDigestMethodLen);
	if (OES_OK == dwRes) {
		pDigestMethod = FX_Alloc(FX_BYTE, nDigestMethodLen);
        autoDigestMethod.attach(pDigestMethod);
		::ZeroMemory(pDigestMethod, nDigestMethodLen);
		dwRes = OES_GetDigestMethod(pDigestMethod, &nDigestMethodLen);
		if (OES_OK != dwRes) {
            m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
            return FSCommonDefines::e_progressError;
		}
	} else {
        m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
        return FSCommonDefines::e_progressError;
	}
    
    CFX_WideString oesDigestMethod = CFX_WideString::FromUTF8((char*)pDigestMethod, nDigestMethodLen);
    if (checkMethod != oesDigestMethod) {
        // check(digest) method not match
        m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
        return FSCommonDefines::e_progressError;
    }

	FX_DWORD offer = 0;
	FX_LPBYTE lpDigestData = NULL;
	int  cpDigestData = 0;
    AutoFxMemory autoTxt;
    AutoFxMemory autoDigestData;

	for (FX_INT32 j = 0; j < refersCount; j++) {
		COFD_SignReference* refer = pSeal->GetReference(j);
		IFX_FileRead* file = refer->GetFile();
		if (file != NULL) {
			cbTxts = file->GetSize();
			pbTxts = FX_Alloc(FX_BYTE, cbTxts);
            autoTxt.attach(pbTxts);
			::ZeroMemory(pbTxts, cbTxts);
			file->ReadBlock((FX_LPVOID)(pbTxts), 0, cbTxts);

			//对每个refer进行has
			dwRes = OES_Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, NULL, &cpDigestData);
			LogOut("Test_C--->startVerify dwRes_1=%d",dwRes);
			if(OES_OK == dwRes) {
				lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
                autoDigestData.attach(lpDigestData);
				::ZeroMemory(lpDigestData, cpDigestData);
				dwRes = OES_Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, lpDigestData, &cpDigestData);
				LogOut("Test_C--->startVerify dwRes_2=%d",dwRes);
				if (OES_OK != dwRes) {
                    m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
                    return FSCommonDefines::e_progressError;
				}
			} else {
                m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
                return FSCommonDefines::e_progressError;
			}

			LogOut("Test_C--->startVerify doing_2:");
			CFX_ByteString fxCheckMethod = CR_Base64_Encode((unsigned char *)lpDigestData, cpDigestData);
			CFX_ByteString wsNewHashData = fxCheckMethod;
			CFX_ByteString wcOldHashData = refer->GetCheckValue().UTF8Encode();
			LogOut("Test_C--->startVerify old hash=%s",(FX_LPCSTR)wcOldHashData);
			LogOut("Test_C--->startVerify new hash=%s",(FX_LPCSTR)wsNewHashData);
			
			if (wsNewHashData.Compare(wcOldHashData) != 0){
				LogOut("Test_C--->startVerify doing_3:");
                m_State = CPDF_SIGNATURE_STATE_VERIFY_CHANGE;
				return FSCommonDefines::e_progressError;
			}
		}

	}

	IFX_FileRead* fileSignature = pSeal->GetFile();
	cbTxts = fileSignature->GetSize();
	pbTxts = FX_Alloc(FX_BYTE, cbTxts);
    autoTxt.attach(pbTxts);
	::ZeroMemory(pbTxts, cbTxts);
	fileSignature->ReadBlock((FX_LPVOID)(pbTxts), 0, cbTxts);

	//对Signature.xml计算摘要
	dwRes = OES_Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, NULL, &cpDigestData);
		if(OES_OK == dwRes)
	{
		lpDigestData = FX_Alloc(FX_BYTE, cpDigestData);
        autoDigestData.attach(lpDigestData);
		::ZeroMemory(lpDigestData, cpDigestData);
		dwRes = OES_Digest(pbTxts, cbTxts, pDigestMethod, nDigestMethodLen, lpDigestData, &cpDigestData);
		if (OES_OK != dwRes) {
            m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
			return FSCommonDefines::e_progressError;
		}
	} else {
        m_State = CPDF_SIGNATURE_STATE_UNKNOWN;
		return FSCommonDefines::e_progressError;
	}

    CFX_ByteString byteSignTime = CFX_ByteString::FromUnicode(signDataTime);
	//FX_BOOL bOnLine = COFD_Module::GetOFDGlobalPermanentData()->m_bOpenInBrowser;
	CFX_WideString fileName = ((IOFD_FileStream *)fileSignature)->GetCurrentFileName(0);

	CFX_ByteString docProperty = CFX_ByteString::FromUnicode(fileName);
	//////////////////////////////
	FX_LPBYTE fxDocProperty = (FX_LPBYTE)docProperty.GetBuffer(docProperty.GetLength());
    if (fxDocProperty[0] != '/') {
		docProperty.Insert(0,'/');
    }
	//CFX_ByteString docProperty = "/Doc_0/Signatures.xml";//temp

	FX_DWORD ret = OES_Verify(
		(FX_LPBYTE)pSealData,
		dwSealDataLen,
		(FX_LPBYTE)docProperty.GetBuffer(docProperty.GetLength()),
		docProperty.GetLength(),
		(FX_LPBYTE)lpDigestData,
		cpDigestData,
		(FX_LPBYTE)bsSignMethod.GetBuffer(bsSignMethod.GetLength()),
		bsSignMethod.GetLength(),
		(FX_LPBYTE)byteSignTime.GetBuffer(byteSignTime.GetLength()),
		byteSignTime.GetLength(),
		(unsigned char *)bsSignValue.GetBuffer(bsSignValue.GetLength()),
		bsSignValue.GetLength(),
		nflags);

	if (ret != OES_OK) {
        m_OESRet = ret;
        m_State = CPDF_SIGNATURE_STATE_VERIFY_INVALID;
        return FSCommonDefines::e_progressError;
	}

	LogOut("Test_C--->startVerify end!");
#endif
    m_State = CPDF_SIGNATURE_STATE_VERIFY_VALID;
	return FSCommonDefines::e_progressFinished;
}

FS_PROGRESSSTATE FR_OFDSignatureImp::continueVerify()
{
    if (m_State == CPDF_SIGNATURE_STATE_VERIFY_VALID) {
        return FSCommonDefines::e_progressFinished;
    }
    return FSCommonDefines::e_progressError;
}

FS_DWORD FR_OFDSignatureImp::getState()
{
	return m_State;
}

FS_BOOL	FR_OFDSignatureImp::clearSignedData()
{
	return FALSE;
}

FSPDFDoc*	FR_OFDSignatureImp::getDocument()
{
    return NULL;
}

FS_DWORD FR_OFDSignatureImp::getAppearanceFlags()
{
	return 0;
}

void FR_OFDSignatureImp::setAppearanceFlags(FS_DWORD apFlags)
{

}

FSString FR_OFDSignatureImp::getKeyValue(FS_SIGNATUREKEYNAME key)
{
	string value = m_Value[key];
	if (value.empty()) {
		return FSString();
	} else {
		return FSString(value.c_str());
	}
}

void FR_OFDSignatureImp::setKeyValue(FS_SIGNATUREKEYNAME key, const char* value)
{
	m_Value[key] = value;
}

FSDateTime FR_OFDSignatureImp::getSigningTime()
{
    if (m_pSignature) {
        FSDateTime result;
        fxutil::CFX_Unitime date = m_pSignature->GetSignatureDateTime();
        result.set(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), date.GetMinute(), date.GetSecond(), date.GetMillisecond(), 0, 0);
        return result;
    }
	return m_SignDate;
}

void FR_OFDSignatureImp::setSigningTime(FSDateTime signTime)
{
	m_SignDate = signTime;
}

FSBitmap* FR_OFDSignatureImp::getBitmap()
{
	return m_Bitmap;
}

void FR_OFDSignatureImp::setBitmap(FSBitmap* bitmap)
{
	// need clone it
	//m_Bitmap = bitmap;
}

void FR_OFDSignatureImp::setImagePath(const char* path)
{
	m_ImagePath = path;
}

FSPDFDictionary*	FR_OFDSignatureImp::getSignatureDict()
{
	return NULL;
}

void FR_OFDSignatureImp::setAppearanceContent(const char* appearanceContent)
{

}
    
void FR_OFDSignatureImp::setSignatureHandler(FSSignatureHandler* handler)
{
#ifdef ENABLE_EXT_SIGN_CALLBACK
    m_pSignHandler = new FR_OFDSignatureHandler(handler);
#endif
}

FR_OFDSignatureImp::FR_OFDSignatureImp()
	: m_pPage(NULL)
	, m_Rect()
	, m_pCertData()
	, m_CertPath()
	, m_CertPassword()
	, m_Bitmap(NULL)
	, m_pSealStream(NULL)
	, m_pSignature(NULL)
	, m_State(CPDF_SIGNATURE_STATE_UNKNOWN)
    , m_OESRet(0)
    , m_pSignHandler(NULL)
{

}

FR_OFDSignatureImp::~FR_OFDSignatureImp()
{
	// need release members
	if (m_pSealStream) {
		m_pSealStream->Release();
		m_pSealStream = NULL;
	}
    
    if (m_pSignHandler) {
        delete m_pSignHandler;
        m_pSignHandler = NULL;
    }
}

void		FR_OFDSignatureImp::init(FR_OFDPageImp* page/*,IOFD_Page* mPage*/, FSRectF rect)
{
	m_pPage = page;
	FSRectF temp;
	temp.left 	= PT_2_MM(rect.left);
	temp.top 	= PT_2_MM(page->getHeight() - rect.top);
	temp.right 	= PT_2_MM(rect.right);
	temp.bottom 	= PT_2_MM(page->getHeight() - rect.bottom);
	
	m_Rect.left = temp.left;
	m_Rect.top = FX_MIN(temp.top, temp.bottom);
	m_Rect.width = FXSYS_fabs(temp.right - temp.left);
	m_Rect.height = FXSYS_fabs(temp.bottom - temp.top);
	m_State = CPDF_SIGNATURE_STATE_UNSIGNED;
}

void		FR_OFDSignatureImp::init(COFD_Signatures* signs, int index)
{
	m_pSignature = signs->GetSignature(index);
	m_State = CPDF_SIGNATURE_STATE_SIGNED;
}

FS_BOOL		FR_OFDSignatureImp::GenerateSeal(time_t tmNow)
{
    // create seal
//    time_t tmNow = time(NULL);
	time_t tmEnd = tmNow * 1.2;
	CFX_WideString wsTimeNow = Time2UTCString(tmNow);
	CFX_WideString wsTimeEnd = Time2UTCString(tmEnd);

	//构建印章结构
	HTFX_ESEAL_PARAM esealParam;
	{
		//name id pictype
		CFX_WideString wsName = L"test_seal";
		CFX_WideString wsGUID = GenerateGUID();
		CFX_WideString wsPicType = L"png";
        
        FSString sealid = this->getKeyValue(e_signatureKeyNameSealId);
        FSString sealname = this->getKeyValue(e_signatureKeyNameSealName);
        if (!sealid.isEmpty()) {
            sealid.toUTF8();
            wsGUID = CFX_WideString::FromUTF8(sealid.getBuffer());
        }
        if (!sealname.isEmpty()) {
            sealname.toUTF8();
            wsName = CFX_WideString::FromUTF8(sealname.getBuffer());
        }

		esealParam.strName.append(wsName.UTF8Encode());
		esealParam.esID.append(wsGUID.UTF8Encode());
		esealParam.strPicType.append(wsPicType.UTF8Encode());

		//width heignt
		esealParam.lnWidth = m_Rect.Width();
		esealParam.lnHeight = m_Rect.Height();

		CFX_WideString wsTimeStart = wsTimeNow;
		CFX_WideString wsTimeCreate = wsTimeNow;

		esealParam.strCreateDate.append(wsTimeCreate.UTF8Encode());
		esealParam.strStartDate.append(wsTimeStart.UTF8Encode());
		esealParam.strEndDate.append(wsTimeEnd.UTF8Encode());

		//PicData
		string picname = m_ImagePath;
        
		IFX_FileRead* pngStream = FX_CreateFileRead(picname.c_str());
		if (!pngStream) {
			return FALSE;
		}

		esealParam.strPicData = GetFileString(pngStream);
        pngStream->Release();

		//CertData
		string strPFX = m_CertPath;			
		string strPassword = m_CertPassword;
        
        // modify by gm 
    if (m_pSignHandler) {
#ifdef ENABLE_EXT_SIGN_CALLBACK
		// callback
        m_CretData = m_pSignHandler->getCertData();
        if (m_CretData.empty()) {
            return FALSE;
        }
        esealParam.strCertData = m_CretData;
#endif
    } else {
		
		FILE* file  = fopen(strPFX.c_str(),"rb");
        if (!file) {
            return FALSE;
        }
		fseek(file,0L,SEEK_END);  
		long nLen= ftell(file);
		rewind(file);
		char* pBuffer = new char[nLen];
		fread(pBuffer,1,nLen,file);
		fclose(file);
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
		PCCERT_CONTEXT pCert = NULL;
		CRYPT_DATA_BLOB CDB;//生成结构体
		CDB.cbData = (DWORD)nLen;
		CDB.pbData = (BYTE*)pBuffer;

		CFX_WideString wsPassword = CFX_WideString::FromUTF8(strPassword.c_str());
		auto hStore = PFXImportCertStore(&CDB, (LPWSTR)wsPassword.GetBuffer(wsPassword.GetLength()), CRYPT_EXPORTABLE);
		if (hStore)
		{
			PCCERT_CONTEXT pCertContext = NULL;
			while (pCertContext = CertEnumCertificatesInStore(hStore, pCertContext))
			{
				if (IsPrivateKeyCert(pCertContext))
				{
					pCert = pCertContext;
					break;
				}
			}
		}
		else
		{
            return FALSE;
		}
		if (pCert)
		{
			esealParam.strCertData.assign((char*)pCert->pbCertEncoded, pCert->cbCertEncoded);
			m_pCertData = pCert;
			//return TRUE;
		}
#else
		// android & ios
		m_CretData = oes_foxit::GetCertData(CFX_WideString::FromUTF8(m_CertPath.c_str()),m_CertPassword.c_str());
		esealParam.strCertData = m_CretData;
#endif
    } // end of if (m_pSignHandler)

	}
	AUTO_RELEASE_PTR(IHTFX_Eseal, pSeal, IHTFX_Eseal::Create(esealParam));

	std::string strSealData;
#if (_FX_OS_==_FX_WIN32_DESKTOP_)
	pSeal->OutputSealData(&strSealData);
#else
	pSeal->OutputSealData(strSealData);
#endif
    //将数据封装为印章
    CFX_WideString sg_seal_esl = L"Seal.esl";
    m_pSealStream = OFD_CreateMemoryStream(sg_seal_esl);
	m_pSealStream->WriteBlock(strSealData.data(), 0, strSealData.length());
	m_pSealStream->Flush();
	return TRUE;
}

CFX_WideString FR_OFDSignatureImp::GenerateGUID()
{
	CFX_WideString wsGUID;

	time_t tm = time(NULL);
	srand(tm);

	for (int i = 0; i < 15; i++)
	{
		srand(rand());

		int x = rand() % 10;

		char szID[10] = { 0 };

		sprintf(szID,"%d",x);

		CFX_WideString dszID = CFX_WideString::FromUTF8(szID);

		wsGUID += dszID;
	}
	return wsGUID;
}

std::string FR_OFDSignatureImp::GetFileString(IFX_FileRead* pFileRead)
{
    std::string strFileData;
    int nPicSize = pFileRead->GetSize();

    char* tempBuf = new char[nPicSize];
    memset(tempBuf, 0, nPicSize);

    pFileRead->ReadBlock(tempBuf, 0, nPicSize);

    strFileData.assign(tempBuf, nPicSize);

    delete[] tempBuf;

    return strFileData;
}

#if (_FX_OS_==_FX_WIN32_DESKTOP_)
FX_BOOL	FR_OFDSignatureImp::IsPrivateKeyCert(PCCERT_CONTEXT pCertContext)
{
	BOOL bFree;
	DWORD dwKeySpec = 0;
	HCRYPTPROV hCryptProv = NULL;
	BOOL bHasPrivateKey = FALSE;

	if ( ::CryptAcquireCertificatePrivateKey(
		pCertContext,
		CRYPT_ACQUIRE_CACHE_FLAG | CRYPT_ACQUIRE_USE_PROV_INFO_FLAG | CRYPT_ACQUIRE_COMPARE_KEY_FLAG
		| 0x00000008 |CRYPT_ACQUIRE_PREFER_NCRYPT_KEY_FLAG,
		NULL,
		&hCryptProv,
		&dwKeySpec,
		&bFree) )
	{
		bHasPrivateKey = IsPrivateHasUsage(pCertContext);
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bHasPrivateKey;
}

FX_BOOL FR_OFDSignatureImp::IsPrivateHasUsage(PCCERT_CONTEXT pCertContext)
{
	PCERT_ENHKEY_USAGE pCrtUsage = NULL;
	DWORD dwUsage;
	BOOL bHasEnhance = FALSE;
	BOOL bRet = ::CertGetEnhancedKeyUsage(
		pCertContext, 
		CERT_FIND_EXT_ONLY_ENHKEY_USAGE_FLAG, 
		NULL, 
		&dwUsage);
	if (bRet && dwUsage>0)
	{
		pCrtUsage = (PCERT_ENHKEY_USAGE)malloc(sizeof(char) * dwUsage);
		::CertGetEnhancedKeyUsage
			(pCertContext, 
			CERT_FIND_EXT_ONLY_ENHKEY_USAGE_FLAG, 
			pCrtUsage, 
			&dwUsage);
		if (pCrtUsage->rgpszUsageIdentifier)
			bHasEnhance = TRUE;
	}

	BYTE usage;
	::CertGetIntendedKeyUsage(pCertContext->dwCertEncodingType, 
		pCertContext->pCertInfo,
		&usage, 
		1);
	BOOL bIsPKC = FALSE;
	if (	( (usage & CERT_DIGITAL_SIGNATURE_KEY_USAGE) == CERT_DIGITAL_SIGNATURE_KEY_USAGE )
		||	( (usage == 0) /*&& !bHasEnhance */)) //没有设置key usage 而只enhanced key usge的，可以用来签名，Mantis ID 0048391
	{
		bIsPKC = TRUE;
	}
	if (pCrtUsage)
	{
		free(pCrtUsage);
	}

	return bIsPKC;
}
#endif

//pOutputStream:指针指向印章全路径，以读写的权限操作！
FX_INT32	FR_OFDSignatureImp::CreateCertSign(IFX_FileWrite* pOutputStream)
{
	//m_rtDrag:宽高 m_nLDPageIndex:页数 rtSign：签名框大小
	//CFX_RectF rtSign;
	//m_pLDPageView->ClientPointToOFDPoint_EX(m_nLDPageIndex, rtSign, m_rtDrag);
	{

	}
	return 0;
}

/*
COFD_Signature*	FR_OFDSignatureImp::GetSignature(COFD_Signatures* signs,int index){
	COFD_Signature* signature = signs->GetSignature(index);
	LogOut("Test_C--->signature_1:0x%p",&signature);
	return signature;
}*/

/*
FX_INT32	FR_OFDSignatureImp::GetSignatureCount(IOFD_Parser* Parser){
	int result = 0;
	m_pOFDParser = Parser;
	for(int i=0;i<m_pOFDParser->CountDocuments();++i){
		IOFD_Document* ofdDoc = m_pOFDParser->GetDocument(i);
		if (NULL == ofdDoc){
			continue;
		}
		signs = ofdDoc->GetSignatures();
		if (NULL == signs) {
			continue;
		}
		result = signs->CountSignature();
	}
	return result;
}
*/
/*
// class CoordinatorAssistant
CoordinatorAssistant::CoordinatorAssistant(IOFD_Page* page)
: m_Page(page)
{

}

void			CoordinatorAssistant::toPDF(FSRectF& rect) {
	toPDF(rect, rect);
}

void			CoordinatorAssistant::toPDF(const FSRectF& src, FSRectF& dst)
{
	dst.left 	= MM_2_PT(src.left);
	dst.top 	= m_Page->getHeight() - MM_2_PT(src.top);
	dst.right 	= MM_2_PT(src.right);
	dst.bottom 	= m_Page->getHeight() - MM_2_PT(src.bottom);
}

void			CoordinatorAssistant::toPDF(const CFX_RectF& src, FSRectF& dst)
{
	FSRectF tmp = { src.left, src.top, src.right(), src.bottom()};
	toPDF(tmp, dst);
}

void			CoordinatorAssistant::toPDF(const CFX_FloatRect& src, FSRectF& dst)
{
	FSRectF tmp = { src.left, src.top, src.right, src.bottom};
	toPDF(tmp, dst);
}

void			CoordinatorAssistant::fromPDF(FSRectF& rect)
{
	fromPDF(rect, rect);
}

void			CoordinatorAssistant::fromPDF(const FSRectF& src, FSRectF& dst)
{
	dst.left 	= PT_2_MM(src.left);
	dst.top 	= PT_2_MM(m_Page->getHeight() - src.top);
	dst.right 	= PT_2_MM(src.right);
	dst.bottom 	= PT_2_MM(m_Page->getHeight() - src.bottom);
}

void			CoordinatorAssistant::fromPDF(const FSRectF& src, CFX_FloatRect& dst)
{
	FSRectF tmp;
	fromPDF(src, tmp);
	dst.left 	= tmp.left;
	dst.top		= tmp.top;
	dst.right	= tmp.right;
	dst.bottom	= tmp.bottom;
}

void			CoordinatorAssistant::fromPDF(const FSRectF& src, CFX_RectF& dst)
{
	FSRectF tmp;
	fromPDF(src, tmp);
	dst.Set(tmp.left, tmp.top, tmp.right-tmp.left, tmp.bottom-tmp.top);
}

void			CoordinatorAssistant::toPDF(float& x, float& y)
{
	x = MM_2_PT(x);
	y = m_Page->GetHeight() - MM_2_PT(y);
}

void			CoordinatorAssistant::fromPDF(float& x, float& y)
{
	x = PT_2_MM(x);
	y = PT_2_MM(m_Page->GetHeight() - y);
}

void			CoordinatorAssistant::toPDF(float& len)
{
	len = m_Page->GetHeight() - MM_2_PT(len);
}

void			CoordinatorAssistant::fromPDF(float& len)
{
	len = PT_2_MM(m_Page->GetHeight() - len);
}
*/
