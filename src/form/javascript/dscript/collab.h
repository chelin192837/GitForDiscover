#ifndef _COLLAB_H_
#define _COLLAB_H_

class jcollab : public CFXJS_EmbedObj
{
public:
	jcollab(CFXJS_Object* pJSObject);
	virtual ~jcollab();

public:
	FX_BOOL user(OBJ_PROP_PARAMS);
};

class CFXJS_Collab : public CFXJS_Object
{
public:
	CFXJS_Collab(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CFXJS_Collab(){}

	DECLARE_JS_CLASS(CFXJS_Collab);

	JS_STATIC_PROP(user, jcollab);
};

#endif//_COLLAB_H_

