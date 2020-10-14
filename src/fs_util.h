#ifndef _FS_UTIL_H_
#define _FS_UTIL_H_

#include "../include/fs_annot.h"
#include "fxcore.h"
#include "../include/fs_render.h"

//Re-define macro defintions to fxcore enum FX_ProgressiveStatus.
//Because android cannot use enum value directly.
#ifdef _ANDROID_
#define FXCORE_PROGRESSIVESTATUS_READY				0
#define FXCORE_PROGRESSIVESTATUS_TOBECONTINUED		1
#define FXCORE_PROGRESSIVESTATUS_FOUND				2
#define FXCORE_PROGRESSIVESTATUS_NOTFOUND			3
#define FXCORE_PROGRESSIVESTATUS_FAILED				4
#define FXCORE_PROGRESSIVESTATUS_DONE				5
#else
#define FXCORE_PROGRESSIVESTATUS_READY				FX_ProgressiveStatus::Ready
#define FXCORE_PROGRESSIVESTATUS_TOBECONTINUED		FX_ProgressiveStatus::ToBeContinued
#define FXCORE_PROGRESSIVESTATUS_FOUND				FX_ProgressiveStatus::Found
#define FXCORE_PROGRESSIVESTATUS_NOTFOUND			FX_ProgressiveStatus::NotFound
#define FXCORE_PROGRESSIVESTATUS_FAILED				FX_ProgressiveStatus::Failed
#define FXCORE_PROGRESSIVESTATUS_DONE				FX_ProgressiveStatus::Done
#endif //_ANDROID_

/** @brief	Result code for functions in Foxit PDF SDK. */
typedef int						FS_RESULT;
typedef void*					FS_LPVOID;

#define	SAFE_DELETE(p)	if(p){\
	delete p;\
	p = NULL;}

class FSDestination;
class FSPDFDoc;
class FSPDFObject;
class CFS_OFDLicenseManager;//CFSCRT_LicenseRightMgr;
class CFSPDF_IconProviderManager;
class CFSCRT_Logger;
class FSSignatureImp;

#ifdef _FPDFAPI_MT_

#define FSCRT_MUTEX
class CFSCRT_Lock
{
public:
	FX_BOOL				TryLock();
	void				Lock();
	void				Unlock();

public:
	CFSCRT_Lock();
	~CFSCRT_Lock();

private:
#ifdef FSCRT_MUTEX
	FX_MUTEX					m_hLock;
#else
	FX_SPINLOCK					m_hSpinLock;
#endif
};
#endif


#ifdef _FPDFAPI_MT_
#define FSCRT_DEFINELOCK(lock)				CFSCRT_Lock lock;
#define FSCRT_DEFINELOCKPTR(ptrLock)		CFSCRT_Lock* ptrLock;
#define FSCRT_LOCKINIT(lock)				, lock()
#define FSCRT_TRYLOCK(lock)					lock.TryLock()
#define FSCRT_LOCK(lock)					lock.Lock();
#define FSCRT_UNLOCK(lock)					lock.Unlock();
#define FSCRT_TRYLOCKPTR(ptrLock)			ptrLock->TryLock()
#define FSCRT_LOCKPTR(ptrLock)				ptrLock->Lock();
#define FSCRT_UNLOCKPTR(ptrLock)			ptrLock->Unlock();
#define FSCRT_LOCKOBJECT(obj, lockObjPtr)	CFSCRT_LockObject obj(lockObjPtr);
#else
#define FSCRT_DEFINELOCK(lock) 
#define FSCRT_DEFINELOCKPTR(ptrLock) 
#define FSCRT_LOCKINIT(lock) 
#define FSCRT_TRYLOCK(lock) 1
#define FSCRT_LOCK(lock) 
#define FSCRT_UNLOCK(lock) 
#define FSCRT_TRYLOCKPTR(ptrLock) 
#define FSCRT_LOCKPTR(ptrLock) 
#define FSCRT_UNLOCKPTR(ptrLock) 
#define FSCRT_LOCKOBJECT(obj, lockObjPtr)
#endif

#ifdef _FPDFAPI_MT_
class CFSCRT_LockObject
{
public:
	CFSCRT_LockObject(CFSCRT_Lock *pObj);
	~CFSCRT_LockObject();

private:
	FSCRT_DEFINELOCKPTR(m_pObj)
};
#endif


/////////////////////////////////////////////////////////
//Internal class CFS_Library 
#define FSPDF_CONFIG_ID_CACHESIZE	0x01

class FR_OFDFontMapper;

// CFS_Library cannot inherit from CFX_Object since it contains a member which is the manager of fxcore.
class CFS_Library
{
public:
	static FS_ERRORCODE			create(const char* sn, const char* key);
	static CFS_Library*			get();
	static void					release();
	static CFSPDF_IconProviderManager*	getIconProviderMgr();
	static CFSCRT_Logger*				getLogger();
	FS_BOOL						setAnnotIconProvider(FSAnnotIconProvider* iconProvider);
	FS_BOOL						setLogFile(CFSCRT_FileStream* pFile);
	FS_BOOL						setNotifier(FSNotifier* notifier);
	FS_BOOL						setActionHandler(FSActionHandler* actionHandler);
	FSActionHandler*			getActionHandler();
	//See the macros FSPDF_CONFIG_ID_XXXX.
	FS_BOOL						getConfig(void* id, void*& lpValue);
	FS_BOOL						setConfig(void* id, void* value);

	FS_BOOL						registerDefaultSignatureHandler();
	FS_BOOL						registerSignatureHandler(const char* filter, const char* subFilter, FSSignatureHandler* signatureHandler);

	FS_ERRORCODE				addExtFont(const char* filepath);

	FS_ERRORCODE				CheckLicense(const char* sn, const char* key);
	FX_BOOL						CheckModuleLicense(const CFX_WideString& strModuleName);

protected:
	CFS_Library();
	~CFS_Library();
	FS_ERRORCODE	init(const char* sn, const char* key);
	FS_ERRORCODE	fxcoreInitialize();
	FS_ERRORCODE	fxcoreFinalize();
private:
	CFX_MapPtrToPtr			m_GlobalConfig;
	CFSPDF_IconProviderManager*	m_pIconProviderManager;
	CFSCRT_Logger*				m_pLogger;
	FSCRT_DEFINELOCK(m_lock);
	CFS_OFDLicenseManager*		m_pLicenseRightMgr;
	FSActionHandler*			m_pActionHandler;

	// Map <filter+subfilter, filter>, used to store registered signature handler name.
	// This is just used to store registered name and when release library, needs to get these handler and release them.
	CFX_MapByteStringToPtr	m_MapSigHandlerName;

	FR_OFDFontMapper*			m_pFontMapper;
};

static CFS_Library*	g_pLibrary = NULL;
/////////////////////////////////////////////////////////
//Internal error code
#define FSCRT_ERRCODE_SUCCESS				0
#define FSCRT_ERRCODE_SUBMAP_NOTEXIST		20
#define FSCRT_ERRCODE_SUBMODULE_NOTFOUND	21
#define FSCRT_ERRCODE_ERROR					-1
#define FSCRT_ERRCODE_INVALIDLICENSE		FSCRT_LICENSETYPE_INVALID
#define FSCRT_ERRCODE_PARAM					-2
#define FSCRT_ERRCODE_OUTOFMEMORY			-22
#define FSCRT_ERRCODE_FORMAT				-3
#define FSCRT_ERRCODE_NOTPARSED					-17

/////////////////////////////////////////////////////////

#define FLOAT_ZERO_CMP	1E-5

#define FLOAT_ISZERO(f)				((f) < 0.0001 && (f) > -0.0001)
#define FLOAT_ISSMALLER(fa,fb)		((fa) < (fb) && !FLOAT_ISZERO((fa) - (fb)))
#define FLOAT_ISBIGGER(fa,fb)		((fa) > (fb) && !FLOAT_ISZERO((fa) - (fb)))
#define FLOAT_ISEQUAL(fa,fb)		FLOAT_ISZERO((fa)-(fb))

#define CHECKVALID_INTRECT_PDF(intRect)				((intRect.right - intRect.left)>0 && (intRect.top - intRect.bottom)>0)
#define CHECKVALID_INTRECT_DEVICE(intRect)			((intRect.right - intRect.left)>0 && (intRect.bottom - intRect.top)>0)
#define CHECKVALID_FLOATRECT_PDF(floatRect)			((floatRect.right - floatRect.left) >FLOAT_ZERO_CMP && (floatRect.top - floatRect.bottom)>FLOAT_ZERO_CMP)
#define CHECKVALID_FLOATRECT_DEVICE(floatRect)		((floatRect.right - floatRect.left)>FLOAT_ZERO_CMP && (floatRect.bottom - floatRect.top)>FLOAT_ZERO_CMP)

#define FSCRT_ARGB_Encode(a,r,g,b)		((((FS_DWORD)(((FS_BYTE)(b) | ((FS_DWORD)((FS_BYTE)(g)) << 8)) | (((FS_DWORD)(FS_BYTE)(r)) << 16)))) | (((FS_DWORD)(FS_BYTE)(a)) << 24))
/** @brief	Used for get the A value from ARGB color format. */
#define FSCRT_ARGB_GetAValue(argb)		((FS_BYTE)(((FS_DWORD)(argb)) >> 24))
/** @brief	Used for get the R value from ARGB color format. */
#define FSCRT_ARGB_GetRValue(argb)		((FS_BYTE)(((FS_DWORD)(argb)) >> 16))
/** @brief	Used for get the G value from ARGB color format. */
#define FSCRT_ARGB_GetGValue(argb)		((FS_BYTE)(((FS_DWORD)(argb)) >> 8))
/** @brief	Used for get the B value from ARGB color format. */
#define FSCRT_ARGB_GetBValue(argb)		((FS_BYTE)(argb))


///////////////////////////////////////////////////////////////////////////
// UTF-8 related methods.
FX_BOOL FSCRT_Util_IsUTF8Data(FX_LPCBYTE pData, FX_DWORD *pLen, FX_DWORD *pPos);

FX_LPWSTR FSCRT_UTF8_DecodeToUnicode(const char* utf8Str);

CFX_ByteString FSCRT_UTF8_FromTextString(const CFX_ByteString bsText);
CFX_ByteString FSCRT_UTF8_ToTextString(const CFX_ByteString bsUtf8);

CFX_WideString FSCRT_UTF8_ToWideString(const char* utf8String);
CFX_WideString FSCRT_UTF8_ToWideString(const char utf8String[], FS_INT32 sLen);
CFX_ByteString FSCRT_UTF8_FromWideString(CFX_WideString wsStr);

///////////////////////////////////////////////////////////////////////////
//
IFX_FileStream* FSCRT_Util_CreateFileFromPath(const char* filePath, FX_DWORD dwModes);

//CFX_ByteString FSCRT_Util_ToPDFDateTimeString(FSDateTime dt);

FS_BOOL FSPDF_Util_CheckShouldRenderAnnot(CPDF_Annot* pAnnot);
FS_BOOL FSPDF_Util_CheckAnnotIsStandard(FSM_ANNOTTYPE bsType);
FS_BOOL FSPDF_Util_CheckAnnotIsStandard(CFX_ByteString bAnnotsType);

FSPointF FSPDF_Util_CFXPointF2FSPointF(const CFX_PointF& fxPoint);
CFX_PointF FSPDF_Util_FSPointF2CFXPointF(const FSPointF& fsPoint);

FS_BOOL FSCRT_Util_IsFloatRectEqual(CFX_FloatRect rect1, CFX_FloatRect rect2);
FS_BOOL FSCRT_Util_IsFloatPointEqual(FSPointF point1, FSPointF point2);
CPDF_Object* FSPDF_ST_CloneObject2Doc(CPDF_Object* pSrcObj , CPDF_IndirectObjects* pTargetDoc ,FX_BOOL bCloneAlways, CFX_MapPtrToPtr &MergeObjs, FX_DWORD& TargetDoclastObjNum);

typedef FX_INT64	FSPDF_UNITIME;

FS_BOOL		FSCRT_Util_IsStringEmpty(const char* inputStr);

#define WMANNOTTYPE_NO			0
#define WMANNOTTYPE_COMMON		1
#define WMANNOTTYPE_GSDKTRIAL	2
#define WMANNOTTYPE_RDKTRIAL	3

#define	FSDK_TRIALWATERMARK_GSDK	"FoxitRestriction"
#define	FSDK_TRIALWATERMARK_RDK		"FoxitRDKRestriction"

FS_INT32	FSCRT_Util_GetWatermarkAnnotType(CPDF_Dictionary* pAnnotDict);
FS_BOOL		FSCRT_Util_RDKTrialWatermarkExisted(CPDF_Page* pPage);


//These definitions will be used if current system does not have.
#ifndef ANSI_CHARSET

#define ANSI_CHARSET            0
#define DEFAULT_CHARSET         1
#define SYMBOL_CHARSET          2
#define SHIFTJIS_CHARSET        128
#define HANGEUL_CHARSET         129
#define HANGUL_CHARSET          129
#define GB2312_CHARSET          134
#define CHINESEBIG5_CHARSET     136
#define OEM_CHARSET             255
#define JOHAB_CHARSET           130
#define HEBREW_CHARSET          177
#define ARABIC_CHARSET          178
#define GREEK_CHARSET           161
#define TURKISH_CHARSET         162
#define VIETNAMESE_CHARSET      163
#define THAI_CHARSET            222
#define EASTEUROPE_CHARSET      238
#define RUSSIAN_CHARSET         204
#define BALTIC_CHARSET          186

#endif

// Unicode subrange
// fsUsb 
// A 128-bit Unicode subset bitfield (USB) identifying up to 126 Unicode subranges. Each bit, except the two most significant bits, represents a single subrange. The most significant bit is always 1 and identifies the bitfield as a font signature; the second most significant bit is reserved and must be 0. Unicode subranges are numbered in accordance with the ISO 10646 standard. For more information, see Unicode Subset Bitfields. 
// fsCsb 
// A 64-bit, code-page bitfield (CPB) that identifies a specific character set or code page. Code pages are in the lower 32 bits of this bitfield. The high 32 are used for non-Windows code pages. For more information, see Code-Page Bitfields. 

typedef struct _FSCRT_FONTUSB
{
	FX_DWORD	dwStartUnicode;
	FX_DWORD	dwEndUnicode;
	FX_WORD		wBitField;	// A 128-bit Unicode subset bitfield identifying up to 126 Unicode subranges.
	FX_WORD		wCodePage;
} FSCRT_FONTUSB;

typedef struct _FSCRT_CHARSETMAP
{
	FS_DWORD	dwCharset;
	FS_INT32	nCodePage;
} FSCRT_CHARSETMAP;

class FSCRT_UnicodeMapper
{
public:
	static FS_INT32		GetCodePageFromUnicode(FS_DWORD unicode);
	//copy from phantom to fix some bug.
	static FS_DWORD		GetCharsetFromUnicode(FS_DWORD unicode);

	static FS_DWORD		GetCharsetFromCodePage(FS_INT32 CodePage);
	static FS_INT32		GetCodePageFromCharset(FS_DWORD charset);
	static FS_INT32		GetSysACP();

};
//*****************************************************************************
//* Datetime
//*****************************************************************************
#define FSCRT_CHAR_NotDigit(ch) (ch < '0' || ch > '9')
#define FSCRT_CHAR_TimeZoom(ch) (ch == '+' || ch == '-' || ch == 'Z')

class CFSPDF_DateTime : public CFX_Object
{
public:
	CFSPDF_DateTime();
	//CFSPDF_DateTime(const CFX_ByteString& dtStr);
	CFSPDF_DateTime(const CFSPDF_DateTime& datetime);
	CFSPDF_DateTime(const FSDateTime& firstPageNumber);
	
	CFSPDF_DateTime&	operator = (const CFSPDF_DateTime& datetime);
	CFSPDF_DateTime&	operator = (const FSDateTime& firstPageNumber);

	FS_BOOL				operator == (CFSPDF_DateTime& datetime);
	FS_BOOL				operator != (CFSPDF_DateTime& datetime);
	FS_BOOL				operator > (CFSPDF_DateTime& datetime);
	FS_BOOL				operator >= (CFSPDF_DateTime& datetime);
	FS_BOOL				operator < (CFSPDF_DateTime& datetime);
	FS_BOOL				operator <= (CFSPDF_DateTime& datetime);
	
	operator			FSPDF_UNITIME();

	FS_BOOL					ParserPDFDateTimeString(const CFX_ByteString& dtStr);
	FX_BOOL					ParserPDFXMPDateTimeString(const CFX_WideString& dtStr);
	CFX_ByteString			ToCommonDateTimeString();
	CFX_ByteString			ToPDFDateTimeString();
	CFX_WideString			ToXMPDateTimeString();
	FSDateTime				ToSystemTime();
	CFSPDF_DateTime			ToGMT();

	void					SetCurrentSysTime();

protected:
	void					Empty();

	FS_BOOL					ParserYear(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue);
	FS_BOOL					ParserMonth(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue);
	FS_BOOL					ParserDay(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue);
	FS_BOOL					ParserHour(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue);
	FS_BOOL					ParserMinute(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue);
	FS_BOOL					ParserSecond(const CFX_ByteString& dtStr, FS_INT32 &index, FS_BOOL &bContinue);
	FS_BOOL					ParserTimeZone(const CFX_ByteString& dtStr, FS_INT32 &index);
	FS_BOOL					ParserXMPTimeZone(const CFX_ByteString& dtStr, FS_INT32 &index);

	FS_BOOL					AddDays(short days);
	FS_BOOL					AddSeconds(int seconds);

public:
	FSDateTime			m_fsDatetime;
protected:
	FSCRT_DEFINELOCK(m_lock);
	long				m_timezone;
};

FS_BOOL FSCRT_Util_CheckDateTime(const FSDateTime dt);
FS_DWORD FSCRT_PointType_RDK2Fxcore(FS_PATHPOINTTYPE pointType);
FS_PATHPOINTTYPE FSCRT_PointType_Fxcore2RDK(FS_DWORD pointFlag);

FSBitmap*	FSCRT_Util_ConvertDIBSource2FSBitmap(CFX_DIBSource* pDIBSrc);

#endif