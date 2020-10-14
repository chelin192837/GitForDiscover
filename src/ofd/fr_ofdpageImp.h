#ifndef _FR_OFDPAGEIMP_H_
#define _FR_OFDPAGEIMP_H_

#include "../../include/common.h"
#include "../../include/fs_pdfdoc.h"
#include "../../include/fs_pdfpage.h"

//class FR_OFDDocImp;
//class FSPDFDoc;

class FR_OFDPageImp : public FSPDFPage, public CFX_Object{
public:

	/*************virtual function begin*************/
	virtual FSPDFDoc*			getDocument() const;
	virtual FSPDFDictionary*	getDict() const;
	virtual FS_BOOL				isParsed() const;
	virtual FS_PROGRESSSTATE	startParse(FS_DWORD flag , FSPause* pause, FS_BOOL isReparse);
//	/*virtual*/ FS_DWORD			getParseFlag(){ return m_parseflag; };
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

	virtual FSSignature* addSignature(FSRectF rect);
	/*************virtual function end*************/

	/*************class FR_OFDPageImp*************/
	FR_OFDPageImp();
	~FR_OFDPageImp();



	//Internal use
	FS_BOOL			init(FSPDFDoc* pFSPDFDoc, FS_INT32 pageIndex);
	FR_OFDPageImp*	retain();
	void			setModified();
	FS_BOOL			initAnnotArray();
	/**
	 * Try to release resources and current page object.
	 */
	FS_BOOL			release();
	FS_BOOL			releaseSource();
	IOFD_Page*		getPage() { return m_pPage; }
	IOFD_TextPage*	getTextPage();


	// createFSAnnot() is only for create a FSAnnot object based on annot dictionary.
	// index = -1 means to add to the last.
	// For widget annot, if use createAnnot, that means treat the widge annot as a common form control.
	// If wants to treat a signature field as FSSignature, please use createFSSignature().
	FSAnnot*	createFSAnnot(COFD_Annot* pAnnotDict, FS_INT32 index);
	FSAnnot*	addAnnotImp(FSM_ANNOTTYPE annotType, FSRectF rect, FS_BOOL bNeedDefaultAP);

		// createFSSignature() is only for create a FSSignature object based on annot dictionary.
	// index = -1 means to add to the last.
	FSSignature*	createFSSignature(COFD_Signature* pSig, FS_INT32 index, FS_BOOL bNewAdd, FS_BOOL bCreateVDict);

	// ofd 2 pdf
	CFX_Matrix		getFormatMatrix();

private:
	//FS_BOOL isFlattened(FS_BOOL isDisplay, FS_INT32 options);

public:
	FSCRT_DEFINELOCK(m_lock);
	FSCRT_DEFINELOCK(m_lockAnnots);

	
public:
	CFX_RectF			m_PageArea;

private:
	//ofd api var
	IOFD_Page*			m_pPage;

	//class var
	FSPDFDoc*			m_pFSPDFDoc;
	FS_INT32			m_pageIndex;
	CFX_ArrayTemplate<void*>*	m_pAnnotsArray;

	FSPause*			m_pParsePause;
	IOFD_TextPage*		m_pTextPage;
	
	FS_DWORD			m_parseflag;
	 
	FS_INT32			m_nCountRef;

	FR_OFDPageImp*		m_pCurRenderPage;
	
	//
};
#endif