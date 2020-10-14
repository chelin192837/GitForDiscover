#ifndef _DATABASE_H_
#define _DATABASE_H_

class SQL_VALUE
{
public:
	typedef enum
	{
		sql_unkown = 0,
		sql_bool,
		sql_int,
		sql_short,
		sql_long,
		sql_float,
		sql_double,
		sql_byte,
		sql_wstring,
		sql_time,
		sql_date,
		sql_timestamp,
		sql_type_date,
		sql_type_time,
		sql_type_timestamp
	}sql_type;

public:
	SQL_VALUE();
	SQL_VALUE(sql_type type, void* lpValue, int nLength);
	SQL_VALUE(sql_type type, const wchar_t* lpwstr, int nLength);
	SQL_VALUE(const SQL_VALUE& value);
	~SQL_VALUE();

	operator int () const;
	operator bool () const;
	operator short () const;
	operator long () const;
	operator float () const;
	operator double () const;
	operator unsigned char* () const;
	operator wchar_t* () const;
	SQL_VALUE& operator = (const SQL_VALUE& value);
	SQL_VALUE& operator = (const int i);
	SQL_VALUE& operator = (const bool b);
	SQL_VALUE& operator = (const short s);
	SQL_VALUE& operator = (const long l);
	SQL_VALUE& operator = (const double d);
	SQL_VALUE& operator = (const unsigned char* lpbyte);
	SQL_VALUE& operator = (const wchar_t* wstring);
	BOOL operator == (const SQL_VALUE& value) const;
	void Clear();
	int GetBufferLen() const {return bufferlen;}
	SQL_VALUE::sql_type GetType() const {return type;}
	static SQL_VALUE::sql_type GetType(int iODBCType);
	int GetYear() const {return time.year;}
	int GetMonth() const {return time.month;}
	int GetDay() const {return time.day;}
	int GetHour() const {return time.hour;}
	int GetMinute() const {return time.minute;}
	int GetSecond() const {return time.second;}
	int GetFraction() const {return time.fraction;}

protected:
	union
	{
		struct _tag_sql_time
		{
			int year; 
			int month; 
			int day; 
			int hour; 
			int minute; 
			int second; 
			int fraction;
		}time;

		bool bValue;
		int iValue;
		short stValue;
		long lValue;
		float fValue;
		double dValue;	
		unsigned char* lpValue;      //alloc buffer at heap
		wchar_t* wsValue;            //alloc buffer at heap		
	};

	sql_type	type;
	int			bufferlen;
};

typedef CFX_ArrayTemplate<SQL_VALUE*>		SQL_VALUEArray;

/* ---------------------------- ADBC ---------------------------- */

class ADBC : public CFXJS_EmbedObj
{
public:
	ADBC(CFXJS_Object* pJSObject);
	virtual ~ADBC();

	 enum SQLTYPE
	 {
		 SQLT_BIGINT       = 0,
		 SQLT_BINARY          ,
		 SQLT_BIT             ,
		 SQLT_CHAR            ,
		 SQLT_DATE            ,
		 SQLT_DECIMAL		  ,
		 SQLT_DOUBLE		  ,	
		 SQLT_FLOAT			  ,
		 SQLT_INTEGER		  ,	
		 SQLT_LONGVARBINARY   ,
		 SQLT_LONGVARCHAR	  ,
		 SQLT_NUMERIC		  ,
		 SQLT_REAL			  ,
		 SQLT_SMALLINT		  ,	
		 SQLT_TIME			  ,
		 SQLT_TIMESTAMP		  ,
		 SQLT_TINYINT		  ,
		 SQLT_VARBINARY		  ,
		 SQLT_VARCHAR		  ,
		 SQLT_NCHAR			  ,
		 SQLT_NVARCHAR		  ,
		 SQLT_NTEXT
	 };

	enum JAVASCRIPTTYPE
	{
		Numeric            = 0,
		String				  ,	
		Binary				  ,	
		Boolean				  ,	
		Time			      ,
		Date				  ,	
		TimeStamp
	};
public:
	static SQLTYPE GetSQLType(int iODBCType);
	static JAVASCRIPTTYPE GetJSType(int iODBCType);

public:
	FX_BOOL JavaScripttypes(OBJ_PROP_PARAMS);
	FX_BOOL SQLtypes(OBJ_PROP_PARAMS);

	FX_BOOL getDataSourceList(OBJ_METHOD_PARAMS);
	FX_BOOL newConnection(OBJ_METHOD_PARAMS);

private:
	void EnumDBDrivers(HKEY hKey, FX_LPCWSTR sRegPath, 
		CFX_WideStringArray& key_array, CFX_WideStringArray& driver_array);
	void EnumDBSourceDescrip(HKEY hKey, CFX_WideStringArray& key_array, CFX_WideStringArray& descrip_array);
};


class CJS_ADBC : public CFXJS_Object
{
public:
	CJS_ADBC(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_ADBC(){};

	DECLARE_JS_CLASS(CJS_ADBC);

	JS_STATIC_PROP(JavaScripttypes, ADBC);
	JS_STATIC_PROP(SQLtypes,ADBC);

	JS_STATIC_METHOD(getDataSourceList, ADBC);
	JS_STATIC_METHOD(newConnection, ADBC);
};

/* ---------------------------- Column ---------------------------- */

class Column : public CFXJS_EmbedObj
{
public:
	Column(CFXJS_Object* pJSObject);
	virtual ~Column();

public:
	FX_BOOL columnNum(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL type(OBJ_PROP_PARAMS);
	FX_BOOL typeName(OBJ_PROP_PARAMS);
	FX_BOOL value(OBJ_PROP_PARAMS);

public:
	void SetValue(SQL_VALUEArray* pValueArray){m_pcbaRowValues = pValueArray;}
	void SetColumnName(CFX_WideString sName){m_sColumnName = sName;}
	void SetType(int iType){m_iType = iType;}
	void SetTypeName(CFX_WideString sTypeName){m_sTypeName = sTypeName;}

private:
	int m_iType;
	CRecordset*				m_pConnect;
	CFX_WideString			m_sTypeName;
	CFX_WideString			m_sColumnName;
	SQL_VALUEArray*			m_pcbaRowValues;
};

class CJS_Column : public CFXJS_Object
{
public:
	CJS_Column(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CJS_Column(){}

	DECLARE_JS_CLASS(CJS_Column);

	JS_STATIC_PROP(columnNum, Column);
	JS_STATIC_PROP(name, Column);
	JS_STATIC_PROP(type, Column);
	JS_STATIC_PROP(typeName, Column);
	JS_STATIC_PROP(value, Column);
};

/* ---------------------------- ColumnInfo ---------------------------- */

class ColumnInfo : public CFXJS_EmbedObj
{
public:
	ColumnInfo(CFXJS_Object* pJSObject);
	virtual ~ColumnInfo();

public:
	FX_BOOL description(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL type(OBJ_PROP_PARAMS);
	FX_BOOL typeName(OBJ_PROP_PARAMS);

public:
	void SetType(int iSQLType){m_iType = iSQLType;}
	void SetTypeName(CFX_WideString sTypeName){m_sTypeName = sTypeName;}
	void SetColName(CFX_WideString sColName){m_sName = sColName;}
	void SetDescription(CFX_WideString sDecription){m_sDescription = sDecription;}

private:
	CFX_WideString		m_sName;
	CFX_WideString		m_sDescription;
	CFX_WideString		m_sTypeName;
	int					m_iType;
};

class CJS_ColumnInfo : public CFXJS_Object
{
public:
	CJS_ColumnInfo(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CJS_ColumnInfo(void){}

	DECLARE_JS_CLASS(CJS_ColumnInfo);

	JS_STATIC_PROP(description, ColumnInfo);
	JS_STATIC_PROP(name, ColumnInfo);
	JS_STATIC_PROP(type, ColumnInfo);
	JS_STATIC_PROP(typeName, ColumnInfo);
};

/* ---------------------------- Connection ---------------------------- */

typedef CFX_ArrayTemplate<CRecordset*>		CADBC_Connections;

class StatementObj;
class Connection : public CFXJS_EmbedObj
{
public:
	Connection(CFXJS_Object* pJSObject);
	virtual ~Connection();

	typedef struct _tag_data_binding 
	{
		SQLSMALLINT TargetType;
		SQLPOINTER TargetValuePtr;
		SQLINTEGER BufferLength;
		long StrLen_or_Ind;

	}DATABINDING, *LPDATABINGDING;


public:
	FX_BOOL close(OBJ_METHOD_PARAMS);
	FX_BOOL getColumnList(OBJ_METHOD_PARAMS);
	FX_BOOL getTableList(OBJ_METHOD_PARAMS);
	FX_BOOL newStatement(OBJ_METHOD_PARAMS);

public:
	void			SetDSN(CFX_WideString sDSN){m_sDSN = sDSN;}
	void			SetConnection(CDatabase* pDataBase);

protected:
	void AddTableList(CFX_WideString sTableName);

private:
	CFX_WideString				m_sDSN;
	CDatabase*					m_pDataBase;
	CRecordset*					m_pConnection;
	StatementObj*				m_pStatement;
};

class CJS_Connection : public CFXJS_Object
{
public:
	CJS_Connection(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_Connection(void){};

	DECLARE_JS_CLASS(CJS_Connection);

	JS_STATIC_METHOD(close, Connection);
	JS_STATIC_METHOD(getColumnList, Connection);
	JS_STATIC_METHOD(getTableList, Connection);
	JS_STATIC_METHOD(newStatement, Connection);
};

/* ---------------------------- DataSourceInfo ---------------------------- */

class DataSourceInfo : public CFXJS_EmbedObj
{
public:
	DataSourceInfo(CFXJS_Object* pJSObject);
	virtual ~DataSourceInfo();

public:
	FX_BOOL description(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);

public:
	void SetName(CFX_WideString sName){m_sName = sName;}
	void SetDriver(CFX_WideString sDriver){m_sDriver = sDriver;}
	void SetDescription(CFX_WideString sDescription){m_sDescription = sDescription;}

private:
	CFX_WideString m_sName;
	CFX_WideString m_sDriver;
	CFX_WideString m_sDescription;
};

class CJS_DataSourceInfo : public CFXJS_Object
{
public:
	CJS_DataSourceInfo(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_DataSourceInfo(void){};

	DECLARE_JS_CLASS(CJS_DataSourceInfo);

	JS_STATIC_PROP(description, DataSourceInfo);
	JS_STATIC_PROP(name, DataSourceInfo);
};

/* ---------------------------- Row ---------------------------- */

class Row : public CFXJS_EmbedObj
{
public:
	Row(CFXJS_Object* pJSObject);
	virtual ~Row();

public:
	FX_BOOL columnArray(OBJ_PROP_PARAMS);
	FX_BOOL columnproperties(OBJ_PROP_PARAMS);

public:
	void SetColumns(CFX_WideStringArray* pColumns){m_pcbaColumns = pColumns;}
	void SetRowValues(SQL_VALUEArray* pValues){m_pcbaRowValues = pValues;}
	void SetStatement(CRecordset* pStatement){m_pStatement = pStatement;}
	void SetRowPosition(int iPosition){m_nRowPosition = iPosition;}

protected:
	BOOL UpdateDatabase(LPCWSTR lpPropName, const CFXJS_PropValue& value);
	void UpdateString(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateBinary(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateNumeric(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateInt(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateFloat(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateShort(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateBit(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateDate(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateTime(SQLHSTMT& hstmt, const CFXJS_PropValue& value);
	void UpdateTimestamp(SQLHSTMT& hstmt, const CFXJS_PropValue& value);

	DATE_STRUCT ParserDate(const CFX_WideString& sDate, FX_BOOL& bRet);
	TIME_STRUCT ParserTime(const CFX_WideString& sDate, FX_BOOL& bRet);
	TIMESTAMP_STRUCT ParserTimestamp(const CFX_WideString& sDate, FX_BOOL& bRet);

	friend JSBool get_row_static(DS_PROPGET_ARGS);
	friend JSBool set_row_static(DS_PROPPUT_ARGS);

private:
	CRecordset*				m_pStatement;
	int						m_nRowPosition; // base 1
	int						m_nColumnIndex;
	CFX_WideStringArray*	m_pcbaColumns;
	SQL_VALUEArray*			m_pcbaRowValues;
};

class CJS_Row : public CFXJS_Object
{
public:
	CJS_Row(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CJS_Row(){}
	
	DECLARE_JS_CLASS(CJS_Row);

	JS_STATIC_PROP(columnArray, Row);
	JS_STATIC_PROP(columnproperties, Row);
};

/* ---------------------------- StatementObj ---------------------------- */

class StatementObj : public CFXJS_EmbedObj
{
public:
	StatementObj(CFXJS_Object* pJSObject);
	virtual ~StatementObj();

public:
	FX_BOOL columnCount(OBJ_PROP_PARAMS);
	FX_BOOL rowCount(OBJ_PROP_PARAMS);

	FX_BOOL execute(OBJ_METHOD_PARAMS);
	FX_BOOL getColumn(OBJ_METHOD_PARAMS);
	FX_BOOL getColumnArray(OBJ_METHOD_PARAMS);
	FX_BOOL getRow(OBJ_METHOD_PARAMS);
	FX_BOOL nextRow(OBJ_METHOD_PARAMS);

public:
	void SetStatement(CRecordset* pStatement){m_pStatement = pStatement;}

private:
	CRecordset*			m_pStatement;
	BOOL				m_bIsConnected;
	int					m_iPositionIndex;
};

class CJS_Statement : public CFXJS_Object
{
public:
	CJS_Statement(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CJS_Statement(){}

	DECLARE_JS_CLASS(CJS_Statement);

	JS_STATIC_PROP(columnCount, StatementObj);
	JS_STATIC_PROP(rowCount, StatementObj);
	
	JS_STATIC_METHOD(execute, StatementObj);
	JS_STATIC_METHOD(getColumn, StatementObj);
	JS_STATIC_METHOD(getColumnArray, StatementObj);
	JS_STATIC_METHOD(getRow, StatementObj);
	JS_STATIC_METHOD(nextRow, StatementObj);
};

/* ---------------------------- TableInfo ---------------------------- */

class TableInfo : public CFXJS_EmbedObj
{
public:
	TableInfo(CFXJS_Object* pJSObject);
	virtual ~TableInfo();

public:
	FX_BOOL description(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);

public:
	void SetTableName(CFX_WideString sTableName){m_sTableName = sTableName;}
	void SetTableDescription(CFX_WideString sTableDescription){m_sTableDescription = sTableDescription;}

private:
	CFX_WideString m_sTableName;
	CFX_WideString m_sTableDescription;
};

class CJS_TableInfo : public CFXJS_Object
{
public:
	CJS_TableInfo(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_TableInfo(){}
	
	DECLARE_JS_CLASS(CJS_TableInfo);

	JS_STATIC_PROP(description,TableInfo);
	JS_STATIC_PROP(name,TableInfo);
};

#endif //_DATABASE_H_