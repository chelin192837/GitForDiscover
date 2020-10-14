#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "color.h"

/* -------------------------- color -------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Color)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Color)
	JS_STATIC_PROP_ENTRY(black)
	JS_STATIC_PROP_ENTRY(blue)
	JS_STATIC_PROP_ENTRY(cyan)	
	JS_STATIC_PROP_ENTRY(dkGray)
	JS_STATIC_PROP_ENTRY(gray)
	JS_STATIC_PROP_ENTRY(green)
	JS_STATIC_PROP_ENTRY(ltGray)
	JS_STATIC_PROP_ENTRY(magenta)
	JS_STATIC_PROP_ENTRY(red)	
	JS_STATIC_PROP_ENTRY(transparent)
	JS_STATIC_PROP_ENTRY(white)
	JS_STATIC_PROP_ENTRY(yellow)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Color)
	JS_STATIC_METHOD_ENTRY(convert, 2)
	JS_STATIC_METHOD_ENTRY(equal, 2)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Color,jcolor,color)

jcolor::jcolor(CFXJS_Object* pJSObject): CFXJS_EmbedObj(pJSObject)
{
	m_crTransparent = CPWL_Color(COLORTYPE_TRANSPARENT);
	m_crBlack = CPWL_Color(COLORTYPE_GRAY, 0);
	m_crWhite = CPWL_Color(COLORTYPE_GRAY, 1);
	m_crRed = CPWL_Color(COLORTYPE_RGB, 1, 0 ,0);
	m_crGreen = CPWL_Color(COLORTYPE_RGB, 0, 1 ,0);
	m_crBlue = CPWL_Color(COLORTYPE_RGB, 0, 0 ,1);
	m_crCyan = CPWL_Color(COLORTYPE_CMYK, 1, 0 ,0, 0);
	m_crMagenta = CPWL_Color(COLORTYPE_CMYK, 0, 1 ,0, 0);
	m_crYellow = CPWL_Color(COLORTYPE_CMYK, 0, 0 ,1, 0);
	m_crDKGray = CPWL_Color(COLORTYPE_GRAY, 0.25);
	m_crGray = CPWL_Color(COLORTYPE_GRAY, 0.5);
	m_crLTGray = CPWL_Color(COLORTYPE_GRAY, 0.75);
}

jcolor::~jcolor(void)
{
}

void jcolor::ConvertPWLColorToArray(const CPWL_Color& color, CFXJS_Array& array)
{
	switch (color.nColorType)
	{
	case COLORTYPE_TRANSPARENT:
		array.SetElement(0, "T");
		break;
	case COLORTYPE_GRAY:
		array.SetElement(0, "G");
		array.SetElement(1, color.fColor1);
		break;
	case COLORTYPE_RGB:
		array.SetElement(0, "RGB");
		array.SetElement(1, color.fColor1);
		array.SetElement(2, color.fColor2);
		array.SetElement(3, color.fColor3);
		break;
	case COLORTYPE_CMYK:
		array.SetElement(0, "CMYK");
		array.SetElement(1, color.fColor1);
		array.SetElement(2, color.fColor2);
		array.SetElement(3, color.fColor3);
		array.SetElement(4, color.fColor4);
		break;
	}
}

void jcolor::ConvertArrayToPWLColor(CFXJS_Array& array, CPWL_Color& color)
{
	int nArrayLen = array.GetLength();
	if (nArrayLen < 1) return;

	CFXJS_Value value;
	CFX_ByteString sSpace;
	array.GetElement(0, value);
	sSpace = value;

	double d1 = 0;
	double d2 = 0;
	double d3 = 0;
	double d4 = 0;

	if (nArrayLen > 1)
	{
		array.GetElement(1, value);
		d1 = value;
	}

	if (nArrayLen > 2)
	{
		array.GetElement(2, value);
		d2 = value;
	}

	if (nArrayLen > 3)
	{
		array.GetElement(3, value);
		d3 = value;
	}

	if (nArrayLen > 4)
	{
		array.GetElement(4, value);
		d4 = value;
	}

	if (sSpace == "T")
	{
		color = CPWL_Color(COLORTYPE_TRANSPARENT);
	}
	else if (sSpace == "G")
	{
		color = CPWL_Color(COLORTYPE_GRAY, (FX_FLOAT)d1);
	}
	else if (sSpace == "RGB")
	{
		color = CPWL_Color(COLORTYPE_RGB, (FX_FLOAT)d1, (FX_FLOAT)d2, (FX_FLOAT)d3);
	}
	else if (sSpace == "CMYK")
	{
		color = CPWL_Color(COLORTYPE_CMYK, (FX_FLOAT)d1, (FX_FLOAT)d2, (FX_FLOAT)d3, (FX_FLOAT)d4);
	}
}

#define JS_IMPLEMENT_COLORPROP(prop, var)\
FX_BOOL jcolor::prop(OBJ_PROP_PARAMS)\
{\
	if (vp.IsGetting())\
	{\
		CFXJS_Array array;\
		ConvertPWLColorToArray(var, array);\
		vp << array;\
	}\
	else\
	{\
		CFXJS_Array array;\
		if (!vp.ConvertToArray(array)) return FALSE;\
		ConvertArrayToPWLColor(array, var);\
	}\
	return TRUE;\
}

JS_IMPLEMENT_COLORPROP(transparent, m_crTransparent)
JS_IMPLEMENT_COLORPROP(black, m_crBlack)
JS_IMPLEMENT_COLORPROP(white, m_crWhite)
JS_IMPLEMENT_COLORPROP(red, m_crRed)
JS_IMPLEMENT_COLORPROP(green, m_crGreen)
JS_IMPLEMENT_COLORPROP(blue, m_crBlue)
JS_IMPLEMENT_COLORPROP(cyan, m_crCyan)
JS_IMPLEMENT_COLORPROP(magenta, m_crMagenta)
JS_IMPLEMENT_COLORPROP(yellow, m_crYellow)
JS_IMPLEMENT_COLORPROP(dkGray, m_crDKGray)
JS_IMPLEMENT_COLORPROP(gray, m_crGray)
JS_IMPLEMENT_COLORPROP(ltGray, m_crLTGray)

FX_BOOL jcolor::convert(OBJ_METHOD_PARAMS)
{
	int iSize = params.size();
	if (iSize < 2) return FALSE;
	CFXJS_Array aSource;
	if (!params[0].ConvertToArray(aSource)) return FALSE;

	CPWL_Color crSource;
	ConvertArrayToPWLColor(aSource, crSource);

	CFX_ByteString sDestSpace = params[1];

	int nColorType = COLORTYPE_TRANSPARENT;
	
	if (sDestSpace == "T")
	{
		nColorType = COLORTYPE_TRANSPARENT;
	}
	else if (sDestSpace == "G")
	{
		nColorType = COLORTYPE_GRAY;
	}
	else if (sDestSpace == "RGB")
	{
		nColorType = COLORTYPE_RGB;
	}
	else if (sDestSpace == "CMYK")
	{
		nColorType = COLORTYPE_CMYK;
	}

	CFXJS_Array aDest;
	CPWL_Color crDest = crSource;
	crDest.ConvertColorType(nColorType);
	ConvertPWLColorToArray(crDest, aDest);
	vRet = aDest;

	return TRUE;
}

FX_BOOL jcolor::equal(OBJ_METHOD_PARAMS)
{
	if (params.size() < 2) return FALSE;

	CFXJS_Array array1, array2;

	if (!params[0].ConvertToArray(array1)) return FALSE;
	if (!params[1].ConvertToArray(array2)) return FALSE;

	CPWL_Color color1;
	CPWL_Color color2;

	ConvertArrayToPWLColor(array1, color1);
	ConvertArrayToPWLColor(array2, color2);

	color1.ConvertColorType(color2.nColorType);

	vRet = color1 == color2;
	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)