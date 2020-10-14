#include "fr_include.h"
#include "../fs_util.h"
#include "fr_ofdpageImp.h"
#include "fr_ofdoutlineImp.h"
#include "fr_ofddocImp.h"
#include "fr_ofdsignatureImp.h"
#include "annotation/fr_ofdannotImp.h"
//***********************Class FR_OFDDocImp***********************
#define FROFD_LOADDOCTYPE_NEW            0
#define FROFD_LOADDOCTYPE_FROMPATH        1    //filenamepath
#define FROFD_LOADDOCTYPE_FROMBUFFER    2
#define FROFD_LOADDOCTYPE_FROMFILEREAD    3

FR_OFDDocImp::FR_OFDDocImp(FS_INT32 loadType)
        : m_pInitParam(NULL), m_pOFDFileRead(NULL), m_pOFDFilePackage(NULL), m_pOFDParser(NULL),
          m_pOFDDocument(NULL), m_isModified(FALSE), m_pOFDStandardSecurityHandler(NULL),
          m_pOFDStandardCryptoHandler(NULL), m_pMapPage(NULL)        //??watch
        , m_DocProvider(NULL), m_pOutlineRoot(NULL), m_pOFDWriteDocument(NULL), m_Signs(NULL) {
    m_uLoadDocType = loadType;
}

FR_OFDDocImp::~FR_OFDDocImp() {
    if (m_pOFDParser)
        m_pOFDParser->Release();
    if (m_pOFDFilePackage)
        m_pOFDFilePackage->Release();
    if (m_pOFDFileRead)
        m_pOFDFileRead->Release();
}

FR_OFDDocImp *
FR_OFDDocImp::creatImp(FS_INT32 loadType, void *pInitParam /*= NULL*/, FS_INT32 bufLen /*= 0*/,
                       FS_BOOL bReleaseBuffer) {
    FR_OFDDocImp *pDoc = new FR_OFDDocImp(loadType);
    if (!pDoc) return NULL;

    switch (loadType) {
        case FROFD_LOADDOCTYPE_NEW: {
            //pDoc->init();
        }
            break;
        case FROFD_LOADDOCTYPE_FROMPATH: {
            //pInitParam means path
            if (NULL == pInitParam || FXSYS_strlen((const char *) pInitParam) < 5) {
                delete pDoc;
                return NULL;
            }
            int len = FXSYS_strlen((const char *) pInitParam);
            CFX_ByteString bsTemp((const char *) pInitParam, len);
            if (FALSE == bsTemp.Right(4).EqualNoCase(".ofd")) {
                delete pDoc;
                return NULL;
            }
            pDoc->m_pInitParam = FX_Alloc(char, len + 1);
            if (pDoc->m_pInitParam) {
                FXSYS_memset(pDoc->m_pInitParam, 0, len + 1);
                FXSYS_memcpy(pDoc->m_pInitParam, (const char *) pInitParam, len);
            } else {
                delete pDoc;
                return NULL;
            }
        }
            break;
        case FROFD_LOADDOCTYPE_FROMBUFFER: {
            if (NULL == pInitParam || bufLen<1)
            {
                delete pDoc;
                return NULL;
            }
            pDoc->m_pInitParam = pInitParam;
            pDoc->m_bufflen = bufLen;
        }
            break;
        case FROFD_LOADDOCTYPE_FROMFILEREAD: {
            /*		if (NULL == pInitParam)
                    {
                        delete pDoc;
                        return NULL;
                    }
                    pDoc->m_pInitParam = pInitParam;
                    pDoc->m_bufflen = 0;*/
        }
            break;

    }
    return pDoc;
}

FR_OFDDocImp *FR_OFDDocImp::createFromFilePath(const char *path) {
    return creatImp(FROFD_LOADDOCTYPE_FROMPATH, (void *) path);
}

// DSS 20200901
FR_OFDDocImp *FR_OFDDocImp::createFromMemory(const void* buffer, FS_INT32 bufLen, FS_BOOL bReleaseBuffer, FS_BOOL bOfd) {
    return creatImp(FROFD_LOADDOCTYPE_FROMBUFFER, (void *) buffer, bufLen, bReleaseBuffer);
}

/****************virtual function begin****************/
void FR_OFDDocImp::release() {
    delete this;
}

FS_ERRORCODE FR_OFDDocImp::loadImp(const char password[], FS_INT32 passwordLen, FS_BOOL bReload) {
/*	if (m_uLoadDocType == FROFD_LOADDOCTYPE_NEW){
		if (m_pOFDDocument)
			return FSCommonDefines::e_errSuccess;	
		return FSCommonDefines::e_errUnknown;
	}

	if (FALSE == bReload){
		FS_BOOL bRet = init();
		if (!bRet) 
			return FSCommonDefines::e_errFile;
	}else
	{
		if (isWrapper()) bReload = FALSE;
	}*/

    //�����ĵ�
//    CFX_WideString wsPath = CFX_WideString::FromUTF8((char *) m_pInitParam);
//    m_pOFDFileRead = FX_CreateFileRead((FX_LPCWSTR) wsPath.GetBuffer(wsPath.GetLength()));
//    if (!m_pOFDFileRead) {
//        //could not be opened
//        return FSCommonDefines::e_errFile;
//    }
    switch (m_uLoadDocType) {
//        case FROFD_LOADDOCTYPE_NEW: {
//            if (m_pOFDDocument)
//                return FSCommonDefines::e_errSuccess;
//            return FSCommonDefines::e_errUnknown;
//        }
//            break;
        case FROFD_LOADDOCTYPE_FROMPATH: {
            CFX_WideString wsPath = CFX_WideString::FromUTF8((char *) m_pInitParam);
            m_pOFDFileRead = FX_CreateFileRead((FX_LPCWSTR) wsPath.GetBuffer(wsPath.GetLength()));
            if (!m_pOFDFileRead) {
                //could not be opened
                return FSCommonDefines::e_errFile;
            }
        }
            break;
        case FROFD_LOADDOCTYPE_FROMBUFFER: {
//            m_pOFDFileRead = FX_CreateFileRead((FX_LPCWSTR) wsPath.GetBuffer(wsPath.GetLength()));
            m_pOFDFileRead = (IFX_FileRead*)FX_CreateMemoryStream((FX_LPBYTE)m_pInitParam, (size_t)m_bufflen);
            if (!m_pOFDFileRead) {
                //could not be opened
                return FSCommonDefines::e_errFile;
            }
        }
            break;
        default: {
            return FSCommonDefines::e_errFile;
        }
            break;
    }

    m_pOFDFilePackage = OFD_FilePackage_Create(m_pOFDFileRead);
    if (!m_pOFDFilePackage) {
        //format is invalid. For files, this may also mean that file is corrupted
        return FSCommonDefines::e_errFormat;
    }
    m_DocProvider.attach(new FR_DocProvider());
    if (NULL == m_pOFDParser) {
        m_pOFDParser = OFD_Parser_Create(m_pOFDFilePackage, m_DocProvider.get());
        if (!m_pOFDParser) {
            return FSCommonDefines::e_errUnknown;
        }
    }
    int count = m_pOFDParser->CountDocuments();
    if (count <= 0) {
        return FSCommonDefines::e_errFormat;
    }

    //int crypt_code = CheckCrypt();
    COFD_CryptoDictionary cDict;
    m_pOFDParser->GetCryptoDict(0, cDict);
    CFX_ByteString bsCrypto = cDict.GetCryptoType();
    CFX_ByteString bsSecurity = cDict.GetSecurityType();
    if (bsCrypto.EqualNoCase("Standard")) {
        return FSCommonDefines::e_errPassword;
    } else if (bsCrypto.EqualNoCase("SM4")) {
        return FSCommonDefines::e_errPassword;
    }
    m_pOFDDocument = m_pOFDParser->GetDocument(0);
    if (!m_pOFDDocument)
        return FSCommonDefines::e_errFormat;
    return FSCommonDefines::e_errSuccess;
    //0 �ɹ���
}

FS_ERRORCODE FR_OFDDocImp::load(const char password[], FS_INT32 passwordLen) {
    return loadImp(password, passwordLen, FALSE);
}

FS_BOOL FR_OFDDocImp::isEncrypted() const {
    if (!m_pOFDParser) return FALSE;

    return FALSE;//m_pOFDParser->IsEncrypted();
}

FS_BOOL FR_OFDDocImp::isModified() const {
    if (!m_pOFDDocument) return FALSE;
    FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock *) &m_lockModified);
    return m_isModified;
}

FS_PASSWORDTYPE FR_OFDDocImp::getPasswordType() const {
    if (FROFD_LOADDOCTYPE_NEW == m_uLoadDocType) return e_pwdNoPassword;
    if (!m_pOFDParser) return e_pwdInvalid;

    FS_ENCRYPTTYPE encryptType = getEncryptionType();
    if (e_encryptPassword != encryptType)
        return e_pwdNoPassword;
    return e_pwdNoPassword;
}

FS_ENCRYPTTYPE FR_OFDDocImp::getEncryptionType() const {
    if (!m_pOFDParser) return e_encryptNone;
    FS_ENCRYPTTYPE encryptType = e_encryptNone;
    COFD_CryptoDictionary cDict;
    m_pOFDParser->GetCryptoDict(0, cDict);
    CFX_ByteString bsCrypto = cDict.GetCryptoType();
    if (bsCrypto.IsEmpty()) {
        encryptType = e_encryptNone;
    } else if (bsCrypto.EqualNoCase("Standard")) {
        encryptType = e_encryptPassword;
    } else if (bsCrypto.EqualNoCase("SM4")) {
        encryptType = e_encryptPassword;    //not prejudge by wq 20170303
    } else {
        encryptType = e_encryptCustom;
    }
    return encryptType;
}

FS_PASSWORDTYPE FR_OFDDocImp::checkPassword(const char password[], FS_INT32 passwordLen) {
    return e_pwdInvalid;
}

FS_BOOL FR_OFDDocImp::saveAs(const char *filePath, FS_DWORD saveFlags) {
    //CopyFile(tmpFilePath.c_str(), m_bsFilePath.c_str());
    //IFX_FileStream* pSaveFileStream = FSCRT_Util_CreateFileFromPath(filePath, FX_FILEMODE_Truncate);
    AutoReleasePtr<IOFD_Creator> creator(OFD_Creator_Create());
    creator->InitParser(this->m_pOFDParser);

#if defined(WIN32) || defined(_WIN32)
    // convert utf8 to unicode in windows
    CFX_WideString ws = CFX_WideString::FromUTF8(filePath);
    AutoReleasePtr<IFX_FileWrite> fileWrite(FX_CreateFileWrite((FX_LPCWSTR)ws));
#else
    AutoReleasePtr<IFX_FileWrite> fileWrite(FX_CreateFileWrite(filePath));
#endif
    if (NULL == fileWrite.get()) {
        return FALSE;
    }
#if (_FX_OS_ == _FX_ANDROID_) || (_FX_OS_ == _FX_IOS_)
    CFX_ByteString bsTemppath(filePath);
    bsTemppath += ".linearize";
    creator->SetTempFilePath(bsTemppath);
#endif
    creator->StartPackage(fileWrite);
    creator->ContinuePackage(NULL);
    creator->EndPackage();
    return TRUE;
}

// DSS 20200901
string FR_OFDDocImp::saveAsStream(FS_DWORD saveFlags) {
    string strRet;

    IFX_MemoryStream* pOfdFile = FX_CreateMemoryStream(TRUE);
    if (!pOfdFile)
        return strRet;

    AutoReleasePtr<IOFD_Creator> creator(OFD_Creator_Create());
    creator->InitParser(this->m_pOFDParser);

    creator->StartPackage(pOfdFile);
    creator->ContinuePackage(NULL);
    creator->EndPackage();

    FX_FILESIZE	nSize = pOfdFile->GetSize();
    FX_LPBYTE pbBuf = pOfdFile->GetBuffer();
    strRet.assign((char*)pbBuf, nSize);

    pOfdFile->Release();

    return strRet;
}

FSBookmark *FR_OFDDocImp::getFirstBookmark() {
    if (m_pOutlineRoot) {
        return m_pOutlineRoot;
    }

    if (!m_pOFDDocument) {
        return NULL;
    }

    COFD_Outline root = m_pOFDDocument->GetOutline();
    if (root.IsNull()) {
        return NULL;
    }

    m_pOutlineRoot = new FR_OFDOutlineImp;
    m_pOutlineRoot->init(this, NULL, 0, root.Clone());

    return m_pOutlineRoot;
}

FSBookmark *FR_OFDDocImp::createFirstBookmark() {
    return (FSBookmark *) NULL;
}

FS_BOOL FR_OFDDocImp::removeBookmark(FSBookmark *bookmark) {
    return FALSE;
}

FS_INT32 FR_OFDDocImp::getPageCount() {
    if (!m_pOFDDocument) return 0;
    return m_pOFDDocument->CountPages();//m_pDoc->GetPageCount();
}

FSPDFPage *FR_OFDDocImp::getPageImp(FS_INT32 pageIndex, FS_BOOL bMapRetain) {
    if (!m_pOFDDocument || pageIndex < 0) return NULL;

    FSCRT_LOCKOBJECT(lock, &m_lockPages);

    FR_OFDPageImp *pOFDPageImp = NULL;
    if (!m_pMapPage)
        m_pMapPage = FX_NEW CFX_MapPtrToPtr();
    if (!m_pMapPage) return NULL;

    FX_BOOL bRet = m_pMapPage->Lookup((void *) pageIndex, (void *&) pOFDPageImp);
    if (bRet && pOFDPageImp) {
        if (bMapRetain)
            return (FSPDFPage *) pOFDPageImp->retain();
        return (FSPDFPage *) pOFDPageImp;
    }

    pOFDPageImp = FX_NEW FR_OFDPageImp();
    if (!pOFDPageImp)
        return NULL;
    if (!pOFDPageImp->init(this, pageIndex)) {
        delete pOFDPageImp;
        //return (FSPDFPage*)FSCommonDefines::e_errFormat;
        return NULL;
    }
    m_pMapPage->SetAt((void *) pageIndex, pOFDPageImp);
    return (FSPDFPage *) pOFDPageImp;
}

FSPDFPage *FR_OFDDocImp::getPage(FS_INT32 pageIndex) {
    return getPageImp(pageIndex, TRUE);;
}

FS_BOOL FR_OFDDocImp::closePage(FS_INT32 pageIndex) {
    FSCRT_LOCKOBJECT(lock, &m_lockPages);
    if (!m_pMapPage || pageIndex < 0 || pageIndex >= getPageCount())
        return FALSE;
    FSPDFPage *pFspdfPage = NULL;
    FX_BOOL bRet = m_pMapPage->Lookup((void *) pageIndex, (void *&) pFspdfPage);
    if (bRet && pFspdfPage && ((FR_OFDPageImp *) pFspdfPage)->release()) {
        m_pMapPage->RemoveKey((void *) pageIndex);
    }
    return TRUE;
}

FS_BOOL FR_OFDDocImp::closePage(FSPDFPage *pPage) {
    if (!pPage || pPage->getDocument() != this
        || !m_pMapPage)
        return FALSE;
    return closePage(pPage->getIndex());
}

FS_DISPLAYMODE FR_OFDDocImp::getDisplayMode() const {
    return FSCommonDefines::e_displayUseNone;
}

FSPDFDictionary *FR_OFDDocImp::getCatalog() const {
    return NULL;
}

FSPDFDictionary *FR_OFDDocImp::getTrailer() const {
    return NULL;
}

FSPDFDictionary *FR_OFDDocImp::getInfo() const {
    return NULL;
}

FSPDFDictionary *FR_OFDDocImp::getEncryptDict() const {
    return NULL;
}

FSPDFObject *FR_OFDDocImp::getIndirectObject(FS_DWORD objnum) {
    return NULL;
}

FS_DWORD FR_OFDDocImp::addIndirectObject(FSPDFObject *pObj) {
    return NULL;
}

void FR_OFDDocImp::deleteIndirectObject(FS_DWORD objnum) {
}

FS_DWORD FR_OFDDocImp::getUserPermissions() const {
    if (!m_pOFDDocument) return 0;

    const int PDF_PERMISSION_PRINT = 0x00000004;
    const int PDF_PERMISSION_MODIFY = 0x00000008;
    const int PDF_PERMISSION_EXTRACT = 0x00000010;
    const int PDF_PERMISSION_ANNOT_FORM = 0x00000020;
    const int PDF_PERMISSION_FILL_FORM = 0x00000100;
    const int PDF_PERMISSION_EXTRACT_ACCESS = 0x00000200;
    const int PDF_PERMISSION_ASSEMBLE = 0x00000400;
    const int PDF_PERMISSION_PRINT_HIGH = 0x00000800;
    const int PDF_PERMISSION_PRINT_LOW = 0x00001000;
    FS_DWORD result = 0;
    bool isAdmin = false;
    COFD_Permissions *permission = m_pOFDDocument->GetPermissions();
    if ((isAdmin) || (permission && permission->IsEdit())) {
        result |= PDF_PERMISSION_MODIFY;
    }
    if ((isAdmin) || (permission && permission->IsAnnot())) {
        result |= PDF_PERMISSION_ANNOT_FORM;
    }
    if ((isAdmin) || (permission && permission->IsExport())) {
        result |= PDF_PERMISSION_EXTRACT;
        result |= PDF_PERMISSION_EXTRACT_ACCESS;
    }
    if ((isAdmin) || (permission && permission->IsPrintable())) {
        result |= PDF_PERMISSION_PRINT;
        result |= PDF_PERMISSION_PRINT_HIGH;
        result |= PDF_PERMISSION_PRINT_LOW;
    }
    result |= PDF_PERMISSION_EXTRACT;
    result |= PDF_PERMISSION_ANNOT_FORM;
    return result;
}

FS_BOOL FR_OFDDocImp::isWrapper() const {
    return FALSE;
}

FSWrapperData FR_OFDDocImp::getWrapperData() const {
    FSWrapperData fsWrapperData;
    return fsWrapperData;
}

FS_INT32 FR_OFDDocImp::getWrapperOffset() const {
    return 0;
}

FSDateTime FR_OFDDocImp::getMetadataTime(CFX_WideStringC wstrTimeKey) const {
    FSDateTime fsDateTime;
    fsDateTime.set(0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (!m_pOFDDocument) return fsDateTime;
    IOFD_DocInfo *OFDDocInfo = m_pOFDDocument->GetDocInfo();
//	if(OFDDocInfo){
    //CFX_WideString wsKey = CFX_WideString::FromUTF8(key);
    fxutil::CFX_Unitime time;
    FX_INT32 ret = OFDDocInfo->GetDate(wstrTimeKey, time);
    //if (ret < 0) return fsDateTime;

    fsDateTime.set(time.GetYear(), time.GetMonth(), time.GetDay(),
                   time.GetHour(), time.GetMinute(), time.GetSecond(), time.GetMillisecond(),
                   0, 0);
    return fsDateTime;
}

FS_BOOL FR_OFDDocImp::hasMetadataKey(const char *key) const {
    if (!m_pOFDDocument || FSCRT_Util_IsStringEmpty(key)) return FALSE;

    IOFD_DocInfo *ofdDocInfo = m_pOFDDocument->GetDocInfo();
    FX_INT32 iRet = 0;
    if (ofdDocInfo) {
        CFX_WideString wsKey = CFX_WideString::FromUTF8(key);
        CFX_WideString wsValue;
        if (wsKey == L"Keywords") {
            iRet = ofdDocInfo->CountKeyWords();
        } else {
            iRet = ofdDocInfo->GetString(wsKey, wsValue);
        }
        return iRet > 0;
    }
    return FALSE;
}

FSDateTime FR_OFDDocImp::getCreationDateTime() const {
    return getMetadataTime(FX_WSTRC(L"CreationDate"));
}

FSDateTime FR_OFDDocImp::getModifiedDateTime() const {
    return getMetadataTime(FX_WSTRC(L"ModDate"));
}

FSString FR_OFDDocImp::getMetadataValue(const char *key) const {
    if (!m_pOFDDocument || FSCRT_Util_IsStringEmpty(key)) return FALSE;

    IOFD_DocInfo *ofdDocInfo = m_pOFDDocument->GetDocInfo();
    if (ofdDocInfo) {
        CFX_WideString wsKey = CFX_WideString::FromUTF8(key);
        CFX_WideString wsValue;
        if (wsKey == L"Keywords") {
            int count = ofdDocInfo->CountKeyWords();
            for (int i = 0; i < count; ++i) {
                CFX_WideString wsTmp;
                ofdDocInfo->GetKeyWord(i, wsTmp);
                if (wsTmp.IsEmpty()) continue;

                if (wsValue.IsEmpty()) {
                    wsValue = wsTmp;
                } else {
                    wsValue += L" ";
                    wsValue += wsTmp;
                }
            }
        } else {
            ofdDocInfo->GetString(wsKey, wsValue);
        }

        if (!wsValue.IsEmpty()) {
            return FSString(wsValue.UTF8Encode());
        }
    }
    return FSString();
}

FS_INT32 FR_OFDDocImp::getPageLabelRangeCount() const {
    return 0;
}

FSPageLabel FR_OFDDocImp::getPageLabelInfo(FS_INT32 index) const {
    FSPageLabel fsLabelInfo;
    return fsLabelInfo;
}

FS_BOOL FR_OFDDocImp::hasForm() const {
    return FALSE;
}

FSForm *FR_OFDDocImp::getForm() {
/*	if (!m_pOFDDocument)
		return NULL;
	if(m_pForm)
		return m_pForm;*/
    return NULL;
//	IOFD_Dictionary* rootDict = m_pDoc->GetRoot();
}

FS_INT32 FR_OFDDocImp::getReadingBookmarkCount() {
    return 0;
}

FSReadingBookmark *FR_OFDDocImp::getReadingBookmark(FS_INT32 index) {
    return NULL;
}

FSReadingBookmark *
FR_OFDDocImp::insertReadingBookmark(FS_INT32 readingBookmarkIndex, const char *title,
                                    FS_INT32 destPageIndex) {
    return NULL;
}

FS_BOOL FR_OFDDocImp::removeReadingBookmark(FSReadingBookmark *readingBookmark) {
    return TRUE;
}

FS_INT32 FR_OFDDocImp::getSignatureCount() {
    if (NULL == m_Signs && NULL != m_pOFDDocument) {
        m_Signs = m_pOFDDocument->GetSignatures();
    }
    if (m_Signs) {
        return m_Signs->CountSignature();
    }
    return 0;
}

FSSignature *FR_OFDDocImp::getSignature(FS_INT32 index) {
    if (index < 0 || index >= getSignatureCount() || NULL == m_Signs) {
        return NULL;
    }

    FR_OFDSignatureImp *signatureImp = new FR_OFDSignatureImp();
    //pSign = signatureImp->GetSignature(signs,index);
    signatureImp->init(m_Signs, index);
    //LogOut("Test_C--->signature_2:%p",signatureImp);
    return signatureImp;
}

//class COFD_SG_Function
CFX_WideString COFD_SG_Function::GetNoExitFilePath(CFX_WideString &oldFilePath) {
    CFX_WideString resFilePath = oldFilePath;
    while (FX_File_Exist(resFilePath)) {
        resFilePath.Insert(resFilePath.GetLength(), '$');
    }
    return resFilePath;
}

void COFD_SG_Function::JumpSealID(FX_LPBYTE pSealData, FX_INT32 pSealDataLen, FX_INT32 _startpos,
                                  FX_LPBYTE _outPutData, FX_INT32 &_inoutPutLen,
                                  FX_INT32 &_pSealDataOffset /*in out*/) {
    if (!pSealData || pSealDataLen <= 0 || _startpos < 0 || _startpos >= pSealDataLen)
        return;

    FX_INT32 _Offset = 0;
    FX_INT32 _DataLenFlags = 0;
    FX_BYTE _Datatype = pSealData[_startpos];
    if (!_Datatype)
        return;
    if (!pSealData[_startpos + 1])
        return;

    switch (pSealData[_startpos + 1]) {
        case 0x81:
            _inoutPutLen = pSealData[_startpos + 2] & 0xff;
            _DataLenFlags = 2;
            break;
        case 0x82:
            _inoutPutLen = (pSealData[_startpos + 3] & 0xff);
            _inoutPutLen += ((pSealData[_startpos + 2] & 0xff) << 8);
            _DataLenFlags = 3;
            break;
        case 0x83:
            _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 16;
            _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 8;
            _inoutPutLen += (pSealData[_startpos + 4] & 0xff);
            _DataLenFlags = 4;
            break;
        case 0x84:
            _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 24;
            _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 16;
            _inoutPutLen += (pSealData[_startpos + 4] & 0xff) << 8;
            _inoutPutLen += (pSealData[_startpos + 5] & 0xff);
            _DataLenFlags = 5;
            break;
        default:
            _inoutPutLen = (pSealData[_startpos + 1] & 0xff);
            _DataLenFlags = 1;
    }

    _Offset = _DataLenFlags + 1;
    _pSealDataOffset = _Offset + _startpos;

    if (_Datatype == 0x16) {
        _startpos += _Offset;
        _startpos += _inoutPutLen;

        if (!pSealData[_startpos + 1]) {
            return;
        }
        switch (pSealData[_startpos + 1]) {
            case 0x81:
                _inoutPutLen = pSealData[_startpos + 2] & 0xff;
                _DataLenFlags = 2;
                break;
            case 0x82:
                _inoutPutLen = (pSealData[_startpos + 3] & 0xff);
                _inoutPutLen += ((pSealData[_startpos + 2] & 0xff) << 8);
                _DataLenFlags = 3;
                break;
            case 0x83:
                _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 16;
                _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 8;
                _inoutPutLen += (pSealData[_startpos + 4] & 0xff);
                _DataLenFlags = 4;
                break;
            case 0x84:
                _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 24;
                _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 16;
                _inoutPutLen += (pSealData[_startpos + 4] & 0xff) << 8;
                _inoutPutLen += (pSealData[_startpos + 5] & 0xff);
                _DataLenFlags = 5;
                break;
            default:
                _inoutPutLen = (pSealData[_startpos + 1] & 0xff);
                _DataLenFlags = 1;
        }
        _Offset = 0;
        _Offset = _DataLenFlags + 1;
        _pSealDataOffset = _startpos + _Offset;
        if (pSealData[_pSealDataOffset + _inoutPutLen] == 0x30) {
            return;
        } else {
            JumpSealID(pSealData, pSealDataLen, _startpos + _Offset + _inoutPutLen, _outPutData,
                       _inoutPutLen, _pSealDataOffset);
        }
    } else {
        if (_Datatype == 0x30) {
            JumpSealID(pSealData, pSealDataLen, _startpos + _Offset, _outPutData, _inoutPutLen,
                       _pSealDataOffset);
        } else {
            JumpSealID(pSealData, pSealDataLen, _startpos + _Offset + _inoutPutLen, _outPutData,
                       _inoutPutLen, _pSealDataOffset);
        }
    }
}

void COFD_SG_Function::JumpPtr(FX_LPBYTE pSealData, FX_INT32 pSealDataLen, FX_INT32 _startpos,
                               FX_LPBYTE _outPutData, FX_INT32 &_inoutPutLen,
                               FX_INT32 &_pSealDataOffset /*in out*/) {
    if (!pSealData || pSealDataLen <= 0 || _startpos < 0 || _startpos >= pSealDataLen)
        return;

    FX_INT32 _Offset = 0;
    FX_INT32 _DataLenFlags = 0;
    FX_BYTE _Datatype = pSealData[_startpos];
    if (!_Datatype)
        return;
    if (!pSealData[_startpos + 1])
        return;

    switch (pSealData[_startpos + 1]) {
        case 0x81:
            _inoutPutLen = pSealData[_startpos + 2] & 0xff;
            _DataLenFlags = 2;
            break;
        case 0x82:
            _inoutPutLen = (pSealData[_startpos + 3] & 0xff);
            _inoutPutLen += ((pSealData[_startpos + 2] & 0xff) << 8);
            _DataLenFlags = 3;
            break;
        case 0x83:
            _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 16;
            _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 8;
            _inoutPutLen += (pSealData[_startpos + 4] & 0xff);
            _DataLenFlags = 4;
            break;
        case 0x84:
            _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 24;
            _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 16;
            _inoutPutLen += (pSealData[_startpos + 4] & 0xff) << 8;
            _inoutPutLen += (pSealData[_startpos + 5] & 0xff);
            _DataLenFlags = 5;
            break;
        default:
            _inoutPutLen = (pSealData[_startpos + 1] & 0xff);
            _DataLenFlags = 1;
    }

    _Offset = _DataLenFlags + 1;
    _pSealDataOffset = _Offset + _startpos;

    if ((_Datatype == 0x16) && (_inoutPutLen == 3)) {
        FX_BYTE _Picdataname[3] = {0};  //modified for efficiency
        _Picdataname[0] = pSealData[_startpos + _Offset];
        _Picdataname[1] = pSealData[_startpos + _Offset + 1];
        _Picdataname[2] = pSealData[_startpos + _Offset + 2];
        CFX_ByteString _picDataName(_Picdataname, _inoutPutLen);
        if ((_picDataName.Compare("bmp") == 0) || (_picDataName.Compare("BMP") == 0) ||
            (_picDataName.Compare("gif") == 0) || (_picDataName.Compare("GIF") == 0) ||
            (_picDataName.Compare("png") == 0) || (_picDataName.Compare("PNG") == 0) ||
            (_picDataName.Compare("jpg") == 0) || (_picDataName.Compare("JPG") == 0)) {
            _startpos += _Offset;
            _startpos += _inoutPutLen;

            if (!pSealData[_startpos + 1]) {
                return;
            }
            switch (pSealData[_startpos + 1]) {
                case 0x81:
                    _inoutPutLen = pSealData[_startpos + 2] & 0xff;
                    _DataLenFlags = 2;
                    break;
                case 0x82:
                    _inoutPutLen = (pSealData[_startpos + 3] & 0xff);
                    _inoutPutLen += ((pSealData[_startpos + 2] & 0xff) << 8);
                    _DataLenFlags = 3;
                    break;
                case 0x83:
                    _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 16;
                    _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 8;
                    _inoutPutLen += (pSealData[_startpos + 4] & 0xff);
                    _DataLenFlags = 4;
                    break;
                case 0x84:
                    _inoutPutLen = (pSealData[_startpos + 2] & 0xff) << 24;
                    _inoutPutLen += (pSealData[_startpos + 3] & 0xff) << 16;
                    _inoutPutLen += (pSealData[_startpos + 4] & 0xff) << 8;
                    _inoutPutLen += (pSealData[_startpos + 5] & 0xff);
                    _DataLenFlags = 5;
                    break;
                default:
                    _inoutPutLen = (pSealData[_startpos + 1] & 0xff);
                    _DataLenFlags = 1;
            }
            _Offset = 0;
            _Offset = _DataLenFlags + 1;
            _pSealDataOffset = _startpos + _Offset;
            return;
        } else {
            JumpPtr(pSealData, pSealDataLen, _startpos + _Offset + _inoutPutLen, _outPutData,
                    _inoutPutLen, _pSealDataOffset);
        }
    } else {
        if (_Datatype == 0x30) {
            JumpPtr(pSealData, pSealDataLen, _startpos + _Offset, _outPutData, _inoutPutLen,
                    _pSealDataOffset);
        } else {
            JumpPtr(pSealData, pSealDataLen, _startpos + _Offset + _inoutPutLen, _outPutData,
                    _inoutPutLen, _pSealDataOffset);
        }
    }
}

void COFD_SG_Function::ParseSealData(FX_LPBYTE psealData/*in*/ , FX_INT32 psealDataLen /*in*/,
                                     FX_INT32 _startpos /*in*/, FX_INT32 &_inoutPutLen/*in out*/,
                                     FX_INT32 &psealDataOffset/*in out*/,
                                     FX_LPBYTE pSealType/*out*/, FX_INT32 nCount) {
    //psealData is begin after seal version,in asn view is the third red point
    if (!psealData || psealDataLen <= 0 || _startpos < 0 || _startpos >= psealDataLen)
        return;
    FX_INT32 _Offset = 0;
    FX_INT32 _DataLenFlags = 0;
    FX_BYTE _Datatype = psealData[_startpos];
    if (!_Datatype)
        return;
    if (!psealData[_startpos + 1])
        return;

    //Parse Picture
    if (5 == nCount && _Datatype ==
                       0x30) //never use picture type to parse  picture ,instead ,using the convention of seal data
    {
        switch (psealData[_startpos + 1]) {
            case 0x81:
                _inoutPutLen = psealData[_startpos + 2] & 0xff;
                _DataLenFlags = 2;
                break;
            case 0x82:
                _inoutPutLen = (psealData[_startpos + 3] & 0xff);
                _inoutPutLen += ((psealData[_startpos + 2] & 0xff) << 8);
                _DataLenFlags = 3;
                break;
            case 0x83:
                _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 16;
                _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 8;
                _inoutPutLen += (psealData[_startpos + 4] & 0xff);
                _DataLenFlags = 4;
                break;
            case 0x84:
                _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 24;
                _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 16;
                _inoutPutLen += (psealData[_startpos + 4] & 0xff) << 8;
                _inoutPutLen += (psealData[_startpos + 5] & 0xff);
                _DataLenFlags = 5;
                break;
            default:
                _inoutPutLen = (psealData[_startpos + 1] & 0xff);
                _DataLenFlags = 1;

        }
        _Offset = _DataLenFlags + 1; // the start position included
        psealDataOffset = _Offset + _startpos; //the length from 0
        _startpos = psealDataOffset;

        if (psealData[psealDataOffset] == 0x16) // picture type descriptor
        {
            switch (psealData[_startpos + 1]) {
                case 0x81:
                    _inoutPutLen = psealData[_startpos + 2] & 0xff;
                    _DataLenFlags = 2;
                    break;
                case 0x82:
                    _inoutPutLen = (psealData[_startpos + 3] & 0xff);
                    _inoutPutLen += ((psealData[_startpos + 2] & 0xff) << 8);
                    _DataLenFlags = 3;
                    break;
                case 0x83:
                    _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 16;
                    _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 8;
                    _inoutPutLen += (psealData[_startpos + 4] & 0xff);
                    _DataLenFlags = 4;
                    break;
                case 0x84:
                    _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 24;
                    _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 16;
                    _inoutPutLen += (psealData[_startpos + 4] & 0xff) << 8;
                    _inoutPutLen += (psealData[_startpos + 5] & 0xff);
                    _DataLenFlags = 5;
                    break;
                default:
                    _inoutPutLen = (psealData[_startpos + 1] & 0xff);
                    _DataLenFlags = 1;
            }

            _Offset = 0;
            _Offset = _DataLenFlags + 1;
            //attention : this type memory is NULL default
            if (pSealType && _inoutPutLen <= 8) {
                unsigned char sealtype[8] = {0};
                for (int i = 0; i < _inoutPutLen; ++i) {
                    sealtype[i] = psealData[_startpos + _Offset + i];
                }

                memcpy(pSealType, sealtype, _inoutPutLen);
            }
            psealDataOffset =
                    _startpos + _Offset + _inoutPutLen; //the end  of picture type descriptor
            _startpos = psealDataOffset;
        }
        if (psealData[_startpos] == 0x04) //parse picture data
        {
            switch (psealData[_startpos + 1]) {
                case 0x81:
                    _inoutPutLen = psealData[_startpos + 2] & 0xff;
                    _DataLenFlags = 2;
                    break;
                case 0x82:
                    _inoutPutLen = (psealData[_startpos + 3] & 0xff);
                    _inoutPutLen += ((psealData[_startpos + 2] & 0xff) << 8);
                    _DataLenFlags = 3;
                    break;
                case 0x83:
                    _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 16;
                    _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 8;
                    _inoutPutLen += (psealData[_startpos + 4] & 0xff);
                    _DataLenFlags = 4;
                    break;
                case 0x84:
                    _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 24;
                    _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 16;
                    _inoutPutLen += (psealData[_startpos + 4] & 0xff) << 8;
                    _inoutPutLen += (psealData[_startpos + 5] & 0xff);
                    _DataLenFlags = 5;
                    break;
                default:
                    _inoutPutLen = (psealData[_startpos + 1] & 0xff);
                    _DataLenFlags = 1;
            }

            _Offset = 0;
            _Offset = _DataLenFlags + 1;
            psealDataOffset = _startpos + _Offset; //the beginning  of picture data descriptor
        }
        return;
    } else {
        switch (psealData[_startpos + 1]) {
            case 0x81:
                _inoutPutLen = psealData[_startpos + 2] & 0xff;
                _DataLenFlags = 2;
                break;
            case 0x82:
                _inoutPutLen = (psealData[_startpos + 3] & 0xff);
                _inoutPutLen += ((psealData[_startpos + 2] & 0xff) << 8);
                _DataLenFlags = 3;
                break;
            case 0x83:
                _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 16;
                _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 8;
                _inoutPutLen += (psealData[_startpos + 4] & 0xff);
                _DataLenFlags = 4;
                break;
            case 0x84:
                _inoutPutLen = (psealData[_startpos + 2] & 0xff) << 24;
                _inoutPutLen += (psealData[_startpos + 3] & 0xff) << 16;
                _inoutPutLen += (psealData[_startpos + 4] & 0xff) << 8;
                _inoutPutLen += (psealData[_startpos + 5] & 0xff);
                _DataLenFlags = 5;
                break;
            default:
                _inoutPutLen = (psealData[_startpos + 1] & 0xff);
                _DataLenFlags = 1;

        }
        _Offset = _DataLenFlags + 1; // the start position included
        psealDataOffset = _Offset + _startpos;
        if (_Datatype == 0x30)
            ParseSealData(psealData, psealDataLen, _startpos + _Offset, _inoutPutLen,
                          psealDataOffset, pSealType, nCount + 1);
        else
            ParseSealData(psealData, psealDataLen, _startpos + _Offset + _inoutPutLen, _inoutPutLen,
                          psealDataOffset, pSealType, nCount);
    }
}

bool COFD_SG_Function::Ber_Decode_Sealdata(const unsigned char *buffer, const int &buf_size,
                                           FX_LPBYTE sealPic, FX_INT32 &sealPicLen,
                                           FX_LPBYTE sealType) {
    FX_INT32 picDataOffset = 0;
    ParseSealData((FX_LPBYTE) buffer, buf_size, 0, sealPicLen, picDataOffset, sealType, 0);
    if (sealPic) {
        buffer += picDataOffset;
        memcpy(FX_LPVOID(sealPic), FX_LPVOID(buffer), sealPicLen);
    }
    return true;
}

bool
COFD_SG_Function::Ber_Decode_PicSize(const unsigned char *buffer, const int &buf_size, long &width,
                                     long &height) {
    int picDataLen = 0;
    FX_INT32 picDataOffset = 0;
    width = 0;
    height = 0;
    ParseSealData((FX_LPBYTE) buffer, buf_size, 0, picDataLen, picDataOffset);
    FX_INT32 _PicWidthDataLen = 0;
    FX_INT32 _PicHeightDataLen = 0;
    _PicWidthDataLen = buffer[picDataOffset + picDataLen + 1] & 0xff;
    _PicHeightDataLen = buffer[picDataOffset + picDataLen + 1 + _PicWidthDataLen + 2] & 0xff;
#ifdef _DEBUG
    char buf [10] = {0};
    memcpy(buf,buffer+picDataLen+picDataOffset+2,_PicWidthDataLen);
    FILE *fd;
    fd = fopen("D:\\_PicWidthData.txt","wb");
    fwrite(buf,1,10,fd);
    fclose(fd);

    char _heightbuf[10] ={0};
    memcpy(_heightbuf,buffer+picDataOffset + picDataLen+_PicWidthDataLen + 4,_PicHeightDataLen);
    FILE *fp = fopen("D:\\_PicHeightData.txt","wb");
    fwrite(_heightbuf,1,10,fp);
    fclose(fp);
#endif
    for (FX_INT32 i = 0; i < _PicWidthDataLen; i++) {
        width += ((buffer[picDataLen + picDataOffset + 2 + i] & 0xff)
                << (8 * (_PicWidthDataLen - 1 - i)));
    }
    for (FX_INT32 j = 0; j < _PicHeightDataLen; j++) {
        height += ((buffer[picDataOffset + picDataLen + 4 + _PicWidthDataLen + j] & 0xff)
                << (8 * (_PicHeightDataLen - 1 - j)));
    }
    return true;
}



//class FR_DocProvider : public IOFD_DocProvider
#define SEAL_TYPE_CACHE_LEN        16

CFX_DIBitmap *FR_DocProvider::GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile) {
    LogOut("FR_DocProvider::GetSealBitmap 2");
    /*
    IFX_FileRead* file = FX_CreateFileRead("D:\\download\\b300.bmp");
    FX_INT64 size = file->GetSize();
    void* memory = malloc(size);
    file->ReadBlock(memory, size);
    CFX_DIBitmap* result = CreateSealBmp((FX_LPBYTE)memory, size);
    free(memory);
    file->Release();
    return result;
    // */
    int iSealDataLen = pSealFile->GetSize();
    FX_LPBYTE pSealData = FX_Alloc(FX_BYTE, iSealDataLen);
    if (!pSealData) {
        return NULL;
    }
    AutoFxMemory autopSealData(pSealData);
    ::ZeroMemory(pSealData, iSealDataLen);
    pSealFile->ReadBlock(pSealData, 0, iSealDataLen);
    FX_LPBYTE pPictureData = NULL;
    int pictureDataLen = 0;

    if (COFD_SG_Function::Ber_Decode_Sealdata(pSealData, iSealDataLen, NULL, pictureDataLen)) {
        pPictureData = FX_Alloc(FX_BYTE, pictureDataLen);
        if (!pPictureData) {
            return NULL;
        }
        AutoFxMemory autopPictureData(pPictureData);
        ::ZeroMemory(pPictureData, pictureDataLen);

        if (COFD_SG_Function::Ber_Decode_Sealdata(pSealData, iSealDataLen, pPictureData,
                                                  pictureDataLen)) {
            return CreateSealBmp(pPictureData, pictureDataLen);
        }
    } else {
        return NULL;
    }

    return NULL;
}


// xhj
CFX_DIBitmap *FR_DocProvider::GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile,
                                            COFD_StampAnnot *pStampAnnot) {
//    CFX_PtrArray m_pImageHandle;
    COFD_Signature *pSignature = pStampAnnot->GetSignature();
    if (pSignature == NULL)
        return NULL;

    CFX_BinaryBuf signValue;
    pSignature->GetSignatureValue(signValue);

    FX_LPBYTE pbBuf = signValue.GetBuffer();
    FX_STRSIZE size = signValue.GetSize();
    if (pbBuf == NULL || size <= 0)
        return NULL;

    CFX_DIBitmap *pDIB = GetSignedValueBitmap((const unsigned char *) pbBuf, size);

//        m_pRenderSeals.Add(pDIB);
//    m_pImageHandle.Add(pDIB);
    return pDIB;
}

CFX_DIBitmap *FR_DocProvider::GetSignedValueBitmap(const unsigned char *pSignBuf, int nLen) {
    unsigned char *pPicBuf = NULL;
    int nPicLen = 0;
    CFX_DIBitmap *pStampBMP = NULL;
    int nImgSubType = -1;
    std::string strErr = "";

    {
        if (pSignBuf == NULL || nLen <= 0) {
            strErr = "parameter error";
            goto end;
        }

        bool bParseImage = ParseSignedvalueImageMrsdk(pSignBuf, nLen, pPicBuf, nPicLen,
                                                      &nImgSubType);
        if (!bParseImage || (pPicBuf == NULL) || (nPicLen <= 0)) {
            strErr = "parse failed";
            goto end;
        }

        //pStampBMP = CreateSealBitmap(pPicBuf, nPicLen);
        if (nImgSubType == OFD_SEAL_OFD) {
            pStampBMP = RenderVetorBitmap(pPicBuf, nPicLen, 245, 245);
        } else {
            pStampBMP = FS_CreateSealBitmap(pPicBuf, nPicLen);
        }

        if (pStampBMP == NULL)
            strErr = "create bitmap failed";
    }

    end:

    if (pPicBuf != NULL) {
        delete[]pPicBuf;
        pPicBuf = NULL;
    }
    return pStampBMP;
}

CFX_DIBitmap *CreateVectorBitmapMrsdk(IOFD_FileStream *pOFDFileStream, int width, int height) {
    if (!pOFDFileStream) return NULL;
    IOFD_FilePackage *pOFDPackage = OFD_FilePackage_Create(pOFDFileStream);
    IOFD_Parser *pOFDParser = OFD_Parser_Create(pOFDPackage);
    if (!pOFDParser) {
        pOFDPackage->Release();
        pOFDFileStream->Release();
        return NULL;
    }
    if (pOFDParser->CountDocuments() == 0) {
        pOFDParser->Release();
        pOFDPackage->Release();
        pOFDFileStream->Release();
        return NULL;
    }
    IOFD_Document *pDoc = pOFDParser->GetDocument(0);
    if (!pDoc) {
        pOFDParser->Release();
        pOFDPackage->Release();
        pOFDFileStream->Release();
        return NULL;
    }

    IOFD_Page *pPage = pDoc->GetPage(0);
    pPage->LoadPage();
    CFX_AffineMatrix matrix;
    CFX_RectF pPageRect, physicalPageRect;
    FX_INT32 nRotate = (pPage->GetPageArea().GetRotate() / 90) % 4;
    pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_CropBox, pPageRect);
    pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, physicalPageRect);
    CFX_Rect deviceRect;

    CFX_AffineMatrix mtOffset;
    mtOffset.Translate(physicalPageRect.left - pPageRect.left,
                       physicalPageRect.top - pPageRect.top);

    deviceRect.Set(0, 0, width, height);
    OFD_GetPageMatrix(matrix, pPageRect, deviceRect, nRotate);
    matrix.Concat(mtOffset, TRUE);

    IOFD_RenderContext *pContentext = IOFD_RenderContext::Create();
    pContentext->AddPage(pPage, &matrix);

    CFX_DIBitmap *pDiBitMap = new CFX_DIBitmap;
    pDiBitMap->Create(FXSYS_round(width), FXSYS_round(height), FXDIB_Argb);
    pDiBitMap->Clear(0xFFFFFFFF);

    CFX_FxgeDevice *pFXgeDevice = new CFX_FxgeDevice;
    pFXgeDevice->Attach(pDiBitMap);

    COFD_RenderDevice OFDRenderDevice;
    OFDRenderDevice.Create(pFXgeDevice);

    IOFD_ProgressiveRenderer *pProgressRender = IOFD_ProgressiveRenderer::Create();

    pProgressRender->StartRender(&OFDRenderDevice, pContentext, NULL, &matrix);

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
    return pDiBitMap;
}

CFX_DIBitmap *
FR_DocProvider::RenderVetorBitmap(FX_LPBYTE pdata, FX_INT32 len, int width, int height) {
    if (!pdata || len <= 0)
        return NULL;
// 	CRect rect;
// 	m_wndPic.GetClientRect(&rect);
// 	if (rect.IsRectEmpty())
// 		return NULL;

    IOFD_FileStream *pOFDFileStream = OFD_CreateMemoryStream(pdata, len);
    if (!pOFDFileStream) return NULL;

    //return CreateVectorBitmap(pOFDFileStream,width,height/*rect.Height(),rect.Height()*/);//otherwise picture will stretch
    return CreateVectorBitmapMrsdk(pOFDFileStream, 245, 245);//client height
}

void FS_FormatSignBMPMrsdk(CFX_DIBitmap *pBmp) {
    pBmp->ConvertFormat(FXDIB_Argb);
    FX_INT32 pitch = pBmp->GetPitch();
    for (int Row = 0; Row < pBmp->GetHeight(); ++Row) {
        FX_LPBYTE pBuf = (pBmp->GetBuffer() + Row * pitch);
        for (FX_INT32 Column = 0; Column < pBmp->GetWidth(); ++Column) {
            if (pBuf[Column * 4] == 255 &&
                pBuf[Column * 4 + 1] == 255 &&
                pBuf[Column * 4 + 2] == 255) {
                pBuf[Column * 4 + 3] = 0;
            }
        }
    }
}

CFX_DIBitmap *FR_DocProvider::FS_CreateSealBitmap(FX_LPBYTE pdata, FX_INT32 len) {
    CFX_DIBitmap *pBitmap = FS_LoadImage(pdata, len);
    if (!pBitmap) return NULL;

    FS_FormatSignBMPMrsdk(pBitmap);
    return pBitmap;
}

CFX_DIBitmap *FR_DocProvider::FS_LoadImage(IFX_FileRead *pFileRead) {
    IFX_Image *pImage = FX_Image_Create();
    FX_HIMAGE hImage = pImage->LoadImage(pFileRead);
    if (!hImage) return NULL;
    pImage->LoadFrame(hImage, 0);

    //CFX_DIBitmap* pTemp = pImage->GetFrameImage(hImage);
    CFX_DIBitmap *pTemp = pImage->GetFrameImageWithTransparency(hImage);
    CFX_DIBitmap *pDIBitmap = pTemp->Clone();

    pImage->FreeImage(hImage);
    pImage->Release();

    return pDIBitmap;
}

CFX_DIBitmap *FR_DocProvider::FS_LoadImage(FX_LPCBYTE pBuf, FX_INT32 nSize) {
    IFX_MemoryStream *pMemoryStream = FX_CreateMemoryStream((FX_LPBYTE) pBuf, nSize);
    CFX_DIBitmap *pDIBitmap = FS_LoadImage(pMemoryStream);
    pMemoryStream->Release();

    return pDIBitmap;
}

bool FR_DocProvider::ParseSignedvalueImageMrsdk(const unsigned char *pBuf, int nBufSize,
                                                unsigned char *&pImageDataBuf, int &nImageSize2Res,
                                                int *pImgSubType) {

    std::string strError = "";
    bool bRes = false;

    {
        if (pBuf == NULL || nBufSize <= 0) {
            strError = "parameter error";
            goto end;
        }

        int nOffset = 0;
        int nOffsetType = 0;

        while (nOffset < nBufSize) {

            nOffsetType = nOffset + 1;
            if (nOffsetType >= nBufSize) {
                bRes = false;
                goto end;
            }

            //SEQUENCE Tag = 0x30
            //长编码【>128】 0x81 : length 1 byte , 0x82 : length 2 byte, 0x83 : length 3 byte, 0x84 : length 4 byte.
            //4 byte, 32位 ， 2147483647 , 21亿，当前规定的最大值。
            //图片数据，注定是长编码
            int nType = 0;
            if (pBuf[nOffset] == 0x30) {
                if (pBuf[nOffsetType] == 0x81 ||
                    pBuf[nOffsetType] == 0x82 ||
                    pBuf[nOffsetType] == 0x83 ||
                    pBuf[nOffsetType] == 0x84) {
#ifdef _DEBUG
                    if (nOffset >= 730){
                        int nTemp = 0;
                        //Debug
                    }
#endif
                    switch (pBuf[nOffsetType]) {
                        case 0x81: {
                            nType = nOffsetType + 1;
                        }
                            break;
                        case 0x82: {
                            nType = nOffsetType + 2;
                        }
                            break;
                        case 0x83: {
                            nType = nOffsetType + 3;
                        }
                            break;
                        case 0x84: {
                            nType = nOffsetType + 4;
                        }
                            break;
                    }
                    nType += 1;
                    if (nType >= nBufSize) {
                        bRes = false;
                        goto end;
                    }
#ifdef _DEBUG
                    if (nOffset >= 730){
                        int nTemp = 0;
                        //Debug
                    }
#endif
                    if (pBuf[nType] == 0x16) {//IA5String , ASN的编码是0x16
                        int nTypeLen = pBuf[nType + 1];    //图片格式类型不会长

                        int nTempOffset = nType + 2 + nTypeLen;

                        {
                            //图片格式名解析
                            char imgType[260];
                            memset(imgType, 0, 260);
                            memcpy(imgType, pBuf + nType + 2, nTypeLen);
                            std::string strSubType = imgType;
                            if (strSubType == "ofd") {
                                *pImgSubType = OFD_SEAL_OFD;
                            }
                        }

                        if (nTempOffset >= nBufSize) {
                            bRes = false;
                            goto end;
                        }

                        if (pBuf[nTempOffset] == 0x04) {//到这里，才确定数据是图片数据。
                            int nPicDataLenOC = nTempOffset + 1;    //OCTET STRING ， ASN的编码是0x04
                            if (nPicDataLenOC >= nBufSize) {
                                bRes = false;
                                goto end;
                            }
                            int nPicLenOffset = 0;
                            switch (pBuf[nPicDataLenOC]) {
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
                            if (nPicDataLenOC + nPicLenOffset + 1 > nBufSize) {
                                bRes = false;
                                goto end;
                            }
                            //解析图片数据长度
                            //这里 定性 图片的长度必然 大于 128， 属于长编码
                            int nPicDataLen = 0;
                            switch (nPicLenOffset) {
                                case 1:
                                    nPicDataLen = pBuf[nPicDataLenOC + 1] & 0xff;
                                    break;
                                case 2:
                                    nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 8) |
                                                  (pBuf[nPicDataLenOC + 2] & 0xff);
                                    break;
                                case 3:
                                    nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 16) |
                                                  ((pBuf[nPicDataLenOC + 2] & 0xff) << 8) |
                                                  (pBuf[nPicDataLenOC + 3] & 0xff);
                                    break;
                                case 4:
                                    nPicDataLen = ((pBuf[nPicDataLenOC + 1] & 0xff) << 24) |
                                                  ((pBuf[nPicDataLenOC + 2] & 0xff) << 16) |
                                                  ((pBuf[nPicDataLenOC + 3] & 0xff) << 8) |
                                                  (pBuf[nPicDataLenOC + 4] & 0xff);
                                    break;
                            }

                            int nPicDataOffset = nPicDataLenOC + nPicLenOffset + 1;
                            if ((nPicDataOffset + nPicDataLen) >= nBufSize) {
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

IOFD_FileStream *FR_DocProvider::GetSealFile(IOFD_Document *pDoc, IOFD_FileStream *pSealFile) {
    int iSealDataLen = pSealFile->GetSize();
    FX_LPBYTE pSealData = FX_Alloc(FX_BYTE, iSealDataLen);
    AutoFxMemory autopSealData(pSealData);
    AutoFxMemory autopPictureData;
    ::ZeroMemory(pSealData, iSealDataLen);
    pSealFile->ReadBlock(pSealData, 0, iSealDataLen);
    FX_LPBYTE pPictureData = NULL;
    int pictureDataLen = 0;

    if (COFD_SG_Function::Ber_Decode_Sealdata(pSealData, iSealDataLen, NULL, pictureDataLen)) {
        pPictureData = FX_Alloc(FX_BYTE, pictureDataLen);
        autopPictureData.attach(pPictureData);
        ::ZeroMemory(pPictureData, pictureDataLen);

        if (!COFD_SG_Function::Ber_Decode_Sealdata(pSealData, iSealDataLen, pPictureData,
                                                   pictureDataLen)) {
            return NULL;
        }
    } else {
        return NULL;
    }
    IOFD_FileStream *pOFDStampStream = OFD_CreateMemoryStream(pPictureData, pictureDataLen, TRUE);
    autopPictureData.detach();
// 	IOFD_FileStream *pWriteFilestream = OFD_CreateFileStream(L"D:\\signtest.ofd",OFD_FILEMODE_Write);
// 	pWriteFilestream->WriteBlock((const char *)pPictureData,0,pictureDataLen);
// 	pWriteFilestream->Flush();
// 	pWriteFilestream->Release();
    return pOFDStampStream;
}

OFD_SEALTYPE FR_DocProvider::GetSealType(IOFD_FileStream *pSealFile) const {
    if (NULL == pSealFile) {
        return OFD_SEAL_BITMAP;
    }
    int iSealDataLen = pSealFile->GetSize();
    FX_LPBYTE pSealData = FX_Alloc(FX_BYTE, iSealDataLen);
    AutoFxMemory autopSealData(pSealData);
    ::ZeroMemory(pSealData, iSealDataLen);
    pSealFile->ReadBlock(pSealData, 0, iSealDataLen);
    int pictureDataLen = 0;
    FX_LPBYTE pSealType = new FX_BYTE[SEAL_TYPE_CACHE_LEN];
    AutoArrayPtr<FX_BYTE> autopSealType(pSealType);
    memset(pSealType, 0, SEAL_TYPE_CACHE_LEN);
    if (COFD_SG_Function::Ber_Decode_Sealdata(pSealData, iSealDataLen, NULL, pictureDataLen,
                                              pSealType)) {
        CFX_ByteString sealtype(pSealType, strlen((const char *) pSealType));
        if (sealtype.Compare("ofd") == 0) {
            return OFD_SEAL_OFD;
        } else {
            return OFD_SEAL_BITMAP;
        }
    } else {
        return OFD_SEAL_BITMAP;
    }
}

CFX_DIBitmap *FR_DocProvider::CreateSealBmp(FX_LPBYTE pdata, FX_INT32 len) {
    CFX_DIBitmap *bmp = NULL;
    CFX_DIBAttribute *attr = NULL;
    if (pdata && len > 0) {
        IFX_MemoryStream *stream = FX_CreateMemoryStream(pdata, len);
        IFX_Image *image = FX_Image_Create();
        FX_HIMAGE himage = image->LoadImage(stream);
        if (himage) {
            /* comment by gm 20171103
            FXCODEC_IMAGE_TYPE imagetpe = image->GetImageType(himage);
            image->SetBackgroundColor(himage, 0x00000000);
            image->EnableTransparentImage(himage, true);
            image->SetImageOpacity(himage, 0);
            // */
            int frame = image->CountImageFrames(himage);
            if (frame > 0 && image->LoadFrame(himage, 0)) {
                bmp = image->GetFrameImage(himage);
                if (!bmp) {
                    return bmp;
                }
                /*LogOut("FOV_DocProvider::CreateSealBmp, bpp=%d", bmp->GetBPP());
                attr = image->GetImageAttribute(himage);
                LogOut("FOV_DocProvider::CreateSealBmp, CFX_DIBAttribute=0x%x", attr);
                if (attr) {
                    LogOut("FOV_DocProvider::CreateSealBmp, CFX_DIBAttribute::m_pGifLocalPalette=0x%x", attr->m_pGifLocalPalette);
                    if (attr->m_pGifLocalPalette) {
                        LogOut("FOV_DocProvider::CreateSealBmp, palette count=%d", attr->m_nGifLocalPalNum);
                        for (int i=0; i<attr->m_nGifLocalPalNum; ++i) {
                            LogOut("FOV_DocProvider::CreateSealBmp, Palette, 0x%x=0x%x", i, attr->m_pGifLocalPalette[i]);
                        }
                    }
                }// */
#if 0    // comment by gm 20171103, fixed redmine #6017
                FXDIB_Format format = bmp->GetFormat();
                if (format != FXDIB_Argb) {
                    if (!bmp->ConvertFormat(FXDIB_Argb)) {
                        return bmp;
                    }
                    int alpha = format&0xff00;
                    if (alpha == 0) {
                        int width = bmp->GetWidth();
                        int height = bmp->GetHeight();
                        for (int i=0; i<height; ++i) {
                            FX_LPCBYTE line = bmp->GetScanline(i);
                            FX_DWORD* buffer = (FX_DWORD*)line;
                            for (int j=0; j<width; ++j) {
                                FX_DWORD& color = buffer[j];
                                if (/*(color == 0xffffffff) || */((color&0xffffff) == 0x000000)) {
                                    color = 0x0;
                                }
                            }
                        }
                    }
                }
#endif // commnet end 20171103
            }
        }
    }
    return bmp;

}


CFX_DIBitmap *FR_DocProvider::GetSealBitmap(IOFD_Document *pDoc, IOFD_FileStream *pSealFile,
                                            OFD_SEALBITMAPTYPE type, COFD_StampAnnot *pStampAnnot) {
    LogOut("OfdToPdfByName: FR_DocProvider::GetSealBitmap 4");
//    return GetSealBitmap(pDoc, pSealFile);
    return GetSealBitmap(pDoc, pSealFile, pStampAnnot);
}

OFD_OESVERSION FR_DocProvider::GetOesVersion(COFD_Signature *pSignature) {
    OFD_OESVERSION ret = OFD_OES_SECOND_V2;
    return ret;
}

OFD_SEALBITMAPTYPE FR_DocProvider::GetSealBitmapType() {
    OFD_SEALBITMAPTYPE ret;
    return ret;
}

FX_BOOL FR_DocProvider::IsOesLibExist() {
    return TRUE;
}


void FR_OFDDocImp::setModified() {
    if (!m_pOFDDocument) return;
    FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock *) &m_lockModified);
    m_isModified = TRUE;
}
