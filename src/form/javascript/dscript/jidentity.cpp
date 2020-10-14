
#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "identity.h"

/* -------------------------- identity -------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_identity)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_identity)
	JS_STATIC_PROP_ENTRY(corporation)
	JS_STATIC_PROP_ENTRY(email)
	JS_STATIC_PROP_ENTRY(loginName)
	JS_STATIC_PROP_ENTRY(name)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_identity)  
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_identity,jidentity,identity)

jidentity::jidentity(CFXJS_Object * pJsObject) : CFXJS_EmbedObj(pJsObject)                           
{
	m_sName = L"";
	m_sEmailAddress = L"";
	m_sCorporation = L"";
}

FX_BOOL jidentity::corporation(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFS_Library* pLib = CFS_Library::get();
		if (!pLib) return FALSE;
		FSActionHandler* pActionHandler = pLib->getActionHandler();
		if (!pActionHandler) return FALSE;
		FSIdentityProperties identityProperties = pActionHandler->getIdentityProperties();
		m_sCorporation = FSCRT_UTF8_ToWideString(identityProperties.corporation.getBuffer());
		vp << m_sCorporation;
	}
	else
	{
		vp >> m_sCorporation;
	}
	return TRUE;
}

FX_BOOL jidentity::email(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFS_Library* pLib = CFS_Library::get();
		if (!pLib) return FALSE;
		FSActionHandler* pActionHandler = pLib->getActionHandler();
		if (!pActionHandler) return FALSE;
		FSIdentityProperties identityProperties = pActionHandler->getIdentityProperties();
		m_sEmailAddress = FSCRT_UTF8_ToWideString(identityProperties.email.getBuffer());
		vp << m_sEmailAddress;
	}
	else
	{
		vp >> m_sEmailAddress;
	}
	return TRUE;
}

FX_BOOL jidentity::loginName(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFS_Library* pLib = CFS_Library::get();
		if (!pLib) return FALSE;
		FSActionHandler* pActionHandler = pLib->getActionHandler();
		if (!pActionHandler) return FALSE;
		FSIdentityProperties identityProperties = pActionHandler->getIdentityProperties();
		vp << FSCRT_UTF8_ToWideString(identityProperties.loginName.getBuffer());
	}
	return TRUE;
}

FX_BOOL jidentity::name(OBJ_PROP_PARAMS)
{
	if (vp.IsGetting())
	{
		CFS_Library* pLib = CFS_Library::get();
		if (!pLib) return FALSE;
		FSActionHandler* pActionHandler = pLib->getActionHandler();
		if (!pActionHandler) return FALSE;
		FSIdentityProperties identityProperties = pActionHandler->getIdentityProperties();
		m_sName = FSCRT_UTF8_ToWideString(identityProperties.name.getBuffer());
		vp << m_sName;
	}
	else
	{
		vp >> m_sName;
	}
	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

