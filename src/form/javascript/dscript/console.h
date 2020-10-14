#ifndef _CONSOLE_H_
#define _CONSOLE_H_

class jconsole : public CFXJS_EmbedObj
{
public:
	jconsole(CFXJS_Object* pJSObject);
	virtual ~jconsole(void);

public:
	FX_BOOL clear(OBJ_METHOD_PARAMS);
	FX_BOOL hide(OBJ_METHOD_PARAMS);
	FX_BOOL println(OBJ_METHOD_PARAMS);
	FX_BOOL show(OBJ_METHOD_PARAMS);
};

class CFXJS_Console : public CFXJS_Object  
{
public:
	CFXJS_Console(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Console(void){};

	DECLARE_JS_CLASS(CFXJS_Console);

	JS_STATIC_METHOD(clear, jconsole);
	JS_STATIC_METHOD(hide, jconsole);
	JS_STATIC_METHOD(println, jconsole);
	JS_STATIC_METHOD(show, jconsole);
};

#endif //_CONSOLE_H_

