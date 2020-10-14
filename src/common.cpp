#include "../include/common.h"

#include "fxcore.h"
#include "fs_util.h"
#include "fs_iconprovider.h"
#include "inc/fs_log.h"
#include "inc/commonImp.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_signatureImp.h"
#include "ofd/fr_ofdfontmapper.h"
#include "inc/fs_license.h"

#define USE_EXT_FONTMAPPER

class CFSPDF_Notifier;
static CFSPDF_Notifier* s_notifier =  NULL;

class CFSPDF_Notifier: public CFX_Object
{
public:
	static CFSPDF_Notifier* get()
	{
        if (!s_notifier)
            s_notifier = FX_NEW CFSPDF_Notifier();
		return s_notifier;
	}
    static void release()
    {
        if (s_notifier)
            delete s_notifier;
        s_notifier = NULL;
    }

	static void FPDF_OOM_Handler(FXMEM_FoxitMgr* pFoxitMgr, void* param)
	{
		FSNotifier* pNotifier = (FSNotifier*)param;
		if (!pNotifier) return;
		pNotifier->OnOutOfMemory();
	}

	FS_BOOL SetOOMNotifier(FSNotifier* notifier)
	{
		FXMEM_FoxitMgr* pFixedMgr = FXMEM_GetDefaultMgr();
		if (!pFixedMgr) 
			return FALSE;
		FXMEM_SetOOMHandler(pFixedMgr, &FPDF_OOM_Handler, notifier);
		return TRUE;
	}

protected:
	CFSPDF_Notifier() {}
	~CFSPDF_Notifier() {}

};



/************************************************************************************************
*									Library													    *
*************************************************************************************************/

FS_ERRORCODE FSLibrary::reinit()
{
	FSLibrary::release();

	return init("", "");
}

FS_ERRORCODE FSLibrary::init(const char* sn, const char* key)
{
	return CFS_Library::create(sn, key);
}

void FSLibrary::release()
{
	CFS_Library::release();
}

FSString FSLibrary::getVersion()
{
	return FSString("1.0.0830");
}

FS_MODULERIGHT FSLibrary::getModuleRight(FS_MODULENAME module)
{
	return FSCommonDefines::e_moduleRightWrite;
}

FS_BOOL FSLibrary::setAnnotIconProvider(FSAnnotIconProvider* iconProvider)
{
	if (!CFS_Library::get())
		return FALSE;
	return CFS_Library::get()->setAnnotIconProvider(iconProvider);
}

FS_BOOL FSLibrary::setNotifier(FSNotifier* notifier)
{
	if (!CFS_Library::get())
		return FALSE;
	return CFS_Library::get()->setNotifier(notifier);
}

FS_BOOL FSLibrary::setActionHandler(FSActionHandler* actionHandler)
{
	if (!CFS_Library::get())
		return FALSE;
	return CFS_Library::get()->setActionHandler(actionHandler);
}

FS_BOOL FSLibrary::registerDefaultSignatureHandler()
{
	if (!CFS_Library::get())
		return FALSE;
	return CFS_Library::get()->registerDefaultSignatureHandler();
}

FS_BOOL FSLibrary::registerSignatureHandler(const char* filter, const char* subFilter, FSSignatureHandler* signatureHandler)
{
	if (!CFS_Library::get())
		return FALSE;
	return CFS_Library::get()->registerSignatureHandler(filter, subFilter, signatureHandler);
}

FS_ERRORCODE FSLibrary::addExtFont(const char* filepath)
{
	if (!CFS_Library::get())
		return FSCommonDefines::e_errUnknown;
	return CFS_Library::get()->addExtFont(filepath);
}


/************************************************************************************************
*									Basic data structures or classes							*
*************************************************************************************************/
void FSPointF::set(FS_FLOAT x, FS_FLOAT y)
{
	this->x = x;
	this->y = y;
}

void FSRectF::set(FS_FLOAT left, FS_FLOAT bottom, FS_FLOAT right, FS_FLOAT top)
{
	this->left = left;
	this->bottom = bottom;
	this->right = right;
	this->top = top;
}

void FSRectI::set(FS_INT32 left, FS_INT32 top, FS_INT32 right, FS_INT32 bottom)
{
	this->left = left;
	this->bottom = bottom;
	this->right = right;
	this->top = top;
}

void FSDateTime::set(FS_UINT16 year, FS_UINT16 month, FS_UINT16 day, FS_UINT16 hour, FS_UINT16 minute, FS_UINT16 second, FS_UINT16 milliseconds, FS_INT16 UTHourOffset, FS_UINT16 UTMinuteOffset)
{
	this->year = year;
	this->month = month;
	this->day = day;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
	this->milliseconds = milliseconds;
	this->UTHourOffset = UTHourOffset;
	this->UTMinuteOffset = UTMinuteOffset;
}

void FSIdentityProperties::set(const char* corporation, const char* email, const char* loginName, const char* name)
{
	this->corporation = FSString();
	if (corporation)
		this->corporation = corporation;
	this->email = FSString();
	if (email)
		this->email = email;
	this->loginName = FSString();
	if (loginName)
		this->loginName = loginName;
	this->name = FSString();
	if (name)
		this->name = name;
}

void FSWrapperData::set(FS_INT32 version, const char* type, const char* appID, const char* uri, const char* description)
{
	this->version = version;
	this->type = FSString();
	if (type)
		this->type = FSString(type, FXSYS_strlen(type));
	this->appID = FSString();
	if (appID)
		this->appID = FSString(appID, FXSYS_strlen(appID));
	this->uri = FSString();
	if (uri)
		this->uri = FSString(uri, FXSYS_strlen(uri));
	this->description = FSString();
	if (description)
		this->description = FSString(description, FXSYS_strlen(description));
}

void FSMatrix::set(FS_FLOAT a, FS_FLOAT b, FS_FLOAT c, FS_FLOAT d, FS_FLOAT e, FS_FLOAT f)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->e = e;
	this->d = d;
	this->f = f;
}

FSMatrix FSMatrix::getReverse()
{
	CFX_Matrix identityMatrix;
	CFX_Matrix m(a, b, c, d, e, f);
	identityMatrix.SetReverse(m);

	FSMatrix ret;
	ret.set(identityMatrix.a, identityMatrix.b, identityMatrix.c, identityMatrix.d, identityMatrix.e, identityMatrix.f);
	return ret;
}

FSPointF FSMatrix::transform(FSPointF point)
{
	FSPointF refPoint;
	refPoint.set(point.x, point.y);

	CFX_Matrix m(a, b, c, d, e, f);
	m.TransformPoint(refPoint.x, refPoint.y);

	return refPoint;
}

FSRectF FSMatrix::transformRect(FSRectF rect)
{
	CFX_FloatRect rectOrigin(rect.left, rect.bottom, rect.right, rect.top);

	CFX_Matrix m(a, b, c, d, e, f);
	m.TransformRect(rectOrigin);

	FSRectF rectFResult;
	rectFResult.set(rectOrigin.left, 
					rectOrigin.bottom,
					rectOrigin.right,
					rectOrigin.top);

	return rectFResult;
}

#ifndef _SWIG_ANDROID_

template <class T>
static CFX_StringData* FX_AllocString(int nLen)
{
	if (nLen <= 0) 
		return NULL;

	CFX_StringData* pData = (CFX_StringData*)FX_Alloc(FX_BYTE, sizeof(long)*3 + (nLen+1)*sizeof(T));
	if (!pData) 
		return NULL;
	pData->m_nAllocLength = nLen*sizeof(T);
	pData->m_nDataLength = nLen*sizeof(T);
	pData->m_nRefs = 1;
	((T*)pData->m_String)[nLen] = 0;

	return pData;
}


static void FX_ReleaseString(CFX_StringData* pData)
{
	if (pData == NULL) return;

	pData->m_nRefs --;
	if (pData->m_nRefs <= 0)
		FX_Free(pData);
}

FSString::FSString ()
:m_pData(NULL)
,m_encoding(e_encUnknow)
,m_nChars(0)
{

}

FSString::FSString(const FSString& str)
{
	m_pData = str.m_pData;
	if (str.m_pData)
		str.m_pData->m_nRefs++;
	m_encoding = str.m_encoding;
	m_nChars = str.m_nChars;
}

FSString::~FSString ()
{
	FX_ReleaseString(m_pData);
}

FSString FSString::createFromByteString (const char* str)
{
	if (FSCRT_Util_IsStringEmpty(str)) 
		return FSString();

	FX_STRSIZE len = (FX_STRSIZE)FXSYS_strlen(str);
	FSString fsStr;
	fsStr.setLength(len);

	FXSYS_memcpy(fsStr.m_pData->m_String, str, len);
	fsStr.m_encoding = e_encPDFText;
	fsStr.m_nChars = len;

	return fsStr;
}

FSString FSString::createFromWideString (const wchar_t* wStr)
{
	FX_STRSIZE len = (FX_STRSIZE)FXSYS_wcslen(wStr);
	if (len < 1)
		return FSString();

	FSString str;
	str.m_encoding = e_encUnicode;
	str.setLength(len);
	if (str.m_pData){
		FXSYS_memcpy(str.m_pData->m_String, wStr, len*sizeof(wchar_t));
		str.m_nChars = len;
	}
	return str;
}

FSString::FSString (const char *buffer, int bufSize, FS_TEXTENCODING encoding)
{
	if (bufSize < 0)
		bufSize = buffer ? (FX_STRSIZE)FXSYS_strlen(buffer) : 0;

	if (bufSize) {
		m_pData = FX_AllocString<char>(bufSize);
		if (m_pData){
			FXSYS_memcpy32(m_pData->m_String, buffer, bufSize*sizeof(char));
			m_nChars = bufSize;
		} else
			m_nChars = 0;
	} else
	{
		m_pData = NULL;
		m_nChars = 0;
	}
	m_encoding = encoding;
}

FS_BOOL FSString::setLength(FS_DWORD size)
{
	if (size < 0)
		return FALSE;
	if (!m_pData && size == 0)
		return TRUE;

	FX_ReleaseString(m_pData);
	if (0 == size) {
		m_pData = NULL;
		return TRUE;
	}
	switch(m_encoding)
	{
	case e_encUTF32BE:
	case e_encUTF32LE:
		m_pData = FX_AllocString<FS_DWORD>(size);
		break;
	case e_encUTF16LE:
	case e_encUTF16BE:
		m_pData = FX_AllocString<FS_WORD>(size);
		break;
	case e_encUnicode:
		m_pData = FX_AllocString<wchar_t>(size);
		break;
	default:
		m_pData = FX_AllocString<FX_CHAR>(size);
		break;
	}
	if (!m_pData)
		return FALSE;
	return TRUE;
}

FSString&  FSString::operator= (const FSString &str)
{
	clear();
	m_pData = str.m_pData;
	if (str.m_pData)
		str.m_pData->m_nRefs++;
	m_encoding = str.m_encoding;
	m_nChars = str.m_nChars;
	return *this;
}

FSString&  FSString::operator= (const char *asciiStr)
{
	clear();
	int bufSize = -1;
	if (bufSize < 0)
		bufSize = asciiStr ? (FX_STRSIZE)FXSYS_strlen(asciiStr) : 0;

	if (bufSize) {
		m_pData = FX_AllocString<char>(bufSize);
		if (m_pData)
			FXSYS_memcpy32(m_pData->m_String, asciiStr, bufSize*sizeof(char));
	} else
		m_pData = NULL;
	m_encoding = e_encUTF8;
	m_nChars = bufSize;
	return *this;
}

FSString&  FSString::operator+= (const FSString &str)
{
	// todo...
	return *this;
}

FSString&  FSString::operator+= (const FS_DWORD ch)
{
	// todo...
	return *this;
}

FSString&  FSString::operator+= (const char *asciiStr)
{
	// todo...
	return *this;
}

FS_TEXTENCODING FSString::getEncoding()
{
	return m_encoding;
}

const char * FSString::getBuffer() const
{
	if (!m_pData)
		return NULL;
	return m_pData->m_String;
}

FS_DWORD FSString::getBufferLen() const
{
	if (!m_pData)
		return 0;
	return m_pData->m_nAllocLength;
}

FS_DWORD FSString::getCharsCount() const
{
	return m_nChars;
}

void FSString::toAscii () 
{
	// todo...
}

void FSString::toUTF8 () 
{
	switch (m_encoding)
	{
		case e_encUnicode:
			UnicodetoUTF8();
			break;
		case e_encAscii	 	:	// ASCII encoded text.
		case e_encUTF8		:	// Unicode encoding scheme.
		case e_encPDFText 	:	// Text represented as PDF Text (section 3.8.1 'Text Strings' in PDF Reference).
		case e_encUTF16BE 	:	// UTF-16BE (big-endian) encoding scheme.
		case e_encUTF16LE 	:	// UTF-16LE (little-endian) encoding scheme.
		case e_encUTF32BE 	:	// UTF-32BE (big-endian) encoding scheme.
		case e_encUTF32LE	:	// UTF-32LE (little-endian) encoding scheme.
			// todo...
			break;
	}
}

void FSString::toPDFText () 
{
	// todo...
}

void FSString::toUTF16LE () 
{
	switch (m_encoding)
	{
		case e_encUTF8:
			UTF8toUTF16LE();
			break;
		case e_encAscii	 	:	// ASCII encoded text.
		case e_encUnicode	:	// Unicode encoding scheme.
		case e_encPDFText 	:	// Text represented as PDF Text (section 3.8.1 'Text Strings' in PDF Reference).
		case e_encUTF16BE 	:	// UTF-16BE (big-endian) encoding scheme.
		case e_encUTF16LE 	:	// UTF-16LE (little-endian) encoding scheme.
		case e_encUTF32BE 	:	// UTF-32BE (big-endian) encoding scheme.
		case e_encUTF32LE	:	// UTF-32LE (little-endian) encoding scheme.
			// todo...
			break;
	}
}

template <class T>
FS_INT32 _UTF8toUTF16LE(const FS_BYTE* pSrc, FS_DWORD pSrcLen, T *pDst, FS_DWORD *pDstLen)
{
	if (pDstLen == NULL) return -1;
	if (pSrcLen < 1)
	{
		*pDstLen = 0;
		return 1;
	}
	const FS_BYTE* pOriSrc = pSrc;
	FS_INT32 i = 0, iPending = 0;
	FS_INT32 utf8Length = 0;
	FX_BYTE b;
	FS_DWORD wUnicode = 0;
	while (i ++ < pSrcLen)
	{
		b = (FX_BYTE)*pOriSrc ++;
		if (iPending == 0)
		{
			if (b < 0x80)
				wUnicode = (FS_DWORD)b;
			else if (b < 0xE0)
				wUnicode = (b & 0x1f), iPending = 1;
			else if (b < 0xF0)
				wUnicode = (b & 0x0f), iPending = 2;
			else if (b < 0xF8)
				wUnicode = (b & 0x07), iPending = 3;
			else if (b < 0xFC)
				wUnicode = (b & 0x03), iPending = 4;
			else //if (b < 0xFE)
				wUnicode = (b & 0x01), iPending = 5;
		}
		else
		{
			b &= 0x3f;
			wUnicode <<= 6;
			wUnicode |= b;
			iPending --;
		}
		if (iPending == 0)
		{
			if(sizeof(T) == 4)
			{
				if(pDst && utf8Length < (FS_INT32)*pDstLen)
					*pDst ++ = wUnicode;
				utf8Length += 1;
			} else if (sizeof(T) == 2)
			{
				if (wUnicode < 0x10000)
				{
					if(pDst && utf8Length < (FS_INT32)*pDstLen)
						*pDst ++ = wUnicode;
					utf8Length += 1;
				} else 
				{
					wUnicode -= 0x10000;
					if (wUnicode > 0xfffff) 
						continue;		// unsupported in UTF-16
					// break the 20-bit code into two words, each uses 10bits
					if(pDst && utf8Length + 2 <= (FS_INT32)*pDstLen)
					{
						*pDst ++ = 0xD800 | (wUnicode >> 10);
						*pDst ++ = 0xDC00 | (wUnicode & 0x3ff);
					}
					utf8Length += 1;
					utf8Length += 1;
				}
			}

		}
	}
	if (!pDst)*pDstLen = utf8Length;
	*pDstLen = utf8Length > *pDstLen ? *pDstLen : utf8Length;
	return 1;
}

void FSString::UTF8toUTF16LE()
{
	if (this->m_pData->m_nAllocLength < 1||m_pData->m_nDataLength < 1)
		return ;
	FX_LPCSTR pOriSrc = this->getBuffer();
	FS_DWORD pDstLen;
	_UTF8toUTF16LE<FS_WORD>((const FS_BYTE *)(const FS_BYTE *)getBuffer(), m_pData->m_nDataLength, NULL, &pDstLen);
	CFX_StringData* pData = FX_AllocString<FS_WORD>(pDstLen);
	_UTF8toUTF16LE<FS_WORD>((const FS_BYTE *)getBuffer(), m_pData->m_nDataLength, (FS_WORD *)pData->m_String, &pDstLen);
	 FX_ReleaseString(m_pData);
	 m_encoding = e_encUTF16LE;
	 m_pData = pData;
}
FS_BOOL FSString::isEmpty()
{
	if (!m_pData || m_nChars == 0)
		return TRUE;
	return FALSE;
}
void FSString::clear()
{
	FX_ReleaseString(m_pData);
	m_pData = NULL;
	m_nChars = 0;
	m_encoding = e_encUnknow;
}
void FSString::UnicodetoUTF8()
{
	if (m_encoding != e_encUnicode)
		return;
	if (!m_pData){
		this->m_encoding = e_encUTF8;
		return;
	}
	CFX_ByteString bsStr = FX_UTF8Encode((FX_LPCWSTR)m_pData->m_String, m_nChars);

	FX_ReleaseString(m_pData);
	m_pData = NULL;
	FS_INT32 len = bsStr.GetLength();
	if (len >= 1)
	{
		CFX_StringData* pData = FX_AllocString<FS_BYTE>(len);
		FXSYS_memcpy(pData->m_String, bsStr.GetBuffer(len), len);
		m_pData = pData;
	}
	this->m_encoding = e_encUTF8;
	return;
}

void FSString::toUTF16BE () 
{
	// todo...
}

void FSString::toUTF32LE () 
{
	// todo...
}

void FSString::toUTF32BE () 
{
	// todo...
}

void FSString::toUpper ()
{
	// todo...
}

void FSString::toLower ()
{
	// todo...
}


FSString FSString::left(int nChars)
{
	// todo...
	return FSString();
}

FSString FSString::right(int nChars)
{
	// todo...
	return FSString();
}

FSString FSString::mid(int startIndex, int nChars)
{
	// todo...
	return FSString();
}

int FSString::compare (const FSString &str) const
{
	// todo...
	return 0;
}
#endif

//////////////////////////////////////////////////////////////////////////
// CFS_Library

CFS_Library::CFS_Library()
	: m_pIconProviderManager(NULL)
	, m_pLogger(NULL)
	, m_pActionHandler(NULL)
#if defined(_LICENSE_BUILD_) || defined(_FS_LICENSED_BUILD_)
	, m_pLicenseRightMgr(NULL)
#endif
	, m_pFontMapper(NULL)
{

}
CFS_Library::~CFS_Library(){
	if (m_pFontMapper) {
		delete m_pFontMapper;
		m_pFontMapper = NULL;
	}
#if defined(_FS_LICENSED_BUILD_)
	if (m_pLicenseRightMgr) {
		delete m_pLicenseRightMgr;
		m_pLicenseRightMgr = NULL;
	}
#endif
	fxcoreFinalize();
	if (m_pActionHandler)
	{
		m_pActionHandler->release();
		m_pActionHandler = NULL;
	}
}

FS_ERRORCODE	CFS_Library::create(const char* sn, const char* key)
{
	if (g_pLibrary)
		return FSCommonDefines::e_errSuccess;
	g_pLibrary = new CFS_Library;
	if (!g_pLibrary)
		return FSCommonDefines::e_errOutOfMemory;
	return g_pLibrary->init(sn, key);
}

FS_ERRORCODE CFS_Library::init(const char* sn, const char* key)
{
	FS_ERRORCODE iRet = FSCommonDefines::e_errUnknown;
	fxcoreInitialize();

	iRet = this->CheckLicense(sn, key);

	if (FSCommonDefines::e_errSuccess != iRet )
		release();
	return iRet;
}

FS_ERRORCODE CFS_Library::fxcoreInitialize()
{
	FXMEM_GetDefaultMgr();
	CCodec_ModuleMgr* pCodecMgr = CCodec_ModuleMgr::Create();

	pCodecMgr->InitJbig2Decoder();
	pCodecMgr->InitJpxDecoder();

	CFX_GEModule::Create();
	CFX_GEModule::Get()->SetCodecModule(pCodecMgr);
	CPDF_ModuleMgr::Create();
	CPDF_ModuleMgr* pPDFMgr = CPDF_ModuleMgr::Get();
	pPDFMgr->SetCodecModule(pCodecMgr);
	pPDFMgr->InitPageModule();
	pPDFMgr->InitRenderModule();
	pPDFMgr->LoadEmbeddedGB1CMaps();
	pPDFMgr->LoadEmbeddedCNS1CMaps();
	pPDFMgr->LoadEmbeddedJapan1CMaps();
	pPDFMgr->LoadEmbeddedKorea1CMaps();

    if(pCodecMgr->GetPngModule()){
                printf("GetPngModule");
            }else{
                printf("InitPngDecoder");
                pCodecMgr->InitPngDecoder();
    }
	
#if !defined(USE_EXT_FONTMAPPER) && defined(_FX_FONTMGREX)
	IFXFM_FontMgr* pFontMgrEx = FXFM_CreateFontManager();
	CFX_GEModule::Get()->SetFontMgrEx(pFontMgrEx);
#else
	m_pFontMapper = FX_NEW FR_OFDFontMapper();
	CFX_GEModule::Get()->SetExtFontMapper(m_pFontMapper);
	m_pFontMapper->m_pFontMgr = CFX_GEModule::Get()->GetFontMgr();
#endif
	return FSCommonDefines::e_errSuccess;
}

FS_ERRORCODE CFS_Library::fxcoreFinalize()
{
	CCodec_ModuleMgr* pCodecMgr = CPDF_ModuleMgr::Get()->GetCodecModule();
	pCodecMgr->Destroy();
	CPDF_ModuleMgr::Destroy();
	CFX_GEModule::Destroy();
	FXMEM_DestroyFoxitMgr(FXMEM_GetDefaultMgr());
	return FSCommonDefines::e_errSuccess;
}

CFS_Library*	CFS_Library::get()
{
	return g_pLibrary;
}

void CFS_Library::release()
{
	if (!g_pLibrary)
		return;
	{
		FSCRT_LOCKOBJECT(lock, &g_pLibrary->m_lock);
		SAFE_DELETE(g_pLibrary->m_pIconProviderManager);
		SAFE_DELETE(g_pLibrary->m_pLogger);

        CFSPDF_Notifier::release();

#if defined(ENABLE_SIGNATURE)
		CPDF_SignatureHandlerMgr* pSigHandlerMgr = CPDF_SignatureHandlerMgr::GetSignatureHandlerMgr();
		if (pSigHandlerMgr)
		{
			// First release all the registered signature handler.
			FX_POSITION pos = g_pLibrary->m_MapSigHandlerName.GetStartPosition();
			while (pos) {
				CFX_ByteString bsKey;
				CFX_ByteString* pbsFilter;
				g_pLibrary->m_MapSigHandlerName.GetNextAssoc(pos, bsKey, (void*&)pbsFilter);
				CFX_ByteString bsSubFilter = bsKey.Right(bsKey.GetLength() - pbsFilter->GetLength());
				CPDF_SignatureHandler* pSigHandler = pSigHandlerMgr->GetSignatureHandler(*pbsFilter, bsSubFilter);
				if (pSigHandler)
				{
					pSigHandlerMgr->UnRegisterSignatureHandler(*pbsFilter, bsSubFilter);
					delete pSigHandler;
				}
				delete pbsFilter;
			}
			g_pLibrary->m_MapSigHandlerName.RemoveAll();
			// Destory signature handler manager.
			pSigHandlerMgr->Destroy();
		}
#endif
	}
	SAFE_DELETE(g_pLibrary);
}

FS_BOOL CFS_Library::setNotifier(FSNotifier* notifier)
{
	CFSPDF_Notifier* pNotifier = CFSPDF_Notifier::get();
	if (!pNotifier) 
		return FALSE;
	return pNotifier->SetOOMNotifier(notifier);
}

FS_BOOL	CFS_Library::setActionHandler(FSActionHandler* actionHandler)
{
	m_pActionHandler = actionHandler;
	return TRUE;
}

FSActionHandler* CFS_Library::getActionHandler()
{
	return m_pActionHandler;
}

FS_BOOL CFS_Library::setLogFile(CFSCRT_FileStream* pFile)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!pFile)
		return FALSE;
	SAFE_DELETE(m_pLogger);
	m_pLogger = new CFSCRT_Logger();
	if (!m_pLogger )
		return FALSE;
	m_pLogger->Initialize(pFile);
	return TRUE;
}

CFSPDF_IconProviderManager* CFS_Library::getIconProviderMgr()
{
	if (NULL == g_pLibrary)
		return NULL;
	if (!g_pLibrary->m_pIconProviderManager)
		g_pLibrary->m_pIconProviderManager = FX_NEW CFSPDF_IconProviderManager;
	return g_pLibrary->m_pIconProviderManager;
}

CFSCRT_Logger* CFS_Library::getLogger()
{
	if (!g_pLibrary)
		return NULL;
	return g_pLibrary->m_pLogger;
}

FS_BOOL CFS_Library::getConfig(void* id, void*& lpValue)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	
	return m_GlobalConfig.Lookup(id, lpValue);
}

FS_BOOL CFS_Library::setConfig(void* id, void* value)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);

	m_GlobalConfig.SetAt(id, value);
	return TRUE;
}



FS_BOOL CFS_Library::setAnnotIconProvider(FSAnnotIconProvider* iconProvider)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (getIconProviderMgr())
		return getIconProviderMgr()->SetAnnotIconProvider(iconProvider);
	return FALSE;
}

FS_BOOL	CFS_Library::registerDefaultSignatureHandler()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
#if defined(ENABLE_SIGNATURE)
	CPDF_SignatureHandlerMgr* pSigHandlerMgr = CPDF_SignatureHandlerMgr::GetSignatureHandlerMgr();
	if (!pSigHandlerMgr)
	{
		pSigHandlerMgr = CPDF_SignatureHandlerMgr::CreateSignatureHandlerMgr();
		if (!pSigHandlerMgr) 
			return FALSE;
	}
	pSigHandlerMgr->RegisterDefaultSignatureHandler();
#endif
	return TRUE;

}

FS_BOOL	CFS_Library::registerSignatureHandler(const char* filter, const char* subFilter, FSSignatureHandler* signatureHandler)
{
	if (FSCRT_Util_IsStringEmpty(filter) || !signatureHandler) 
		return FALSE;
	FSCRT_LOCKOBJECT(lock, &m_lock);
#if defined(ENABLE_SIGNATURE)
	CPDF_SignatureHandlerMgr* pSigHandlerMgr = CPDF_SignatureHandlerMgr::GetSignatureHandlerMgr();
	if (!pSigHandlerMgr)
	{
		pSigHandlerMgr = CPDF_SignatureHandlerMgr::CreateSignatureHandlerMgr();
		if (!pSigHandlerMgr) 
			return FALSE;
	}
	// Transform input data to fxcore data type.
	CFSPDF_SignatureHandler* pSigHandler = FX_NEW CFSPDF_SignatureHandler(signatureHandler);
	if (!pSigHandler) 
		return FALSE;
	CFX_ByteString* pbsFilter = FX_NEW CFX_ByteString(filter, FXSYS_strlen(filter));
	CFX_ByteString bsSubFilter(subFilter, FXSYS_strlen(subFilter));
	// Check if there is already a registered signature handler with same filter and sub filter name.
	// If yes, unregister it from Mgr and delete it.
	CPDF_SignatureHandler* pOldSigHandler = pSigHandlerMgr->GetSignatureHandler(*pbsFilter, bsSubFilter);
	if (pOldSigHandler)
	{
		pSigHandlerMgr->UnRegisterSignatureHandler(*pbsFilter, bsSubFilter);
		delete pOldSigHandler;
	}
	pSigHandlerMgr->RegisterSignatureHandler(*pbsFilter, bsSubFilter, (const CPDF_SignatureHandler*)pSigHandler);
	// Store the filter+subfilter name.
	CFX_ByteString bsKey = *pbsFilter+bsSubFilter;
	CFX_ByteString* bsTemp;
	if (!m_MapSigHandlerName.Lookup(bsKey, (void*&)bsTemp))
		m_MapSigHandlerName.SetAt(bsKey, (void*)pbsFilter);
	else
		delete pbsFilter;
#endif
	return TRUE;
}

FS_ERRORCODE				CFS_Library::addExtFont(const char* filepath)
{
	if (!m_pFontMapper) {
		return FSCommonDefines::e_errUnknown;
	}
	m_pFontMapper->AddExtFont(filepath);
	return FSCommonDefines::e_errSuccess;
}

FS_ERRORCODE		CFS_Library::CheckLicense(const char* sn, const char* key)
{
#if defined(_FS_LICENSED_BUILD_)
	if (NULL == m_pLicenseRightMgr) {
		m_pLicenseRightMgr = new CFS_OFDLicenseManager();
	}
	CFX_WideString wsSerialnum = CFX_WideString::FromUTF8(sn);
	CFX_WideString wsPath = CFX_WideString::FromUTF8(key);
	FX_INT32 ret = m_pLicenseRightMgr->CheckLicense(wsSerialnum, wsPath);
	if (OFD_LICENSE_OK==ret) { 
		return FSCommonDefines::e_errSuccess;
	} else if (OFD_LICENSE_DIR==ret || OFD_LICENSE_WRITE==ret) {
		return FSCommonDefines::e_errFile;
	} else if (OFD_LICENSE_SERIAL==ret || OFD_LICENSE_SERIALEXPIRED==ret){
		return FSCommonDefines::e_errInvalidLicense;
	} else if (OFD_LICENSE_NET==ret) {
		return FSCommonDefines::e_errNetwork;
	} else if (OFD_LICENSE_EXCEED==ret) {
		return FSCommonDefines::e_errLicenseExceed;	
	} else if (OFD_LICENSE_EXPIRED==ret) {
		return FSCommonDefines::e_errLicenseExpired;
	} else if (OFD_LICENSE_FILE==ret) {
		return FSCommonDefines::e_errFormat;
	} else if (OFD_LICENSE_HARDWARE==ret) {
		return FSCommonDefines::e_errDeviceChanged;
	} else if (OFD_LICENSE_VERSION==ret) {
		return FSCommonDefines::e_errLicenseVersion;
	} else {
		return FSCommonDefines::e_errUnknown;
	}
#else
	return FSCommonDefines::e_errSuccess;
#endif
}

FX_BOOL			CFS_Library::CheckModuleLicense(const CFX_WideString& strModuleName)
{
#if defined(_FS_LICENSED_BUILD_)
	if (NULL == m_pLicenseRightMgr) {
		return FALSE;
	}
	return m_pLicenseRightMgr->CheckModuleLicense(strModuleName);
#else
	return TRUE;
#endif
}

//////////////////////////////////////////////////////////////////////////
// FSFont

const FX_CHAR * g_StandardFontNames[] = {
	"Courier",
	"Courier-Bold",
	"Courier-BoldOblique",
	"Courier-Oblique",
	"Helvetica",
	"Helvetica-Bold",
	"Helvetica-BoldOblique",
	"Helvetica-Oblique",
	"Times-Roman",
	"Times-Bold",
	"Times-BoldItalic",
	"Times-Italic",
	"Symbol",
	"ZapfDingbats"
};

FSFont* FSFont::create(const char* fontName, FS_DWORD fontStyles, FS_INT32 weight, FS_FONTCHARSET charset)
{
	FSFontImp* pFont = FX_NEW FSFontImp;
	if (!pFont) return NULL;
	if (FALSE == pFont->initFromAttribute(fontName, fontStyles, weight, charset))
	{
		pFont->release();
		pFont = NULL;
	}
	return pFont;
}

FSFont* FSFont::createStandard(FS_STANDARDFONTID fontID)
{
	FSFontImp* pFont = FX_NEW FSFontImp;
	if (!pFont) return NULL;
	if (FALSE == pFont->initFromStandardID(fontID))
	{
		pFont->release();
		pFont = NULL;
	}
	return pFont;
}
//////////////////////////////////////////////////////////////////////////
// FSFontImp
FSFontImp::FSFontImp()
	:m_pFontEx(NULL)
	,m_bOwnFont(FALSE)
	,m_nCountRef(1)
	,m_pMapDoc2PDFFont(NULL)
	,m_iLoadType(FSCRT_FONTLOADTYPE_UNKNOWN)
{

}

FSFontImp::~FSFontImp()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lockMap);
		if (m_pMapDoc2PDFFont)
		{
			FX_POSITION pos = m_pMapDoc2PDFFont->GetStartPosition();
			while (pos) 
			{
				void* pKey = NULL;
				void* pValue = NULL;
				m_pMapDoc2PDFFont->GetNextAssoc(pos, pKey, pValue);
				if (pKey)
				{
					((FSPDFDocImp*)pKey)->removeMapRecord((CPDF_Dictionary*)pValue);
				}
			}
			m_pMapDoc2PDFFont->RemoveAll();
			delete m_pMapDoc2PDFFont;
			m_pMapDoc2PDFFont = NULL;
		}
	}
	if (NULL != m_pFontEx)
	{
		CFX_Font* pFont = m_pFontEx->GetFont();
		m_pFontEx->Release();
		m_pFontEx = NULL;

		if (m_bOwnFont && pFont)
			delete pFont;
	}
}

void FSFontImp::release()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		m_nCountRef --;	
		if (0 != m_nCountRef) {
			return;
		}
	}
	delete this;
}

FSString FSFontImp::getName()
{
	if (!m_pFontEx || !m_pFontEx->GetFont()) 
		return FSString();

	CFX_WideString psName = getPsName();
#if _FX_OS_ == _FX_IOS_
    if(psName.Equal(L"FoxitFixedOTF"))
        psName = L"Courier";
    if(psName.Equal(L"FoxitFixedOTF-Bold"))
        psName = L"Courier-Bold";
    if(psName.Equal(L"FoxitFixedOTF-BoldItalic"))
        psName = L"Courier-BoldOblique";
    if(psName.Equal(L"FoxitFixedOTF-Italic"))
        psName = L"Courier-Oblique";
    if(psName.Equal(L"FoxitSansOTF"))
        psName = L"Helvetica";
    if(psName.Equal(L"FoxitSansOTF-Bold"))
        psName = L"Helvetica-Bold";
    if(psName.Equal(L"FoxitSansPS-BoldItalic"))
        psName = L"Helvetica-BoldOblique";
    if(psName.Equal(L"FoxitSansOTF-Italic"))
        psName = L"Helvetica-Oblique";
    if(psName.Equal(L"FoxitSerifOTF"))
        psName = L"Times-Roman";
    if(psName.Equal(L"FoxitSerifOTF-Bold"))
        psName = L"Times-Bold";
    if(psName.Equal(L"FoxitSerifOTF-BoldItalic"))
        psName = L"Times-BoldItalic";
    if(psName.Equal(L"FoxitSerifOTF-Italic"))
        psName = L"Times-Italic";
    if(psName.Equal(L"FoxitSymbol"))
        psName = L"Symbol";
    if(psName.Equal(L"FoxitDingbats"))
        psName = L"ZapfDingbats";
#endif
	CFX_ByteString bsName = FSCRT_UTF8_FromWideString(psName);
	FSString retStr(bsName.GetBuffer(bsName.GetLength()), bsName.GetLength());
	bsName.ReleaseBuffer();
	return retStr;
}

FS_BOOL FSFontImp::initFromAttribute(const char* fontName, FS_DWORD fontStyles, FS_INT32 weight, FS_FONTCHARSET charset)
{
	if (NULL == fontName 
		|| charset<FSCommonDefines::e_fontCharsetANSI || charset>FSCommonDefines::e_fontCharsetBaltic
		)
		return FALSE;
	if (!initMap()) return FALSE;
	CFX_Font* pFont = FX_NEW CFX_Font();
	if (!pFont) return FALSE;
	CFX_WideString wStr = CFX_WideString::FromUTF8(fontName, FXSYS_strlen(fontName));
	FS_INT32 iCPForCreateFont = FSCRT_UnicodeMapper::GetCodePageFromCharset(charset);
	// SDK-5146: To convert fontName from widestring to bytestring, it's better to use the relative codepage which is used for creating font.
	// If want to create a font with charset GB2312/CHINESEBIG5/SHIFTJIS/HANGEUL, just use the relative codepage to do the conversion of fontName.
	// For other font charset, check the system ACP and get relative codepage to do the conversion.
	// (Code of CFXFM_FontMgr::GetUnicodeEncodedName() in fxfm_fontex_imp.cpp can be a reference.)
	FS_INT32 iCPForConvert = iCPForCreateFont;
	if ( FSCommonDefines::e_fontCharsetGB2312 != charset && FSCommonDefines::e_fontCharsetChineseBig5 != charset 
		&& FSCommonDefines::e_fontCharsetShift_JIS != charset && FSCommonDefines::e_fontCharsetHangeul != charset)
	{
		FS_INT32 iSysACP = FSCRT_UnicodeMapper::GetSysACP();
		FS_INT32 iTempCharset = FSCRT_UnicodeMapper::GetCharsetFromCodePage(iSysACP);
		iCPForConvert = FSCRT_UnicodeMapper::GetCodePageFromCharset(iTempCharset);
	}
	CFX_CharMap* pCharMap = CFX_CharMap::GetDefaultMapper(iCPForConvert);
	CFX_ByteString bStr;
	bStr.ConvertFrom(wStr, pCharMap);
#if (_FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_)
	if(874 == iCPForCreateFont)//the font for thai can't be mapped by fontstyle != 0
		fontStyles = 0;
#endif
	if (!pFont->LoadSubst(bStr, TRUE, fontStyles, weight, 0, iCPForCreateFont))
	{
		delete pFont;
		return FALSE;
	}
	m_pFontEx = FX_CreateFontEx(pFont);
	if (NULL == m_pFontEx)
	{
		delete pFont;
		return FALSE;
	}
	m_bOwnFont = TRUE;
	m_iLoadType = FSCRT_FONTLOADTYPE_ATTRIBUTE;
	m_pro.fontStyle = fontStyles;
	m_pro.weight = weight;
	m_pro.charset = charset;
	return TRUE;
}

FS_BOOL FSFontImp::initFromStandardID(FS_STANDARDFONTID fontID)
{
	if (fontID < FSCommonDefines::e_fontStandardIDCourier||fontID > FSCommonDefines::e_fontStandardIDZapfDingbats)
		return FALSE;
	if (!initMap()) 
		return FALSE;
	CFX_Font* pFont = FX_NEW CFX_Font();
	if (!pFont) 
		return FALSE;
	if (!pFont->LoadMemory((FS_INT32)fontID))
	{
		delete pFont;
		return FALSE;
	}
	m_pFontEx = FX_CreateFontEx(pFont);
	if (NULL == m_pFontEx)
	{
		delete pFont;
		return FALSE;
	}
	m_bOwnFont = TRUE;
	m_iLoadType = FSCRT_FONTLOADTYPE_STANDARD;
	m_pro.fontID = fontID;
	return TRUE;
}

FS_BOOL FSFontImp::initFromPDFFont(FSPDFDocImp* pDoc, CPDF_Font* pPDFFont)
{
	if (!pPDFFont) 
		return FALSE;
	if (!initMap()) 
		return FALSE;
	m_bOwnFont = FALSE;
	m_pFontEx = FX_CreateFontEx(&(pPDFFont->m_Font));
	if (m_pFontEx)
	{
		FSCRT_LOCKOBJECT(lock, &m_lockMap);
		// Record to map
		m_pMapDoc2PDFFont->SetAt(pDoc, pPDFFont->GetFontDict());
		m_iLoadType = FSCRT_FONTLOADTYPE_FROMPDFFONT;
	}
	return (NULL != m_pFontEx);
}

FS_BOOL FSFontImp::initMap()
{
	if (!m_pMapDoc2PDFFont)
		m_pMapDoc2PDFFont = FX_NEW CFX_MapPtrToPtr();
	return (NULL != m_pMapDoc2PDFFont);
}

CFX_WideString FSFontImp::getPsName()
{
	if (!m_pFontEx || !m_pFontEx->GetFont())
		return CFX_WideString();
	return m_pFontEx->GetFont()->GetPsName();
}

CPDF_Font*	FSFontImp::toPDFFont(CPDF_Document* pDoc)
{
	if (!m_pFontEx || !m_pFontEx->GetFont()) 
		return NULL;
	CPDF_Font* pPDFFont = NULL;
	if (FSCRT_FONTLOADTYPE_ATTRIBUTE == m_iLoadType) 
	{
		CFX_Font* pFont = m_pFontEx->GetFont();
		if (!pFont) 
			return NULL;
		FS_BOOL bVert = pFont->IsVertical();
		FS_INT32 charset = m_pro.charset;
		pPDFFont = pDoc->AddFont(pFont, charset, bVert);
	} else if (FSCRT_FONTLOADTYPE_STANDARD == m_iLoadType) 
		pPDFFont = pDoc->AddStandardFont(g_StandardFontNames[m_pro.fontID], NULL);

	return pPDFFont;
}

FSFontImp*	FSFontImp::retain()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_nCountRef++;
	return this;
}

CPDF_Dictionary* FSFontImp::getPDFFontDict(FSPDFDocImp* pDoc)
{
	if (!pDoc || !pDoc->getPDFDocument() || !m_pFontEx) 
		return NULL;
	FSCRT_LOCKOBJECT(lock, &m_lockMap);
	if (!m_pMapDoc2PDFFont && !initMap())
		return NULL;

	// 1. check if there's already a matched CPDF_Font in this document. 
	// If yes, use it directly.
	void* pData = NULL;
	if (m_pMapDoc2PDFFont->Lookup(pDoc, pData))
		return (CPDF_Dictionary*)pData;

	// 2. If no matched CPDF_Font in this document, create one 
	// and record to maps in FSFontImp and FSPDFDocImp.
	CPDF_Font* pPDFFont = toPDFFont(pDoc->getPDFDocument());
	if (!pPDFFont) 
		return NULL;
	CPDF_Dictionary* pFontDict = pPDFFont->GetFontDict();
	// Record to FSFontImp's map
	m_pMapDoc2PDFFont->SetAt(pDoc, pFontDict);
	// Record to FSPDFDocImp's maps
	pDoc->addToFontMaps(this, pFontDict);

	return pFontDict;
}

// bReleaseFont: TRUE means to release current font after remove Map record.
FS_BOOL FSFontImp::removeMapRecord(FSPDFDocImp* pDoc)
{
	if (!m_pFontEx || !pDoc || !m_pMapDoc2PDFFont) 
		return FALSE;
	FSCRT_LOCKOBJECT(lock, &m_lockMap);

	m_pMapDoc2PDFFont->RemoveKey(pDoc);
	return TRUE;
}

FS_BOOL	FSFontImp::isMapEmpty()
{
	FSCRT_LOCKOBJECT(lock, &m_lockMap);
	if (!m_pFontEx || !m_pMapDoc2PDFFont || m_pMapDoc2PDFFont->GetCount()<1)
		return TRUE;
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// FSDefaultAppearance
void FSDefaultAppearance::set(FS_DWORD flags, FSFont* font, FS_FLOAT fontSize, FS_ARGB textColor)
{
	this->flags = flags;
	if (font)
//		this->font = ((FSFontImp*)font)->retain();
    this->font = ((FSFontImp*)font);
	else
		this->font = NULL;
	this->fontSize = fontSize;
	this->textColor = textColor;
}

//////////////////////////////////////////////////////////////////////////
// FSPDFPath
FSPDFPath* FSPDFPath::create()
{
	FSPDFPathImp* pPathImp = NULL;
	pPathImp = FX_NEW FSPDFPathImp();
	if (pPathImp && FALSE == pPathImp->init())
	{
		delete pPathImp;
		pPathImp = NULL;
	}
	return pPathImp;
}

//////////////////////////////////////////////////////////////////////////
// FSPDFPathImp
FSPDFPathImp::FSPDFPathImp()
	:m_pPath(NULL)
    ,m_nCountRef(1)
{
	
}

FSPDFPathImp::~FSPDFPathImp()
{
	SAFE_DELETE(m_pPath);
}

FS_BOOL	FSPDFPathImp::init()
{
	if (!m_pPath)
		m_pPath = FX_NEW CFX_PathData;
	else
		clear();
	return (NULL != m_pPath);
}

FSPDFPathImp*	FSPDFPathImp::retain()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_nCountRef++;
	return this;
}

void		FSPDFPathImp::release()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		m_nCountRef --;	
		if (m_nCountRef != 0) {
			return ;
		}
	}
	delete this;
}

FS_INT32	FSPDFPathImp::getPointCount()
{
	if (!m_pPath)
		return 0;
	return m_pPath->GetPointCount();
}

FSPointF	FSPDFPathImp::getPoint(FS_INT32 index)
{
	FSPointF retPt;
	retPt.set(0, 0);
	if (!m_pPath||index < 0) 
		return retPt;
	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	if (index > nPointsCount - 1)
		return retPt;

	FX_PATHPOINT* pPathPoint = m_pPath->GetPoints() + index;
	if (pPathPoint)
		retPt.set(pPathPoint->m_PointX, pPathPoint->m_PointY);
	return retPt;
}

FS_PATHPOINTTYPE		FSPDFPathImp::getPointType(FS_INT32 index)
{
	FS_PATHPOINTTYPE retType = (FS_PATHPOINTTYPE)0;
	if (!m_pPath || index < 0) 
		return retType;

	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	if(index > nPointsCount -1)
		return retType;

	FX_PATHPOINT* pPathPoint = m_pPath->GetPoints() + index;
	if (pPathPoint)
		retType = FSCRT_PointType_Fxcore2RDK(pPathPoint->m_Flag);
	return retType;
}

FS_BOOL		FSPDFPathImp::setPoint(FS_INT32 index, FSPointF point, FS_PATHPOINTTYPE pointType)
{
	if (!m_pPath 
		|| pointType < FSCommonDefines::e_pointTypeMoveTo 
		|| pointType > FSCommonDefines::e_pointTypeBezierToCloseFigure
		|| index < 0)
		return FALSE;
	FS_INT32 nPointsCount = m_pPath->GetPointCount(); 
	if (index > nPointsCount -1)
		return FALSE;
	m_pPath->SetPoint(index, point.x, point.y, FSCRT_PointType_RDK2Fxcore(pointType));
	return TRUE;
}

FS_BOOL	FSPDFPathImp::moveTo(FSPointF point)
{
	if (!m_pPath) 
		return FALSE;
	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	FX_BOOL bRet = m_pPath->AddPointCount(1);
	if (!bRet)
		return FALSE;

	m_pPath->SetPoint(nPointsCount, point.x, point.y, FXPT_MOVETO);
	return TRUE;
}

FS_BOOL		FSPDFPathImp::lineTo(FSPointF point)
{
	if (!m_pPath) 
		return FALSE;
	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	FX_BOOL bRet = m_pPath->AddPointCount(1);
	if (!bRet)
		return FALSE;

	m_pPath->SetPoint(nPointsCount, point.x, point.y, FXPT_LINETO);
	return TRUE;
}

FS_BOOL	FSPDFPathImp::cubicBezierTo(FSPointF point1, FSPointF point2, FSPointF point3)
{
	if (!m_pPath) 
		return FALSE;
	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	FX_BOOL bRet = m_pPath->AddPointCount(3);
	if (!bRet)
		return FALSE;

	m_pPath->SetPoint(nPointsCount++, point1.x, point1.y, FXPT_BEZIERTO);
	m_pPath->SetPoint(nPointsCount++, point2.x, point2.y, FXPT_BEZIERTO);
	m_pPath->SetPoint(nPointsCount, point3.x, point3.y, FXPT_BEZIERTO);
	return TRUE;
}

FS_BOOL	FSPDFPathImp::closeFigure()
{
	if (!m_pPath) 
		return FALSE;
	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	if (nPointsCount < 1)
		return FALSE;
	FS_INT32 flag = m_pPath->GetFlag(nPointsCount - 1);
	if (FXPT_MOVETO == flag) 
		return FALSE;

	FX_FLOAT fpointX = m_pPath->GetPointX(nPointsCount - 1);
	FX_FLOAT fpointY = m_pPath->GetPointY(nPointsCount - 1);
	m_pPath->SetPoint(nPointsCount - 1, fpointX, fpointY, flag | FXPT_CLOSEFIGURE);
	return TRUE;
}

FS_BOOL	FSPDFPathImp::removePoint(FS_INT32 index)
{
	if (!m_pPath || index < 0)
		return FALSE;

	FS_INT32 nPointsCount = m_pPath->GetPointCount();
	if (index > nPointsCount -1)
		return FALSE;

	FX_PATHPOINT *pPoint =  m_pPath->GetPoints();
	if (!pPoint)
		return FALSE;
	if (index + 1 < nPointsCount)
		FXSYS_memmove(pPoint + index, pPoint + index + 1, sizeof(FX_PATHPOINT) * (nPointsCount - index - 1));
	m_pPath->TrimPoints(nPointsCount - 1);
	return TRUE;
}

void	FSPDFPathImp::clear()
{
	if (m_pPath)
		m_pPath->SetPointCount(0);
}

FS_BOOL	FSPDFPathImp::setPointCount(FS_INT32 pointCount)
{
	if (!m_pPath || pointCount<0)
		return FALSE;
	return m_pPath->SetPointCount(pointCount);
}

class CFSCRT_StreamRead : public FSFileRead, public CFX_Object
{
public:
	CFSCRT_StreamRead(FS_BOOL isRawData, CPDF_Stream* pStream):m_isRawData(isRawData),m_pStream(pStream) 
	{
		m_streamAcc.LoadAllData(pStream, isRawData);
	}
	virtual ~CFSCRT_StreamRead() {}

	virtual FS_BOOL		readBlock(void* buffer, FS_UINT64 offset, FS_UINT64 size)
	{
		if (!m_pStream)
			return FALSE;
		FX_LPCBYTE pData = m_streamAcc.GetData();
		memcpy(buffer, pData+offset, size);
		return TRUE;

	}
	virtual FS_UINT64	getSize() 
	{
		return m_streamAcc.GetSize();
	}

	virtual void		release() {delete this;}

private:
	CPDF_StreamAcc  m_streamAcc;
	CPDF_Stream*	m_pStream;
	// raw data flag
	FS_BOOL			m_isRawData;
};


FSFileSpec* FSFileSpec::create(FSPDFDoc* pdfDoc)
{
	FSFileSpecImp* pFileSpec = FX_NEW FSFileSpecImp(pdfDoc, NULL);
	if (!pFileSpec)
		return NULL;
	if (!pFileSpec->create()) {
		delete pFileSpec;
		return NULL;
	}
	return pFileSpec;
}

FSFileSpecImp::FSFileSpecImp(FSPDFDoc* pdfDoc, CPDF_Dictionary* pDict)
{
	m_pDoc = pdfDoc;
	m_pAttachmentDict = pDict;
}

FS_BOOL FSFileSpecImp::create()
{
	if (!m_pDoc) 
		return FALSE;
	if (m_pAttachmentDict)
		m_pAttachmentDict->Release();
	CPDF_Document* pSTDoc = ((FSPDFDocImp*)m_pDoc)->getPDFDocument();
	m_pAttachmentDict = CPDF_Dictionary::Create();
	if (!m_pAttachmentDict) 
		return FALSE;
	m_pAttachmentDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("Filespec"));
	pSTDoc->AddIndirectObject(m_pAttachmentDict);
	return TRUE;
}

void FSFileSpecImp::release()
{
	delete this;
}

FSString FSFileSpecImp::getFileName()
{
	if (!m_pAttachmentDict) 
		return FSString();
	CPDF_FileSpec spec(m_pAttachmentDict);
	CFX_WideString wsFileName;
	FS_BOOL bRet = spec.GetFileName(wsFileName);
	if (!bRet) 
		return FSString();

	CFX_ByteString bsUtf8 = wsFileName.UTF8Encode();
	return FSString((FX_LPCSTR)bsUtf8);
}

void FSFileSpecImp::setFileName(const char* fileName)
{
	if (!fileName||!m_pAttachmentDict) 
		return;
	CPDF_FileSpec spec(m_pAttachmentDict);
	CFX_ByteString bsFileName = CFX_ByteString(fileName);
	CFX_WideString wsFileName = CFX_WideString::FromUTF8(bsFileName);
	spec.SetFileName(wsFileName);
}

FS_RESULT FSFileSpecImp::getParamDict(CPDF_Dictionary*& pDict)
{
	if (!m_pAttachmentDict) 
		return FSCRT_ERRCODE_ERROR;

	pDict = NULL;
	CPDF_Dictionary* pEFDict = m_pAttachmentDict->GetDict(FX_BSTRC("EF"));
	if (!pEFDict) 
		return FSCRT_ERRCODE_ERROR;

	CPDF_Stream* pStream = pEFDict->GetStream(FX_BSTRC("F"));
	if (!pStream) 
		return FSCRT_ERRCODE_ERROR;

	CPDF_Dictionary* pEmbFileDict = pStream->GetDict();
	pDict = pEmbFileDict->GetDict(FX_BSTRC("Params"));
	
	return pDict ? FSCRT_ERRCODE_SUCCESS : FSCRT_ERRCODE_ERROR;
}

FS_INT64 FSFileSpecImp::getFileSize()
{
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS || !pParamDict) 
		return 0;

	return pParamDict->GetInteger64(FX_BSTRC("Size"));
}

FSFileRead* FSFileSpecImp::getFileData()
{
	if (!m_pAttachmentDict) 
		return NULL;

	CPDF_FileSpec spec(m_pAttachmentDict);
	CPDF_Stream* pStream = spec.GetFileStream();
	if (!pStream) 
		return NULL;

	CFSCRT_StreamRead* pStreamRead = FX_NEW CFSCRT_StreamRead(FALSE, pStream);
	return pStreamRead;
}

FS_BOOL FSFileSpecImp::embed(const char* filePath)
{
	if (!m_pAttachmentDict) 
		return FALSE;
	IFX_FileStream* pFileStream = FX_CreateFileStream(filePath,FX_FILEMODE_ReadOnly);
	if (!pFileStream) 
		return FALSE;

	CPDF_Dictionary* pEFDict = m_pAttachmentDict->GetDict(FX_BSTRC("EF"));
	if (!pEFDict) {
		pEFDict = FX_NEW CPDF_Dictionary;	
		if (!pEFDict) {
			pFileStream->Release();
			return FALSE;
		}
		m_pAttachmentDict->SetAt(FX_BSTRC("EF"), pEFDict);
	}

	CPDF_Dictionary* pDict = NULL;
	CPDF_Stream* pEmbFileStream = pEFDict->GetStream(FX_BSTRC("F"));
	CPDF_Dictionary* pFDict = pEmbFileStream?pEmbFileStream->GetDict():NULL;
	if (pFDict) {
		pDict = (CPDF_Dictionary*)pFDict->Clone();
		pDict->RemoveAt(FX_BSTRC("Subtype"));
		pDict->RemoveAt(FX_BSTRC("Filter"));
		pDict->RemoveAt(FX_BSTRC("DecodeParms"));
		pDict->RemoveAt(FX_BSTRC("FFilter"));
		pDict->RemoveAt(FX_BSTRC("FDecodeParms"));
		pDict->RemoveAt(FX_BSTRC("DL"));
	} else {
		pDict = FX_NEW CPDF_Dictionary;
		if (!pDict) {
			pFileStream->Release();
			return FALSE;
		}
		pDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("EmbeddedFile"));
	}

	FX_DWORD fileSize = (FX_DWORD)pFileStream->GetSize();
	CPDF_Stream* pStream = FX_NEW CPDF_Stream(pFileStream, NULL, 0, fileSize, pDict, 0);
	if (!pStream) {
		pFileStream->Release();
		return FALSE;
	}
	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)m_pDoc)->getPDFDocument();
	pPDFDoc->AddIndirectObject(pStream);
	pEFDict->SetAtReference(FX_BSTRC("F"), pPDFDoc, pStream);

	CPDF_Dictionary* pParam = pDict->GetDict(FX_BSTRC("Params"));
	if (!pParam) {
		pParam = FX_NEW CPDF_Dictionary;
		if (!pParam) {
			pFileStream->Release();
			return FALSE;
		}
		pDict->SetAt(FX_BSTRC("Params"), pParam);
	}
	pParam->SetAtInteger(FX_BSTRC("Size"), fileSize);

	return TRUE;

}
FS_BOOL FSFileSpecImp::isEmbedded()
{
	if (!m_pAttachmentDict) 
		return FALSE;
	if (m_pAttachmentDict->GetType() != PDFOBJ_DICTIONARY) 
		return FALSE;

	CPDF_Dictionary* pEFDict = m_pAttachmentDict->GetDict(FX_BSTRC("EF"));
	if (!pEFDict) 
		return FSCRT_ERRCODE_SUCCESS;

	CPDF_Stream* pStream = pEFDict->GetStream(FX_BSTRC("F"));
	return pStream != NULL;
}
FSString FSFileSpecImp::getDescription()
{
	if (!m_pAttachmentDict) 
		return FSString();
	CFX_ByteString bsDescription = m_pAttachmentDict->GetString(FX_BSTRC("Desc"));
	CFX_ByteString utf8 = FSCRT_UTF8_FromTextString(bsDescription);
	return FSString(utf8);
}
void FSFileSpecImp::setDescription(const char* description)
{
	if (!m_pAttachmentDict) 
		return;
	CFX_ByteString utf8(description);
	CFX_ByteString textString = FSCRT_UTF8_ToTextString(utf8);
	m_pAttachmentDict->SetAtString(FX_BSTRC("Desc"), textString);
}
FSDateTime FSFileSpecImp::getCreationDateTime()
{
	FSDateTime fsDateTime = {0};
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS) return fsDateTime;

	CFX_ByteString bsCreationDate = pParamDict->GetString(FX_BSTRC("CreationDate"));

	CFSPDF_DateTime dtTime;
	if (dtTime.ParserPDFDateTimeString(bsCreationDate))
	{
		fsDateTime = dtTime.ToSystemTime();
		fsDateTime.UTHourOffset = dtTime.m_fsDatetime.UTHourOffset;
		fsDateTime.UTMinuteOffset = dtTime.m_fsDatetime.UTMinuteOffset;
	}

	return fsDateTime;
}
void FSFileSpecImp::setCreationDateTime(const FSDateTime* dateTime)
{
	if (!dateTime || !FSCRT_Util_CheckDateTime(*dateTime)) 
		return;
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS || pParamDict == NULL) 
		return;

	CFSPDF_DateTime dt(*dateTime);
	CFX_ByteString strDateTime = dt.ToPDFDateTimeString();

	pParamDict->SetAtString(FX_BSTRC("CreationDate"), strDateTime);
}
FSDateTime FSFileSpecImp::getModifiedDateTime()
{
	FSDateTime fsDateTime = {0};
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS) 
		return fsDateTime;

	CFX_ByteString bsCreationDate = pParamDict->GetString(FX_BSTRC("ModDate"));

	CFSPDF_DateTime dtTime;
	if (dtTime.ParserPDFDateTimeString(bsCreationDate))
	{
		fsDateTime = dtTime.ToSystemTime();
		fsDateTime.UTHourOffset = dtTime.m_fsDatetime.UTHourOffset;
		fsDateTime.UTMinuteOffset = dtTime.m_fsDatetime.UTMinuteOffset;
	}

	return fsDateTime;
}
void FSFileSpecImp::setModifiedDateTime(const FSDateTime* dateTime)
{
	if (!dateTime ||!FSCRT_Util_CheckDateTime(*dateTime)) 
		return;
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS || pParamDict == NULL) 
		return;

	CFSPDF_DateTime dt(*dateTime);
	CFX_ByteString strDateTime = dt.ToPDFDateTimeString();

	pParamDict->SetAtString(FX_BSTRC("ModDate"), strDateTime);
}
FSString FSFileSpecImp::getChecksum()
{
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS || pParamDict==NULL) 
		return FSString();

	CFX_ByteString bsCheckSum = pParamDict->GetString(FX_BSTRC("CheckSum"));
	return FSString(bsCheckSum, bsCheckSum.GetLength(), (FS_TEXTENCODING)0);
}
void FSFileSpecImp::setChecksum(const char* checksum)
{
	CPDF_Dictionary* pParamDict = NULL;
	FS_RESULT ret = getParamDict(pParamDict);
	if (ret != FSCRT_ERRCODE_SUCCESS || pParamDict==NULL) 
		return;

	pParamDict->SetAtString(FX_BSTRC("CheckSum"), CFX_ByteString(checksum));	
}
