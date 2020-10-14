#ifndef _FS_ANNOT_IMP_H_
#define _FS_ANNOT_IMP_H_

#include "../../include/common.h"
#include "../../include/fs_pdfpage.h"
#include "../../include/fs_pdfobject.h"
#include "../../include/fs_annot.h"
#include "../../include/fs_pdfdoc.h"

#include "../fxcore.h"

#include"fs_annot_util.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_wnd.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_fontmap.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_edit.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_utils.h"
#include "../widget/pdfwindow/include/fx_systemhandlerimp.h"

#include "../widget/ifpdf_widget.h"
#include "../widget/fpwl_edit.h"

class FSPDFPage;
class CFSPDF_AnnotIconProvider;
class FSPDFPathImp;

///////////////////////////////////////////////////////////////////
#define FSANNOT_KEY_LINEENDINGSTYLES			"LE"

#define FSANNOT_STRINGKEY_CONTENT		"Contents"
#define FSANNOT_STRINGKEY_UNIQUEID		"NM"
#define FSANNOT_STRINGKEY_TITLE			"T"
#define FSANNOT_STRINGKEY_SUBJECT		"Subj"
#define FSANNOT_STRINGKEY_STATE			"State"
#define FSANNOT_STRINGKEY_STATEMODEL	"StateModel"
#define FSANNOT_STRINGKEY_DEFAULTAP		"DA"
#define FSANNOT_STRINGKEY_RICHTEXT		"RC"
#define FSANNOT_STRINGKEY_DEFAULTSTYLE	"DS"

#define FSANNOT_BOOLEANKEY_OPEN			"Open"
#define FSANNOT_BOOLEANKEY_CAP			"Cap"

#define FSANNOT_DATETIMEKEY_CREATION	"CreationDate"
#define FSANNOT_DATETIMEKEY_MODIFY		"M"

#define FSANNOT_INTEGERKEY_FLAGS		"F"
#define FSANNOT_INTEGERKEY_ALIGNMENT	"Q"

#define FSANNOT_INTEGERKEY_FXLINETYPE	"FxLineType"

#define FSANNOT_FLOATKEY_OPACITY		"CA"
#define FSANNOT_FLOATKEY_LL				"LL"
#define FSANNOT_FLOATKEY_LLE			"LLE"

#define FSANNOT_RECTKEY_RECT			"Rect"
#define FSANNOT_RECTKEY_RD				"RD"
#define FSANNOT_RECTKEY_BBOX			"BBox"

#define FSANNOT_DICTKEY_BORDEREFFECT	"BE"
#define FSANNOT_DICTKEY_BORDERSTYLE		"BS"
#define FSANNOT_DICTKEY_MK				"MK"
#define FSANNOT_DICTKEY_AP				"AP"
#define FSANNOT_DICTKEY_IRT				"IRT"
#define FSANNOT_DICTKEY_ACTION			"A"
#define FSANNOT_DICTKEY_POPUP			"Popup"
#define FSANNOT_DICTKEY_POPUPPARENT		"Parent"
#define FSANNOT_DICTKEY_RESOURCES		"Resources"
#define FSANNOT_DICTKEY_EXTGSTATE		"ExtGState"

#define FSANNOT_NAMEKEY_SUBTYPE				"Subtype"
#define FSANNOT_NAMEKEY_INTENT				"IT"
#define FSANNOT_NAMEKEY_RT					"RT"
#define FSANNOT_NAMEKEY_ICONNAME			"Name"
#define FSANNOT_NAMEKEY_HIGHLIGHTINGMODE	"H"
#define FSANNOT_NAMEKEY_SYMBOL				"Sy"
#define FSANNOT_NAMEKEY_CAPPOSITIONING		"CP"

#define FSANNOT_ARRAYKEY_BORDER					"Border"
#define FSANNOT_ARRAYKEY_QUADPOINTS				"QuadPoints"
#define FSANNOT_ARRAYKEY_CALLOUTLINE			"CL"
#define FSANNOT_ARRAYKEY_VERTICES				"Vertices"
#define FSANNOT_ARRAYKEY_INKLIST				"InkList"
#define FSANNOT_ARRAYKEY_LINEPOINTS				"L"
#define FSANNOT_ARRAYKEY_CAPOFFSET				"CO"
#define FSANNOT_ARRAYKEY_MATRIX					"Matrix"

#define FSANNOT_COLORKEY_BORDERCOLOR		"C"
#define FSANNOT_COLORKEY_FILLCOLOR			"IC"

#define FSANNOT_STREAMKEY_NORMALAP			"N"

//macros for border effect of annotation
#define FSANNOT_BORDEREFFECT_STYLE	0
#define FSANNOT_BORDEREFFECT_CLOUDY	1

class FSPDFAnnotImp: public FSAnnot, public CFX_Object
{
public:
	// Functions for release, in FSAnnot
	virtual ~FSPDFAnnotImp();
	virtual FSPDFPage*			getPage() const {return m_pFSPDFPage;}
	virtual FS_BOOL				isMarkup() const;
	virtual FSM_ANNOTTYPE		getType() const;
	virtual FS_INT32			getIndex() const;
	virtual FSString			getContent() const;
	virtual void				setContent(const char* content);
	virtual FSDateTime			getModifiedDateTime() const;
	virtual void				setModifiedDateTime(FSDateTime dataTime);
	virtual FS_DWORD			getFlags() const;
	virtual void				setFlags(FS_DWORD flags);
	virtual FSString			getUniqueID() const;
	virtual void				setUniqueID(const char* uniqueID);
	virtual FSRectF				getRect() const;
	virtual FSBorderInfo		getBorderInfo() const;
	virtual void				setBorderInfo(FSBorderInfo border);
	virtual FS_ARGB				getBorderColor() const;
	virtual void				setBorderColor(FS_ARGB color);
	virtual FSRectI				getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix);
	virtual FSPDFDictionary*	getDict() const;
	virtual FS_BOOL				removeProperty(FSM_ANNOTPROPERTY property);
	// FSPDFAnnotImp is directly used to represent annotations which don't have their own class.
	// And these annotations cannot be reset AP or be moved
	// So here, return FALSE directly in move() and resetap() -- these two functions need to be implementated in concrete annot classes.
	virtual FS_BOOL				move(FSRectF rect) { return FALSE;}
	virtual FS_BOOL				resetAppearanceStream() { return FALSE; }

	// Internal use
	FSPDFAnnotImp();
	FSPDFAnnotImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	void				init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);

	FS_BOOL				hasProperty(FX_BSTR key) const;
	FSRectF				getRotateRect();
	FS_INT32			getRotation();
	FS_ARGB				getColor(FS_BOOL bFill) const;
	void				setColor(FS_ARGB color, FS_BOOL bFill);
	FSString			getString(FX_BSTR key) const;
	CFX_ByteString		getStringImpl(FX_BSTR key) const; // returned bytestring is in UTF-8 encoding.
	CFX_WideString		getUnicodeString(FX_BSTR key) const;
	void				setString(FX_BSTR key, const char* value);
	void				setName(FX_BSTR key, const char* nameValue);
	FSString			getName(FX_BSTR key) const;

	CFX_Matrix			getMatrix(FX_BSTR bsKey) const;
	void				setMatrix(FX_BSTR bsKey, const CFX_Matrix& mt);
	FSDateTime			getDateTime(FX_BSTR key) const;
	void				setDateTime(FX_BSTR key, FSDateTime dateTime);
	FS_INT32			getInteger(FX_BSTR key) const;
	void				setInteger(FX_BSTR key, FS_INT32 value);
	FS_FLOAT			getFloat(FX_BSTR key) const;
	void				setFloat(FX_BSTR key, FS_FLOAT value);
	FSRectF				getRect(FX_BSTR key) const;
	CFX_FloatRect		getRectImp(FX_BSTR key) const;
	void				setRect(FX_BSTR key, FSRectF rectF);
	void				setRectImp(FX_BSTR key, CFX_FloatRect fxRectF);
	CPDF_Dictionary*	getSubDict(FX_BSTR key, FS_BOOL bCreate = FALSE) const;
	CPDF_Array*			getArray(FX_BSTR key, FS_BOOL bCreate = FALSE) const;
	FS_BOOL				setArray(FX_BSTR key, CPDF_Array* pArray);
	FS_BOOL				getBoolean(FX_BSTR key) const;
	void				setBoolean(FX_BSTR key, FS_BOOL bStatus);
	FS_BOOL				getBorderColorImp(FS_ARGB& color/*, FX_BOOL isFromAP = TRUE*/) const;
	void				setBorderColorImp(FS_ARGB color);
	FSM_BORDERSTYLE		getBorderStyle() const;
	FS_FLOAT			getBorderWidth() const;
	void				getBorderDash(FX_FLOAT& dashOffset, CFX_FloatArray& dashPatternArray) const;
	FS_INT32			getBorderEffect() const;
	FX_FLOAT			getCloudyIntensity() const;
	FS_BOOL				getFillColor(FS_ARGB& color) const;
	void				setFillColor(FS_ARGB color);
	FS_INT32			getQuadPointsCount() const;
	FSQuadPoints		getQuadpoints(FS_INT32 index) const;
	FS_BOOL				getQuadPointsImpl(CFX_PointsF &quadPoints) const;
	void				setQuadPoints(const FSQuadPoints* quadPoints, FS_INT32 count);
	FS_BOOL				setQuadPointsImpl(const CFX_PointsF &quadPoints);
	FS_BOOL				getColorImpl(FX_BSTR key, FS_ARGB& color) const;
	void				setColorImpl(FX_BSTR bsKey, FS_ARGB color);
	CPDF_Rect			getInnerRectImp();
	void				setRectMargin(FSRectF innerRect);
	FS_BOOL				getLineEndingStyles(CFX_ByteString& startStyle, CFX_ByteString& endStyle);
	void				setLineEndingStyles(const char* newStartStyle, const char* newEndStyle);
	FS_BOOL				getVertexesImp(CFX_PointsF &vertexes);
	FS_BOOL				setVerticesImp(CFX_PointsF vertexes);

	static FS_BOOL		moveImp(FSAnnot* pAnnot, FSRectF rect, FS_BOOL isResetAp);
	// Get the matrix used to transform from current /Rect to the new rect.
	FS_BOOL				getTransformMatrix(FSRectF newRect, CPDF_Matrix& matrix);
	FS_BOOL				transformVertices(FSRectF newRect);
	FS_BOOL				transformQuadPoints(FSRectF newRect);


	virtual	FX_BOOL		drawAppearance(CFX_RenderDevice* pDevice, const CFX_Matrix* pUser2Device, FX_BSTR bsMode, CPDF_RenderOptions* pOptions);
	virtual FX_BOOL		drawInContext(const CPDF_Page* pPage, const CPDF_RenderContext* pContext, const CFX_AffineMatrix* pUser2Device, CPDF_Annot::AppearanceMode mode);
	void				clearCachedAppearance();
	CPDF_Form*			getAPForm(FX_BSTR mode);
	CPDF_Stream*		writeAppearance(FX_BSTR bsAPType, const CFX_FloatRect& rcBBox, const CFX_Matrix& matrix, CFX_ByteTextBuf& bsContents, FX_BSTR bsAPState, FSPDFANNOT_APPPARAM* pParam = NULL);
	
	CPDF_Dictionary*	getPDFDict() const { return m_pAnnotDict; }
	void				setModified();
	CPDF_Matrix			getRotateMatrix(FS_INT32 rotate, CFX_FloatRect rect);
	CPDF_Matrix			getRotateMatrix();
	CPDF_Rect			getLineEndingStyleStream(FS_BOOL bStartStyle, CFX_ByteString& csAP, FSPointF& point, FSPointF& direction, FX_FLOAT& fWidth, FS_BOOL& bNeedFill);
	//FS_BOOL				isSetFreeTextBorderColor() { return m_bSetFreeTextBorderColor; }
	//void				setFreeTextBorderColorFlag(FS_BOOL bSet) { m_bSetFreeTextBorderColor = bSet; }

	CPDF_Action			GetAction() const;
	CPDF_AAction		GetAAction() const;
	CPDF_Action			GetAAction(CPDF_AAction::AActionType eAAT);

	FS_INT32 getVertexCount();
	FSPointF getVertex(FS_INT32 index);
	void setVertexes(const FSPointF* vertexes, FS_INT32 count);
	FSPDFAnnot_Path* getVerticesImp();
protected:
	CPDF_Dictionary*	m_pAnnotDict;
	FSPDFPage*			m_pFSPDFPage;
	CFX_MapPtrToPtr		m_APMap;
	//FS_BOOL				m_bSetFreeTextBorderColor;

private:
	void				setBorderDash(FX_FLOAT dashOffset, const CFX_FloatArray &dashPatternArray);
	FS_BOOL				setBorderInfo2Border(FSBorderInfo border);
	FS_BOOL				setBorderInfo2BorderStyleEffect(FSBorderInfo border);
};

template<typename IAnnotation> class tFSPDFAnnotImp : public IAnnotation, public FSPDFAnnotImp 
{
public:
	virtual ~tFSPDFAnnotImp(){}
	// move()/resetap() need to be implemented in concrete annot class.
	virtual FS_BOOL				move(FSRectF rect) { return FSPDFAnnotImp::move(rect); } 
	virtual FS_BOOL				resetAppearanceStream() { return FSPDFAnnotImp::resetAppearanceStream(); } 
	virtual FSPDFPage*			getPage() const { return FSPDFAnnotImp::getPage();}
	virtual FS_BOOL				isMarkup() const { return FSPDFAnnotImp::isMarkup();}
	virtual FSM_ANNOTTYPE		getType() const { return FSPDFAnnotImp::getType();}
	virtual FS_INT32			getIndex() const{ return FSPDFAnnotImp::getIndex(); }
	virtual FSString			getContent() const { return FSPDFAnnotImp::getContent(); }
	virtual void				setContent(const char* content) { FSPDFAnnotImp::setContent(content); }
	virtual FSDateTime			getModifiedDateTime() const { return FSPDFAnnotImp::getModifiedDateTime(); }
	virtual void				setModifiedDateTime(FSDateTime dataTime) { FSPDFAnnotImp::setModifiedDateTime(dataTime); }
	virtual FS_DWORD			getFlags() const { return FSPDFAnnotImp::getFlags(); }
	virtual void				setFlags(FS_DWORD flags) { FSPDFAnnotImp::setFlags(flags); }
	virtual FSString			getUniqueID() const { return FSPDFAnnotImp::getUniqueID(); }
	virtual void				setUniqueID(const char* uniqueID) { FSPDFAnnotImp::setUniqueID(uniqueID); }
	virtual FSRectF				getRect() const { return FSPDFAnnotImp::getRect(); }
	virtual FSBorderInfo		getBorderInfo() const { return FSPDFAnnotImp::getBorderInfo(); }
	virtual void				setBorderInfo(FSBorderInfo border) { FSPDFAnnotImp::setBorderInfo(border); }
	virtual FS_ARGB				getBorderColor() const { return FSPDFAnnotImp::getBorderColor(); }
	virtual void				setBorderColor(FS_ARGB color) { FSPDFAnnotImp::setBorderColor(color); }
	virtual FSRectI				getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix) { return FSPDFAnnotImp::getDeviceRect(bTransformIcon, matrix); }
	virtual FSPDFDictionary*	getDict() const { return FSPDFAnnotImp::getDict(); }
	virtual FS_BOOL				removeProperty(FSM_ANNOTPROPERTY property) { return FSPDFAnnotImp::removeProperty(property); }
};

class FSPDFMarkupImp : public tFSPDFAnnotImp<FSMarkup>
{
public:
	virtual ~FSPDFMarkupImp() {}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	// FSPDFMarkupImp is directly used to represent annotations which don't have their own class.
	// And these annotations cannot be reset AP or be moved.
	// So here, return FALSE directly for move() and resetap()
	virtual FS_BOOL			move(FSRectF rect) {return FALSE; }
	virtual FS_BOOL			resetAppearanceStream() {return FALSE; }

	// Functions for release, in FSMarkup
	virtual FSPopup*		getPopup();
	virtual void			setPopup(FSPopup* popup);
	virtual FS_BOOL			isMarkup() const { return TRUE; }
	virtual FSString		getTitle() const;
	virtual void			setTitle(const char* title);
	virtual FSString		getSubject() const;
	virtual void			setSubject(const char* subject);
	virtual FS_FLOAT		getOpacity() const;
	virtual void			setOpacity(FS_FLOAT opacity);
	virtual FSString		getIntent() const;
	virtual void			setIntent(const char* intent);
	virtual FSDateTime		getCreationDateTime() const;
	virtual void			setCreationDateTime(FSDateTime dateTime);
	virtual FS_INT32		getReplyCount();
	virtual FSNote*			getReply(FS_INT32 index) const;
	virtual FSNote*			addReply();
	virtual FS_BOOL			removeReply(FS_INT32 index);
	virtual FS_BOOL			removeAllReplies();
	virtual FS_BOOL			isGrouped();
	virtual FSMarkup*		getGroupHeader();
	virtual FS_INT32		getGroupElementCount();
	virtual FSMarkup*		getGroupElement(FS_INT32 index);
	virtual FS_BOOL			ungroup();
	virtual FS_INT32		getStateAnnotCount(FSM_ANNOTSTATEMODEL stateModel);
	virtual FSNote*			getStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FS_INT32 index);
	virtual FSNote*			addStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FSM_ANNOTSTATE state);
	virtual FS_BOOL			removeAllStateAnnots();

	// Internal use
	FSPDFMarkupImp() {}
	FSPDFMarkupImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	void init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	CPDF_Dictionary*	getPDFDict() const { return (CPDF_Dictionary*)getDict(); }
	FSPDFAnnotImp		getAnnotImp() const { return *this; }
private:
	FS_BOOL replyImpl(FS_INT32 index, FS_INT32 implType, FSNote* &note, FS_INT32& count) const;
	FSNote* getReplyImpl(CPDF_Dictionary* pReplyAnnotDict, CPDF_AnnotList& annotList, FS_INT32 index) const;
	FS_BOOL removeReplyImpl(FS_INT32 replyIndex, FS_BOOL bRemoveAll);

	FS_BOOL				isGroupHeader();
	CPDF_Dictionary*	getGroupHeaderDict();

	// index=-1 means to get the last state annot.
	FSNote*	getStateAnnotImp(FSM_ANNOTSTATEMODEL model, FS_INT32 index);
};

template<typename IMarkup> class tFSPDFMarkupImp : public IMarkup, public FSPDFMarkupImp
{
public:
	virtual ~tFSPDFMarkupImp(){}
	// Inherited from FSAnnot
	// Not to implement move()/resetap() in this template, but implement them in concrete annot class.
	virtual FS_BOOL				move(FSRectF rect) { return FSPDFAnnotImp::move(rect); } 
	virtual FS_BOOL				resetAppearanceStream() { return FSPDFAnnotImp::resetAppearanceStream(); } 
	virtual FSPDFPage*			getPage() const { return FSPDFAnnotImp::getPage();}
	virtual FS_BOOL				isMarkup() const { return TRUE;}
	virtual FSM_ANNOTTYPE		getType() const { return FSPDFAnnotImp::getType();}
	virtual FS_INT32			getIndex() const{ return FSPDFAnnotImp::getIndex(); }
	virtual FSString			getContent() const { return FSPDFAnnotImp::getContent(); }
	virtual void				setContent(const char* content) { FSPDFAnnotImp::setContent(content); }
	virtual FSDateTime			getModifiedDateTime() const { return FSPDFAnnotImp::getModifiedDateTime(); }
	virtual void				setModifiedDateTime(FSDateTime dataTime) { FSPDFAnnotImp::setModifiedDateTime(dataTime); }
	virtual FS_DWORD			getFlags() const { return FSPDFAnnotImp::getFlags(); }
	virtual void				setFlags(FS_DWORD flags) { FSPDFAnnotImp::setFlags(flags); }
	virtual FSString			getUniqueID() const { return FSPDFAnnotImp::getUniqueID(); }
	virtual void				setUniqueID(const char* uniqueID) { FSPDFAnnotImp::setUniqueID(uniqueID); }
	virtual FSRectF				getRect() const { return FSPDFAnnotImp::getRect(); }
	virtual FSBorderInfo		getBorderInfo() const { return FSPDFAnnotImp::getBorderInfo(); }
	virtual void				setBorderInfo(FSBorderInfo border) { FSPDFAnnotImp::setBorderInfo(border); }
	virtual FS_ARGB				getBorderColor() const { return FSPDFAnnotImp::getBorderColor(); }
	virtual void				setBorderColor(FS_ARGB color) { FSPDFAnnotImp::setBorderColor(color); }
	virtual FSRectI				getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix) { return FSPDFAnnotImp::getDeviceRect(bTransformIcon, matrix); }
	virtual FSPDFDictionary*	getDict() const { return FSPDFAnnotImp::getDict(); }
	virtual FS_BOOL				removeProperty(FSM_ANNOTPROPERTY property) { return FSPDFAnnotImp::removeProperty(property); }

	// Inherited from FSMarkup
	virtual FSPopup*			getPopup(){return FSPDFMarkupImp::getPopup();}
	virtual void				setPopup(FSPopup* popup){ FSPDFMarkupImp::setPopup(popup); }
	virtual FSString			getTitle() const{return  FSPDFMarkupImp::getTitle();}
	virtual void				setTitle(const char* title) { FSPDFMarkupImp::setTitle(title); }
	virtual						FSString getSubject() const { return FSPDFMarkupImp::getSubject(); }
	virtual void				setSubject(const char* subject) { return FSPDFMarkupImp::setSubject(subject); }
	virtual	FS_FLOAT			getOpacity() const { return FSPDFMarkupImp::getOpacity(); }
	virtual void				setOpacity(FS_FLOAT opacity) { FSPDFMarkupImp::setOpacity(opacity); }
	virtual FSString			getIntent() const { return FSPDFMarkupImp::getIntent(); }
	virtual void				setIntent(const char* intent) { FSPDFMarkupImp::setIntent(intent); }
	virtual FSDateTime			getCreationDateTime() const { return FSPDFMarkupImp::getCreationDateTime(); }
	virtual void				setCreationDateTime(FSDateTime dateTime) { FSPDFMarkupImp::setCreationDateTime(dateTime); }
	virtual FS_INT32			getReplyCount() { return FSPDFMarkupImp::getReplyCount(); }
	virtual FSNote*				getReply(FS_INT32 index) const { return FSPDFMarkupImp::getReply(index); }
	virtual FSNote*				addReply() { return FSPDFMarkupImp::addReply(); }
	virtual FS_BOOL				removeReply(FS_INT32 index){ return FSPDFMarkupImp::removeReply(index); }
	virtual FS_BOOL				removeAllReplies() { return FSPDFMarkupImp::removeAllReplies(); }
	virtual FS_BOOL				isGrouped() { return FSPDFMarkupImp::isGrouped(); }
	virtual FSMarkup*			getGroupHeader() { return FSPDFMarkupImp::getGroupHeader(); }
	virtual FS_INT32			getGroupElementCount() { return FSPDFMarkupImp::getGroupElementCount(); }
	virtual FSMarkup*			getGroupElement(FS_INT32 index) { return FSPDFMarkupImp::getGroupElement(index); }
	virtual FS_BOOL				ungroup()  { return FSPDFMarkupImp::ungroup(); }
	virtual FS_INT32			getStateAnnotCount(FSM_ANNOTSTATEMODEL stateModel) { return FSPDFMarkupImp::getStateAnnotCount(stateModel); }
	virtual FSNote*				getStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FS_INT32 index) { return FSPDFMarkupImp::getStateAnnot(stateModel, index); }
	virtual FSNote*				addStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FSM_ANNOTSTATE state) { return FSPDFMarkupImp::addStateAnnot(stateModel, state); }
	virtual FS_BOOL				removeAllStateAnnots() { return FSPDFMarkupImp::removeAllStateAnnots(); }
};

class FSPDFNoteImp : public tFSPDFMarkupImp<FSNote>
{
public:
	virtual ~FSPDFNoteImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotNote; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	virtual FSMarkup*			getReplyTo();
	virtual FS_BOOL				getOpenStatus() const;
	virtual void				setOpenStatus(FS_BOOL status);
	virtual FSString			getIconName() const ;
	virtual void				setIconName(const char* iconName);
	virtual FS_BOOL				isStateAnnot();
	virtual FSM_ANNOTSTATEMODEL	getStateModel();
	virtual FSM_ANNOTSTATE		getState();
	virtual void				setState( FSM_ANNOTSTATE state);

	// Internal use
	FSPDFNoteImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL isReply();
};

class FSPDFTextMarkupImp : public tFSPDFMarkupImp<FSTextMarkup>
{
public:
	virtual ~FSPDFTextMarkupImp() {}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	// FSPDFTextMarkupImp is directly used to represent annotations which don't have their own class.
	// And these annotations cannot be reset AP or be moved.
	// So here, return FALSE directly for move() and resetap()
	virtual FS_BOOL			move(FSRectF rect) {return FALSE;}
	virtual FS_BOOL			resetAppearanceStream() {return FALSE;}

	// Functions for release in FSTextMarkup
	virtual FS_INT32		getQuadPointsCount() const;
	virtual FSQuadPoints	getQuadPoints(FS_INT32 index) const;
	virtual void			setQuadPoints(const FSQuadPoints* quadPoints,FS_INT32 count);

	// Internal use
	FSPDFTextMarkupImp() {}
	FSPDFTextMarkupImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	void					init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);

	FS_BOOL initParam(FX_BSTR bsBM,FSPDFANNOT_APPPARAM& param,CFX_ByteTextBuf& bufContent);
	FS_BOOL	setColorToAPStream(FS_BOOL bFill, CFX_ByteTextBuf& bufContent);
	FS_BOOL	setQuadPointsToAPStream(FSM_ANNOTTYPE annotType, CFX_ByteTextBuf& bufContent);
};

template<typename ITextMarkup> class tFSPDFTextMarkupImp : public ITextMarkup, public FSPDFTextMarkupImp 
{
public:
	virtual ~tFSPDFTextMarkupImp(){}

	// Inherited from FSAnnot
	// Not to implement move()/resetap() in this template, but implement them in concrete annot class.
	virtual FS_BOOL				move(FSRectF rect) { return FSPDFAnnotImp::move(rect); } 
	virtual FS_BOOL				resetAppearanceStream() { return FSPDFAnnotImp::resetAppearanceStream(); } 
	virtual FSPDFPage*			getPage() const { return FSPDFAnnotImp::getPage();}
	virtual FS_BOOL				isMarkup() const { return TRUE;}
	virtual FSM_ANNOTTYPE		getType() const { return FSPDFAnnotImp::getType();}
	virtual FS_INT32			getIndex() const{ return FSPDFAnnotImp::getIndex(); }
	virtual FSString			getContent() const { return FSPDFAnnotImp::getContent(); }
	virtual void				setContent(const char* content) { FSPDFAnnotImp::setContent(content); }
	virtual FSDateTime			getModifiedDateTime() const { return FSPDFAnnotImp::getModifiedDateTime(); }
	virtual void				setModifiedDateTime(FSDateTime dataTime) { FSPDFAnnotImp::setModifiedDateTime(dataTime); }
	virtual FS_DWORD			getFlags() const { return FSPDFAnnotImp::getFlags(); }
	virtual void				setFlags(FS_DWORD flags) { FSPDFAnnotImp::setFlags(flags); }
	virtual FSString			getUniqueID() const { return FSPDFAnnotImp::getUniqueID(); }
	virtual void				setUniqueID(const char* uniqueID) { FSPDFAnnotImp::setUniqueID(uniqueID); }
	virtual FSRectF				getRect() const { return FSPDFAnnotImp::getRect(); }
	virtual FSBorderInfo		getBorderInfo() const { return FSPDFAnnotImp::getBorderInfo(); }
	virtual void				setBorderInfo(FSBorderInfo border) { FSPDFAnnotImp::setBorderInfo(border); }
	virtual FS_ARGB				getBorderColor() const { return FSPDFAnnotImp::getBorderColor(); }
	virtual void				setBorderColor(FS_ARGB color) { FSPDFAnnotImp::setBorderColor(color); }
	virtual FSRectI				getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix* matrix) { return FSPDFAnnotImp::getDeviceRect(bTransformIcon, matrix); }
	virtual FSPDFDictionary*	getDict() const { return FSPDFAnnotImp::getDict(); }
	virtual FS_BOOL				removeProperty(FSM_ANNOTPROPERTY property) { return FSPDFAnnotImp::removeProperty(property); }

	virtual FSPopup*			getPopup(){return FSPDFMarkupImp::getPopup();}
	virtual void				setPopup(FSPopup* popup){ FSPDFMarkupImp::setPopup(popup); }
	virtual FSString			getTitle() const{return  FSPDFMarkupImp::getTitle();}
	virtual void				setTitle(const char* title) { FSPDFMarkupImp::setTitle(title); }
	virtual FSString			getSubject() const { return FSPDFMarkupImp::getSubject(); }
	virtual void				setSubject(const char* subject) { return FSPDFMarkupImp::setSubject(subject); }
	virtual	FS_FLOAT			getOpacity() const { return FSPDFMarkupImp::getOpacity(); }
	virtual void				setOpacity(FS_FLOAT opacity) { FSPDFMarkupImp::setOpacity(opacity); }
	virtual FSString			getIntent() const { return FSPDFMarkupImp::getIntent(); }
	virtual void				setIntent(const char* intent) { FSPDFMarkupImp::setIntent(intent); }
	virtual FSDateTime			getCreationDateTime() const { return FSPDFMarkupImp::getCreationDateTime(); }
	virtual void				setCreationDateTime(FSDateTime dateTime) { FSPDFMarkupImp::setCreationDateTime(dateTime); }
	virtual FS_INT32			getReplyCount() { return FSPDFMarkupImp::getReplyCount(); }
	virtual FSNote*				getReply(FS_INT32 index) const { return FSPDFMarkupImp::getReply(index); }
	virtual FSNote*				addReply() { return FSPDFMarkupImp::addReply(); }
	virtual FS_BOOL				removeReply(FS_INT32 index){ return FSPDFMarkupImp::removeReply(index); }
	virtual FS_BOOL				removeAllReplies() { return FSPDFMarkupImp::removeAllReplies(); }
	virtual FS_BOOL				isGrouped() { return FSPDFMarkupImp::isGrouped(); }
	virtual FSMarkup*			getGroupHeader() { return FSPDFMarkupImp::getGroupHeader(); }
	virtual FS_INT32			getGroupElementCount() { return FSPDFMarkupImp::getGroupElementCount(); }
	virtual FSMarkup*			getGroupElement(FS_INT32 index) { return FSPDFMarkupImp::getGroupElement(index); }
	virtual FS_BOOL				ungroup()  { return FSPDFMarkupImp::ungroup(); }
	virtual FS_INT32			getStateAnnotCount(FSM_ANNOTSTATEMODEL stateModel) { return FSPDFMarkupImp::getStateAnnotCount(stateModel); }
	virtual FSNote*				getStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FS_INT32 index) { return FSPDFMarkupImp::getStateAnnot(stateModel, index); }
	virtual FSNote*				addStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FSM_ANNOTSTATE state) { return FSPDFMarkupImp::addStateAnnot(stateModel, state); }
	virtual FS_BOOL				removeAllStateAnnots() { return FSPDFMarkupImp::removeAllStateAnnots(); }

	virtual FS_INT32			getQuadPointsCount() const{ return FSPDFTextMarkupImp::getQuadPointsCount(); }
	virtual FSQuadPoints		getQuadPoints(FS_INT32 index) const { return FSPDFTextMarkupImp::getQuadPoints(index); }
	virtual void				setQuadPoints(const FSQuadPoints* quadPoints, FS_INT32 count) { FSPDFTextMarkupImp::setQuadPoints(quadPoints, count); }
};

class FSPDFHighlightImp : public tFSPDFTextMarkupImp<FSHighlight>
{
public:
	virtual ~FSPDFHighlightImp(){};
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotHighlight; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Internal use
	FSPDFHighlightImp() {}
	FSPDFHighlightImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf & bufContent);
};

class FSPDFUnderlineImp : public tFSPDFTextMarkupImp<FSUnderline>
{
public:
	virtual ~FSPDFUnderlineImp(){};
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotUnderline; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Internal use
	FSPDFUnderlineImp() {}
	FSPDFUnderlineImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf & bufContent);
};

class FSPDFStrikeOutImp : public tFSPDFTextMarkupImp<FSStrikeOut>
{
public:
	virtual ~FSPDFStrikeOutImp(){};
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotStrikeOut; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Internal use
	FSPDFStrikeOutImp() {}
	FSPDFStrikeOutImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf & bufContent);
};

class FSPDFSquigglyImp : public tFSPDFTextMarkupImp<FSSquiggly>
{
public:
	virtual ~FSPDFSquigglyImp(){};
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotSquiggly; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Internal use
	FSPDFSquigglyImp() {}
	FSPDFSquigglyImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf & bufContent);
};

class FSPDFLinkImp: public tFSPDFAnnotImp<FSLink>
{
public:
	virtual ~FSPDFLinkImp();
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotLink; }
	virtual FS_BOOL				isMarkup() const{ return FALSE; }
	virtual FS_BOOL				move(FSRectF rect);	
	virtual FS_BOOL				resetAppearanceStream();

	// Functions for release in FSLink
	virtual FS_INT32				getQuadPointsCount() const;
	virtual FSQuadPoints			getQuadPoints(FS_INT32 index) const;
	virtual void					setQuadPoints(const FSQuadPoints* quadPoints, FS_INT32 nCount);
	virtual FSM_HIGHLIGHTINGMODE	getHighlightingMode();
	virtual void					setHighlightingMode(FSM_HIGHLIGHTINGMODE mode);
	virtual FSAction*				getAction();
	virtual void					setAction(FSAction* action);
	virtual FS_BOOL					removeAction();

	// Internal use
	FSPDFLinkImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
private:
	FSAction*			m_pLinkAct;
};

class FSPDFSquareImp : public tFSPDFMarkupImp<FSSquare>
{
public:
	virtual ~FSPDFSquareImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE	getType() const {return FSAnnot::e_annotSquare;}
	virtual FS_BOOL			move(FSRectF rect);
	virtual FS_BOOL			resetAppearanceStream();

	// Functions for release in FSSquare
	virtual FS_ARGB			getFillColor() const;
	virtual void			setFillColor(FS_ARGB fillColor);
	virtual FSRectF			getInnerRect() const;
	virtual void			setInnerRect(FSRectF innerRect);

	// Internal use
	FSPDFSquareImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
};

class FSPDFCircleImp : public tFSPDFMarkupImp<FSCircle>
{
public:
	virtual ~FSPDFCircleImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const {return FSAnnot::e_annotCircle;}
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Functions for release in FSCircle
	virtual FS_ARGB		getFillColor() const;
	virtual void		setFillColor(FS_ARGB fillColor);
	virtual FSRectF		getInnerRect() const;
	virtual void		setInnerRect(FSRectF innerRect);

	// Internal use
	FSPDFCircleImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
};

#define FT_OPACITY_ALIAS			"FoxitSDKFreetext"
#define FT_KNEE_DEFLENGTH			10.0f

class FSPDFFreeTextImp : public tFSPDFMarkupImp<FSFreeText>
{
public:
	virtual ~FSPDFFreeTextImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotFreeText; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Function for release in FSFreeText
	virtual FS_ARGB				getFillColor() const;
	virtual void				setFillColor(FS_ARGB fillColor);
	virtual FS_INT32			getAlignment() const;
	virtual void				setAlignment(FS_INT32 alignment);
	virtual FSRectF				getInnerRect() const;
	virtual void				setInnerRect(FSRectF innerRect);
	virtual FSDefaultAppearance	getDefaultAppearance();
	virtual FS_BOOL				setDefaultAppearance(FSDefaultAppearance defaultAP);
	virtual FSString			getCalloutLineEndingStyle() const;
	virtual void				setCalloutLineEndingStyle(const char* endingStyle);
	virtual FS_INT32			getCalloutLinePointCount() const ;
	virtual FSPointF			getCalloutLinePoint(FS_INT32 index) const;
	virtual void				setCalloutLinePoints(FSPointF point1, FSPointF point2, FSPointF point3);

	// Internal use
	FSPDFFreeTextImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	CPDF_Font*	getPDFFont(CFX_ByteString sFontName);
	FS_BOOL resetAppearance_TW(FX_BOOL bTextOverflow);
	void readyAppearance_CO(FX_BOOL bTextOverflow);
	FS_BOOL resetAppearance_CO(FX_BOOL bTextOverflow);
	FS_BOOL resetAppearance_TB(FX_BOOL bTextOverflow);
	void setStreamExtGState(CPDF_Stream* pStream, const CFX_ByteString& sExtAlias, FX_FLOAT fOpacity);
	void transformRectWithTextMatrix(CPDF_Rect& rcText);

	FS_BOOL hasKneePoint();
	void updateCalloutPoints(CPDF_Rect rectBody, FSPointF startPoint, FX_FLOAT fKneeLength, 
							FSPointF& pKneePoint, FSPointF& endPoint);
	FS_FLOAT getKneeLength() const;
	void adjustPosition(const CPDF_Matrix& mt, const CPDF_Rect& rcBBoxOld, const CPDF_Rect& rcBBoxNew);
	FS_BOOL correctCallOutTextArea(const CPDF_Rect& rcBBox, const CPDF_Rect& rcRD);
	CPDF_Rect getBBoxByTextRect(const CPDF_Rect& rcText, FX_BOOL bCalculateKnee);
	CPDF_Rect getTextRect() const;
	FS_BOOL getTextColor(FS_ARGB& textColor);
	FS_BOOL getFontInfo(CFX_ByteString& fontName, FS_FLOAT& fontSize);
};

class CFT_Edit : public IPWL_Provider, public IPWL_FocusHandler
{
public:
	CFT_Edit(FSPDFFreeTextImp* pAnnot, FX_BOOL bTextOverflow);
	IFSPDF_Edit*             m_pEdit;
	FX_BOOL					 m_bRichText;
	virtual ~CFT_Edit();

public: //IPWL_Provider

	virtual CPDF_Matrix		GetWindowMatrix(void* pAttachedData);

	virtual CFX_WideString	LoadPopupMenuString(FX_INT32 nIndex);

	virtual FX_BOOL			CanUndo(CPWL_Edit* pEdit);
	virtual FX_BOOL			CanRedo(CPWL_Edit* pEdit);

	virtual void			Undo(CPWL_Edit* pEdit);
	virtual void			Redo(CPWL_Edit* pEdit);

public: //IPWL_FocusHandler

	virtual void			OnSetFocus(CPWL_Wnd* pWnd);
	virtual void			OnKillFocus(CPWL_Wnd* pWnd);

public:
	IFX_SystemHandler*		GetSystemHandler();

	FSAnnot*				GetAnnot() const;

	CPDF_Rect				GetTextBBox() const;
	CFX_ByteString			GetAppearanceStream() const;
	// 
	CPDF_Matrix				GetCurMatrix() const;
	CPDF_Matrix				GetTextMatrix() const;

	CPDF_Point				GetEditOrigin() const;
	CPDF_Rect				GetEditBBox(FX_BOOL bTextOverflow) ;
protected:
	CPDF_Rect				GetTWEditBBox(FX_BOOL bTextOverflow) ;
	CPDF_Rect				GetTBCOEditBBox(FX_BOOL bTextOverflow) ;
private:
	FSPDFFreeTextImp*		m_pFreeTextAnnot;	
	//CBA_SpellCheck*			m_pSpellCheck;
	CPWL_FontMap*			m_pFontMap;
	FX_BOOL					m_bModify;
	IFX_SystemHandler*		m_pSystemHandler;
};

class FSPDFLineImp : public tFSPDFMarkupImp<FSLine>
{
public:
	virtual ~FSPDFLineImp() {}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE	getType() const { return FSAnnot::e_annotLine; }
	virtual FS_BOOL			move(FSRectF rect);
	virtual FS_BOOL			resetAppearanceStream();

	// Inherited from FSMarkup and need to be implemented in the derive class
	virtual void		setIntent(const char* intent);

	// Functions for release in FSLine
	// LE related
	virtual FSString	getLineStartingStyle() const;
	virtual void		setLineStartingStyle(const char *startingStyle);
	virtual FSString	getLineEndingStyle() const;
	virtual void		setLineEndingStyle(const char *endingStyle);
	virtual	FS_ARGB		getStyleFillColor() const;
	virtual void		setStyleFillColor(FS_ARGB color);
	// start and end points
	virtual FSPointF	getStartPoint() const;
	virtual void		setStartPoint(FSPointF point);
	virtual FSPointF	getEndPoint() const;
	virtual void		setEndPoint(FSPointF point);
	// caption related
	virtual FS_BOOL		hasCaption() const;
	virtual void		enableCaption(FS_BOOL cap);
	virtual FSString	getCaptionPositionType() const;
	virtual void		setCaptionPositionType(const char* captionPos);
	virtual FSOffset	getCaptionOffset() const;
	virtual void		setCaptionOffset(FSOffset offset);

	//Internal use
	FSPDFLineImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL transformLinePoints(FSRectF newRect);
	CPDF_Point getLLPoint(FS_BOOL isStartPoint);
	CPDF_Point getLLEPoint(FS_BOOL isStartPoint);
};

class FSPDFInkImp : public tFSPDFMarkupImp<FSInk>
{
public:
	virtual ~FSPDFInkImp();
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE	getType() const { return FSAnnot::e_annotInk;}
	virtual FS_BOOL			move(FSRectF rect);
	virtual FS_BOOL			resetAppearanceStream();

	// Functions for release in FSInk.
	virtual FSPDFPath*		getInkList();
	virtual void			setInkList(FSPDFPath* inkList);	

	// Internal use
	FSPDFInkImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
	FS_BOOL					transformInkList(FSRectF newRect);
private:
	FS_BOOL					normalizeInkList(FSPDFPath * inkList);
	FSPDFPathImp*			m_pInkList;
};

class FSPDFStampImp : public tFSPDFMarkupImp<FSStamp>
{
public:
	virtual ~FSPDFStampImp();
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE	getType() const { return FSAnnot::e_annotStamp; }
	virtual FS_BOOL			move(FSRectF rect);
	virtual FS_BOOL			resetAppearanceStream();

	// Functions for release in FSStamp.
	virtual FSString	getIconName() const;
	virtual void		setIconName(const char* iconName);
	virtual void		setBitmap(FSBitmap* bitmap);

	// Internal use
	FSPDFStampImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
protected:
	FS_BOOL createStampAPFromImage();
	FS_BOOL updateStampAP();

private:
	CPDF_Image*			m_pImage;
};

class FSPDFPolygonImp : public tFSPDFMarkupImp<FSPolygon>
{
public:
	virtual ~FSPDFPolygonImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotPolygon; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Inherited from FSMarkup and need to be implemented in the derive class
	virtual void		setIntent(const char* intent);

	// Functions for release in FSPolygon
	virtual FS_ARGB				getFillColor() const;
	virtual void				setFillColor(FS_ARGB fillColor);
	virtual FS_INT32			getVertexCount();
	virtual FSPointF			getVertex(FS_INT32 index);
	virtual void				setVertexes(const FSPointF* vertexes, FS_INT32 count);

	// Internal use
	FSPDFPolygonImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
};

class FSPDFPolyLineImp : public tFSPDFMarkupImp<FSPolyLine>
{
public:
	virtual ~FSPDFPolyLineImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotPolyLine; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Functions for release in FSPolyLine
	virtual FS_ARGB				getStyleFillColor() const;
	virtual void				setStyleFillColor(FS_ARGB fillColor);
	virtual FS_INT32			getVertexCount();
	virtual FSPointF			getVertex(FS_INT32 index);
	virtual void				setVertexes(const FSPointF* vertexes, FS_INT32 count);
	virtual FSString			getLineStartingStyle() const;
	virtual void				setLineStartingStyle(const char* endingStyle);
	virtual FSString			getLineEndingStyle() const;
	virtual void				setLineEndingStyle(const char* endingStyle);

	// Internal use
	FSPDFPolyLineImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
};

class FSPDFCaretImp : public tFSPDFMarkupImp<FSCaret>
{
public:
	virtual ~FSPDFCaretImp(){}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotCaret; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Functions for release in FSCaret
	virtual FSRectF				getInnerRect() const;
	virtual void				setInnerRect(FSRectF innerRect);

	// Internal use 
	FSPDFCaretImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
};

class FSPDFPopupImp: public tFSPDFAnnotImp<FSPopup>
{
public:
	virtual ~FSPDFPopupImp() {}
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FS_BOOL				isMarkup() const{ return FALSE; }
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotPopup; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream() { return FALSE;}

	// Functions for release in FSPopup
	virtual FS_BOOL				getOpenStatus() const;
	virtual void				setOpenStatus(FS_BOOL status);

	// Internal use
	FSPDFPopupImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
};

class FSFileAttachmentImp: public tFSPDFMarkupImp<FSFileAttachment>
{
public:
	virtual ~FSFileAttachmentImp();
	//Inherited from FSAnnot and need to be implemented in the derive class.
	virtual FSM_ANNOTTYPE		getType() const { return FSAnnot::e_annotFileAttachment; }
	virtual FS_BOOL				move(FSRectF rect);
	virtual FS_BOOL				resetAppearanceStream();

	// Functions for release in FSFileAttachment
	virtual FS_BOOL		setFileSpec(FSFileSpec* fileSpecification);
	virtual FSFileSpec*	getFileSpec();
	virtual FSString	getIconName() const;
	virtual void		setIconName(const char* iconName);

	// Internal use
	FSFileAttachmentImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page);
private:
	FSFileSpec*			m_pFileSpec;
};

#endif
