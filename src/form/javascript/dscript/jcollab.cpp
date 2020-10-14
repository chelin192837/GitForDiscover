
#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "collab.h"

/* -------------------------- identity -------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Collab)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Collab)
	JS_STATIC_PROP_ENTRY(user)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Collab)  
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Collab,jcollab,Collab)

jcollab::jcollab(CFXJS_Object * pJsObject) : CFXJS_EmbedObj(pJsObject)                           
{
}

jcollab::~jcollab()
{
}

FX_BOOL jcollab::user(OBJ_PROP_PARAMS)
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

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

