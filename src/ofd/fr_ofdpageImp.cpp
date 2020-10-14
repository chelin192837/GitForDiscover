#include "fr_include.h"
#include "../fs_util.h"
#include "fr_ofdpageImp.h"
#include "fr_ofddocImp.h"
#include "fr_ofdsignatureImp.h"

#include "annotation/fr_ofdannot_util.h"
#include "annotation/fr_ofdannotImp.h"

#include "fr_include.h"
#include "../inc/fpdf_define.h"

bool isOFDPage(FSPDFPage *page) {
//    return (NULL == dynamic_cast<FR_OFDPageImp *>(page)) ? false : true;
    return (NULL == (FR_OFDPageImp *)(page)) ? false : true;
}


FR_OFDPageImp::FR_OFDPageImp()
        : m_pFSPDFDoc(NULL), m_pageIndex(-1), m_pPage(NULL), m_nCountRef(1), m_PageArea(),
          m_pAnnotsArray(NULL), m_pParsePause(NULL), m_pTextPage(NULL), m_parseflag(0),
          m_pCurRenderPage(NULL) {
}

FR_OFDPageImp::~FR_OFDPageImp() {
}

FS_BOOL FR_OFDPageImp::releaseSource() {
    FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
    if (m_pAnnotsArray) {
        for (FS_INT32 i = 0; i < m_pAnnotsArray->GetSize(); i++) {
            FSAnnot *pAnnot = (FSAnnot *) m_pAnnotsArray->GetAt(i);
            if (pAnnot)
                delete pAnnot;
        }
        m_pAnnotsArray->RemoveAll();

        delete m_pAnnotsArray;
        m_pAnnotsArray = NULL;
    }
    SAFE_DELETE(m_pTextPage);
    //SAFE_DELETE(m_pPage);
    return TRUE;
}

FS_BOOL FR_OFDPageImp::release() {
    {
        FSCRT_LOCKOBJECT(lock, &m_lock);
        m_nCountRef--;
        if (m_nCountRef != 0) {
            return FALSE;
        }
        releaseSource();
        m_pParsePause = NULL;
        m_pFSPDFDoc = NULL;
    }
    delete this;
    return TRUE;
}

FS_BOOL FR_OFDPageImp::init(FSPDFDoc *pFSPDFDoc, FS_INT32 pageIndex) {
    if (!pFSPDFDoc || !((FR_OFDDocImp *) pFSPDFDoc)->getOFDDocument()) return FALSE;

    IOFD_Document *pOFDDoc = ((FR_OFDDocImp *) pFSPDFDoc)->getOFDDocument();
    m_pPage = pOFDDoc->GetPage(pageIndex);
    if (!m_pPage)
        return FALSE;
    m_pPage->LoadPage();
    m_pPage->GetPageArea().GetPageArea(OFD_PAGEAREA_PhysicalBox, m_PageArea);
    m_pFSPDFDoc = pFSPDFDoc;
    m_pageIndex = pageIndex;
    return TRUE;
}

FR_OFDPageImp *FR_OFDPageImp::retain() {
    FSCRT_LOCKOBJECT(lock, &m_lock);
    m_nCountRef++;
    return this;
}

void FR_OFDPageImp::setModified() {
    if (m_pFSPDFDoc)
        ((FR_OFDDocImp *) m_pFSPDFDoc)->setModified();
}

FSPDFDoc *FR_OFDPageImp::getDocument() const {
    return m_pFSPDFDoc;
}

FSPDFDictionary *FR_OFDPageImp::getDict() const {
    return NULL;
}

FS_BOOL FR_OFDPageImp::isParsed() const {
    if (!m_pPage)
        return FALSE;
    return TRUE;
}

FS_PROGRESSSTATE FR_OFDPageImp::startParse(FS_DWORD flag, FSPause *pause, FS_BOOL isReparse) {
    FSCRT_LOCKOBJECT(lock, &m_lock);

/*	if (!m_pPage
		|| flag < e_parsePageNormal || flag > (e_parsePageNormal | e_parsePageTextOnly  | e_parseTextOutputHyphen)
		) 
		return FSCommonDefines::e_progressError;
	
	if (m_parseflag == flag)
	{
		if (!isReparse)
		return FSCommonDefines::e_progressFinished;
	//	if (m_pPage->isParsed() && !isReparse)
	//		return FSCommonDefines::e_progressFinished;
	}
	m_parseflag = flag;

	CPDF_ParseOptions parseOption;
	if (e_parsePageTextOnly == (flag&e_parsePageTextOnly))
		parseOption.m_bTextOnly = TRUE;
	else
		parseOption.m_bTextOnly = FALSE;
	m_pPage->LoadPage();
	m_pParsePause = pause;
	SAFE_DELETE(m_pTextPage);*/
    //return continueParse();
    return FSCommonDefines::e_progressFinished;
}

FS_PROGRESSSTATE FR_OFDPageImp::continueParse() {
    return FSCommonDefines::e_progressFinished;
}

FS_INT32 FR_OFDPageImp::getIndex() const {
    return m_pageIndex;
}

FS_FLOAT FR_OFDPageImp::getHeight() const {
    if (!m_pPage) return 0.0f;
    return MM_2_PT(m_PageArea.Height());
}

FS_FLOAT FR_OFDPageImp::getWidth() const {
    if (!m_pPage) return 0.0f;
    FS_FLOAT width = MM_2_PT(m_PageArea.Width());
    return width;
}

FS_ROTATION FR_OFDPageImp::getRotation() const {
    return (FS_ROTATION) 0;
}

FSBitmap *FR_OFDPageImp::loadThumbnail() {
    return NULL;
}

CFX_Matrix FR_OFDPageImp::getFormatMatrix() {
    CFX_Matrix ofd_2_pdf;
    ofd_2_pdf.a = MM_2_PT(1.0);
    ofd_2_pdf.d = -MM_2_PT(1.0);
    ofd_2_pdf.f = MM_2_PT(m_PageArea.height);
    return ofd_2_pdf;
}

FSMatrix
FR_OFDPageImp::getDisplayMatrix(FS_INT32 xPos, FS_INT32 yPos, FS_INT32 xSize, FS_INT32 ySize,
                                FS_ROTATION rotate) {
    FSMatrix fsMatrix;
    fsMatrix.set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    if (!m_pPage || rotate < FSCommonDefines::e_rotation0 ||
        rotate > FSCommonDefines::e_rotation270)
        return fsMatrix;

    CFX_Matrix matrix(1, 0, 0, 1, 0, 0);
    CFX_Rect devicePageRect;
    devicePageRect.Set(xPos, yPos, xSize, ySize);
    OFD_GetPageMatrix(matrix, m_PageArea, devicePageRect, 0);
    //matrix.Translate((FX_FLOAT)(-rtClipBox.left), (FX_FLOAT)(-rtClipBox.top));

    CFX_Matrix ofd_2_pdf = getFormatMatrix();
    CFX_Matrix pdf_2_ofd;
    pdf_2_ofd.SetReverse(ofd_2_pdf);

    pdf_2_ofd.Concat(matrix);

    matrix = pdf_2_ofd;
    fsMatrix.set(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);
    return fsMatrix;
}

FSRectF FR_OFDPageImp::calcContentBBox(FS_CALCMARGINMODE mode) {
    FSRectF fsRectF;
    return fsRectF;
}


IOFD_TextPage *FR_OFDPageImp::getTextPage() {
    FSCRT_LOCKOBJECT(lock, &m_lock);

    if (!m_pTextPage) {
        m_pTextPage = IOFD_TextPage::CreateTextPage(m_pPage);
    }
    return m_pTextPage;
}

/*
FS_BOOL FR_OFDPageImp::initAnnotArray()
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	if(m_pAnnotsArray)
		return TRUE;
	CPDF_AnnotList annotList(m_pPage);
	FS_INT32 annotCount = annotList.Count();
	m_pAnnotsArray = FX_NEW CFX_PtrArray();
	if (!m_pAnnotsArray)
		return FALSE;
	if (!m_pAnnotsArray->SetSize(annotCount))
		return FALSE;
	// RDK and GSDK trial watermark should not be counted.
	// Check if there's any RDK or GSDK trial watermark.
	// If yes, reduce annot count of original annot array.
	FS_INT32 nWatermark = 0;
	FS_INT32 i=0;
	for (i=0; i<annotCount; i++)
	{
		CPDF_Annot* pAnnot = annotList.GetAt(i);
		FS_INT32 iWMAnnotType = FSCRT_Util_GetWatermarkAnnotType(pAnnot->m_pAnnotDict);
		if (WMANNOTTYPE_RDKTRIAL == iWMAnnotType || WMANNOTTYPE_GSDKTRIAL == iWMAnnotType){
			nWatermark++;
			continue;
		}
		FSAnnot* pFSAnnot = NULL;
		// When analyse /Annots, we should store signature field as FSSignature.
		if (FSANNOT_Util_IsSignatureField(pAnnot->m_pAnnotDict))
			pFSAnnot = createFSSignature(pAnnot->m_pAnnotDict, i-nWatermark, FALSE, FALSE);
		else
			pFSAnnot = createFSAnnot(pAnnot->m_pAnnotDict, i-nWatermark);
		//m_pAnnotsArray->SetAt(i-nWatermark, pFSAnnot); //TOCHECK: this seems to be useless since it has been done in createFSSignature/creatFSAnnot.
	}
	m_pAnnotsArray->RemoveAt(i-nWatermark, nWatermark);
	return TRUE;
}
*/

FS_BOOL FR_OFDPageImp::initAnnotArray() {
    FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
    if (m_pAnnotsArray)
        return TRUE;
    IOFD_PageAnnots *pageAnnots = m_pPage->GetDocument()->GetPageAnnots(getIndex());
    FS_INT32 annotCount = pageAnnots ? pageAnnots->CountAnnots() : 0;
    m_pAnnotsArray = FX_NEW CFX_PtrArray();
    if (!m_pAnnotsArray)
        return FALSE;
    //* temp comment by gm
    if (!m_pAnnotsArray->SetSize(annotCount))
        return FALSE;
    // RDK and GSDK trial watermark should not be counted.
    // Check if there's any RDK or GSDK trial watermark.
    // If yes, reduce annot count of original annot array.
    FS_INT32 nWatermark = 0;
    FS_INT32 i = 0;
    for (i = 0; i < annotCount; i++) {
        COFD_Annot *pAnnot = (COFD_Annot *) pageAnnots->GetAnnot(i);
        /*
        FS_INT32 iWMAnnotType = FSCRT_Util_GetWatermarkAnnotType(pAnnot->m_pAnnotDict);
        if (WMANNOTTYPE_RDKTRIAL == iWMAnnotType || WMANNOTTYPE_GSDKTRIAL == iWMAnnotType){
            nWatermark++;
            continue;
        }
        // */
        FSAnnot *pFSAnnot = NULL;
        // When analyse /Annots, we should store signature field as FSSignature.
        /*
        if (FRANNOT_Util_IsSignatureField(pAnnot))
            pFSAnnot = createFSSignature(pAnnot, i-nWatermark, FALSE, FALSE);
        else	// */
        pFSAnnot = createFSAnnot(pAnnot, i - nWatermark);
        //m_pAnnotsArray->SetAt(i-nWatermark, pFSAnnot); //TOCHECK: this seems to be useless since it has been done in createFSSignature/creatFSAnnot.
    }
    m_pAnnotsArray->RemoveAt(i - nWatermark, nWatermark);
    // */
    return TRUE;
}

FS_INT32 FR_OFDPageImp::getAnnotCount() {
    FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
    initAnnotArray();
    if (m_pAnnotsArray)
        return m_pAnnotsArray->GetSize();
    return 0;
}

FSAnnot *FR_OFDPageImp::getAnnot(FS_INT32 index) {
    FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
    FS_INT32 iAnnotCount = getAnnotCount();
    if (index < 0 || index >= iAnnotCount) return NULL;
    return (FSAnnot *) m_pAnnotsArray->GetAt(index);
}

FSAnnot *FR_OFDPageImp::getAnnotAtPos(FSPointF pos, FS_FLOAT tolerance) {
    return getAnnotAtDevicePos(NULL, pos, tolerance);
}

FSAnnot *
FR_OFDPageImp::getAnnotAtDevicePos(const FSMatrix *matrix, FSPointF pos, FS_FLOAT tolerance) {
    if (tolerance < 0.0f || tolerance > 30.0f)
        return NULL;
    if (!m_pAnnotsArray && !initAnnotArray())
        return NULL;

    FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
    FS_INT32 iAnnotCount = getAnnotCount();
    /*
    if(m_pFSPDFDoc)
    {
        FSForm* form = m_pFSPDFDoc->getForm();
        if(form && form->getFormFiller())
        {
            FSFormFillerImp* formFiller = (FSFormFillerImp*)form->getFormFiller();
            if(formFiller && formFiller->GetFocusAnnot())
            {
                FSAnnot* pAnnot = formFiller->GetFocusAnnot();
                if(FSANNOT_Util_CheckAnnotIsAtPoint(pAnnot, pos.x, pos.y, tolerance, matrix))
                    return pAnnot;
            }
        }
    }
    // */
    for (FS_INT32 iValidIndex = iAnnotCount - 1; iValidIndex >= 0; iValidIndex--) {
        FSAnnot *pAnnot = (FSAnnot *) m_pAnnotsArray->GetAt(iValidIndex);
        if (!pAnnot)
            continue;
        /*
        if(FSAnnot::e_annotPopup == pAnnot->getType() || FSAnnot_Util_ChectAnnotIsReply((CPDF_Dictionary*)pAnnot->getDict()) || FSAnnot_Util_CheckIsStateAnnot(pAnnot))
            continue;
        // */

        FX_BOOL bIn = FRANNOT_Util_CheckAnnotIsAtPoint(pAnnot, pos.x, pos.y, tolerance, matrix);
        if (bIn)
            return pAnnot;
    }
    return NULL;
}

FSAnnot *FR_OFDPageImp::addAnnot(FSM_ANNOTTYPE annotType, FSRectF rect) {
    LogOut("FR_OFDPageImp::addAnnot begin");
//	if (!m_pPage
//		//|| FALSE == FSANNOT_Util_CheckAnnotIsSupport(annotType))
//        || FSAnnot::e_annotLine != annotType
//		|| FSAnnot::e_annotStamp != annotType
//	    || FSAnnot::e_annotWidget == annotType)
//	     //Not support to add widget annot (form control &signature).
//        {
//            LogOut("FR_OFDPageImp::addAnnot !annotType:%d ",annotType);
//            LogOut("FR_OFDPageImp::addAnnot !m_pPage:%d ",m_pPage);
//            return NULL;
//        }

//	if (!CHECKVALID_FLOATRECT_PDF(rect))
//	{
//        LogOut("FR_OFDPageImp::addAnnot !CHECKVALID_FLOATRECT_PDF");
//		if (FSAnnot::e_annotNote == annotType
//			|| FSAnnot::e_annotFreeText == annotType
//			|| FSAnnot::e_annotSquare == annotType
//			|| FSAnnot::e_annotCircle == annotType
//			|| FSAnnot::e_annotFileAttachment == annotType
//			|| FSAnnot::e_annotStamp == annotType
//			|| FSAnnot::e_annotCaret == annotType
//			|| FSAnnot::e_annotLink == annotType
//			|| FSAnnot::e_annotLine == annotType)
//		{
//            LogOut("FR_OFDPageImp::addAnnot some annotType return null");
//				return NULL;
//		}
//		else
//			rect.set(0, 0, 0, 0);
//	}

    return addAnnotImp(annotType, rect, TRUE);
}


FS_BOOL FR_OFDPageImp::removeAnnot(FSAnnot *annot) {
    FS_INT32 annotCount = m_pAnnotsArray ? m_pAnnotsArray->GetSize() : 0;
    for (FS_INT32 i = 0; i < annotCount; i++) {
        if (((FSAnnot *) m_pAnnotsArray->GetAt(i)) == annot) {
            IOFD_Document *pDoc = ((FR_OFDDocImp *) m_pFSPDFDoc)->getOFDDocument();
            int pageindex = getIndex();
            IOFD_WriteDocument *writeDoc = OFD_WriteDoc_Create(pDoc);
            IOFD_WritePageAnnots *pageAnnots = writeDoc->CreatePageAnnots(pageindex);
            pageAnnots->DeleteAnnot(i);
            m_pAnnotsArray->RemoveAt(i);
            return TRUE;
        }
    }
    return FALSE;
}

FS_BOOL FR_OFDPageImp::hasTransparency() {
    if (!m_pPage)
        return FALSE;

    return FALSE;//m_pPage->BackgroundAlphaNeeded();
}

/*FS_BOOL FR_OFDPageImp::isFlattened(FS_BOOL isDisplay, FS_INT32 options){
	if ((options & FSPDFPageDefines::e_flattenOptionNoAnnot) && (options & FSPDFPageDefines::e_flattenOptionNoFormControl))
		return TRUE;
	FS_INT32 nCount = getAnnotCount();
	for (FS_INT32 i = 0;i < nCount;i++)
	{
		FSAnnot* annot = getAnnot(i);
		if (isNeedFlattenAnnot((CPDF_Dictionary*)annot->getDict(), isDisplay, options))
			return FALSE;
	}
	return TRUE;
}*/

FS_BOOL FR_OFDPageImp::flatten(FS_BOOL isDisplay, FS_DWORD options) {
    return FALSE;
    /*if (options > (FSPDFPageDefines::e_flattenOptionAll | FSPDFPageDefines::e_flattenOptionNoFormControl | FSPDFPageDefines::e_flattenOptionNoAnnot))
        return FALSE;

    if (isFlattened(isDisplay, options))
        return TRUE;
    */
/*	if (!m_pPage) return FALSE;

	CPDF_Document* pDocument = m_pPage->m_pDocument;
	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	if (!pDocument || !pPageDict) return FALSE;

	CPDF_ObjectArray ObjectArray;
	CPDF_RectArray	 RectArray;
	// Used to store the object number of annotations which are flattened and needed to be removed at last.
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	CFX_DWordArray removeAnnotArray; 
	FX_BOOL bNewPage = FALSE;
	CPDF_Page *pPage = m_pPage;
	if (!pPage->IsParsed()){
		pPage = FX_NEW CPDF_Page;
		if (!pPage) return FALSE;
		pPage->Load(pDocument, pPageDict);
		pPage->ParseContent();
		bNewPage = TRUE;
	}

	if (!parserAnnotsForFlatten(isDisplay, options, &RectArray, &ObjectArray,&removeAnnotArray)) {
		if (bNewPage) delete pPage;
		return TRUE;
	}

	annotsRectForFlatten(RectArray);
	generateContentForFlatten(ObjectArray);
	if (!removeAnnotForFlatten(removeAnnotArray)) 
	{		
		if (bNewPage) delete pPage;
		return TRUE;
	}
	if(bNewPage)
		delete pPage;
	if (m_pPage)
	{
		//Render cache is created in the previous time, it should be destroyed first.
		CPDF_RenderModuleDef* pModule = CPDF_ModuleMgr::Get()->GetRenderModule();
		pModule->DestroyPageCache(m_pPage->GetRenderCache());

		m_pPage->Load(m_pPage->m_pDocument, m_pPage->m_pFormDict);
		m_pPage->StartParse(NULL, TRUE);
	}

	setModified();*/
}

FS_BOOL
FR_OFDPageImp::setAnnotGroup(FSMarkup **annotArray, FS_INT32 arrayCount, FS_INT32 headerIndex) {
    return TRUE;
}

FSSignature *FR_OFDPageImp::addSignature(FSRectF rect) {
    /*
    *创建印章结构之前要初始化印章区域；
    */
    FSSignature *pFSSig = NULL;
    FR_OFDSignatureImp *signature = new FR_OFDSignatureImp();
//	FSBitmap* m_pngPath = signature->getBitmap();

    signature->init(this,/* m_pPage,*/rect);
    pFSSig = signature;
    return pFSSig;
}


FSAnnot *FR_OFDPageImp::createFSAnnot(COFD_Annot *pAnnotDict, FS_INT32 index) {
    if (!pAnnotDict || index < -1) return NULL;

    CFX_ByteString bsType = pAnnotDict->GetSubType().UTF8Encode();
    FSM_ANNOTTYPE annotType = FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(bsType);

    FX_BOOL isMarkup = FSANNOT_Util_CheckAnnotIsMarkup(annotType);
    FSAnnot *pFsAnnot = NULL;
    switch (annotType) {




        /* temp comment by gm
        case FSAnnot::e_annotNote:
            pFsAnnot = (FSNote*)FX_NEW FSPDFNoteImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotHighlight:
            pFsAnnot = (FSHighlight*)FX_NEW FSPDFHighlightImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotSquiggly:
            pFsAnnot = (FSSquiggly*)FX_NEW FSPDFSquigglyImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotUnderline:
            pFsAnnot = (FSUnderline*)FX_NEW FSPDFUnderlineImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotStrikeOut:
            pFsAnnot = (FSStrikeOut*)FX_NEW FSPDFStrikeOutImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotLink:
            pFsAnnot = (FSLink*)FX_NEW FSPDFLinkImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotSquare:
            pFsAnnot = (FSSquare*)FX_NEW FSPDFSquareImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotCircle:
            pFsAnnot = (FSCircle*)FX_NEW FSPDFCircleImp(pAnnotDict, this);
            break;

        case FSAnnot::e_annotWidget:
            pFsAnnot = (FSFormControl*)FX_NEW FSFormControlImp(pAnnotDict, this);
            break;

        // */
        case FSAnnot::e_annotFreeText:
            pFsAnnot = (FSFreeText *) FX_NEW FROFDFreeTextImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotLine:    // 4
            pFsAnnot = (FSLine *) FX_NEW FROFDLineImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotStamp:    // 13
            pFsAnnot = (FSStamp *) FX_NEW FROFDStampImp(pAnnotDict, this);
            break;
        case FSAnnot::e_annotInk:    // 15
            pFsAnnot = (FSInk *) FX_NEW FROFDInkImp(pAnnotDict, this);
            break;
            /*
            case FSAnnot::e_annotCaret:
                pFsAnnot = (FSCaret*)FX_NEW FSPDFCaretImp(pAnnotDict, this);
                break;
            case FSAnnot::e_annotInk:
                pFsAnnot = (FSInk*)FX_NEW FSPDFInkImp(pAnnotDict, this);
                break;
            case FSAnnot::e_annotFileAttachment:
                pFsAnnot = (FSFileAttachment*)FX_NEW FSFileAttachmentImp(pAnnotDict, this);
                break;
            case FSAnnot::e_annotPolygon:
                pFsAnnot = (FSPolygon*)FX_NEW FSPDFPolygonImp(pAnnotDict, this);
                break;
            case FSAnnot::e_annotPolyLine:
                pFsAnnot = (FSPolyLine*)FX_NEW FSPDFPolyLineImp(pAnnotDict, this);
                break;
            case FSAnnot::e_annotPopup:
                pFsAnnot = (FSPopup*)FX_NEW FSPDFPopupImp(pAnnotDict, this);
                break;
                // */
        default: {
            if (isMarkup)
                pFsAnnot = (FSMarkup *) FX_NEW FROFDMarkupImp(pAnnotDict, this);
            else
                pFsAnnot = (FSAnnot *) FX_NEW FROFDAnnotImp(pAnnotDict, this);
        }

    }
    if (!pFsAnnot) return NULL;

    //* temp comment by gm
    {
        FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
        if (!m_pAnnotsArray) m_pAnnotsArray = FX_NEW CFX_PtrArray;
        if (index < 0 || index >= m_pAnnotsArray->GetSize())
            m_pAnnotsArray->Add(pFsAnnot);
        else
            m_pAnnotsArray->SetAt(index, pFsAnnot);
    }

    // */
//	pFsAnnot->getPage();
//    ((FROFDAnnotImp*)pFsAnnot)->createAppearanceStream();   // 调用每个Annot类创建Appearance

    return pFsAnnot;
}


FSAnnot *FR_OFDPageImp::addAnnotImp(FSM_ANNOTTYPE annotType, FSRectF rect, FS_BOOL bNeedDefaultAP) {
    LogOut("FR_OFDPageImp::addAnnotImp begin");
    FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
    if (!m_pAnnotsArray)
        getAnnotCount();
    if (!m_pAnnotsArray) {
        LogOut("FR_OFDPageImp::addAnnotImp !m_pAnnotsArray");
        return NULL;
    }
    COFD_WriteAnnot *pAnnotDict = FR_OFDANNOT_UTIL_InsertAnnot(annotType, -1, rect, m_pPage,
                                                               ((FR_OFDDocImp *) m_pFSPDFDoc)->getOFDDocument());
    FSAnnot *pNewAnnot = createFSAnnot(pAnnotDict->GetReadAnnot(), -1);
    LogOut("FR_OFDPageImp::addAnnotImp pNewAnnot %x", pNewAnnot);
    if (pNewAnnot && bNeedDefaultAP)
        FR_OFDANNOT_UTIL_SetDefaultProperties(pNewAnnot);
    if (pAnnotDict || pNewAnnot)
        setModified();
    return pNewAnnot;
}

FSSignature *FR_OFDPageImp::createFSSignature(COFD_Signature *pSig, FS_INT32 index, FS_BOOL bNewAdd,
                                              FS_BOOL bCreateVDict) {
    return NULL;
    /*
    if (!pSig || index<-1 || !m_pAnnotsArray || !CFS_Library::get())
        return NULL;

#if defined(ENABLE_SIGNATURE)
    FSSignature* pFSSig = FX_NEW FSSignatureImp(pSig, this, bNewAdd, bCreateVDict);
    if (!pFSSig) return NULL;
    {
        FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
        if (index<0 || index>=m_pAnnotsArray->GetSize())
            m_pAnnotsArray->Add(pFSSig);
        else
            m_pAnnotsArray->SetAt(index, pFSSig);
    }
    return pFSSig;
#else
    return NULL;
#endif
    // */
}


