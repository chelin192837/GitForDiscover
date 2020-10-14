#ifndef _LINK_H_
#define _LINK_H_

class Link : public CFXJS_EmbedObj
{
public:
	Link(CFXJS_Object* pJSObject);
	virtual ~Link();

public:
	FX_BOOL borderColor(OBJ_PROP_PARAMS);
	FX_BOOL borderWidth(OBJ_PROP_PARAMS);
	FX_BOOL highlightMode(OBJ_PROP_PARAMS);
	FX_BOOL rect(OBJ_PROP_PARAMS);

	FX_BOOL setAction(OBJ_METHOD_PARAMS);

public:
	void				SetAnnot(CFSCRT_LTPDFAnnot* pLink);
	CFX_ByteString		GetHighLightMode(CFX_ByteString cbValue);
	CFX_ByteString		GetModeFromUser(CFX_ByteString cbUser);

private:
	CFSCRT_LTPDFAnnot*			m_pLink;
};

class CJS_Link : public CFXJS_Object
{
public:
	CJS_Link(JSFXObject* pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_Link(){};

public:
	DECLARE_JS_CLASS(CJS_Link);
	JS_STATIC_PROP(borderColor, Link);
	JS_STATIC_PROP(borderWidth, Link);
	JS_STATIC_PROP(highlightMode, Link);
	JS_STATIC_PROP(rect, Link);

	JS_STATIC_METHOD(setAction, Link);
};

#endif //_LINK_H_

