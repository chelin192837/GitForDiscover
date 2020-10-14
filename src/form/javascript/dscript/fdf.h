#ifndef _FDF_H_
#define _FDF_H_

class FDF : public CFXJS_EmbedObj
{
public:
	FDF(CFXJS_Object * pJSObject);
	virtual ~FDF(void);

	FX_BOOL addContact(OBJ_METHOD_PARAMS);
	FX_BOOL addRequest(OBJ_METHOD_PARAMS);
	FX_BOOL close(OBJ_METHOD_PARAMS);
	FX_BOOL mail(OBJ_METHOD_PARAMS);
	FX_BOOL save(OBJ_METHOD_PARAMS);
};

class CJS_FDF : public CFXJS_Object
{
public:
	CJS_FDF(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_FDF(void){};

	DECLARE_JS_CLASS(CJS_FDF);

	JS_STATIC_METHOD(addContact, FDF);
	JS_STATIC_METHOD(addRequest, FDF);
	JS_STATIC_METHOD(close, FDF);
	JS_STATIC_METHOD(mail, FDF);
	JS_STATIC_METHOD(save, FDF);
};


#endif //_FDF_H_