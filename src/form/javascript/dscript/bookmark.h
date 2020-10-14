#ifndef _BOOKMARK_H_
#define _BOOKMARK_H_

class Bookmark : public CFXJS_EmbedObj
{
public:
	Bookmark(CFXJS_Object* pJSObject);
	virtual ~Bookmark();

public:
	FX_BOOL children(OBJ_PROP_PARAMS);
	FX_BOOL color(OBJ_PROP_PARAMS);
	FX_BOOL doc(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL parent(OBJ_PROP_PARAMS);
	FX_BOOL style(OBJ_PROP_PARAMS);
	
public:
	void				SetDoc(CFX_WideString cwDoc);
	void				SetPrivateMember(CPDF_Bookmark& bookmark);
	void				SetParent(CPDF_Bookmark& bookmark);

private:
	CPDF_Bookmark		m_Bookmark;
	CPDF_Bookmark		m_ParentBookmark;
	CFX_WideString		m_doc;
};

class CJS_Bookmark : public CFXJS_Object
{
public:
	CJS_Bookmark(JSFXObject* pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_Bookmark(){};

public:
	DECLARE_JS_CLASS(CJS_Bookmark);

	JS_STATIC_PROP(children, Bookmark);
	JS_STATIC_PROP(color, Bookmark);
	JS_STATIC_PROP(doc, Bookmark);
	JS_STATIC_PROP(name, Bookmark);
	JS_STATIC_PROP(parent, Bookmark);
	JS_STATIC_PROP(style, Bookmark);	
};

#endif //_BOOKMARK_H_

