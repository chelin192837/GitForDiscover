#ifndef _UTIL_H_
#define _UTIL_H_

class jutil : public CFXJS_EmbedObj
{
public:
	jutil(CFXJS_Object * pJSObject);
	virtual ~jutil(void);

public:
	FX_BOOL printd(OBJ_METHOD_PARAMS);
	FX_BOOL printf(OBJ_METHOD_PARAMS);
	FX_BOOL printx(OBJ_METHOD_PARAMS);
	FX_BOOL scand(OBJ_METHOD_PARAMS);
	FX_BOOL byteToChar(OBJ_METHOD_PARAMS);

public:
	static void		printd(const CFX_WideString &cFormat,CFXJS_Date Date,bool bXFAPicture, CFX_WideString &cPurpose);
	static void		printx(const CFX_ByteString &cFormat,const CFX_ByteString &cSource, CFX_ByteString &cPurpose);
	static int		ParstDataType(CFX_WideString* sFormat);
};

class CFXJS_Util : public CFXJS_Object
{
public:
	CFXJS_Util(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Util(void){};

	DECLARE_JS_CLASS(CFXJS_Util);

	JS_STATIC_METHOD(printd, jutil);
	JS_STATIC_METHOD(printf, jutil);
	JS_STATIC_METHOD(printx, jutil);
	JS_STATIC_METHOD(scand, jutil);
	JS_STATIC_METHOD(byteToChar, jutil);
};

FX_INT64 JFX_atoi64(const char *nptr);
#endif //_UTIL_H_
