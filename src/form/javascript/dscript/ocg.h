#ifndef _OCG_H_
#define _OCG_H_

class CJS_OCGNotify;
class CFXJS_Runtime;

class OCG : public CFXJS_EmbedObj
{
public:
	OCG(CFXJS_Object * pJSObject);
	virtual ~OCG();

public:
	FX_BOOL constants(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL state(OBJ_PROP_PARAMS);

	FX_BOOL getIntent(OBJ_METHOD_PARAMS);
	FX_BOOL setAction(OBJ_METHOD_PARAMS);

public:
	void             SetOCGInfo(FSPDFDoc* pDocument, FSPDFPage* pDocView, CPDF_Dictionary* pDict);
	static  void     RunJavaScript(CFXJS_Runtime* pRuntime, CFX_WideString swJavaScript);
	CFXJS_Runtime*     GetJSRuntime(){return m_pRuntime;}
	CFX_WideString   GetJSScript(){return m_swJavaScript;}

private:
	FSPDFDoc*			m_pDocument;
	FSPDFPage*			m_pDocView;
	CPDF_Dictionary*			m_pOCGDict;
	CFX_WideString				m_swJavaScript;
	CFXJS_Runtime*				m_pRuntime;
	static IPDF_OCNotify*		s_pNotify;
};

class CJS_OCG : public CFXJS_Object
{
public:
	CJS_OCG(JSFXObject * pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_OCG(){};

public:
	DECLARE_JS_CLASS(CJS_OCG);
	JS_STATIC_PROP(constants, OCG);
	JS_STATIC_PROP(name, OCG);
	JS_STATIC_PROP(state, OCG);

	JS_STATIC_METHOD(getIntent, OCG);
	JS_STATIC_METHOD(setAction, OCG);
};

class CJS_OCGNotify : public IPDF_OCNotify
{
public:
	CJS_OCGNotify(OCG* pOcg)
	{
		if(pOcg)
			m_pOcg = pOcg;
	}
	~CJS_OCGNotify(){}

	virtual void	OnOCGStateChanged(CPDF_Document *pDoc, CPDF_OCGroup ocg, FX_BOOL bVisible);
private:
	OCG* m_pOcg;
};


#endif//_OCG_H_

