#ifndef _CURSOR_H_
#define _CURSOR_H_

class cursor : public CFXJS_EmbedObj
{
public:
	cursor(CFXJS_Object* pJSObject);
	virtual ~cursor();
	
public:
	FX_BOOL delay(OBJ_PROP_PARAMS);
	FX_BOOL hidden(OBJ_PROP_PARAMS);
	FX_BOOL visible(OBJ_PROP_PARAMS);
};

class CJS_cursor : public CFXJS_Object
{
public:
	CJS_cursor(JSFXObject* pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_cursor(){};

public:
	DECLARE_JS_CLASS(CJS_cursor);

	JS_STATIC_PROP(delay, cursor);
	JS_STATIC_PROP(hidden, cursor);
	JS_STATIC_PROP(visible, cursor);
};

#endif //_CURSOR_H_