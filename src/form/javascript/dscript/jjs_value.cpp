#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

/* ---------------------------- CJS_Value ---------------------------- */

CFXJS_Value::CFXJS_Value(JSValue *pValue,VALUETYPE t) : m_pValue(pValue) , m_eType(t)
{
}

CFXJS_Value::CFXJS_Value() : m_pValue(NULL),  m_eType(VT_unknown)
{
}

CFXJS_Value::CFXJS_Value(const int &iValue) : m_pValue(NULL)
{
	operator =(iValue);
}

CFXJS_Value::CFXJS_Value(const bool &bValue) : m_pValue(NULL)
{
	operator =(bValue);
}

CFXJS_Value::CFXJS_Value(const float &fValue) : m_pValue(NULL)
{
	operator =(fValue);
}

CFXJS_Value::CFXJS_Value(const double &dValue) : m_pValue(NULL)
{
	operator =(dValue);
}

/*
CJS_Value::CJS_Value(std::string &string) : m_pValue(NULL)
{
	operator =(string);
}
*/

CFXJS_Value::CFXJS_Value(JSFXObject * pJsObj) : m_pValue(NULL)
{
	operator =(pJsObj);
}

CFXJS_Value::CFXJS_Value(JSObject * pJsObj) : m_pValue(NULL)
{
	operator =(pJsObj);
}

CFXJS_Value::CFXJS_Value(CFXJS_Object * pJsObj) : m_pValue(NULL)
{
	operator =(pJsObj);
}

CFXJS_Value::CFXJS_Value(FX_LPCWSTR pWstr) : m_pValue(NULL)
{
	operator =(pWstr);
}

CFXJS_Value::CFXJS_Value(FX_LPCSTR pStr) : m_pValue(NULL)
{
	operator = (pStr);
}

CFXJS_Value::CFXJS_Value(CFXJS_Array& array) : m_pValue(NULL)
{
	operator = (array);
}

CFXJS_Value::~CFXJS_Value()
{
}

void CFXJS_Value::Attach(JSValue *pValue,VALUETYPE t)
{
	m_pValue = pValue;
	m_eType = t;
}

void CFXJS_Value::Attach(CFXJS_Value *pValue)
{
	if (pValue)
		Attach(pValue->ToJSValue(),pValue->GetType());
}

void CFXJS_Value::Detach()
{
	m_pValue = NULL;
	m_eType = VT_unknown;
}

/* ---------------------------------------------------------------------------------------- */

CFXJS_Value::operator int() const
{
	if (m_pValue)
		return DS_ToInt32(m_pValue);
	
	return 0;
}

CFXJS_Value::operator bool() const
{
	if (m_pValue)
		return DS_ToBoolean(m_pValue);
	
	return false;
}

CFXJS_Value::operator double() const
{
	if (m_pValue)
		return DS_ToNumber(m_pValue);
	
	return 0;
}

CFXJS_Value::operator float() const
{
	if (m_pValue)
		return (float)DS_ToNumber(m_pValue);
	
	return 0;
}

CFXJS_Value::operator CFXJS_Object *() const
{
	Dobject * pObj = NULL;
	if (m_pValue)
		pObj = DS_ToObject(m_pValue);
	
	if (pObj && FXSYS_wcscmp(DS_GetClassname(pObj),L"FxObj") == 0)
	{
		JSFXObject * pJSObj = (JSFXObject *)pObj;
		return (CFXJS_Object *)DS_GetPrivate(pJSObj);
	}
	return NULL;
}

CFXJS_Value::operator JSFXObject *() const
{
	Dobject * pObj = NULL;
	if (m_pValue)
		pObj = DS_ToObject(m_pValue);
	
	if (pObj && FXSYS_wcscmp(DS_GetClassname(pObj),L"FxObj") == 0)
	{
		return (JSFXObject *)pObj;
	}
	return NULL;
}

CFXJS_Value::operator JSObject *() const
{
	Dobject * pObj = NULL;
	if (m_pValue)
		pObj = DS_ToObject(m_pValue);
	
	if (pObj && FXSYS_wcscmp(DS_GetClassname(pObj),L"Object") == 0)
	{
		return (JSObject *)pObj;
	}
	return NULL;
}

CFXJS_Value::operator FX_LPCWSTR() const
{
	if (m_pValue)
		return (FX_WCHAR *)DS_ToString(m_pValue);
	
	return (FX_WCHAR *)L"";
}

CFXJS_Value::operator FX_WCHAR *() const
{
	if (m_pValue)
	{
		return (FX_WCHAR *)DS_ToString(m_pValue);
	}
	
	return (FX_WCHAR *)L"";
}

/*
#include <locale.h>
CFX_WideString CJS_Value::s2ws(const CFX_ByteString & str)
{
	CFX_ByteString sCurLocal = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "C");

	int nSize = str.GetLength() + 1;
	wchar_t * swTemp = new wchar_t[nSize];
	wmemset(swTemp, 0, nSize);
	mbstowcs(swTemp,str,nSize);
	CFX_WideString swRet = swTemp;
	delete []swTemp;

	setlocale(LC_ALL, sCurLocal);

	return swRet;
}

CFX_ByteString CJS_Value::ws2s(const CFX_WideString & str)
{
	CFX_ByteString sCurLocal = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "C");

	int nSize = str.GetLength() + 1;
	char * sTemp = new char[nSize];
	memset(sTemp, 0, nSize);
	wcstombs(sTemp,str,nSize);
	CFX_ByteString sRet = sTemp;
	delete []sTemp;

	setlocale(LC_ALL, sCurLocal);

	return sRet;
}
*/

CFXJS_Value::operator CFX_ByteString() const
{
	return CFX_ByteString::FromUnicode(operator FX_WCHAR *());
}

JSValue* CFXJS_Value::ToJSValue()
{
	if (m_pValue)
		return m_pValue;

	return NULL;
}

/*
CJS_Value::operator JSValue *()
{
	if (m_pValue)
		return m_pValue;
	else
		return &m_Value;
}
*/

CFXJS_Value::operator Darray *() const
{
	if (m_pValue && IsArrayObject())
		return (Darray *)DS_ToObject(m_pValue);
	
	return NULL;
}

/* ---------------------------------------------------------------------------------------- */

void CFXJS_Value::operator =(int iValue)
{
	if (m_pValue)
		DS_ValuePutNumber(m_pValue,iValue);
	else
	{
		m_pValue = DS_NewNumber(iValue);
	}

	m_eType = VT_number;
}

void CFXJS_Value::operator =(bool bValue)
{
	if (m_pValue)
		DS_ValuePutBoolean(m_pValue,bValue);
	else
	{
		m_pValue = DS_NewBoolean(bValue);
	}

	m_eType = VT_boolean;
}

void CFXJS_Value::operator =(double dValue)
{
	if (m_pValue)
		DS_ValuePutNumber(m_pValue,dValue);
	else
	{
		m_pValue = DS_NewNumber(dValue);
	}

	m_eType = VT_number;
}

void CFXJS_Value::operator = (float fValue)
{
	if (m_pValue)
		DS_ValuePutNumber(m_pValue,fValue);
	else
	{
		m_pValue = DS_NewNumber(fValue);
	}

	m_eType = VT_number;
}

void CFXJS_Value::operator =(JSFXObject* pObj)
{
	if (m_pValue)
		DS_ValuePutObject(m_pValue, pObj);
	else
	{
		m_pValue = DS_NewObject(pObj);
	}

	m_eType = VT_fxobject;
}

void CFXJS_Value::operator = (JSObject * pObj)
{
	if (m_pValue)
		DS_ValuePutObject(m_pValue, pObj);
	else
	{
		m_pValue = DS_NewObject(pObj);
	}

	m_eType = VT_object;
}

void CFXJS_Value::operator =(CFXJS_Object * pObj)
{
	if (pObj)
		operator = ((JSFXObject*)*pObj);
}

void CFXJS_Value::operator =(FX_LPCWSTR pWstr)
{
	if (m_pValue)
		DS_ValuePutString(m_pValue,(wchar_t *)pWstr);
	else
	{
		m_pValue = DS_NewString((wchar_t *)pWstr);
	}

	m_eType = VT_string;
}

void CFXJS_Value::SetNull()
{
	if (m_pValue)
		DS_ValueCopy(m_pValue,DS_NewNull());
	else
	{
		m_pValue = DS_NewNull();
	}

	m_eType = VT_null;
}

void CFXJS_Value::operator = (FX_LPCSTR pStr)
{	
	operator = (CFX_WideString::FromLocal(pStr));
}

void CFXJS_Value::operator = (CFXJS_Array & array)
{
	if (m_pValue)
	{
		DS_ValuePutObject(m_pValue, (Darray*)array);
	}
	else
	{
		m_pValue = DS_NewObject((Darray*)array);
	}

	m_eType = VT_object;
}

void CFXJS_Value::operator = (CFXJS_Date & date)
{
	if (m_pValue)
	{
		DS_ValuePutDate(m_pValue,date);
	}
	else
	{		
		m_pValue = DS_NewDate(date);
	}

	m_eType = VT_date;
}

void CFXJS_Value::operator = (CFXJS_Value value)
{
	if (m_pValue)
	{
		DS_ValueCopy(m_pValue,value.m_pValue);
	}
	else
	{
		m_pValue = DS_NewValue();
		if(m_pValue && value.m_pValue)
			DS_ValueCopy(m_pValue,value.m_pValue);
		else
			m_pValue = NULL;
	}

	m_eType = value.m_eType;
}

/* ---------------------------------------------------------------------------------------- */

VALUETYPE CFXJS_Value::GetType() const
{
	return m_eType;
}

FX_BOOL CFXJS_Value::IsArrayObject() const 
{
	if (m_pValue && GetType() == VT_object)
	{
		Dobject * pObj = DS_ToObject(m_pValue);
		if (pObj && FXSYS_wcscmp(DS_GetClassname(pObj),CLASSNAME_ARRAY) == 0)
			return TRUE;
	}

	return FALSE;
}

FX_BOOL CFXJS_Value::IsDateObject() const
{
	if (m_pValue && GetType() == VT_object)
	{
		Dobject * pObj = DS_ToObject(m_pValue);
		if (pObj && FXSYS_wcscmp(DS_GetClassname(pObj),CLASSNAME_DATE) == 0)
			return TRUE;
	}

	return FALSE;
}

//CJS_Value::operator CJS_Array()
FX_BOOL CFXJS_Value::ConvertToArray(CFXJS_Array &array) const
{
	if (m_pValue && IsArrayObject())
	{
		array.Attach((Darray*)DS_ToObject(m_pValue));
		return TRUE;
	}

	return FALSE;
}

FX_BOOL CFXJS_Value::ConvertToDate(CFXJS_Date &date) const
{
	if (m_pValue && GetType() == VT_date)
	{
		date = (double)(*this);
		return TRUE;
	}

	if (m_pValue && IsDateObject())
	{
		date.Attach(m_pValue);
		return TRUE;
	}

	return FALSE;	
}

/* ---------------------------- CJS_PropValue ---------------------------- */

CFXJS_PropValue::CFXJS_PropValue(const CFXJS_Value &value) : 
	CFXJS_Value(value),
	m_bIsSetting(0)
{
}

CFXJS_PropValue::CFXJS_PropValue() : CFXJS_Value(),
                                 m_bIsSetting(0)
{
}

CFXJS_PropValue::~CFXJS_PropValue()
{
}

FX_BOOL CFXJS_PropValue::IsSetting()
{
	return m_bIsSetting;
}

FX_BOOL CFXJS_PropValue::IsGetting()
{
	return !m_bIsSetting;
}

void CFXJS_PropValue::operator <<(int iValue)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator =(iValue);
}

void CFXJS_PropValue::operator >>(int & iValue)
{
	ASSERT(m_bIsSetting);
	iValue = CFXJS_Value::operator int();
}


void CFXJS_PropValue::operator <<(bool bValue)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator =(bValue);
}

void CFXJS_PropValue::operator >>(bool &bValue)
{
	ASSERT(m_bIsSetting);
	bValue = CFXJS_Value::operator bool();

}

void CFXJS_PropValue::operator <<(double dValue)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator =(dValue);
}

void CFXJS_PropValue::operator >>(double &dValue)
{
	ASSERT(m_bIsSetting);
	dValue = CFXJS_Value::operator double();
}

void CFXJS_PropValue::operator <<(CFXJS_Object *pObj)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator = (pObj);
}

void CFXJS_PropValue::operator >>(CFXJS_Object *&ppObj)
{
	ASSERT(m_bIsSetting);
	ppObj = CFXJS_Value::operator CFXJS_Object *();
}

void CFXJS_PropValue::operator<<(JSFXObject* pObj)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator = (pObj);
}

void CFXJS_PropValue::operator>>(JSFXObject* &ppObj)
{
	ASSERT(m_bIsSetting);
	ppObj = CFXJS_Value::operator JSFXObject *();
}

void CFXJS_PropValue::operator<<(JSObject* pObj)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator = (pObj);
}

void CFXJS_PropValue::operator>>(JSObject*& ppObj)
{
	ASSERT(m_bIsSetting);
	ppObj = CFXJS_Value::operator JSObject *();
}

void CFXJS_PropValue::StartSetting()
{
	m_bIsSetting = 1;
}

void CFXJS_PropValue::StartGetting()
{
	m_bIsSetting = 0;
}
void CFXJS_PropValue::operator <<(CFX_ByteString string)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator =((FX_LPCSTR)string);
}

void CFXJS_PropValue::operator >>(CFX_ByteString &string)
{
	ASSERT(m_bIsSetting);
	string = CFXJS_Value::operator CFX_ByteString();
}

void CFXJS_PropValue::operator <<(FX_LPCWSTR c_string)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator =(c_string);
}

/*
void CJS_PropValue::operator >>(std::string &string)
{
	ASSERT(m_bIsSetting);
	string = CJS_Value::operator const char * ();
}
*/

/*
void CJS_PropValue::operator <<(const std::string &string)
{
	ASSERT(!m_bIsSetting);
	CJS_Value::operator = (string.c_str());
}
*/

void CFXJS_PropValue::operator >>(CFX_WideString &wide_string)
{
	ASSERT(m_bIsSetting);
	wide_string = CFXJS_Value::operator FX_LPCWSTR();
}

void CFXJS_PropValue::operator <<(CFX_WideString wide_string)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator = (wide_string);
}

void CFXJS_PropValue::operator >>(CFXJS_Array &array)
{
	ASSERT(m_bIsSetting);
	ConvertToArray(array);
}

void CFXJS_PropValue::operator <<(CFXJS_Array &array)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator=(array);
}

void CFXJS_PropValue::operator>>(CFXJS_Date &date)
{
	ASSERT(m_bIsSetting);
	ConvertToDate(date);
}

void CFXJS_PropValue::operator<<(CFXJS_Date &date)
{
	ASSERT(!m_bIsSetting);
	CFXJS_Value::operator=(date);
}

/* ======================================== CJS_Array ========================================= */

CFXJS_Array::CFXJS_Array() : m_pArray(NULL)
{
}

CFXJS_Array::~CFXJS_Array()
{		
}

void CFXJS_Array::Attach(Darray * pArray)
{
	m_pArray = pArray;
}

FX_BOOL CFXJS_Array::IsAttached()
{
	return m_pArray != NULL;
}

void CFXJS_Array::GetElement(unsigned index,CFXJS_Value &value)
{
	Value * p = DS_GetArrayElemnet(m_pArray,index);
	value.Attach(p,VT_object);
}

void CFXJS_Array::SetElement(unsigned index,CFXJS_Value value)
{
	if (!m_pArray)
		m_pArray = DS_NewArray();

	DS_PutArrayElement(m_pArray,index,value.ToJSValue(),value.GetType());
}

int CFXJS_Array::GetLength()
{
	if (m_pArray)
		return DS_GetArrayLength(m_pArray);
	return 0;
}

CFXJS_Array:: operator Darray *()
{
	if (!m_pArray)
		m_pArray = DS_NewArray();

	return m_pArray;
}

/* ======================================== CJS_Date ========================================= */

CFXJS_Date::CFXJS_Date() : m_pDate(NULL)
{
}

CFXJS_Date::CFXJS_Date(double dMsec_time) 
{
	m_pDate = DS_NewDate(dMsec_time);		
}

CFXJS_Date::CFXJS_Date(int year, int mon, int day,int hour, int min, int sec) 
{
	m_pDate = DS_NewDate(MakeDate(year,mon,day,hour,min,sec,0));	
}

double CFXJS_Date::MakeDate(int year, int mon, int day,int hour, int min, int sec,int ms)
{
	return DS_MakeDate(DS_MakeDay(year,mon,day), DS_MakeTime(hour,min,sec,ms));
}

CFXJS_Date::~CFXJS_Date()
{
}

FX_BOOL	CFXJS_Date::IsValidDate()
{
	if (m_pDate)
		return !DS_PortIsNan(DS_ToNumber(m_pDate));
	
	return FALSE;
}

void CFXJS_Date::Attach(Value *pDate)
{
	m_pDate = pDate;
}

int CFXJS_Date::GetYear()
{
	if (m_pDate && IsValidDate())
		return DS_GetYearFromTime(DS_LocalTime(DS_ToNumber(m_pDate)));

	return 0;
}

void CFXJS_Date::SetYear(int iYear)
{
	if (m_pDate)
	{
		double date = MakeDate(iYear,GetMonth(),GetDay(),GetHours(),GetMinutes(),GetSeconds(),0);
		DS_ValueCopy(m_pDate, DS_NewDate(date));
	}
}

int CFXJS_Date::GetMonth()
{
	if (m_pDate && IsValidDate())
		return DS_GetMonthFromTime(DS_LocalTime(DS_ToNumber(m_pDate)));

	return 0;
}

void CFXJS_Date::SetMonth(int iMonth)
{
	if (m_pDate)
	{
		double date = MakeDate(GetYear(),iMonth,GetDay(),GetHours(),GetMinutes(),GetSeconds(),0);
		DS_ValueCopy(m_pDate, DS_NewDate(date));
	}
}

int CFXJS_Date::GetDay()
{
	if (m_pDate && IsValidDate())
		return DS_GetDayFromTime(DS_LocalTime(DS_ToNumber(m_pDate)));

	return 0;
}

void CFXJS_Date::SetDay(int iDay)
{
	if (m_pDate)
	{
		double date = MakeDate(GetYear(),GetMonth(),iDay,GetHours(),GetMinutes(),GetSeconds(),0);
		DS_ValueCopy(m_pDate,DS_NewDate(date));
	}
}

int CFXJS_Date::GetHours()
{
	if (m_pDate && IsValidDate())
		return DS_GetHourFromTime(DS_LocalTime(DS_ToNumber(m_pDate)));

	return 0;
}

void CFXJS_Date::SetHours(int iHours)
{
	if (m_pDate)
	{
		double date = MakeDate(GetYear(),GetMonth(),GetDay(),iHours,GetMinutes(),GetSeconds(),0);
		DS_ValueCopy(m_pDate,DS_NewDate(date));
	}
}

int CFXJS_Date::GetMinutes()
{
	if (m_pDate && IsValidDate())
		return DS_GetMinFromTime(DS_LocalTime(DS_ToNumber(m_pDate)));

	return 0;
}

void CFXJS_Date::SetMinutes(int minutes)
{
	if (m_pDate)
	{
		double date = MakeDate(GetYear(),GetMonth(),GetDay(),GetHours(),minutes,GetSeconds(),0);
		DS_ValueCopy(m_pDate,DS_NewDate(date));
	}
}

int CFXJS_Date::GetSeconds()
{
	if (m_pDate && IsValidDate())
		return DS_GetSecFromTime(DS_LocalTime(DS_ToNumber(m_pDate)));

	return 0;
}

void CFXJS_Date::SetSeconds(int seconds)
{
	if (m_pDate)
	{
		double date = MakeDate(GetYear(),GetMonth(),GetDay(),GetHours(),GetMinutes(),seconds,0);
		DS_ValueCopy(m_pDate,DS_NewDate(date));
	}
}

CFXJS_Date::operator JSValue *()
{
	return m_pDate;
}

CFXJS_Date::operator double() const
{
	if (m_pDate)
		return DS_ToNumber(m_pDate);

	return 0;
}

CFX_WideString CFXJS_Date::ToString() const
{
	if (m_pDate)
		return DS_ToString(m_pDate);

	return L"";
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)