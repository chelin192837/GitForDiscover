#ifndef _EVENT_H_
#define _EVENT_H_

class jevent : public CFXJS_EmbedObj
{
public:
	jevent(CFXJS_Object * pJSObject);
	virtual ~jevent(void){}

public:
	FX_BOOL change(OBJ_PROP_PARAMS);
	FX_BOOL changeEx(OBJ_PROP_PARAMS);
	FX_BOOL commitKey(OBJ_PROP_PARAMS);
	FX_BOOL fieldFull(OBJ_PROP_PARAMS);
	FX_BOOL keyDown(OBJ_PROP_PARAMS);
	FX_BOOL modifier(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL rc(OBJ_PROP_PARAMS);
	FX_BOOL richChange(OBJ_PROP_PARAMS);
	FX_BOOL richChangeEx(OBJ_PROP_PARAMS);
	FX_BOOL richValue(OBJ_PROP_PARAMS);
	FX_BOOL selEnd(OBJ_PROP_PARAMS);
	FX_BOOL selStart(OBJ_PROP_PARAMS);
	FX_BOOL shift(OBJ_PROP_PARAMS);
	FX_BOOL source(OBJ_PROP_PARAMS);
	FX_BOOL target(OBJ_PROP_PARAMS);
	FX_BOOL targetName(OBJ_PROP_PARAMS);
	FX_BOOL type(OBJ_PROP_PARAMS);
	FX_BOOL value(OBJ_PROP_PARAMS);
	FX_BOOL willCommit(OBJ_PROP_PARAMS);

};

class CFXJS_Event : public CFXJS_Object
{
public:
	CFXJS_Event(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Event(void){};

	DECLARE_JS_CLASS(CFXJS_Event);

	JS_STATIC_PROP(change, jevent);
	JS_STATIC_PROP(changeEx, jevent);
	JS_STATIC_PROP(commitKey, jevent);
	JS_STATIC_PROP(fieldFull, jevent);
	JS_STATIC_PROP(keyDown, jevent);
	JS_STATIC_PROP(modifier, jevent);
	JS_STATIC_PROP(name, jevent);
	JS_STATIC_PROP(rc, jevent);
	JS_STATIC_PROP(richChange, jevent);
	JS_STATIC_PROP(richChangeEx, jevent);
	JS_STATIC_PROP(richValue, jevent);
	JS_STATIC_PROP(selEnd, jevent);
	JS_STATIC_PROP(selStart, jevent);
	JS_STATIC_PROP(shift, jevent);
	JS_STATIC_PROP(source, jevent);
	JS_STATIC_PROP(target, jevent);
	JS_STATIC_PROP(targetName, jevent);
	JS_STATIC_PROP(type, jevent);
	JS_STATIC_PROP(value, jevent);
	JS_STATIC_PROP(willCommit, jevent);
};

#endif //_EVENT_H_
