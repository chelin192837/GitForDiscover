#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "util.h"
#include "publicmethods.h"
#include <time.h>

#if _FX_OS_  == _FX_ANDROID_
#include <ctype.h>
#endif
BEGIN_JS_STATIC_CONST(CFXJS_Util)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Util)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Util)
	JS_STATIC_METHOD_ENTRY(printd, 3)
	JS_STATIC_METHOD_ENTRY(printf, 20)
	JS_STATIC_METHOD_ENTRY(printx, 2)
	JS_STATIC_METHOD_ENTRY(scand, 2)
	JS_STATIC_METHOD_ENTRY(byteToChar, 1)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Util,jutil,util)

jutil::jutil(CFXJS_Object *pJSObject) : CFXJS_EmbedObj(pJSObject)
{
}

jutil::~jutil(void)
{
}


struct stru_TbConvert
{
	FX_LPCWSTR lpszJSMark;
	FX_LPCWSTR lpszCppMark;
};

const stru_TbConvert cwTable[] = {
	(FX_LPCWSTR)L"mmmm", (FX_LPCWSTR)L"%B",
	(FX_LPCWSTR)L"mmm", (FX_LPCWSTR)L"%b",
	(FX_LPCWSTR)L"mm",  (FX_LPCWSTR)L"%m",
	//"m"
	(FX_LPCWSTR)L"dddd", (FX_LPCWSTR)L"%A",
	(FX_LPCWSTR)L"ddd", (FX_LPCWSTR)L"%a",
	(FX_LPCWSTR)L"dd",  (FX_LPCWSTR)L"%d",
	//"d",   "%w",
	(FX_LPCWSTR)L"yyyy", (FX_LPCWSTR)L"%Y",
	(FX_LPCWSTR)L"yy",  (FX_LPCWSTR)L"%y",
	(FX_LPCWSTR)L"HH",  (FX_LPCWSTR)L"%H",
	//"H"
	(FX_LPCWSTR)L"hh",  (FX_LPCWSTR)L"%I",
	//"h"
	(FX_LPCWSTR)L"MM",  (FX_LPCWSTR)L"%M",
	//"M"
	(FX_LPCWSTR)L"ss",  (FX_LPCWSTR)L"%S",
	//"s
	(FX_LPCWSTR)L"TT",  (FX_LPCWSTR)L"%p",
	//"t"
#if defined(_WIN32)
	(FX_LPCWSTR)L"tt",  (FX_LPCWSTR)L"%p",
	(FX_LPCWSTR)L"h",  (FX_LPCWSTR)L"%#I",
#else
	(FX_LPCWSTR)L"tt",  (FX_LPCWSTR)L"%P",
	(FX_LPCWSTR)L"h",  (FX_LPCWSTR)L"%l",
#endif
};

#define UTIL_INT			0
#define UTIL_DOUBLE			1
#define UTIL_STRING			2

int jutil::ParstDataType(CFX_WideString* sFormat)
{
        size_t i = 0;
	bool bPercent = FALSE;
	for (i=0; i<(size_t)sFormat->GetLength(); ++i)
	{
		wchar_t c = (*sFormat)[i];
		if (c == L'%')
		{
			bPercent = true;
			continue;
		}

		if (bPercent)
		{
			if (c == L'c' || c == L'C' || c == L'd' || c == L'i' || c == L'o' || c == L'u' || c == L'x' || c == L'X')
			{
				return UTIL_INT;
			}
			else if (c == L'e' || c == L'E' || c == L'f' || c == L'g' || c == L'G')
			{
				return UTIL_DOUBLE;
			}
			else if (c == L's' || c == L'S')
			{
				// Map s to S since we always deal internally
				// with wchar_t strings.
				
				//(*sFormat)[i] = L'S';
				sFormat->SetAt(i, (FX_WCHAR)L'S');
				return UTIL_STRING;
			}
			else if (c == L'.' || c == L'+' || c == L'-' || c == L'#' || c == L' ' || CFXJS_PublicMethods::IsDigit(c))
			{
				continue;
			}
			else break;
		}
	}

	return -1;
}

FX_BOOL jutil::printf(OBJ_METHOD_PARAMS)
{
	size_t iSize = params.size();
	if (iSize < 1)
		return FALSE;
	CFX_WideString  c_ConvChar((FX_LPCWSTR)params[0]);
	//std::vector<std::wstring> c_strConvers;
	CFX_ArrayTemplate<CFX_WideString> c_strConvers;
	int iOffset = 0;
	int iOffend = 0;
	c_ConvChar.Insert(0,(FX_WCHAR)L'S');
	while(iOffset != -1)
	{
		iOffend = c_ConvChar.Find((FX_WCHAR)L'%',iOffset+1);
		//iOffend = c_ConvChar.find(L"%",iOffset+1);
		CFX_WideString strSub;
		if (iOffend == -1)
			strSub = c_ConvChar.Mid(iOffset);			
		else
			strSub = c_ConvChar.Mid(iOffset ,iOffend - iOffset);

		c_strConvers.Add(strSub);
		//c_strConvers.push_back(strSub);
		iOffset = iOffend ;
	}

	CFX_WideString c_strResult;

	//for(int iIndex = 1;iIndex < params.size();iIndex++)
	CFX_WideString c_strFormat;
	for(size_t iIndex = 0;iIndex < (size_t)c_strConvers.GetSize();iIndex++)
	{
		c_strFormat = c_strConvers[iIndex];
		if (iIndex == 0)
		{
			c_strResult = c_strFormat;
			continue;
		}


		CFX_WideString strSegment;
		if (iIndex >= iSize) {
			c_strResult += c_strFormat;
			continue;
		}

		switch (ParstDataType(&c_strFormat))
		{
			case UTIL_INT:
				strSegment.Format((FX_LPCWSTR)c_strFormat,(int)params[iIndex]);
				break;
			case UTIL_DOUBLE:
				strSegment.Format((FX_LPCWSTR)c_strFormat,(double)params[iIndex]);
				break;
			case UTIL_STRING:
				strSegment.Format((FX_LPCWSTR)c_strFormat,(FX_LPCWSTR)params[iIndex]);
				break;
			default:
				strSegment.Format((FX_LPCWSTR)L"%S", (FX_LPCWSTR)c_strFormat);
				break;
		}
		c_strResult += (wchar_t*)strSegment.GetBuffer(strSegment.GetLength()+1);
	}

	c_strResult.Delete(0, 1);
	//c_strResult.erase(c_strResult.begin());
	vRet = (FX_LPCWSTR)c_strResult;
	return TRUE;
}

FX_BOOL jutil::printd(OBJ_METHOD_PARAMS)
{
	int iSize = params.size();
	if (iSize < 2)
		return FALSE;

	CFXJS_Value p1;
	p1 = params[0];

	CFXJS_Value p2 = params[1];
	CFXJS_Date jsDate;
	if (!p2.ConvertToDate(jsDate))
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPRINT1);
		return FALSE;
	}

	if (!jsDate.IsValidDate())
	{
		sError = JSGetStringFromID((CFXJS_Context*)cc, IDS_STRING_JSPRINT2);
		return FALSE;
	}

	if (p1.GetType() == VT_number)
	{
		int nFormat = p1;

		CFX_WideString swResult;

		switch (nFormat)
		{
		case 0:
			swResult.Format((FX_LPCWSTR)L"D:%04d%02d%02d%02d%02d%02d", 
				jsDate.GetYear(),
				jsDate.GetMonth() + 1,
				jsDate.GetDay(),
				jsDate.GetHours(),
				jsDate.GetMinutes(),
				jsDate.GetSeconds());
			break;
		case 1:
			swResult.Format((FX_LPCWSTR)L"%04d.%02d.%02d %02d:%02d:%02d", 
				jsDate.GetYear(),
				jsDate.GetMonth() + 1,
				jsDate.GetDay(),
				jsDate.GetHours(),
				jsDate.GetMinutes(),
				jsDate.GetSeconds());
			break;
		case 2:
			swResult.Format((FX_LPCWSTR)L"%04d/%02d/%02d %02d:%02d:%02d", 
				jsDate.GetYear(),
				jsDate.GetMonth() + 1,
				jsDate.GetDay(),
				jsDate.GetHours(),
				jsDate.GetMinutes(),
				jsDate.GetSeconds());
			break;
		default:
			return FALSE;
		}

		vRet = swResult;
		return TRUE;
	}
	else if (p1.GetType() == VT_string)
	{
		CFX_WideString cFormat = (FX_LPCWSTR)p1;		

		bool bXFAPicture = false;
		if (iSize > 2)
		{
			//CJS_Value value;
			bXFAPicture = params[2];
		}

		if (bXFAPicture)
		{
			return FALSE; //currently, it doesn't support XFAPicture.
		}

        int iIndex;
		for(iIndex = 0;iIndex<sizeof(cwTable)/sizeof(stru_TbConvert);iIndex++)
		{
			int iStart = 0;
			int iEnd;
			while((iEnd = cFormat.Find(cwTable[iIndex].lpszJSMark, iStart)) != -1)
			{
				int length = cFormat.GetLength();
				CFX_WideString part1 = cFormat.Left(iEnd);
				CFX_WideString part2 = cFormat.Right(length - (iEnd+FXSYS_wcslen(cwTable[iIndex].lpszJSMark)));
				cFormat = part1 + (CFX_WideString)cwTable[iIndex].lpszCppMark + part2;
				//cFormat.replace(iEnd, FXSYS_wcslen(cwTable[iIndex].lpszJSMark), (CFX_WideString)cwTable[iIndex].lpszCppMark);
				iStart = iEnd;
			}
		}

		int iYear,iMonth,iDay,iHour,iMin,iSec;
		iYear = jsDate.GetYear();
		iMonth = jsDate.GetMonth();
		iDay = jsDate.GetDay();
		iHour = jsDate.GetHours();
		iMin = jsDate.GetMinutes();
		iSec = jsDate.GetSeconds();

		struct tm time = {0};
		time.tm_year = iYear-1900;
		time.tm_mon = iMonth;
		time.tm_mday = iDay;
		time.tm_hour = iHour;
		time.tm_min = iMin;
		time.tm_sec = iSec;
		//COleDateTime cppTm(iYear,iMonth+1,iDay,iHour,iMin,iSec);
		//CString strFormat = cppTm.Format(cFormat.c_str());

		struct stru_TbConvertAd
		{
			FX_LPCWSTR lpszJSMark;
			int     iValue;
		};

		stru_TbConvertAd cTableAd[] ={
			(FX_LPCWSTR)L"m", iMonth+1,
				(FX_LPCWSTR)L"d", iDay,
				(FX_LPCWSTR)L"H", iHour,
				(FX_LPCWSTR)L"h", iHour>12?iHour-12:iHour,
				(FX_LPCWSTR)L"M", iMin,
				(FX_LPCWSTR)L"s", iSec
		};

		//cFormat = strFormat.GetBuffer(strFormat.GetLength()+1);
		for(iIndex = 0;iIndex<sizeof(cTableAd)/sizeof(stru_TbConvertAd);iIndex++)
		{
			wchar_t tszValue[10];
			//_itot(cTableAd[iIndex].iValue,tszValue,10);
			CFX_WideString sValue;
			sValue.Format((FX_LPCWSTR)L"%d",cTableAd[iIndex].iValue);
			memcpy(tszValue, (wchar_t *)sValue.GetBuffer(sValue.GetLength()+1),sValue.GetLength()*sizeof(wchar_t));

			//strFormat.Replace(cTableAd[iIndex].lpszJSMark,"%d");
			//strFormat.Format(strFormat,cTableAd[iIndex].iValue);
			int iStart = 0;
			int iEnd;
			while((iEnd = cFormat.Find(cTableAd[iIndex].lpszJSMark,iStart)) != -1)
			{
				if (iEnd > 0)
				{
					if (cFormat[iEnd-1] == L'%')
					{
						iStart = iEnd+1;
						continue;
					}
				}

				int length = cFormat.GetLength();
				CFX_WideString part1 = cFormat.Left(iEnd);
				CFX_WideString part2 = cFormat.Right(length - (iEnd+FXSYS_wcslen(cTableAd[iIndex].lpszJSMark)));
				cFormat = part1 + tszValue + part2;
				//cFormat.replace(iEnd, FXSYS_wcslen(cTableAd[iIndex].lpszJSMark), tszValue);
				iStart = iEnd;
			}
		}

	//	CFX_WideString strFormat;
//		strFormat.Format((FX_LPCWSTR)L"%d,%d,%d,%d,%d,%d",iYear, iMonth, iDay, iHour, iMin, iSec);
//		CString strFormat = cppTm.Format(cFormat.c_str());
		wchar_t buf[64] = {0};
#if _FX_OS_ == _FX_ANDROID_
		char timeBuf[64] = {0};
		//wchar_t* pFormat = (wchar_t*)(FX_LPCWSTR)cFormat;
		CFX_ByteString formatObj = CFX_ByteString::FromUnicode(cFormat.GetBuffer(cFormat.GetLength()));
		char* strFormat = (FX_LPSTR)(FX_LPCSTR)formatObj;
		FX_INT32 size = strftime(timeBuf, 64, strFormat, &time);
	//	strFormat = wcsftime(buf, 64, pFormat, &time);
		CFX_ByteString tmpTime(timeBuf, strlen(timeBuf));
		CFX_WideString wsTime;
		wsTime.ConvertFrom(tmpTime);
		cFormat = wsTime;//buf;
#else
		CFX_WideString strFormat;
		strFormat = wcsftime(buf, 64, (const wchar_t*)(FX_LPCWSTR)cFormat, &time);
		cFormat = buf;
#endif
		vRet = (FX_LPCWSTR)cFormat;
		//rtRet = strFormat.GetBuffer(strFormat.GetLength()+1);
		return TRUE;
	}
	return FALSE;
}

void jutil::printd(const CFX_WideString &cFormat2, CFXJS_Date jsDate, bool bXFAPicture, CFX_WideString &cPurpose)
{
	CFX_WideString cFormat = cFormat2;
	    
	if (bXFAPicture)
	{
		return ; //currently, it doesn't support XFAPicture.
	}

    int iIndex;
	for(iIndex = 0;iIndex<sizeof(cwTable)/sizeof(stru_TbConvert);iIndex++)
	{
		int iStart = 0;
		int iEnd;
		while((iEnd = cFormat.Find(cwTable[iIndex].lpszJSMark,iStart)) != -1)
		{
			int length = cFormat.GetLength();
			CFX_WideString part1 = cFormat.Left(iEnd);
			CFX_WideString part2 = cFormat.Right(length - (iEnd+FXSYS_wcslen(cwTable[iIndex].lpszJSMark)));
			cFormat = part1 + (CFX_WideString)cwTable[iIndex].lpszCppMark + part2;
			//cFormat.replace(iEnd,FXSYS_wcslen(cwTable[iIndex].lpszJSMark), (CFX_WideString)cwTable[iIndex].lpszCppMark);
			iStart = iEnd;
		}
	}

	int iYear,iMonth,iDay,iHour,iMin,iSec;
	iYear = jsDate.GetYear();
	iMonth = jsDate.GetMonth();
	iDay = jsDate.GetDay();
	iHour = jsDate.GetHours();
	iMin = jsDate.GetMinutes();
	iSec = jsDate.GetSeconds();

	struct tm time = {0};
	time.tm_year = iYear-1900;
	time.tm_mon = iMonth;
	time.tm_mday = iDay;
	time.tm_hour = iHour;
	time.tm_min = iMin;
	time.tm_sec = iSec;
//	COleDateTime cppTm(iYear,iMonth+1,iDay,iHour,iMin,iSec);
	//CString strFormat = cppTm.Format(cFormat.c_str());

	struct stru_TbConvertAd
	{
		FX_LPCWSTR lpszJSMark;
		int     iValue;
	};

	stru_TbConvertAd cTableAd[] ={
		(FX_LPCWSTR)L"m", iMonth+1,
			(FX_LPCWSTR)L"d", iDay,
			(FX_LPCWSTR)L"H", iHour,
			(FX_LPCWSTR)L"h", iHour>12?iHour-12:iHour,
			(FX_LPCWSTR)L"M", iMin,
			(FX_LPCWSTR)L"s", iSec
	};

	//cFormat = strFormat.GetBuffer(strFormat.GetLength()+1);
	for(iIndex = 0;iIndex<sizeof(cTableAd)/sizeof(stru_TbConvertAd);iIndex++)
	{
		wchar_t tszValue[10];
		//_itot(cTableAd[iIndex].iValue,tszValue,10);
		CFX_WideString sValue;
		sValue.Format((FX_LPCWSTR)L"%d",cTableAd[iIndex].iValue);
		memcpy(tszValue, (wchar_t *)sValue.GetBuffer(sValue.GetLength()+1),sValue.GetLength()*sizeof(wchar_t));


		//strFormat.Replace(cTableAd[iIndex].lpszJSMark,"%d");
		//strFormat.Format(strFormat,cTableAd[iIndex].iValue);
		int iStart = 0;
		int iEnd;
		while((iEnd = cFormat.Find(cTableAd[iIndex].lpszJSMark,iStart)) != -1)
		{
			if (iEnd > 0)
			{
				if (cFormat[iEnd-1] == L'%')
				{
					iStart = iEnd+1;
					continue;
				}
			}
			int length = cFormat.GetLength();
			CFX_WideString part1 = cFormat.Left(iEnd);
			CFX_WideString part2 = cFormat.Right(length - (iEnd+FXSYS_wcslen(cTableAd[iIndex].lpszJSMark)));
			cFormat = part1 + tszValue + part2;
			//cFormat.replace(iEnd,FXSYS_wcslen(cTableAd[iIndex].lpszJSMark),tszValue);
			iStart = iEnd;
		}
	}

		CFX_WideString strFormat;
//		strFormat.Format((FX_LPCWSTR)L"%d,%d,%d,%d,%d,%d",iYear, iMonth, iDay, iHour, iMin, iSec);
//		CString strFormat = cppTm.Format(cFormat.c_str());
		wchar_t buf[64] = {0};
		strFormat = wcsftime(buf, 64, (const wchar_t*)(FX_LPCWSTR)cFormat, &time);
		cFormat = buf;
		cPurpose = cFormat;
}

FX_BOOL jutil::printx(OBJ_METHOD_PARAMS)
{
	int iSize = params.size();
	if (iSize<2)
		return FALSE;
	CFX_WideString sFormat = (FX_LPCWSTR)params[0];
	CFX_WideString sSource = (FX_LPCWSTR)params[1];
	CFX_ByteString cFormat = CFX_ByteString::FromUnicode(sFormat);
	CFX_ByteString cSource = CFX_ByteString::FromUnicode(sSource);
	CFX_ByteString cDest;
	printx(cFormat,cSource,cDest);
	vRet = (FX_LPCSTR)cDest;
	return TRUE;
}

void jutil::printx(const CFX_ByteString &cFormat,const CFX_ByteString &cSource2,CFX_ByteString &cPurpose)
{
	CFX_ByteString cSource(cSource2);
	if (!cPurpose.IsEmpty())
		//cPurpose.clear();
		cPurpose.Empty();
	int itSource = 0;
	int iSize = cSource.GetLength();
	for(size_t iIndex = 0; iIndex < (size_t)cFormat.GetLength() && itSource<iSize; iIndex++)
	{
		char letter = cFormat[iIndex];
		switch(letter)
		{
		case '?':
			//cPurpose.push_back(cSource[itSource]);
			cPurpose += cSource[itSource];
			itSource++;
			break;
		case 'X':
			{
				while(itSource < iSize)
				{
					if (cSource[itSource]>='0'&&cSource[itSource]<='9' || cSource[itSource]>='a' && cSource[itSource]<='z' || cSource[itSource]>='A' && cSource[itSource]<='Z')
					{
						//cPurpose.push_back(cSource[itSource]);
						cPurpose += cSource[itSource];
						itSource++;
						break;
					}
					itSource++;
				}
				break;
			}
			break;
		case 'A':
			{
				while(itSource < iSize)
				{
					if (cSource[itSource]>='a' && cSource[itSource]<='z' || cSource[itSource]>='A' && cSource[itSource]<='Z')
					{
						//cPurpose.push_back(cSource[itSource]);
						cPurpose += cSource[itSource];
						itSource++;
						break;
					}
					itSource++;
				}
				break;
			}
			break;
		case '9':
			{
				while(itSource < iSize)
				{
					if (cSource[itSource]>='0'&&cSource[itSource]<='9')
					{
						//cPurpose.push_back(cSource[itSource]);
						cPurpose += cSource[itSource];
						itSource++;
						break;
					}
					itSource++;
				}
				break;
			}
		case '*':
			{
				CFX_ByteString bsAppend = cSource.Mid(itSource,iSize-itSource);
				cPurpose += bsAppend;
				//cPurpose.append(cSource,itSource,iSize-itSource);
				itSource = iSize-1;
				break;
			}
		case '\\':
			break;
		case '>':
			{
				cSource.MakeUpper();
// 				for(std::string::iterator it = cSource.begin();it != cSource.end(); it++)
// 				{
// 					*it = toupper(*it);
// 				}
				break;
			}
		case '<':
			{
				cSource.MakeLower();
// 				for(std::string::iterator it = cSource.begin();it != cSource.end(); it++)
// 				{
// 					*it = tolower(*it);
// 				}
				break;
			}
		case '=':
			break;
		default:
			//cPurpose.push_back(letter);
			cPurpose += letter;
			break;
		}
	}
}

FX_BOOL jutil::scand(OBJ_METHOD_PARAMS)
{
	int iSize = params.size();
	if (iSize < 2)
		return FALSE;
	CFX_WideString sFormat = (FX_LPCWSTR)params[0];
	CFX_WideString sDate = (FX_LPCWSTR)params[1];

	double dDate = DS_GetDateTime();
	if (sDate.GetLength() > 0)
	{
		FX_BOOL bWrongFormat = FALSE;
		dDate = CFXJS_PublicMethods::MakeRegularDate(sDate,sFormat,bWrongFormat);
	}	
	
	if (!DS_PortIsNan(dDate))
	{
		CFXJS_Date date(dDate);
		vRet = date;
	}
	else
	{
		vRet.SetNull();
	}

	return TRUE;
}

FX_INT64 JFX_atoi64(const char *nptr)
{
        int c;              /* current char */
        FX_INT64 total;      /* current total */
        int sign;           /* if '-', then negative, otherwise positive */

        /* skip whitespace */
        while ( isspace((int)(unsigned char)*nptr) )
            ++nptr;

        c = (int)(unsigned char)*nptr++;
        sign = c;           /* save sign indication */
        if (c == '-' || c == '+')
            c = (int)(unsigned char)*nptr++;    /* skip sign */

        total = 0;

        while (isdigit(c)) {
            total = 10 * total + (c - '0');     /* accumulate digit */
            c = (int)(unsigned char)*nptr++;    /* get next char */
        }

        if (sign == '-')
            return -total;
        else
            return total;   /* return result, negated if necessary */
}

FX_BOOL jutil::byteToChar(OBJ_METHOD_PARAMS)
{
	int iSize = params.size();
	if (iSize == 0)
		return FALSE;
	int nByte = (int)params[0];
	unsigned char cByte = (unsigned char)nByte;
	CFX_WideString csValue;
	csValue.Format((FX_LPCWSTR)L"%c", cByte);
	vRet = csValue; 
	return TRUE;
}

FX_INT32 find_last_of(const CFX_WideString& wsString, FX_WCHAR wch)
{
	int length = wsString.GetLength();
	if(length == 0)
		return -1;
	int i = 0;
	for(i=length-1; i>=0; i--)
	{
		FX_LPCWSTR pWstr = (FX_LPCWSTR)wsString;
		if(pWstr[i] == wch)
			return i;
	}
	return i;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)