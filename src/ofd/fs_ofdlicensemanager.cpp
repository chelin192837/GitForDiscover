#include "fxlib.h"
#include "include/fs_ofdutils.h"
#include "include/fs_ofdsdkmgr.h"
#include "include/fs_ofdlicensemanager.h"

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_
	#include <iphlpapi.h>
	#include <Winhttp.h>
#else
	#include <sys/ioctl.h>
	#include <sys/socket.h>
	#include <linux/hdreg.h>
	#include <netinet/in.h>
	#include <net/if.h>
	#include <errno.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <ctype.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/param.h>
	#include <net/if_arp.h>

	#include <unistd.h>
	#include <sys/stat.h>
	#include <scsi/sg.h>
	#include <linux/types.h>
	#ifdef SOLARIS
		#include <sys/sockio.h>
	#endif
#endif

#define FS_PRODUCTVERSION_BASE		'A'

#if 1
	#define	HTFoxit_LicenseServer		L"pmo.htfoxit.com"
	#define	HTFoxit_LicenseServer2		"pmo.htfoxit.com"
	#define	HTFoxit_LicenseHost		"Host: 10.6.8.164\n"
#else
	#define	HTFoxit_LicenseServer		L"http://10.6.8.164/license.php"
#endif
	#define	HTFoxit_LicenseObjectName	L"license.php"

#define FS_PUBLIC_KEY_BASE64 "AAAAgM3WJOqZ0BoaX9zkDEeg0sYXVt2rmkXQFF+90SHgmWTG9FVyTdOK+en/55F1I4dXTaPV3u/tdzFJ1ode2KlDIsFKy5EGJNvmIOU3A8MqmkSgeBuY0DV7W4cav3K16qORd+S61FRfc7fWLE5HgPpyKCStFvg9cVhJa3i7PSdV42H3AAAAARE="

#define FS_AES_KEY_ENCODEMACADDR "kR5Cek1reHMCe7vP"

//
FX_BOOL FS_CheckModuleLicense(FX_LPCWSTR module_name)
{
#ifdef _FS_LICENSED_BUILD_
	CFX_WideString wsModule;
#if _FX_PRODUCT_ == _FS_GSDK_
	wsModule = CFX_WideString(module_name);
#endif
	return CFS_OFDSDKMgr::Get()->CheckModuleLicense(wsModule);
#else
	return FS_IsValidDate();
#endif
}

//
CFX_WideString FS_ProductAB2FullName(const CFX_WideString& abName)
{
	if (abName.Equal(CFX_WideString(_FS_PRODUCTAB_GSDK_)))
		return _FS_PRODUCTNAME_GSDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_ASDK_))
		return _FS_PRODUCTNAME_ASDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_MSDK_))
		return _FS_PRODUCTNAME_MSDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_EISDK_))
		return _FS_PRODUCTNAME_EISDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_ESSDK_))
		return _FS_PRODUCTNAME_ESSDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_IMGSDK_))
		return _FS_PRODUCTNAME_IMGSDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_QTSDK_))
		return _FS_PRODUCTNAME_QTSDK_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_CONVERTOR_))
		return _FS_PRODUCTNAME_CONVERTOR_;
	else if (abName.Equal((FX_LPCWSTR)_FS_PRODUCTAB_EOSDK_))
		return _FS_PRODUCTNAME_EOSDK_;
	else
		return L"";
}

//
CFS_OFDLicenseManager::CFS_OFDLicenseManager()
{
	ClearLicenseData();
}

//
CFS_OFDLicenseManager::~CFS_OFDLicenseManager()
{

}

//
FX_INT32 CFS_OFDLicenseManager::CheckLicense(const CFX_WideString& wsSerialnum, const CFX_WideString& wsPath)
{
#ifdef _FS_LICENSED_BUILD_
	int ret = -1;
	ClearLicenseData();

	CFX_WideString wsLicenseFile = GetLicenseFile(wsPath);
	IFX_FileRead* pKeyFile = FX_CreateFileRead(wsLicenseFile);
	if (!pKeyFile)
	{
		ret = CheckSerialnum(wsSerialnum);
		if (ret != 0)
			return ret == OFD_LICENSE_SERIALEXPIRED ? OFD_LICENSE_SERIALEXPIRED : OFD_LICENSE_SERIAL;

		ret = ActiveLicense(wsSerialnum, wsLicenseFile);
		if (ret != 0)
			return ret;

		pKeyFile = FX_CreateFileRead(wsLicenseFile);
		if (!pKeyFile)
			return OFD_LICENSE_WRITE;
	}
	
	ret = CheckLicenseFile(pKeyFile);
	pKeyFile->Release();

	return ret;
#else
	return 0;
#endif
}

//
OFD_LICENSEDATA* CFS_OFDLicenseManager::GetLicenseData()
{
	return &m_licenseData;
}

//
FX_BOOL	CFS_OFDLicenseManager::CheckModuleLicense(const CFX_WideString& strModuleName)
{
	if (!m_licenseData.bLicense)
		return FALSE;

	if (!strModuleName.IsEmpty())
		return m_licenseData.wsModule.Find((FX_LPCWSTR)strModuleName) > -1;

	return TRUE;
}

//
FX_BOOL	CFS_OFDLicenseManager::IsTrial()
{
	return m_licenseData.bTrial;
}

#define FS_SERIALNUM_SPLIT	L'-'

#define FS_CHAR_IsNUM(c) (c >= '0' && c <= '9')

//
FX_INT32 CFS_OFDLicenseManager::CheckSerialnum(const CFX_WideString& wsSerialnum)
{
	if (wsSerialnum.GetLength() != 29)
		return -1;

	if (wsSerialnum[5] != FS_SERIALNUM_SPLIT || wsSerialnum[11] != FS_SERIALNUM_SPLIT
		|| wsSerialnum[17] != FS_SERIALNUM_SPLIT || wsSerialnum[23] != FS_SERIALNUM_SPLIT)
		return -1;

	if (wsSerialnum[0] != L'F')// Foixt
		return -1;

	// Product AB name
	CFX_WideString wsABName;
	wsABName.Insert(0, wsSerialnum[1]);
	wsABName.Insert(1, wsSerialnum[2]);
	CFX_WideString wsFullName = FS_ProductAB2FullName(wsABName);
	CFX_WideString wsProductName = GetProductName();
	if (!wsProductName.Equal(wsFullName))
		return -1;

	// Major version num
	int version_major = wsSerialnum[3] - L'A';
	if (_FS_VERSION_MAJOR != version_major)
		return -1;

	// OS/CPU
	FX_WCHAR os = _FS_OS_;
	FX_WCHAR cpu = _FS_CPU_;
	if (wsSerialnum[6] != os || wsSerialnum[7] != cpu)
		return -1;

	// Active date
	CFX_WideString wsLisenceGenDate;
	if (FS_CHAR_IsNUM(wsSerialnum[9]) && FS_CHAR_IsNUM(wsSerialnum[10]) &&
		FS_CHAR_IsNUM(wsSerialnum[12]) && FS_CHAR_IsNUM(wsSerialnum[13])) {
		wsLisenceGenDate.Insert(0, wsSerialnum[9]);
		wsLisenceGenDate.Insert(1, wsSerialnum[10]);
		wsLisenceGenDate.Insert(2, wsSerialnum[12]);
		wsLisenceGenDate.Insert(3, wsSerialnum[13]);
	} else
		return -1;

	CFX_WideString wsActiveMonth;
	if (FS_CHAR_IsNUM(wsSerialnum[14]))
		wsActiveMonth.Insert(0, wsSerialnum[14]);
	else
		return -1;

	int days = wsLisenceGenDate.GetInteger() + wsActiveMonth.GetInteger() * 30;

	short year;
	unsigned char month, day, hour, minute, second;
	FS_GetLocalTime(year, month, day, hour, minute, second);
	year -= 2016;
	month --;
	if (year * 365 + month * 30 + day > days)
		return OFD_LICENSE_SERIALEXPIRED;

	CFX_WideString checkSum;
	checkSum.Insert(0, wsSerialnum[24]);
	checkSum.Insert(1, wsSerialnum[25]);
	checkSum.Insert(2, wsSerialnum[26]);
	checkSum.Insert(3, wsSerialnum[27]);
	checkSum.Insert(4, wsSerialnum[28]);

	CFX_WideString wsHash = wsSerialnum.Mid(0, 5);
	wsHash += wsSerialnum.Mid(6, 5);
	wsHash += wsSerialnum.Mid(12, 5);
	wsHash += wsSerialnum.Mid(18, 5);
	CFX_ByteString bsHash = wsHash.UTF8Encode();

	unsigned char digest[16] = {0};
	CRYPT_MD5Generate((FX_LPCBYTE )bsHash, bsHash.GetLength(), digest);
	CFX_ByteString bsTemp;
	CFX_WideString wsTemp;
	for (int i = 0; i < 16; i ++)
	{
		FX_BYTE b = digest[i];
		CFX_WideString bsTemp1;	bsTemp1.Format((FX_LPCWSTR)L"%X", (b & 0xF0) >> 4);
		wsTemp.Insert(i * 2, bsTemp1[0]);
		CFX_WideString bsTemp2;	bsTemp2.Format((FX_LPCWSTR)L"%X", b & 0x0F);
		wsTemp.Insert(i * 2 + 1, bsTemp2[0]);
	}

	CFX_WideString wsHashCode;
	wsHashCode.Insert(0, wsTemp[4]);
	wsHashCode.Insert(1, wsTemp[12]);
	wsHashCode.Insert(2, wsTemp[17]);
	wsHashCode.Insert(3, wsTemp[23]);
	wsHashCode.Insert(4, wsTemp[29]);

	int ret = wsHashCode.CompareNoCase(checkSum);
	return ret == 0 ? 0 : -1;
}

static FX_BOOL FS_CheckLicenseInfo(const CFX_WideString& wsSN, const CFX_WideString& wsModules, const CFX_WideString& wsUsers,
	const CFX_WideString& wsLicensee, const CFX_WideString& wsExpireDate, const CFX_WideString& wsLicenseDate,
	const CFX_WideString& wsRestrictions, const CFX_WideString& wsSignature)
{
	CFX_Base64Decoder base64Decoder;

	CFX_ByteString bsSign_Base64 = wsSignature.UTF8Encode();
	CFX_ByteString bsSign;
	base64Decoder.Decode(bsSign_Base64, bsSign);

	CFX_ByteString bsPubKey_Base64(FS_PUBLIC_KEY_BASE64);
	CFX_ByteString bsPubKey;
	base64Decoder.Decode(bsPubKey_Base64, bsPubKey);

	CFX_ByteString data_block;
	data_block += CFX_WideString(wsSN).UTF8Encode();
	data_block += CFX_WideString(wsModules).UTF8Encode();
	data_block += CFX_WideString(wsUsers).UTF8Encode();
	data_block += CFX_WideString(wsLicensee).UTF8Encode();
	data_block += CFX_WideString(wsExpireDate).UTF8Encode();
	data_block += CFX_WideString(wsLicenseDate).UTF8Encode();
	data_block += CFX_WideString(wsRestrictions).UTF8Encode();

	FX_BOOL bRet = FXPKI_VerifyRsaSign(data_block ,bsSign, bsPubKey);
	
	return bRet;
}

//
FX_INT32 CFS_OFDLicenseManager::CheckLicenseFile(IFX_FileRead* pKeyFile)
{
	if (!ParseLicenseFile(pKeyFile))
		return -1;

	FX_BOOL bValid = FS_CheckLicenseInfo(m_licenseData.wsSN, m_licenseData.wsModule, m_licenseData.wsUsers
		, m_licenseData.wsLicense, m_licenseData.wsExpireDate, m_licenseData.wsLicenseDate
		, m_licenseData.wsRestrictions, m_licenseData.wsSignature);

	m_licenseData.bLicense = bValid;
	if (!m_licenseData.bLicense)
		return OFD_LICENSE_FILE;

	if (!CheckTime())
	{
		m_licenseData.bLicense = FALSE;
		return OFD_LICENSE_EXPIRED;
	}

	if (ParseRestrictionContent(m_licenseData.wsRestrictions, m_licenseData.restriction) == FALSE)
	{
		m_licenseData.bLicense = FALSE;
		return -1;
	}

	if (!CheckVerNum())
	{
		m_licenseData.bLicense = FALSE;
		return OFD_LICENSE_VERSION;
	}

	if (!CheckMacAddr())
	{
		m_licenseData.bLicense = FALSE;
		return OFD_LICENSE_HARDWARE;
	}

	return 0;
}

//
FX_INT32 CFS_OFDLicenseManager::ActiveLicense(const CFX_WideString& wsSerialnum, CFX_WideString& file)
{
	CFX_WideString XMLPostData = GetXMLPostData(wsSerialnum);
	if (XMLPostData.IsEmpty())
		return -1;

	CFX_ByteString key;
	CFX_ByteString sessionID;
	if (!GetEncryptKey(key, sessionID))
		return OFD_LICENSE_NET;

	CFX_ByteString bsPostData = XMLPostData.UTF8Encode();

	CFX_ByteString bsEncryptPostData = EncryptData_Foxit_AES(key, bsPostData);
	bsEncryptPostData.Insert(bsEncryptPostData.GetLength(), L'\n');
	bsEncryptPostData += sessionID;

	CFX_ByteString bsRecive;
	if (!PostData((FX_LPCWSTR)HTFoxit_LicenseServer, (FX_LPCWSTR)HTFoxit_LicenseObjectName, bsEncryptPostData, bsRecive, TRUE))
		return OFD_LICENSE_NET;

	CFX_ByteString bsDecryptRecive = DecryptData_Foxit_AES(key, bsRecive);
	CFX_ByteString bsGetKeyFile;
	CFX_WideString wsCode, wsMessage, wsActiveCount;
	int iRet = ParseServerXML((FX_LPBYTE)(FX_LPCSTR)bsDecryptRecive, bsDecryptRecive.GetLength(),
		wsCode, wsMessage, wsActiveCount, bsGetKeyFile, key);

	if (iRet == OFD_LICENSE_OK) {
		IFX_FileWrite* fw = FX_CreateFileWrite(file);
		if (fw)
		{
			fw->WriteBlock((FX_LPBYTE)(FX_LPCSTR)bsGetKeyFile, bsGetKeyFile.GetLength());
			fw->Release();
		}
		else
			iRet = OFD_LICENSE_WRITE;
	}

	return iRet;
}

//
CFX_WideString CFS_OFDLicenseManager::GetLicenseFile(const CFX_WideString& wsLicensePath)
{
	CFX_WideString wsFile = wsLicensePath;
	wsFile.Replace((FX_LPCWSTR)L"\\", (FX_LPCWSTR)L"/");

	int len = wsFile.GetLength();
	if (wsFile[len - 1] != '/')
		wsFile.Insert(len, L'/');

	wsFile += _FS_KEYFILE_;

	return wsFile;
}

//
FX_BOOL CFS_OFDLicenseManager::ParseLicenseFile(IFX_FileRead* pKeyFile)
{
	if (!pKeyFile)
		return FALSE;

	// 读取keyFile的内容
	FX_DWORD dwFileSize = pKeyFile->GetSize();
	if (dwFileSize < 1)
	{
		return FALSE;
	}

	FX_CHAR* pBuf = FX_Alloc(FX_CHAR, dwFileSize + 1);
	FXSYS_memset(pBuf, 0, dwFileSize + 1);
	if (dwFileSize != pKeyFile->ReadBlock(pBuf, dwFileSize))
	{
		return FALSE;
	}

	CFX_WideString wsKeyContent = CFX_WideString::FromUTF8(pBuf);
	wsKeyContent += L"\n";// 某些keyFile最后没有换行，会导致验证失败
	FX_Free(pBuf);

	// 分析从keyFile中读取的内容
	int nSubStrIndex = -1, nIndex = -1;
	const wchar_t *pSubStr[8] = {L"SN=", L"Modules=", L"Users=", L"Licensee="
		, L"ExpireDate=", L"LicenseDate=", L"Sign=", L"Restrictions="};
	CFX_WideString wsTemp[8];

	// 获取key content中每一项的内容
	for (int i = 0; i < 8; i++)
	{
		nSubStrIndex = wsKeyContent.Find(pSubStr[i]);
		if(nSubStrIndex != -1)
		{
			
			nSubStrIndex += FXSYS_wcslen(pSubStr[i]);
			nIndex = wsKeyContent.Find(L'\n', nSubStrIndex);
			if (nIndex == -1)
			{
				wsTemp[i] = wsKeyContent.Mid(nSubStrIndex);
			}
			else
			{
				wsTemp[i] = wsKeyContent.Mid(nSubStrIndex, nIndex-nSubStrIndex);
			}
		}
		else
		{
			wsTemp[i] = L"";
		}
	}

	// 将得到的每一项内容赋值给OFD_LICENSEDATA
	m_licenseData.wsSN = wsTemp[0];
	m_licenseData.wsModule = wsTemp[1];
	m_licenseData.wsUsers = wsTemp[2];
	m_licenseData.wsLicense =  wsTemp[3];
	m_licenseData.wsExpireDate = wsTemp[4];
	m_licenseData.wsLicenseDate = wsTemp[5];
	m_licenseData.wsSignature = wsTemp[6];
	m_licenseData.wsRestrictions = wsTemp[7];

	m_licenseData.bTrial = m_licenseData.wsLicense.Find(FOFDSDK_TRIAL) > 0;

	return TRUE;
}

//Format: 2016-05-31
static FX_BOOL FS_TimeStrToSysTime(const CFX_WideString& wsTime, short& year, unsigned char& month, unsigned char& day)
{
	CFX_WideString wsTemp;
	int token1 = wsTime.Find(L'-');
	if (token1 < 0) return FALSE;
	wsTemp = wsTime.Left(token1);
	year = wsTime.GetInteger();

	int token2 = wsTime.Find(L'-', token1 + 1);
	if (token2 < 0) return FALSE;
	wsTemp = wsTime.Mid(token1 + 1, token2 - token1 - 1);
	month = wsTemp.GetInteger();

	wsTemp = wsTime.Mid(token2 + 1, wsTime.GetLength() - token2 - 1);
	if (wsTemp.GetLength() < 0) return FALSE;
	day = wsTemp.GetInteger();

	return TRUE;
}

//
FX_BOOL CFS_OFDLicenseManager::CheckTime()
{
	if (!m_licenseData.wsExpireDate.IsEmpty() && !m_licenseData.wsLicenseDate.IsEmpty())
	{
		OFD_DATETIME stStart, stExpire, stLocal;
		FS_GetLocalTime(stLocal.year, stLocal.month, stLocal.day, stLocal.hour, stLocal.minute, stLocal.second);
		if (!m_licenseData.wsLicenseDate.IsEmpty() && !FS_TimeStrToSysTime(m_licenseData.wsLicenseDate, stStart.year, stStart.month, stStart.day))
		{
			return FALSE;
		}
		if (!m_licenseData.wsExpireDate.IsEmpty())
		{
			if (!FS_TimeStrToSysTime(m_licenseData.wsExpireDate, stExpire.year, stExpire.month, stExpire.day))
			{
				return FALSE;
			}

			stExpire.day += 1;// 保证截至日当前是有效的。
			FX_DWORD dwLocal = 0, dwExpire = 0;
			dwLocal |= (stLocal.year << 16) | (stLocal.month << 8) | stLocal.day;
			dwExpire |= (stExpire.year << 16) | (stExpire.month << 8) | stExpire.day;
			if( dwLocal >= dwExpire)// 当前时间大于等于试用到期时间
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//
FX_BOOL CFS_OFDLicenseManager::ParseRestrictionContent(const CFX_WideString& wsRestriction, OFD_RESTRICTION_CONTENT& restrictionContent)
{
	if (wsRestriction.IsEmpty())
	{
		return FALSE;
	}
	if ((wsRestriction.Left(4)).Compare(L"Ver:") != 0 || wsRestriction.Find(L"Keyver:") == -1)
	{
		return FALSE;
	}

	int nSubStrIndex = -1, nIndex = -1;
	const wchar_t *pSubStr[8] = {L"Ver:", L"Keyver:", L"Edition:", L"Code:"
		, L"Lang:", L"LICSVR:", L"LICRQD", L"md5:"};//L"Code:",L"LICSVR:", L"LICRQD" Ignore
	CFX_WideString wsTemp[8];

	//获取key content中每一项的内容
	for(int i = 0; i < 8; i++)
	{
		nSubStrIndex = wsRestriction.Find(pSubStr[i]);
		if (nSubStrIndex != -1)
		{
			nSubStrIndex += wcslen(pSubStr[i]);
			nIndex = wsRestriction.Find(L',', nSubStrIndex);
			if (nIndex == -1)
			{
				wsTemp[i] = wsRestriction.Mid(nSubStrIndex);
			}
			else
			{
				wsTemp[i] = wsRestriction.Mid(nSubStrIndex, nIndex-nSubStrIndex);
			}
		}
		else
		{
			wsTemp[i] = L"";
		}
	}

	restrictionContent.nVerNumber = wsTemp[0];
	CFX_WideString wsVersion;
	int token1 = wsTemp[0].Find(L'.');
	if (token1 > 0)
	{
		wsVersion = wsTemp[0].Left(token1);
		restrictionContent.nVer_Major = wsVersion.GetInteger();

		int token2 = wsTemp[0].Find(L'.', token1 + 1);
		if (token2 > token1)
		{
			wsVersion = wsTemp[0].Mid(token1 + 1, token2 - token1 - 1);
			restrictionContent.nVer_Inter = wsVersion.GetInteger();

			wsVersion = wsTemp[0].Right(wsTemp[0].GetLength() - token2 - 1);
			restrictionContent.nVer_Minor = wsVersion.GetInteger();
		}
	}
	else
	{
		restrictionContent.nVer_Major = wsTemp[0].GetInteger();
		restrictionContent.nVer_Inter = 0;
		restrictionContent.nVer_Minor = 0;
	}

	restrictionContent.nKeyverNumber = (wsTemp[1].IsEmpty()) ? 0 : wsTemp[1].GetInteger();
	restrictionContent.wsEdition = wsTemp[2];
	restrictionContent.wsLanguage = wsTemp[4];
	restrictionContent.wsMd5 = wsTemp[7];
	return TRUE;
}

//
CFX_WideString CFS_OFDLicenseManager::GetProductName()
{
	return _FS_PRODUCTNAME_;
}

// TODO
FX_BOOL CFS_OFDLicenseManager::CheckVerNum()
{
	if (m_licenseData.restriction.nVer_Major < _FS_VERSION_MAJOR)
		return FALSE;
//	if (m_licenseData.restriction.nVer_Inter < _FS_VERSION_INTER)
//		return FALSE;

	return TRUE;
}

static int splitMacAddrsString(CFX_WideString& strMacAddrs, CFX_WideStringArray& strArray)
{
	int pos = 0;

	int len = strMacAddrs.GetLength();
	CFX_WideString subStr;
	while (pos < len)
	{
		FX_WCHAR ch1 = strMacAddrs[pos];

		if (ch1 != L'|') {
			subStr += ch1;
			if (pos == len - 1)
				strArray.Add(subStr);
		} 
		else if (!subStr.IsEmpty()) {
			strArray.Add(subStr);
			subStr.Empty();
		}

		pos ++;
	}
	return pos;
}

//
FX_BOOL CFS_OFDLicenseManager::CheckMacAddr()
{
	FX_BOOL bRet = FALSE;

	CFX_WideString wsMacAddr = GetMacAddr();
	if(wsMacAddr.IsEmpty())
	{
		wsMacAddr = GetHardDiskID();
	}

#if 0
	CFX_ByteString bsData = wsMacAddr.UTF8Encode();
	unsigned char digest[16] = {0};
	CRYPT_MD5Generate((FX_LPCBYTE )bsData, bsData.GetLength(), digest);
	CFX_ByteString bsTemp;
	CFX_WideString wsHash;
	for (int i = 0; i < 16; i ++)
	{
		FX_BYTE b = digest[i];
		CFX_WideString bsTemp1;	bsTemp1.Format((FX_LPCWSTR)L"%x", (b & 0xF0) >> 4);
		wsHash.Insert(i * 2, bsTemp1[0]);
		CFX_WideString bsTemp2;	bsTemp2.Format((FX_LPCWSTR)L"%x", b & 0x0F);
		wsHash.Insert(i * 2 + 1, bsTemp2[0]);
	}

	bRet = wsHash.Equal(m_licenseData.restriction.wsMd5);
#else
	CFX_WideStringArray wsMacAddrs;
	splitMacAddrsString(wsMacAddr, wsMacAddrs);

	if (wsMacAddrs.GetSize() < 1)
		return bRet;

	CFX_ByteString licEncryptMacs = m_licenseData.restriction.wsMd5.UTF8Encode();
	CFX_ByteString licMacs = DecryptData_Foxit_AES(FS_AES_KEY_ENCODEMACADDR, licEncryptMacs);

	for (int i = 0; i < wsMacAddrs.GetSize(); i ++)
	{
		CFX_ByteString aMac = wsMacAddrs[i].UTF8Encode();
		if (licMacs.Find(aMac) > -1)
		{
			bRet = TRUE;
			break;
		}
	}
#endif

	return bRet;
}

#if _FX_OS_ == _FX_WIN32_DESKTOP_ || _FX_OS_ == _FX_WIN64_

//
CFX_WideString CFS_OFDLicenseManager::GetMacAddr()
{
#ifdef _FS_LICENSED_BUILD_
	CFX_WideString sMacAddress;

	FX_DWORD Err;

	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	FX_DWORD AdapterInfoSize = 0;

	// Enumerate all of the adapter specific information using the IP_ADAPTER_INFO structure.
	// Note:  IP_ADAPTER_INFO contains a linked list of adapter entries.

	if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
	{
		if (Err != ERROR_BUFFER_OVERFLOW)
		{
			//printf("GetAdaptersInfo sizing failed with error %d\n", Err);
			return L"";
		}
	}

	if ((pAdapterInfo = (PIP_ADAPTER_INFO) GlobalAlloc(GPTR, AdapterInfoSize)) == NULL)
	{
		//printf("Memory allocation error\n");
		return L"";
	}

	if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
	{
		//printf("GetAdaptersInfo failed with error %d\n", Err);
		return L"";
	}

	pAdapt = pAdapterInfo;
	while (pAdapt)
	{
		CFX_WideString sMac;
		{
			//Physical Address. . . . . . 
			for (int i = 0; i < pAdapt->AddressLength; i++)
			{
				CFX_WideString s;
				if (i == (pAdapt->AddressLength - 1))
				{
					s.Format((FX_LPCWSTR)L"%.2X", (int)pAdapt->Address[i]);
				}
				else
				{
					s.Format((FX_LPCWSTR)L"%.2X-", (int)pAdapt->Address[i]);
				}

				sMac = sMac + s;
			} 
			pAdapt = pAdapt->Next;
			if(sMac.IsEmpty())
				continue;
			//剔除虚拟网卡的MAC00-50-56-C0-00-01			00-50-56-C0-00-08
			if (sMac.Compare(L"00-50-56-C0-00-01") == 0 || sMac.Compare(L"00-50-56-C0-00-08") == 0)
				continue;

			sMacAddress += sMac ;
			sMacAddress += L"|";
		}

	}
//	CFX_WideString wsPCName = GetHostIP();
//	sMacAddress += wsPCName;
//	sMacAddress += L"|";

//	sMacAddress = sMacAddress.Left(sMacAddress.GetLength() -1);
	int nLen = sMacAddress.GetLength();
	if (nLen > 0 && sMacAddress[nLen - 1] == L'|')
		sMacAddress.Delete(nLen - 1, 1);

	return sMacAddress;
#else
	return L"08-D4-0C-DB-56-73|20-47-47-75-E9-8A|08-D4-0C-DB-56-74|08-D4-0C-DB-56-73|Chen";
#endif
}

CFX_WideString CFS_OFDLicenseManager::GetHardDiskID()
{
	return L"";
}

#else

/*
CFX_WideString CFS_OFDLicenseManager::GetMacAddr()
{
#ifdef _FS_LICENSED_BUILD_
       struct ifreq ifreq;
        int sock = 0;

        if ((sock=socket(AF_INET,SOCK_STREAM,0))<0)
        {
                perror("socket");
                close(sock);
                return L"";
        }

        strcpy(ifreq.ifr_name,"eth0");
        if (ioctl(sock,SIOCGIFHWADDR,&ifreq)<0)
        {
                strcpy(ifreq.ifr_name, "eth1");
                if(ioctl(sock, SIOCGIFHWADDR, &ifreq)<0)
                {
                        perror("ioctl");
                        close(sock);
                        return L"";
                }
        }

	CFX_ByteString bsMac;
	bsMac.Format("%02x-%02x-%02x-%02x-%02x-%02x",
	                (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
	                (unsigned char)ifreq.ifr_hwaddr.sa_data[1],
	                (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
	                (unsigned char)ifreq.ifr_hwaddr.sa_data[3],
	                (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
	                (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);

	CFX_WideString wsMac = bsMac.UTF8Decode();

        close(sock);

	return wsMac;
#else
	CFX_WideString wsMac;
	return wsMac;
#endif
}
*/

CFX_WideString CFS_OFDLicenseManager::GetMacAddr()
{
	CFX_WideString wsMacAddress = L"";

#ifdef _FS_LICENSED_BUILD_
    	int fd;
    	int interfaceNum = 0;
    	struct ifreq buf[16];
    	struct ifconf ifc;
    	struct ifreq ifrcopy;

    	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
        	close(fd);
        	return wsMacAddress;
    	}


		ifc.ifc_len = sizeof(buf);
    	ifc.ifc_buf = (caddr_t)buf;
    	if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    	{
        		interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
        		while (interfaceNum-- > 0)
        		{
            			//ignore the interface that not up or not runing
            			ifrcopy = buf[interfaceNum];
            			if(ioctl(fd, SIOCGIFFLAGS, &ifrcopy))
            			{
                			continue;
            			}
			
				if( strcmp(buf[interfaceNum].ifr_name, "lo") == 0 )
				{
               			continue;
       			}

				//get the mac of this interface
				if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[interfaceNum])))
				{
					CFX_ByteString bsMac;
					bsMac.Format("%02x-%02x-%02x-%02x-%02x-%02x",
                    				(unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[0],
                    				(unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[1],
                    				(unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[2],
                    				(unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[3],
                    				(unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[4],
                    				(unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[5]);

					CFX_WideString wsMac = bsMac.UTF8Decode();

					if (wsMac.Compare(L"00-50-56-C0-00-01") == 0 || wsMac.Compare(L"00-50-56-C0-00-08") == 0)
						continue;

					wsMacAddress += wsMac ;
					wsMacAddress += L"|";

					break;
				}
				else
				{
					continue;
				}
        	}
    	}
	close(fd);

	int nLen = wsMacAddress.GetLength();
	if (nLen > 0 && wsMacAddress[nLen - 1] == L'|')
		wsMacAddress.Delete(nLen - 1, 1);

	return wsMacAddress;
#else
	return wsMacAddress;
#endif
}

CFX_WideString CFS_OFDLicenseManager::GetHardDiskID()
{
    CFX_WideString wsDisk = L"";

    struct hd_driveid hid;
    int fd = open ("/dev/sda", O_RDONLY);

    if (fd < 0)
    {
        fd = open("/dev/hda",O_RDONLY);

        if(fd < 0 )
        {
                return wsDisk;
        }
    }

	memset(hid.serial_no, 0, 20);

    if (ioctl (fd, HDIO_GET_IDENTITY, &hid) < 0)
    {
		close (fd);
        return wsDisk;
    }

    close (fd);

	//memcpy(hardc, (const char*)(hid.serial_no), strlen((const char*)(hid.serial_no)));

	CFX_WideString wsHardDisk = CFX_WideString::FromUTF8(hid.serial_no, 20);

	return wsHardDisk;
}

#endif

/*
CFX_WideString CFS_OFDLicenseManager::GetHostIP()
{
	char hostName[255];
	FX_WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 1, 1 );
	int err = WSAStartup(wVersionRequested, &wsaData);
	PHOSTENT hostinfo;
	CFX_WideString wsPCName;
	if (gethostname(hostName, sizeof(hostName)) == 0)
	{
		wsPCName = CFX_WideString::FromLocal(hostName);
	}
	return wsPCName;
}
*/

//
CFX_WideString CFS_OFDLicenseManager::GetXMLPostData(const CFX_WideString& wsSerialnum)
{
	CFX_ByteString bsData = "";
	CFX_ByteString bsFormat = "<?xml version=\"1.0\" encoding=\"utf-8\"?> \
							  <registration_request> \
							  <Action>Active</Action> \
							  <SN></SN> \
							  <Product name=\"%s\" version=\"%s\" keyversion=\"%s\" code=\"%s\" edition=\"\" language=\"zh_cn\" /> \
							  <SerialNumber>%s</SerialNumber> \
							  <Customer required = \"\"> \
							  <Exist></Exist> \
							  <MacAddress>%s</MacAddress> \
							  <ClientInformation laptop=\"\" sn=\"\" />\
							  <LicenseeName></LicenseeName> \
							  <FirstName></FirstName> \
							  <LastName></LastName> \
							  <EmailAddress></EmailAddress> \
							  <Job></Job> \
							  <NumUser></NumUser> \
							  <Company></Company> \
							  <Address1></Address1> \
							  <City></City> \
							  <State></State> \
							  <Country></Country> \
							  <Phone></Phone> \
							  <Trial2Activation></Trial2Activation> \
							  </Customer> \
							  <Distributor id=\"\" name=\"\" />\
							  <os name=\"%s\" platform=\"%s\" />\
							  </registration_request> ";

	CFX_WideString wsProductName = GetProductName();
	CFX_WideString wsProductVersion;
	wsProductVersion.Format((FX_LPCWSTR)L"%d.%d.%d", _FS_VERSION_MAJOR, _FS_VERSION_INTER, _FS_VERSION_MINOR);
	CFX_WideString wsKeyversion = L"1";
	CFX_WideString wsCode = _FS_PRODUCTAB_;
	CFX_WideString wsMacAddress = GetMacAddr();
	if(wsMacAddress.IsEmpty())
	{
		wsMacAddress = GetHardDiskID();
	}
	CFX_WideString wsOsName = _FS_OSNAME_;
	CFX_WideString wsOsPlatform = _FS_OS_PLATFORM_;

	CFX_ByteString bsSerialnum = wsSerialnum.UTF8Encode();
	CFX_ByteString bsProductName = wsProductName.UTF8Encode();
	CFX_ByteString bsProductVersion = wsProductVersion.UTF8Encode();
	CFX_ByteString bsKeyversion = wsKeyversion.UTF8Encode();
	CFX_ByteString bsCode = wsCode.UTF8Encode();
	CFX_ByteString bsMacAddress = wsMacAddress.UTF8Encode();
	CFX_ByteString bsOsName = wsOsName.UTF8Encode();
	CFX_ByteString bsOsPlatform = wsOsPlatform.UTF8Encode();

	bsData.Format(bsFormat,
		(FX_LPCSTR)TranslationXML(bsProductName),
		(FX_LPCSTR)TranslationXML(bsProductVersion),
		(FX_LPCSTR)TranslationXML(bsKeyversion),
		(FX_LPCSTR)TranslationXML(bsCode),
		(FX_LPCSTR)TranslationXML(bsSerialnum),
		(FX_LPCSTR)TranslationXML(bsMacAddress),
		(FX_LPCSTR)TranslationXML(bsOsName),
		(FX_LPCSTR)TranslationXML(bsOsPlatform));

	return bsData.UTF8Decode();
}

//
CFX_WideString CFS_OFDLicenseManager::TranslationXML(const CFX_WideString& wsData)
{
	if (wsData.IsEmpty())
		return L"";
	CFX_WideString destStr = wsData;
	CFX_WideString ReStr = L"";
	destStr.Replace((FX_LPCWSTR)L"&", (FX_LPCWSTR)L"&amp;");
	destStr.Replace((FX_LPCWSTR)L"\"", (FX_LPCWSTR)L"&quot;");
	destStr.Replace((FX_LPCWSTR)L"<", (FX_LPCWSTR)L"&lt;");
	destStr.Replace((FX_LPCWSTR)L">", (FX_LPCWSTR)L"&gt;");
	return destStr;
}

//
CFX_ByteString CFS_OFDLicenseManager::TranslationXML(const CFX_ByteString& bsData)
{
	if (bsData.IsEmpty())
		return "";
	CFX_ByteString destStr = bsData;
	CFX_ByteString ReStr = "";
	destStr.Replace("&", "&amp;");
	destStr.Replace("\"", "&quot;");
	destStr.Replace("<", "&lt;");
	destStr.Replace(">", "&gt;");
	return destStr;
}

//
FX_BOOL CFS_OFDLicenseManager::GetEncryptKey(CFX_ByteString& key, CFX_ByteString& sessionID)
{
	CFX_ByteString bsGetReceive;
	FX_BOOL bRet = PostData((FX_LPCWSTR)HTFoxit_LicenseServer, (FX_LPCWSTR)HTFoxit_LicenseObjectName, "", bsGetReceive, FALSE);
	if (bRet)
	{
		key = bsGetReceive;
		FX_STRSIZE pos = bsGetReceive.Find('\n');
		key = bsGetReceive.Left(pos);
		sessionID = bsGetReceive.Right(bsGetReceive.GetLength() - pos - 1);
	}
	return bRet;
}

#define AES_CONTENT_LEN	2048
#define AES_BLOCK_LEN	16

//
CFX_ByteString CFS_OFDLicenseManager::EncryptData_Foxit_AES(const CFX_ByteString& key, const CFX_ByteString& plain, FX_BOOL bUTF8)
{
	CFX_ByteString bsData = plain;

	FX_BYTE* context = FX_Alloc(FX_BYTE, AES_CONTENT_LEN);
	FXSYS_memset(context, 0, AES_CONTENT_LEN);

	CRYPT_AESSetKey(context, AES_BLOCK_LEN, (FX_LPCBYTE)key, key.GetLength(), TRUE);

	const FX_CHAR* iv = "0000000000000000";
	CRYPT_AESSetIV(context, (FX_LPCBYTE)iv);

	int nPadding = 0;
	int nDataLen = plain.GetLength();
	if (0 == nDataLen % AES_BLOCK_LEN)
	{
		nPadding = AES_BLOCK_LEN;
	}
	else
	{
		nPadding = AES_BLOCK_LEN - nDataLen % AES_BLOCK_LEN;
	}
	nDataLen += nPadding;
	char szFill = 0x00 + nPadding;
	for (int i = 0; i < nPadding; i ++)
		bsData += szFill;

	FX_LPBYTE pszOutput = FX_Alloc(FX_BYTE, nDataLen + 1);
	FXSYS_memset(pszOutput, 0, nDataLen + 1);

	CRYPT_AESEncrypt(context, (FX_LPBYTE)pszOutput, bsData, nDataLen);

	CFX_ByteString bsDst;
	if (bUTF8) {
		CFX_Base64Encoder base64Encoder;
		base64Encoder.Encode(CFX_ByteStringC(pszOutput, nDataLen), bsDst);
	} else
		bsDst = CFX_ByteString(pszOutput, nDataLen);

	FX_Free(pszOutput);
	FX_Free(context);

	return bsDst;
}

//
CFX_ByteString CFS_OFDLicenseManager::DecryptData_Foxit_AES(const CFX_ByteString& key, const CFX_ByteString& encrypted, FX_BOOL bUTF8)
{
	CFX_ByteString bsEncrypted;
	if (bUTF8) {
		CFX_Base64Decoder base64Decoder;
		base64Decoder.Decode(encrypted, bsEncrypted);
	} else
		bsEncrypted = encrypted;

	FX_BYTE* context = FX_Alloc(FX_BYTE, AES_CONTENT_LEN);
	FXSYS_memset(context, 0, AES_CONTENT_LEN);

	CRYPT_AESSetKey(context, AES_BLOCK_LEN, (FX_LPCBYTE)key, key.GetLength(), FALSE);

	const FX_CHAR* iv = "0000000000000000";
	CRYPT_AESSetIV(context, (FX_LPCBYTE)iv);

	int nDataLen = bsEncrypted.GetLength();
	FX_LPBYTE pszOutput = FX_Alloc(FX_BYTE, nDataLen + 1);
	FXSYS_memset(pszOutput, 0, nDataLen + 1);

	CRYPT_AESDecrypt(context, (FX_LPBYTE)pszOutput, (FX_LPCBYTE)bsEncrypted, bsEncrypted.GetLength());

	CFX_ByteString bsTemp((FX_LPCSTR)pszOutput);

	FX_Free(pszOutput);
	FX_Free(context);

	int nResultLen = bsTemp.GetLength();
	int nStartPadding = bsTemp.GetLength() - 1;
	char cPadding = bsTemp[nStartPadding];
	CFX_ByteString bsRet;
	int nCount = 0;
	while (TRUE) {
		if (bsTemp[nStartPadding] == cPadding && nCount <= AES_BLOCK_LEN) {
			nCount ++;
			nStartPadding --;
		} else {
			bsTemp.Delete(nStartPadding + 1, nCount);
			bsRet = CFX_ByteString((FX_LPCBYTE)bsTemp, bsTemp.GetLength());
			break;
		}
	}

	return bsRet;
}

#if _FS_OS_ == _FS_OS_WINDOWS_

//
FX_BOOL Local_PostData(FX_LPCWSTR ServerName, FX_LPCWSTR ObjectName, char* csPostData, int data_len, CFS_OFDLicenseManager* pLicenseMgr, FX_BOOL bCloseSession)
{
#ifdef _FS_LICENSED_BUILD_
	FX_DWORD dwSize = 0;
	FX_DWORD dwDownloaded = 0;
	FX_LPSTR pszOutBuffer = NULL;
//	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
	HINTERNET hConnect = NULL, hRequest = NULL;
	static HINTERNET hSession = NULL;

	FX_BOOL  bResults = FALSE;

	if (!hSession)
	{
		hSession = WinHttpOpen(L"User-Agent", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	}

	if (hSession)
	{
		hConnect = WinHttpConnect(hSession, (LPCWSTR)ServerName, 8012, 0);
	}

	if (hConnect)
	{
		hRequest = WinHttpOpenRequest(hConnect, L"POST", (LPCWSTR)ObjectName, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}

	LPCWSTR header = L"Content-type: application/x-www-form-urlencoded\r\n";

	int len = lstrlenW(header);

	WinHttpAddRequestHeaders(hRequest,header,DWORD(len), WINHTTP_ADDREQ_FLAG_ADD);

	if (hRequest)
	{
		bResults = WinHttpSendRequest(hRequest, 0, 0, csPostData, data_len, data_len, 0 );
	}

	if (bResults)
	{
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	}

	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;

			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				//printf( "Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

				break;
			}

			if (!dwSize)
				break;

			pszOutBuffer = FX_Alloc(FX_CHAR, dwSize + 1);

			if (!pszOutBuffer)
			{
				//printf("Out of memory\n");
				break;
			}

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			{
				//printf( "Error %u in WinHttpReadData.\n", GetLastError());
			}
			else
			{
				if (pLicenseMgr->m_rev)
					pLicenseMgr->m_rev = FX_Realloc(FX_CHAR, pLicenseMgr->m_rev, pLicenseMgr->m_rev_len + dwDownloaded);
				else
					pLicenseMgr->m_rev = FX_Alloc(FX_CHAR, dwDownloaded);

				FXSYS_memcpy(pLicenseMgr->m_rev + pLicenseMgr->m_rev_len, pszOutBuffer, dwDownloaded);
				pLicenseMgr->m_rev_len += dwDownloaded;

				//printf("%s", pszOutBuffer);
			}

			delete [] pszOutBuffer;

			if (!dwDownloaded)
				break;

		} while (dwSize > 0);
	}

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (bCloseSession && hSession) {
		WinHttpCloseHandle(hSession);
		hSession = NULL;
	}

	return pLicenseMgr->m_rev != NULL;
#else
	return FALSE;
#endif
}

#else

class CFX_HTTP_Module
{
public:
	CFX_HTTP_Module();
	~CFX_HTTP_Module();

	int		Connect(const char* pIP, int nPort);
	int		SendData(const char* pwData, int nLen);
	int		RecvData(CFS_OFDLicenseManager* pLicenseMgr);

public:
	void		Close();
	FX_BOOL		ExtractHttpRevData(const CFX_ByteString& bsHttpContents, CFX_ByteString& bsData);

	int		m_sockfd;
static	CFX_ByteString*	m_cookie;
};

#define BUFSIZE 4096

CFX_ByteString*	CFX_HTTP_Module::m_cookie = NULL;

CFX_HTTP_Module::CFX_HTTP_Module()
{
	m_sockfd = -1;
}

CFX_HTTP_Module::~CFX_HTTP_Module()
{
	Close();
}

int CFX_HTTP_Module::Connect(const char* pIP, int nPort)
{
#ifdef _FS_LICENSED_BUILD_
	if( pIP == NULL || nPort < 0 )
		return 1000;

	//printf("Connect --------------->1 \n");
	if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		return 1001;
	};

	struct hostent* pHost = gethostbyname(pIP);
	if( pHost == NULL )
		return 1002;

    char str[64] = {0};
    inet_ntop(pHost->h_addrtype, *pHost->h_addr_list, str, sizeof(str));

	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(nPort);
	//if (inet_pton(AF_INET, pIP, &servaddr.sin_addr) <= 0 )
	if (inet_pton(AF_INET, (const char*)(str), &servaddr.sin_addr) <= 0 )
	{
		return 1002;
	};

	if (connect(m_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		return 1003;
	}

	//printf("Connect --------------->2 \n");
#endif
	return 0;
}

int CFX_HTTP_Module::SendData(const char* pData, int nLen)
{
#ifdef _FS_LICENSED_BUILD_
	char dataLen[64] = {0};
	char readBuf[BUFSIZE] = {0};

	sprintf(dataLen, "%d", nLen);

	char package[BUFSIZE] = {0};

	strcat(package, "POST /license.php HTTP/1.1\n");
	strcat(package, HTFoxit_LicenseHost);
	strcat(package, "Connection:Keep-Alive\n");
	if (m_cookie) {
		strcat(package, (FX_LPCSTR)*m_cookie);
		strcat(package, "\n");
	}
	strcat(package, "Content-Type: application/x-www-form-urlencoded\n");
	strcat(package, "Content-Length: ");

	strcat(package, dataLen);
	strcat(package, "\n\n");	

	if (nLen == 0)
		strcat(package, "");
	else
		strcat(package, pData);

	strcat(package, "\r\n\r\n");

	//printf(" len= %d\n data=%s\n", strlen(package), package);

	int ret = write(m_sockfd, package, strlen(package));
	if (ret < 0) {
		return 2002;
	}
#endif
	return 0;
}

FX_BOOL CFX_HTTP_Module::ExtractHttpRevData(const CFX_ByteString& bsHttpContents, CFX_ByteString& bsData)
{
	int pos = bsHttpContents.Find("HTTP/1.1 200 OK");
	if (pos < 0) return FALSE;
	pos = bsHttpContents.Find("Content-Length: ");
	if (pos < 0) return FALSE;
	int pos2 = bsHttpContents.Find("\r", pos);
	CFX_ByteString bsDataLen = bsHttpContents.Mid(pos + 16, pos2 - pos - 16);
	int data_len = FXSYS_atoi(bsDataLen);
	if (data_len < 0) return FALSE;
	int pos3 = bsHttpContents.Find("\r\n\r\n", pos2);
	if (pos3 < 0 || pos3 + 4 + data_len > bsHttpContents.GetLength())
		return -1;

	bsData = bsHttpContents.Mid(pos3 + 4, data_len);

	if (!m_cookie) {
		int pos4 = bsHttpContents.Find("Cookie");
		if (pos4 > -1)
		{
			int pos5 = bsHttpContents.Find(";", pos4);
			m_cookie = FX_NEW CFX_ByteString;
			*m_cookie = bsHttpContents.Mid(pos4, pos5 - pos4);
			m_cookie->TrimRight();
		}
		else
			return FALSE;
	}

	return TRUE;
}

int CFX_HTTP_Module::RecvData(CFS_OFDLicenseManager* pLicenseMgr)
{
#ifdef _FS_LICENSED_BUILD_
	fd_set   t_set1;
	FD_ZERO(&t_set1);

	FD_SET(m_sockfd, &t_set1);

	struct timeval  tv;
	char readBuf[BUFSIZE] = {0};
	int nRead = 0;

	FX_CHAR* rev = NULL;
	int rev_len = 0;

	//printf("Recv --------------->1 \n");

	while(1)
	{
		sleep(2);
		tv.tv_sec= 0;
		tv.tv_usec= 0;
		int nSelect = 0;
		nSelect = select(m_sockfd +1, &t_set1, NULL, NULL, NULL);//&tv
//		printf("When select Socket, ret == %d ...\n", nSelect);

		//if (nSelect == 0) continue;
		if (nSelect < 0)
		{
			if (rev) FX_Free(rev);

			return 3001;
		};
 
		if (nSelect > 0)
		{
			memset(readBuf, 0, BUFSIZE);
			nRead = read(m_sockfd, readBuf, 4095);
			if (nRead == 0)
			{
				if (!rev) {
					return 3002;
				} else {
					CFX_ByteString bsContents(rev, rev_len);

					CFX_ByteString bsData;
					if (ExtractHttpRevData(bsContents, bsData))
					{
						pLicenseMgr->m_rev_len = bsData.GetLength();
						pLicenseMgr->m_rev = FX_Alloc(FX_CHAR, pLicenseMgr->m_rev_len);
						FXSYS_memcpy(pLicenseMgr->m_rev, (FX_LPCBYTE)bsData, pLicenseMgr->m_rev_len);
					}
					FX_Free(rev);
					return 0;
				}
			}
 
			if (rev)
				rev = FX_Realloc(FX_CHAR, rev, rev_len + nRead);
			else
				rev = FX_Alloc(FX_CHAR, nRead);

			FXSYS_memcpy(rev + rev_len, readBuf, nRead);
			rev_len += nRead;
			
			//printf("Recv raw: len= %d\n    data= %s\n", nRead, readBuf);
		}
	}
#endif
	return 0;
}

void CFX_HTTP_Module::Close()
{
	if ( m_sockfd >= 0 )
		close(m_sockfd);
	m_sockfd = -1;
}

FX_BOOL Local_PostData(FX_LPCWSTR ServerName, FX_LPCWSTR ObjectName, char* csPostData, int data_len, CFS_OFDLicenseManager* pLicenseMgr, FX_BOOL bCloseSession)
{
	CFX_HTTP_Module module;

	if (!bCloseSession && CFX_HTTP_Module::m_cookie) {
		delete CFX_HTTP_Module::m_cookie;
		CFX_HTTP_Module::m_cookie = NULL;
	}
	
	int nRet = module.Connect(HTFoxit_LicenseServer2, 8012);
	if (!nRet) {
		nRet = module.SendData(csPostData, data_len);
		if (!nRet) {
			nRet = module.RecvData(pLicenseMgr);
			//if (nRet)
				//printf("HTTP module recvData fail err = %d \n", nRet);
		} //else
			//printf("HTTP module sendData fail err = %d \n", nRet);
	

	} //else
		//printf("HTTP module connect fail err = %d \n", nRet);

	if (bCloseSession && CFX_HTTP_Module::m_cookie) {
		delete CFX_HTTP_Module::m_cookie;
		CFX_HTTP_Module::m_cookie = NULL;
	}

	//printf("Local_PostData nRet = %d \n", nRet);
	return nRet == 0;
}

#endif

//
FX_BOOL CFS_OFDLicenseManager::PostData(FX_LPCWSTR ServerName, FX_LPCWSTR objectName, const CFX_ByteString& csPostData, CFX_ByteString &csGetReceive, FX_BOOL bCloseSession)
{
	m_rev = NULL;
	m_rev_len = 0;
	FX_BOOL ret = Local_PostData((FX_LPCWSTR)ServerName, (FX_LPCWSTR)objectName , (char*)(FX_LPCSTR)csPostData, csPostData.GetLength(), this, bCloseSession);

	if (ret && m_rev && m_rev_len > 0)
		csGetReceive = CFX_ByteString(m_rev, m_rev_len);
	if (m_rev) {
		FX_Free(m_rev);
		m_rev = NULL;
		m_rev_len = 0;
	}

	return ret;
}

//
int CFS_OFDLicenseManager::ParseServerXML(FX_LPBYTE pData, FX_DWORD dwDataLen, CFX_WideString &csCode, CFX_WideString &wsMessage, 
	CFX_WideString &csActiveCount, CFX_ByteString &csKey, const CFX_ByteString& key)
{
	CXML_Element* pRoot = CXML_Element::Parse((FX_LPCBYTE)pData , dwDataLen);
	if (!pRoot)
		return OFD_LICENSE_NET;

	//Code
	if (CXML_Element* pCode = pRoot->GetElement("" , "Code"))
	{
		csCode = pCode->GetContent(0);
		if (!csCode.Equal(FX_WSTRC(L"OK")))
		{
			if (csCode.Equal(FX_WSTRC(L"ERR106")))
				return OFD_LICENSE_SERIAL;
			if (csCode.Equal(FX_WSTRC(L"ERR108")))
				return OFD_LICENSE_SERIALEXPIRED;
			if (csCode.Equal(FX_WSTRC(L"ERR202")))
				return OFD_LICENSE_EXCEED;
			if (csCode.Equal(FX_WSTRC(L"ERR302")))
				return OFD_LICENSE_EXPIRED;
			return OFD_LICENSE_NET;
		}
	}

	//Message
	if (CXML_Element* pMessage = pRoot->GetElement("" , "Message"))
	{
		wsMessage = pMessage->GetContent(0);
	}

	//ActivationCount
	if (CXML_Element* pActiveCount = pRoot->GetElement("" , "ActivationCount"))
	{
		csActiveCount = pActiveCount->GetContent(0);
	}

	//LicenseKey
	if (CXML_Element* pKey = pRoot->GetElement("" , "LicenseKey"))
	{
		CFX_WideString wsContent = pKey->GetContent(0);
		csKey = wsContent.UTF8Encode();
//		CFX_ByteString bsBase64Key = CFX_ByteString::FromUnicode(wsContent);
//		csKey = DecryptData_OpenSSL_AES(key, bsBase64Key);
//		csKey = DecryptData_Foxit_AES(key, bsBase64Key);
	}

	delete pRoot;
	return 0;
}

//
void CFS_OFDLicenseManager::ClearLicenseData()
{
	m_licenseData.bLicense = FALSE;
	m_licenseData.bTrial = TRUE;
	m_licenseData.wsSN.Empty();
	m_licenseData.wsModule.Empty();
	m_licenseData.wsUsers.Empty();
	m_licenseData.wsLicense.Empty();
	m_licenseData.wsExpireDate.Empty();
	m_licenseData.wsLicenseDate.Empty();
	m_licenseData.wsRestrictions.Empty();
	m_licenseData.wsSignature.Empty();

	m_licenseData.restriction.wsEdition.Empty();
	m_licenseData.restriction.wsLanguage.Empty();
	m_licenseData.restriction.wsMd5.Empty();
	m_licenseData.restriction.nKeyverNumber = 1;
	m_licenseData.restriction.nVer_Major = 0;
	m_licenseData.restriction.nVer_Inter = 0;
	m_licenseData.restriction.nVer_Minor = 0;
}


