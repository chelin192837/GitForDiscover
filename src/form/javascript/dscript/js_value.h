#ifndef _JS_VALUE_H_
#define _JS_VALUE_H_

class CFXJS_Array;
class CFXJS_Date;

class CFXJS_Value
{
public:
	CFXJS_Value();
	CFXJS_Value(JSValue *pValue,VALUETYPE t);
	CFXJS_Value(const int &iValue);
	CFXJS_Value(const double &dValue);
	CFXJS_Value(const float &fValue);	
	CFXJS_Value(const bool &bValue);
	CFXJS_Value(JSFXObject *);
	CFXJS_Value(JSObject *);
	CFXJS_Value(CFXJS_Object *);
	CFXJS_Value(FX_LPCSTR pStr);
	CFXJS_Value(FX_LPCWSTR pWstr);
	CFXJS_Value(CFXJS_Array& array);
	
	~CFXJS_Value();

	void SetNull();
    void Attach(JSValue *pValue,VALUETYPE t);
	void Attach(CFXJS_Value *pValue);
	void Detach();

	operator int() const;
	operator bool() const;
	operator double() const;
	operator float() const;
	operator CFXJS_Object *() const;
	operator JSFXObject *() const;
	operator JSObject *() const;
	operator Darray *() const;
	operator FX_LPCWSTR() const;
	operator FX_WCHAR *() const;
	operator CFX_ByteString() const;
	JSValue* ToJSValue();

	void operator = (int iValue);
	void operator = (bool bValue);	
	void operator = (double);	
	void operator = (float);	
	void operator = (CFXJS_Object *);	
	void operator = (JSFXObject *);
	void operator = (JSObject *);
	void operator = (CFXJS_Array &);
	void operator = (CFXJS_Date &);
	void operator = (FX_LPCWSTR pWstr);	
	void operator = (FX_LPCSTR pStr);	
	void operator = (CFXJS_Value value);
	
	FX_BOOL IsArrayObject() const;
	FX_BOOL	IsDateObject() const;
	VALUETYPE GetType() const;

	FX_BOOL ConvertToArray(CFXJS_Array &) const;
	FX_BOOL ConvertToDate(CFXJS_Date &) const;

protected:	
	JSValue  *m_pValue;
	VALUETYPE m_eType;
};

template<class TYPE> class CJS_ParametersTmpl : public CFX_ArrayTemplate<TYPE>
{
public:
	void push_back(TYPE newElement){CFX_ArrayTemplate<TYPE>::Add(newElement);}
	int size() const{return CFX_ArrayTemplate<TYPE>::GetSize();}
};
typedef CJS_ParametersTmpl<CFXJS_Value> CJS_Parameters;

class CFXJS_PropValue: public CFXJS_Value
{
public:
	CFXJS_PropValue(const CFXJS_Value &);
	CFXJS_PropValue();
	~CFXJS_PropValue();
public:
	FX_BOOL IsSetting();
	FX_BOOL IsGetting();
	void operator<<(int );
	void operator>>(int &);
	void operator<<(bool);
	void operator>>(bool &);
	void operator<<(double );
	void operator>>(double &);
	void operator<<(CFXJS_Object *pObj);
	void operator>>(CFXJS_Object *&ppObj);
	void operator<<(CFX_ByteString);
	void operator>>(CFX_ByteString &);
	void operator<<(CFX_WideString);
	void operator>>(CFX_WideString &);
	void operator<<(FX_LPCWSTR c_string);

	void operator<<(JSFXObject*);
	void operator>>(JSFXObject* &);

	void operator<<(JSObject*);
	void operator>>(JSObject* &);

	void operator>>(CFXJS_Array &array);
	void operator<<(CFXJS_Array &array);

	void operator<<(CFXJS_Date &date);
	void operator>>(CFXJS_Date &date);
	void StartSetting();
	void StartGetting();
private:
	FX_BOOL m_bIsSetting;
};

class CFXJS_Array
{
public:
	CFXJS_Array();
	virtual ~CFXJS_Array();

	void Attach(Darray * pArray);
	void GetElement(unsigned index,CFXJS_Value &value);
	void SetElement(unsigned index,CFXJS_Value value);
    int GetLength();
	FX_BOOL IsAttached();
	operator Darray *();
private:
	Darray	* m_pArray;
};

class CFXJS_Date
{
friend class CFXJS_Value;
public:
	CFXJS_Date();
	CFXJS_Date(double dMsec_time);
	CFXJS_Date(int year, int mon, int day,int hour, int min, int sec);
	virtual ~CFXJS_Date();
	void Attach(Value *pDate);

	int     GetYear();
	void    SetYear(int iYear);

	int     GetMonth();
	void    SetMonth(int iMonth);

	int     GetDay();
	void    SetDay(int iDay);

	int     GetHours();
	void    SetHours(int iHours);

	int     GetMinutes();
	void    SetMinutes(int minutes);

	int     GetSeconds();
	void    SetSeconds(int seconds);

	operator JSValue*();
	operator double() const;

	CFX_WideString	ToString() const;

	static double	MakeDate(int year, int mon, int mday,int hour, int min, int sec,int ms);

	FX_BOOL	IsValidDate();

protected:
	Value	* m_pDate;
};

static CFXJS_Value PARAM_TO_JSVALUE(Value * p)
{
	assert(p != NULL);

	CFXJS_Value ret;
	
	VALUETYPE type = GET_VALUE_TYPE(p);
	switch(type)
	{
	case VT_string:
		ret.Attach(p,VT_string);
		break;
	case VT_number:
		ret.Attach(p,VT_number);
		break;
	case VT_boolean:
		ret.Attach(p,VT_boolean);
		break;
	case VT_date:
		ret.Attach(p,VT_date);
		break;
	case VT_object:
		{
			Dobject * pObj = DS_ToObject(p);
			if (pObj && FXSYS_wcscmp(DS_GetClassname(pObj), CLASSNAME_STRING) == 0)
			{
				ret.Attach(DS_GetObjectValue(pObj), VT_string);
			}
			else
				ret.Attach(p,VT_object);
		}
		break;
	case VT_fxobject:
		ret.Attach(p,VT_fxobject);
		break;
	case VT_null:
		ret.Attach(p,VT_null);
		break;
	case VT_undefined:
		ret.Attach(p,VT_undefined);
		break;
	}
	
	return ret;
}

#endif //_JS_VALUE_H_

