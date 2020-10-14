#ifndef _FSCRT_ST_PDF_ACTIONHANDLER_H_
#define _FSCRT_ST_PDF_ACTIONHANDLER_H_

class FSFormFillerImp;
class CPDFSDK_FormActionHandler;
class CPDFSDK_MediaActionHandler;

typedef struct _FSPDF_FieldActionParam
{
	_FSPDF_FieldActionParam()
	{
		bModifier = FALSE;
		bShift = FALSE;
		nCommitKey = 0;
		bKeyDown = FALSE;
		nSelEnd = nSelStart = 0;
		bWillCommit = FALSE;
		bFieldFull = FALSE;
		bRC = TRUE;
	}

	FX_BOOL					bModifier;		//in
	FX_BOOL					bShift;			//in
	int						nCommitKey;		//in
	CFX_WideString			sChange;		//in[out]
	CFX_WideString			sChangeEx;		//in
	FX_BOOL					bKeyDown;		//in
	int						nSelEnd;		//in[out]
	int						nSelStart;		//in[out]
	CFX_WideString			sValue;			//in[out]
	FX_BOOL					bWillCommit;	//in
	FX_BOOL					bFieldFull;		//in
	FX_BOOL					bRC;			//in[out]
}FSPDF_FieldActionParam;

class CPDFSDK_FormActionHandler
{
public:
	FX_BOOL	DoAction_Hide(const CPDF_Action& action, FSPDFDoc* pDocument);
	FX_BOOL	DoAction_SubmitForm(const CPDF_Action& action, FSPDFDoc* pDocument);
	FX_BOOL	DoAction_ResetForm(const CPDF_Action& action, FSPDFDoc* pDocument);
	FX_BOOL	DoAction_ImportData(const CPDF_Action& action, FSPDFDoc* pDocument);
};

class CFSCRT_STPDFActionHandler : public CFX_Object
{
public:
	CFSCRT_STPDFActionHandler();
	~CFSCRT_STPDFActionHandler();

	virtual void		Destroy();
	virtual FX_BOOL		DoAction_DocOpen(const CPDF_Action& action, FSPDFDoc* pDocument/*, CPDFSDK_DocView *pDocView*/);
	virtual FX_BOOL		DoAction_JavaScript(const CPDF_Action& JsAction,CFX_WideString csJSName, FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/);
	virtual FX_BOOL		DoAction_Page(const CPDF_Action& action, enum CPDF_AAction::AActionType eType, FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/);
	virtual FX_BOOL		DoAction_Document(const CPDF_Action& action, enum CPDF_AAction::AActionType eType, FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/);
	virtual FX_BOOL		DoAction_BookMark(CPDF_Bookmark *pBookMark, const CPDF_Action& action, CPDF_AAction::AActionType type, FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/);	
	virtual FX_BOOL		DoAction_Screen(const CPDF_Action& action, CPDF_AAction::AActionType type, FSPDFDoc* pDocument,/* CReader_DocView *pDocView,*/ CFSCRT_LTPDFAnnot* pScreen);
	virtual FX_BOOL		DoAction_Link(const CPDF_Action& action, FSPDFDoc* pDocument/*, CReader_DocView *pDocView*/);
	virtual FX_BOOL		DoAction_Field(const CPDF_Action& action, CPDF_AAction::AActionType type, FSPDFDoc* pDocument, CPDF_FormField* pFormField, FSPDF_FieldActionParam& data);
	virtual FX_BOOL		DoAction_FieldJavaScript(const CPDF_Action& JsAction, CPDF_AAction::AActionType type, FSPDFDoc* pDocument, CPDF_FormField* pFormField, FSPDF_FieldActionParam& data);

public:
	//void				SetFormActionHandler(CPDFSDK_FormActionHandler* pHandler);
	//void				SetMediaActionHandler(CPDFSDK_MediaActionHandler* pHandler);

private:
	FX_BOOL				ExecuteDocumentOpenAction(const CPDF_Action& action, FSPDFDoc* pDocument, /*CReader_DocView *pDocView,*/ CFX_PtrList& list);
	FX_BOOL				ExecuteDocumentPageAction(const CPDF_Action& action, CPDF_AAction::AActionType type, FSPDFDoc* pDocument, /*CReader_DocView *pDocView,*/ CFX_PtrList& list);
	FX_BOOL				ExecuteFieldAction(const CPDF_Action& action, CPDF_AAction::AActionType type, FSPDFDoc* pDocument, /*CReader_DocView* pDocView,*/ CPDF_FormField* pFormField, FSPDF_FieldActionParam& data, CFX_PtrList& list);
	FX_BOOL				ExecuteScreenAction(const CPDF_Action& action, CPDF_AAction::AActionType type, FSPDFDoc* pDocument, /*CReader_DocView* pDocView,*/ CFSCRT_LTPDFAnnot* pScreen, CFX_PtrList& list);
	FX_BOOL				ExecuteBookMark(const CPDF_Action& action, FSPDFDoc* pDocument, /*CReader_DocView* pDocView,*/ CPDF_Bookmark* pBookmark, CFX_PtrList& list);
	FX_BOOL				ExecuteLinkAction(const CPDF_Action& action, FSPDFDoc* pDocument, /*CReader_DocView* pDocView,*/ CFX_PtrList& list);

	void				DoAction_NoJs(const CPDF_Action& action, FSPDFDoc* pDocument/*, CReader_DocView* pDocView*/);
	void				RunDocumentPageJavaScript(FSPDFDoc* pDocument, CPDF_AAction::AActionType type, const CFX_WideString& script);
	void				RunDocumentOpenJavaScript(FSPDFDoc* pDocument, const CFX_WideString& sScriptName, const CFX_WideString& script);
	void				RunFieldJavaScript(FSPDFDoc* pDocument, CPDF_FormField* pFormField, CPDF_AAction::AActionType type, FSPDF_FieldActionParam& data, const CFX_WideString& script);

private:
	FX_BOOL				IsValidField(FSPDFDoc* pDocument, CPDF_Dictionary* pFieldDict);
	FX_BOOL				IsValidDocView(FSPDFDoc* pDocument/*, CReader_DocView* pDocView*/);

	void				DoAction_GoTo(FSPDFDoc* pDocument, /*CReader_DocView *pDocView,*/ const CPDF_Action& action);
	void				DoAction_GoToR(FSPDFDoc* pDocument, const CPDF_Action& action);
	void				DoAction_Launch(FSPDFDoc* pDocument, const CPDF_Action& action);
	void				DoAction_URI(FSPDFDoc* pDocument, const CPDF_Action& action);
	void				DoAction_Named(FSPDFDoc* pDocument, const CPDF_Action& action);
	void				DoAction_SetOCGState(FSPDFDoc* pDocument, /*CReader_DocView* pDocView,*/ const CPDF_Action& action);

protected:
	CPDFSDK_FormActionHandler*		m_pFormActionHandler;
	CPDFSDK_MediaActionHandler*		m_pMediaActionHandler;
};

#endif //_FSCRT_ST_PDF_ACTIONHANDLER_H_
