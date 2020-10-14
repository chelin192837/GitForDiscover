#ifndef _FS_OFDCUSTOMCRYPT_H_
#define _FS_OFDCUSTOMCRYPT_H_

class COFD_Permissions;
class IFX_FileStream;
class COFD_CryptoDictionary;

class CFS_CustomCryptoHandler;
class CFS_CustomSecurityHandler;

static const FX_BYTE defIV[8] = {
	0x8a, 0xa9,
	0x75, 0x08,
	0xfa, 0xb6,
	0x2f, 0x4e
};

class CFS_SecurityData : public CFX_Object
{
public:
	CFX_ByteString securityType;
	CFX_ByteString cryptoType;
	int keyLen;
	OFD_SECURITYHANDLER securityHandler;
};

class CFS_CustomCryptoHandler : public IOFD_CryptoHandler
{
public:
	CFS_CustomCryptoHandler(CFS_CustomSecurityHandler* customSecurity,const CFX_ByteString& password);
	CFS_CustomCryptoHandler(CFS_CustomSecurityHandler* customSecurity,const CFX_ByteString& userKeyField, const CFX_ByteString& ownerKeyField);
	~CFS_CustomCryptoHandler();
	void	Release();
	FX_BOOL Init(FX_INT32 cipher, FX_LPCBYTE key, FX_INT32 keylen);

	FX_BOOL IsEncrypted(FX_LPCBYTE src_buf, FX_DWORD src_size);
	FX_BOOL IsEncrypted(IFX_FileStream* pFile);
	CFX_ByteString GetCryptoType();
	FX_BOOL EncryptData(FX_LPCBYTE src_buf, FX_DWORD src_size, FX_LPBYTE dest_buf, FX_DWORD& dest_size);
	FX_BOOL DecryptData(FX_LPCBYTE src_buf, FX_DWORD src_size, FX_LPBYTE dest_buf, FX_DWORD& dest_size);
	FX_LPVOID EnCryptStart();
	FX_LPVOID EnCryptStart(FX_INT32 cipher, FX_LPCBYTE key, FX_INT32 keylen);
	FX_DWORD EnCryptGetSize(FX_DWORD src_size);
	FX_BOOL	EnCryptStream(FX_LPVOID context, FX_LPCBYTE src_buf, FX_DWORD src_size, CFX_BinaryBuf& dest_buf);
	FX_BOOL EnCryptFinish(FX_LPVOID context, CFX_BinaryBuf& dest_buf);
	FX_BOOL	EnCryptStream(FX_LPVOID context, IFX_FileStream* pFromFile, IFX_FileStream* pToFile);
	FX_BOOL EnCryptFinish(FX_LPVOID context, IFX_FileStream* pToFile);
	FX_LPVOID DecryptStart();
	FX_LPVOID DecryptStart(FX_INT32 cipher, FX_LPCBYTE key, FX_INT32 keylen);
	FX_DWORD DecryptGetSize(FX_DWORD src_size);
	FX_BOOL	DecryptStream(FX_LPVOID context, FX_LPCBYTE src_buf, FX_DWORD src_size, CFX_BinaryBuf& dest_buf);
	FX_BOOL DecryptFinish(FX_LPVOID context, CFX_BinaryBuf& dest_buf);
	FX_BOOL	DecryptStream(FX_LPVOID context, IFX_FileStream* pFromFile, IFX_FileStream* pToFile);
	FX_BOOL DecryptFinish(FX_LPVOID context, IFX_FileStream* pToFile);

protected:
	CFS_CustomSecurityHandler* m_customSecurity;
	CFX_ByteString m_userKeyField;
	CFX_ByteString m_ownerKeyField;
	CFX_ByteString m_password;
};

class CFS_CustomSecurityHandler:public IOFD_SecurityHandler
{
public:
	CFS_CustomSecurityHandler(int document, CFS_SecurityData* securityData);
	~CFS_CustomSecurityHandler();
	
	void	Release();
	FX_BOOL Init(FX_LPCBYTE password, FX_DWORD pass_size, COFD_CryptoDictionary *pCrptoDict, const COFD_Permissions* pPermissions);
	FX_BOOL IsAdmin();
	FX_BOOL GetCryptInfo(FX_INT32& cipher, FX_LPCBYTE& key, int& keylen);
	FX_BOOL OnCreate(COFD_CryptoDictionary* pEncryptDict, const COFD_Permissions* pPermissions, FX_LPCBYTE user_pass, FX_DWORD user_size, FX_LPCBYTE admin_pass, FX_DWORD admin_size);
	FX_INT32  GetFilter();

protected:
	COFD_CryptoDictionary* m_pCrptoDict;
	const COFD_Permissions* m_pPermissions;
	CFS_SecurityData* m_securityData;
	int m_document;
	friend class CFS_CustomCryptoHandler;
};

#endif
