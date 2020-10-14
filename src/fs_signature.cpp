#include "../include/common.h"
#include "inc/fs_signatureImp.h"
#include "../include/fs_signature.h"
#include "../include/fs_pdfpage.h"
//#include "../include/fs_pdfobject.h"
#include "fs_util.h"
//#include "inc/fs_pdfpageImp.h"
#include "form/fs_pdfformImp.h"
#include "fs_pause.h"
#include "annotation/fs_annoticonprovider.h"

#if defined(ENABLE_SIGNATURE)
FS_PROGRESSSTATE FSCRT_Util_TransformProgressStatusFromFxcore(FX_ProgressiveStatus fxStatus)
{
	switch(fxStatus)
	{
	case FXCORE_PROGRESSIVESTATUS_READY:
	case FXCORE_PROGRESSIVESTATUS_TOBECONTINUED:
		return FSCommonDefines::e_progressToBeContinued;
	case FXCORE_PROGRESSIVESTATUS_FOUND:
	case FXCORE_PROGRESSIVESTATUS_DONE:
		return FSCommonDefines::e_progressFinished;
	case FXCORE_PROGRESSIVESTATUS_NOTFOUND:
	case FXCORE_PROGRESSIVESTATUS_FAILED:
	default:
		return FSCommonDefines::e_progressError;
	}
}

FX_ProgressiveStatus FSCRT_Util_TransformProgressStatusToFxcore(FS_PROGRESSSTATE fsState)
{
	switch(fsState)
	{
	case FSCommonDefines::e_progressToBeContinued:
		return (FX_ProgressiveStatus)FXCORE_PROGRESSIVESTATUS_TOBECONTINUED;
	case FSCommonDefines::e_progressFinished:
		return (FX_ProgressiveStatus)FXCORE_PROGRESSIVESTATUS_DONE;
	case FSCommonDefines::e_progressError:
	default:
		return (FX_ProgressiveStatus)FXCORE_PROGRESSIVESTATUS_FAILED;
	}
}

///////////////////////////////////////////////////////
// CFSRDK_FileRead
CFSRDK_FileRead::CFSRDK_FileRead(IFX_FileRead* pFxFileRead)
	: m_pFxFileRead(pFxFileRead) 
{
}

CFSRDK_FileRead::~CFSRDK_FileRead()
{
}

FS_BOOL	CFSRDK_FileRead::readBlock(void* buffer, FS_UINT64 offset, FS_UINT64 size)
{
	if (!m_pFxFileRead)
		return FALSE;
	return m_pFxFileRead->ReadBlock(buffer, offset, size);
}

FS_UINT64	CFSRDK_FileRead::getSize() 
{
	if (!m_pFxFileRead)
		return FALSE;
	return m_pFxFileRead->GetSize();
}

void	CFSRDK_FileRead::release()
{
	if (m_pFxFileRead)
	{
		m_pFxFileRead->Release();
		m_pFxFileRead = NULL;
	}
	delete this;
}
/////////////////////////////////////////////////////////
// CFSPDF_SignatureHandler
CFSPDF_SignatureHandler::CFSPDF_SignatureHandler()
	:m_pFSSigHandler(NULL) 
	,m_pFSFileRead(NULL)
{

}

CFSPDF_SignatureHandler::CFSPDF_SignatureHandler(FSSignatureHandler* pSigHandler)
	:m_pFSSigHandler(pSigHandler)
	,m_pFSFileRead(NULL)
{

}

CFSPDF_SignatureHandler::~CFSPDF_SignatureHandler() 
{
	m_pFSSigHandler = NULL;
	if (m_pFSFileRead)
		SAFE_DELETE(m_pFSFileRead);
}

FX_BOOL		CFSPDF_SignatureHandler::StartCalcDigest(IFX_FileRead* pFile, const CFX_DWordArray& byteArray, CPDF_Signature* pSig, FX_LPVOID pClientData /*= NULL*/)
{
	if (!m_pFSSigHandler || !pClientData) return FALSE;
	if (m_pFSFileRead)
	{
		delete m_pFSFileRead;
		m_pFSFileRead = NULL;
	}
	m_pFSFileRead = FX_NEW CFSRDK_FileRead(pFile);
	return m_pFSSigHandler->startCalcDigest(m_pFSFileRead, byteArray.GetData(), byteArray.GetSize(), 
		((FSSignatureClientData*)pClientData)->pFSSig, ((FSSignatureClientData*)pClientData)->pClientData);
}

FX_INT32	CFSPDF_SignatureHandler::ContinueCalcDigest(IFX_Pause* pPause /*= NULL*/, FX_LPVOID pClientData /*= NULL*/) 
{
	if (!m_pFSSigHandler || !pClientData) return FXCORE_PROGRESSIVESTATUS_FAILED;

	FS_PROGRESSSTATE progressState = FSCommonDefines::e_progressError;
	if (!pPause)
		progressState = m_pFSSigHandler->continueCalcDigest(NULL, ((FSSignatureClientData*)pClientData)->pClientData);
	else 
	{
		CFSRDK_Pause tempPause(pPause);
		progressState = m_pFSSigHandler->continueCalcDigest(&tempPause, ((FSSignatureClientData*)pClientData)->pClientData);
	}
	return FSCRT_Util_TransformProgressStatusToFxcore(progressState);
}

CFX_ByteString		CFSPDF_SignatureHandler::GetDigest(FX_LPVOID pClientData /*= NULL*/) const
{
	if (!m_pFSSigHandler || !pClientData) return CFX_ByteString();

	FSString sDigest = m_pFSSigHandler->getDigest(((FSSignatureClientData*)pClientData)->pClientData);
	CFX_ByteString bsDigest(sDigest.getBuffer(), sDigest.getBufferLen());
	return bsDigest;
}

FX_BOOL		CFSPDF_SignatureHandler::Sign(const CFX_ByteString& bsDigest, CFX_ByteString& bsSignedData, FX_LPVOID pClientData /*= NULL*/)
{
	if (!m_pFSSigHandler || !pClientData) return FALSE;

	CFX_ByteString tempDigest = (CFX_ByteString)bsDigest;
	FS_DWORD dwDigestLen = tempDigest.GetLength();

	CFX_WideString certPath;
	CFX_WideString certPass;
	if(((FSSignatureClientData*)pClientData))
	{
		FSSignatureImp* sig = (FSSignatureImp*)((FSSignatureClientData*)pClientData)->pFSSig;
		sig->getPDFSignature()->GetSignerCertPath(certPath, certPass);
	}
	CFX_ByteString bsPath = certPath.UTF8Encode();
	CFX_ByteString bsPass = certPass.UTF8Encode();
	FSString sSignedData = m_pFSSigHandler->sign((void*)tempDigest.GetBuffer(dwDigestLen), dwDigestLen, (FX_LPCSTR)bsPath,
		(FX_LPCSTR)bsPass, bsPass.GetLength(), ((FSSignatureClientData*)pClientData)->m_algorithm, ((FSSignatureClientData*)pClientData)->pClientData);

	tempDigest.ReleaseBuffer();
	if (sSignedData.isEmpty()) return FALSE;
	bsSignedData = CFX_ByteString(sSignedData.getBuffer(), sSignedData.getBufferLen());
	return TRUE;
}

FX_BOOL		CFSPDF_SignatureHandler::Verify(const CFX_ByteString& bsDigest, const CFX_ByteString& bsSignedData, FX_DWORD& dwVerResult, FX_LPVOID pClientData /*= NULL*/)
{
	if (!m_pFSSigHandler || !pClientData) return FALSE;

	CFX_ByteString tempDigest = (CFX_ByteString)bsDigest;
	FS_DWORD dwDigestLen = tempDigest.GetLength();
	CFX_ByteString tempSignedData = (CFX_ByteString)bsSignedData;
	FS_DWORD dwSignedLen = tempSignedData.GetLength();
	dwVerResult = m_pFSSigHandler->verify(tempDigest.GetBuffer(dwDigestLen), dwDigestLen, tempSignedData.GetBuffer(dwSignedLen), dwSignedLen, ((FSSignatureClientData*)pClientData)->pClientData);
	return (dwVerResult&FSSignatureDefines::e_signatureStateVerifyValid)?TRUE:FALSE;
}

////////////////////////////////////////////////////////
//
FSSignatureImp::FSSignatureImp(CPDF_Signature* pSignature, FSPDFPage* pPage, FS_BOOL bNewAdd, FS_BOOL bCreateVDict)
	: m_pSignature(pSignature)
	, m_bNewAdd(bNewAdd)
	, m_pImage(NULL)
	, m_bNewImage(FALSE)
	, m_pSigSign(NULL)
	, m_pSignFile(NULL)
	, m_pSignPause(NULL)
	, m_pSigVerify(NULL)
	, m_pVerifyPause(NULL)
	, m_pAPForm(NULL)
	, m_bCreateSigVDict(bCreateVDict)
{
	if (pSignature)
		FSFormControlImp::init(pSignature->GetSignatureDict(), pPage);
}

FSSignatureImp::~FSSignatureImp()
{
	if (m_pImage)
	{
		if(m_bNewImage)
			delete m_pImage;
		m_pImage = NULL;
		m_bNewImage = FALSE;
	}
	if (m_pSigVerify)
		SAFE_DELETE(m_pSigVerify);
	if (m_pSigSign)
		SAFE_DELETE(m_pSigSign);
	if (m_pSignFile)
	{
		m_pSignFile->Release();
		m_pSignFile = NULL;
	}
	if (m_pAPForm)
		SAFE_DELETE(m_pAPForm);

	//No bother to delete it , looks like core will do the job.
	//if (m_bNewAdd)
	//	delete m_pSignature;
	m_pSignature = NULL;
	m_pSignPause = NULL;
	m_pVerifyPause = NULL;
	m_signClientData.pClientData = NULL;
	m_signClientData.pFSSig = NULL;
	m_verifyClientData.pClientData = NULL;
	m_verifyClientData.pFSSig = NULL;
}

FS_BOOL FSSignatureImp::move(FSRectF rect)
{
	return FSPDFAnnotImp::moveImp((FSFormControl*)(FSSignature*)this,rect, FALSE);
}

FS_BOOL FSSignatureImp::resetAppearanceStream()
{
	//TODO: how to use this?? limit not support? or else?
	return FALSE;
}

FSPDFDictionary* FSSignatureImp::getDict() const
{
	return (FSPDFDictionary*)m_pSignature->GetSignatureDict();
}

// FSSignature
FS_BOOL	FSSignatureImp::isSigned()
{
	if (!m_pSignature) return FALSE;
	
	if (CPDF_SIGNATURE_STATE_UNSIGNED & m_pSignature->GetVerifyState())
		return FALSE;
	return TRUE;
}


 FS_PROGRESSSTATE FSSignatureImp::startSign(const char* savePath, const char* certPath, const char password[], FS_INT32 passwordLen, FS_DIGEST_ALGORITHM digestAlgorithm, FSPause* pause, void* clientData)
{
	if (!m_pSignature || !getPage() || !getPage()->getDocument() 
		|| isSigned() || FSCRT_Util_IsStringEmpty(certPath)
		|| digestAlgorithm<FSSignatureDefines::e_digestSHA1 || digestAlgorithm>FSSignatureDefines::e_digestSHA512
		|| (passwordLen>0 && !password)
		)
		return FSCommonDefines::e_progressError;
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (m_pSignFile)
		SAFE_DELETE(m_pSignFile);
	m_pSignFile = FSCRT_Util_CreateFileFromPath(savePath, FX_FILEMODE_Truncate);
	if (!m_pSignFile) return FSCommonDefines::e_progressError;

	if (m_pSigSign)
		SAFE_DELETE(m_pSigSign);
	
	m_pSignature->SetSignerCertPath(CFX_WideString::FromUTF8(certPath), CFX_WideString::FromUTF8(password, passwordLen));

	CPDF_Document* pDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
	m_signClientData.pClientData = clientData;
	m_signClientData.pFSSig = this;
	m_signClientData.m_algorithm = digestAlgorithm;
	m_pSigSign = FX_NEW CPDF_SignatureSign(pDoc, m_pSignature, m_pSignFile, &m_signClientData);
	if (m_pSigSign)
	{
		FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
		if (FSCRT_ERRCODE_SUCCESS == ret)
		{
			// Generate AP by RDK and used for signing.
			CPDF_Form* pAPForm = generateSignAP();
			if (pAPForm)
			{
				m_pSignature->ResetAppearance(pAPForm);
				if (m_pSigSign->Start())
				{
					m_pSignPause = pause;
					return continueSign();
				}
			}
		}
	}
	// Failed, so release resources and return error.
	m_pSignFile->Release();
	m_pSignFile = NULL;
	return FSCommonDefines::e_progressError;
}

FS_PROGRESSSTATE FSSignatureImp::continueSign()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!m_pSignature || !getPage() || !getPage()->getDocument()
		|| !m_pSigSign)
		return FSCommonDefines::e_progressError;

	FX_ProgressiveStatus status = (FX_ProgressiveStatus)FXCORE_PROGRESSIVESTATUS_FAILED;
	if (m_pSignPause)
	{
		CFSCRT_Pause tempPause(m_pSignPause);
		status = m_pSigSign->Continue(&tempPause);
	}
	else
		status = m_pSigSign->Continue(NULL);
	
	FS_PROGRESSSTATE progressState = FSCRT_Util_TransformProgressStatusFromFxcore(status);
	if (FSCommonDefines::e_progressToBeContinued != progressState)
	{
		m_pSignFile->Release();
		m_pSignFile = NULL;
	}
	return progressState;
}

FS_PROGRESSSTATE FSSignatureImp::startVerify(FSPause* pause, void* clientData)
{
	if (!m_pSignature || !getPage() || !getPage()->getDocument() || !isSigned())
		return FSCommonDefines::e_progressError;

	FSCRT_LOCKOBJECT(lock, &m_lock);
	CPDF_Parser* pParser = ((FSPDFDocImp*)getPage()->getDocument())->getParser();
	if (!pParser) return FSCommonDefines::e_progressError;

	IFX_FileRead* pFileRead = pParser->GetFileAccess();
	if (m_pSigVerify)
		SAFE_DELETE(m_pSigVerify);
	m_verifyClientData.pClientData = clientData;
	m_verifyClientData.pFSSig = this;
	m_pSigVerify = FX_NEW CPDF_SignatureVerify(pFileRead, m_pSignature, &m_verifyClientData);
	if (!m_pSigVerify || !m_pSigVerify->Start()) 
		return FSCommonDefines::e_progressError;

	m_pVerifyPause = pause;
	return continueVerify();
}

FS_PROGRESSSTATE FSSignatureImp::continueVerify()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!m_pSignature || !getPage() || !getPage()->getDocument()
		|| !m_pSigVerify) 
		return FSCommonDefines::e_progressError;

	FX_ProgressiveStatus status = (FX_ProgressiveStatus)FXCORE_PROGRESSIVESTATUS_FAILED;
	if (m_pVerifyPause)
	{
		CFSCRT_Pause tempPause(m_pVerifyPause);
		status = m_pSigVerify->Continue(&tempPause);
	}
	else
		status = m_pSigVerify->Continue(NULL);

	return FSCRT_Util_TransformProgressStatusFromFxcore(status);
}

FS_DWORD FSSignatureImp::getState()
{
	if (!m_pSignature || !m_pSignature->GetSignatureDict()) return FSSignatureDefines::e_signatureStateUnknown;

	CPDF_Dictionary* pSigFieldDict = m_pSignature->GetSignatureDict();
	if (!pSigFieldDict->KeyExist("V") && !m_bCreateSigVDict) 
		return FSSignatureDefines::e_signatureStateNoSignData;

	return m_pSignature->GetVerifyState();
}

FS_BOOL	FSSignatureImp::clearSignedData()
{
	if (!m_pSignature || !getPage() || !getPage()->getDocument()) 
		return FALSE;
	
	// To clear signed data is to remove /V and AP of the signature, but keep signature field dictionary.
	FS_BOOL bRet = ((FSPDFDocImp*)getPage()->getDocument())->clearSignedData(m_pSignature);
	if (bRet)
	{
		// When clear successfully, we need to create the /V dictionary in order that this signature can be used again.
		m_pSignature->CreateSigVDict();
		m_bCreateSigVDict = TRUE;
	}
	return bRet;
}

FSPDFDoc*	FSSignatureImp::getDocument()
{
	if (!m_pSignature || !getPage()) return NULL;

	return getPage()->getDocument();
}

CFX_ByteString FSUtil_SignatureKeyName_TransformToString(FS_SIGNATUREKEYNAME keyName)
{
	switch(keyName)
	{
	case FSSignatureDefines::e_signatureKeyNameSigner:
		return "Name";
	case FSSignatureDefines::e_signatureKeyNameLocation:
		return "Location";
	case FSSignatureDefines::e_signatureKeyNameReason:
		return "Reason";
	case FSSignatureDefines::e_signatureKeyNameContactInfo:
		return "ContactInfo";
	case FSSignatureDefines::e_signatureKeyNameDN:
		return "DN";
	case FSSignatureDefines::e_signatureKeyNameText:
		return "Text";
	case FSSignatureDefines::e_signatureKeyNameFilter:
		return "Filter";
	case FSSignatureDefines::e_signatureKeyNameSubFilter:
		return "SubFilter";
	}
	return "";
}

FS_DWORD FSSignatureImp::getAppearanceFlags()
{
	if (!m_pSignature) return 0;
	FX_DWORD flag = m_pSignature->GetFlags("APF");
	//-1 means error from core.
	return flag == -1?0:flag;
}

void FSSignatureImp::setAppearanceFlags(FS_DWORD apFlags)
{
	if (!m_pSignature) return ;

	// Check if /V dictionary exists.
	CPDF_Dictionary* pSigFieldDict = m_pSignature->GetSignatureDict();
	if (!pSigFieldDict->KeyExist("V") && !m_bCreateSigVDict)
	{ 
		if (!m_pSignature->CreateSigVDict()) 
			return;
		else
			m_bCreateSigVDict = TRUE;
	}

	m_pSignature->SetFlags("APF", apFlags);
	//setModified(); // TODO
}

FSString FSSignatureImp::getKeyValue(FS_SIGNATUREKEYNAME key)
{
	if (!m_pSignature) return FSString();
	CFX_ByteString bsKey = FSUtil_SignatureKeyName_TransformToString(key);
	if (bsKey.IsEmpty()) return FSString();
	
	CFX_WideString wsValue;
	if (!m_pSignature->GetKeyValue(bsKey, wsValue))
		return FSString();
	CFX_ByteString bsUTF8Value = FSCRT_UTF8_FromWideString(wsValue);
	if (bsUTF8Value.IsEmpty()) return FSString();
	FSString fsRetString(bsUTF8Value.GetBuffer(bsUTF8Value.GetLength()));
	bsUTF8Value.ReleaseBuffer();
	return fsRetString;
}

void FSSignatureImp::setKeyValue(FS_SIGNATUREKEYNAME key, const char* value)
{
	if (!m_pSignature
		|| (FSSignatureDefines::e_signatureKeyNameFilter == key && FSCRT_Util_IsStringEmpty(value))
		)
		return;
	// Prepare key name string.
	CFX_ByteString bsKey = FSUtil_SignatureKeyName_TransformToString(key);
	if (bsKey.IsEmpty()) return ;

	// Check if /V dictionary exists.
	CPDF_Dictionary* pSigFieldDict = m_pSignature->GetSignatureDict();
	if (!pSigFieldDict->KeyExist("V") && !m_bCreateSigVDict)
	{ 
		if (!m_pSignature->CreateSigVDict()) 
			return;
		else
			m_bCreateSigVDict = TRUE;
	}

	// Transform UTF-8 value string to widestring.
	CFX_WideString wsValue;
	if (!FSCRT_Util_IsStringEmpty(value))
		wsValue = FSCRT_UTF8_ToWideString(value);
	m_pSignature->SetKeyValue(bsKey, wsValue);
	//setModified(); // TODO
}

FSDateTime FSSignatureImp::getSigningTime()
{
	FSDateTime retDate;
	retDate.set(0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (!m_pSignature) return retDate;
	
	FXCRT_DATETIMEZONE dt;
	if (m_pSignature->GetDataTime(dt))
	{
		retDate.set(dt.year, dt.month, dt.day, 
					dt.hour, dt.minute, dt.second, dt.milliseconds, 
					dt.tzHour, dt.tzMinute);
	}
	return retDate;
}

void FSSignatureImp::setSigningTime(FSDateTime signTime)
{
	if (!m_pSignature || !FSCRT_Util_CheckDateTime(signTime))
		return ;

	// Check if /V dictionary exists.
	CPDF_Dictionary* pSigFieldDict = m_pSignature->GetSignatureDict();
	if (!pSigFieldDict->KeyExist("V") && !m_bCreateSigVDict)
	{ 
		if (!m_pSignature->CreateSigVDict()) 
			return;
		else
			m_bCreateSigVDict = TRUE;
	}

	FXCRT_DATETIMEZONE dt;
	dt.year = signTime.year;
	dt.month = signTime.month;
	dt.day = signTime.day;
	dt.hour = signTime.hour;
	dt.minute = signTime.minute;
	dt.second = signTime.second;
	dt.milliseconds = signTime.milliseconds;
	dt.tzHour = signTime.UTHourOffset;
	dt.tzMinute = signTime.UTMinuteOffset;
	m_pSignature->SetDataTime(dt);
	//setModified();//todo: needed?
}

CPDF_Image* FSCRT_Util_GetBitmapFromAPForm(CPDF_PageObjects* pPageObjs)
{
	CPDF_Image* pImage = NULL;
	FX_POSITION pos = pPageObjs->GetFirstObjectPosition();
	while (pos)
	{
		CPDF_PageObject* pObj = pPageObjs->GetNextObject(pos);
		switch (pObj->m_Type)
		{
		case PDFPAGE_TEXT:
		case PDFPAGE_PATH:
		case PDFPAGE_SHADING:
		case PDFPAGE_INLINES:
			break;
		case PDFPAGE_IMAGE:
			{
				CPDF_ImageObject* pImageObj = (CPDF_ImageObject*)pObj;
				return pImageObj->m_pImage->Clone();
				//if (CPDF_Image* pImage = pImageObj->m_pImage)
				//{
				//	CFX_DIBSource* pDIBSource = pImage->LoadDIBSource();
				//	pBitmap = pDIBSource->Clone();
				//	delete pDIBSource;
				//	return pBitmap;
				//}
			}
		case PDFPAGE_FORM:
			{
				CPDF_FormObject *pFormObj = (CPDF_FormObject*)pObj;
				CPDF_Form *pForm = pFormObj->m_pForm;
				pImage = FSCRT_Util_GetBitmapFromAPForm(pForm);
				if (pImage) return pImage;
			}
		}
	}

	return pImage;
}

FSBitmap* FSSignatureImp::getBitmap()
{
	if (!m_pSignature || !getPage() || !getDict())
		return NULL;

	if (!m_pImage)
	{
		FS_DWORD dwSigFieldDictObjNum = getDict()->getObjNum();
		CPDF_Page* pPage = ((FSPDFPageImp*)getPage())->getPage();
		CPDF_Annot* pAnnot = NULL;
		CPDF_AnnotList pAnnotList(pPage);
		for (FS_INT32 i=0; i<pAnnotList.Count(); i++)
		{
			pAnnot = pAnnotList.GetAt(i);
			if (pAnnot->m_pAnnotDict->GetObjNum() == dwSigFieldDictObjNum)
				break;
		}
		if (!pAnnot) return NULL;

		CPDF_Form* pAPForm = pAnnot->GetAPForm(pPage, CPDF_Annot::Normal);
		if (!pAPForm) return NULL;

		m_pImage = FSCRT_Util_GetBitmapFromAPForm(pAPForm);
		if (!m_pImage) return NULL;
		m_bNewImage = FALSE;
	}
	CFX_DIBSource* pDIBSource = m_pImage->LoadDIBSource();
	if (!pDIBSource)
		return NULL;
	FSBitmap* pFSBitmap = FSCRT_Util_ConvertDIBSource2FSBitmap(pDIBSource);
	delete pDIBSource;
	return pFSBitmap;
}

void FSSignatureImp::setBitmap(FSBitmap* bitmap)
{
	if (!m_pSignature || !bitmap)
		return ;

	if (m_pImage)
	{
		if (m_bNewImage)
		{
			delete m_pImage;
			m_bNewImage = FALSE;
		}
		m_pImage = NULL;
	}
	CPDF_Document* pDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
	m_pImage = FX_NEW CPDF_Image(pDoc);
	if (!m_pImage) return ;
	m_pImage->SetImage(((FSBitmapImp*)bitmap)->getBitmap(), PDF_IMAGE_NO_COMPRESS);
	m_bNewImage = TRUE;
	// no need to setModified() because this is not set to PDF File content.
}

void	FSSignatureImp::setSignerCertPath(const char* certPath, const char password[], FS_INT32 passwordLen)
{
	if (!m_pSignature)
		return;

	CFX_WideString wsPath = FSCRT_UTF8_ToWideString(certPath);
	if (wsPath.IsEmpty()) return ;

	CFX_WideString wsPassword = FSCRT_UTF8_ToWideString(password, passwordLen);
	if (wsPassword.IsEmpty()) return ;
	m_pSignature->SetSignerCertPath(wsPath, wsPassword);
	// no need to setModified() because this is not set to PDF File content.
}

 void	FSSignatureImp::getSignerCertPath(FSString& certPath, FSString& certPassword)
{
	if (!m_pSignature)
		return;
	CFX_WideString wsPath , wsPassword;
	m_pSignature->GetSignerCertPath(wsPath, wsPassword);
	certPath = FSCRT_UTF8_FromWideString(wsPath);
	certPassword = FSCRT_UTF8_FromWideString(wsPassword);
}

FSPDFDictionary*	FSSignatureImp::getSignatureDict()
{
	if (!m_pSignature || !m_pSignature->GetSignatureDict())
		return NULL;

	return (FSPDFDictionary*)m_pSignature->GetSignatureDict()->GetDict("V");
}

void FSSignatureImp::setAppearanceContent(const char* appearanceContent)
{
	if (!m_pSignature || isSigned())
		return ;

	m_bsCustomApContent.Empty();
	CFX_ByteString utf8(appearanceContent);
	m_bsCustomApContent = FSCRT_UTF8_ToTextString(utf8);
}

//Internal use

CPDF_Rect FSCRT_Util_GetCenterSquare(CPDF_Rect rect)
{
	FX_FLOAT fWidth = rect.right -  rect.left;
	FX_FLOAT fHeight = rect.top - rect.bottom;

	FX_FLOAT fCenterX = (rect.left + rect.right)/2.0f;
	FX_FLOAT fCenterY = (rect.top + rect.bottom)/2.0f;

	FX_FLOAT fRadius = (fWidth > fHeight) ? fHeight / 2 : fWidth / 2;

	return CPDF_Rect(fCenterX - fRadius,fCenterY - fRadius,fCenterX + fRadius,fCenterY + fRadius);
}

CPDF_Form*		FSSignatureImp::generateSignAP()
{
	if (!m_pSignature || !m_pSignature->GetSignatureDict() || !getPage())
		return NULL;

	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
	CPDF_Dictionary* pSigFieldDict = m_pSignature->GetSignatureDict();
	CPDF_Dictionary* pPageDict = (CPDF_Dictionary*)getPage()->getDict();

	// Get ap flags
	FS_DWORD apFlags = getAppearanceFlags(); //TODO If ap Flags is 0, then ??
	// Get description
	CFX_WideString wsDescription;
	if (!getDescription(wsDescription))
		return NULL;
	//Get image source
	CPDF_Stream* pImageStream = NULL;
	if (apFlags & FSSignatureDefines::e_signatureAPFlagBitmap)
	{
		loadImageStream(pImageStream);
	}

	// Prepare AP stream
	CPDF_Dictionary* pAPStreamDict = FX_NEW CPDF_Dictionary();
	if (!pAPStreamDict) return NULL;
	CPDF_Stream* pAPStream = FX_NEW CPDF_Stream(NULL, 0, pAPStreamDict);
	if (!pAPStream)
	{
		pAPStreamDict->Release();
		return NULL;
	}
	pAPStreamDict->SetAtName(FX_BSTRC("Type"), "XObject");
	pAPStreamDict->SetAtName(FX_BSTRC("Subtype"), "Form");

	//
	CPDF_Rect rcBBox = getRotatedRect();
	CPDF_Matrix matrix = getRotatedMatrix();
	pAPStreamDict->SetAtMatrix("Matrix", matrix);	
	pAPStreamDict->SetAtRect("BBox", rcBBox);
	CPDF_Dictionary* pAPStreamResDict = pAPStreamDict->GetDict("Resources");
	if (!pAPStreamResDict)
	{
		pAPStreamResDict = FX_NEW CPDF_Dictionary;
		if (!pAPStreamResDict)
		{
			pAPStream->Release();
			return NULL;
		}
		pAPStreamDict->SetAt("Resources",pAPStreamResDict);
	}

	CPDF_Dictionary * pExtGState = pAPStreamResDict->GetDict("ExtGState");
	if (!pExtGState)
	{
		pExtGState = FX_NEW CPDF_Dictionary;
		if (!pAPStreamResDict)
		{
			pAPStream->Release();
			return NULL;
		}
		pAPStreamResDict->SetAt("ExtGState", pExtGState);
	}	

	CFX_ByteString sAppStream;
	if (m_bsCustomApContent.IsEmpty() && 0 != apFlags)
	{
		if (apFlags & FSSignatureDefines::e_signatureAPFlagFoxitFlag)
		{
			CPDF_Dictionary* pBKOpacity = pAPStreamResDict->GetDict("BackgroundOpacity");
			if (!pBKOpacity)
			{
				pBKOpacity = FX_NEW CPDF_Dictionary;
				if (!pAPStreamResDict)
				{
					pAPStream->Release();
					return NULL;
				}
				FX_DWORD dwObject = pPDFDoc->AddIndirectObject(pBKOpacity);
				pExtGState->SetAtReference("BackgroundOpacity", pPDFDoc, dwObject);
			}
			pBKOpacity->SetAtName("Type", "ExtGState");
			pBKOpacity->SetAtNumber("CA", 0.6f);
			pBKOpacity->SetAtNumber("ca", 0.6f);
			pBKOpacity->SetAt("AIS", FX_NEW CPDF_Boolean(false));
			pBKOpacity->SetAtName("BM" , "Multiply ");
		}

		CFSPDF_WidgetProperties properties;
		properties.m_rtWidget = rcBBox;
		properties.m_pSystemHandler = FX_NEW FX_SystemHandlerImp(NULL);

		CPDF_FormControl* pPDFFormControl = GetCPDFFormControl();
		CPDF_DefaultAppearance da = pPDFFormControl->GetDefaultAppearance();
		FX_ARGB color;
		int iColorType = 2;
		da.GetColor(color, iColorType);
		properties.m_sTextColor = color;

		IFSPDF_Signature* pSigWnd = IFSPDF_Signature::Create();
		if (!pSigWnd) {
			pAPStream->Release();
			return NULL;
		}

		pSigWnd->Initialize((FSSignature*)this, &properties);
		pSigWnd->Update();

		CFX_WideString wsText;
		if ((apFlags & FSSignatureDefines::e_signatureAPFlagText) 
			&& m_pSignature->GetKeyValue("Text", wsText) && !wsText.IsEmpty())
		{
			pSigWnd->SetShowText(TRUE);
			pSigWnd->SetText(wsText);
		}
		pSigWnd->SetDescription(wsDescription);

		CFX_ByteString sAliasImage = "IMG";
		if (pImageStream)
		{	
			if (CPDF_Dictionary* pImageDict = pImageStream->GetDict())
			{
				sAliasImage = pImageDict->GetString("Name");
				if (sAliasImage.IsEmpty())
				{
					sAliasImage = "IMG";
				}			
				pSigWnd->SetShowImage(TRUE);
				pSigWnd->SetImageStream(pImageStream, sAliasImage);
			}

			CPDF_Dictionary* pXObject = FX_NEW CPDF_Dictionary;
			if (!pXObject)
			{
				pAPStream->Release();
				return NULL;
			}
			pXObject->SetAtReference(sAliasImage, pPDFDoc, pImageStream);
			pAPStreamResDict->SetAt("XObject", pXObject);
		}

		CFX_ByteString sAPContent = pSigWnd->GetAppearanceStream();
		if (apFlags & FSSignatureDefines::e_signatureAPFlagFoxitFlag)
		{
			sAppStream += "q\n";
			sAppStream += "/BackgroundOpacity gs\n";
			sAppStream += CFSPDF_AnnotStdIconProvider::GetIconAppStream(FSPDF_ANNOTTYPE_SIGNATURE, FSPDF_ANNOT_ICONNAME_SIGNATURE_FOXIEFILAG, FSCRT_Util_GetCenterSquare(rcBBox),
																		CFSPDFAnnot_Color(CT_RGB,0.91f,0.855f,0.92f), CFSPDFAnnot_Color(CT_TRANSPARENT));
			sAppStream += "Q\n";
		}

		sAppStream += "q\n";
		sAppStream += sAPContent;
		sAppStream += "\nQ\n";
		pSigWnd->Release();
		properties.m_pSystemHandler->Release();
		properties.m_pSystemHandler = NULL;
	}
	else if (!m_bsCustomApContent.IsEmpty())
	{
		sAppStream += "q\n";
		sAppStream += m_bsCustomApContent;
		sAppStream += "\nQ\n";
		m_bsCustomApContent.Empty();
	}
	pAPStream->SetData((FX_BYTE*)(FX_LPCSTR)sAppStream, sAppStream.GetLength(), FALSE, FALSE);

	//set font dictionary to new AP dictionary.
	CPDF_Dictionary* pOldApDict = pSigFieldDict->GetDict("AP");
	if (pOldApDict)
	{
		CPDF_Dictionary *pOldAPStreamDict = NULL, *pOldResourceDict = NULL, *pOldFontDict = NULL;
		CPDF_Stream* pOldStream = NULL;
		if (pOldStream = pOldApDict->GetStream("N"))
		{
			if (pOldAPStreamDict = pOldStream->GetDict())
			{
				if (pOldResourceDict = pOldAPStreamDict->GetDict("Resources"))
				{
					if (pOldFontDict = pOldResourceDict->GetDict("Font"))
					{
						pAPStreamResDict->SetAt("Font", pOldFontDict->Clone());
					}
				}
			}
		}
	}

	if (m_pAPForm)
		SAFE_DELETE(m_pAPForm);
	// Generate CPDF_Form
	m_pAPForm = FX_NEW CPDF_Form(pPDFDoc, pAPStreamResDict, pAPStream);
	if (!m_pAPForm)
		pAPStream->Release();

	return m_pAPForm;
}

FS_BOOL FSSignatureImp::getDescription(CFX_WideString& wcDes)
{
	FS_DWORD dwAPOpt = getAppearanceFlags();

	FS_BOOL bLabel = dwAPOpt & FSSignatureDefines::e_signatureAPFlagLabel;
	if (dwAPOpt & FSSignatureDefines::e_signatureAPFlagSigner)
	{
		if (bLabel)
			wcDes += L"Digitally signed by ";
		if (!getSubDescription("Name", wcDes))
		{
			wcDes.Empty();
			return FALSE;
		}
	}
	if (dwAPOpt & FSSignatureDefines::e_signatureAPFlagDN)
	{
		if (bLabel)
			wcDes += L"DN: ";
		if (!getSubDescription("DN", wcDes))
		{
			wcDes.Empty();
			return FALSE;
		}
	}
	if (dwAPOpt & FSSignatureDefines::e_signatureAPFlagReason)
	{
		if (bLabel)
			wcDes += L"Reason: ";
		if (!getSubDescription("Reason", wcDes))
		{
			wcDes.Empty();
			return FALSE;
		}	
	}
	if (dwAPOpt & FSSignatureDefines::e_signatureAPFlagLocation)
	{
		if (bLabel)
			wcDes += L"Location: ";
		if (!getSubDescription("Location", wcDes))
		{
			wcDes.Empty();
			return FALSE;
		}	
	}

	if (dwAPOpt & FSSignatureDefines::e_signatureAPFlagSigningTime)
	{
		if (bLabel)
			wcDes += L"Date: ";
		if (!getSubDescription("M", wcDes))
		{
			wcDes.Empty();
			return FALSE;
		}	
	}

	return TRUE;
}

FS_BOOL FSSignatureImp::getSubDescription(FX_BSTR key, CFX_WideString& wcDes)
{
	if (!m_pSignature)
		return FALSE;
	CFX_WideString wcValue;
	m_pSignature->GetKeyValue(key, wcValue);
	wcDes += wcValue + L"\r\n";
	return TRUE;
}

FS_BOOL FSSignatureImp::loadImageStream(CPDF_Stream*& pImageStream)
{
	pImageStream = NULL;
	if (!m_pSignature || !getPage() || !getDocument()
		|| !m_pImage)
		return FALSE;

	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)getDocument())->getPDFDocument();
	return addImageResource(pPDFDoc, pImageStream);
}

CPDF_Rect FSSignatureImp::getRotatedRect()
{
	CPDF_Rect rcRect;
	FXSYS_memset(&rcRect, 0, sizeof(CPDF_Rect));
	if (!m_pSignature || !m_pSignature->GetSignatureDict() || !GetCPDFFormControl()) 
		return rcRect;

	CPDF_Rect rcAnnot = m_pSignature->GetSignatureDict()->GetRect("Rect");
	FX_FLOAT fWidth = rcAnnot.right - rcAnnot.left;
	FX_FLOAT fHeight = rcAnnot.top - rcAnnot.bottom;
	FX_INT32 nRotate = GetCPDFFormControl()->GetRotation();

	switch(FXSYS_abs(nRotate % 360))
	{
	case 0:
	case 180:
	default:
		rcRect = CPDF_Rect(0, 0, fWidth, fHeight);	
		break;
	case 90:
	case 270:
		rcRect = CPDF_Rect(0, 0, fHeight, fWidth);
		break;
	}

	return rcRect;
}

CPDF_Matrix FSSignatureImp::getRotatedMatrix()
{
	CPDF_Matrix matrix;
	if (!m_pSignature || !m_pSignature->GetSignatureDict() || !GetCPDFFormControl()) 
		return matrix;

	CPDF_Rect rcAnnot = m_pSignature->GetSignatureDict()->GetRect("Rect");
	FX_FLOAT fWidth = rcAnnot.right - rcAnnot.left;
	FX_FLOAT fHeight = rcAnnot.top - rcAnnot.bottom;
	FX_FLOAT fLeft = rcAnnot.left;
	FX_FLOAT fBottom = rcAnnot.bottom;
	FX_INT32 nRotate = GetCPDFFormControl()->GetRotation();

	switch (FXSYS_abs(nRotate % 360))
	{
	case 0:
	default:
		matrix = CPDF_Matrix(1, 0, 0, 1, 0, 0);
		break;
	case 90:
		matrix = CPDF_Matrix(0, 1, -1, 0, fWidth, 0);
		break;
	case 180:
		matrix = CPDF_Matrix(-1, 0, 0, -1, fWidth, fHeight);
		break;
	case 270:
		matrix = CPDF_Matrix(0, -1, 1, 0, 0, fHeight);
		break;
	}

	return matrix;
}

FS_BOOL FSSignatureImp::addImageResource(CPDF_Document* pPDFDoc, CPDF_Stream*& pImageStream)
{
	if (!pPDFDoc || !m_pImage) return NULL;
	int nWidth = m_pImage->GetPixelWidth();
	int nHeight = m_pImage->GetPixelHeight();

	CPDF_Stream* pStream = NULL;
	pStream = m_pImage->GetStream();
	if (!pStream) return FALSE;

	if (0 == pStream->GetObjNum())
		pPDFDoc->AddIndirectObject(pStream);

	CPDF_Dictionary* pStreamDict = FX_NEW CPDF_Dictionary();//TODO
	pStreamDict->SetAtName("Subtype", "Form");
	pStreamDict->SetAtName("Name", "IMG");
	CPDF_Array* pMatrix = FX_NEW CPDF_Array();//TODO
	pStreamDict->SetAt("Matrix", pMatrix);
	pMatrix->AddInteger(1);
	pMatrix->AddInteger(0);
	pMatrix->AddInteger(0);
	pMatrix->AddInteger(1);
	pMatrix->AddInteger(-nWidth / 2);
	pMatrix->AddInteger(-nHeight / 2);
	CPDF_Dictionary* pResource = FX_NEW CPDF_Dictionary();//TODO
	CPDF_Dictionary* pExtGStateDic = FX_NEW CPDF_Dictionary;//TODO
	pResource->SetAt("ExtGState" , pExtGStateDic);

	CPDF_Dictionary* pBldModeGsDic = FX_NEW CPDF_Dictionary;
	pBldModeGsDic->SetAtName("BM" , "Multiply ");
	pBldModeGsDic->SetAtName("Type"  , "ExtGState");
	pExtGStateDic->SetAt("FXSigBldModeGs" , pBldModeGsDic);

	pStreamDict->SetAt("Resources", pResource);
	CPDF_Dictionary* pXObject = FX_NEW CPDF_Dictionary(); //TODO
	pResource->SetAt("XObject", pXObject);
	pXObject->SetAtReference("Img", pPDFDoc, pStream);
	CPDF_Array* pProcSet = FX_NEW CPDF_Array();//TODO
	pResource->SetAt("ProcSet", pProcSet);
	pProcSet->AddName("PDF");
	pProcSet->AddName("ImageC");
	pStreamDict->SetAtName("Type", "XObject");
	CPDF_Array* pBBox = FX_NEW CPDF_Array();//TODO
	pStreamDict->SetAt("BBox", pBBox);
	pBBox->AddInteger(0);
	pBBox->AddInteger(0);
	pBBox->AddInteger(nWidth);
	pBBox->AddInteger(nHeight);
	pStreamDict->SetAtInteger("FormType", 1);

	pImageStream = FX_NEW CPDF_Stream(NULL, 0, NULL);//TODO
	CFX_ByteString csStream;
	csStream.Format("q\n%d 0 0 %d 0 0 cm\n/Img Do\nQ", nWidth, nHeight);
	csStream = "/FXSigBldModeGs gs " + csStream;
	//
	pImageStream->InitStream((FX_BYTE*)(FX_LPCSTR)csStream, csStream.GetLength(), pStreamDict);
	pPDFDoc->AddIndirectObject(pImageStream);

	return TRUE;
}
#endif