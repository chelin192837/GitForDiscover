#ifndef _REPORT_H_
#define _REPORT_H_

class JReport : public CFXJS_EmbedObj
{
public:
	JReport(CFXJS_Object * pJSObject);
	virtual ~JReport();

public:
	FX_BOOL save(OBJ_METHOD_PARAMS);
	FX_BOOL writeText(OBJ_METHOD_PARAMS);
};

class CFXJS_Report : public CFXJS_Object
{
public:
	CFXJS_Report(JSFXObject * pObject) : CFXJS_Object(pObject){};
	virtual ~CFXJS_Report(){};

public:
	DECLARE_JS_CLASS(CFXJS_Report);

	JS_STATIC_METHOD(save, JReport)
	JS_STATIC_METHOD(writeText, JReport);
};

#endif //_REPORT_H_

