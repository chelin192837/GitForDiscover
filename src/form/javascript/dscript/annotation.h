#ifndef _ANNOTATION_H_
#define _ANNOTATION_H_

class JDocument;

enum JS_ANNOTTYPE
{
	ANNOT_UNKNOWN = -1,
	ANNOT_CARET = 0,
	ANNOT_CIRCLE,
	ANNOT_FILEATTACHMENT,
	ANNOT_FREETEXT,
	ANNOT_HIGHLIGHT,
	ANNOT_INK,
	ANNOT_LINE,
	ANNOT_POLYGON,
	ANNOT_POLYLINE,
	ANNOT_SOUND,
	ANNOT_SQUARE,
	ANNOT_SQUIGGLY,
	ANNOT_STAMP,
	ANNOT_STRIKEOUT,
	ANNOT_TEXT,
	ANNOT_UNDERLINE
};

enum ANNOT_TYPE
{
	ANNOT_ARROWBEGIN = 0,
	ANNOT_ARROWEND,
	ANNOT_ATTACHICON,
	ANNOT_AUTHOR,
	ANNOT_BORDEREFFECTINTENSITY,
	ANNOT_BORDEREFFECTSTYLE,
	ANNOT_CALLOUT,
	ANNOT_CARETSYMBOL,
	ANNOT_CONTENTS,
	ANNOT_CREATIONDATE,
	ANNOT_DASH,
	ANNOT_FILLCOLOR,
	ANNOT_GESTURES,
	ANNOT_HIDE,
	ANNOT_LEADEREXTEND,
	ANNOT_LEADERLENGTH,
	ANNOT_LINEENDING,
	ANNOT_ANNOTNAME,
	ANNOT_POINT,
	ANNOT_POINTS,
	ANNOT_POPUPOPEN,
	ANNOT_POPUPRECT,
	ANNOT_PRINT,
	ANNOT_READONLY,
	ANNOT_RECT,
	ANNOT_SOUNDICON,
	ANNOT_STROKECOLOR,
	ANNOT_STYLE,
	ANNOT_SUBJECT,
	ANNOT_TEXTFONT,
	ANNOT_TEXTSIZE,
	ANNOT_WIDTH,
	ANNOT_ALIGNMENT,
	ANNOT_VERTICES,
	ANNOT_QUADS,
	ANNOT_NOTEICON,
	ANNOT_OPACITY
};	

struct CFXJS_AnnotObj{
	CFXJS_AnnotObj();
	int					nAnnotType;
	ANNOT_TYPE			nDoAnnotType;
	CFX_WideString		cwType;
	FSAnnot*		pAnnot;
	int					nPageNo;
	int					nAlignment;
	CFX_WideString		cwLineBegin;
	CFX_WideString		cwLineEnd;
	CFX_WideString		cwAttachIcon;
	CFX_WideString		cwAuthor;

	CFX_WideString		cwSymbol;
	CFX_WideString		cwContents;
	CJS_IntArray*		dashArray;
	FX_COLORREF			fillcolor; 
	bool				bHide;
	int					iLeaderExtend; 
	int					iLeaderLength;
	CFX_WideString		cwLineEnding;
	CFX_WideString		cwNoteIcon;
	CFX_WideString		cwAnnotName;
	CPDF_Point			pointArray;
	CJS_PointsArray*	pointsArray;
	bool				bPopupOpen;
	CPDF_Rect				popupArray;
	bool				bPrint;
	bool				bReadOnly;
	CPDF_Rect				rectArray;
	CFX_WideString		cwSoundIcon;
	FX_COLORREF			strokecolor; 
	CFX_WideString		cwStyle;
	CFX_WideString		cwSubject;
	CFX_WideString		cwTextFont;
	int					iTextSize;
	int					iWidth;	
	CJS_PointsArray*	verticesArray;
	CJS_PointsArray*	gesturesArray;
	CJS_PointsArray*	quadsArray;
	float				fOpacity;
	CFX_WideString		cwBorderEffectStyle;
	int					iBorderEffectIntensity;

};

CFXJS_AnnotObj::CFXJS_AnnotObj() :pointArray(0,0)
{
}

class Annotation : public CFXJS_EmbedObj
{
public:
	Annotation(CFXJS_Object* pJSObject);
	virtual ~Annotation();
	
public:
	FX_BOOL alignment(OBJ_PROP_PARAMS);
	FX_BOOL AP(OBJ_PROP_PARAMS);
	FX_BOOL arrowBegin(OBJ_PROP_PARAMS);
	FX_BOOL arrowEnd(OBJ_PROP_PARAMS);
	FX_BOOL attachIcon(OBJ_PROP_PARAMS);
	FX_BOOL author(OBJ_PROP_PARAMS);
	FX_BOOL borderEffectIntensity(OBJ_PROP_PARAMS);
	FX_BOOL borderEffectStyle(OBJ_PROP_PARAMS);
	FX_BOOL callout(OBJ_PROP_PARAMS);
	FX_BOOL caretSymbol(OBJ_PROP_PARAMS);
	FX_BOOL contents(OBJ_PROP_PARAMS);
	FX_BOOL creationDate(OBJ_PROP_PARAMS);
	FX_BOOL dash(OBJ_PROP_PARAMS);
	FX_BOOL delay(OBJ_PROP_PARAMS);
	FX_BOOL doc(OBJ_PROP_PARAMS);
	FX_BOOL doCaption(OBJ_PROP_PARAMS);
	FX_BOOL fillColor(OBJ_PROP_PARAMS);
	FX_BOOL gestures(OBJ_PROP_PARAMS);
	FX_BOOL hidden(OBJ_PROP_PARAMS);
	FX_BOOL inReplyTo(OBJ_PROP_PARAMS);
	FX_BOOL intent(OBJ_PROP_PARAMS);
	FX_BOOL leaderExtend(OBJ_PROP_PARAMS);
	FX_BOOL leaderLength(OBJ_PROP_PARAMS);
	FX_BOOL lineEnding(OBJ_PROP_PARAMS);
	FX_BOOL lock(OBJ_PROP_PARAMS);
	FX_BOOL modDate(OBJ_PROP_PARAMS);
	FX_BOOL name(OBJ_PROP_PARAMS);
	FX_BOOL noteIcon(OBJ_PROP_PARAMS);
	FX_BOOL noView(OBJ_PROP_PARAMS);
	FX_BOOL opacity(OBJ_PROP_PARAMS);
	FX_BOOL page(OBJ_PROP_PARAMS);
	FX_BOOL point(OBJ_PROP_PARAMS);
	FX_BOOL points(OBJ_PROP_PARAMS);
	FX_BOOL popupOpen(OBJ_PROP_PARAMS);
	FX_BOOL popupRect(OBJ_PROP_PARAMS);
	FX_BOOL print(OBJ_PROP_PARAMS);
	FX_BOOL quads(OBJ_PROP_PARAMS);
	FX_BOOL readOnly(OBJ_PROP_PARAMS);
	FX_BOOL rect(OBJ_PROP_PARAMS);
	FX_BOOL refType(OBJ_PROP_PARAMS);
	FX_BOOL richContents(OBJ_PROP_PARAMS);
	FX_BOOL richDefaults(OBJ_PROP_PARAMS);
	FX_BOOL rotate(OBJ_PROP_PARAMS);
	FX_BOOL seqNum(OBJ_PROP_PARAMS);
	FX_BOOL soundIcon(OBJ_PROP_PARAMS);	
	FX_BOOL state(OBJ_PROP_PARAMS);
	FX_BOOL stateModel(OBJ_PROP_PARAMS);
	FX_BOOL strokeColor(OBJ_PROP_PARAMS);
	FX_BOOL style(OBJ_PROP_PARAMS);	
	FX_BOOL subject(OBJ_PROP_PARAMS);
	FX_BOOL textFont(OBJ_PROP_PARAMS);
	FX_BOOL textSize(OBJ_PROP_PARAMS);
	FX_BOOL toggleNoView(OBJ_PROP_PARAMS);
	FX_BOOL type(OBJ_PROP_PARAMS);	
	FX_BOOL vertices(OBJ_PROP_PARAMS);
	FX_BOOL width(OBJ_PROP_PARAMS);	

	FX_BOOL destroy(OBJ_METHOD_PARAMS);

public:
	void					Initial(JDocument* pDocument, FSAnnot* pAnnot);
	JS_ANNOTTYPE			GetAnnotType();

public:
	static void				UpdateAnnot(FSAnnot *pAnnot,FX_BOOL bResetApp, FX_BOOL bRefresh);
	static FX_BOOL			CheckAllow(const CFX_WideString& string);
	

	static FSAnnot*	AddAnnot(FSPDFDoc* pDocument,CFXJS_AnnotObj& annotobj);
	static void				SetAlignment(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,int nAlignment);
	static void				ArrowBegin(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwBegin);
	static void				ArrowEnd(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwEnd);
	static void				AttachIcon(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwAttachIcon);
	static void				Author(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwAuthor);
	static void				BorderEffectStyle(FSPDFDoc* pReaderDoc,FSAnnot* pAnnot,CFX_WideString cwBorderEffectStyle);
	static void				BorderEffectIntensity(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,int iBorderEffectIntensity);
	static void				CallOut(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CJS_PointsArray* calloutArray);
	static void				CaretSymbol(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwSymbol);
	static void				Contents(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwContents);
	static void				Dash(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CJS_IntArray* dashArray);
	static void				FillColor(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,FX_COLORREF fillColo);
	static void				Gestures(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CJS_PointsArray* gesturesArray);
	static void				Hidden(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,bool bHide);
	static void				LeaderExtend(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,int iLeaderExtend);
	static void				LeaderLength(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,int iLeaderLength);
	static void				LineEnding(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwLineEnding);
	static void				Name(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwName);
	static void				NoteIcon(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwNoteIcon);
	static void				Opacity(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,float fOpacity);
	static void				Point(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CPDF_Point pointArray);
	static void				Points(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CJS_PointsArray* pointsArray);
	static void				PopupOpen(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,bool bOpen);
	static void				PopupRect(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CPDF_Rect popupArray);
	static void				BPrint(FSPDFDoc* pReaderDoc,FSAnnot*pAnnot,bool bPrint);
	static void				BReadOnly(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,bool bRead);
	static void				Rect(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CPDF_Rect rectArray);
	static void				SoundIcon(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwSoundIcon);
	static void				StrockColor(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,FX_COLORREF strokecolor);
	static void				SetStyle(FSPDFDoc* pReaderDoc,FSAnnot* pAnnot,CFX_WideString cwStyle);
	static void				Subject(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwSubject);
	static void				TextFont(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CFX_WideString cwTextFont);
	static void				TextSize(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,int iTextSize);
	static void				SetWidth(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,int iWidth);
	static void				Vertices(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CJS_PointsArray* verticesArray);
	static void				Quads(FSPDFDoc* pReaderDoc,FSAnnot *pAnnot,CJS_PointsArray* quadsArray);

	static FX_BOOL			SetStartPoint(FSAnnot *pAnnot,CPDF_Point& point);
	static FX_BOOL			SetEndPoint(FSAnnot *pAnnot,CPDF_Point& point);


	static void				DoDelay(FSPDFDoc* pReaderDoc,CFXJS_AnnotObj *pData);
	void					SetDelay(FX_BOOL bDelay);

	static FSAnnot*			AddLine(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddCircle(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddSquare(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddPolygon(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddPolyline(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddInk(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddUnderline(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddHighlight(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddCaret(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddSquiggly(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddStrikeOut(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddFileAttachment(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	static FSAnnot*			AddText(FSPDFPage* pPage,CFXJS_AnnotObj& annotobj);
	

	

private:
	JDocument*				m_pDocument;
	FSAnnot*			m_pAnnot;
	FX_BOOL					m_bCanSet;
	FX_BOOL					m_bDelay;
};

class CJS_Annotation : public CFXJS_Object
{
public:
	CJS_Annotation(JSFXObject* pObject) : CFXJS_Object(pObject){};
	virtual ~CJS_Annotation(){};

public:
	DECLARE_JS_CLASS(CJS_Annotation);

	JS_STATIC_PROP(alignment, Annotation);
	JS_STATIC_PROP(AP, Annotation);
	JS_STATIC_PROP(arrowBegin, Annotation);
	JS_STATIC_PROP(arrowEnd, Annotation);
	JS_STATIC_PROP(attachIcon, Annotation);
	JS_STATIC_PROP(author, Annotation);
	JS_STATIC_PROP(borderEffectIntensity, Annotation);
	JS_STATIC_PROP(borderEffectStyle, Annotation);
	JS_STATIC_PROP(callout, Annotation);
	JS_STATIC_PROP(caretSymbol, Annotation);
	JS_STATIC_PROP(contents, Annotation);
	JS_STATIC_PROP(creationDate, Annotation);
	JS_STATIC_PROP(dash, Annotation);
	JS_STATIC_PROP(delay, Annotation);
	JS_STATIC_PROP(doc, Annotation);
	JS_STATIC_PROP(doCaption, Annotation);
	JS_STATIC_PROP(fillColor, Annotation);
	JS_STATIC_PROP(gestures, Annotation);
	JS_STATIC_PROP(hidden, Annotation);
	JS_STATIC_PROP(inReplyTo, Annotation);
	JS_STATIC_PROP(intent, Annotation);
	JS_STATIC_PROP(leaderExtend, Annotation);
	JS_STATIC_PROP(leaderLength, Annotation);
	JS_STATIC_PROP(lineEnding, Annotation);
	JS_STATIC_PROP(lock, Annotation);
	JS_STATIC_PROP(modDate, Annotation);
	JS_STATIC_PROP(name, Annotation);
	JS_STATIC_PROP(noteIcon, Annotation);
	JS_STATIC_PROP(noView, Annotation);
	JS_STATIC_PROP(opacity, Annotation);
	JS_STATIC_PROP(page, Annotation);
	JS_STATIC_PROP(point, Annotation);
	JS_STATIC_PROP(points, Annotation);
	JS_STATIC_PROP(popupOpen, Annotation);
	JS_STATIC_PROP(popupRect, Annotation);
	JS_STATIC_PROP(print, Annotation)
	JS_STATIC_PROP(quads, Annotation);
	JS_STATIC_PROP(readOnly, Annotation);
	JS_STATIC_PROP(rect, Annotation);
	JS_STATIC_PROP(refType, Annotation);
	JS_STATIC_PROP(richContents, Annotation);
	JS_STATIC_PROP(richDefaults, Annotation);
	JS_STATIC_PROP(rotate, Annotation);
	JS_STATIC_PROP(seqNum, Annotation);
	JS_STATIC_PROP(soundIcon, Annotation);
	JS_STATIC_PROP(state, Annotation);
	JS_STATIC_PROP(stateModel, Annotation);
	JS_STATIC_PROP(strokeColor, Annotation);
	JS_STATIC_PROP(style, Annotation);	
	JS_STATIC_PROP(subject, Annotation);
	JS_STATIC_PROP(textFont, Annotation);
	JS_STATIC_PROP(textSize, Annotation);
	JS_STATIC_PROP(toggleNoView, Annotation);
	JS_STATIC_PROP(type, Annotation);
	JS_STATIC_PROP(vertices, Annotation);
	JS_STATIC_PROP(width, Annotation);	

	JS_STATIC_METHOD(destroy,Annotation);
};

#endif //_ANNOTATION_H_
