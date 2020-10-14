#ifndef _FULLSCREEN_H_
#define _FULLSCREEN_H_

class FullScreen : public CFXJS_EmbedObj
{
public:
	FullScreen(CFXJS_Object* pJSObject);
	virtual ~FullScreen();	
	virtual void	TimerProc();

	void	SetApp(CFSCRT_LTAppHandler* pApp);

public:
	FX_BOOL backgroundColor(OBJ_PROP_PARAMS);
	FX_BOOL cursor(OBJ_PROP_PARAMS);
	FX_BOOL isFullScreen(OBJ_PROP_PARAMS);
	FX_BOOL timeDelay(OBJ_PROP_PARAMS);
	FX_BOOL useTimer(OBJ_PROP_PARAMS);
	FX_BOOL usePageTiming(OBJ_PROP_PARAMS);
	
public:
	int				m_iTimeDelay;
	int				m_iCursorStyle;
	FX_BOOL			m_fbUseTimer;
	FX_BOOL			m_fbUsePageTiming;
	FX_BOOL			m_isFullScreen;
	FX_BOOL			m_Timer;
//	FX_BOOL			m_IsMessageBoxOpen;

	CFSCRT_LTAppHandler*	m_pApp;
};


class CJS_FullScreen : public CFXJS_Object
{
public:
	CJS_FullScreen(JSFXObject* pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_FullScreen(){};

public:
	DECLARE_JS_CLASS(CJS_FullScreen);

	JS_STATIC_PROP(backgroundColor,FullScreen);
	JS_STATIC_PROP(cursor,FullScreen);
	JS_STATIC_PROP(isFullScreen,FullScreen);
	JS_STATIC_PROP(timeDelay,FullScreen);
	JS_STATIC_PROP(useTimer,FullScreen);
	JS_STATIC_PROP(usePageTiming,FullScreen);
};

#endif //_FULLSCREEN_H_

