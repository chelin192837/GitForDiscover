#include "../include/fs_render.h"
#include "../include/fs_pdfdoc.h"
#include "../include/fs_pdfpage.h"
#include "../include/fs_pdfobject.h"
#include "../include/fs_bookmark.h"

#include <string>;
using namespace std;

#include "fxcore.h"
#include "fs_file.h"
#include "fs_util.h"
#include "fs_pause.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_bookmarkImp.h"
#include "inc/commonImp.h"
#include "form/fs_pdfformImp.h"
#include "inc/fs_readingbookmarkImp.h"
#include "inc/fs_signatureImp.h"

#include "ofd/fr_include.h"
#include "ofd/fr_ofddocImp.h"


///////////////////////////////////
// Structure FSPageLabel
void FSPageLabel::set(FS_INT32 start, FS_PAGELABELSTYLE style, const char prefix[100], FS_INT32 firstPageNumber)
{
	this->start = start;
	this->style = style;
	
	if (prefix)
		FXSYS_memcpy(this->prefix, prefix, 100);
	else
		FXSYS_memset(this->prefix, 0, sizeof(const char)*100);

	this->firstPageNumber = firstPageNumber;
}

//////////////////////////////////
// Class FSPDFDoc
FSPDFDoc*	FSPDFDoc::create()
{
	return FSPDFDocImp::create();
}

FSPDFDoc*	FSPDFDoc::createFromFilePath(const char* path)
{
//    // PDF to OFD
//	string strPdf("/storage/emulated/0/PDF/empty.pdf");			// in
//	string strOfd("/storage/emulated/0/PDF/save/empty.ofd");		// out
//	PdfToOfdByName(strPdf , strOfd);
//    string strPdf1("/storage/emulated/0/PDF/seal.pdf");			// in
//    string strOfd1("/storage/emulated/0/PDF/save/seal.ofd");		// out
//    PdfToOfdByName(strPdf1 , strOfd1);
//    string strPdf2("/storage/emulated/0/PDF/二维码.pdf");			// in
//    string strOfd2("/storage/emulated/0/PDF/save/二维码.ofd");		// out
//    PdfToOfdByName(strPdf2 , strOfd2);
//    string strPdf3("/storage/emulated/0/PDF/回单样例-20200716.pdf");			// in
//    string strOfd3("/storage/emulated/0/PDF/save/回单样例-20200716.ofd");		// out
//    PdfToOfdByName(strPdf3 , strOfd3);
//	std::string strPdf4("/storage/emulated/0/PDF/sk-export-文本+图像-200页.pdf");			// in
//	std::string strOfd4("/storage/emulated/0/PDF/save/sk-export-文本+图像-200页.ofd");		// out
//	PdfToOfdByName(strPdf4 , strOfd4);
//	std::string strPdf5("/storage/emulated/0/PDF/速度测试画像.pdf");			// in
//	std::string strOfd5("/storage/emulated/0/PDF/save/速度测试画像.ofd");		// out
//	PdfToOfdByName(strPdf5 , strOfd5);
//
//    // OFD to PDF
//    string strOfd20("/storage/emulated/0/OFD/ofd_to_pdf_crash_test.ofd");		// in
//    string strPdf20("/storage/emulated/0/OFD/save/ofd_to_pdf_crash_test.pdf");	// out
//    OfdToPdfByName(strOfd20 , strPdf20);
//    string strOfd21("/storage/emulated/0/OFD/ofd_to_pdf_crash_test2.ofd");		// in
//    string strPdf21("/storage/emulated/0/OFD/save/ofd_to_pdf_crash_test2.pdf");	// out
//    OfdToPdfByName(strOfd21 , strPdf21);
//    string strOfd22("/storage/emulated/0/OFD/ofd_to_pdf_crash_test3.ofd");		// in
//    string strPdf22("/storage/emulated/0/OFD/save/ofd_to_pdf_crash_test3.pdf");	// out
//    OfdToPdfByName(strOfd22 , strPdf22);
//    string strOfd23("/storage/emulated/0/OFD/ofd_to_pdf_crash_test4.ofd");		// in
//    string strPdf23("/storage/emulated/0/OFD/save/ofd_to_pdf_crash_test4.pdf");	// out
//    OfdToPdfByName(strOfd23 , strPdf23);
//    string strOfd24("/storage/emulated/0/OFD/ofd_to_pdf_crash_test5.ofd");		// in
//    string strPdf24("/storage/emulated/0/OFD/save/ofd_to_pdf_crash_test5.pdf");	// out
//    OfdToPdfByName(strOfd24 , strPdf24);
//	string strOfd26("/storage/emulated/0/OFD/ofd_to_pdf_crash_test6.ofd");		// in
//	string strPdf26("/storage/emulated/0/OFD/save/ofd_to_pdf_crash_test6.pdf");	// out
//	OfdToPdfByName(strOfd26 , strPdf26);
//    string strOfd27("/storage/emulated/0/OFD/单个多页OFD文件.ofd");		// in
//    string strPdf27("/storage/emulated/0/OFD/save/单个多页OFD文件.pdf");	// out
//    OfdToPdfByName(strOfd27 , strPdf27);
//    string strOfd28("/storage/emulated/0/OFD/方正章.ofd");		// in
//    string strPdf28("/storage/emulated/0/OFD/save/方正章.pdf");	// out
//    OfdToPdfByName(strOfd28 , strPdf28);
//    string strOfd29("/storage/emulated/0/OFD/未命名-铅笔.ofd");		// in
//    string strPdf29("/storage/emulated/0/OFD/save/未命名-铅笔.pdf");	// out
//    OfdToPdfByName(strOfd29 , strPdf29);

	if (NULL != path) {
		CFX_ByteString fullpath(path);
		int index = fullpath.ReverseFind('.');
		if (index >=0) {
			index++;
			CFX_ByteString ext = fullpath.Mid(index, fullpath.GetLength()-index);
			if (ext.EqualNoCase("ofd")) {
				return FR_OFDDocImp::createFromFilePath(path);
			}else if(ext.EqualNoCase("pdf")){
				return FSPDFDocImp::createFromFilePath(path);
			}else{				
				return NULL;
			}
		}
	}
	return NULL;
}


FSPDFDoc*	FSPDFDoc::createFromMemory(const void* buffer, FS_INT32 bufLen, FS_BOOL bReleaseBuffer, FS_BOOL bOfd)
{
    if (bOfd)
        return FR_OFDDocImp::createFromMemory(buffer, bufLen, bReleaseBuffer);
    else
	    return FSPDFDocImp::createFromMemory(buffer, bufLen, bReleaseBuffer);
}



FSPDFDoc*	FSPDFDoc::createFromHandler(FSFileRead* pFileRead)
{
	return FSPDFDocImp::createFromHandler(pFileRead);
}

//////////////////////////////////
// Class FSPDFDocImp
#define FSPDF_LOADDOCTYPE_NEW			0
#define FSPDF_LOADDOCTYPE_FROMPATH		1
#define FSPDF_LOADDOCTYPE_FROMBUFFER	2
#define FSPDF_LOADDOCTYPE_FROMFILEREAD	3

FSPDFDocImp::FSPDFDocImp(FS_INT32 loadType)
	:m_pDoc(NULL)
	,m_pParser(NULL)
	,m_pFileStream(NULL)
	,m_uLoadDocType(loadType)
	,m_pMapPage(NULL)
	,m_pInitParam(NULL)
	,m_bufflen(0)
	,m_isModified(FALSE)
	,m_pBmRoot(NULL)
	,m_bReleaseBuffer(FALSE)
	//,m_pMapFSFont2FontDict(NULL)
	,m_pMapFontDict2FSFont(NULL)
	,m_pForm(NULL)
	,m_pJSRuntime(NULL)
	,m_pMetadata(NULL)
	,m_pXMLReadingBookmark(NULL)
	,m_pReadingBookmarksArray(NULL)
	,m_pSigEdit(NULL)
	,m_pSTActionHandler(NULL)
{}

FSPDFDocImp* FSPDFDocImp::creatImp(FS_INT32 loadType, void* pInitParam /*= NULL*/, FS_INT32 bufLen /*= 0*/, FS_BOOL bReleaseBuffer)
{
	FSPDFDocImp* pDoc = FX_NEW FSPDFDocImp(loadType);
	if (NULL == pDoc) return NULL;

	switch(loadType)
	{
	case FSPDF_LOADDOCTYPE_NEW:
		{
			pDoc->init();
		}
		break;
	case FSPDF_LOADDOCTYPE_FROMPATH:
		{
			//pInitParam means path
			if (NULL == pInitParam || FXSYS_strlen((const char*)pInitParam)<5)
			{
				delete pDoc;
				return NULL;
			}
			int len = FXSYS_strlen((const char*)pInitParam);
			CFX_ByteString bsTemp((const char*)pInitParam, len);
			if (FALSE == bsTemp.Right(4).EqualNoCase(".pdf"))
			{
				delete pDoc;
				return NULL;
			}
			pDoc->m_pInitParam = FX_Alloc(char, len + 1);
			if(pDoc->m_pInitParam)
			{
				FXSYS_memset(pDoc->m_pInitParam, 0, len + 1);
				FXSYS_memcpy(pDoc->m_pInitParam, (const char*)pInitParam, len);
			}
			else
			{
				delete pDoc;
				return NULL;
			}
		}
		break;
	case FSPDF_LOADDOCTYPE_FROMFILEREAD:
		{
			if (NULL == pInitParam)
			{
				delete pDoc;
				return NULL;
			}
			pDoc->m_pInitParam = pInitParam;
			pDoc->m_bufflen = 0;
		}
		break;
	case FSPDF_LOADDOCTYPE_FROMBUFFER:
		{
			if (NULL == pInitParam || bufLen<1)
			{
				delete pDoc;
				return NULL;
			}
			pDoc->m_pInitParam = pInitParam;
			pDoc->m_bufflen = bufLen;
		}
		break;
	}
	return pDoc;
}


FSPDFDocImp* FSPDFDocImp::create()
{
	return creatImp(FSPDF_LOADDOCTYPE_NEW);
}

FSPDFDocImp* FSPDFDocImp::createFromFilePath(const char* path)
{
	return creatImp(FSPDF_LOADDOCTYPE_FROMPATH, (void*)path);
}

FSPDFDocImp* FSPDFDocImp::createFromMemory(const void* pBuffer, FS_INT32 bufflen, FS_BOOL bReleaseBuffer)
{
	return creatImp(FSPDF_LOADDOCTYPE_FROMBUFFER, (void*)pBuffer, bufflen, bReleaseBuffer);
}

FSPDFDocImp* FSPDFDocImp::createFromHandler(FSFileRead* pFileRead)
{
	return creatImp(FSPDF_LOADDOCTYPE_FROMFILEREAD, (void*)pFileRead);
}

FSPDFDocImp::~FSPDFDocImp()
{
	SAFE_DELETE(m_pBmRoot);
	SAFE_DELETE(m_pMetadata);
	m_pXMLReadingBookmark = NULL;

	if(m_pSTActionHandler)
	{
		m_pSTActionHandler->Destroy();
		m_pSTActionHandler = NULL;
	}

#if defined(ENABLE_JSENGINE)
    FSCRT_GetJSRuntimeFactory()->DeleteJSRuntime();
#endif
    
	if (m_pSigEdit)
		SAFE_DELETE(m_pSigEdit);

	{
		FSCRT_LOCKOBJECT(lock, &m_lockPages);
		if (m_pMapPage) 
		{
			FX_POSITION pos = m_pMapPage->GetStartPosition();
			while (pos)
			{
				void* pKey = NULL;
				FSPDFPage* pPage = NULL;
				m_pMapPage->GetNextAssoc(pos, pKey, (void*&)pPage);
				if (pPage) {
					((FSPDFPageImp*)pPage)->releaseSource();
				}
			}
			pos = m_pMapPage->GetStartPosition();
			while (pos)
			{
				void* pKey = NULL;
				FSPDFPage* pPage = NULL;
				m_pMapPage->GetNextAssoc(pos, pKey, (void*&)pPage);
				if (pPage) {
					((FSPDFPageImp*)pPage)->destroy();
					pPage = NULL;
				}
			}
			m_pMapPage->RemoveAll();

			delete m_pMapPage;
			m_pMapPage = NULL;
		}
	}

	{
		FSCRT_LOCKOBJECT(lock, &m_lockFontMaps);
		if (m_pMapFontDict2FSFont)
		{
			FX_POSITION pos = m_pMapFontDict2FSFont->GetStartPosition();
			while (pos) {

				void* pKey = NULL;
				void* pValue = NULL;

				m_pMapFontDict2FSFont->GetNextAssoc(pos, pKey, pValue);
				if (pValue)
				{
					FSFontImp* pFont = ((FSFontImp*)pValue);
					pFont->removeMapRecord(this);
					// When the map in FSFontImp is empty, it usually means the font is useless.
					// So we need to release it.
					if (pFont->isMapEmpty())
						pFont->release();
				}
			}
			m_pMapFontDict2FSFont->RemoveAll();
			delete m_pMapFontDict2FSFont;
			m_pMapFontDict2FSFont = NULL;
			//m_pMapFSFont2FontDict->RemoveAll();
			//delete m_pMapFSFont2FontDict;
			//m_pMapFSFont2FontDict = NULL;
		}
	}

	{
		FSCRT_LOCKOBJECT(lock, &m_lockReadingBookmarks);
		if (m_pReadingBookmarksArray)
		{
			for (FX_INT32 i = 0; i < m_pReadingBookmarksArray->GetSize(); i++)
			{
				FSReadingBookmark* pReadingBookmark = (FSReadingBookmark*)m_pReadingBookmarksArray->GetAt(i);
				SAFE_DELETE(pReadingBookmark);
			}
			m_pReadingBookmarksArray->RemoveAll();
			delete m_pReadingBookmarksArray;
			m_pReadingBookmarksArray = NULL;
		}
	}

	if (FSPDF_LOADDOCTYPE_NEW == m_uLoadDocType)
	{
		if (m_pDoc)
		{
			delete m_pDoc;
			m_pDoc = NULL;
		}
	}
	else
	{
		if (m_pParser)
		{
			delete m_pParser;
			m_pParser = NULL;
		}
		if (FSPDF_LOADDOCTYPE_FROMPATH == m_uLoadDocType && m_pInitParam)
		{
			FX_Free(m_pInitParam);
			m_pInitParam = NULL;
		}else if(FSPDF_LOADDOCTYPE_FROMBUFFER == m_uLoadDocType && m_bReleaseBuffer)
			SAFE_DELETE(m_pInitParam);
	}

	if (m_pFileStream)
	{
		((IFX_FileStream*)m_pFileStream)->Release();
		m_pFileStream = NULL;
	}

	if(m_pForm)
	{
		delete m_pForm;
		m_pForm = NULL;
	}
}

void FSPDFDocImp::release()
{
	delete this;
}

FS_BOOL FSPDFDocImp::init()
{
	//m_pMapFSFont2FontDict = FX_NEW CFX_MapPtrToPtr();
	//if (!m_pMapFSFont2FontDict) return FALSE;
	m_pMapFontDict2FSFont = FX_NEW CFX_MapPtrToPtr();
	if (!m_pMapFontDict2FSFont) 
	{
		//delete m_pMapFSFont2FontDict;
		//m_pMapFSFont2FontDict = NULL;
		return FALSE;
	}

	switch(m_uLoadDocType)
	{
	case FSPDF_LOADDOCTYPE_NEW:
		m_pDoc = FX_NEW CPDF_Document();
		if (!m_pDoc) return FALSE;

		m_pDoc->CreateNewDoc();
		break;
	case FSPDF_LOADDOCTYPE_FROMPATH:
		{
			m_pFileStream = FSCRT_Util_CreateFileFromPath((const char*)m_pInitParam, FX_FILEMODE_ReadOnly);	
		}
		break;
	case FSPDF_LOADDOCTYPE_FROMBUFFER:
		if (NULL == m_pInitParam || m_bufflen == 0) 
			return FALSE;

		m_pFileStream = FX_NEW CFSCRT_MemoryFile((FX_LPVOID)m_pInitParam, m_bufflen);
		break;
	case FSPDF_LOADDOCTYPE_FROMFILEREAD:
		if (NULL == m_pInitParam) 
			return FALSE;

		m_pFileStream = FX_NEW CFSCRT_FileStream((FSFileRead*)m_pInitParam);
		break;
	default:
		break;
	}

	if (!m_pFileStream && FSPDF_LOADDOCTYPE_NEW != m_uLoadDocType)
		return FALSE;
	return TRUE;
}

FS_PASSWORDTYPE FSPDFDocImp::checkPassword(const char password[], FS_INT32 passwordLen)
{
	if (FSPDF_LOADDOCTYPE_NEW == m_uLoadDocType) return e_pwdNoPassword;
	if (!m_pParser) return e_pwdInvalid;

	CPDF_Dictionary* pEncryDict = m_pParser->GetEncryptDict();
	if (!pEncryDict) 
		return e_pwdNoPassword;

	CFX_ByteString bsFilter = pEncryDict->GetString(FX_BSTRC("Filter"));
	if (bsFilter.IsEmpty())
		return e_pwdNoPassword;

	if (bsFilter != FX_BSTRC("Standard")) return e_pwdInvalid;

	CPDF_StandardSecurityHandler* pStdHandler = (CPDF_StandardSecurityHandler*)m_pParser->GetSecurityHandler();
	if (!pStdHandler) return e_pwdInvalid;

	if (pStdHandler->CheckPassword((FX_LPCBYTE)password, passwordLen, FALSE, NULL))
		return e_pwdUser;		
	else if (pStdHandler->CheckPassword((FX_LPCBYTE)password, passwordLen, TRUE, NULL))
		return e_pwdOwner;

	return e_pwdInvalid;
}

FS_ERRORCODE FSPDFDocImp::loadImp(const char password[], FS_INT32 passwordLen, FS_BOOL bReload)
{
	if (m_uLoadDocType == FSPDF_LOADDOCTYPE_NEW)
	{
		if (m_pDoc)
			return FSCommonDefines::e_errSuccess;
		
		return FSCommonDefines::e_errUnknown;
	}

	if (FALSE == bReload)
	{
		FS_BOOL bRet = init();
		if (!bRet) return FSCommonDefines::e_errFile;
	}
	else
	{
		if (isWrapper()) 
			bReload = FALSE;
	}

	if (NULL == m_pParser)
		m_pParser = FX_NEW CPDF_Parser();
	if (!m_pParser) return FSCommonDefines::e_errUnknown;
	m_pParser->SetFileStreamOption(TRUE);
	if (password && passwordLen > 0)
	{
		// If yes, code should be adjusted.
		CFX_ByteString bsPW(password, passwordLen);
		m_pParser->SetPassword(bsPW);
	}
	FX_DWORD dwRet = m_pParser->StartParse(m_pFileStream, bReload, FALSE);
	m_pDoc = m_pParser->GetDocument();
	if (PDFPARSE_ERROR_SUCCESS != dwRet)
	{
		switch (dwRet) 
		{
		case PDFPARSE_ERROR_PASSWORD: 
			return FSCommonDefines::e_errPassword;
		case PDFPARSE_ERROR_FORMAT:
			return FSCommonDefines::e_errFormat;
		case PDFPARSE_ERROR_HANDLER:
			return FSCommonDefines::e_errSecurityHandler;
		case PDFPARSE_ERROR_CERT:
			return FSCommonDefines::e_errCertificate;
		default:
			return FSCommonDefines::e_errUnknown;
		}
	}
	//TODO: about dynamic watermark.

	CPDF_DocJSActions docJS(m_pDoc);
	int iCount = docJS.CountJSActions();
	CFSCRT_STPDFActionHandler* pActionHandler = GetActionHandler();
	if(pActionHandler)
	{
		for (int i = 0; i < iCount; i ++)
		{
			CFX_ByteString csJSName;
			CPDF_Action jsAction = docJS.GetJSAction(i, csJSName);
			pActionHandler->DoAction_JavaScript(jsAction,CFX_WideString::FromLocal(csJSName),this);
		}
	}

	return FSCommonDefines::e_errSuccess;
}

FS_ERRORCODE FSPDFDocImp::load(const char password[], FS_INT32 passwordLen)
{
	return loadImp(password, passwordLen, FALSE);
}

FS_BOOL FSPDFDocImp::reload()
{
	if (!m_pDoc) return FALSE;

	CFX_ByteString bsPW;
	if (FSPDF_LOADDOCTYPE_NEW != m_uLoadDocType)
	{
		bsPW = m_pParser->GetPassword();
		FS_PASSWORDTYPE passwordType = checkPassword(bsPW, bsPW.GetLength());
		if (e_pwdInvalid == passwordType)
			return FALSE;
	}
	if (FSCommonDefines::e_errSuccess != loadImp(bsPW, bsPW.GetLength(), TRUE))
		return FALSE;
	else
		return TRUE;
}

FS_BOOL	FSPDFDocImp::isEncrypted() const
{
	if (!m_pParser) return FALSE;

	return m_pParser->IsEncrypted();
}

void FSPDFDocImp::setModified()
{
	if (!m_pDoc) return;
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lockModified);
	m_isModified = TRUE; 
}
FS_BOOL	FSPDFDocImp::isModified() const
{
	if (!m_pDoc) return FALSE;
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lockModified);
	return m_isModified;
}

FS_PASSWORDTYPE FSPDFDocImp::getPasswordType() const
{
	if (FSPDF_LOADDOCTYPE_NEW == m_uLoadDocType) return e_pwdNoPassword;
	if (!m_pParser) return e_pwdInvalid;

	FS_ENCRYPTTYPE encryptType = getEncryptionType();
	if (e_encryptPassword != encryptType)
		return e_pwdNoPassword;

	if(m_pParser->IsOwner())
		return e_pwdOwner;
	else
		return e_pwdUser;
	return e_pwdInvalid;
}

FS_ENCRYPTTYPE FSPDFDocImp::getEncryptionType() const
{
	if (!m_pParser) return e_encryptNone;

	CPDF_Dictionary* pEncryptDict = m_pParser->GetEncryptDict();
	FS_ENCRYPTTYPE encryptType = e_encryptNone;
	if (pEncryptDict)
	{
		CFX_ByteString bsFilter = pEncryptDict->GetString(FX_BSTRC("Filter"));
		if (bsFilter == FX_BSTRC("Standard"))
			encryptType = e_encryptPassword;
		else if (bsFilter == FX_BSTRC("FoxitDRM"))
			encryptType = e_encryptFoxitDRM;
		else if (bsFilter == FX_BSTRC("Adobe.PubSec"))
			encryptType = e_encryptCertificate;
		else if (bsFilter == FX_BSTRC("MicrosoftIRMServices"))
			encryptType = e_encryptRMS;
		else
			encryptType = e_encryptCustom;
	}
	return encryptType;
}

FS_DWORD FSPDFDocImp::transformSaveFlag2Fxcore(FS_DWORD rdkSaveFlags)
{
	FS_DWORD dwFlag = 0;
	if (rdkSaveFlags & e_saveFlagIncremental)
		dwFlag |= FPDFCREATE_INCREMENTAL;
	if (rdkSaveFlags & e_saveFlagNoOriginal)
		dwFlag |= FPDFCREATE_NO_ORIGINAL;
	if (rdkSaveFlags & e_saveFlagObjectStream)
		dwFlag |= FPDFCREATE_OBJECTSTREAM;
	return dwFlag;
}

FS_BOOL FSPDFDocImp::saveAs(const char* filePath, FS_DWORD saveFlags)
{
	if (!m_pDoc || saveFlags>(e_saveFlagIncremental | e_saveFlagNoOriginal | e_saveFlagObjectStream /*| e_saveFlagRemoveSecurity*/))
		return FALSE;

	FSCRT_LOCKOBJECT(lock, &m_lockModified);
	SetCurrentSystemTimeToModifyDate();
	IFX_FileStream* pSaveFileStream = FSCRT_Util_CreateFileFromPath(filePath, FX_FILEMODE_Truncate);
	if (!pSaveFileStream) return FALSE;

	CPDF_Creator* pCreator = FX_NEW CPDF_Creator(m_pDoc);
	if (!pCreator)
	{
		pSaveFileStream->Release();
		return FALSE;
	}
	FS_DWORD fxSaveFlags = transformSaveFlag2Fxcore(saveFlags);
	if (m_pParser && m_pParser->IsXRefStream())
		fxSaveFlags |= FPDFCREATE_OBJECTSTREAM;
	else if (m_pParser && (saveFlags & e_saveFlagIncremental))
		fxSaveFlags &= ~FPDFCREATE_OBJECTSTREAM;


	FS_BOOL bRet = FALSE;
	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;

	if (FSCRT_ERRCODE_SUCCESS == ret)
	{
		 //Currently, not use FPDFCREATE_PROGRESSIVE for saving yet.
		 //If use FPDFCREATE_PROGRESSIVE, please call pCreate->Continue() next.
		bRet = pCreator->Create(pSaveFileStream, fxSaveFlags);
		delete pCreator;
		pSaveFileStream->Release();
		m_isModified = FALSE;
	}
	return bRet;
}

// DSS 20200901
string FSPDFDocImp::saveAsStream(FS_DWORD saveFlags)
{
	string strRet;

    if (!m_pDoc || saveFlags>(e_saveFlagIncremental | e_saveFlagNoOriginal /* | e_saveFlagObjectStream | e_saveFlagRemoveSecurity*/))
        return strRet;

    FSCRT_LOCKOBJECT(lock, &m_lockModified);
    SetCurrentSystemTimeToModifyDate();

	IFX_MemoryStream* pPdfFile = FX_CreateMemoryStream(TRUE);
	if (!pPdfFile)
		return strRet;

	CPDF_Creator* pCreator = FX_NEW CPDF_Creator(m_pDoc);
	if (!pCreator)
	{
		pPdfFile->Release();
		return strRet;
	}
    FS_DWORD fxSaveFlags = transformSaveFlag2Fxcore(saveFlags);
    if (m_pParser && m_pParser->IsXRefStream())
        fxSaveFlags |= FPDFCREATE_OBJECTSTREAM;
    else if (m_pParser && (saveFlags & e_saveFlagIncremental))
        fxSaveFlags &= ~FPDFCREATE_OBJECTSTREAM;

    //Currently, not use FPDFCREATE_PROGRESSIVE for saving yet.
	//If use FPDFCREATE_PROGRESSIVE, please call pCreate->Continue() next.
	FS_BOOL bRet = pCreator->Create(pPdfFile, fxSaveFlags);
	delete pCreator;
	if(bRet)
	{
		FX_FILESIZE	nSize = pPdfFile->GetSize();
		FX_LPBYTE pbBuf = pPdfFile->GetBuffer();
		strRet.assign((char*)pbBuf, nSize);
	}

	pPdfFile->Release();
	m_isModified = FALSE;

	return strRet;
}

FSBookmark* FSPDFDocImp::loadFirstBookmark(FS_BOOL bCreate /* = FALSE */)
{
	if (!m_pDoc) return NULL;
	if (m_pBmRoot && !bCreate) return m_pBmRoot;

	CPDF_Dictionary* pRoot = m_pDoc->GetRoot();
	if (!pRoot) return NULL;

	CPDF_Dictionary* pOutlinesDict = pRoot->GetDict("Outlines");
	if (!pOutlinesDict && !bCreate) return NULL;

	if (bCreate)
	{
		SAFE_DELETE(m_pBmRoot);
		pRoot->RemoveAt("Outlines");

		pOutlinesDict = FX_NEW CPDF_Dictionary;
		if (!pOutlinesDict) return NULL;

		FX_INT32 iOutlinesNum = m_pDoc->AddIndirectObject(pOutlinesDict);

		pRoot->SetAtReference(FX_BSTRC("Outlines"), m_pDoc, iOutlinesNum);
		pOutlinesDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("Outlines"));
		pOutlinesDict->SetAtInteger(FX_BSTRC("Count"), 0);
	}

	m_pBmRoot = FX_NEW FSBookmarkImp();
	if (!m_pBmRoot) return NULL;

	m_pBmRoot->init(this, pOutlinesDict);
	if (bCreate)
		setModified();

	return (FSBookmark*)m_pBmRoot;
}

FSBookmark* FSPDFDocImp::getFirstBookmark()
{
	return loadFirstBookmark();
}

FSBookmark* FSPDFDocImp::createFirstBookmark()
{
	return loadFirstBookmark(TRUE);
}

FS_BOOL FSPDFDocImp::removeBookmark(FSBookmark* bookmark)
{
	if (!m_pDoc || !bookmark || !m_pBmRoot) return FALSE;

	if(((FSBookmarkImp*)bookmark)->removeFromBMTree())
	{
		setModified();
		return TRUE;
	}

	return FALSE;
}

FS_INT32 FSPDFDocImp::getPageCount()
{
	if(!m_pDoc) return 0;
	return m_pDoc->GetPageCount();
}

FSPDFPage* FSPDFDocImp::getPageImp(FS_INT32 pageIndex, FS_BOOL bMapRetain)
{
	if (!m_pDoc || pageIndex < 0) return NULL;

	FSCRT_LOCKOBJECT(lock, &m_lockPages);
	FSPDFPageImp* pPDFPageImp = NULL;
	if (!m_pMapPage)
		m_pMapPage = FX_NEW CFX_MapPtrToPtr();
	if (!m_pMapPage) return NULL;

	FX_BOOL bRet = m_pMapPage->Lookup((void*)pageIndex, (void*&)pPDFPageImp);
	if (bRet && pPDFPageImp)
	{
		if (bMapRetain)
			return (FSPDFPage*)pPDFPageImp->retain();
		return (FSPDFPage*)pPDFPageImp;
	}

	pPDFPageImp = FX_NEW FSPDFPageImp();
	if (!pPDFPageImp)
		return NULL;
	if(!pPDFPageImp->init(this, pageIndex))
	{
		delete pPDFPageImp;
		return NULL;
	}
	m_pMapPage->SetAt((void*)pageIndex, pPDFPageImp);
	return (FSPDFPage*)pPDFPageImp;
}

FSPDFPage*	FSPDFDocImp::getPage(FS_INT32 pageIndex)
{
	return getPageImp(pageIndex, TRUE);
}

FS_BOOL FSPDFDocImp::closePage(FSPDFPage* pPage)
{
	if(!pPage || pPage->getDocument() != this
		|| !m_pMapPage)
		return FALSE;
	return closePage(pPage->getIndex());
}

FS_BOOL FSPDFDocImp::closePage(FS_INT32 pageIndex)
{
	FSCRT_LOCKOBJECT(lock, &m_lockPages);
	if(!m_pMapPage || pageIndex < 0 || pageIndex >= getPageCount())
		return FALSE;
	FSPDFPage* pFspdfPage = NULL;
	FX_BOOL bRet = m_pMapPage->Lookup((void*)pageIndex, (void*&)pFspdfPage);
	if (bRet && pFspdfPage && ((FSPDFPageImp*)pFspdfPage)->release()) 
	{
		m_pMapPage->RemoveKey((void*)pageIndex);
	}
	return TRUE;
}

/** @see e_displayXXX.*/
FS_DISPLAYMODE FSPDFDocImp::getDisplayMode() const
{
	if (!m_pDoc) return FSCommonDefines::e_displayUseNone;

	CPDF_Dictionary* pRootDict = m_pDoc->GetRoot();
	if (!pRootDict) return FSCommonDefines::e_displayUseNone;

	CPDF_Name* pPageMode = (CPDF_Name*)pRootDict->GetElement(FX_BSTRC("PageMode"));
	if (!pPageMode) return FSCommonDefines::e_displayUseNone;

	CFX_ByteString bsMode = pPageMode->GetString();
	if (bsMode.IsEmpty() || bsMode == FX_BSTRC("UseNone"))
		return FSCommonDefines::e_displayUseNone;
	else if (bsMode == FX_BSTRC("UseOutlines"))
		return FSCommonDefines::e_displayUseOutlines;
	else if (bsMode == FX_BSTRC("UseThumbs"))
		return FSCommonDefines::e_displayUseThumbs;
	else if (bsMode == FX_BSTRC("FullScreen"))
		return FSCommonDefines::e_displayFullScreen;
	else if (bsMode == FX_BSTRC("UseOC"))
		return FSCommonDefines::e_displayUseOC;
	else if (bsMode == FX_BSTRC("UseAttachments"))
		return FSCommonDefines::e_displayUseAttachment;

	return FSCommonDefines::e_displayUseNone;
}

//Get specific dictionary objects.
FSPDFDictionary* FSPDFDocImp::getCatalog() const
{
	if (!m_pDoc) 
		return NULL;

	return (FSPDFDictionary*)(m_pDoc->GetRoot());
}

FSPDFDictionary* FSPDFDocImp::getTrailer() const
{
	if (!m_pParser) 
		return NULL;
	return (FSPDFDictionary*)m_pParser->GetTrailer();
}

FSPDFDictionary* FSPDFDocImp::getInfo() const
{
	if (!m_pDoc) 
		return NULL;

	return (FSPDFDictionary*) m_pDoc->GetInfo();
}

FSPDFDictionary* FSPDFDocImp::getEncryptDict() const
{
	if (!m_pParser) 
		return NULL;

	return (FSPDFDictionary*)m_pParser->GetEncryptDict();
}

//Indirect objects related methods
FSPDFObject* FSPDFDocImp::getIndirectObject(FS_DWORD objnum)
{
	if (!m_pDoc || objnum < 1)
		return NULL;

	return (FSPDFObject*)m_pDoc->GetIndirectObject(objnum);
}

FS_DWORD FSPDFDocImp::addIndirectObject(FSPDFObject* pObj)
{
	if (!m_pDoc || !pObj)
		return 0;
	FSCRT_LOCKOBJECT(lock, &m_lockModified);
	setModified();
	return m_pDoc->AddIndirectObject((CPDF_Object*)pObj);
}

void FSPDFDocImp::deleteIndirectObject(FS_DWORD objnum)
{
	if (!m_pDoc || objnum < 1)
		return;

	FSCRT_LOCKOBJECT(lock, &m_lockModified);
	if(m_pDoc->GetIndirectObject(objnum) != NULL)
		setModified();
	m_pDoc->DeleteIndirectObject(objnum);
}

FS_DWORD FSPDFDocImp::getUserPermissions() const
{
	if (!m_pDoc) return 0;

	return m_pDoc->GetUserPermissions();
}

FS_BOOL FSPDFDocImp::isWrapper() const
{
	if (!m_pParser) return FALSE;

	CPDF_Dictionary* pTrailer = m_pParser->GetTrailer();
	if (!pTrailer) return FALSE;

	CPDF_Dictionary *pWrapper = pTrailer->GetDict(FX_BSTRC("Wrapper"));
	return (pWrapper != NULL);
}

FSWrapperData FSPDFDocImp::getWrapperData() const
{
	FSWrapperData fsWrapperData;
	fsWrapperData.set(0, NULL, NULL, NULL, NULL);
	if (!m_pParser) return fsWrapperData;

	CPDF_Dictionary* pTrailer = m_pParser->GetTrailer();
	if (!pTrailer) return fsWrapperData;

	CPDF_Dictionary* pWrapperDict = pTrailer->GetDict(FX_BSTRC("Wrapper"));
	if (!pWrapperDict) return fsWrapperData;

	fsWrapperData.version = pWrapperDict->GetInteger(FX_BSTRC("Version"));

	CFX_ByteString bsType = pWrapperDict->GetString(FX_BSTRC("Type"));
	CFX_ByteString bsUTF8 = FSCRT_UTF8_FromTextString(bsType);
	fsWrapperData.type = FSString(bsUTF8.GetBuffer(bsUTF8.GetLength()), bsUTF8.GetLength());
	bsUTF8.ReleaseBuffer();

	CFX_ByteString bsAppID = pWrapperDict->GetString(FX_BSTRC("Application"));
	bsUTF8 = FSCRT_UTF8_FromTextString(bsAppID);
	fsWrapperData.appID = FSString(bsUTF8.GetBuffer(bsUTF8.GetLength()), bsUTF8.GetLength());
	bsUTF8.ReleaseBuffer();

	CFX_ByteString bsURI = pWrapperDict->GetString(FX_BSTRC("URI"));
	bsUTF8 = FSCRT_UTF8_FromTextString(bsURI);
	fsWrapperData.uri = FSString(bsUTF8.GetBuffer(bsUTF8.GetLength()), bsUTF8.GetLength());
	bsUTF8.ReleaseBuffer();

	CFX_ByteString bsDes = pWrapperDict->GetString(FX_BSTRC("Description"));
	bsUTF8 = FSCRT_UTF8_FromTextString(bsDes);
	fsWrapperData.description = FSString(bsUTF8.GetBuffer(bsUTF8.GetLength()), bsUTF8.GetLength());
	bsUTF8.ReleaseBuffer();

	return fsWrapperData;
}

FS_INT32 FSPDFDocImp::getWrapperOffset() const
{
	if (!m_pParser) return -1;

	CPDF_Dictionary* pTrailer = m_pParser->GetTrailer();
	if (!pTrailer) return -1;

	CPDF_Dictionary *pWrapper = pTrailer->GetDict(FX_BSTRC("Wrapper"));
	if (!pWrapper) return -1;

	return pTrailer->GetInteger(FX_BSTRC("WrapperOffset"));
}

FSDateTime FSPDFDocImp::getCreationDateTime() const
{
	return getMetadataTime(FX_WSTRC(L"CreationDate"));
}

FSDateTime FSPDFDocImp::getModifiedDateTime() const
{
	return getMetadataTime(FX_WSTRC(L"ModDate"));
}

FSDateTime FSPDFDocImp::getMetadataTime(CFX_WideStringC wstrTimeKey) const
{
	FSDateTime fsDateTime;
	fsDateTime.set(0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (!m_pDoc) return fsDateTime;

	CPDF_Metadata metadata;
	FX_BOOL bRet = metadata.LoadDoc(m_pDoc);
	// For some PDF file which only have /Info dictionary, CPDF_Metadata::LoadDoc() will return FALSE.
	// Need to wait fxcore team to check : jira CORE-2539
	//if (FALSE == bRet) return fsDateTime;

	FXCRT_DATETIMEZONE dt;
	FX_INT32 bUseInfoOrXML;
	FX_INT32 ret = metadata.GetDateTime(wstrTimeKey, dt, bUseInfoOrXML);
	if (ret != 0) return fsDateTime;

	fsDateTime.set(	dt.year, dt.month, dt.day, 
					dt.hour, dt.minute, dt.second, dt.milliseconds, 
					dt.tzHour, dt.tzMinute);
	return fsDateTime;
}

FS_BOOL FSPDFDocImp::hasMetadataKey(const char* key) const
{
	if (!m_pDoc || FSCRT_Util_IsStringEmpty(key)) return FALSE;

	CPDF_Metadata metadata;
	FX_BOOL bRet = metadata.LoadDoc(m_pDoc);
	// For some PDF file which only have /Info dictionary, CPDF_Metadata::LoadDoc() will return FALSE.
	// Need to wait fxcore team to check : jira CORE-2539
	//if (FALSE == bRet) return FALSE;

	FX_INT32 bUseInfoOrXML;
	FX_LPWSTR lpWstrKey = FSCRT_UTF8_DecodeToUnicode(key);
	if (!lpWstrKey) return FALSE;

	FX_INT32 iRet = 0;
	CFX_WideString wstrKey(lpWstrKey);
	if (wstrKey.Equal(FX_WSTRC(L"ModDate")) || wstrKey.Equal(FX_WSTRC(L"CreationDate")))
	{
		FXCRT_DATETIMEZONE dt;
		iRet = metadata.GetDateTime(wstrKey, dt, bUseInfoOrXML);
	}
	else
	{
		CFX_WideString ws;
		iRet = metadata.GetString(wstrKey, ws, bUseInfoOrXML);
	}
	FX_Free(lpWstrKey);
	return (0 == iRet);
}

FSString FSPDFDocImp::getMetadataValue(const char* key) const
{
	if (!m_pDoc || FSCRT_Util_IsStringEmpty(key)) return FALSE;

	CPDF_Metadata metadata;
	FX_BOOL bRet = metadata.LoadDoc(m_pDoc);
	// For some PDF file which only have /Info dictionary, CPDF_Metadata::LoadDoc() will return FALSE.
	// Need to wait fxcore team to check : jira CORE-2539
	//if (FALSE == bRet) return FALSE;

	FX_LPWSTR lpWstrKey = FSCRT_UTF8_DecodeToUnicode(key);
	if (!lpWstrKey) return FSString();

	CFX_WideString wsStr;
	FX_INT32 bUseInfoOrXML;
	FX_INT32 ret = metadata.GetString(lpWstrKey, wsStr, bUseInfoOrXML);
	FX_Free(lpWstrKey);
	if (0 == ret)
	{
		FSString strValue = FSString::createFromWideString(wsStr);
		strValue.toUTF8();
		return strValue;
	}
	return FSString();
}

FS_INT32 FSPDFDocImp::getPageLabelRangeCount() const
{
	if (!m_pDoc) return 0;

	CPDF_Dictionary* pRootDict = m_pDoc->GetRoot();
	if (!pRootDict) return 0;

	CPDF_Dictionary* pPageLabelsDict = pRootDict->GetDict(FX_BSTRC("PageLabels"));
	if (!pPageLabelsDict) return 0;

	CPDF_Array* pNumsArray = pPageLabelsDict->GetArray("Nums");
	if (!pNumsArray) return 0;

	FS_INT32 arrayCount = pNumsArray->GetCount();
	//The count must be even number because elements of "Nums" array are paired.
	if (arrayCount%2 != 0) 
		return 0;

	return arrayCount/2;
}

void FSPDFDocImp::SetCurrentSystemTimeToModifyDate()
{
	CFSPDF_DateTime tData;
	tData.SetCurrentSysTime();

	CPDF_Metadata metadata;
	FX_BOOL bRet = metadata.LoadDoc(m_pDoc);
	FXCRT_DATETIMEZONE fxDT;
	fxDT.year = tData.m_fsDatetime.year;
	fxDT.month = tData.m_fsDatetime.month;
	fxDT.day = tData.m_fsDatetime.day;
	fxDT.hour = tData.m_fsDatetime.hour;
	fxDT.minute = tData.m_fsDatetime.minute;
	fxDT.second = tData.m_fsDatetime.second;
	fxDT.milliseconds = tData.m_fsDatetime.milliseconds;
	fxDT.tzHour = tData.m_fsDatetime.UTHourOffset;
	fxDT.tzMinute = tData.m_fsDatetime.UTMinuteOffset;
	metadata.SetDateTime((FX_WSTR)L"ModDate", fxDT);
}

FSPageLabel FSPDFDocImp::getPageLabelInfo(FS_INT32 index) const
{
	FSPageLabel fsLabelInfo;
	fsLabelInfo.set(-1, e_pageLabelStyleNone, NULL, 0);

	if (!m_pDoc) return fsLabelInfo;

	CPDF_Dictionary* pRootDict = m_pDoc->GetRoot();
	if (!pRootDict) return fsLabelInfo;

	CPDF_Dictionary* pPageLabelsDict = pRootDict->GetDict(FX_BSTRC("PageLabels"));
	if (!pPageLabelsDict) return fsLabelInfo;

	CPDF_Array* pNumsArray = pPageLabelsDict->GetArray("Nums");
	if (!pNumsArray) return fsLabelInfo;

	FS_INT32 arrayCount = pNumsArray->GetCount();
	//The count must be even number because elements of "Nums" array are paired.
	if (arrayCount%2 != 0) return fsLabelInfo;

	if (index < 0 || index >= arrayCount/2) return fsLabelInfo;
	CPDF_Object* pPDFObject = pNumsArray->GetElement(index*2+1); //for either direct or reference object
	if (NULL == pPDFObject) return fsLabelInfo;
	CPDF_Dictionary* pDictRange = pPDFObject->GetDict();
	if (NULL == pPDFObject) return fsLabelInfo;
	CFX_ByteString bsStyle = pDictRange->GetString(FX_BSTRC("S"));
	fsLabelInfo.style = e_pageLabelStyleNone;
	if (FALSE == bsStyle.IsEmpty())
	{
		const FX_CHAR szPageLabelsStyles[] = {'D', 'R', 'r', 'A', 'a'};
		const FX_INT32 iStyleCount = sizeof(szPageLabelsStyles) / sizeof(szPageLabelsStyles[0]);
		for (FX_INT32 i = 0; i < iStyleCount; i++)
		{
			if (bsStyle == szPageLabelsStyles[i])
			{
				fsLabelInfo.style = (FS_PAGELABELSTYLE)(i+1);
				break;
			}
		}
	}

	if (pDictRange->KeyExist(FX_BSTRC("St")))
		fsLabelInfo.firstPageNumber = pDictRange->GetInteger(FX_BSTRC("St"));
	else
		fsLabelInfo.firstPageNumber = 1;

	CFX_WideString wsPrefix = pDictRange->GetUnicodeText(FX_BSTRC("P"));
	CFX_ByteString bsPrefix = wsPrefix.UTF8Encode();
	FX_INT32 len = 100;
	if (bsPrefix.GetLength() < len) len = bsPrefix.GetLength();
	FXSYS_memcpy(fsLabelInfo.prefix, bsPrefix.GetBuffer(-1), len);

	fsLabelInfo.start = pNumsArray->GetInteger(index*2);

	return fsLabelInfo;
}

FS_BOOL FSPDFDocImp::hasForm() const
{
	if (!m_pDoc)
		return FALSE;
	CPDF_Dictionary* catalogDic = m_pDoc->GetRoot();
	if (!catalogDic)
		return FALSE;
	CPDF_Dictionary* pAcroForm = catalogDic->GetDict("AcroForm");
	if (!pAcroForm)
		return FALSE;
	CPDF_Array* pFields = pAcroForm->GetArray(FX_BSTR("Fields"));
	if (!pFields)
		return FALSE;
	if(pFields->GetCount() == 0)
		return FALSE; 
	return TRUE;
}

FS_BOOL FSPDFDocImp::addToFontMaps(FSFontImp* pFSFont, CPDF_Dictionary* pFontDict)
{
	FSCRT_LOCKOBJECT(lock, &m_lockFontMaps);
	if (!m_pDoc || !m_pMapFontDict2FSFont /*|| !m_pMapFSFont2FontDict */
		|| !pFSFont || !pFontDict) 
		return FALSE;
	
	m_pMapFontDict2FSFont->SetAt(pFontDict, pFSFont);
	//m_pMapFSFont2FontDict->SetAt(pFSFont, pFontDict);
	return TRUE;
}

FSFontImp* FSPDFDocImp::getFSFont(CPDF_Font* pPDFFont)
{
	FSCRT_LOCKOBJECT(lock, &m_lockFontMaps);
	if (!m_pDoc || !m_pMapFontDict2FSFont /*|| !m_pMapFSFont2FontDict */
		|| !pPDFFont) 
		return FALSE;
	void* pData = NULL;
	// 1. check if there's a matched FSFontImp.
	// If yes, use it directly.
	if (m_pMapFontDict2FSFont->Lookup(pPDFFont->GetFontDict(), pData))
		return (FSFontImp*)pData;
	
	// 2. If no matched FSFontImp, create a new FSFontImp 
	// and record to maps in FSFontImp and FSPDFDocImp.
	// Create a new FSFontImp (based on the CPDF_Font) and record to FSFontImp's map.
	FSFontImp* pFSFont = FX_NEW FSFontImp;
	if (!pFSFont) return NULL;
	if (FALSE == pFSFont->initFromPDFFont(this, pPDFFont))
	{
		pFSFont->release();
		return NULL;
	}
	// Record to FSPDFDocImp's maps
	addToFontMaps(pFSFont, pPDFFont->GetFontDict());
	return pFSFont;
}

FS_BOOL FSPDFDocImp::removeMapRecord(/*FSFontImp* pFSFont*/CPDF_Dictionary* pFontDict)
{
	FSCRT_LOCKOBJECT(lock, &m_lockFontMaps);
	if (!m_pDoc || /*!pFSFont*/!pFontDict) 
		return FALSE;
	if ( /*!m_pMapFSFont2FontDict ||*/ !m_pMapFontDict2FSFont)
		return TRUE;

	//void* pData;
	//m_pMapFSFont2FontDict->Lookup(pFSFont, pData);
	//m_pMapFSFont2FontDict->RemoveKey(pFSFont);
	//if (pData)
	//	m_pMapFontDict2FSFont->RemoveKey(pData);
	m_pMapFontDict2FSFont->RemoveKey(pFontDict);
	return TRUE;
}

FSForm*	FSPDFDocImp::getForm()
{
	if (!m_pDoc)
		return NULL;
	if(m_pForm)
		return m_pForm;
	if(!hasForm())
		return NULL;
	CPDF_Dictionary* rootDict = m_pDoc->GetRoot();
	if (!rootDict || !rootDict->KeyExist(FX_BSTR("AcroForm")))
		return NULL;

	m_pForm = FX_NEW FSFormImp();
	if (!m_pForm)
		return NULL;

	if (m_pForm->ST_LoadFromPDF(this) != FSCRT_ERRCODE_SUCCESS)
	{
		delete m_pForm;
		return NULL;
	}
	m_pForm->SetPDFDoc(this);
	setForm(m_pForm);

	return m_pForm;
}

FSFormImp*	 FSPDFDocImp::GetForm()
{
	return (FSFormImp*)getForm();
}
IJS_Runtime* FSPDFDocImp::GetJsRuntime()
{
	if (m_pJSRuntime)
		return m_pJSRuntime;

	FSFormImp* pForm = GetForm();
	if(!pForm) return NULL;

#if defined(ENABLE_JSENGINE)
	return FSCRT_GetJSRuntimeFactory()->GetJSRuntime(NULL);
#else
	return NULL;
#endif

}

FSAnnot* FSPDFDocImp::GetFocusAnnot()
{
	return NULL;
}

void FSPDFDocImp::GetPageCount(FX_INT32* nPageCount)
{
	if(m_pDoc && nPageCount)
		*nPageCount = m_pDoc->GetPageCount();
}

void FSPDFDocImp::GetPage(FX_INT32 nPageIndex, FSPDFPage** page)
{
	if(!page) return;
	*page = getPage(nPageIndex);
	return;
}

//Use for setting the XML data to Metadata Stream. 
FS_BOOL FSPDFDocImp::updateMetadataStream()
{
	if (!m_pDoc || !m_pMetadata) return FALSE;

	CXML_Element* pMetdataRootElement = m_pMetadata->GetRoot();
	if (!pMetdataRootElement) return FALSE;

	CPDF_Dictionary *pRoot = m_pDoc->GetRoot();
	if (!pRoot) return FALSE;

	CPDF_Stream *pXMLStream = pRoot->GetStream(FX_BSTRC("Metadata"));
	if (!pXMLStream) return FALSE;

	CFX_ByteString bstrXMLStream = pMetdataRootElement->OutputStream();
	bstrXMLStream = FSPDF_METADATA_BEGINXML + bstrXMLStream + FSPDF_METADATA_ENDXML;
	pXMLStream->SetData(bstrXMLStream, bstrXMLStream.GetLength(), FALSE, FALSE);

	return TRUE;
}

FS_BOOL FSPDFDocImp::createNewMetadataRDF()
{
	if (!m_pMetadata) return FALSE;

	CXML_Element* pMetadataRoot = m_pMetadata->GetRoot();
	if (!pMetadataRoot) return FALSE;

	CXML_Element* pRDFElem = FX_NEW CXML_Element(FSPDF_METADATA_rdf, FSPDF_METADATA_RDF);
	if (!pRDFElem) return FALSE;

	pRDFElem->SetAttrValue(FX_BSTRC("xmlns:rdf"), FX_WSTRC(L"http://www.w3.org/1999/02/22-rdf-syntax-ns#"));
	pMetadataRoot->AddChildElement(pRDFElem);

	return updateMetadataStream();
}

//Load the root <pdfx:FoxitReadingBookmark>
CXML_Element* FSPDFDocImp::loadXMLReadingBookmark(FS_BOOL bCreate /*= FALSE*/)
{
	if (m_pXMLReadingBookmark) return m_pXMLReadingBookmark;

	CXML_Element* pRootReadingBookmark = NULL;
	if (!m_pMetadata)
	{
		m_pMetadata = FX_NEW CPDF_Metadata();
		if (!m_pMetadata) return NULL;
		m_pMetadata->LoadDoc(m_pDoc);
	}

	CXML_Element* pMetadataRoot = m_pMetadata->GetRoot();
	if (!pMetadataRoot && bCreate)
	{
		//If no metadata, we should create.
		if (!m_pMetadata->CreateNewMetadata()) return NULL;
		pMetadataRoot = m_pMetadata->GetRoot();
	}
	if (!pMetadataRoot) return NULL;
 
	CXML_Element* pRDF = m_pMetadata->GetRDF();
	if (!pRDF && bCreate)
	{
		//If no RDF, we should create.
		if (!createNewMetadataRDF()) return NULL;
		pRDF = m_pMetadata->GetRDF();
	}
	if (!pRDF) return NULL;

	//count the <rdf:description>
	FX_DWORD dwCountRDF = pRDF->CountElements(FSPDF_METADATA_rdf, FSPDF_METADATA_DESCRIPTION);
	if (dwCountRDF == 0 && !bCreate) return NULL;

	for (FS_DWORD i = 0; i < dwCountRDF; i++)
	{
		//find the <xmlns:pdfx>
		CXML_Element* pDescription = pRDF->GetElement(FSPDF_METADATA_rdf, FSPDF_METADATA_DESCRIPTION, i);
		if (!pDescription) continue;
		if (FALSE == pDescription->HasAttr(FSPDF_METADATA_PDFXTAG)) continue;

		//get or create <pdfx:FoxitReadingBookmark>
		pRootReadingBookmark = pDescription->GetElement(FSPDF_METADATA_PDFX, FSREADINGBOOKMARK_ROOTSPACE);
		//If no <pdfx:FoxitReadingBookmark>, we should create.
		if (!pRootReadingBookmark && bCreate)
		{
			pRootReadingBookmark = FX_NEW CXML_Element(FSPDF_METADATA_PDFX, FSREADINGBOOKMARK_ROOTSPACE);
			if (!pRootReadingBookmark) return NULL;
			pDescription->AddChildElement(pRootReadingBookmark);
		}
		break;
	}

	//If no <rdf：descrition xmlns:pdfx = "....">, we should create.
	if (!pRootReadingBookmark && bCreate)
	{
		CXML_Element* pDescriptionElem = FX_NEW CXML_Element(FSPDF_METADATA_rdf, FSPDF_METADATA_DESCRIPTION);
		if (!pDescriptionElem) return NULL;

		//set pdfx attribute.
		pDescriptionElem->SetAttrValue("rdf:about", FX_WSTRC(L""));
		pDescriptionElem->SetAttrValue(FSPDF_METADATA_PDFXTAG, FX_WSTRC(L"http://ns.adobe.com/pdfx/1.3/"));       

		pRootReadingBookmark = FX_NEW CXML_Element(FSPDF_METADATA_PDFX, FSREADINGBOOKMARK_ROOTSPACE);
		if (!pRootReadingBookmark) {
			delete pDescriptionElem;
			return NULL;
		}

		pDescriptionElem->AddChildElement(pRootReadingBookmark);
		pRDF->AddChildElement(pDescriptionElem);
	}

	if (bCreate)
		updateMetadataStream();

	return pRootReadingBookmark;
}

FS_BOOL	FSPDFDocImp::initReadingBookmark(FS_BOOL bCreate /*= FALSE*/)
{
	if (!m_pDoc) return FALSE;
	
	FSCRT_LOCKOBJECT(lock, &m_lockReadingBookmarks);
	if (m_pReadingBookmarksArray) return TRUE;

	m_pXMLReadingBookmark = loadXMLReadingBookmark(bCreate);
	if (!m_pXMLReadingBookmark) return FALSE;

	//If the children of <pdfx:FoxitReadingBookmark> is no all <FoxitReadingBookmark:Bookmark>, we should return false.
	FS_INT32 countElement = m_pXMLReadingBookmark->CountElements(FSREADINGBOOKMARK_ROOTSPACE, FSREADINGBOOKMARK_ROOTTAG);
	FS_INT32 countChildren = m_pXMLReadingBookmark->CountChildren();

	if (countElement != countChildren) return FALSE;

	//create FSReadingBookmark and set them to m_pReadingBookmarksArray.
	m_pReadingBookmarksArray = FX_NEW CFX_PtrArray();
	if (!m_pReadingBookmarksArray) return FALSE;

	if (!m_pReadingBookmarksArray->SetSize(countElement)) return FALSE;

	for (FS_INT32 i = 0; i < countElement; i++)
	{
		CXML_Element* pXMLElement = m_pXMLReadingBookmark->GetElement(i);
		FSReadingBookmarkImp* fsReadingBookmark = FX_NEW FSReadingBookmarkImp(pXMLElement, this);
		m_pReadingBookmarksArray->SetAt(i, fsReadingBookmark);
	}

	return TRUE;
}

FS_INT32 FSPDFDocImp::getReadingBookmarkCount()
{
	if (!m_pDoc || !initReadingBookmark()) return 0;

	FSCRT_LOCKOBJECT(lock, &m_lockReadingBookmarks);
	return m_pReadingBookmarksArray->GetSize();
}

FSReadingBookmark* FSPDFDocImp::getReadingBookmark(FS_INT32 index)
{
	if (!m_pDoc || index < 0 || !initReadingBookmark()) return NULL;

	FSCRT_LOCKOBJECT(lock, &m_lockReadingBookmarks);
	if (index >= m_pReadingBookmarksArray->GetSize()) return NULL;

	return (FSReadingBookmark*)m_pReadingBookmarksArray->GetAt(index);
}

FSReadingBookmark* FSPDFDocImp::insertReadingBookmark(FS_INT32 readingBookmarkIndex, const char* title, FS_INT32 destPageIndex)
{
	if (!m_pDoc || FSCRT_Util_IsStringEmpty(title) || !initReadingBookmark(TRUE)) return NULL;

	FX_INT32 pageCount = m_pDoc->GetPageCount();
	if (destPageIndex < 0 || destPageIndex >= pageCount) return NULL;

	FX_INT32 iCount = getReadingBookmarkCount();

	if (readingBookmarkIndex < 0) readingBookmarkIndex = 0;
	if (readingBookmarkIndex >= iCount) readingBookmarkIndex = iCount;

	CFX_WideString wsTitle;
	wsTitle = CFX_WideString::FromUTF8(title, FXSYS_strlen(title));

	CFX_WideString wsPageIndex;
	CFX_ByteString bsPageIndex;
	bsPageIndex.Format("%d", destPageIndex);
	wsPageIndex.ConvertFrom(bsPageIndex);

	CFSPDF_DateTime tData;
	tData.SetCurrentSysTime();
	CFX_WideString wsCurrentSysTime = tData.ToXMPDateTimeString();

	CXML_Element* pXMLElement = FX_NEW CXML_Element(FSREADINGBOOKMARK_ROOTSPACE, FSREADINGBOOKMARK_ROOTTAG);
	if (!pXMLElement) return NULL;

	CFX_ByteString key[] = {FSREADINGBOOKMARK_CHILDPARAMTAG_TITLE, FSREADINGBOOKMARK_CHILDPARAMTAG_PAGEINDEX,
				FSREADINGBOOKMARK_CHILDPARAMTAG_CREATIONDATETIME, FSREADINGBOOKMARK_CHILDPARAMTAG_MODIFIEDDATETIME};

	CFX_WideString contents[] = {wsTitle, wsPageIndex, wsCurrentSysTime, wsCurrentSysTime};

	for (FS_INT32 i = 0; i < sizeof(key)/sizeof(key[0]); i++)
	{
		CXML_Element* pChildElement = FX_NEW CXML_Element(FSREADINGBOOKMARK_ROOTTAG, key[i]);
		if (!pChildElement)		
		{
			delete pXMLElement;
			return NULL;
		}

		pChildElement->AddChildContent(contents[i]);
		pXMLElement->AddChildElement(pChildElement);
	}

	FSReadingBookmarkImp* fsBookmarkImp = FX_NEW FSReadingBookmarkImp(pXMLElement, this);
	if (!fsBookmarkImp)
	{
		delete pXMLElement;
		return NULL;
	}
	m_pXMLReadingBookmark->InsertChildElement(readingBookmarkIndex, pXMLElement);

	FSCRT_LOCKOBJECT(lock, &m_lockReadingBookmarks);
	m_pReadingBookmarksArray->InsertAt(readingBookmarkIndex, fsBookmarkImp);

	updateMetadataStream();

	setModified();

	return (FSReadingBookmark*)fsBookmarkImp;
}

FS_BOOL FSPDFDocImp::removeReadingBookmark(FSReadingBookmark* readingBookmark)
{
	if (!readingBookmark || !m_pXMLReadingBookmark || !m_pReadingBookmarksArray) return FALSE;

	FSCRT_LOCKOBJECT(lock, &m_lockReadingBookmarks);
	FX_INT32 index = m_pReadingBookmarksArray->Find(readingBookmark, 0);
	if (index < 0) return FALSE;

	m_pXMLReadingBookmark->RemoveChild(index);

	delete readingBookmark;
	m_pReadingBookmarksArray->RemoveAt(index, 1);

	updateMetadataStream();

	setModified();

	return TRUE;
}

FS_INT32	FSPDFDocImp::getSignatureCount()
{
	if (!m_pSigEdit && !loadSignatures())
		return 0;
#if defined(ENABLE_SIGNATURE)
	return m_pSigEdit->CountSignatures();
#else
	return 0;
#endif
}

FSSignature*	FSPDFDocImp::getSignature(FS_INT32 index)
{
	if (!CFS_Library::get()) return NULL;
	FS_INT32 nSigCount = getSignatureCount();
	if (index<0 || index>=nSigCount) return NULL;

#if defined(ENABLE_SIGNATURE)
	CPDF_Signature* pSig = m_pSigEdit->GetSignature(index);
	if (!pSig) return NULL;

	//Get the page which signature belongs to.
	FSSignature* pFSSig = NULL;
	FSPDFPage* pFSPage = NULL;
	CPDF_Dictionary* pSigFieldDict = pSig->GetSignatureDict();
	if (pSigFieldDict->KeyExist("P"))
	{
		CPDF_Dictionary* pSigPage = pSigFieldDict->GetDict("P");
		pFSPage = getPageByDict(pSigPage);
		pFSSig = (FSSignature*)((FSPDFPageImp*)pFSPage)->getAnnotbyDict(pSigFieldDict);
	}
	else
	{
		// Enumerate all the page to find the signature object.
		FS_INT32 nPageCount = getPageCount();
		for (FS_INT32 iPageIndex = 0; iPageIndex<nPageCount; iPageIndex++)
		{
			FSPDFPageImp* pPage = (FSPDFPageImp*)getPage(iPageIndex);
			if (!pPage) continue;
			FSAnnot* pTempAnnot = pPage->getAnnotbyDict(pSigFieldDict);
			if (pTempAnnot)
			{
				pFSSig = (FSSignature*)pTempAnnot;
				break;
			}
		}
	}
	return pFSSig;
#else
	return 0;
#endif
}

FS_BOOL	FSPDFDocImp::loadSignatures()
{
	if (!m_pDoc) return FALSE;
#if defined(ENABLE_SIGNATURE)
	if (!m_pSigEdit)
	{
		m_pSigEdit = FX_NEW CPDF_SignatureEdit(m_pDoc);
		if (!m_pSigEdit)
			return FALSE;
	}
	FS_BOOL bRet = m_pSigEdit->LoadSignatures();
	if (FALSE == bRet)
		SAFE_DELETE(m_pSigEdit);
	return bRet;
#else
	return TRUE;
#endif
}

CPDF_Signature*	FSPDFDocImp::getSignatureAtPos(CPDF_Page* pPage, FSPointF point, FS_FLOAT fTolerance)
{
	if (!pPage || !m_pDoc) return NULL;
	if (!m_pSigEdit && !loadSignatures()) return NULL;

#if defined(ENABLE_SIGNATURE)
	return m_pSigEdit->GetSignatureAtPos(pPage, point.x, point.y, fTolerance);
#else
	return NULL;
#endif
}

FS_BOOL		FSPDFDocImp::addSignature(CPDF_Signature* pNewSig, CPDF_Page* pPage, FSRectF newRect)
{
	if (!pNewSig || !pPage) return FALSE;
	if (!m_pSigEdit && !loadSignatures()) return FALSE;

#if defined(ENABLE_SIGNATURE)
	CPDF_Rect tempRect(newRect.left, newRect.bottom, newRect.right, newRect.top);
	return m_pSigEdit->AddNewSignature(pNewSig, pPage, &tempRect);
#else
	return NULL;
#endif
}

FS_BOOL		FSPDFDocImp::removeSignatureField(CPDF_Signature* pSig)
{
	//TODO: remove the new signature object from document.
	return FALSE;
}

FS_BOOL		FSPDFDocImp::clearSignedData(CPDF_Signature* pSig)
{// When this is called, that means the pSig is retrieved from CPDF_SignatureEdit or added to CPDF_SignatureEdit
 // So m_pSigEdit should be valid.
	if (!pSig) return FALSE;
	if (!m_pSigEdit) return FALSE;
#if defined(ENABLE_SIGNATURE)
	FS_BOOL ret = m_pSigEdit->RemoveSignature(pSig);
	setModified();
	return ret;
#else
	return NULL;
#endif
}


CPDF_SignatureEdit* FSPDFDocImp::getSigantureEdit()
{
	if (!m_pSigEdit)
		loadSignatures();
	return m_pSigEdit;
}

FSPDFPage*	FSPDFDocImp::getPageByDict(CPDF_Dictionary* pPageDict)
{
	if (!pPageDict || !m_pDoc) return NULL;

	FS_INT32 iExpectPageIndex = -1;
	FS_DWORD dwExpectPageObjNum = pPageDict->GetObjNum();
	FS_INT32 nPageCount = m_pDoc->GetPageCount();
	for (FS_INT32 i=0; i<nPageCount; i++)
	{
		CPDF_Dictionary* pPageDict = m_pDoc->GetPage(i);
		if (!pPageDict) continue;
		if (pPageDict->GetObjNum() == dwExpectPageObjNum)
		{
			iExpectPageIndex = i;
			break;
		}
	}
	if (iExpectPageIndex>-1)
		return getPage(iExpectPageIndex);
	return NULL;
}

CFSCRT_STPDFActionHandler* FSPDFDocImp::GetActionHandler()
{
	getForm();

	if(!m_pSTActionHandler && m_pForm)
	{
		m_pSTActionHandler = FX_NEW CFSCRT_STPDFActionHandler();
		m_pForm->ST_GetSTInterForm()->SetActionHandler(m_pSTActionHandler);
	}
	return m_pSTActionHandler;
}



bool ParseSignedvalueImage(const unsigned char *pBuf, const int nBufSize, unsigned char *&pImageDataBuf, int &nImageSize2Res, int *pImgSubType)
{
	std::string strError = "";
	bool bRes = false;

	{
		if (pBuf == NULL || nBufSize <= 0){
			strError = "parameter error";
			goto end;
		}

		int nOffset = 0;
		int nOffsetType = 0;

		while (nOffset < nBufSize){

			nOffsetType = nOffset + 1;
			if (nOffsetType >= nBufSize){
				bRes = false;
				goto end;
			}

			//SEQUENCE Tag = 0x30
			//长编码【>128】 0x81 : length 1 byte , 0x82 : length 2 byte, 0x83 : length 3 byte, 0x84 : length 4 byte.
			//4 byte, 32位 ， 2147483647 , 21亿，当前规定的最大值。
			//图片数据，注定是长编码
			int nType = 0;
			if (pBuf[nOffset] == 0x30){
				if (pBuf[nOffsetType] == 0x81 ||
					pBuf[nOffsetType] == 0x82 ||
					pBuf[nOffsetType] == 0x83 ||
					pBuf[nOffsetType] == 0x84)
				{
#ifdef _DEBUG
					if (nOffset >= 730){
						int nTemp = 0;
						//Debug
					}
#endif
					switch (pBuf[nOffsetType])
					{
						case 0x81:
						{
							nType = nOffsetType + 1;
						}
							break;
						case 0x82:
						{
							nType = nOffsetType + 2;
						}
							break;
						case 0x83:
						{
							nType = nOffsetType + 3;
						}
							break;
						case 0x84:
						{
							nType = nOffsetType + 4;
						}
							break;
					}
					nType += 1;
					if (nType >= nBufSize){
						bRes = false;
						goto end;
					}
#ifdef _DEBUG
					if (nOffset >= 730){
						int nTemp = 0;
						//Debug
					}
#endif
					if (pBuf[nType] == 0x16){//IA5String , ASN的编码是0x16
						int nTypeLen = pBuf[nType + 1];	//图片格式类型不会长

						int nTempOffset = nType + 2 + nTypeLen;

						{
							//图片格式名解析
							char imgType[260];
							memset(imgType, 0, 260);
							memcpy(imgType, pBuf + nType + 2, nTypeLen);
							std::string strSubType = imgType;
							if (strSubType == "ofd"){
								*pImgSubType = OFD_SEAL_OFD;
							}
						}

						if (nTempOffset >= nBufSize){
							bRes = false;
							goto end;
						}

						if (pBuf[nTempOffset] == 0x04){//到这里，才确定数据是图片数据。
							int nPicDataLenOC = nTempOffset + 1;	//OCTET STRING ， ASN的编码是0x04
							if (nPicDataLenOC >= nBufSize){
								bRes = false;
								goto end;
							}
							int nPicLenOffset = 0;
							switch (pBuf[nPicDataLenOC])
							{
								case 0x81:
									nPicLenOffset = 1;
									break;
								case 0x82:
									nPicLenOffset = 2;
									break;
								case 0x83:
									nPicLenOffset = 3;
									break;
								case 0x84:
									nPicLenOffset = 4;
									break;
							}
							if (nPicDataLenOC + nPicLenOffset  + 1 > nBufSize){
								bRes = false;
								goto end;
							}
							//解析图片数据长度
							//这里 定性 图片的长度必然 大于 128， 属于长编码
							int nPicDataLen = 0;
							switch (nPicLenOffset)
							{
								case 1:
									nPicDataLen = pBuf[nPicDataLenOC + 1] & 0xff;
									break;
								case 2:
									nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 8) | (pBuf[nPicDataLenOC + 2] & 0xff);
									break;
								case 3:
									nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 16) | ((pBuf[nPicDataLenOC + 2] & 0xff) << 8) |
												  (pBuf[nPicDataLenOC + 3] & 0xff);
									break;
								case 4:
									nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 24) | ((pBuf[nPicDataLenOC + 2] & 0xff) << 16) |
												  ((pBuf[nPicDataLenOC + 3] & 0xff) << 8) | (pBuf[nPicDataLenOC + 4] & 0xff);
									break;
							}

							int nPicDataOffset = nPicDataLenOC + nPicLenOffset + 1;
							if ((nPicDataOffset + nPicDataLen) >= nBufSize){
								bRes = false;
								goto end;
							}
							nImageSize2Res = nPicDataLen;
							pImageDataBuf = new unsigned char[nPicDataLen];
							memset(pImageDataBuf, 0, nPicDataLen);
							memcpy(pImageDataBuf, (pBuf + nPicDataOffset), nPicDataLen);
							bRes = true;
							goto end;
						}
					}
				}
			}
			nOffset++;
		}

	}

end:

	return bRes;
}

CFX_DIBitmap * CreateVectorBitmap(IOFD_FileStream* pOFDFileStream,int width ,int height)
{
	if (!pOFDFileStream) return NULL;
	IOFD_FilePackage* pOFDPackage = OFD_FilePackage_Create(pOFDFileStream);
	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage);
	if (!pOFDParser)
	{
		pOFDPackage->Release();
		pOFDFileStream->Release();
		return NULL;
	}
	if (pOFDParser->CountDocuments() == 0)
	{
		pOFDParser->Release();
		pOFDPackage->Release();
		pOFDFileStream->Release();
		return NULL;
	}
	IOFD_Document* pDoc = pOFDParser->GetDocument(0);
	if (!pDoc){
		pOFDParser->Release();
		pOFDPackage->Release();
		pOFDFileStream->Release();
		return NULL;
	}

	IOFD_Page *pPage = pDoc->GetPage(0);
	pPage->LoadPage();
	CFX_AffineMatrix matrix;
	CFX_RectF pPageRect,physicalPageRect;
	FX_INT32 nRotate = (pPage->GetPageArea().GetRotate() / 90) % 4;
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_CropBox,pPageRect);
	pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox,physicalPageRect);
	CFX_Rect  deviceRect;

	CFX_AffineMatrix mtOffset;
	mtOffset.Translate(physicalPageRect.left - pPageRect.left, physicalPageRect.top - pPageRect.top);

	deviceRect.Set(0,0,width, height);
	OFD_GetPageMatrix(matrix, pPageRect, deviceRect, nRotate);
	matrix.Concat(mtOffset, TRUE);

	IOFD_RenderContext * pContentext = IOFD_RenderContext::Create();
	pContentext->AddPage(pPage,&matrix);

	CFX_DIBitmap *pDiBitMap = new CFX_DIBitmap;
	pDiBitMap->Create(FXSYS_round(width),FXSYS_round(height),FXDIB_Argb);
	pDiBitMap->Clear(0xFFFFFFFF);

	CFX_FxgeDevice * pFXgeDevice = new CFX_FxgeDevice;
	pFXgeDevice->Attach(pDiBitMap);

	COFD_RenderDevice  OFDRenderDevice;
	OFDRenderDevice.Create(pFXgeDevice);

	IOFD_ProgressiveRenderer * pProgressRender = IOFD_ProgressiveRenderer::Create();

	pProgressRender->StartRender(&OFDRenderDevice,pContentext,NULL,&matrix);

	pProgressRender->DoRender(NULL);
	pProgressRender->StopRender();
	delete pFXgeDevice;
	pContentext->Release();
	pProgressRender->Release();
	pPage->UnloadPage();
	pDoc->Release();
	pOFDParser->Release();
	pOFDPackage->Release();
	pOFDFileStream->Release();
	return  pDiBitMap;
}

CFX_DIBitmap*   RenderVetorBitmap(FX_LPBYTE pdata, FX_INT32 len,int width,int height)
{
	if (!pdata || len <= 0)
		return NULL;
// 	CRect rect;
// 	m_wndPic.GetClientRect(&rect);
// 	if (rect.IsRectEmpty())
// 		return NULL;

	IOFD_FileStream * pOFDFileStream = OFD_CreateMemoryStream(pdata,len);
	if (!pOFDFileStream)  return NULL;

	//return CreateVectorBitmap(pOFDFileStream,width,height/*rect.Height(),rect.Height()*/);//otherwise picture will stretch
	return CreateVectorBitmap(pOFDFileStream,245,245);//client height
}

CFX_DIBitmap* FS_LoadImage(IFX_FileRead* pFileRead)
{
	IFX_Image* pImage = FX_Image_Create();
	FX_HIMAGE hImage = pImage->LoadImage(pFileRead);
	if (!hImage) return NULL;
	pImage->LoadFrame(hImage, 0);

	//CFX_DIBitmap* pTemp = pImage->GetFrameImage(hImage);
	CFX_DIBitmap* pTemp = pImage->GetFrameImageWithTransparency(hImage);
	CFX_DIBitmap* pDIBitmap = pTemp->Clone();

	pImage->FreeImage(hImage);
	pImage->Release();

	return pDIBitmap;
}

CFX_DIBitmap* FS_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize)
{
	IFX_MemoryStream* pMemoryStream = FX_CreateMemoryStream((FX_LPBYTE)pBuf, nSize);
	CFX_DIBitmap* pDIBitmap = FS_LoadImage(pMemoryStream);
	pMemoryStream->Release();

	return pDIBitmap;
}

void FS_FormatSignBMP(CFX_DIBitmap* pBmp){
	pBmp->ConvertFormat(FXDIB_Argb);
	FX_INT32 pitch = pBmp->GetPitch();
	for (int Row = 0; Row < pBmp->GetHeight(); ++Row)
	{
		FX_LPBYTE pBuf = (pBmp->GetBuffer() + Row * pitch);
		for (FX_INT32 Column = 0;Column < pBmp->GetWidth();++Column)
		{
			if (pBuf[Column*4]==255 &&
				pBuf[Column*4+1]==255 &&
				pBuf[Column*4+2]==255)
			{
				pBuf[Column * 4 + 3] = 0;
			}
		}
	}
}

CFX_DIBitmap* FS_CreateSealBitmap(FX_LPBYTE pdata, FX_INT32 len)
{
	CFX_DIBitmap* pBitmap = FS_LoadImage(pdata, len);
	if (!pBitmap) return NULL;

	FS_FormatSignBMP(pBitmap);
	return pBitmap;
}

CFX_DIBitmap* GetSignedValueBitmap(const unsigned char *pSignBuf, int nLen)
{
	unsigned char *pPicBuf = NULL;
	int nPicLen = 0;
	CFX_DIBitmap *pStampBMP = NULL;
	int nImgSubType = -1;
	std::string strErr = "";

	{
		if (pSignBuf == NULL || nLen <= 0){
			strErr = "parameter error";
			goto end;
		}

		bool bParseImage = ParseSignedvalueImage(pSignBuf, nLen, pPicBuf, nPicLen, &nImgSubType);
		if (!bParseImage || (pPicBuf == NULL) || (nPicLen <= 0)){
			strErr = "parse failed";
			goto end;
		}

		//pStampBMP = CreateSealBitmap(pPicBuf, nPicLen);
		if(nImgSubType == OFD_SEAL_OFD)
		{
			pStampBMP = RenderVetorBitmap(pPicBuf, nPicLen, 245, 245);
		}
		else
		{
			pStampBMP = FS_CreateSealBitmap(pPicBuf, nPicLen);
		}

		if (pStampBMP == NULL)
			strErr = "create bitmap failed";
	}

	end:

	if (pPicBuf != NULL){
		delete []pPicBuf;
		pPicBuf = NULL;
	}
	return pStampBMP;
}

class COFD_DocProviderEx : public IOFD_DocProvider {
public:
//#ifdef _FXCORE_OLDVERSION_
//	IFX_ImageToPDF *m_pImageToPDF;
//#else
//	IFX_Image *m_pImageToPDF;
//#endif
	CFX_PtrArray m_pImageHandle;

//	COFD_DocProviderEx() : m_pImageToPDF(NULL), m_pImageHandle() {}
	COFD_DocProviderEx() : m_pImageHandle() {}

	~COFD_DocProviderEx() {
//		if (m_pImageToPDF) {
//			int a = m_pImageHandle.GetSize();
//			for (int i = 0; i < a; i++) {
//				m_pImageToPDF->FreeImage((FX_HIMAGE) m_pImageHandle[i]);
//			}
//			m_pImageToPDF->Release();
//			m_pImageToPDF = NULL;
//			m_pImageHandle.RemoveAll();
//		}

//        m_pRenderSeals.Add(pDIB);	// Old
//		m_pImageHandle.Add(pDIB);	// New
		int count = m_pImageHandle.GetSize();
		for (int i = 0; i < count; i ++)
		{
			CFX_DIBitmap* pDIB = (CFX_DIBitmap*)m_pImageHandle.GetAt(i);
			if (pDIB)
				delete pDIB;
		}
		m_pImageHandle.RemoveAll();
	}
	// xhj
	virtual CFX_DIBitmap *GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile, COFD_StampAnnot *pStampAnnot) {
		COFD_Signature* pSignature = pStampAnnot->GetSignature();
		if(pSignature == NULL)
			return NULL;

		CFX_BinaryBuf signValue;
		pSignature->GetSignatureValue(signValue);

		FX_LPBYTE pbBuf = signValue.GetBuffer();
		FX_STRSIZE size = signValue.GetSize();
		if(pbBuf == NULL || size <= 0)
			return NULL;

		CFX_DIBitmap* pDIB = GetSignedValueBitmap((const unsigned char *)pbBuf, size);

//        m_pRenderSeals.Add(pDIB);
		m_pImageHandle.Add(pDIB);
		return pDIB;
	}

	virtual CFX_DIBitmap *GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile, OFD_SEALBITMAPTYPE type,
										COFD_StampAnnot *pStampAnnot) {
		return NULL;
	}

	virtual IOFD_FileStream *GetSealFile(IOFD_Document *pDoc, IOFD_FileStream *pSealFile) {
		return NULL;
	}

	virtual void SetSignVersion(OFD_SIGNVERSION eVersion) {};

	virtual OFD_OESVERSION GetOesVersion(COFD_Signature *pSignature) { return OFD_OES_FIRST; }

	virtual OFD_SEALBITMAPTYPE GetSealBitmapType() { return OFD_SEALBITMAP_NORMAL; }

	virtual FX_BOOL IsOesLibExist() { return TRUE; }
};

// L"/storage/emulated/0/FoxitOFD/快速指南.pdf";
// L"/storage/emulated/0/FoxitOFD/save/快速指南.ofd";

// PDF to OFD
//bool PdfToOfdByName(const char* pChPdf, const char* pChOfd)
bool PdfToOfdByName(string strPdf , string strOfd)
{
    LogOut("PdfToOfdByName: %s, %s", strPdf.c_str(), strOfd.c_str());

	if (strPdf.length()<=0 || strOfd.length()<=0)
		return false;

	CFX_WideString wsPdfName = CFX_WideString::FromUTF8(strPdf.c_str());
	CFX_WideString wsOfdName = CFX_WideString::FromUTF8(strOfd.c_str());

	IFX_FileRead *pFile = FX_CreateFileRead(wsPdfName);
	if (!pFile)
		return false;

	CPDF_Parser parser;

//	if(pParam != NULL && strlen(pParam->_password) > 0)
//	{
//		parser.SetPassword(pParam->_password);
//	}
	FX_DWORD err = parser.StartParse(pFile);
	if(err != PDFPARSE_ERROR_SUCCESS)
		return false;

	CPDF_Document *pDoc = parser.GetDocument();
	if (!pDoc)
		return false;

	IFX_FileWrite* pOFDFile = FX_CreateFileWrite((FX_LPCWSTR)wsOfdName);

	IOFD_CreatorProvider* pCreatorProvider = OFD_CreatorProvider_Create(pOFDFile);
	IFX_PDFConverterProvider*  provider = FX_GetDefBorderProvider();
	IFX_PDFConverterEx* pPDFConverter = IFX_PDFConverterEx::Create(provider, pCreatorProvider);

	IFX_ConverterFactory* pFactory = PDF_ConvertOFDFactoryCreate();

	FX_INT32 ret = pPDFConverter->StartConvert(pDoc, pFactory);

	if(ret == -1)
	{
		pCreatorProvider->Release();

		pOFDFile->Release();

		FX_ReleaseDefBorderProvider(provider);

		pPDFConverter->Release();

		pFactory->Release();

		return false;
	}

	pPDFConverter->DoConvert(NULL);

	pCreatorProvider->Release();

	pOFDFile->Flush();

	pOFDFile->Release();

	FX_ReleaseDefBorderProvider(provider);

	pPDFConverter->Release();

	pFactory->Release();

    LogOut("PdfToOfdByName: Over");

	return true;
}

//bool PdfToOfdByData(string strIn, string & strOut)
//{
//	IFX_MemoryStream* pPdfFile = FX_CreateMemoryStream((FX_LPBYTE)strIn.c_str(), (size_t)strIn.length());
//	IFX_MemoryStream* pOfdFile = FX_CreateMemoryStream(TRUE);
//	IFX_MemoryStream* pOutput = pOfdFile;
//
//	if (!pPdfFile || !pOfdFile)
//		return false;
//
//	CPDF_Parser parser;
//
////	if(pParam != NULL && strlen(pParam->_password) > 0)
////	{
////		parser.SetPassword(pParam->_password);
////	}
//	FX_DWORD err = parser.StartParse(pPdfFile);
//	if(err != PDFPARSE_ERROR_SUCCESS)
//		return false;
//
//	CPDF_Document *pDoc = parser.GetDocument();
//	if (!pDoc)
//		return false;
//
//	IOFD_CreatorProvider* pCreatorProvider = OFD_CreatorProvider_Create(pOutput);
//	IFX_PDFConverterProvider*  provider = FX_GetDefBorderProvider();
//	IFX_PDFConverterEx* pPDFConverter = IFX_PDFConverterEx::Create(provider, pCreatorProvider);
//
//	IFX_ConverterFactory* pFactory = PDF_ConvertOFDFactoryCreate();
//
//	FX_INT32 ret = pPDFConverter->StartConvert(pDoc, pFactory);
//	if(ret == -1)
//	{
//		pCreatorProvider->Release();
//
//		FX_ReleaseDefBorderProvider(provider);
//
//		pPDFConverter->Release();
//
//		pFactory->Release();
//
//		return false;
//	}
//
//	pPDFConverter->DoConvert(NULL);
//
//	pCreatorProvider->Release();
//
//	FX_FILESIZE	nSize = pOutput->GetSize();
//	FX_LPBYTE pbBuf = pOutput->GetBuffer();
////	CFX_ByteString bsBuf(pbBuf, nSize);
//	strOut.assign((char*)pbBuf, nSize);
//
//	FX_ReleaseDefBorderProvider(provider);
//	pPDFConverter->Release();
//	pFactory->Release();
//
////	pPdfFile->Release();
////	pOfdFile->Release();
//
//	return true;
//}

bool PdfToOfdByData(string strIn, string & strOut)
{

//    IFX_MemoryStream* pPdfFile = FX_CreateMemoryStream((FX_LPBYTE)strIn.c_str(), (size_t)strIn.length());
	IFX_MemoryStream* pOfdMem = FX_CreateMemoryStream(TRUE);

//    if (!pPdfFile || !pOutput)
	if(strIn.empty() || strIn.length()<1 || !pOfdMem)
		return false;

	CPDF_Parser parser;
//	if(pParam != NULL && strlen(pParam->_password) > 0)
//	{
//		parser.SetPassword(pParam->_password);
//	}
//    FX_DWORD err = parser.StartParse(pPdfFile);
	FX_DWORD err = parser.StartParse((FX_LPCBYTE)strIn.c_str(), (FX_DWORD)strIn.length());
	if(err != PDFPARSE_ERROR_SUCCESS)
		return false;

	CPDF_Document *pDoc = parser.GetDocument();
	if (!pDoc)
		return false;

	IOFD_CreatorProvider* pCreatorProvider = OFD_CreatorProvider_Create(pOfdMem);
	IFX_PDFConverterProvider*  provider = FX_GetDefBorderProvider();
	IFX_PDFConverterEx* pPDFConverter = IFX_PDFConverterEx::Create(provider, pCreatorProvider);

	IFX_ConverterFactory* pFactory = PDF_ConvertOFDFactoryCreate();

	FX_INT32 ret = pPDFConverter->StartConvert(pDoc, pFactory);
	if(ret == -1)
	{
		pCreatorProvider->Release();
		FX_ReleaseDefBorderProvider(provider);
		pPDFConverter->Release();
		pFactory->Release();
		return false;
	}
	pPDFConverter->DoConvert(NULL);

	pCreatorProvider->Release();
	FX_ReleaseDefBorderProvider(provider);
	pPDFConverter->Release();
	pFactory->Release();

	FX_FILESIZE	nSize = pOfdMem->GetSize();
	FX_LPBYTE pbBuf = pOfdMem->GetBuffer();
	strOut.assign((char*)pbBuf, nSize);

	pOfdMem->Release();

	return true;
}

// OFD to PDF
bool OfdToPdfByName(string strOfd , string strPdf)
{
	LogOut("OfdToPdfByName: %s, %s", strOfd.c_str(),strPdf.c_str());

	CFX_WideString wsOfdName = CFX_WideString::FromUTF8(strOfd.c_str());
	CFX_WideString wsPdfName = CFX_WideString::FromUTF8(strPdf.c_str());

	IOFD_FilePackage* pOFDPackage = OFD_FilePackage_Create(wsOfdName);
	if (!pOFDPackage) {
		LogOut("OfdToPdfByName: pOFDPackage = NULL");
		return false;
	}

//	FR_DocProvider docPriv;
//	docPriv.Init();
//	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, &docPriv);
	COFD_DocProviderEx* docPriv = new COFD_DocProviderEx;
	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, docPriv);
//		if (!pOFDPackage) {
//			if (pOFDPackage) {
//				pOFDPackage->Release();
//				pOFDPackage = NULL;
//			}
//			return false;
//		}
	if (!pOFDParser) {
		LogOut("OfdToPdfByName: pOFDParser = NULL");
		return false;
	}

	IOFD_Document* pDoc = pOFDParser->GetDocument(0);

	CPDF_Document *pPDFDoc = FX_NEW CPDF_Document;
	pPDFDoc->CreateNewDoc();

	IOFD_ToPDFConverter* pPDFConverter = OFD_ToPDFConverter_Create();
	LogOut("OfdToPdfByName: StartConvert");
	FX_INT32 ret = pPDFConverter->StartConvert(pDoc, pPDFDoc);
	if(ret == -1)
	{
		LogOut("OfdToPdfByName: StartConvert error");
		return false;
	}
	LogOut("OfdToPdfByName: DoConvert");
	pPDFConverter->DoConvert(NULL);
	LogOut("OfdToPdfByName: Release");
	pPDFConverter->Release();

	LogOut("OfdToPdfByName: CPDF_Creator");
	CPDF_Creator saver(pPDFDoc);
//		saver.Create(pPDFFile);
	LogOut("OfdToPdfByName: CPDF_Creator::Create");
	saver.Create(wsPdfName);
	LogOut("OfdToPdfByName: Over");

	delete pPDFDoc;
	delete docPriv;
	if (pOFDParser) {
		pOFDParser->Release();
		pOFDParser = NULL;
	}
	if (pOFDPackage) {
		pOFDPackage->Release();
		pOFDPackage = NULL;
	}

	return true;
}

bool OfdToPdfByData(string strIn, string & strOut)
{
	IFX_MemoryStream* pOfdMem = FX_CreateMemoryStream((FX_LPBYTE)strIn.c_str(), (size_t)strIn.length());
	IOFD_FilePackage* pOFDPackage = OFD_FilePackage_Create(pOfdMem);

	IFX_MemoryStream* pPdfMem = FX_CreateMemoryStream(TRUE);

	if (!pOFDPackage) {
		return false;
	}

//	FR_DocProvider docPriv;
//	docPriv.Init();
//	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, &docPriv);
	COFD_DocProviderEx* docPriv = new COFD_DocProviderEx;
	IOFD_Parser* pOFDParser = OFD_Parser_Create(pOFDPackage, docPriv);
	if (!pOFDParser)
		return false;

	IOFD_Document* pDoc = pOFDParser->GetDocument(0);

	CPDF_Document *pPDFDoc = FX_NEW CPDF_Document;
	pPDFDoc->CreateNewDoc();

	IOFD_ToPDFConverter* pPDFConverter = OFD_ToPDFConverter_Create();
	pPDFConverter->StartConvert(pDoc, pPDFDoc);
	pPDFConverter->DoConvert(NULL);
	pPDFConverter->Release();

	CPDF_Creator saver(pPDFDoc);
//		saver.Create(pPDFFile);
//	saver.Create(wsPdfName);
	saver.Create(pPdfMem);

	FX_FILESIZE	nSize = pPdfMem->GetSize();
	FX_LPBYTE pbBuf = pPdfMem->GetBuffer();
	strOut.assign((char*)pbBuf, nSize);

	delete pPDFDoc;
	delete docPriv;
	if (pOFDParser) {
		pOFDParser->Release();
		pOFDParser = NULL;
	}
	if (pOFDPackage) {
		pOFDPackage->Release();
		pOFDPackage = NULL;
	}

	pOfdMem->Release();
	pPdfMem->Release();

	return true;
}
