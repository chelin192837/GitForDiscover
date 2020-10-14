#ifndef _ICON_H_
#define _ICON_H_

class JIcon : public CFXJS_EmbedObj
{
public:
	JIcon(CFXJS_Object* pJSObject);
	virtual ~JIcon();

public:
	FX_BOOL name(OBJ_PROP_PARAMS);
	
public:
	void				SetStream(CPDF_Stream* pIconStream);
	CPDF_Stream*		GetStream();
	void				SetIconName(CFX_WideString name);
	CFX_WideString		GetIconName();
private:
	CPDF_Stream*		m_pIconStream;
	CFX_WideString		m_swIconName;
};

class CFXJS_Icon : public CFXJS_Object
{
public:
	CFXJS_Icon(JSFXObject* pObject) : CFXJS_Object(pObject){};
	virtual ~CFXJS_Icon(){};

public:
	DECLARE_JS_CLASS(CFXJS_Icon);

	JS_STATIC_PROP(name, JIcon);
};

#endif //_ICON_H_

