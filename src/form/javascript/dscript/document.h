#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

class FXPrintParamsObj : public CFXJS_EmbedObj
{
public:
	FXPrintParamsObj(CFXJS_Object* pJSObject);
	virtual ~FXPrintParamsObj(){}
	
public:
	FX_BOOL bUI;
	int nStart;
	int nEnd;
	FX_BOOL bSilent;
	FX_BOOL bShrinkToFit;
	FX_BOOL bPrintAsImage;
	FX_BOOL bReverse;
	FX_BOOL bAnnotations;
};

class CFXJS_PrintParamsObj : public CFXJS_Object
{
public:
	CFXJS_PrintParamsObj(JSFXObject* pObject) : CFXJS_Object(pObject) {}
	virtual ~CFXJS_PrintParamsObj(){}
	
	DECLARE_JS_CLASS(CFXJS_PrintParamsObj);
};


class JIcon;
class JField;

struct JIconElement
{
	JIconElement() : IconName(L""), IconStream(NULL), NextIcon(NULL){}
	virtual ~JIconElement()
	{
	}
	CFX_WideString	IconName;
	JIconElement*	NextIcon;
	JIcon*			IconStream;
};

class JIconTree
{
public:
	JIconTree():m_pHead(NULL), m_pEnd(NULL), m_iLength(0)
	{

	}

	virtual ~JIconTree()
	{
	}

public:
	void			InsertIconElement(JIconElement* pNewIcon);
	void			DeleteIconElement(CFX_WideString swIconName);
	void			DeleteIconTree();
	int				GetLength();
	JIconElement*	operator[](int iIndex);

private:
	JIconElement*	m_pHead;
	JIconElement*	m_pEnd;
	int				m_iLength;
};

struct CJS_DelayData;
struct CJS_DelayAnnot;
struct CFXJS_AnnotObj;

class JDocument : public CFXJS_EmbedObj
{
public:
	JDocument(CFXJS_Object* pJSObject);
	virtual ~JDocument();

public:
	FX_BOOL	ADBE(OBJ_PROP_PARAMS);
	FX_BOOL	author(OBJ_PROP_PARAMS);
	FX_BOOL	baseURL(OBJ_PROP_PARAMS);
	FX_BOOL	bookmarkRoot(OBJ_PROP_PARAMS);
	FX_BOOL	calculate(OBJ_PROP_PARAMS);
	FX_BOOL	Collab(OBJ_PROP_PARAMS);
	FX_BOOL	creationDate(OBJ_PROP_PARAMS);
	FX_BOOL	creator(OBJ_PROP_PARAMS);
	FX_BOOL	delay(OBJ_PROP_PARAMS);
	FX_BOOL	dirty(OBJ_PROP_PARAMS);
	FX_BOOL	documentFileName(OBJ_PROP_PARAMS);
	FX_BOOL external(OBJ_PROP_PARAMS);
	FX_BOOL	filesize(OBJ_PROP_PARAMS);
	FX_BOOL	icons(OBJ_PROP_PARAMS);
	FX_BOOL	info(OBJ_PROP_PARAMS);
	FX_BOOL	keywords(OBJ_PROP_PARAMS);
	FX_BOOL	layout(OBJ_PROP_PARAMS);
	FX_BOOL	media(OBJ_PROP_PARAMS);
	FX_BOOL	modDate(OBJ_PROP_PARAMS);
	FX_BOOL	mouseX(OBJ_PROP_PARAMS);
	FX_BOOL	mouseY(OBJ_PROP_PARAMS);
	FX_BOOL	numFields(OBJ_PROP_PARAMS);
	FX_BOOL	numPages(OBJ_PROP_PARAMS);
	FX_BOOL	pageNum(OBJ_PROP_PARAMS);
	FX_BOOL	pageWindowRect(OBJ_PROP_PARAMS);
	FX_BOOL	path(OBJ_PROP_PARAMS);
	FX_BOOL	producer(OBJ_PROP_PARAMS);
	FX_BOOL	subject(OBJ_PROP_PARAMS);
	FX_BOOL	title(OBJ_PROP_PARAMS);
	FX_BOOL	zoom(OBJ_PROP_PARAMS);
	FX_BOOL	zoomType(OBJ_PROP_PARAMS);

	FX_BOOL addAnnot(OBJ_METHOD_PARAMS);
	FX_BOOL	addField(OBJ_METHOD_PARAMS);
	FX_BOOL	addLink(OBJ_METHOD_PARAMS);
	FX_BOOL	addIcon(OBJ_METHOD_PARAMS);
	FX_BOOL	calculateNow(OBJ_METHOD_PARAMS);
	FX_BOOL	closeDoc(OBJ_METHOD_PARAMS);
	FX_BOOL	createDataObject(OBJ_METHOD_PARAMS);
	FX_BOOL deletePages(OBJ_METHOD_PARAMS);
	FX_BOOL	exportAsText(OBJ_METHOD_PARAMS);
	FX_BOOL	exportAsFDF(OBJ_METHOD_PARAMS);
	FX_BOOL	exportAsXFDF(OBJ_METHOD_PARAMS);
	FX_BOOL extractPages(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnot(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnots(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnot3D(OBJ_METHOD_PARAMS);
	FX_BOOL	getAnnots3D(OBJ_METHOD_PARAMS);
	FX_BOOL	getField(OBJ_METHOD_PARAMS);
	FX_BOOL	getIcon(OBJ_METHOD_PARAMS);
	FX_BOOL	getLinks(OBJ_METHOD_PARAMS);
	FX_BOOL	getNthFieldName(OBJ_METHOD_PARAMS);
	FX_BOOL	getOCGs(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageBox(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageNthWord(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageNthWordQuads(OBJ_METHOD_PARAMS);
	FX_BOOL	getPageNumWords(OBJ_METHOD_PARAMS);
	FX_BOOL getPrintParams(OBJ_METHOD_PARAMS);
	FX_BOOL getURL(OBJ_METHOD_PARAMS);
	FX_BOOL	importAnFDF(OBJ_METHOD_PARAMS);
	FX_BOOL	importAnXFDF(OBJ_METHOD_PARAMS);
	FX_BOOL	importTextData(OBJ_METHOD_PARAMS);
	FX_BOOL insertPages(OBJ_METHOD_PARAMS);
	FX_BOOL	mailForm(OBJ_METHOD_PARAMS);
	FX_BOOL	print(OBJ_METHOD_PARAMS);
	FX_BOOL	removeField(OBJ_METHOD_PARAMS);
	FX_BOOL replacePages(OBJ_METHOD_PARAMS);
	FX_BOOL	resetForm(OBJ_METHOD_PARAMS);
	FX_BOOL	saveAs(OBJ_METHOD_PARAMS);
	FX_BOOL	submitForm(OBJ_METHOD_PARAMS);
	FX_BOOL	mailDoc(OBJ_METHOD_PARAMS);
	FX_BOOL	removeIcon(OBJ_METHOD_PARAMS);
	
public:
	void AttachDoc(FSPDFDoc* pDoc);
	FSPDFDoc* GetReaderDoc();

	static FX_BOOL				ExtractFileName(FSPDFDoc* pDoc, CFX_ByteString& strFileName);
	static FX_BOOL				ExtractFolderName(FSPDFDoc* pDoc, CFX_ByteString& strFolderName);

public:
	void						AddDelayData(CJS_DelayData* pData);
	void						DoFieldDelay(const CFX_WideString& sFieldName, int nControlIndex);

	void						AddDelayAnnotData(CFXJS_AnnotObj *pData);
	void						DoAnnotDelay();

private:
	CFX_WideString				ReversalStr(CFX_WideString cbFrom);
	CFX_WideString				CutString(CFX_WideString cbFrom);
	bool						IsEnclosedInRect(CFX_FloatRect rect, CFX_FloatRect LinkRect);
	int							CountWords(CPDF_TextObject* pTextObj);
	CFX_WideString				GetObjWordStr(CPDF_TextObject* pTextObj, int nWordIndex);

	FX_BOOL						ParserParams(JSObject *pObj,CFXJS_AnnotObj& annotobj);

protected:
	//Helper Functions
	//1. Subform Form Data
	void JS_docGetPath(CFX_WideString& wsDocPath);
	FX_BOOL JS_docSubmitForm(void* formData, FX_INT32 length, FX_LPCWSTR URL);
	FX_BOOL FDFToURLEncodedData(FX_LPBYTE& pBuf, FX_STRSIZE& nBufSize);
	FX_BOOL ExportFieldsToFDFTextBuf(const CFX_PtrArray& fields,FX_BOOL bIncludeOrExclude, CFX_ByteTextBuf& textBuf);
	FX_BOOL SubmitFields(const CFX_WideString& csDestination, const CFX_PtrArray& fields,
		FX_BOOL bIncludeOrExclude, FX_BOOL bUrlEncoded);
	FX_BOOL SubmitForm(const CFX_WideString& sDestination, FX_BOOL bUrlEncoded);

	//2. Mail Doc
	FX_BOOL JS_docmailForm(void* mailData, FX_INT32 length, FS_BOOL bUI,FX_LPCWSTR To, FX_LPCWSTR Subject, FX_LPCWSTR CC, FX_LPCWSTR BCC, FX_LPCWSTR Msg);

	//3. SetChangeMark
	FX_BOOL JS_SetChangeMark(FX_BOOL bChanged);

	//4. OpenURL
	FX_BOOL JS_OpenURL(CFX_WideString& wsURL);

private:
	JIconTree*					m_pIconTree;
	FSPDFDocImp*				m_pDocument;
	CFX_WideString				m_cwBaseURL;

	FX_BOOL								m_bDelay;
	CFX_ArrayTemplate<CJS_DelayData*>	m_DelayData;
	CFX_ArrayTemplate<CFXJS_AnnotObj*>	m_DelayAnnotData;
};

class CFXJS_Document : public CFXJS_Object
{
public:
	CFXJS_Document(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Document(){};

	virtual FX_BOOL	InitInstance(IDS_Context* cc);	

	DECLARE_JS_CLASS(CFXJS_Document);

	JS_STATIC_PROP(ADBE, JDocument);
	JS_STATIC_PROP(author, JDocument);
	JS_STATIC_PROP(baseURL, JDocument);
	JS_STATIC_PROP(bookmarkRoot, JDocument);
	JS_STATIC_PROP(calculate, JDocument);
	JS_STATIC_PROP(Collab, JDocument);
	JS_STATIC_PROP(creationDate, JDocument);
	JS_STATIC_PROP(creator, JDocument);
	JS_STATIC_PROP(delay, JDocument);
	JS_STATIC_PROP(dirty, JDocument);
	JS_STATIC_PROP(documentFileName, JDocument);
	JS_STATIC_PROP(external, JDocument);
	JS_STATIC_PROP(filesize, JDocument);
	JS_STATIC_PROP(icons, JDocument);
	JS_STATIC_PROP(info, JDocument);
	JS_STATIC_PROP(keywords, JDocument);
	JS_STATIC_PROP(layout, JDocument);
	JS_STATIC_PROP(media, JDocument);
	JS_STATIC_PROP(modDate, JDocument);
	JS_STATIC_PROP(mouseX, JDocument);
	JS_STATIC_PROP(mouseY, JDocument);
	JS_STATIC_PROP(numFields, JDocument);
	JS_STATIC_PROP(numPages, JDocument);
	JS_STATIC_PROP(pageNum, JDocument);
	JS_STATIC_PROP(pageWindowRect, JDocument);
	JS_STATIC_PROP(path, JDocument);
	JS_STATIC_PROP(producer, JDocument);
	JS_STATIC_PROP(subject, JDocument);
	JS_STATIC_PROP(title, JDocument);
	JS_STATIC_PROP(zoom, JDocument);
	JS_STATIC_PROP(zoomType, JDocument);

	JS_STATIC_METHOD(addAnnot,JDocument);
	JS_STATIC_METHOD(addField, JDocument);
	JS_STATIC_METHOD(addLink, JDocument);
	JS_STATIC_METHOD(addIcon, JDocument);
	JS_STATIC_METHOD(calculateNow, JDocument);
	JS_STATIC_METHOD(closeDoc, JDocument);
	JS_STATIC_METHOD(createDataObject, JDocument);
	JS_STATIC_METHOD(deletePages, JDocument);
	JS_STATIC_METHOD(exportAsText, JDocument);
	JS_STATIC_METHOD(exportAsFDF, JDocument);
	JS_STATIC_METHOD(exportAsXFDF, JDocument);
	JS_STATIC_METHOD(extractPages, JDocument);
	JS_STATIC_METHOD(getAnnot, JDocument);
	JS_STATIC_METHOD(getAnnots, JDocument);
	JS_STATIC_METHOD(getAnnot3D, JDocument);
	JS_STATIC_METHOD(getAnnots3D, JDocument);
	JS_STATIC_METHOD(getField, JDocument);
	JS_STATIC_METHOD(getIcon, JDocument);
	JS_STATIC_METHOD(getLinks, JDocument);
	JS_STATIC_METHOD(getNthFieldName, JDocument);
	JS_STATIC_METHOD(getOCGs, JDocument);
	JS_STATIC_METHOD(getPageBox, JDocument);
	JS_STATIC_METHOD(getPageNthWord, JDocument);
	JS_STATIC_METHOD(getPageNthWordQuads, JDocument);
	JS_STATIC_METHOD(getPageNumWords, JDocument);
	JS_STATIC_METHOD(getPrintParams, JDocument);
	JS_STATIC_METHOD(getURL, JDocument);
	JS_STATIC_METHOD(importAnFDF, JDocument);
	JS_STATIC_METHOD(importAnXFDF, JDocument);
	JS_STATIC_METHOD(importTextData, JDocument);
	JS_STATIC_METHOD(insertPages, JDocument);
	JS_STATIC_METHOD(mailForm, JDocument);
	JS_STATIC_METHOD(print, JDocument);
	JS_STATIC_METHOD(removeField, JDocument);
	JS_STATIC_METHOD(replacePages, JDocument);
	JS_STATIC_METHOD(removeIcon, JDocument);
	JS_STATIC_METHOD(resetForm, JDocument);
	JS_STATIC_METHOD(saveAs, JDocument);
	JS_STATIC_METHOD(submitForm, JDocument);
	JS_STATIC_METHOD(mailDoc, JDocument);
};

#endif//_DOCUMENT_H_

