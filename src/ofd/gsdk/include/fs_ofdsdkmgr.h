#ifndef _FS_OFDSDKMGR_H_
#define _FS_OFDSDKMGR_H_

#if _FX_OS_ == _FX_LINUX_DESKTOP_
#include<pthread.h>
#endif

class CFS_OFDFontMapper;

class CFS_OFDSDKMgr;
class CFS_OFDDocument;
class CFS_OFDLicenseManager;
class CFS_SecurityData;
class CFS_OFDSignatureManage;

#define OFD_LF_FACESIZE	32

typedef struct _OFD_LOGFONT
{
	FX_INT32	lfHeight;
	FX_INT32	lfWidth;
	FX_INT32	lfEscapement;
	FX_INT32	lfOrientation;
	FX_INT32	lfWeight;
	FX_BYTE		lfItalic;
	FX_BYTE		lfUnderline;
	FX_BYTE		lfStrikeOut;
	FX_BYTE		lfCharSet;
	FX_BYTE		lfOutPrecision;
	FX_BYTE		lfClipPrecision;
	FX_BYTE		lfQuality;
	FX_BYTE		lfPitchAndFamily;
	wchar_t		lfFaceName[OFD_LF_FACESIZE];
} OFD_LOGFONT;

class CFS_OFDFontMgr : public CFX_Object
{
public:
	CFS_OFDFontMgr();
	virtual ~CFS_OFDFontMgr();

	CFX_Font*	GetFontByName(const CFX_WideString& wsFontName);
	CFX_WideString	MatchFontName_ByFamilyName(const CFX_ByteString& bsFamilyName);

protected:
	CFX_Font* MatchFontByName(const CFX_WideString& wsFontName);
	CFX_Font* MatchFontByName2(const CFX_WideString& wsFontName);
	CFX_MapPtrTemplate<OFD_LOGFONT*, CFX_Font*> m_Log2Font;
	FX_INT32 m_iFontCount;
	void** m_pFontTable;
};

#define OFDSDK_MODE_COMMON	0x00
#define OFDSDK_MODE_EISDK	0x01

class CFS_OFDSDKMgr
{
public:
	static FX_BOOL		Create();
	static void			Destory();
	static CFS_OFDSDKMgr*	Get();

	FX_INT32 CheckLicense(const CFX_WideString& wsSerialnum, const CFX_WideString& wsPath);
	FX_BOOL	CheckModuleLicense(const CFX_WideString& strModuleName);
	FX_BOOL NeedAddCopyright();

//	CFX_DIBitmap* LoadCopyrightLogo();

	CFS_OFDFontMgr* GetFontMgr();
	void AddFontFileMapper(const CFX_WideString& fontName, const CFX_WideString& fontFile);

	void RegisterSecurityHandler(CFS_SecurityData* securityData);
	void UnregisterSecurityHandler(CFS_SecurityData* securityData);
	CFS_SecurityData* GetRegisterSecurityHandler(FX_LPCSTR securityType, FX_LPCSTR cryptoType, int keyLen);

	CFS_OFDSignatureManage* GetSignatureManageHandler();

	int InitWorkPath(const CFX_WideString& wsWorkPath);
	CFX_WideString GetWorkPath()	{ return m_wsWorkPath; }
	void* GetWorkPathDLLHandler(int key);
	CFX_WideString GetLicensePath()	{ return m_wsLicensePath; }
protected:
	CFS_OFDSDKMgr();
	~CFS_OFDSDKMgr();

	FX_BOOL Initialize();
	void Finalize();

private:
	CCodec_ModuleMgr* m_pCodecModule;

//	CFX_DIBitmap* m_pCopyrightImage;

	CFS_OFDFontMgr* m_fontMgr;
	CFS_OFDFontMapper* m_fontMapper;

	FX_INT32 m_sdkMode;

	CFS_OFDLicenseManager* m_licenseManager;
	FX_BOOL m_bAddCopyright;

	CFX_PtrArray m_securityHandler;

	CFS_OFDSignatureManage* m_SignatureManage;

	CFX_WideString m_wsWorkPath;
	CFX_MapPtrToPtr m_dllHandlers;
	CFX_WideString	m_wsLicensePath;
	
	FX_INT32 m_nCheckLicense;
public:
	CCodec_ModuleMgr* GetCodecModule();

	void Unlock(FX_LPSTR license_id, FX_LPSTR unlock_code);

	FX_INT32 GetMode();
	void SetMode(FX_INT32 mode);

	FX_BOOL IsEISDKMode();

public:
	FX_CRITICAL_SECTION		m_Lock;
};

class CFS_OFDSDKLock
{
public:
	CFS_OFDSDKLock();
	~CFS_OFDSDKLock();
};

#define FS_OFDSDKLOCK CFS_OFDSDKLock _fs_lock

#endif //_FS_OFDSDKMGR_H_
