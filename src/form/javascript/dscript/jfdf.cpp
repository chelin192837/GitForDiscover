#if 0
#include "fdf.h"

/* ---------------------- FDF ---------------------- */

BEGIN_JS_STATIC_CONST(CJS_FDF)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CJS_FDF)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CJS_FDF)
	JS_STATIC_METHOD_ENTRY(addContact, 1)
	JS_STATIC_METHOD_ENTRY(addRequest, 3)
	JS_STATIC_METHOD_ENTRY(close, 0)
	JS_STATIC_METHOD_ENTRY(mail, 6)
	JS_STATIC_METHOD_ENTRY(save, 1)
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CJS_FDF,FDF)

FDF::FDF(CJS_Object * pJSObject): CJS_EmbedObj(pJSObject)
{
}

FDF::~FDF()
{
}
//js proto:void addContact(UserEntity oUserEntity) throw;
FX_BOOL FDF::addContact(OBJ_METHOD_PARAMS)
{
	return TRUE;
}
//js proto: void addRequest(string cType,string cReturnAddress,string cName) throw;
FX_BOOL FDF::addRequest(OBJ_METHOD_PARAMS)
{
	return TRUE;
}

//js protocol:void close() throw;
FX_BOOL FDF::close(OBJ_METHOD_PARAMS)
{
	return TRUE;
}
//js protocol: void mail(bool bUI,string cTo,string cCC,string cBcc,string cSubject,string cMsg) throw;
FX_BOOL FDF::mail(OBJ_METHOD_PARAMS)
{
	return TRUE;
}
//js protocol:void save(string cDIPath) throw
FX_BOOL FDF::save(OBJ_METHOD_PARAMS)
{
	return TRUE;
}
#endif