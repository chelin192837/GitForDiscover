#ifndef _FS_OFDLICENSEMANAGER_H_
#define _FS_OFDLICENSEMANAGER_H_

#define _FS_NONE_			0
#define _FS_GSDK_			1
#define _FS_ASDK_			2
#define _FS_MSDK_			3
#define _FS_EISDK_			4
#define _FS_ESSDK_			5
#define _FS_IMGSDK_			6
#define _FS_QTSDK_			7
#define _FS_CONVERTOR_		8
#define _FS_EOSDK_			9

//#ifndef _FX_PRODUCT_
//#define _FX_PRODUCT_ _FS_GSDK_
//#endif

#define _FS_PRODUCTNAME_NONE_		L"Foxit OFD None"
#define _FS_PRODUCTNAME_GSDK_		L"Foxit OFD GSDK"
#define _FS_PRODUCTNAME_ASDK_		L"Foxit OFD ASDK"
#define _FS_PRODUCTNAME_MSDK_		L"Foxit OFD MSDK"
#define _FS_PRODUCTNAME_EISDK_		L"Foxit OFD EISDK"
#define _FS_PRODUCTNAME_ESSDK_		L"Foxit OFD ESSDK"
#define _FS_PRODUCTNAME_IMGSDK_		L"Foxit OFD IMGSDK"
#define _FS_PRODUCTNAME_QTSDK_		L"Foxit OFD QTSDK"
#define _FS_PRODUCTNAME_CONVERTOR_	L"Foxit OFD Convertor"
#define _FS_PRODUCTNAME_EOSDK_		L"Foxit OFD EOSDK"

#define _FS_PRODUCTAB_NONE_			L"SN"
#define _FS_PRODUCTAB_GSDK_			L"SG"
#define _FS_PRODUCTAB_ASDK_			L"SA"
#define _FS_PRODUCTAB_MSDK_			L"SM"
#define _FS_PRODUCTAB_EISDK_		L"SF"
#define _FS_PRODUCTAB_ESSDK_		L"SE"
#define _FS_PRODUCTAB_IMGSDK_		L"SI"
#define _FS_PRODUCTAB_QTSDK_		L"SQ"
#define _FS_PRODUCTAB_CONVERTOR_	L"SC"
#define _FS_PRODUCTAB_CREATERSDK_	L"SR"
#define _FS_PRODUCTAB_EOSDK_		L"SO"

#if _FX_PRODUCT_ == _FS_GSDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_GSDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_GSDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	5
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsgkey.txt"
#elif _FX_PRODUCT_ == _FS_ASDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_ASDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_ASDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	4
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsakey.txt"
#elif _FX_PRODUCT_ == _FS_MSDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_MSDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_MSDK_
	#define _FS_VERSION_MAJOR	2
	#define _FS_VERSION_INTER	0
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsmkey.txt"
#elif _FX_PRODUCT_ == _FS_EISDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_EISDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_EISDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	1
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsfkey.txt"
#elif _FX_PRODUCT_ == _FS_ESSDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_ESSDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_ESSDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	2
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsekey.txt"
#elif _FX_PRODUCT_ == _FS_IMGSDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_IMGSDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_IMGSDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	1
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsikey.txt"
#elif _FX_PRODUCT_ == _FS_QTSDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_QTSDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_QTSDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	2
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsqkey.txt"
#elif _FX_PRODUCT_ == _FS_CONVERTOR_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_CONVERTOR_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_CONVERTOR_
	#define _FS_VERSION_MAJOR	2
	#define _FS_VERSION_INTER	0
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsckey.txt"
#elif _FX_PRODUCT_ == _FS_EOSDK_
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_EOSDK_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_EOSDK_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	0
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsokey.txt"
#else
	#define _FS_PRODUCTNAME_	_FS_PRODUCTNAME_NONE_
	#define _FS_PRODUCTAB_		_FS_PRODUCTAB_NONE_
	#define _FS_VERSION_MAJOR	1
	#define _FS_VERSION_INTER	0
	#define _FS_VERSION_MINOR	0
	#define _FS_KEYFILE_		L"fsnkey.txt"
#endif

#define _FS_CPU_X86_		L'8'
#define _FS_CPU_X64_		L'6'
#define _FS_CPU_MIPS_		L'1'
#define _FS_CPU_SPARC_		L'2'
#define _FS_CPU_ARM_		L'3'

#if _FX_CPU_ == _FX_X86_
	#define _FS_CPU_	_FS_CPU_X86_
	#define _FS_OS_PLATFORM_	L"X86"
#elif _FX_CPU_ == _FX_X64_
	#define _FS_CPU_	_FS_CPU_X64_
	#define _FS_OS_PLATFORM_	L"X64"
#elif _FX_CPU_ == _FX_MIPS_
	#define _FS_CPU_	_FS_CPU_MIPS_
	#define _FS_OS_PLATFORM_	L"MIPS"
#elif _FX_CPU_ == _FX_SPARC_
	#define _FS_CPU_	_FS_CPU_SPARC_
	#define _FS_OS_PLATFORM_	L"SPARC"
#elif (_FX_CPU_ == _FX_ARM_) || (_FX_CPU_ == _FX_ARM64_)
	#define _FS_CPU_	_FS_CPU_ARM_
	#define _FS_OS_PLATFORM_	L"ARM"
#endif

#define _FS_OS_WINDOWS_		L'W'
#define _FS_OS_LINUX_		L'L'
#define _FS_OS_NONE_		L'N'

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	#define _FS_OS_		_FS_OS_WINDOWS_
	#define _FS_OSNAME_	L"Windows"
#elif _FX_OS_ == _FX_LINUX_DESKTOP_
	#define _FS_OS_		_FS_OS_LINUX_
	#define _FS_OSNAME_	L"Linux"
#else
	#define _FS_OS_		L"Unknown"
	#define _FS_OSNAME_	L"Unknown"
#endif

#define FOFDSDK_TRIAL			L"Trial"

// Common
#define FOFDMODULE_STD			L"FOFDStandard"

// GSDK
#define FOFDMODULE_EDIT			(FX_LPCWSTR)L"FOFDEdit"
#define FOFDMODULE_ANNOT		(FX_LPCWSTR)L"FOFDAnnot"
#define FOFDMODULE_PAGEMANAGER	(FX_LPCWSTR)L"FOFDPageManager"
#define FOFDMODULE_WATERMARK	(FX_LPCWSTR)L"FOFDWatermark"
#define FOFDMODULE_TEXTPAGE		(FX_LPCWSTR)L"FOFDTextPage"
#define FOFDMODULE_CUSTOMTAG	(FX_LPCWSTR)L"FOFDCustomTag"
#define FOFDMODULE_OFFICETREE	(FX_LPCWSTR)L"FOFDOfficeTree"
#define FOFDMODULE_SECURITY		(FX_LPCWSTR)L"FOFDSecurity"
#define FOFDMODULE_BARCODE		(FX_LPCWSTR)L"FOFDBarcode"

// Other SDK
#define FOFDMODULE_OCR			(FX_LPCWSTR)L"FOFDOCR"
#define FOFDMODULE_MRC			(FX_LPCWSTR)L"FOFDMRC"
#define FOFDMODULE_ADV			(FX_LPCWSTR)L"FOFDAdvance"


typedef struct _OFD_RESTRICTION_CONTENT_
{
	CFX_WideString		nVerNumber;

	int					nVer_Major;
	int					nVer_Inter;
	int					nVer_Minor;

	int					nKeyverNumber;
	CFX_WideString		wsEdition;
	CFX_WideString		wsLanguage;
	CFX_WideString		wsMd5;
} OFD_RESTRICTION_CONTENT;

typedef struct _OFD_LICENSEDATA_
{
	FX_BOOL			bLicense;
	FX_BOOL			bTrial;
	CFX_WideString	wsSN;				/**< 注册SN					*/
	CFX_WideString	wsModule;
	CFX_WideString	wsUsers;			/**< 注册的用户名允许数量	*/
	CFX_WideString	wsLicense;			/**< 注册的授权许可			*/
	CFX_WideString	wsExpireDate;		/**< 注册的期限				*/
	CFX_WideString	wsLicenseDate;		/**< 注册时间				*/
	CFX_WideString	wsRestrictions;
	CFX_WideString	wsSignature;

	OFD_RESTRICTION_CONTENT restriction;
} OFD_LICENSEDATA;

class CFS_OFDLicenseManager : public CFX_Object
{
public:
	CFS_OFDLicenseManager();
	virtual ~CFS_OFDLicenseManager();

	//check application license
	virtual FX_INT32				CheckLicense(const CFX_WideString& wsSerialnum, const CFX_WideString& wsPath);
	virtual OFD_LICENSEDATA*		GetLicenseData();
	//check module license
	virtual FX_BOOL					CheckModuleLicense(const CFX_WideString& strModuleName);
	virtual FX_BOOL					IsTrial();

protected:
	FX_INT32	CheckSerialnum(const CFX_WideString& wsSerialnum);
	FX_INT32	CheckLicenseFile(IFX_FileRead* pKeyFile);
	FX_INT32	ActiveLicense(const CFX_WideString& wsSerialnum, CFX_WideString& file);

	CFX_WideString GetLicenseFile(const CFX_WideString& wsLicensePath);

	FX_BOOL ParseLicenseFile(IFX_FileRead* pKeyFile);
	FX_BOOL CheckTime();
	FX_BOOL ParseRestrictionContent(const CFX_WideString& wsRestriction, OFD_RESTRICTION_CONTENT& restrictionContent);
	CFX_WideString GetProductName();	
	FX_BOOL CheckVerNum();
	FX_BOOL CheckMacAddr();
	CFX_WideString GetMacAddr();
//	CFX_WideString GetHostIP();
	CFX_WideString GetHardDiskID();

	CFX_WideString GetXMLPostData(const CFX_WideString& wsSerialnum);
	CFX_WideString TranslationXML(const CFX_WideString& wsData);
	CFX_ByteString TranslationXML(const CFX_ByteString& bsData);
	FX_BOOL GetEncryptKey(CFX_ByteString& key, CFX_ByteString& sessionID);
	CFX_ByteString EncryptData_Foxit_AES(const CFX_ByteString& key, const CFX_ByteString& plain, FX_BOOL bUTF8 = TRUE);
	CFX_ByteString DecryptData_Foxit_AES(const CFX_ByteString& key, const CFX_ByteString& encrypted, FX_BOOL bUTF8 = TRUE);
	FX_BOOL PostData(FX_LPCWSTR ServerName, FX_LPCWSTR objectName, const CFX_ByteString& csPostData, CFX_ByteString &csGetReceive, FX_BOOL bCloseSession);
	int ParseServerXML(FX_LPBYTE pData, FX_DWORD dwDataLen, CFX_WideString &csCode, CFX_WideString &wsMessage, 
		CFX_WideString &csActiveCount, CFX_ByteString &csKey, const CFX_ByteString& key);

	void ClearLicenseData();

	OFD_LICENSEDATA m_licenseData;

public:
	char* m_rev;
	unsigned long m_rev_len;
};

FX_BOOL FS_CheckModuleLicense(FX_LPCWSTR module_name);

class CFS_OFDPage;
class CFX_DIBitmap;
void FS_AddCopyrightInfo(CFS_OFDPage*);
void FS_AddCopyrightInfo(CFX_DIBitmap*);

#endif //_FS_OFDLICENSEMANAGER_H_
