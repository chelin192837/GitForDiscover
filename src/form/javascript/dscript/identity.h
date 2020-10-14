#ifndef _IDENTITY_H_
#define _IDENTITY_H_

class jidentity : public CFXJS_EmbedObj
{
public:
	jidentity(CFXJS_Object * pJSObject);

public:
	FX_BOOL corporation(OBJ_PROP_PARAMS);
	FX_BOOL email(OBJ_PROP_PARAMS);
	FX_BOOL loginName(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);

private:
	CFX_WideString m_sName;
	CFX_WideString m_sEmailAddress;
	CFX_WideString m_sCorporation;

};

class CFXJS_identity : public CFXJS_Object
{
public:
	CFXJS_identity(JSFXObject * pObject) : CFXJS_Object(pObject){};
	virtual ~CFXJS_identity(){};

public:
	DECLARE_JS_CLASS(CFXJS_identity);

	JS_STATIC_PROP(corporation, jidentity);
	JS_STATIC_PROP(email, jidentity);
	JS_STATIC_PROP(loginName, jidentity);
	JS_STATIC_PROP(name, jidentity);
};

#endif //_IDENTITY_H_