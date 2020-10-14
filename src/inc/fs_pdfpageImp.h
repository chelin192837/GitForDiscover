#ifndef _FS_PDFPAGE_IMP_H_
#define _FS_PDFPAGE_IMP_H_

#include "../../include/common.h"
#include "../../include/fs_pdfpage.h"

class FSPDFDoc;
class CPDF_Page;
template<class TYPE>
class CFX_ArrayTemplate;
class CFSCRT_Lock;
class FSFormFillerImp;

typedef CFX_ArrayTemplate<CPDF_Dictionary*>   CPDF_ObjectArray;
typedef CFX_ArrayTemplate<CPDF_Rect>   CPDF_RectArray;

#define FSPDF_TYPE_MAX  0
#define FSPDF_TYPE_MIN  1

#define FSPDF_VALUE_TOP     0
#define FSPDF_VALUE_LEFT    1
#define FSPDF_VALUE_RIGHT   2
#define FSPDF_VALUE_BOTTOM  3

#define FSPDF_FLATTEN_GETVALUE(type, value1, value2) (type == FSPDF_TYPE_MAX ? FX_MAX(value1, value2) : FX_MIN(value1, value2))

class FSPDFPageImp : public FSPDFPage, public CFX_Object
{
public:
	FSPDFPageImp();
	virtual ~FSPDFPageImp();

	virtual FSPDFDoc*			getDocument() const;
	virtual FSPDFDictionary*	getDict() const;
	virtual FS_BOOL				isParsed() const;
	virtual FS_PROGRESSSTATE	startParse(FS_DWORD flag , FSPause* pause, FS_BOOL isReparse);
	/*virtual*/ FS_DWORD			getParseFlag(){ return m_parseflag; };
	virtual FS_PROGRESSSTATE	continueParse();
	virtual FS_INT32			getIndex() const;
	virtual FS_FLOAT			getHeight() const;
	virtual FS_FLOAT			getWidth() const;
	virtual FS_ROTATION			getRotation() const;
	virtual FSBitmap*			loadThumbnail();
	virtual FSMatrix			getDisplayMatrix(FS_INT32 xPos, FS_INT32 yPos, FS_INT32 xSize, FS_INT32 ySize, FS_ROTATION rotate); 
	virtual FSRectF				calcContentBBox(FS_CALCMARGINMODE mode);
	virtual FS_INT32			getAnnotCount();
	virtual FSAnnot*			getAnnot(FS_INT32 index);
	virtual FSAnnot*			getAnnotAtPos(FSPointF pos, FS_FLOAT tolerance);
	virtual FSAnnot*			getAnnotAtDevicePos(const FSMatrix* matrix, FSPointF pos, FS_FLOAT tolerance);
	virtual FSAnnot*			addAnnot(FSM_ANNOTTYPE annotType, FSRectF rect);
	virtual FS_BOOL				removeAnnot(FSAnnot* annot);
	virtual FS_BOOL				hasTransparency();
	virtual FS_BOOL				flatten(FS_BOOL isDisplay, FS_DWORD options);
	virtual FS_BOOL				setAnnotGroup(FSMarkup** annotArray, FS_INT32 arrayCount, FS_INT32 headerIndex);

	virtual FSSignature* getSignatureAtPos(FSPointF pos, FS_FLOAT tolerance);
	virtual FSSignature* addSignature(FSRectF rect);

	virtual FSFormControl*		addControl(CPDF_FormControl* pFormControl, FSRectF rect);
	virtual FSFormControl*		getControlAtPos(FSPointF pos, FS_FLOAT tolerance);
	virtual FSFormControl*		getControlByDict(CPDF_Dictionary* dict);
	FS_BOOL						removeAnnotByObjNum(FX_DWORD objNum);
	FS_RESULT					DeviceToPagePoint(const CFX_Point &devicePoint, CFX_PointF &pagePoint);
	FS_RESULT					DeviceToPageRect(const CFX_Rect &deviceRect, CFX_FloatRect &pageRect);
	FS_RESULT					PageToDevicePoint(const CFX_PointF &pagePoint, CFX_Point &devicePoint);
	FS_RESULT					PageToDeviceRect(const CFX_FloatRect &pageRect, CFX_Rect &deviceRect);

	FSFormControl*				GetFormControl(FS_INT32 index, FS_INT32* indexInAnnotList = NULL);
	FS_INT32					CountFormControls();
	FSFormFillerImp*			getFormFiller();
	//Internal use
	FS_BOOL			init(FSPDFDoc* pFSPDFDoc, FS_INT32 pageIndex);
	FSPDFPageImp*	retain();
	void			setModified();
	/** 
	 * Destroy current page object, and ignore the reference counter.
	 * Currently, this can only be used in document's destructor, to ensure release all the page resources.
	 */
	void			destroy();
	/**
	 * Try to release resources and current page object.
	 */
	FS_BOOL			release();
	FS_BOOL			releaseSource();
	CPDF_Page*		getPage() { return m_pPage; }
	IPDF_TextPage*	getTextPage();
	CFX_ArrayTemplate<void*>*	getAnnotArray() { return m_pAnnotsArray; }
	FS_INT32		getWatermarkAnnotType(FS_INT32 annotIndex);

	FS_BOOL			initAnnotArray();
	FSAnnot*		getAnnotbyDict(CPDF_Dictionary* pAnnotDict);

	// createFSAnnot() is only for create a FSAnnot object based on annot dictionary.
	// index = -1 means to add to the last.
	// For widget annot, if use createAnnot, that means treat the widge annot as a common form control.
	// If wants to treat a signature field as FSSignature, please use createFSSignature().
	FSAnnot*	createFSAnnot(CPDF_Dictionary* pAnnotDict, FS_INT32 index);
	FSAnnot*	addAnnotImp(FSM_ANNOTTYPE annotType, FSRectF rect, FS_BOOL bNeedDefaultAP);

	// createFSSignature() is only for create a FSSignature object based on annot dictionary.
	// index = -1 means to add to the last.
	FSSignature*	createFSSignature(CPDF_Dictionary* pSigFieldDict, FS_INT32 index, FS_BOOL bNewAdd, FS_BOOL bCreateVDict);
	FSSignature*	createFSSignature(CPDF_Signature* pSig, FS_INT32 index, FS_BOOL bNewAdd, FS_BOOL bCreateVDict);

private:
	FS_BOOL parserAnnotsForFlatten(FS_BOOL isDisplay, FS_DWORD nUsage, CPDF_RectArray* pRectArray, CPDF_ObjectArray* pObjectArray,CFX_DWordArray* pRemoveAnnotArray);
	FS_BOOL getContentsRectForFlatten(CPDF_RectArray* pRectArray);
	void parserStreamForFlatten(CPDF_Dictionary* pStream, CPDF_RectArray* pRectArray,CPDF_ObjectArray* pObjectArray);
	void annotsRectForFlatten(CPDF_RectArray &RectArray);
	void generateContentForFlatten(CPDF_ObjectArray &ObjectArray);
	void setPageContentsForFlatten(CFX_ByteString key);
	FS_BOOL flattenIsValiableRect(CPDF_Rect rect, CPDF_Rect rcPage);
	FS_BOOL removeAnnotForFlatten(CFX_DWordArray removeAnnotArray);
	FS_BOOL deleteAnnotByObjNum(FX_DWORD objNum);
	CPDF_Rect flattenCalculateRect(const CPDF_RectArray& pRectArray);
	FS_FLOAT flattenGetMinMaxValue(const CPDF_RectArray& array, FS_INT32 type, FS_INT32 value);
	CFX_AffineMatrix flattenGetMatrix(CPDF_Rect rcAnnot, CPDF_Rect rcStream, CFX_AffineMatrix matrix);
	FS_BOOL isFlattened(FS_BOOL isDisplay, FS_INT32 options);
	FS_BOOL isNeedFlattenAnnot(CPDF_Dictionary* pAnnotDic, FS_BOOL isDisplay, FS_INT32 options);

	FS_BOOL getOldGroupElements(FSMarkup* pMarkup, CFX_MapPtrToPtr& mapNewElements);

	//Form Filler implementation.
public:
	FS_RESULT		OnMouseMove(FS_DWORD nFlags, const CFX_PointF& point);
	FS_RESULT		OnLButtonDown(FS_DWORD nFlags, const CFX_PointF& point);
	FS_RESULT		OnLButtonUp(FS_DWORD nFlags, const CFX_PointF& point);
	FS_RESULT		OnLButtonDbClick(FS_DWORD nFlags, const CFX_PointF& point);
	FS_RESULT		OnKeyDown(FS_DWORD nKeyCode, FS_DWORD nFlags);
	FS_RESULT		OnKeyUp(FS_DWORD nKeyCode, FS_DWORD nFlags);
	FS_RESULT		OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags);

public:
	FSCRT_DEFINELOCK(m_lock);
	FSCRT_DEFINELOCK(m_lockAnnots);
private:
	FSPDFDoc*			m_pFSPDFDoc;
	CPDF_Page*			m_pPage;
	//ATTENTION: if add or remove page, this index should be update somewhere.
	FS_INT32			m_pageIndex;
	FSPause*			m_pParsePause;
	FS_DWORD			m_parseflag;
	IPDF_TextPage*		m_pTextPage;

	CFX_ArrayTemplate<void*>*	m_pAnnotsArray; //store FSAnnot();

	FS_INT32			m_nCountRef;
};


FS_INT32 getPageRotation(CPDF_Dictionary* pFormDict) ;

#endif //_FS_PDFPAGE_IMP_H_