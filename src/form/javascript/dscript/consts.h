#ifndef _CONSTS_H_
#define _CONSTS_H_

/* ------------------------------ border ------------------------------ */

class CFXJS_Border : public CFXJS_Object
{
public:
	CFXJS_Border(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Border(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ display ------------------------------ */

class CFXJS_Display : public CFXJS_Object
{
public:
	CFXJS_Display(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Display(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ font ------------------------------ */

class CFXJS_Font : public CFXJS_Object
{
public:
	CFXJS_Font(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Font(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ highlight ------------------------------ */

class CFXJS_Highlight : public CFXJS_Object
{
public:
	CFXJS_Highlight(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Highlight(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ position ------------------------------ */

class CFXJS_Position : public CFXJS_Object
{
public:
	CFXJS_Position(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Position(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ scaleHow ------------------------------ */

class CFXJS_ScaleHow : public CFXJS_Object
{
public:
	CFXJS_ScaleHow(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_ScaleHow(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ scaleWhen ------------------------------ */

class CFXJS_ScaleWhen : public CFXJS_Object
{
public:
	CFXJS_ScaleWhen(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_ScaleWhen(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ style ------------------------------ */

class CFXJS_Style : public CFXJS_Object
{
public:
	CFXJS_Style(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Style(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ zoomtype ------------------------------ */

class CFXJS_Zoomtype : public CFXJS_Object
{
public:
	CFXJS_Zoomtype(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Zoomtype(void){};

	DECLARE_JS_CLASS_CONST();
};

/* ------------------------------ CJS_GlobalConsts ------------------------------ */

class CFXJS_GlobalConsts : public CFXJS_Object
{
public:
	static int				Init(IDS_Runtime* pRuntime);
};

/* ------------------------------ CJS_GlobalArrays ------------------------------ */

class CFXJS_GlobalArrays : public CFXJS_Object
{
public:
	static int				Init(IDS_Runtime* pRuntime);
};

#endif //_CONSTS_H_

