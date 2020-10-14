#ifndef _APP_H_
#define _APP_H_

class CFXJS_Runtime;
class CFXJS_Timer;

/******************************************************************************/
/* timer object																  */
/******************************************************************************/

class JTimerObj : public CFXJS_EmbedObj
{
public:
	JTimerObj(CFXJS_Object* pJSObject);
	virtual ~JTimerObj();
	
public:
	void			SetTimer(CFXJS_Timer* pTimer);
	CFXJS_Timer*		GetTimer() const;
	
private:
	CFXJS_Timer*		m_pTimer;
};

class CFXJS_TimerObj : public CFXJS_Object
{
public:
	CFXJS_TimerObj(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CFXJS_TimerObj(){}
	
	DECLARE_JS_CLASS(CFXJS_TimerObj);
};

//*****************************************************************************
//* app																		  
//*****************************************************************************
class japp : public CFXJS_EmbedObj
{
public:
	japp(CFXJS_Object * pJSObject);
	virtual ~japp();

public:
	FX_BOOL						activeDocs(OBJ_PROP_PARAMS);
	FX_BOOL						calculate(OBJ_PROP_PARAMS);
	FX_BOOL						formsVersion(OBJ_PROP_PARAMS);
	FX_BOOL						fs(OBJ_PROP_PARAMS);
	FX_BOOL						fullscreen(OBJ_PROP_PARAMS);
	FX_BOOL						language(OBJ_PROP_PARAMS);
	FX_BOOL						media(OBJ_PROP_PARAMS);
	FX_BOOL						platform(OBJ_PROP_PARAMS);
	FX_BOOL						runtimeHighlight(OBJ_PROP_PARAMS);
	FX_BOOL						viewerType(OBJ_PROP_PARAMS);
	FX_BOOL						viewerVariation(OBJ_PROP_PARAMS);
	FX_BOOL						viewerVersion(OBJ_PROP_PARAMS);	


	FX_BOOL						alert(OBJ_METHOD_PARAMS);
	FX_BOOL						beep(OBJ_METHOD_PARAMS);
	FX_BOOL						browseForDoc(OBJ_METHOD_PARAMS);
	FX_BOOL						clearInterval(OBJ_METHOD_PARAMS);
	FX_BOOL						clearTimeOut(OBJ_METHOD_PARAMS);
	FX_BOOL						execDialog(OBJ_METHOD_PARAMS);
	FX_BOOL						execMenuItem(OBJ_METHOD_PARAMS);
	FX_BOOL						findComponent(OBJ_METHOD_PARAMS);
	FX_BOOL						goBack(OBJ_METHOD_PARAMS);
	FX_BOOL						goForward(OBJ_METHOD_PARAMS);
	FX_BOOL						launchURL(OBJ_METHOD_PARAMS);
	FX_BOOL						mailMsg(OBJ_METHOD_PARAMS);
	FX_BOOL						newFDF(OBJ_METHOD_PARAMS);
	FX_BOOL						newDoc(OBJ_METHOD_PARAMS);
	FX_BOOL						openDoc(OBJ_METHOD_PARAMS);
	FX_BOOL						openFDF(OBJ_METHOD_PARAMS);
	FX_BOOL						popUpMenuEx(OBJ_METHOD_PARAMS);
	FX_BOOL						popUpMenu(OBJ_METHOD_PARAMS);
	FX_BOOL						response(OBJ_METHOD_PARAMS);
	FX_BOOL						setInterval(OBJ_METHOD_PARAMS);
	FX_BOOL						setTimeOut(OBJ_METHOD_PARAMS);

private: 
	void						TimerProc(CFXJS_Timer* pTimer);
	void						RunJsScript(CFXJS_Runtime * pRuntime,const CFX_WideString & wsScript);

public:
	static CFX_WideString		SysPathToPDFPath(const CFX_WideString& sOldPath);
	static CFX_WideString		PDFPathToSysPath(const CFX_WideString& sOldPath);
	static CFX_WideString		RelativePathToSysPath(const CFX_WideString& sOldPath, const CFX_WideString& sFilePath);
	void						GetAppInfo(FX_INT32 type, CFX_WideString& info, CFXJS_Context* cc);

private:

	bool						m_bCalculate;	
	CFXJS_Runtime*				m_pRuntime;
	bool						m_bRuntimeHighLight;

	CFX_ArrayTemplate<CFXJS_Timer*>	m_aTimer;
	
public:

};

class CFXJS_App : public CFXJS_Object
{
public:
	CFXJS_App(JSFXObject * pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_App(void){};

	DECLARE_JS_CLASS(CFXJS_App);

	JS_STATIC_PROP(activeDocs, japp);
	JS_STATIC_PROP(calculate, japp);
	JS_STATIC_PROP(formsVersion, japp);
	JS_STATIC_PROP(fs, japp);
	JS_STATIC_PROP(fullscreen, japp);
	JS_STATIC_PROP(language, japp);
	JS_STATIC_PROP(media, japp);
	JS_STATIC_PROP(platform, japp);
	JS_STATIC_PROP(runtimeHighlight, japp);
	JS_STATIC_PROP(viewerType, japp);
	JS_STATIC_PROP(viewerVariation, japp);
	JS_STATIC_PROP(viewerVersion, japp);	

	JS_STATIC_METHOD(alert, japp);
	JS_STATIC_METHOD(beep, japp);
	JS_STATIC_METHOD(browseForDoc, japp);
	JS_STATIC_METHOD(clearInterval, japp);
	JS_STATIC_METHOD(clearTimeOut, japp);
	JS_STATIC_METHOD(execDialog, japp);
	JS_STATIC_METHOD(execMenuItem, japp);
	JS_STATIC_METHOD(findComponent, japp);
	JS_STATIC_METHOD(goBack, japp);
	JS_STATIC_METHOD(goForward, japp);
	JS_STATIC_METHOD(launchURL, japp);
	JS_STATIC_METHOD(mailMsg, japp);
	JS_STATIC_METHOD(newFDF, japp);
	JS_STATIC_METHOD(newDoc, japp);
	JS_STATIC_METHOD(openDoc, japp);
	JS_STATIC_METHOD(openFDF, japp);
	JS_STATIC_METHOD(popUpMenuEx, japp);
	JS_STATIC_METHOD(popUpMenu, japp);
	JS_STATIC_METHOD(response, japp);
	JS_STATIC_METHOD(setInterval, japp);
	JS_STATIC_METHOD(setTimeOut, japp);
};

#endif //_APP_H_
