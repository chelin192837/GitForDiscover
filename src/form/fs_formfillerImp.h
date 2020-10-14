#ifndef _FSPDF_LTFORMFILLER_H_
#define _FSPDF_LTFORMFILLER_H_

class FSPDFDoc;
class FSPDFPage;
class FSForm;
class IFSPDF_AnnotHandler;
class CFSCRT_STPDFActionHandler;
class CFXJS_RuntimeFactory;
class IJS_Runtime;
class CFSCRT_JS_ActionHandler;
class FSFormFillerImp;

extern CFXJS_RuntimeFactory* FSCRT_GetJSRuntimeFactory();

class CFSCRT_LTWindowslessHanler
{
private:
	FSFormFillerImp*				m_pOwner;

#ifdef _FS_PDFIUM_BUILD_
	FPDF_FORMFILLINFO* m_pFFI;
#endif

public:
	CFSCRT_LTWindowslessHanler() : m_pOwner(NULL)
	{
		//FXSYS_memset(&m_formInterWindowless, 0, sizeof(FSPDF_FORMINTERACTION_WINDOWLESS));
		m_pOwner = NULL;
	};
	CFSCRT_LTWindowslessHanler(FSFormFillerImp*);


	FS_RESULT	ReleaseHandler();

	FS_RESULT   OutputSelectedRect(FS_INT32 pageIndex, FSRectF* pdfRect);

	FS_RESULT   SetCursor(FS_INT32 nCursorType);

	FS_RESULT   SetTimer(FS_INT32 uElapse, FS_CALLBACK_TIMER lpTimerFunc, FS_INT32* timeID);

	FS_RESULT   KillTimer(FS_INT32 timerID);

	FS_RESULT   GotFocusOnControl(FSFormControl* control, const char* fieldValue);

	FS_RESULT   LostFocusOnControl(FSFormControl* control, const char* fieldValue);
};
/*
class CFSCRT_LTFormFiller 
{
	friend class CFSCRT_LTWindowslessHanler;
public:
	CFSCRT_LTFormFiller(FSForm* form):m_pForm(form)
		,m_show(0)
#ifdef _FS_PDFIUM_BUILD_
		,m_nPDFiumAlpha(0)
#endif
	{

		//FXSYS_memset(&m_formInterWindowless, 0, sizeof(FSPDF_FORMINTERACTION_WINDOWLESS));
		m_pWidgetHandler = NULL;
		m_pSTActionHandler = NULL;
		m_typeToColor = NULL;
		m_pCaptureAnnot = m_pFocusAnnot = m_pHoverAnnot = NULL;
		m_bUserWindowless = FALSE;
	}
	//FSFormFillerImp::FSFormFillerImp(CFSCRT_LTPDFDocument* pDocument);
	~CFSCRT_LTFormFiller();

	//void SetWindowlessExtent(FSPDF_FORMINTERACTION_WINDOWLESS* windowlessExtent) 
	//{
	//	if(windowlessExtent) m_formInterWindowless.SetHandle(windowlessExtent);
	//	m_bUserWindowless = TRUE;
	//}
#ifdef _FS_PDFIUM_BUILD_
	void SetWindowlessExtent(FPDF_FORMFILLINFO* FFI) 
	{
		if(FFI) m_formInterWindowless.SetHandle(FFI);
		
		m_bUserWindowless = TRUE;
	}
#endif

	void UnsetWindowlessExtent() 
	{
		m_formInterWindowless.ReleaseHandler();
		m_bUserWindowless = FALSE;
	}

	FX_BOOL useWindowless() {return m_bUserWindowless;}

	FS_RESULT TriggerWindowlessEvent(FSPDFPage* page, FSMatrix* device2Page, FS_DWORD eventType, FS_LPVOID eventData);

	FS_RESULT InitiateJavaScript(FSPDFDoc* doc);
	//Doc js actions.
	FS_RESULT DoDocJavaScriptAction(FSPDFDoc* doc, FS_INT32 trigger);
	//Page js actions.
	FS_RESULT DoPageJavaScriptAction(FSPDFPage* page, FS_INT32 trigger);
	//Field js actions.
	FS_RESULT ValidateValue(const char* fieldName, const char* value, FS_BOOL* bValidate);
	FS_RESULT KeyStroke(const char* fieldName, FSPDF_JAVASCRIPTEVENT* jsEvent);

	FS_RESULT Caculate(const char* fieldName);
	FS_RESULT Format(const char* fieldName, FS_BOOL* formated);
	//FS_RESULT DoFieldJavaScriptAction(const char* fieldName, FS_INT32 trigger);
	//Control js actions.
	FS_RESULT DoControlJavaScriptAction(FSFormControl* control, FS_INT32 trigger);

	FS_RESULT		Initialize();
	void			Release();
	FS_RESULT		SetHighlightColor(FS_INT32 fieldType, FS_ARGB color);
	FS_RESULT		GetHighlightColor(FS_INT32 fieldType, FS_ARGB* color);

#ifdef _FS_PDFIUM_BUILD_
	void			SetAlpha(FX_UINT8 alpha){m_nPDFiumAlpha = alpha;};
	FX_UINT8		GetAlpha(){return m_nPDFiumAlpha;};
#endif

	FS_RESULT		SetShowHighlight(FS_BOOL show);
	FS_RESULT		GetShowHighlight(FS_BOOL* show);
	FS_RESULT		GetForm(FSForm* form);

	IFSPDF_AnnotHandler*		GetWidgetHandler();
	CFSCRT_STPDFActionHandler* GetActionHandler();

	FS_RESULT			SetFocusAnnot(FSAnnot* pFocusAnnot);
	FSAnnot*	GetFocusAnnot() {return m_pFocusAnnot;}

public:
	FS_RESULT		ST_GetWidgetFlag(FSAnnot* pAnnot, FS_INT32* flag);
	FS_RESULT		ST_IsWidgetReadOnly(FSAnnot* pAnnot, FS_BOOL* bReadOnly);

	//add by zhh 2014-2-17
	FS_RESULT		ST_IsWidgetInteractive(FSAnnot* pAnnot, FS_BOOL* bInterac);
protected:
	FS_RESULT			Load();
public:
	FSForm*								m_pForm;
	CFSCRT_JS_ActionHandler*			m_pJSActionHandle;
	CFSCRT_LTWindowslessHanler			m_formInterWindowless;
public:
	CFXJS_RuntimeFactory* GetRuntimFactory() {return FSCRT_GetJSRuntimeFactory();}
protected:	
	FSPDFDoc*	GetDocument();
	CFX_MapPtrToPtr*	m_typeToColor;
#ifdef _FS_PDFIUM_BUILD_
	FX_UINT8			m_nPDFiumAlpha;
#endif
	FX_BOOL				m_show;
	FX_BOOL				m_bUserWindowless;
public:
#if _FSDK_JS_ENGINE_ == _FSDK_JS_ENGINE_DSCRIPT_
	IJS_Runtime*		GetJSRuntime();
#else
	IJS_Runtime*		GetJSRuntime(CFSCRT_LTPDFDocument* pDoc);
#endif

protected:
	CFSCRT_STPDFActionHandler* m_pSTActionHandler;
	IFSPDF_AnnotHandler* m_pWidgetHandler;
public:
	FSAnnot*			m_pCaptureAnnot;
	FSAnnot*			m_pFocusAnnot;
	FSAnnot*			m_pHoverAnnot;

protected:
	virtual FS_RESULT	_Recover();
	virtual void		_Clear();
protected:
	FS_RESULT ST_TriggerWindowlessEvent(FSPDFPage* page, FSMatrix* device2Page, FS_DWORD eventType, FS_LPVOID eventData);
	FS_RESULT ST_InitiateJavaScript(FSPDFDoc* doc);
	FS_RESULT ST_DoDocJavaScriptAction(FSPDFDoc* doc, FS_INT32 trigger);
	FS_RESULT ST_DoPageJavaScriptAction(FSPDFPage* page, FS_INT32 trigger);
	FS_RESULT ST_ValidateValue(const char* fieldName, const char* value, FS_BOOL* bValidate);
	FS_RESULT ST_KeyStroke(const char* fieldName, FSPDF_JAVASCRIPTEVENT* jsEvent);
	FS_RESULT ST_Caculate(const char* fieldName);
	FS_RESULT ST_Format(const char* fieldName, FS_BOOL* formated);
	FS_RESULT ST_DoControlJavaScriptAction(FSFormControl* control, FS_INT32 trigger);
};
*/

class FSFormFillerImp : public FSFormFiller, public CFX_Object
{
public:
	FSFormFillerImp(FSFormImp* pForm, FSFormFillerAssist* assist, CFSCRT_JS_ActionHandler* pJSActionHandle);
	virtual ~FSFormFillerImp();

	void release();
	void render(FSPDFPage* page, const FSMatrix& matrix, FSRenderer* renderer);
	FS_BOOL	onLButtonDown(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onLButtonUp(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onLButtonDoubleClick(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onMouseMove(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onRButtonDown(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onRButtonUp(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onRButtonDoubleClick(FSPDFPage* page, FSPointF* point, FS_DWORD flags);
	FS_BOOL	onKeyDown(FS_DWORD keyCode, FS_DWORD flags);
	FS_BOOL	onKeyUp(FS_DWORD keyCode, FS_DWORD flags);
	FS_BOOL	onChar(FS_DWORD charCode, FS_DWORD flags);

	FS_RESULT	GetForm(FSForm** form);
	IJS_Runtime* GetJSRuntime();
	FSFormFillerAssist* GetFormFillerAssist() {return m_pFormFillerAssist;}

	IFSPDF_AnnotHandler*		GetWidgetHandler();
	CFSCRT_STPDFActionHandler* GetActionHandler();

	FS_RESULT	SetFocusAnnot(FSAnnot* pFocusAnnot);
	FSAnnot*	GetFocusAnnot() {return m_pFocusAnnot;}

	FS_RESULT	ST_GetWidgetFlag(FSAnnot* pAnnot, FS_INT32* flag);
	FS_RESULT	ST_IsWidgetReadOnly(FSAnnot* pAnnot, FS_BOOL* bReadOnly);

public:
	CFSCRT_JS_ActionHandler*			m_pJSActionHandle;
	CFSCRT_LTWindowslessHanler			m_formInterWindowless;

	void highlightFormFields(FS_BOOL isHighlight) {m_isShowHighlight = isHighlight;}
	void setHighlightColor(FS_ARGB color) {m_hightlightColor = color;} 
	FX_BOOL m_isShowHighlight;
	FS_ARGB	m_hightlightColor;
public:
	CFXJS_RuntimeFactory* GetRuntimFactory() {return FSCRT_GetJSRuntimeFactory();}
	FX_BOOL useWindowless() {return TRUE;}
	FSCRT_DEFINELOCK(m_lock);
protected:
	FSFormFillerAssist* m_pFormFillerAssist;
	FSFormImp* m_pForm;

	IFSPDF_AnnotHandler* m_pWidgetHandler;

	FSRenderer* m_renderer;
public:
	FSAnnot*			m_pCaptureAnnot;
	FSAnnot*			m_pFocusAnnot;
	FSAnnot*			m_pHoverAnnot;
};

#endif //_FSPDF_LTFORMFILLER_H_

