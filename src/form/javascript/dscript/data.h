#ifndef _DATA_H_
#define _DATA_H_

class Data : public CFXJS_EmbedObj
{
public:
	Data(CFXJS_Object * pJSObject);
	virtual ~Data();
	
public:
	FX_BOOL creationDate(OBJ_PROP_PARAMS);

private:
	CFX_WideString m_swDataName;
	
};

class CJS_Data : public CFXJS_Object
{
public:
	CJS_Data(JSFXObject * pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_Data(){};

public:
	DECLARE_JS_CLASS(CJS_Data);
	JS_STATIC_PROP(creationDate, Data);
};

#endif//_DATA_H_