#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "event.h"
#include "field.h"

/* -------------------------- event -------------------------- */

BEGIN_JS_STATIC_CONST(CFXJS_Event)
END_JS_STATIC_CONST()

BEGIN_JS_STATIC_PROP(CFXJS_Event)
	JS_STATIC_PROP_ENTRY(change)
	JS_STATIC_PROP_ENTRY(changeEx)
	JS_STATIC_PROP_ENTRY(commitKey)
	JS_STATIC_PROP_ENTRY(fieldFull)
	JS_STATIC_PROP_ENTRY(keyDown)
	JS_STATIC_PROP_ENTRY(modifier)
	JS_STATIC_PROP_ENTRY(name)
	JS_STATIC_PROP_ENTRY(rc)
	JS_STATIC_PROP_ENTRY(richChange)
	JS_STATIC_PROP_ENTRY(richChangeEx)
	JS_STATIC_PROP_ENTRY(richValue)
	JS_STATIC_PROP_ENTRY(selEnd)
	JS_STATIC_PROP_ENTRY(selStart)
	JS_STATIC_PROP_ENTRY(shift)
	JS_STATIC_PROP_ENTRY(source)
	JS_STATIC_PROP_ENTRY(target)
	JS_STATIC_PROP_ENTRY(targetName)
	JS_STATIC_PROP_ENTRY(type)
	JS_STATIC_PROP_ENTRY(value)
	JS_STATIC_PROP_ENTRY(willCommit)
END_JS_STATIC_PROP()

BEGIN_JS_STATIC_METHOD(CFXJS_Event)  
END_JS_STATIC_METHOD()

IMPLEMENT_JS_CLASS(CFXJS_Event,jevent,event)

jevent::jevent(CFXJS_Object * pJsObject) : CFXJS_EmbedObj(pJsObject)                           
{
}

/*event::~event(void)
{
}*/

FX_BOOL jevent::change(OBJ_PROP_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	CFX_WideString &wChange = pEvent->Change();
	if (vp.IsSetting())
	{
		if (vp.GetType() == VT_string)
			vp >> wChange;
	}
	else
	{
		vp << wChange;
	}
	return TRUE;
}

FX_BOOL jevent::changeEx(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp << pEvent->ChangeEx();
	return TRUE;	
}

FX_BOOL jevent::commitKey(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp << pEvent->CommitKey();
	return TRUE;
}

FX_BOOL jevent::fieldFull(OBJ_PROP_PARAMS)
{	
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (!vp.IsGetting() && FXSYS_wcscmp((const wchar_t*)pEvent->Name(),L"Keystroke") != 0)
		return FALSE;

	if (pEvent->FieldFull())
		vp << TRUE;
	else
		vp << FALSE;
	return TRUE;
}

FX_BOOL jevent::keyDown(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (pEvent->KeyDown())
		vp << TRUE;
	else
		vp << FALSE;
	return TRUE;
}

FX_BOOL jevent::modifier(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (pEvent->Modifier())
		vp << TRUE;
	else
		vp << FALSE;
	return TRUE;
}

FX_BOOL jevent::name(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp << pEvent->Name();
	return TRUE;
}

FX_BOOL jevent::rc(OBJ_PROP_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

    FX_BOOL &bRc = pEvent->Rc();
	if (vp.IsSetting())
	{
		vp>>bRc;
	}
	else
	{
		vp<<bRc;
	}
	return TRUE;
}

FX_BOOL jevent::richChange(OBJ_PROP_PARAMS)
{
	return TRUE;
	if (vp.IsSetting())
	{
	}
	else
	{
		;
	}
	return TRUE;
}

FX_BOOL jevent::richChangeEx(OBJ_PROP_PARAMS)
{
	return TRUE;
	if (vp.IsSetting())
	{
	}
	else
	{
		;
	}
	return TRUE;
}


FX_BOOL jevent::richValue(OBJ_PROP_PARAMS)
{
	return TRUE;
	if (vp.IsSetting())
	{
	}
	else
	{
		;
	}
	return TRUE;
}

FX_BOOL jevent::selEnd(OBJ_PROP_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (FXSYS_wcscmp((const wchar_t*)pEvent->Name(),L"Keystroke") != 0)
	{
		return TRUE;
	}

	int &iSelEnd = pEvent->SelEnd();
	if (vp.IsSetting())
	{
		vp >> iSelEnd;
	}
	else
	{
		vp << iSelEnd;
	}
	return TRUE;
}

FX_BOOL jevent::selStart(OBJ_PROP_PARAMS)
{	
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (FXSYS_wcscmp((const wchar_t*)pEvent->Name(),L"Keystroke") != 0)
	{
		return TRUE;
	}
	int &iSelStart = pEvent->SelStart();
	if (vp.IsSetting())
	{
		vp >> iSelStart;
	}
	else
	{
		vp << iSelStart;
	}
	return TRUE;
}

FX_BOOL jevent::shift(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (pEvent->Shift())
		vp << TRUE;
	else
		vp << FALSE;
	return TRUE;
}

FX_BOOL jevent::source(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;	

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp << pEvent->Source()->GetJSObject();
	return TRUE;
}

FX_BOOL jevent::target(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp<<pEvent->Target_Field()->GetJSObject();
	return TRUE;
}

FX_BOOL jevent::targetName(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp << pEvent->TargetName();
	return TRUE;
}

FX_BOOL jevent::type(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	vp << pEvent->Type();
	return TRUE;
}

FX_BOOL jevent::value(OBJ_PROP_PARAMS)
{
	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (FXSYS_wcscmp((const wchar_t*)pEvent->Type(),L"Field") != 0)
		return FALSE;
	if(!pEvent->m_pValue)
		return FALSE;
	CFX_WideString & val = pEvent->Value();
	if (vp.IsSetting())
	{
		val = vp;
	}
	else
	{
		vp << val;
	}
	return TRUE;
}

FX_BOOL jevent::willCommit(OBJ_PROP_PARAMS)
{
	if (!vp.IsGetting())return FALSE;

	CFXJS_Context* pContext = (CFXJS_Context*)cc;
	ASSERT(pContext != NULL);
	CFXJS_EventHandler* pEvent = pContext->GetEventHandler();
	ASSERT(pEvent != NULL);

	if (pEvent->WillCommit())
		vp << TRUE;
	else
		vp << FALSE;
	return TRUE;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)