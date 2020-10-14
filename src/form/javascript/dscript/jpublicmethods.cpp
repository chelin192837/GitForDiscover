#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "publicmethods.h"
#include "util.h"
#include "field.h"
#include "color.h"


/* -------------------------------- CJS_PublicMethods -------------------------------- */

#define DOUBLE_CORRECT	0.000000000000001

BEGIN_JS_STATIC_GLOBAL_FUN(CFXJS_PublicMethods)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFNumber_Format,6)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFNumber_Keystroke,6)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFPercent_Format,2)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFPercent_Keystroke,2)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFDate_FormatEx,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFDate_KeystrokeEx,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFDate_Format,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFDate_Keystroke,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFTime_FormatEx,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFTime_KeystrokeEx,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFTime_Format,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFTime_Keystroke,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFSpecial_Format,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFSpecial_Keystroke,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFSpecial_KeystrokeEx,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFSimple,3)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFMakeNumber,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFSimple_Calculate,2)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFRange_Validate,4)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFMergeChange,1)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFParseDateEx,2)
	JS_STATIC_GLOBAL_FUN_ENTRY(AFExtractNums,1)
END_JS_STATIC_GLOBAL_FUN()

IMPLEMENT_JS_STATIC_GLOBAL_FUN(CFXJS_PublicMethods)

struct stru_TbConvert
{
	FX_LPCSTR lpszJSMark;
	FX_LPCSTR lpszCppMark;
};

static const stru_TbConvert csTable[] = {"mmmm","%B",
	"mmm", "%b",
	"mm",  "%m",
	//"m"
	"dddd","%A",
	"ddd", "%a",
	"dd",  "%d",
	//"d",   "%w",
	"yyyy","%Y",
	"yy",  "%y",
	"HH",  "%H",
	//"H"
	"hh",  "%I",
	//"h"
	"MM",  "%M",
	//"M"
	"ss",  "%S",
	//"s
	"tt",  "%p"
	//"t"
};

static FX_LPCWSTR months[] =
{
	(FX_LPCWSTR)L"Jan", (FX_LPCWSTR)L"Feb", (FX_LPCWSTR)L"Mar", (FX_LPCWSTR)L"Apr", (FX_LPCWSTR)L"May", (FX_LPCWSTR)L"Jun", (FX_LPCWSTR)L"Jul", (FX_LPCWSTR)L"Aug", (FX_LPCWSTR)L"Sep", (FX_LPCWSTR)L"Oct", (FX_LPCWSTR)L"Nov", (FX_LPCWSTR)L"Dec"
};

static FX_LPCWSTR fullmonths[] = 
{ 
	(FX_LPCWSTR)L"January", (FX_LPCWSTR)L"February", (FX_LPCWSTR)L"March", (FX_LPCWSTR)L"April", (FX_LPCWSTR)L"May", (FX_LPCWSTR)L"June", (FX_LPCWSTR)L"July", (FX_LPCWSTR)L"August", (FX_LPCWSTR)L"September", (FX_LPCWSTR)L"October", (FX_LPCWSTR)L"November", (FX_LPCWSTR)L"December" 
};

FX_BOOL CFXJS_PublicMethods::IsNumber(FX_LPCWSTR string)
{
	CFX_WideString sTrim = StrTrim(string);
	FX_LPCWSTR pTrim = sTrim;
	FX_LPCWSTR p = pTrim;

	FX_BOOL bDigits = TRUE;
	FX_BOOL bDot = FALSE;
	FX_BOOL bKXJS = FALSE;

	wchar_t c;
	while (c = *p)
	{
		if (c == '.' || c == ',')
		{
			if (bDot) return FALSE;
			bDot = TRUE;
		}
		else if (c == '-' || c == '+')
		{
			if (p != pTrim)
				return FALSE;
		}
		else if (c == 'e' || c == 'E')
		{
			if (bKXJS) return FALSE;

			p++;
			c = *p;
			if (c == '+' || c == '-')
			{
				bKXJS = TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if (!IsDigit(c))
		{
			return FALSE;
		}
		p++;
	}

	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::IsDigit(wchar_t ch)
{
	return (ch >= L'0' && ch <= L'9');
}

FX_BOOL CFXJS_PublicMethods::IsDigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

FX_BOOL CFXJS_PublicMethods::IsAlphabetic(wchar_t ch)
{
	return ((ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z'));
}

FX_BOOL CFXJS_PublicMethods::IsAlphaNumeric(wchar_t ch)
{
	return (IsDigit(ch) || IsAlphabetic(ch));
}

FX_BOOL CFXJS_PublicMethods::maskSatisfied(wchar_t c_Change,wchar_t c_Mask)
{
	switch (c_Mask)
	{
	case L'9':
        return IsDigit(c_Change);		
    case L'A':
        return IsAlphabetic(c_Change);		
    case L'O':
        return IsAlphaNumeric(c_Change);		
    case L'X':
        return TRUE;		
	default:
        return (c_Change == c_Mask);
	}
}

FX_BOOL CFXJS_PublicMethods::isReservedMaskChar(wchar_t ch)
{
	return ch == L'9' || ch == L'A' || ch == L'O' || ch == L'X';
}

double CFXJS_PublicMethods::AF_Simple(FX_LPCWSTR sFuction, double dValue1, double dValue2)
{
	if (FXSYS_wcsicmp(sFuction,(FX_LPCWSTR)L"AVG") == 0 || FXSYS_wcsicmp(sFuction,(FX_LPCWSTR)L"SUM") == 0)
	{
		return dValue1 + dValue2;
	}
	else if (FXSYS_wcsicmp(sFuction, (FX_LPCWSTR)L"PRD") == 0)
	{
		return dValue1 * dValue2;
	}
	else if (FXSYS_wcsicmp(sFuction,(FX_LPCWSTR)L"MIN") == 0)
	{
		return FX_MIN(dValue1, dValue2);
	}
	else if (FXSYS_wcsicmp(sFuction,(FX_LPCWSTR)L"MAX") == 0)
	{
		return FX_MAX(dValue1, dValue2);
	}

	return dValue1;
}

CFX_WideString CFXJS_PublicMethods::StrLTrim(FX_LPCWSTR pStr)
{
	while (*pStr && *pStr == L' ') pStr++;

	return pStr;
}

CFX_WideString CFXJS_PublicMethods::StrRTrim(FX_LPCWSTR pStr)
{
	FX_LPCWSTR p = pStr;

	while (*p) p++;
	p--;
	if (p >= pStr)
	{		
		while (*p && *p == L' ') p--;
		p++;
		return CFX_WideString(pStr,p-pStr);
	}
	return L"";
}

CFX_WideString CFXJS_PublicMethods::StrTrim(FX_LPCWSTR pStr)
{
	return StrRTrim(StrLTrim(pStr));
}

CFX_ByteString CFXJS_PublicMethods::StrLTrim(FX_LPCSTR pStr)
{
	while (*pStr && *pStr == ' ') pStr++;

    return pStr;
}

CFX_ByteString CFXJS_PublicMethods::StrRTrim(FX_LPCSTR pStr)
{
	FX_LPCSTR p = pStr;

	while (*p) p++;
	p--;
	if (p >= pStr)
	{		
		while (*p && *p == ' ') p--;
		p++;
		return CFX_ByteString(pStr,p-pStr);
	}
	return "";
}

CFX_ByteString CFXJS_PublicMethods::StrTrim(FX_LPCSTR pStr)
{
	return StrRTrim(StrLTrim(pStr));
}

double CFXJS_PublicMethods::ParseNumber(FX_LPCWSTR swSource, FX_BOOL& bAllDigits, FX_BOOL& bDot, FX_BOOL& bSign, FX_BOOL& bKXJS)
{
	bDot = FALSE;
	bSign = FALSE;
	bKXJS = FALSE;

	FX_BOOL bDigitExist = FALSE;

	FX_LPCWSTR p = swSource;
	wchar_t c;

	FX_LPCWSTR pStart = NULL;
	FX_LPCWSTR pEnd = NULL;

	while (c = *p)
	{
		if (!pStart && c != L' ')
		{
			pStart = p;
		}

		pEnd = p;
		p++;
	}

	if (!pStart)
	{
		bAllDigits = FALSE;
		return 0;
	}

	while (pEnd != pStart)
	{
		if (*pEnd == L' ')
			pEnd --;
		else
			break;
	}

	double dRet = 0;
	p = pStart;
	bAllDigits = TRUE;
	CFX_WideString swDigits;

	while (p <= pEnd)
	{	
		c = *p;

		if (IsDigit(c))
		{
			swDigits += c;
			bDigitExist = TRUE;
		}
		else 
		{
			switch (c)
			{
			case L' ':
				bAllDigits = FALSE;
				break;
			case L'.':
			case L',':
				if (!bDot)
				{
					if (bDigitExist)
					{
						swDigits += L'.';
					}
					else
					{
						swDigits += L'0';
						swDigits += L'.';
						bDigitExist = TRUE;
					}

					bDot = TRUE;
					break;
				}
			case 'e':
			case 'E':
				if (!bKXJS)
				{
					p++;
					c = *p;
					if (c == '+' || c == '-')
					{
						bKXJS = TRUE;
						swDigits += 'e';
						swDigits += c;
					}
					break;
				}
			case L'-':
				if (!bDigitExist && !bSign)
				{
					swDigits += c;
					bSign = TRUE;
					break;
				}
			default:
				bAllDigits = FALSE;

				if (p != pStart && !bDot && bDigitExist)
				{
					swDigits += L'.';
					bDot = TRUE;
				}
				else
				{
					bDot = FALSE;
					bDigitExist = FALSE;
					swDigits = L"";
				}
				break;
			}
		}

		p++;
	}

	if (swDigits.GetLength() > 0 && swDigits.GetLength() < 17)
	{
		CFX_ByteString sDigits = swDigits.UTF8Encode();

		if (bKXJS)
		{
			dRet = atof(sDigits);
		}
		else
		{
			if (bDot)
			{
				char* pStopString;
				dRet = ::strtod(sDigits, &pStopString);
			}
			else
			{
				dRet = atol(sDigits);
			}
		}

	}

	return dRet;
}

double CFXJS_PublicMethods::ParseStringToNumber(FX_LPCWSTR swSource)
{
	FX_BOOL bAllDigits = FALSE;
	FX_BOOL bDot = FALSE;
	FX_BOOL bSign = FALSE;
	FX_BOOL bKXJS = FALSE;

	return ParseNumber(swSource, bAllDigits, bDot, bSign, bKXJS);
}

FX_BOOL	CFXJS_PublicMethods::ConvertStringToNumber(FX_LPCWSTR swSource, double & dRet, FX_BOOL & bDot)
{
	FX_BOOL bAllDigits = FALSE;
	FX_BOOL bSign = FALSE;
	FX_BOOL bKXJS = FALSE;

	dRet = ParseNumber(swSource, bAllDigits, bDot, bSign, bKXJS);

	return bAllDigits;
}

CFXJS_Array CFXJS_PublicMethods::AF_MakeArrayFromList(CFXJS_Value val)
{
	CFXJS_Array StrArray;
	if(val.IsArrayObject())
	{
		val.ConvertToArray(StrArray);
		return StrArray;
	}
	CFX_WideString wsStr = (FX_LPCWSTR)val;
	CFX_ByteString t = CFX_ByteString::FromUnicode(wsStr);
	const char * p = (const char *)t;
	int nLen = strlen(p);

	int ch = ',' ;
	int nIndex = 0;

	while (*p)
	{
		const char * pTemp = strchr(p, ch);
		if (pTemp == NULL)
		{
			StrArray.SetElement(nIndex, (FX_LPCSTR)StrTrim(p));
			break;
		}
		else
		{
			char * pSub = new char[pTemp - p + 1];
#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
			strncpy_s(pSub, pTemp - p, p, pTemp - p);
#else
			strncpy(pSub, p, pTemp - p);
#endif
			*(pSub + (pTemp - p)) = '\0';

			StrArray.SetElement(nIndex, (FX_LPCSTR)StrTrim(pSub));
			delete []pSub;
			
			nIndex ++;
			p = ++pTemp;
		}
		
	}
	return StrArray;
}

int CFXJS_PublicMethods::ParseStringInteger(const CFX_WideString& string,int nStart,int& nSkip, int nMaxStep)
{
	int nRet = 0;
	nSkip = 0;
	for (int i=nStart, sz=string.GetLength(); i < sz; i++)
	{
		if (i-nStart > 10)
			break;

		FX_WCHAR c = string.GetAt(i);
		if (IsDigit((wchar_t)c))
		{
			nRet = nRet * 10 + (c - '0');
			nSkip = i - nStart + 1;
			if (nSkip >= nMaxStep) 
				break;
		}
		else
			break;
	}

	return nRet;
}

CFX_WideString CFXJS_PublicMethods::ParseStringString(const CFX_WideString& string, int nStart, int& nSkip)
{
	CFX_WideString swRet;
	nSkip = 0;
	for (int i=nStart, sz=string.GetLength(); i < sz; i++)
	{
		FX_WCHAR c = string.GetAt(i);
		if ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z'))
		{
			swRet += c;
			nSkip = i - nStart + 1;
		}
		else
			break;
	}

	return swRet;
}

FX_BOOL CFXJS_PublicMethods::IsPM(const CFX_WideString& value, FX_BOOL bDouble, int nStart, int& nSkip)
{
	FX_INT32 length = value.GetLength() - nStart;
	CFX_WideString wsPM = value.Mid(nStart, length);
	wsPM.TrimLeft();

	nSkip = length - wsPM.GetLength();
	if (wsPM.GetLength() < 1)
		return FALSE;

	CFX_WideString wsFirst = wsPM.GetAt(0), wsSecond;
	if (bDouble)
	{
		if (wsPM.GetLength() < 2) return FALSE;

		nSkip += 2;
		wsSecond = wsPM.GetAt(1);
		return (wsFirst.CompareNoCase(FX_LPCWSTR("p")) == 0 && wsSecond.CompareNoCase(FX_LPCWSTR("m")) == 0);
	}
	else
	{
		nSkip += 1;
		
		wsFirst = wsPM.GetAt(0);
		return (wsFirst.CompareNoCase(FX_LPCWSTR("p")) == 0);
	}

	return FALSE;
}

double CFXJS_PublicMethods::ParseNormalDate(const CFX_WideString & value, FX_BOOL& bWrongFormat)
{
	double dt = DS_GetDateTime();

	int nYear = DS_GetYearFromTime(dt);
	int nMonth = DS_GetMonthFromTime(dt) + 1;
	int nDay = DS_GetDayFromTime(dt);
	int nHour = 0;//DS_GetHourFromTime(dt);
	int nMin = 0;//DS_GetMinFromTime(dt);
	int nSec = 0;//DS_GetSecFromTime(dt);

	int number[3];

	int nSkip = 0;
	int nLen = value.GetLength();
	int nIndex = 0;
	int i = 0;
	while (i < nLen)
	{
		if (nIndex > 2) break;

		FX_WCHAR c = value.GetAt(i);
		if (IsDigit((wchar_t)c))
		{
			number[nIndex++] = ParseStringInteger(value, i, nSkip, 4);
			i += nSkip;			
		}
		else
		{
			i ++;
		}
	}

	if (nIndex == 2)
	{
		// case2: month/day
		// case3: day/month
		if ((number[0] >= 1 && number[0] <= 12) && (number[1] >= 1 && number[1] <= 31))
		{
			nMonth = number[0];
			nDay = number[1];
		}
		else if ((number[0] >= 1 && number[0] <= 31) && (number[1] >= 1 && number[1] <= 12))
		{
			nDay = number[0];
			nMonth = number[1];
		}
		else
			return 0;

		bWrongFormat = FALSE;
	}
	else if (nIndex == 3)
	{
		// case1: year/month/day
		// case2: month/day/year
		// case3: day/month/year

		if (number[0] > 12 && (number[1] >= 1 && number[1] <= 12) && (number[2] >= 1 && number[2] <= 31))
		{
			nYear = number[0];
			nMonth = number[1];
			nDay = number[2];
		}
		else if ((number[0] >= 1 && number[0] <= 12) && (number[1] >= 1 && number[1] <= 31) && number[2] > 31)
		{
			nMonth = number[0];
			nDay = number[1];
			nYear = number[2];
		}
		else if ((number[0] >= 1 && number[0] <= 31) && (number[1] >= 1 && number[1] <= 12) && number[2] > 31)
		{
			nDay = number[0];
			nMonth = number[1];
			nYear = number[2];
		}
		else
			return 0;

		bWrongFormat = FALSE;
	}
	else
	{
		bWrongFormat = TRUE;
		return dt;
	}

	CFX_WideString swTemp;
	swTemp.Format((FX_LPCWSTR)L"%d/%d/%d %d:%d:%d",nMonth,nDay,nYear,nHour,nMin,nSec);
	return DS_DateParse(swTemp);
}

double CFXJS_PublicMethods::MakeRegularDate(const CFX_WideString & value, const CFX_WideString & format, FX_BOOL& bWrongFormat)
{
	double dt = DS_GetDateTime();

	if (format.IsEmpty() || value.IsEmpty())
		return dt;

	int nYear = DS_GetYearFromTime(dt);
	int nMonth = DS_GetMonthFromTime(dt) + 1;
	int nDay = DS_GetDayFromTime(dt);
	int nHour = 0;//DS_GetHourFromTime(dt);
	int nMin = 0;//DS_GetMinFromTime(dt);
	int nSec = 0;//DS_GetSecFromTime(dt);

	int nYearSub = 99; //nYear - 2000;

	FX_BOOL bPm = FALSE;
	FX_BOOL bExit = FALSE;
	bWrongFormat = FALSE;
	
	int i=0;
	int j=0;

	while (i < format.GetLength())
	{
		if (bExit) break;

		FX_WCHAR c = format.GetAt(i);		
		switch (c)
		{
/*			case ',':
			i++;
			break;

			case ' ':
			case ':':
			case '.':
			case '-':
			case '\\':
			case '/':
			i++;
			if (value.GetAt(j) != ' ' && value.GetAt(j) != ':' && value.GetAt(j) != '.' 
			&& value.GetAt(j) != '-' && value.GetAt(j) != '\\' && value.GetAt(j) != '/' )
			{
			bWrongFormat = TRUE;
			bExit = TRUE;
			}
			j++;
			break;		*/	
			case 'y':
			case 'm':
			case 'd':
			case 'H':
			case 'h':
			case 'M':
			case 's':
			case 't':
				{
					int oldj = j;
					int nSkip = 0;

					if (format.GetAt(i+1) != c)
					{
						switch (c)
						{
							case 'y':
								i++;
								j++;
								break;
							case 'm':
								nMonth = ParseStringInteger(value, j, nSkip, 2);
								i++;
								j += nSkip;
								break;
							case 'd':
								nDay = ParseStringInteger(value, j, nSkip, 2);
								i++;
								j += nSkip;
								break;
							case 'H':
								nHour = ParseStringInteger(value, j, nSkip, 2);
								i++;
								j += nSkip;
								break;
							case 'h':
								nHour = ParseStringInteger(value, j, nSkip, 2);
								i++;
								j += nSkip;
								break;
							case 'M':
								nMin = ParseStringInteger(value, j, nSkip, 2);
								i++;
								j += nSkip;
								break;
							case 's':
								nSec = ParseStringInteger(value, j, nSkip, 2);
								i++;
								j += nSkip;
								break;
							case 't':
								bPm = IsPM(value, FALSE, j, nSkip);
								i++;
								j += nSkip;
								break;
						}					
					}
					else if (format.GetAt(i+1) == c && format.GetAt(i+2) != c)
					{
						switch (c)
						{
							case 'y':
								nYear = ParseStringInteger(value, j, nSkip, 4);
								i += 2;
								j += nSkip;
								break;
							case 'm':
								nMonth = ParseStringInteger(value, j, nSkip, 2);
								i += 2;
								j += nSkip;
								break;
							case 'd':
								nDay = ParseStringInteger(value, j, nSkip, 2);
								i += 2;
								j += nSkip;
								break;
							case 'H':
								nHour = ParseStringInteger(value, j, nSkip, 2);
								i += 2;
								j += nSkip;
								break;
							case 'h':
								nHour = ParseStringInteger(value, j, nSkip, 2);
								i += 2;
								j += nSkip;
								break;
							case 'M':
								nMin = ParseStringInteger(value, j, nSkip, 2);
								i += 2;
								j += nSkip;
								break;
							case 's':
								nSec = ParseStringInteger(value, j, nSkip, 2);
								i += 2;
								j += nSkip;
								break;
							case 't':
								bPm = IsPM(value, TRUE, j, nSkip);
								j += nSkip;
								i += 2;
								break;
						}
					}
					else if (format.GetAt(i+1) == c && format.GetAt(i+2) == c && format.GetAt(i+3) != c)
					{
						switch (c)
						{
							case 'm':
								{
									CFX_WideString sMonth = ParseStringString(value, j, nSkip);
									FX_BOOL bFind = FALSE;
									if (!sMonth.IsEmpty())
									{
										for (int m = 0; m < 12; m++)
										{
											if (sMonth.CompareNoCase(months[m]) == 0)
											{
												nMonth = m + 1;
												i+=3;
												j+=nSkip;
												bFind = TRUE;
												break;
											}
										}
									}
									
									if (!bFind)
									{
										nMonth = ParseStringInteger(value, j, nSkip, 3);
										i+=3;
										j += nSkip;
									}
								}
								break;
							case 'y':
								break;
							default:
								i+=3;
								j+=3;
								break;
						}
					}
					else if (format.GetAt(i+1) == c && format.GetAt(i+2) == c && format.GetAt(i+3) == c && format.GetAt(i+4) != c)
					{
						switch (c)
						{
							case 'y':
								nYear = ParseStringInteger(value, j, nSkip, 4);
								j += nSkip;
								i += 4;
								break;
							case 'm':
								{
									FX_BOOL bFind = FALSE;

									CFX_WideString sMonth = ParseStringString(value, j, nSkip);
									sMonth.MakeLower();
									if (!sMonth.IsEmpty())
									{
										for (int m = 0; m < 12; m++)
										{
											CFX_WideString sFullMonths = fullmonths[m];
											sFullMonths.MakeLower();
											if (sFullMonths.Find(sMonth, 0) != -1)
											{
												nMonth = m + 1;
												i += 4;
												j += nSkip;
												bFind = TRUE;
												break;
											}
										}
									}
									
									if (!bFind)
									{
										nMonth = ParseStringInteger(value, j, nSkip, 4);
										i+=4;
										j += nSkip;
									}
								}
								break;
							default:
								i += 4;
								j += 4;
								break;
						}					
					}
					else
					{
						if (format.GetAt(i) != value.GetAt(j))
						{
							bWrongFormat = TRUE;
							bExit = TRUE;
						}
						i++;
						j++;
					}
					
					if (oldj == j)
					{
						bWrongFormat = TRUE;
						bExit = TRUE;
					}
				}

				break;			
			default:
				if (value.GetLength() <= j)
				{
					bExit = TRUE;
				}
				//else if (format.GetAt(i) != value.GetAt(j))
				//{
				//	bWrongFormat = TRUE;
				//	bExit = TRUE;
				//}
				else
				{
					FX_BOOL bDigitF = IsDigit((wchar_t)format.GetAt(i));
					FX_BOOL bDigitV = IsDigit((wchar_t)value.GetAt(j));
					if ((bDigitF && !bDigitV) || (!bDigitF && bDigitV))
					{
						bWrongFormat = TRUE;
						bExit = TRUE;
					}
				}
				i++;
				CFX_WideString wsPM = value.GetAt(j);
				if (wsPM.CompareNoCase(L"a") != 0 && wsPM.CompareNoCase(L"p") != 0)
					j++;
				break;
		}		
	}

	if (bPm && nHour <= 12)
		nHour += 12;

	if (nYear >= 0 && nYear <= nYearSub)
		nYear += 2000;

	if (nMonth < 1 || nMonth > 12)
		bWrongFormat = TRUE;

	if (nDay < 1 || nDay > 31)
		bWrongFormat = TRUE;

	if (nHour < 0 || nHour > 24)
		bWrongFormat = TRUE;

	if (nMin < 0 || nMin > 60)
		bWrongFormat = TRUE;

	if (nSec < 0 || nSec > 60)
		bWrongFormat = TRUE;

	double dRet = 0;

	if (bWrongFormat)
	{
		//dRet = DS_GetDateTime();

		//modify by zhh 2014/7/22 Format error Keep the input content
	    dRet = ParseNormalDate(value, bWrongFormat);
		if (bWrongFormat)
			return 0;
		//end modify.

//		dRet = DS_DateParse((FX_LPCWSTR)value);
//		dRet = DS_MakeDate(DS_MakeDay(nYear,nMonth - 1,nDay),DS_MakeTime(nHour,nMin,nSec,0));
	}
	else
	{
		dRet = DS_MakeDate(DS_MakeDay(nYear,nMonth - 1,nDay),DS_MakeTime(nHour, nMin, nSec, 0));

		if (DS_PortIsNan(dRet))
		{
			dRet = DS_DateParse(value);
		}
	}

	if (DS_PortIsNan(dRet))
	{
		dRet = ParseNormalDate(value, bWrongFormat);
	}

	return dRet;

//	return DS_MakeDate(DS_MakeDay(nYear, nMonth - 1, nDay),DS_MakeTime(nHour,nMin,nSec,0));
}

CFX_WideString CFXJS_PublicMethods::MakeFormatDate(double dDate, const CFX_WideString & format)
{
	CFX_WideString sRet = L"",sPart = L"";
	int nYear = DS_GetYearFromTime(dDate);
	int nMonth = DS_GetMonthFromTime(dDate) + 1;
	int nDay = DS_GetDayFromTime(dDate);
	int nHour = DS_GetHourFromTime(dDate);
	int nMin = DS_GetMinFromTime(dDate);
	int nSec = DS_GetSecFromTime(dDate);
	int i = 0;
	FX_WCHAR c;
	while (i < format.GetLength())
	{
		c = format.GetAt(i);
		sPart = L"";
		switch (c)
		{
			case 'y':
			case 'm':
			case 'd':
			case 'H':
			case 'h':
			case 'M':
			case 's':
			case 't':
				if (format.GetAt(i+1) != c)
				{
					switch (c)
					{
						case 'y':
							sPart += c;
							break;
						case 'm':
							sPart.Format((FX_LPCWSTR)L"%d",nMonth);
							break;
						case 'd':
							sPart.Format((FX_LPCWSTR)L"%d",nDay);
							break;
						case 'H':
							sPart.Format((FX_LPCWSTR)L"%d",nHour);
							break;
						case 'h':
							sPart.Format((FX_LPCWSTR)L"%d",nHour>12?nHour - 12:nHour);
							break;
						case 'M':
							sPart.Format((FX_LPCWSTR)L"%d",nMin);
							break;
						case 's':
							sPart.Format((FX_LPCWSTR)L"%d",nSec);
							break;
						case 't':				
							sPart += nHour>12?'p':'a';
							break;
					}					
					i++;
				}
				else if (format.GetAt(i+1) == c && format.GetAt(i+2) != c)
				{
					CFX_ByteString bsPart = "";
					switch (c)
					{
					case 'y':
						bsPart.Format("%02d",nYear - (nYear / 100) * 100);
						break;
					case 'm':
						bsPart.Format("%02d",nMonth);
						break;
					case 'd':
						bsPart.Format("%02d",nDay);
						break;
					case 'H':
						bsPart.Format("%02d",nHour);
						break;
					case 'h':
						bsPart.Format("%02d",nHour>12?nHour - 12:nHour);
						break;
					case 'M':
						bsPart.Format("%02d",nMin);
						break;
					case 's':
						bsPart.Format("%02d",nSec);
						break;
					case 't':
						bsPart = nHour>12? "pm": "am";
						break;
					}
					sPart.ConvertFrom(bsPart);
					i+=2;
				}
				else if (format.GetAt(i+1) == c && format.GetAt(i+2) == c && format.GetAt(i+3) != c)
				{		
					switch (c)
					{
						case 'm':
							i+=3;
							sPart += months[nMonth - 1];
							break;
						default:
							i+=3;
							sPart += c;
							sPart += c;
							sPart += c;
							break;
					}					
				}
				else if (format.GetAt(i+1) == c && format.GetAt(i+2) == c && format.GetAt(i+3) == c && format.GetAt(i+4) != c)
				{
					switch (c)
					{
						case 'y':
							sPart.Format((FX_LPCWSTR)L"%04d",nYear);
							i += 4;
							break;	
						case 'm':
							i+=4;
							sPart += fullmonths[nMonth - 1];
							break;
						default:
							i += 4;
							sPart += c;
							sPart += c;
							sPart += c;
							sPart += c;
							break;
					}					
				}
				else
				{
					i++;
					sPart += c;
				}
				break;			
			default:
				i++;
				sPart += c;
				break;
		}
		
		sRet += sPart;
	}

	return sRet;
}

/* -------------------------------------------------------------------------- */
#define CVTBUFSIZE 64
char * mycvt( double arg, int ndigits, int * decpt, int * sign, char * buf, int eflag )
{
    int r2;
    double fi, fj;
    char * p, * p1;
    if ( ndigits < 0 ) ndigits = 0;
    if ( ndigits >= CVTBUFSIZE - 1 ) ndigits = CVTBUFSIZE - 2;
    r2 = 0;
    * sign = 0;
    p = & buf[0];
    if ( arg < 0 )
    {
        * sign = 1;
        arg = - arg;
	}
    arg = modf( arg, & fi );
    p1 = & buf[CVTBUFSIZE];
    if ( fi != 0 )
    {
        p1 = & buf[CVTBUFSIZE];
        while ( fi != 0 )
        {
            fj = modf( fi / 10, & fi );
            *-- p1 = ( int )(( fj + .03 ) * 10 ) + '0';
            r2 ++;     
		}
        while ( p1 < & buf[CVTBUFSIZE] ) * p ++ = * p1 ++;
	}
    else if ( arg > 0 )
    {
        while (( fj = arg * 10 ) < 1 )
        {
            arg = fj;
            r2 --;
		} 
	}
    p1 = & buf[ndigits];
    if ( eflag == 0 ) p1 += r2;
    * decpt = r2;
    if ( p1 < & buf[0] )
    {
        buf[0] = '\0';
        return buf;
	}
    while ( p <= p1 && p < & buf[CVTBUFSIZE] )
    {
        arg *= 10;
        arg = modf( arg, & fj );
        * p ++ = ( int ) fj + '0';
	}
    if ( p1 >= & buf[CVTBUFSIZE] )
    {
        buf[CVTBUFSIZE - 1] = '\0';
        return buf;
	}
    p = p1;
    * p1 += 5;
    while (* p1 > '9' )
    {
        * p1 = '0';
        if ( p1 > buf )
            ++*-- p1;
        else
        {
            * p1 = '1';
            (* decpt )++;
            if ( eflag == 0 )
            {
                if ( p > buf ) * p = '0';
                p ++;
			}
		} 
	}
    * p = '\0';
    return buf;
}
bool myfcvt(double value,int count, int *dec, char * strBuf)
{	
	int neg = 0;
	mycvt(value,count,dec,&neg, (char*)strBuf,0);
	if(value < 0)
		return TRUE;
	else
		return FALSE;
}
//function AFNumber_Format(nDec, sepStyle, negStyle, currStyle, strCurrency, bCurrencyPrepend)
FX_BOOL CFXJS_PublicMethods::AFNumber_Format(OBJ_METHOD_PARAMS)
{

	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (params.size() != 6)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}
	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString& Value = pEvent->Value();	
	CFX_ByteString strValue = StrTrim(CFX_ByteString::FromUnicode(Value));
	
	if (strValue.IsEmpty()) return TRUE;
	
	int iDec = params[0];
	int iSepStyle = params[1];
	int iNegStyle = params[2];
	int icurrStyle = params[3]; //it's no use!
	CFX_WideString wstrCurrency((FX_LPCWSTR)params[4]);
	FX_BOOL bCurrencyPrepend = params[5];
	
	if (iDec < 0) iDec = -iDec;
	
	if (iSepStyle < 0 || iSepStyle > 3)
		iSepStyle = 0;
	
	if (iNegStyle < 0 || iNegStyle > 3)
		iNegStyle = 0;
	
	
	//////////////////////////////////////////////////////
	//for processing decimal places
	strValue.Replace(",", ".");
	double dValue = atof(strValue);
	if (iDec > 0)
		dValue += DOUBLE_CORRECT;//校正
		    
	int iDec2 = 0;
	FX_BOOL bNagative = FALSE;
	
#if defined(WINCE) || defined(_WIN32) || defined(_WIN64)
	strValue = _fcvt(dValue,iDec,&iDec2,&bNagative);
#else
	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
	if (fabs(dValue) > DOUBLE_CORRECT)
		_fcvt_s(strValue.LockBuffer(), strValue.GetLength(), dValue,iDec,&iDec2,&bNagative);
	#else
		#if _FX_OS_ != _FX_ANDROID_ && _FX_OS_ != _FX_IOS_
				strValue = fcvt(dValue,iDec,&iDec2,&bNagative);
		#else
				char chFloat[256] = {0};
				bNagative = myfcvt(dValue,iDec,&iDec2,chFloat);
				strValue = chFloat;
		#endif
	#endif
#endif
	if (strValue.IsEmpty())
	{
		dValue = 0;
#if defined(WINCE) || defined(_WIN32) || defined(_WIN64)
		strValue = _fcvt(dValue,iDec,&iDec2,&bNagative);
#else
	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
		if (fabs(dValue) > DOUBLE_CORRECT)
			_fcvt_s(strValue.LockBuffer(), strValue.GetLength(), dValue,iDec,&iDec2,&bNagative);
	#else
#if _FX_OS_ != _FX_ANDROID_	
		strValue = fcvt(dValue,iDec,&iDec2,&bNagative);
#else
		bNagative = myfcvt(dValue,iDec,&iDec2,chFloat);
		strValue = chFloat;
#endif
	#endif
#endif
		if (strValue.IsEmpty())
		{
			strValue = "0";
			iDec2 = 1;
		}

	}
	
	if (iDec2 < 0)
	{
		for (int iNum = 0;iNum < abs(iDec2);iNum++)
		{
			strValue = "0" + strValue;
		}
		iDec2 = 0;
		
	}
	int iMax = strValue.GetLength();
	if (iDec2 > iMax)
	{
		for (int iNum = 0;iNum <= iDec2 - iMax ;iNum++)
		{
			strValue += "0";
		}
		iMax = iDec2+1;			
	}
	///////////////////////////////////////////////////////
    //for processing seperator style
	if (iDec2 < iMax)
	{
		if (iSepStyle == 0 || iSepStyle == 1)
		{
			strValue.Insert(iDec2, '.');
			iMax++;
		}
		else if (iSepStyle == 2 || iSepStyle == 3)
		{
			strValue.Insert(iDec2, ',');
			iMax++;
		}
		
		if (iDec2 == 0)
			strValue.Insert(iDec2, '0');
	}
	if (iSepStyle == 0 || iSepStyle == 2)
	{
		char cSeperator;
		if (iSepStyle == 0)
			cSeperator = ',';
		else
			cSeperator = '.';
		
		int iDecPositive,iDecNagative;
		iDecPositive = iDec2;
		iDecNagative = iDec2;		
		
		for (iDecPositive = iDec2 -3; iDecPositive > 0;iDecPositive -= 3)
		{
			strValue.Insert(iDecPositive, cSeperator);
			iMax++;
		}
	}
	
	//////////////////////////////////////////////////////////////////////
    //for processing currency string

	Value = CFX_WideString::FromLocal(strValue);
	
	CFX_WideString strValue2(Value);

	if (bCurrencyPrepend)
		strValue2 = wstrCurrency + strValue2;
	else
		strValue2 = strValue2 + wstrCurrency;
	
	
	
	/////////////////////////////////////////////////////////////////////////
	//for processing negative style
	if (bNagative)
	{
		if (iNegStyle == 0)
		{
			strValue2.Insert(0, (FX_WCHAR)L'-');
		}
		if (iNegStyle == 2 || iNegStyle == 3)
		{
			strValue2.Insert(0,(FX_WCHAR)L'(');
			strValue2.Insert(strValue2.GetLength(),(FX_WCHAR)L')');
		}
		if (iNegStyle == 1 || iNegStyle == 3)
		{
			if (JField * fTarget = pEvent->Target_Field())
			{
				CFXJS_Array arColor;
				CFXJS_Value vColElm;
				vColElm = L"RGB";
				arColor.SetElement(0,vColElm);
				vColElm = 1;
				arColor.SetElement(1,vColElm);
				vColElm = 0;
				arColor.SetElement(2,vColElm);
				
				arColor.SetElement(3,vColElm);
				
				CFXJS_PropValue vProp;
				vProp.StartGetting();
				vProp<<arColor;
				vProp.StartSetting();
				fTarget->textColor(cc,vProp,sError);// red
			}
		}
	}
	else
	{
		if (iNegStyle == 1 || iNegStyle == 3)
		{
			if (JField *fTarget = pEvent->Target_Field())
			{
				CFXJS_Array arColor;
				CFXJS_Value vColElm;
				vColElm = L"RGB";
				arColor.SetElement(0,vColElm);
				vColElm = 0;
				arColor.SetElement(1,vColElm);
				arColor.SetElement(2,vColElm);
				arColor.SetElement(3,vColElm);
				
				CFXJS_PropValue vProp;
				vProp.StartGetting();
				fTarget->textColor(cc,vProp,sError);
				
				CFXJS_Array aProp;
				vProp.ConvertToArray(aProp);

				CPWL_Color crProp;
				CPWL_Color crColor;
				jcolor::ConvertArrayToPWLColor(aProp, crProp);
				jcolor::ConvertArrayToPWLColor(arColor, crColor);

				if (crColor != crProp)
				{
					CFXJS_PropValue vProp2;
					vProp2.StartGetting();
					vProp2<<arColor;
					vProp2.StartSetting();
     				fTarget->textColor(cc,vProp2,sError);
				}
			}
		}
	}
	Value = strValue2;

	return TRUE;
}

//function AFNumber_Keystroke(nDec, sepStyle, negStyle, currStyle, strCurrency, bCurrencyPrepend)
FX_BOOL CFXJS_PublicMethods::AFNumber_Keystroke(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);
	
	if(params.size() < 2)
		return FALSE;
	int iSepStyle = params[1];
	
	if (iSepStyle < 0 || iSepStyle > 3)
		iSepStyle = 0;
	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString & val = pEvent->Value();	
	CFX_WideString & w_strChange = pEvent->Change();
    CFX_WideString w_strValue = val;

	if (pEvent->WillCommit())
	{
		CFX_WideString wstrChange = w_strChange;
		CFX_WideString wstrValue = StrLTrim(w_strValue);
		if (wstrValue.IsEmpty())
			return TRUE;
		
		CFX_WideString swTemp = wstrValue;
		swTemp.Replace((FX_LPCWSTR)L",", (FX_LPCWSTR)L".");
		if (!IsNumber(swTemp)) //!(IsNumber(wstrChange) && 
		{
			pEvent->Rc() = FALSE;
			sError = JSGetStringFromID(pContext, IDS_STRING_JSAFNUMBER_KEYSTROKE);
			Alert(pContext, sError);
			return TRUE;
		}
		return TRUE; // it happens after the last keystroke and before validating,
	}
	
	CFX_WideString w_strValue2 (w_strValue);
	CFX_WideString w_strChange2(w_strChange);
	
	CFX_WideString w_strSelected = w_strValue2.Mid(pEvent->SelStart(),(pEvent->SelEnd() - pEvent->SelStart()));
	//std::wstring w_strSelected = w_strValue2.substr(pEvent->SelStart(),(pEvent->SelEnd() - pEvent->SelStart()));
	FX_BOOL bHasSign = (w_strValue2.Find((FX_WCHAR)L'-') != -1) && (w_strSelected.Find((FX_WCHAR)L'-') == -1);
	if (bHasSign)
	{
		//can't insert "change" in front to sign postion.
		if (pEvent->SelStart() == 0)
		{
            FX_BOOL &bRc = pEvent->Rc();
			bRc = FALSE;
			return TRUE;
		}
	}

	char cSep = L'.';

	switch (iSepStyle)
	{
	case 0:
	case 1:
		cSep = L'.';
		break;
	case 2:
	case 3:
		cSep = L',';
		break;
	}
	
	FX_BOOL bHasSep = (w_strValue2.Find((FX_WCHAR)cSep) != -1);
	for(int i = 0; i < w_strChange2.GetLength(); i++)
	//for (std::wstring::iterator it = w_strChange2.begin(); it != w_strChange2.end(); it++)
	{
		if (w_strChange2[i] == cSep)
		{
			if (bHasSep)
			{
				FX_BOOL &bRc = pEvent->Rc();
				bRc = FALSE;
				return TRUE;
			}
			else
			{
				bHasSep = TRUE;
				continue;
			}
		}
		if (w_strChange2[i] == L'-')
		{
			if (bHasSign)
			{
				FX_BOOL &bRc = pEvent->Rc();
				bRc = FALSE;
				return TRUE;
			}
			else if(i != 0)
			//else if (it != w_strChange2.begin()) //sign's position is not correct
			{
				FX_BOOL &bRc = pEvent->Rc();
				bRc = FALSE;
				return TRUE;
			}
			else if (pEvent->SelStart() != 0)
			{
				FX_BOOL &bRc = pEvent->Rc();
				bRc = FALSE;
				return TRUE;
			}
			bHasSign = TRUE;
			continue;
		}
		
		if (!IsDigit((char)w_strChange2[i]))
		{			
			FX_BOOL &bRc = pEvent->Rc();
			bRc = FALSE;
			return TRUE;
		}
	}
	
	CFX_WideString w_prefix = w_strValue2.Mid(0, pEvent->SelStart());
	//std::wstring w_prefix = w_strValue2.substr(0,pEvent->SelStart());
	CFX_WideString w_postfix;
	if (pEvent->SelEnd()<w_strValue2.GetLength())
		w_postfix = w_strValue2.Mid(pEvent->SelEnd());
		//w_postfix  = w_strValue2.substr(pEvent->SelEnd());
	w_strValue2 = w_prefix + w_strChange2 + w_postfix;
	w_strValue = w_strValue2;
	val = w_strValue;
	return TRUE;		
	
}

//function AFPercent_Format(nDec, sepStyle)
FX_BOOL CFXJS_PublicMethods::AFPercent_Format(OBJ_METHOD_PARAMS)
{
#if _FX_OS_ != _FX_ANDROID_
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

    if (params.size() != 2)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}
	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString& Value = pEvent->Value();
	
//     HWND hMainFrame = NULL;
// 	
// 	CPDFSDK_FormFillApp *pApp = pContext->GetReaderApp();
// 	ASSERT(pApp);
// 	hMainFrame = pApp->GetMainFrameWnd();
		
	CFX_ByteString strValue = StrTrim(CFX_ByteString::FromUnicode(Value));
	
	if (strValue.IsEmpty())
		return TRUE;
	
	int iDec = params[0];
	int iSepStyle = params[1];
	
	//ASSERT(iDec > 0);
	if (iDec < 0)
		iDec = -iDec;
	
	if (iSepStyle < 0 || iSepStyle > 3)
		iSepStyle = 0;
	
	
	//////////////////////////////////////////////////////
	//for processing decimal places
	double dValue = atof(strValue);
	dValue *= 100;
	if (iDec > 0)
		dValue += DOUBLE_CORRECT;//校正

	int iDec2;
	FX_BOOL bNagative = FALSE;
#if defined(WINCE) || defined(_WIN32) || defined(_WIN64)
	strValue = _fcvt(dValue,iDec,&iDec2,&bNagative);
#else
	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
	if (fabs(dValue) > DOUBLE_CORRECT)
		_fcvt_s(strValue.LockBuffer(), strValue.GetLength(), dValue, iDec, &iDec2, &bNagative);
	#else
		strValue = fcvt(dValue,iDec,&iDec2,&bNagative);
	#endif
#endif
    if (strValue.IsEmpty())
	{
		dValue = 0;
#if defined(WINCE) || defined(_WIN32) || defined(_WIN64)
		strValue = _fcvt(dValue,iDec,&iDec2,&bNagative);
#else
	#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
		if (fabs(dValue) > DOUBLE_CORRECT)
			_fcvt_s(strValue.LockBuffer(), strValue.GetLength(), dValue,iDec,&iDec2,&bNagative);
	#else
		strValue = fcvt(dValue,iDec,&iDec2,&bNagative);
	#endif
#endif
	}

	if (iDec2 < 0)
	{
		for (int iNum = 0; iNum < abs(iDec2); iNum++)
		{
			strValue = "0" + strValue;
		}
		iDec2 = 0;
		
	}
	int iMax = strValue.GetLength();
	if (iDec2 > iMax)
	{
		for (int iNum = 0; iNum <= iDec2 - iMax; iNum++)
		{
			strValue += "0";
		}
		iMax = iDec2+1;			
	}
	///////////////////////////////////////////////////////
    //for processing seperator style
	if (iDec2 < iMax)
	{
		if (iSepStyle == 0 || iSepStyle == 1)
		{
			strValue.Insert(iDec2, '.');
			iMax++;
		}
		else if (iSepStyle == 2 || iSepStyle == 3)
		{
			strValue.Insert(iDec2, ',');
			iMax++;
		}
		
		if (iDec2 == 0)
			strValue.Insert(iDec2, '0');
	}
	if (iSepStyle == 0 || iSepStyle == 2)
	{
		char cSeperator;
		if (iSepStyle == 0)
			cSeperator = ',';
		else
			cSeperator = '.';
		
		int iDecPositive,iDecNagative;
		iDecPositive = iDec2;
		iDecNagative = iDec2;
			
		for (iDecPositive = iDec2 -3; iDecPositive > 0; iDecPositive -= 3)
		{
			strValue.Insert(iDecPositive,cSeperator);
			iMax++;
		}
	}
	////////////////////////////////////////////////////////////////////
	//nagative mark
	if(bNagative)
		strValue = "-" + strValue;
	strValue += "%";
	Value = CFX_WideString::FromLocal(strValue);
#endif
	return TRUE;
}
//AFPercent_Keystroke(nDec, sepStyle)
FX_BOOL CFXJS_PublicMethods::AFPercent_Keystroke(OBJ_METHOD_PARAMS)
{
	return AFNumber_Keystroke(cc,params,vRet,sError);
}

//function AFDate_FormatEx(cFormat)
FX_BOOL CFXJS_PublicMethods::AFDate_FormatEx(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (params.size() != 1)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}	
	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString& val = pEvent->Value();
	
	CFX_WideString strValue = val;	
	if (strValue.IsEmpty()) return TRUE;		

	CFX_WideString sFormat = (FX_LPCWSTR)params[0];

	FX_BOOL bWrongFormat = FALSE;
	double dDate = 0.0f;

	if(strValue.Find(L"GMT") != -1)
	{
		//for GMT format time
		//such as "Tue Aug 11 14:24:16 GMT+08002009"
		dDate = MakeInterDate(strValue);
	}
	else
	{
		dDate = MakeRegularDate(strValue,sFormat,bWrongFormat);
	}

	if (bWrongFormat || DS_PortIsNan(dDate))
	{
		CFX_WideString swMsg;
		swMsg.Format(JSGetStringFromID(pContext, IDS_STRING_JSPARSEDATE), (FX_LPCWSTR)sFormat);
		Alert(pContext, swMsg);
		return FALSE;
	}
	
	val =  MakeFormatDate(dDate,sFormat);

	return TRUE;
}

double CFXJS_PublicMethods::MakeInterDate(CFX_WideString strValue)
{
	FX_INT32 nHour(0), nMin(0), nSec(0), nYear(0), nMonth(0), nDay(0);

	CFX_WideStringArray wsArray;
	CFX_WideString sMonth = L"";
	CFX_WideString sTemp = L"";
	int nSize = strValue.GetLength();

	for(int i = 0; i < nSize; i++)
	{
		FX_WCHAR c = strValue.GetAt(i);
		if(c == L' ' || c == L':')
		{	
			wsArray.Add(sTemp);
			sTemp = L"";
			continue;
		}

		sTemp += c;
	}
	
	wsArray.Add(sTemp);
	if(wsArray.GetSize() != 8)return 0;

	sTemp = wsArray[1];
	if(sTemp.Compare(L"Jan") == 0) nMonth = 1;
	if(sTemp.Compare(L"Feb") == 0) nMonth = 2;
	if(sTemp.Compare(L"Mar") == 0) nMonth = 3;
	if(sTemp.Compare(L"Apr") == 0) nMonth = 4;
	if(sTemp.Compare(L"May") == 0) nMonth = 5;
	if(sTemp.Compare(L"Jun") == 0) nMonth = 6;
	if(sTemp.Compare(L"Jul") == 0) nMonth = 7;
	if(sTemp.Compare(L"Aug") == 0) nMonth = 8;
	if(sTemp.Compare(L"Sep") == 0) nMonth = 9;
	if(sTemp.Compare(L"Oct") == 0) nMonth = 10;
	if(sTemp.Compare(L"Nov") == 0) nMonth = 11;
	if(sTemp.Compare(L"Dec") == 0) nMonth = 12;

	nDay = (FX_INT32)ParseStringToNumber(wsArray[2]);
	nHour = (FX_INT32)ParseStringToNumber(wsArray[3]);
	nMin = (FX_INT32)ParseStringToNumber(wsArray[4]);
	nSec = (FX_INT32)ParseStringToNumber(wsArray[5]);
	nYear = (FX_INT32)ParseStringToNumber(wsArray[7]);

	double dRet = DS_MakeDate(DS_MakeDay(nYear,nMonth - 1,nDay),DS_MakeTime(nHour, nMin, nSec, 0));

	if (DS_PortIsNan(dRet))
	{
		dRet = DS_DateParse(strValue);
	}
	
	return dRet;
}

//AFDate_KeystrokeEx(cFormat)
FX_BOOL CFXJS_PublicMethods::AFDate_KeystrokeEx(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (params.size() != 1)
	{
		sError = L"AFDate_KeystrokeEx's parameters' size r not correct";
		return FALSE;
	}	
	
	if (pEvent->WillCommit())
	{
		if(!pEvent->m_pValue)
			return FALSE;
		CFX_WideString strValue = pEvent->Value();
		if (strValue.IsEmpty()) return TRUE;

		CFX_WideString sFormat = (FX_LPCWSTR)params[0];

		FX_BOOL bWrongFormat = FALSE;
		double dRet = MakeRegularDate(strValue,sFormat,bWrongFormat);
		if (bWrongFormat || DS_PortIsNan(dRet))
		{
			CFX_WideString swMsg;
			swMsg.Format(JSGetStringFromID(pContext, IDS_STRING_JSPARSEDATE), (FX_LPCWSTR)sFormat);
			Alert(pContext, swMsg);
			pEvent->Rc() = FALSE;
			return TRUE;
		}
	}
	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::AFDate_Format(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)
	{
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);

		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	int iIndex = params[0];
	FX_LPCWSTR cFormats[] =  {(FX_LPCWSTR)L"m/d", (FX_LPCWSTR)L"m/d/yy", (FX_LPCWSTR)L"mm/dd/yy", (FX_LPCWSTR)L"mm/yy", (FX_LPCWSTR)L"d-mmm", (FX_LPCWSTR)L"d-mmm-yy", (FX_LPCWSTR)L"dd-mmm-yy",
		(FX_LPCWSTR)L"yy-mm-dd", (FX_LPCWSTR)L"mmm-yy", (FX_LPCWSTR)L"mmmm-yy", (FX_LPCWSTR)L"mmm d, yyyy", (FX_LPCWSTR)L"mmmm d, yyyy",
		(FX_LPCWSTR)L"m/d/yy h:MM tt", (FX_LPCWSTR)L"m/d/yy HH:MM" };

	ASSERT(iIndex < sizeof(cFormats)/sizeof(FX_LPCWSTR));

	if (iIndex < 0)
		iIndex = 0;
	if (iIndex >= sizeof(cFormats)/sizeof(FX_LPCWSTR))
		iIndex = 0;
	CJS_Parameters newParams;
	CFXJS_Value val(cFormats[iIndex]);
	newParams.push_back(val);
	return AFDate_FormatEx(cc,newParams,vRet,sError);
}

//AFDate_KeystrokeEx(cFormat)
FX_BOOL CFXJS_PublicMethods::AFDate_Keystroke(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)
	{
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);

		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	int iIndex = params[0];
	FX_LPCWSTR cFormats[] =  {(FX_LPCWSTR)L"m/d", (FX_LPCWSTR)L"m/d/yy", (FX_LPCWSTR)L"mm/dd/yy", (FX_LPCWSTR)L"mm/yy", (FX_LPCWSTR)L"d-mmm", (FX_LPCWSTR)L"d-mmm-yy", (FX_LPCWSTR)L"dd-mmm-yy",
		(FX_LPCWSTR)L"yy-mm-dd", (FX_LPCWSTR)L"mmm-yy", (FX_LPCWSTR)L"mmmm-yy", (FX_LPCWSTR)L"mmm d, yyyy", (FX_LPCWSTR)L"mmmm d, yyyy",
		(FX_LPCWSTR)L"m/d/yy h:MM tt", (FX_LPCWSTR)L"m/d/yy HH:MM" };

	ASSERT(iIndex<sizeof(cFormats)/sizeof(FX_LPCWSTR));

	if (iIndex < 0)
		iIndex = 0;
	if (iIndex >= sizeof(cFormats)/sizeof(FX_LPCWSTR))
		iIndex = 0;
	CJS_Parameters newParams;
	CFXJS_Value val(cFormats[iIndex]);
	newParams.push_back(val);
	return AFDate_KeystrokeEx(cc,newParams,vRet,sError);
}

//function AFTime_Format(ptf)
FX_BOOL CFXJS_PublicMethods::AFTime_Format(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)
	{
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	int iIndex = params[0];
	FX_LPCWSTR cFormats[] = {(FX_LPCWSTR)L"HH:MM", (FX_LPCWSTR)L"h:MM tt", (FX_LPCWSTR)L"HH:MM:ss", (FX_LPCWSTR)L"h:MM:ss tt"};

	ASSERT(iIndex<sizeof(cFormats)/sizeof(FX_LPCWSTR));

	if (iIndex < 0)
		iIndex = 0;
	if (iIndex >= sizeof(cFormats)/sizeof(FX_LPCWSTR))
		iIndex = 0;
	CJS_Parameters newParams;
	CFXJS_Value val(cFormats[iIndex]);
	newParams.push_back(val);
	return AFDate_FormatEx(cc,newParams,vRet,sError);
}

FX_BOOL CFXJS_PublicMethods::AFTime_Keystroke(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)
	{
		CFXJS_Context* pContext = (CFXJS_Context*)cc;
		ASSERT(pContext != NULL);
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	int iIndex = params[0];
	FX_LPCWSTR cFormats[] = {(FX_LPCWSTR)L"HH:MM", (FX_LPCWSTR)L"h:MM tt", (FX_LPCWSTR)L"HH:MM:ss", (FX_LPCWSTR)L"h:MM:ss tt"};

	ASSERT(iIndex<sizeof(cFormats)/sizeof(FX_LPCWSTR));

	if (iIndex < 0)
		iIndex = 0;
	if (iIndex >= sizeof(cFormats)/sizeof(FX_LPCWSTR))
		iIndex = 0;
	CJS_Parameters newParams;
	CFXJS_Value val(cFormats[iIndex]);
	newParams.push_back(val);
	return AFDate_KeystrokeEx(cc,newParams,vRet,sError);
}

FX_BOOL CFXJS_PublicMethods::AFTime_FormatEx(OBJ_METHOD_PARAMS)
{
	return AFDate_FormatEx(cc,params,vRet,sError);
}

FX_BOOL CFXJS_PublicMethods::AFTime_KeystrokeEx(OBJ_METHOD_PARAMS)
{
	return AFDate_KeystrokeEx(cc,params,vRet,sError);
}

//function AFSpecial_Format(psf)
FX_BOOL CFXJS_PublicMethods::AFSpecial_Format(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);

	if (params.size() != 1)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CFX_ByteString cFormat;
	int iIndex = params[0];

	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString& Value = pEvent->Value();	
	CFX_ByteString strSrc = (FX_LPCSTR)CFX_ByteString::FromUnicode(Value);
	
	switch (iIndex) 
	{
	case 0:                         
		cFormat = "99999";
		break;
	case 1:                         
		cFormat = "99999-9999";
		break;
	case 2:                         
		{
			CFX_ByteString NumberStr;
			jutil::printx("9999999999", strSrc,NumberStr); 
			if (NumberStr.GetLength() >= 10 )
				cFormat = "(999) 999-9999";
			else 
				cFormat = "999-9999";
			break;
		}
	case 3:
		cFormat = "999-99-9999";
		break;
	}
	
	CFX_ByteString strDes;
	jutil::printx(cFormat,strSrc,strDes);
	Value = CFX_WideString::FromLocal(strDes);
	return TRUE;
}


//function AFSpecial_KeystrokeEx(mask)
FX_BOOL CFXJS_PublicMethods::AFSpecial_KeystrokeEx(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();

	ASSERT(pEvent != NULL);

	if (params.size() < 1)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString& valEvent = pEvent->Value();

	CFX_WideString wstrMask = (FX_LPCWSTR)params[0];
	if (wstrMask.IsEmpty()) return TRUE;
	
	CFX_WideString wstrValue(valEvent);
	
	if (pEvent->WillCommit())
	{
		if (wstrValue.IsEmpty())
			return TRUE;
		int iIndexMask = 0;
		for(int i=0; i<wstrValue.GetLength(); i++)
		//for (std::wstring::iterator it = wstrValue.begin(); it != wstrValue.end(); it++)
		{
			wchar_t w_Value = wstrValue[i];
            if (!maskSatisfied(w_Value,wstrMask[iIndexMask]))
				break;
			iIndexMask++;
		}

		if (iIndexMask != wstrMask.GetLength() || iIndexMask != wstrValue.GetLength() && wstrMask.GetLength() != 0)
		{
			Alert(pContext, JSGetStringFromID(pContext, IDS_STRING_JSAFNUMBER_KEYSTROKE));
			pEvent->Rc() = FALSE;
		}
		return TRUE;
	}

	
	CFX_WideString &wideChange = pEvent->Change();
	CFX_WideString wChange(wideChange);
	
	if (wChange.IsEmpty())
		return TRUE;
    int iIndexMask = pEvent->SelStart();
	//iIndexMask++;
	
	
	if (wstrValue.GetLength() - (pEvent->SelEnd()-pEvent->SelStart()) + wChange.GetLength() > wstrMask.GetLength())
	{
		Alert(pContext, JSGetStringFromID(pContext, IDS_STRING_JSPARAM_TOOLONG));
		pEvent->Rc() = FALSE;
		return TRUE;
	}
	
	
	if (iIndexMask >= wstrMask.GetLength() && (!wChange.IsEmpty()))
	{
		Alert(pContext, JSGetStringFromID(pContext, IDS_STRING_JSPARAM_TOOLONG));
		pEvent->Rc() = FALSE;
		return TRUE;
	}
	
	for(int i=0; i<wChange.GetLength(); i++)
	//for (std::wstring::iterator it = wChange.begin(); it != wChange.end(); it++)
	{
		if (iIndexMask >= wstrMask.GetLength())
		{
			Alert(pContext, JSGetStringFromID(pContext, IDS_STRING_JSPARAM_TOOLONG));
			pEvent->Rc() = FALSE;
			return TRUE;
		}
		wchar_t w_Mask = wstrMask[iIndexMask];
		if (!isReservedMaskChar(w_Mask))
		{
			//wChange.insert(it,w_Mask);		
			wChange.SetAt(i, w_Mask);
			//*it = w_Mask;
		}
		wchar_t w_Change = wChange[i];
		
        if (!maskSatisfied(w_Change,w_Mask))
		{
			pEvent->Rc() = FALSE;
			return TRUE;
		}
		iIndexMask++;
	}
	
	wideChange = wChange;	
	
	return TRUE;
}


//function AFSpecial_Keystroke(psf)
FX_BOOL CFXJS_PublicMethods::AFSpecial_Keystroke(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (params.size() != 1)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CFX_ByteString cFormat;
	int iIndex = (int)params[0];	

	if(!pEvent->m_pValue)
		return FALSE;
	//CJS_Value val = pEvent->Value();
	CFX_WideString& val = pEvent->Value();
	CFX_ByteString strSrc = CFX_ByteString::FromUnicode(val);
	CFX_WideString wstrChange(pEvent->Change());
	
	switch (iIndex) 
	{
	case 0:                         
		cFormat = "99999";
		break;
	case 1:                         
		//cFormat = "99999-9999";
		cFormat = "999999999";
		break;
	case 2:                         
		{
			CFX_ByteString NumberStr;
			jutil::printx("9999999999", strSrc,NumberStr); 
			if (strSrc.GetLength() + wstrChange.GetLength() > 7 )
				//cFormat = "(999) 999-9999";
				cFormat = "9999999999";
			else 
				//cFormat = "999-9999";
				cFormat = "9999999";
			break;
		}
	case 3:
		//cFormat = "999-99-9999";
		cFormat = "999999999";
		break;
	}
    
	CJS_Parameters params2;
	CFXJS_Value vMask((FX_LPCSTR)cFormat);
	params2.push_back(vMask);
	
    return AFSpecial_KeystrokeEx(cc,params2,vRet,sError);
}

FX_BOOL CFXJS_PublicMethods::AFMergeChange(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEventHandler = pContext->GetEventHandler();
	ASSERT(pEventHandler != NULL);

	if (params.size() != 1)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CFX_WideString swValue = pEventHandler->Value();
	if (pEventHandler->WillCommit())
	{
		vRet = swValue;
		return TRUE;
	}

	CFX_WideString prefix,postfix;

	if (pEventHandler->SelStart() >= 0)
		prefix = swValue.Mid(0,pEventHandler->SelStart());
	else
		prefix = L"";


	if (pEventHandler->SelEnd() >= 0 && pEventHandler->SelEnd() <= swValue.GetLength())
		postfix = swValue.Mid(pEventHandler->SelEnd(), swValue.GetLength() - pEventHandler->SelEnd());
	else postfix = L"";

	vRet = prefix + pEventHandler->Change() + postfix;

	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::AFParseDateEx(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);

	if (params.size() != 2)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CFX_WideString sValue = (FX_LPCWSTR)params[0];
	CFX_WideString sFormat = (FX_LPCWSTR)params[1];

	FX_BOOL bWrongFormat = FALSE;
	double dDate = MakeRegularDate(sValue,sFormat,bWrongFormat);

	if (DS_PortIsNan(dDate))
	{
		CFX_WideString swMsg;
		swMsg.Format(JSGetStringFromID(pContext, IDS_STRING_JSPARSEDATE), (FX_LPCWSTR)sFormat);
		Alert((CFXJS_Context *)cc, swMsg);
		return FALSE;
	}
	
	vRet = dDate;

	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::AFSimple(OBJ_METHOD_PARAMS)
{
	if (params.size() != 3)
	{
		CFXJS_Context* pContext = (CFXJS_Context *)cc;
		ASSERT(pContext != NULL);

		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	vRet = (double)AF_Simple((FX_LPCWSTR)params[0], (double)params[1], (double)params[2]);
	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::AFMakeNumber(OBJ_METHOD_PARAMS)
{
	if (params.size() != 1)
	{
		CFXJS_Context* pContext = (CFXJS_Context *)cc;
		ASSERT(pContext != NULL);

		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}
    vRet = ParseStringToNumber((FX_LPCWSTR)params[0]);
	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::AFSimple_Calculate(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);

	if (params.size() != 2)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CFXJS_Value params1 = params[1];

	if (!params1.IsArrayObject() && params1.GetType() != VT_string)
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}
	
	FSPDFDocImp* pReaderDoc = (FSPDFDocImp*)pContext->GetReaderDocument();
    ASSERT(pReaderDoc != NULL);

	FSFormImp* pReaderInterForm = pReaderDoc->GetForm();
	ASSERT(pReaderInterForm != NULL);
	CPDF_InterForm* pInterForm = pReaderInterForm->ST_GetSTInterForm()->m_pInterForm;
	ASSERT(pInterForm != NULL);

	double dValue(0.0);
	CFX_WideString sFunction = (FX_LPCWSTR)params[0];
//	if (FXSYS_wcscmp(sFunction, L"PRD") == 0) // Changed for WinCE by Yvonne
	if (sFunction.Compare(L"PRD") == 0)
    	dValue = 1.0;
	else
		dValue = 0.0;

	CFXJS_Array FieldNameArray = AF_MakeArrayFromList(params1);

	int nFieldsCount = 0;

	for (int i = 0,isz = FieldNameArray.GetLength(); i<isz; i++)
	{
		CFXJS_Value jsValue;
		FieldNameArray.GetElement(i,jsValue);
        CFX_WideString wsFieldName = (FX_LPCWSTR)jsValue;

		//SDK-744
		int cField = pInterForm->CountFields(wsFieldName);
		if (cField < 1)
			return FALSE; 

        for (int j=0,jsz=pInterForm->CountFields(wsFieldName); j<jsz; j++)
		{
			if (CPDF_FormField* pFormField = pInterForm->GetField(j, wsFieldName))
			{
				double dTemp = 0.0;

				switch (pFormField->GetFieldType())
				{
				case FIELDTYPE_TEXTFIELD:
				case FIELDTYPE_COMBOBOX:
					{
						dTemp = ParseStringToNumber(pFormField->GetValue());
						break;
					}
				case FIELDTYPE_PUSHBUTTON:
					{
						dTemp = 0.0;
						break;
					}
				case FIELDTYPE_CHECKBOX:
				case FIELDTYPE_RADIOBUTTON:
					{
						dTemp = 0.0;
						for (int c=0,csz=pFormField->CountControls(); c<csz; c++)
						{
							if (CPDF_FormControl* pFormCtrl = pFormField->GetControl(c))
							{
								if (pFormCtrl->IsChecked())
								{
									dTemp += ParseStringToNumber(pFormCtrl->GetExportValue());
									break;
								}
								else
									continue;
							}
						}
						break;
					}
				case FIELDTYPE_LISTBOX:
					{
						dTemp = 0.0;
						if (pFormField->CountSelectedItems() > 1)
							break;
						else
						{
							dTemp = ParseStringToNumber(pFormField->GetValue());
							break;
						}
					}
				default:
					break;
				}

// 				if (i == 0 && j == 0 && (FXSYS_wcscmp(sFunction,L"MIN") == 0 || FXSYS_wcscmp(sFunction, L"MAX") == 0)) // Changed for WinCE by Yvonne
 				if (i == 0 && j == 0 && (sFunction.Compare(L"MIN") == 0 || sFunction.Compare(L"MAX") == 0))
					dValue = dTemp;

				dValue = AF_Simple(sFunction, dValue, dTemp);

				nFieldsCount++;
			}
		}
	}

// 	if (FXSYS_wcscmp(sFunction, L"AVG") == 0 && nFieldsCount > 0) // Changed for WinCE by Yvonne
	if (sFunction.Compare(L"AVG") == 0 && nFieldsCount > 0)
		dValue /= nFieldsCount;

	dValue = (double)floor(dValue * FXSYS_pow((double)10,(double)6) + 0.49) / FXSYS_pow((double)10,(double)6);
	CFXJS_Value jsValue = dValue;
	((CFXJS_EventHandler*)pContext->GetEventHandler())->Value() = jsValue;

	return TRUE;
}

/* This function validates the current event to ensure that its value is 
** within the specified range. */

FX_BOOL CFXJS_PublicMethods::AFRange_Validate(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context *)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (params.size() != 4) 
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	if(!pEvent->m_pValue)
		return FALSE;
	if (pEvent->Value().IsEmpty() )
		return TRUE;
	double dEentValue = atof(CFX_ByteString::FromUnicode(pEvent->Value()));
	FX_BOOL bGreaterThan, bLessThan;
	double  dGreaterThan, dLessThan;
    bGreaterThan = (FX_BOOL)params[0];
	CFX_WideString swMsg;
	dGreaterThan = (double)params[1];
	bLessThan = (FX_BOOL)params[2];
	dLessThan = (double)params[3];

	if (bGreaterThan && bLessThan)
	{
		if (dEentValue < dGreaterThan || dEentValue > dLessThan)
		{
		#if (_FX_OS_ == _FX_ANDROID_) ||  (_FX_OS_ == _FX_IOS_) || (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
			CFX_ByteString bsPOne;
			CFX_ByteString bsPThree;
			CFX_ByteString bsMsg = "";
			bsPOne = CFX_ByteString::FromUnicode((FX_LPCWSTR)params[1]);
			bsPThree = CFX_ByteString::FromUnicode((FX_LPCWSTR)params[3]);
			bsMsg.Format(JSGetByteStringFromID(pContext, IDS_STRING_JSRANGE1), (FX_LPCSTR)bsPOne,(FX_LPCSTR)bsPThree);
			swMsg.ConvertFrom(bsMsg);
		#else
			swMsg.Format(JSGetStringFromID(pContext, IDS_STRING_JSRANGE1),(FX_LPCWSTR)params[1], (FX_LPCWSTR)params[3]);
		#endif	
		}
	}
	else if (bGreaterThan)
	{
		if (dEentValue < dGreaterThan)
			swMsg.Format(JSGetStringFromID(pContext, IDS_STRING_JSRANGE2), (FX_LPCWSTR)params[1]);
	}
	else if (bLessThan)
	{
		if (dEentValue > dLessThan)
			swMsg.Format(JSGetStringFromID(pContext, IDS_STRING_JSRANGE3), (FX_LPCWSTR)params[3]);
	}

	if (!swMsg.IsEmpty())
	{
		Alert(pContext, swMsg);
		pEvent->Rc() = FALSE;
	}
	return TRUE;
}

FX_BOOL CFXJS_PublicMethods::AFExtractNums(OBJ_METHOD_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);

	if (params.size() != 1) 
	{
		sError = JSGetStringFromID(pContext, IDS_STRING_JSPARAMERROR);
		return FALSE;
	}

	CFXJS_Array nums;

	CFX_WideString str = (FX_LPCWSTR)params[0];
	CFX_WideString sPart;

	if (str.GetAt(0) == L'.' || str.GetAt(0) == L',')
		str = L"0" + str;

	int nIndex = 0;
	for (int i=0, sz=str.GetLength(); i<sz; i++)
	{
		FX_WCHAR wc = str.GetAt(i);
		if (IsDigit((wchar_t)wc))
		{
			sPart += wc;
		}
		else
		{
			if (sPart.GetLength() > 0)
			{
				nums.SetElement(nIndex,(FX_LPCWSTR)sPart);
				sPart = L"";
				nIndex ++;
			}
		}
	}

	if (sPart.GetLength() > 0)
	{
		nums.SetElement(nIndex,(FX_LPCWSTR)sPart);	
	}

	if (nums.GetLength() > 0)
		vRet = nums;
	else
		vRet.SetNull();

	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)
