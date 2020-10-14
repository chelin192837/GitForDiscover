#ifndef _FIELD_H_
#define _FIELD_H_

class JDocument;
enum FIELD_PROP
{
	FP_ALIGNMENT,
	FP_BORDERSTYLE,
	FP_BUTTONALIGNX,
	FP_BUTTONALIGNY,
	FP_BUTTONFITBOUNDS,
	FP_BUTTONPOSITION,
	FP_BUTTONSCALEHOW,
	FP_BUTTONSCALEWHEN,
	FP_CALCORDERINDEX,
	FP_CHARLIMIT,
	FP_COMB,
	FP_COMMITONSELCHANGE,
	FP_CURRENTVALUEINDICES,
	FP_DEFAULTVALUE,
	FP_DONOTSCROLL,
	FP_DISPLAY,
	FP_FILLCOLOR,
	FP_HIDDEN,
	FP_HIGHLIGHT,
	FP_LINEWIDTH,
	FP_MULTILINE,
	FP_MULTIPLESELECTION,
	FP_PASSWORD,
	FP_RECT,
	FP_RICHTEXT,
	FP_RICHVALUE,
	FP_ROTATION,
	FP_STROKECOLOR,
	FP_STYLE,
	FP_TEXTCOLOR,
	FP_TEXTFONT,
	FP_TEXTSIZE,
	FP_USERNAME,
	FP_VALUE
};

class CJS_WideStringArray
{
public:
	CJS_WideStringArray(){}
	virtual ~CJS_WideStringArray()
	{
		for (FX_INT32 i=0,sz=m_Data.GetSize(); i<sz; i++)
			delete m_Data.GetAt(i);
		m_Data.RemoveAll();
	}

	void Add(const CFX_WideString& string)
	{
		m_Data.Add(new CFX_WideString(string));
	}

	FX_INT32 GetSize() const
	{
		return m_Data.GetSize();
	}

	CFX_WideString GetAt(FX_INT32 i) const
	{
		return *m_Data.GetAt(i);
	}

private:
	CFX_ArrayTemplate<CFX_WideString*>	m_Data;
};

struct CJS_DelayData
{
	CFX_WideString			sFieldName;
	FX_INT32						nControlIndex;
	enum FIELD_PROP			eProp;
	FX_INT32				num;
	bool					b;
	CFX_ByteString			string;
	CFX_WideString			widestring;
	CPDF_Rect				rect;
	CPWL_Color				color;
	CFX_DWordArray			wordarray;
	CJS_WideStringArray		widestringarray;
	bool					busy;
};

class JField : public CFXJS_EmbedObj
{
public:
	JField(CFXJS_Object* pJSObject);	
	virtual ~JField(void);

    FX_BOOL alignment(OBJ_PROP_PARAMS);
    FX_BOOL borderStyle(OBJ_PROP_PARAMS);
	FX_BOOL buttonAlignX(OBJ_PROP_PARAMS);
	FX_BOOL buttonAlignY(OBJ_PROP_PARAMS);
	FX_BOOL buttonFitBounds(OBJ_PROP_PARAMS);
	FX_BOOL buttonPosition(OBJ_PROP_PARAMS);
	FX_BOOL buttonScaleHow(OBJ_PROP_PARAMS);
    FX_BOOL buttonScaleWhen(OBJ_PROP_PARAMS);
	FX_BOOL calcOrderIndex(OBJ_PROP_PARAMS);
	FX_BOOL charLimit(OBJ_PROP_PARAMS);
	FX_BOOL comb(OBJ_PROP_PARAMS);
	FX_BOOL commitOnSelChange(OBJ_PROP_PARAMS);
	FX_BOOL currentValueIndices(OBJ_PROP_PARAMS);
    FX_BOOL defaultStyle(OBJ_PROP_PARAMS);
	FX_BOOL defaultValue(OBJ_PROP_PARAMS);
	FX_BOOL doNotScroll(OBJ_PROP_PARAMS);
	FX_BOOL doNotSpellCheck(OBJ_PROP_PARAMS);
	FX_BOOL delay(OBJ_PROP_PARAMS);
	FX_BOOL display(OBJ_PROP_PARAMS);
    FX_BOOL doc(OBJ_PROP_PARAMS);
	FX_BOOL editable(OBJ_PROP_PARAMS);
	FX_BOOL exportValues(OBJ_PROP_PARAMS);
	FX_BOOL fileSelect(OBJ_PROP_PARAMS);
	FX_BOOL fillColor(OBJ_PROP_PARAMS);
	FX_BOOL hidden(OBJ_PROP_PARAMS);
    FX_BOOL highlight(OBJ_PROP_PARAMS);
	FX_BOOL lineWidth(OBJ_PROP_PARAMS);
	FX_BOOL multiline(OBJ_PROP_PARAMS);
	FX_BOOL multipleSelection(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL numItems(OBJ_PROP_PARAMS);
    FX_BOOL page(OBJ_PROP_PARAMS);
	FX_BOOL password(OBJ_PROP_PARAMS);
	FX_BOOL print(OBJ_PROP_PARAMS);
	FX_BOOL radiosInUnison(OBJ_PROP_PARAMS);
	FX_BOOL readonly(OBJ_PROP_PARAMS);
	FX_BOOL rect(OBJ_PROP_PARAMS);
    FX_BOOL required(OBJ_PROP_PARAMS);
	FX_BOOL richText(OBJ_PROP_PARAMS);
	FX_BOOL richValue(OBJ_PROP_PARAMS);
	FX_BOOL rotation(OBJ_PROP_PARAMS);
	FX_BOOL strokeColor(OBJ_PROP_PARAMS);
	FX_BOOL style(OBJ_PROP_PARAMS);
	FX_BOOL submitName(OBJ_PROP_PARAMS);
	FX_BOOL textColor(OBJ_PROP_PARAMS);
	FX_BOOL textFont(OBJ_PROP_PARAMS);
	FX_BOOL textSize(OBJ_PROP_PARAMS);
	FX_BOOL type(OBJ_PROP_PARAMS);
	FX_BOOL userName(OBJ_PROP_PARAMS);
	FX_BOOL value(OBJ_PROP_PARAMS);
	FX_BOOL valueAsString(OBJ_PROP_PARAMS);
	FX_BOOL source(OBJ_PROP_PARAMS);

	FX_BOOL browseForFileToSubmit(OBJ_METHOD_PARAMS);
	FX_BOOL buttonGetCaption(OBJ_METHOD_PARAMS);
	FX_BOOL buttonGetIcon(OBJ_METHOD_PARAMS);
	FX_BOOL buttonImportIcon(OBJ_METHOD_PARAMS);
	FX_BOOL buttonSetCaption(OBJ_METHOD_PARAMS);
	FX_BOOL buttonSetIcon(OBJ_METHOD_PARAMS);
	FX_BOOL checkThisBox(OBJ_METHOD_PARAMS);
	FX_BOOL clearItems(OBJ_METHOD_PARAMS);
	FX_BOOL defaultIsChecked(OBJ_METHOD_PARAMS);
	FX_BOOL deleteItemAt(OBJ_METHOD_PARAMS);
	FX_BOOL getArray(OBJ_METHOD_PARAMS);
	FX_BOOL getItemAt(OBJ_METHOD_PARAMS);
	FX_BOOL getLock(OBJ_METHOD_PARAMS);
	FX_BOOL insertItemAt(OBJ_METHOD_PARAMS);
	FX_BOOL isBoxChecked(OBJ_METHOD_PARAMS);
	FX_BOOL isDefaultChecked(OBJ_METHOD_PARAMS);
	FX_BOOL setAction(OBJ_METHOD_PARAMS);
	FX_BOOL setFocus(OBJ_METHOD_PARAMS);
	FX_BOOL setItems(OBJ_METHOD_PARAMS);
	FX_BOOL setLock(OBJ_METHOD_PARAMS);
	FX_BOOL signatureGetModifications(OBJ_METHOD_PARAMS);
	FX_BOOL signatureGetSeedValue(OBJ_METHOD_PARAMS);
	FX_BOOL signatureInfo(OBJ_METHOD_PARAMS);
	FX_BOOL signatureSetSeedValue(OBJ_METHOD_PARAMS);
	FX_BOOL signatureSign(OBJ_METHOD_PARAMS);
	FX_BOOL signatureValidate(OBJ_METHOD_PARAMS);

public:
	static void SetAlignment(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string);
    static void SetBorderStyle(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string);
	static void SetButtonAlignX(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetButtonAlignY(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetButtonFitBounds(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetButtonPosition(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetButtonScaleHow(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
    static void SetButtonScaleWhen(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetCalcOrderIndex(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetCharLimit(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetComb(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetCommitOnSelChange(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetCurrentValueIndices(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_DWordArray& array);
    static void SetDefaultStyle(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex);
	static void SetDefaultValue(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_WideString& string);
	static void SetDoNotScroll(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetDisplay(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetFillColor(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPWL_Color& color);
	static void SetHidden(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
    static void SetHighlight(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string);
	static void SetLineWidth(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetMultiline(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetMultipleSelection(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetPassword(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetRect(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPDF_Rect& rect);
	static void SetRichText(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, bool b);
	static void SetRichValue(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex);
	static void SetRotation(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetStrokeColor(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPWL_Color& color);
	static void SetStyle(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string);
	static void SetTextColor(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CPWL_Color& color);
	static void SetTextFont(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_ByteString& string);
	static void SetTextSize(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, FX_INT32 number);
	static void SetUserName(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CFX_WideString& string);
	static void SetValue(FSPDFDoc* pDocument, const CFX_WideString& swFieldName, FX_INT32 nControlIndex, const CJS_WideStringArray& strArray);

public:
	static void	AddField(FSPDFDoc* pDocument, FX_INT32 nPageIndex, FX_INT32 nFieldType, 
											const CFX_WideString& sName, const CPDF_Rect& rcCoords);
public:
	static void	UpdateFormField(FSPDFDoc* pDocument, CPDF_FormField* pFormField, 
											FX_BOOL bChangeMark, FX_BOOL bResetAP, FX_BOOL bRefresh);
	static void	UpdateFormControl(FSPDFDoc* pDocument, CPDF_FormControl* pFormControl, 
											FX_BOOL bChangeMark, FX_BOOL bResetAP, FX_BOOL bRefresh);

	static FSFormControl*		GetWidget(FSPDFDoc* pDocument, CPDF_FormControl* pFormControl);
	static void	GetFormFields(FSPDFDoc* pDocument, const CFX_WideString& csFieldName, CFX_PtrArray& FieldsArray);

	static void	DoDelay(FSPDFDoc* pDocument, CJS_DelayData* pData);

public:
	FX_BOOL	AttachField(JDocument* pDocument, const CFX_WideString& csFieldName);
	void SetDelay(FX_BOOL bDelay);

protected:
	void ParseFieldName(const CFX_WideString &strFieldNameParsed,CFX_WideString &strFieldName,FX_INT32 & iControlNo);
	void GetFormFields(const CFX_WideString& csFieldName, CFX_PtrArray& FieldsArray);
	CPDF_FormControl* GetSmartFieldControl(CPDF_FormField* pFormField);
	FX_BOOL	ValueIsOccur(CPDF_FormField* pFormField, CFX_WideString csOptLabel);

	void AddDelay_Int(enum FIELD_PROP prop, FX_INT32 n);
	void AddDelay_Bool(enum FIELD_PROP prop,bool b);
	void AddDelay_String(enum FIELD_PROP prop, const CFX_ByteString& string);
	void AddDelay_WideString(enum FIELD_PROP prop, const CFX_WideString& string);
	void AddDelay_Rect(enum FIELD_PROP prop, const CPDF_Rect& rect);
	void AddDelay_Color(enum FIELD_PROP prop, const CPWL_Color& color);
	void AddDelay_WordArray(enum FIELD_PROP prop, const CFX_DWordArray& array);
	void AddDelay_WideStringArray(enum FIELD_PROP prop, const CJS_WideStringArray& array);

	void DoDelay();
protected:
	//Helper Functions
	//1. SetChangeMark
	static FX_BOOL JS_SetChangeMark(FSPDFDoc* pDoc, FX_BOOL bChanged);

	//2. JS_Browse
	FX_BOOL JS_fieldBrowse(CFX_WideString& wsFilePath);

	//3. JS_GetCurPageIndex
	FX_BOOL JS_GetCurPageIndex(FX_INT32& nPageIndex);

	//4. JS_GetPageRotation
	static FX_BOOL JS_GetPageRotation(FSPDFPage* pPage, FX_INT32& nPageRotation);

	//5. JS_GetPage
	static FX_BOOL JS_GetPage(FSPDFDoc* pDoc,FX_INT32 pageIndex, FSPDFPage** ppPage);

public:
	JDocument*							m_pJSDoc;
	FSPDFDocImp*						m_pDocument;
	CFX_WideString						m_FieldName;
	FX_INT32									m_nFormControlIndex;
	FX_BOOL								m_bCanSet;

	FX_BOOL								m_bDelay;
};

class CFXJS_Field : public CFXJS_Object
{
public:
	CFXJS_Field(JSFXObject* pObject) : CFXJS_Object(pObject) {};
	virtual ~CFXJS_Field(void){};

	DECLARE_JS_CLASS(CFXJS_Field);

    JS_STATIC_PROP(alignment, JField);
    JS_STATIC_PROP(borderStyle, JField);
	JS_STATIC_PROP(buttonAlignX, JField);
	JS_STATIC_PROP(buttonAlignY, JField);
	JS_STATIC_PROP(buttonFitBounds, JField);
	JS_STATIC_PROP(buttonPosition, JField);
	JS_STATIC_PROP(buttonScaleHow, JField);
    JS_STATIC_PROP(buttonScaleWhen, JField);
	JS_STATIC_PROP(calcOrderIndex, JField);
	JS_STATIC_PROP(charLimit, JField);
	JS_STATIC_PROP(comb, JField);
	JS_STATIC_PROP(commitOnSelChange, JField);
	JS_STATIC_PROP(currentValueIndices, JField);
    JS_STATIC_PROP(defaultStyle, JField);
	JS_STATIC_PROP(defaultValue, JField);
	JS_STATIC_PROP(doNotScroll, JField);
	JS_STATIC_PROP(doNotSpellCheck, JField);
	JS_STATIC_PROP(delay, JField);
	JS_STATIC_PROP(display, JField);
    JS_STATIC_PROP(doc, JField);
	JS_STATIC_PROP(editable, JField);
	JS_STATIC_PROP(exportValues, JField);
	JS_STATIC_PROP(fileSelect, JField);
	JS_STATIC_PROP(fillColor, JField);
	JS_STATIC_PROP(hidden, JField);
    JS_STATIC_PROP(highlight, JField);
	JS_STATIC_PROP(lineWidth, JField);
	JS_STATIC_PROP(multiline, JField);
	JS_STATIC_PROP(multipleSelection, JField);
	JS_STATIC_PROP(name, JField);
	JS_STATIC_PROP(numItems, JField);
    JS_STATIC_PROP(page, JField);
	JS_STATIC_PROP(password, JField);
	JS_STATIC_PROP(print, JField);
	JS_STATIC_PROP(radiosInUnison, JField);
	JS_STATIC_PROP(readonly, JField);
	JS_STATIC_PROP(rect, JField);
    JS_STATIC_PROP(required, JField);
	JS_STATIC_PROP(richText, JField);
	JS_STATIC_PROP(richValue, JField);
	JS_STATIC_PROP(rotation, JField);
	JS_STATIC_PROP(strokeColor, JField);
	JS_STATIC_PROP(style, JField);
	JS_STATIC_PROP(submitName, JField);
	JS_STATIC_PROP(textColor, JField);
	JS_STATIC_PROP(textFont, JField);
	JS_STATIC_PROP(textSize, JField);
	JS_STATIC_PROP(type, JField);
	JS_STATIC_PROP(userName, JField);
	JS_STATIC_PROP(value, JField);
	JS_STATIC_PROP(valueAsString, JField);
	JS_STATIC_PROP(source, JField);

	JS_STATIC_METHOD(browseForFileToSubmit, JField);
	JS_STATIC_METHOD(buttonGetCaption, JField);
	JS_STATIC_METHOD(buttonGetIcon, JField);
	JS_STATIC_METHOD(buttonImportIcon, JField);
	JS_STATIC_METHOD(buttonSetCaption, JField);
	JS_STATIC_METHOD(buttonSetIcon, JField);
	JS_STATIC_METHOD(checkThisBox, JField);
	JS_STATIC_METHOD(clearItems, JField);
 	JS_STATIC_METHOD(defaultIsChecked, JField);
	JS_STATIC_METHOD(deleteItemAt, JField);
	JS_STATIC_METHOD(getArray, JField);
	JS_STATIC_METHOD(getItemAt, JField);
	JS_STATIC_METHOD(getLock, JField);
	JS_STATIC_METHOD(insertItemAt, JField);
	JS_STATIC_METHOD(isBoxChecked, JField);
	JS_STATIC_METHOD(isDefaultChecked, JField);
	JS_STATIC_METHOD(setAction, JField);
	JS_STATIC_METHOD(setFocus, JField);
	JS_STATIC_METHOD(setItems, JField);
	JS_STATIC_METHOD(setLock, JField);
	JS_STATIC_METHOD(signatureGetModifications, JField);
	JS_STATIC_METHOD(signatureGetSeedValue, JField);
	JS_STATIC_METHOD(signatureInfo, JField);
	JS_STATIC_METHOD(signatureSetSeedValue, JField);
	JS_STATIC_METHOD(signatureSign, JField);
	JS_STATIC_METHOD(signatureValidate, JField);
};

#endif //_FIELD_H_

