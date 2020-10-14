#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "icon.h"

/* ---------------------- Icon ---------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Icon)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Icon)
	JS_STATIC_PROP_ENTRY(name)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Icon) 
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Icon,JIcon,Icon)

JIcon::JIcon(CFXJS_Object* pJSObject) : CFXJS_EmbedObj(pJSObject),
	m_pIconStream(NULL),
	m_swIconName(L"")
{
}

JIcon::~JIcon()
{

}

void JIcon::SetStream(CPDF_Stream* pIconStream)
{
	if(pIconStream)
		m_pIconStream = pIconStream;
}

CPDF_Stream* JIcon::GetStream()
{
	return m_pIconStream;
}

void JIcon::SetIconName(CFX_WideString name)
{
	m_swIconName = name;
}

CFX_WideString JIcon::GetIconName()
{
	return m_swIconName;
}

FX_BOOL JIcon::name(OBJ_PROP_PARAMS)
{
	if(!vp.IsGetting())return FALSE;

	vp << m_swIconName;
	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)