#ifndef _COLOR_H_
#define _COLOR_H_

class jcolor : public CFXJS_EmbedObj
{
public:
	jcolor(CFXJS_Object* pJSObject);
	virtual ~jcolor(void);

	FX_BOOL black(OBJ_PROP_PARAMS);
	FX_BOOL blue(OBJ_PROP_PARAMS);
	FX_BOOL cyan(OBJ_PROP_PARAMS);	
	FX_BOOL dkGray(OBJ_PROP_PARAMS);
	FX_BOOL gray(OBJ_PROP_PARAMS);
	FX_BOOL green(OBJ_PROP_PARAMS);
	FX_BOOL ltGray(OBJ_PROP_PARAMS);
	FX_BOOL magenta(OBJ_PROP_PARAMS);
	FX_BOOL red(OBJ_PROP_PARAMS);	
	FX_BOOL transparent(OBJ_PROP_PARAMS);
	FX_BOOL white(OBJ_PROP_PARAMS);
	FX_BOOL yellow(OBJ_PROP_PARAMS);

	FX_BOOL convert(OBJ_METHOD_PARAMS);
	FX_BOOL equal(OBJ_METHOD_PARAMS);

public:  
	static void		ConvertPWLColorToArray(const CPWL_Color& color, CFXJS_Array& array);
	static void		ConvertArrayToPWLColor(CFXJS_Array& array, CPWL_Color& color);

private:
	CPWL_Color		m_crTransparent;
	CPWL_Color		m_crBlack;
	CPWL_Color		m_crWhite;
	CPWL_Color		m_crRed;
	CPWL_Color		m_crGreen;
	CPWL_Color		m_crBlue;
	CPWL_Color		m_crCyan;
	CPWL_Color		m_crMagenta;
	CPWL_Color		m_crYellow;
	CPWL_Color		m_crDKGray;
	CPWL_Color		m_crGray;
	CPWL_Color		m_crLTGray;
};

class CFXJS_Color : public CFXJS_Object
{
public:
	CFXJS_Color(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Color(void){};

	DECLARE_JS_CLASS(CFXJS_Color);

	JS_STATIC_PROP(black, jcolor);
	JS_STATIC_PROP(blue, jcolor);
	JS_STATIC_PROP(cyan, jcolor);	
	JS_STATIC_PROP(dkGray, jcolor);
	JS_STATIC_PROP(gray, jcolor);
	JS_STATIC_PROP(green, jcolor);
	JS_STATIC_PROP(ltGray, jcolor);
	JS_STATIC_PROP(magenta, jcolor);
	JS_STATIC_PROP(red, jcolor);	
	JS_STATIC_PROP(transparent, jcolor);
	JS_STATIC_PROP(white, jcolor);
	JS_STATIC_PROP(yellow, jcolor);

	JS_STATIC_METHOD(convert,jcolor);
	JS_STATIC_METHOD(equal,jcolor);

};

#endif //_COLOR_H_

