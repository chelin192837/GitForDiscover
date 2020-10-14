#ifndef _MEDIA_H_
#define _MEDIA_H_

/* ------------------------------- MediaData ------------------------------- */

class MediaData : public CFXJS_EmbedObj
{
public:
	MediaData(CFXJS_Object* pJSObject);
	virtual ~MediaData();

public:
//	FX_BOOL isOpen(OBJ_PROP_PARAMS);

public:
	void SetURL(CFX_WideString wsURL);
	void SetMIMEType(CFX_WideString wsMIMEType);

private:
	CFX_WideString		m_wsURL;
	CFX_WideString		m_wsMIMEType;
};

class CJS_MediaData : public CFXJS_Object
{
public:
	CJS_MediaData(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_MediaData(){};

public:
	DECLARE_JS_CLASS(CJS_MediaData);

//	JS_STATIC_PROP(isOpen, MediaData);
};

/* ------------------------------- PlayerInfoList ------------------------------- */

class PlayerInfoList : public CFXJS_EmbedObj
{
public:
	PlayerInfoList(CFXJS_Object* pJSObject);
	virtual ~PlayerInfoList();

public:
//	FX_BOOL isOpen(OBJ_PROP_PARAMS);

public:
	void SetMIMEType(CFX_WideString wsMIMEType);

private:
	CFX_WideString		m_wsMIMEType;
};

class CJS_PlayerInfoList : public CFXJS_Object
{
public:
	CJS_PlayerInfoList(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_PlayerInfoList(){};

public:
	DECLARE_JS_CLASS(CJS_PlayerInfoList);
};

/* ------------------------------- MediaSettings ------------------------------- */

typedef struct _FLOATING_INFO_TAG
{
	_FLOATING_INFO_TAG()
	{
		align = 4; 
		over = 0;
		canResize = 0;
		hasClose = true;
		hasTitle = true;
		title = L"";
		isOffScreen = false;
		rect = FX_RECT();
		width = 0; 
		height = 0;
	}
	int align;
	int over;
	int canResize;
	FX_BOOL hasClose;
	FX_BOOL hasTitle;
	CFX_WideString title;
	FX_BOOL isOffScreen;
	FX_RECT rect;
	int width;
	int height;

}FLOATING_INFO, *LPFLOATING_INFO;

class MediaSettings : public CFXJS_EmbedObj
{
public:
	MediaSettings(CFXJS_Object* pJSObject);
	virtual ~MediaSettings();

public:
	FX_BOOL name(OBJ_PROP_PARAMS);

public:
	void			SetName(CFX_WideString wsName);
	void			SetWindowType(int iType);
	void			SetFloatingInfo(LPFLOATING_INFO pFloating);
	void			SetPlayerInfoList(PlayerInfoList* pPlayerInfo);
	void			SetMediaData(MediaData* pData);
	void			SetAutoPlay(FX_BOOL bIsAutoPlay);
	void			SetBaseURL(CFX_WideString wsURL);
	void			SetBGColor(FX_COLORREF bgcolor);
	void			SetBGOpacity(float fOpacity);
	void			SetDuration(int iDuration);
	void			SetLayout(int iLayout);
	void			SetMonitorType(int iMonitorType);
	void			SetPage(int iPage);
	void			SetPalindrome(FX_BOOL bPalindrome);
	void			SetRepeat(int iRepeat);
	void			SetIsShowUI(FX_BOOL bIsShowUI);
	void			SetIsVisible(FX_BOOL bIsVisible);
	void			SetVolume(int iVolume);

	CFX_WideString  GetName() const;
	int				GetWindowType() const;
	LPFLOATING_INFO GetFloatingInfo() const;
	PlayerInfoList*	GetPlayerInfoList() const;
	MediaData*		GetMediaData() const;
	FX_BOOL			IsAutoPlay() const;
	CFX_WideString	GetBaseURL() const;
	FX_COLORREF		GetBGColor() const;
	float			GetBGOpacity() const;
	int				GetDuration() const;
	int				GetMonitorType() const;
	int				GetPage() const;
	FX_BOOL			GetPalindrome() const;
	int				GetRepeat() const;
	FX_BOOL			IsShowUI() const;
	FX_BOOL			IsVisible() const;
	int				GetVolume() const;
	int				GetLayout() const;

private:
	CFX_WideString	m_wsName;
	int				m_iWindowType;
	LPFLOATING_INFO m_pFloatingInfo;
	PlayerInfoList* m_pPlayerInfoList;
	MediaData*		m_pMediaData;
	FX_BOOL			m_bAutoPlay;
	CFX_WideString	m_wsBaseURL;
	FX_COLORREF		m_cBGColor;
	float			m_fBGOpacity;
	int				m_iDuration;
	int				m_iLayout;
	int				m_iMonitorType;
	int				m_iPage;
	FX_BOOL			m_bPalindrome;
	int				m_iRate;
	int				m_iRepeat;
	FX_BOOL			m_bShowUI;
	FX_BOOL			m_bVisible;
	int				m_iVolume;
};

class CJS_MediaSettings : public CFXJS_Object
{
public:
	CJS_MediaSettings(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_MediaSettings(){};

public:
	DECLARE_JS_CLASS(CJS_MediaSettings);

	JS_STATIC_PROP(name, MediaSettings);
};

/* ------------------------------- Rendition ------------------------------- */

class Rendition : public CFXJS_EmbedObj
{
public:
	Rendition(CFXJS_Object* pJSObject);
	virtual ~Rendition();

public:
	FX_BOOL uiName(OBJ_PROP_PARAMS);

public:
	void				SetRendition(CPDF_Dictionary* pRenditionDict);
	CFX_ByteString		GetMediaContentType() const;
	FX_COLORREF			GetBGColor() const;
	CPDF_FileSpec		GetMediaClipFile() const;
	CFX_ByteString		GetMediaClipContentType() const;
	CPDF_Dictionary*	GetRendition() const;

private:
	CPDF_Dictionary*	m_pRenditionDict;
};

class CJS_Rendition : public CFXJS_Object
{
public:
	CJS_Rendition(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_Rendition(){};

public:
	DECLARE_JS_CLASS(CJS_Rendition);

	JS_STATIC_PROP(uiName, Rendition);
};

/* ------------------------------- ScreenAnnot ------------------------------- */

class ScreenAnnot : public CFXJS_EmbedObj
{
public:
	ScreenAnnot(CFXJS_Object* pJSObject);
	virtual ~ScreenAnnot();

public:
	FX_BOOL page(OBJ_PROP_PARAMS);

public:
	void				SetAnnot(CFSCRT_LTPDFAnnot* pAnnot);
	CFSCRT_LTPDFAnnot*		GetAnnot() const {return m_pScreenAnnot;}

private:
	CFSCRT_LTPDFAnnot*		m_pScreenAnnot;
};

class CJS_ScreenAnnot : public CFXJS_Object
{
public:
	CJS_ScreenAnnot(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_ScreenAnnot(){};

public:
	DECLARE_JS_CLASS(CJS_ScreenAnnot);

	JS_STATIC_PROP(page, ScreenAnnot);
};


/* ------------------------------- MediaPlayer ------------------------------- */

class MediaPlayer : public CFXJS_EmbedObj
{
public:
	MediaPlayer(CFXJS_Object* pJSObject);
	virtual ~MediaPlayer();

public:
	FX_BOOL isOpen(OBJ_PROP_PARAMS);
	FX_BOOL settings(OBJ_PROP_PARAMS);
	FX_BOOL visible(OBJ_PROP_PARAMS);

	FX_BOOL close(OBJ_METHOD_PARAMS);
	FX_BOOL stop(OBJ_METHOD_PARAMS);

public:
	//void					SetPlayer(IFX_MediaPlayer* pPlayer);
	CFX_WideString			GetPlayerName() const;
	FX_BOOL					IsOpen() const;
	FX_BOOL					IsVisible() const;

private:
	//IFX_MediaPlayer*		m_pMediaPlayer;
};

class CJS_MediaPlayer : public CFXJS_Object
{
public:
	CJS_MediaPlayer(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_MediaPlayer(){};

public:
	DECLARE_JS_CLASS(CJS_MediaPlayer);

	JS_STATIC_PROP(isOpen, MediaPlayer);
	JS_STATIC_PROP(settings, MediaPlayer);
	JS_STATIC_PROP(visible, MediaPlayer);

	JS_STATIC_METHOD(close, MediaPlayer);
	JS_STATIC_METHOD(stop, MediaPlayer);
};

/* ------------------------------- MediaPlayer ------------------------------- */

class jdoc_media : public CFXJS_EmbedObj
{
public:
	jdoc_media(CFXJS_Object* pJSObject);
	virtual ~jdoc_media();

public:
//	FX_BOOL windowType(OBJ_PROP_PARAMS);

	FX_BOOL getAnnot(OBJ_METHOD_PARAMS);
	FX_BOOL getOpenPlayers(OBJ_METHOD_PARAMS);
	FX_BOOL getRendition(OBJ_METHOD_PARAMS);

public:
	void				SetDocument(FSPDFDoc* pDoc);
//	CPDF_Rendition		GetRenditionByName(CFX_WideString wsName);
	void				ParseParams(JSObject* pObj, int& iPage, CFX_WideString& wsAnnotName, CFX_WideString& wsAnnotTitle);
	void				GetAnnot(CJS_ScreenAnnot* pScreen, int iPage, CFX_WideString wsAnnotName, CFX_WideString wsAnnotTitle) const;
	
private:
	FSPDFDoc*		m_pDocument;
	//IFX_MediaPlayerArray	m_aOpenPlayers;
	//IFX_AppMedia*			m_pAppMedia;
	//IFX_MediaPlayer*		m_pMediaPlayer;
	//IFX_PlayerMgr*			m_pPlayerMgr;
};


class CFXJS_doc_media : public CFXJS_Object
{
public:
	CFXJS_doc_media(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_doc_media(){};

	DECLARE_JS_CLASS(CFXJS_doc_media);
	
//	JS_STATIC_PROP(windowType, jdoc_media);

	JS_STATIC_METHOD(getAnnot, jdoc_media);
	JS_STATIC_METHOD(getOpenPlayers, jdoc_media);
	JS_STATIC_METHOD(getRendition, jdoc_media);
};

/* ------------------------------- app_media ------------------------------- */

class JDocument;

struct PARAMS_OF_PLAYER
{
	PARAMS_OF_PLAYER()
	{
		m_pRendition = NULL;
		m_pAnnot = NULL;
		m_pSettings = new MediaSettings(NULL);
		wsURL = L"";
		wsMIMETYPE = L"";
		m_pDocument = NULL;
	}

	virtual ~PARAMS_OF_PLAYER()
	{
		if (m_pSettings)
		{
			delete m_pSettings;
			m_pSettings = NULL;
		}
	}

	Rendition*		m_pRendition;
	ScreenAnnot*	m_pAnnot;
	MediaSettings*	m_pSettings;
	CFX_WideString	wsURL;
	CFX_WideString	wsMIMETYPE;
	JDocument*		m_pDocument;
};

class app_media : public CFXJS_EmbedObj
{
public:
	app_media(CFXJS_Object* pJSObject);
	virtual ~app_media();

public:
	FX_BOOL closeReason(OBJ_PROP_PARAMS);
	FX_BOOL windowType(OBJ_PROP_PARAMS);

	FX_BOOL getPlayers(OBJ_METHOD_PARAMS);
	FX_BOOL getURLData(OBJ_METHOD_PARAMS);
	FX_BOOL openPlayer(OBJ_METHOD_PARAMS);

private:
	void			ParsePlayerArg(JSObject* pObj, PARAMS_OF_PLAYER& player_params);
	//void			SetPlayerArgs(FX_LPPLAYER_ARGS pPlayerArgs, const PARAMS_OF_PLAYER& player_params);

public:
	void			SetDocument(FSPDFDoc* pDoc);

private:
	FSPDFDoc*	m_pDocument;
	//IFX_PlayerMgr*		m_pPlayerMgr;
	//IFX_AppMedia*		m_pAppMedia;
	//IFX_MediaPlayer*	m_pMediaPlayer;
};


class CJS_app_media : public CFXJS_Object
{
public:
	CJS_app_media(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CJS_app_media(){};

	DECLARE_JS_CLASS(CJS_app_media);
	
	JS_STATIC_PROP(closeReason, app_media);
	JS_STATIC_PROP(windowType, app_media);

	JS_STATIC_METHOD(getPlayers, app_media);
	JS_STATIC_METHOD(getURLData, app_media);
	JS_STATIC_METHOD(openPlayer, app_media);
};

#endif //_MEDIA_H_

