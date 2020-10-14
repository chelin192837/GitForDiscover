
#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "media.h"

/* -------------------------- identity -------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_doc_media)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_doc_media)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_doc_media)
JS_STATIC_METHOD_ENTRY(getAnnot, 1)
JS_STATIC_METHOD_ENTRY(getOpenPlayers, 0)
JS_STATIC_METHOD_ENTRY(getRendition, 1)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_doc_media,jdoc_media,doc_media)

jdoc_media::jdoc_media(CFXJS_Object * pJsObject) : CFXJS_EmbedObj(pJsObject)                           
{
}

jdoc_media::~jdoc_media()
{
}

FX_BOOL jdoc_media::getAnnot(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

FX_BOOL jdoc_media::getOpenPlayers(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

FX_BOOL jdoc_media::getRendition(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

