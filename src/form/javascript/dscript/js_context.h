#ifndef _JS_CONTEXT_H_
#define _JS_CONTEXT_H_

class CFXJS_EventHandler;
class CFXJS_Runtime;

//used for wrap pdfium, new js action handler and old js action handler.
class CFSCRT_JS_ActionHandler
{
public:
#ifdef _FS_PDFIUM_BUILD_
	FPDF_FORMFILLINFO* m_pFFI;
#endif

public:
	CFSCRT_JS_ActionHandler();

	//FS_RESULT SetHandler(FSPDF_JAVASCRIPTACTION_HANDLER* handler);
	//FS_RESULT SetHandler(FSPDF_ACTION_HANDLER* handler);
#ifdef _FS_PDFIUM_BUILD_
	FS_RESULT SetHandler(FPDF_FORMFILLINFO* handler);
#endif

public://functions from FSPDF_JAVASCRIPTACTION_HANDLER
	FS_RESULT   Release();

	FS_RESULT   InvalidateRect(FSPDFDoc* document, FS_INT32 pageIndex, FSRectF* pdfRect);

	FS_RESULT   GetCurrentPageIndex(FSPDFDoc* document, FS_INT32* pageIndex);

	FS_RESULT   SetCurrentPageIndex(FSPDFDoc* document, FS_INT32 pageIndex);

	FS_RESULT   GetRotation(FSPDFDoc* document, FS_INT32 pageIndex, FS_INT32* rotation);

	FS_RESULT   ExecuteNamedAction(FSPDFDoc* document, const char* namedAction);

	FS_RESULT   SetChangeMark(FSPDFDoc* document, FS_BOOL changeMark);

	FS_RESULT   GetChangeMark(FSPDFDoc* document, FS_BOOL* changeMark);

	FS_RESULT   GetOpenedDocs(FSPDFDoc** docs, FS_INT32* count);

	FS_RESULT   Alert(const char* msg, const char* title, FS_INT32 type, FS_INT32 icon, FS_INT32* retCode);

	FS_RESULT   Beep(FS_INT32 type);

	FS_RESULT   Response(const char* question, const char* title, const char* defaultValue, 
		const char* label, FS_BOOL isPassword, char* response);

	FS_RESULT   GetFilePath(FSPDFDoc* document, char* filePath);

	FS_RESULT   Mail(FSPDFDoc* document, FX_LPBYTE data, FS_DWORD len, FS_BOOL bUI, const char* To, 
		const char* Subject, const char* CC, const char* BCC, const char* Msg, FS_INT32 nMailType);

	FS_RESULT   Print(FSPDFDoc* document, FS_BOOL isUI, FS_INT32 start, FS_INT32 end, FS_BOOL isSilent ,
		FS_BOOL isShrinkToFit, FS_BOOL isPrintAsImage, FS_BOOL isReverse, FS_BOOL isAnnotations);

	FS_RESULT   SubmitForm(FSPDFDoc* document, FS_LPVOID formData, FS_DWORD length, const char* URL);

	FS_RESULT   LaunchURL(const char* URL); 

	FS_RESULT   BrowseFile(char* filePath);

	FS_RESULT   GetLanguage(FS_INT32* language);

	//FS_RESULT   GetIdentityProperties(FSPDF_IDENTITYPROPERTIES* identityProperties);

	//FS_RESULT   PopupMenu(const FSPDF_MENULIST* menus, FS_INT32 count, char* seletedMenu);

	FS_RESULT   GetAppInfo(FS_INT32 type, char* info);

	/*FS_RESULT   OutputJSMessage(char* message);*/
};

class CFXJS_Context : public IJS_Context, public IDS_Context
{
public:
	CFXJS_Context(CFXJS_Runtime* pRuntime);
	virtual ~CFXJS_Context();

public:
	virtual FX_BOOL				Compile(const CFX_WideString& script, CFX_WideString& info);
	virtual FX_BOOL				RunScript(const CFX_WideString& script, CFX_WideString& info);

public:
	virtual void				OnApp_Init();

	virtual void				OnDoc_Open(FSPDFDoc* pDoc, const CFX_WideString& strTargetName);
	virtual void				OnDoc_WillPrint(FSPDFDoc* pDoc);
	virtual void				OnDoc_DidPrint(FSPDFDoc* pDoc);
	virtual void				OnDoc_WillSave(FSPDFDoc* pDoc);
	virtual void				OnDoc_DidSave(FSPDFDoc* pDoc);
	virtual void				OnDoc_WillClose(FSPDFDoc* pDoc);

	virtual void				OnPage_Open(FSPDFDoc* pTarget);
	virtual void				OnPage_Close(FSPDFDoc* pTarget);
	virtual void				OnPage_InView(FSPDFDoc* pTarget);
	virtual void				OnPage_OutView(FSPDFDoc* pTarget);
	
	virtual void				OnField_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget);
	virtual void				OnField_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget);
	virtual void				OnField_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget);
	virtual void				OnField_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField *pTarget);
	virtual void				OnField_Focus(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, const CFX_WideString& Value);
	virtual void				OnField_Blur(FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, const CFX_WideString& Value);

	virtual void				OnField_Calculate(CPDF_FormField* pSource, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL& bRc);
	virtual void				OnField_Format(int nCommitKey, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL bWillCommit);
	virtual void				OnField_Keystroke(int nCommitKey, CFX_WideString& strChange, const CFX_WideString& strChangeEx,
									FX_BOOL bKeyDown, FX_BOOL bModifier, int &nSelEnd,int &nSelStart, FX_BOOL bShift,
									CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL bWillCommit, 
									FX_BOOL bFieldFull, FX_BOOL &bRc);
	virtual void				OnField_Validate(CFX_WideString& strChange, const CFX_WideString& strChangeEx, FX_BOOL bKeyDown,
									FX_BOOL bModifier, FX_BOOL bShift, CPDF_FormField* pTarget, CFX_WideString& Value, FX_BOOL& bRc);

	virtual void				OnScreen_Focus(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_Blur(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_Open(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_Close(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_MouseDown(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_MouseUp(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_MouseEnter(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_MouseExit(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_InView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);
	virtual void				OnScreen_OutView(FX_BOOL bModifier, FX_BOOL bShift, FSAnnot* pScreen);

	virtual void				OnBookmark_MouseUp(CPDF_Bookmark* pBookMark);
	virtual void				OnLink_MouseUp(FSPDFDoc* pTarget);

	virtual void				OnMenu_Exec(FSPDFDoc* pTarget, const CFX_WideString& strTargetName);
	virtual void				OnBatchExec(FSPDFDoc* pTarget);
	virtual void				OnConsole_Exec();
	virtual void				OnExternal_Exec();

	virtual void				EnableMessageBox(FX_BOOL bEnable) {m_bMsgBoxEnable = bEnable;}
	FX_BOOL						IsMsgBoxEnabled() const {return m_bMsgBoxEnable;}
	virtual IJS_Runtime*		GetRuntime(){return (IJS_Runtime*)m_pRuntime;}

	virtual void				SetAttachedData(void* pData){}
	virtual void*				GetAttachedData() const{return NULL;}

public:
	
	CFXJS_Runtime*				GetJSRuntime(){return m_pRuntime;}

	FX_BOOL						DoJob(int nMode, const CFX_WideString& script, CFX_WideString& info, Value*& pRet, Dobject* pVariables);

	CFXJS_EventHandler*			GetEventHandler(){return m_pEventHandler;};	
	FSPDFDoc*					GetReaderDocument();
	CFSCRT_LTAppHandler*			GetReaderApp();

private:
	CFXJS_Runtime*				m_pRuntime;	
	CFXJS_EventHandler*			m_pEventHandler;	

	FX_BOOL						m_bBusy;	
	FX_BOOL						m_bMsgBoxEnable;
};

FS_RESULT outputMessage(CFX_ByteString info, CFSCRT_JS_ActionHandler* handle);

// static CFX_WideString JSGetStringFromID(CJS_Context* pContext, UINT ID)
// {
// 	ASSERT(pContext != NULL);
// 
// 	return JS_LoadString(pContext->GetReaderApp(), ID);
// }

#endif //_JS_CONTEXT_H_

