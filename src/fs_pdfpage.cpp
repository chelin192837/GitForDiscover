#include "../include/fs_pdfdoc.h"
#include "../include/fs_pdfobject.h"
#include "../include/fs_pdfpage.h"
#include "../include/fs_render.h"
#include "../include/common.h"
#include "../include/fs_annot.h"
#include "ofd/fr_include.h"

#include "fxcore.h"
#include "fs_file.h"
#include "fs_util.h"
#include "fs_pause.h"
#include "annotation/fs_annot_util.h"
#include "annotation/fs_annot_imp.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_renderImp.h"
#include "watermark/inc/fs_watermark.h"
#include "form/fs_pdfformImp.h"
#include "inc/fs_signatureImp.h"

////////////////////////////////////////////////
// Class FSPDFPageImp
FSPDFPageImp::FSPDFPageImp()
	:m_pFSPDFDoc(NULL)
	,m_pPage(NULL)
	,m_pParsePause(NULL)
	,m_parseflag(e_parsePageNormal)
	,m_pAnnotsArray(NULL)
	,m_pTextPage(NULL)
	,m_nCountRef(1)
{}

FSPDFPageImp* FSPDFPageImp::retain()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_nCountRef++;
	return this;
}

FSPDFPageImp::~FSPDFPageImp()
{
	//If you indeed want to release resources of current page object, please call release(), which is just an internal method.
}

void	FSPDFPageImp::destroy()
{
	FSCRT_LOCK(m_lock);
	m_nCountRef = 1;
	FSCRT_UNLOCK(m_lock);

	release();
}

FS_BOOL FSPDFPageImp::releaseSource()
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	if (m_pAnnotsArray) 
	{
		for (FS_INT32 i = 0; i < m_pAnnotsArray->GetSize(); i++)
		{
			FSAnnot* pAnnot = (FSAnnot*)m_pAnnotsArray->GetAt(i);
			if (pAnnot)
				delete pAnnot;
		}
		m_pAnnotsArray->RemoveAll();

		delete m_pAnnotsArray;
		m_pAnnotsArray = NULL;
	}
	SAFE_DELETE(m_pTextPage);
	SAFE_DELETE(m_pPage);
	return TRUE;
}

FS_BOOL FSPDFPageImp::release()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		m_nCountRef --;	
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

void FSPDFPageImp::setModified()
{
	if(m_pFSPDFDoc)
		((FSPDFDocImp*)m_pFSPDFDoc)->setModified();
}

IPDF_TextPage*	FSPDFPageImp::getTextPage()
{	
	FSCRT_LOCKOBJECT(lock, &m_lock);

	if(m_pTextPage)
		return m_pTextPage;
	CPDFText_ParseOptions opt;
	// Comment following code. They will be useful if e_parseStreamOrder is released.
	//if(m_parseflag & e_parseStreamOrder)
	//	opt.m_bNormalizeObjs = FALSE;
	//else
	opt.m_bNormalizeObjs = TRUE;
	if(m_parseflag & e_parseTextOutputHyphen)
		opt.m_bOutputHyphen = TRUE;
	else
		opt.m_bOutputHyphen = FALSE;
	m_pTextPage = IPDF_TextPage::CreateTextPage(m_pPage, opt);	
	if(!m_pTextPage)
		return NULL;
	if(m_pTextPage->ParseTextPage())
		return m_pTextPage;
	SAFE_DELETE(m_pTextPage);
	return NULL;
}

FS_BOOL FSPDFPageImp::init(FSPDFDoc* pFSPDFDoc, FS_INT32 pageIndex)
{
	if (!pFSPDFDoc || !((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument()) return FALSE;

	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument();
	CPDF_Dictionary* pPageDict = ((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument()->GetPage(pageIndex);
	if (!pPageDict)
		return FALSE;

	CPDF_Page* pPage = FX_NEW CPDF_Page();
	if (!pPage) return FALSE;
	pPage->Load(pPDFDoc, pPageDict);

	m_pFSPDFDoc = pFSPDFDoc;
	m_pPage = pPage;
	m_pageIndex = pageIndex;
	return TRUE;
}

FSPDFDoc* FSPDFPageImp::getDocument() const
{
	return m_pFSPDFDoc;
}

FSPDFDictionary* FSPDFPageImp::getDict() const
{
	if (!m_pPage || !m_pPage->m_pFormDict)
		return NULL;
	return (FSPDFDictionary*)(m_pPage->m_pFormDict);
}

FS_BOOL	FSPDFPageImp::isParsed() const
{
	if (!m_pPage) 
		return FALSE;
	return m_pPage->IsParsed();
}

FS_PROGRESSSTATE FSPDFPageImp::startParse(FS_DWORD flag , FSPause* pause, FS_BOOL isReparse/* = false*/)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!m_pPage
		|| flag < e_parsePageNormal || flag > (e_parsePageNormal | e_parsePageTextOnly /*| e_parseStreamOrder*/ | e_parseTextOutputHyphen)
		) 
		return FSCommonDefines::e_progressError;
	
	if (m_parseflag == flag)
	{
		if (m_pPage->IsParsed() && !isReparse)
			return FSCommonDefines::e_progressFinished;
	}
	m_parseflag = flag;

	CPDF_ParseOptions parseOption;
	if (e_parsePageTextOnly == (flag&e_parsePageTextOnly))
		parseOption.m_bTextOnly = TRUE;
	else
		parseOption.m_bTextOnly = FALSE;
	m_pPage->StartParse(&parseOption, isReparse);
	m_pParsePause = pause;
	SAFE_DELETE(m_pTextPage);
	return continueParse();
}

FS_PROGRESSSTATE FSPDFPageImp::continueParse()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!m_pPage) return FSCommonDefines::e_progressError;

	if (m_pParsePause)
	{
		CFSCRT_Pause ltPause(m_pParsePause);
		m_pPage->ContinueParse(&ltPause);
	}
	else
		m_pPage->ContinueParse(NULL);

	FS_PROGRESSSTATE state = (FS_PROGRESSSTATE)m_pPage->GetParseState();
	if (FSCommonDefines::e_progressFinished == state && m_pTextPage)
	{
		SAFE_DELETE(m_pTextPage);
		getTextPage();
	}
	return state;
}

FS_INT32 FSPDFPageImp::getIndex() const
{
	return m_pageIndex;
}

FS_FLOAT FSPDFPageImp::getHeight() const
{
	if (!m_pPage) return 0.0f;
	return m_pPage->GetPageHeight();
}

FS_FLOAT FSPDFPageImp::getWidth() const
{
	if (!m_pPage) return 0.0f;
	return m_pPage->GetPageWidth();
}

FS_INT32 getPageRotation(CPDF_Dictionary* pDict) 
{
	if (!pDict) return 0;
	while (pDict && !pDict->KeyExist(FX_BSTRC("Rotate"))) {
		CPDF_Object* pParent = pDict->GetElement(FX_BSTRC("Parent"));
		if (!pParent) break;

		pDict = pParent->GetDict();
	}

	if (pDict)
	{
		FX_INT32 rotate = pDict->GetInteger(FX_BSTRC("Rotate")) / 90;
		rotate %= 4;
		if (rotate < 0) rotate += 4;
		return rotate;
	}
	else
		return 0;
}

FS_ROTATION FSPDFPageImp::getRotation() const
{
	if (!m_pPage) return FSCommonDefines::e_rotation0;

	return (FS_ROTATION)getPageRotation(m_pPage->m_pFormDict);
}


FSMatrix FSPDFPageImp::getDisplayMatrix(FS_INT32 xPos, FS_INT32 yPos, FS_INT32 xSize, FS_INT32 ySize, FS_ROTATION rotate)
{
	FSMatrix fsMatrix;
	fsMatrix.set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	if (!m_pPage
		|| rotate < FSCommonDefines::e_rotation0 || rotate > FSCommonDefines::e_rotation270) 
		return fsMatrix;

	CFX_Matrix matrix(1, 0, 0, 1, 0, 0);
	m_pPage->GetDisplayMatrix(matrix, xPos, yPos, xSize, ySize, rotate);

	fsMatrix.set(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);
	return fsMatrix;
}

FSBitmap* FSPDFPageImp::loadThumbnail()
{
	if (!m_pPage || !m_pPage->m_pFormDict || !m_pPage->m_pDocument) return NULL;

	CPDF_Stream* pImageStream = m_pPage->m_pFormDict->GetStream("Thumb");
	//todo. If stream is NULL, we would create a new bitmap for thumbnail.
	if (!pImageStream)
		return NULL;

	CPDF_Image *pImage = m_pPage->m_pDocument->LoadImageF(pImageStream);
	if (!pImage) 
		return NULL;

	FX_DWORD MatteColor = 0;
	CFX_DIBSource* pMask = NULL;
	CFX_DIBSource* pDibSrc = pImage->LoadDIBSource(&pMask, &MatteColor);
	if (pMask)
	{
		delete pMask;
		pMask = NULL;
	}
	FSBitmap* pFSBitmap = FSCRT_Util_ConvertDIBSource2FSBitmap(pDibSrc);
	delete pDibSrc;
	return pFSBitmap;
}

FSRectF FSPDFPageImp::calcContentBBox(FS_CALCMARGINMODE mode)
{
	FSRectF fsRectF;
	fsRectF.set(0.0f, 0.0f, 0.0f, 0.0f);
	if (!isParsed() || mode < e_calcContentsBox || mode > e_calcDetection) 
		return fsRectF;

	FX_INT32 iMode = 0;
	if (e_calcDetection == mode)
		iMode = 3;
	CPDF_Rect pdfMargin;
	if (!FX_CalcPDFPageMargin(m_pPage, iMode, 95, 80, 16, 65, pdfMargin))
		return fsRectF;

	fsRectF.set(pdfMargin.left, pdfMargin.bottom, pdfMargin.right, pdfMargin.top);
	return fsRectF;
}

FS_BOOL FSPDFPageImp::initAnnotArray()
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

FSFormControl* FSPDFPageImp::GetFormControl(FS_INT32 index, FS_INT32* indexInAnnotList)
{
	FS_INT32 controlCount = 0;
	FS_INT32 annotCount = getAnnotCount();
	for (int i = 0; i < annotCount; i++) 
	{
		FSAnnot* pAnnot = (FSAnnot*)m_pAnnotsArray->GetAt(i);
		if (pAnnot)
		{
			FSM_ANNOTTYPE type = pAnnot->getType();
			if (FSAnnot::e_annotWidget == type)
			{
				if(controlCount == index)
				{
					if(indexInAnnotList) *indexInAnnotList = i;
					return (FSFormControl*)pAnnot;
				}
				controlCount++;
			}
		}
	}

	return NULL;
}

FS_INT32 FSPDFPageImp::CountFormControls()
{
	FS_INT32 controlCount = 0;
	FS_INT32 annotCount = getAnnotCount();
	for (int i = 0; i < annotCount; i++)
	{
		FSAnnot* pAnnot = (FSAnnot*)m_pAnnotsArray->GetAt(i);
		if (pAnnot)
		{
			FSM_ANNOTTYPE type = pAnnot->getType();
			if (FSAnnot::e_annotWidget == type)
			{
				controlCount++;
			}
		}
	}

	return controlCount;
}

FSFormFillerImp* FSPDFPageImp::getFormFiller()
{
	if(!m_pFSPDFDoc) return NULL;
	FSFormImp* pForm = (FSFormImp*)((FSPDFDocImp*)m_pFSPDFDoc)->getForm();
	if(!pForm) return NULL;
	return pForm->GetFormFiller();
}

FS_INT32 FSPDFPageImp::getAnnotCount()
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	initAnnotArray();
	if(m_pAnnotsArray)
		return m_pAnnotsArray->GetSize();
	return 0;
}

// createFSAnnot() is only for create a FSAnnot object based on annot dictionary.
// index = -1 means to add to the last.
FSAnnot* FSPDFPageImp::createFSAnnot(CPDF_Dictionary* pAnnotDict, FS_INT32 index)
{
	if (!pAnnotDict || index<-1) return NULL;

	CFX_ByteString bsType = pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE);
	FSM_ANNOTTYPE annotType = FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(bsType);

	FX_BOOL isMarkup = FSANNOT_Util_CheckAnnotIsMarkup(annotType);
	FSAnnot* pFsAnnot = NULL;
	switch (annotType)
	{
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
	case FSAnnot::e_annotFreeText:
		pFsAnnot = (FSFreeText*)FX_NEW FSPDFFreeTextImp(pAnnotDict, this);
		break;
	case FSAnnot::e_annotWidget:
		pFsAnnot = (FSFormControl*)FX_NEW FSFormControlImp(pAnnotDict, this);
		break;
	case FSAnnot::e_annotLine:
		pFsAnnot = (FSLine*)FX_NEW FSPDFLineImp(pAnnotDict, this);
		break;
	case FSAnnot::e_annotStamp:
		pFsAnnot = (FSStamp*)FX_NEW FSPDFStampImp(pAnnotDict, this);
		break;
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
	default:
		{
			if (isMarkup)
				pFsAnnot = (FSMarkup*)FX_NEW FSPDFMarkupImp(pAnnotDict, this);
			else
				pFsAnnot = (FSAnnot*)FX_NEW FSPDFAnnotImp(pAnnotDict, this);
		}
	}
	if (!pFsAnnot) return NULL;
	
	{
		FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
		if(!m_pAnnotsArray) m_pAnnotsArray = FX_NEW CFX_PtrArray;
		if (index<0 || index>=m_pAnnotsArray->GetSize())
			m_pAnnotsArray->Add(pFsAnnot);
		else
			m_pAnnotsArray->SetAt(index, pFsAnnot);
	}

	return pFsAnnot;
}

FSAnnot* FSPDFPageImp::getAnnot(FS_INT32 index)
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	FS_INT32 iAnnotCount = getAnnotCount();
	if (index < 0 || index >= iAnnotCount) return NULL;
	return (FSAnnot*)m_pAnnotsArray->GetAt(index);
}

FSAnnot* FSPDFPageImp::getAnnotAtDevicePos(const FSMatrix* matrix, FSPointF pos, FS_FLOAT tolerance)
{
	if (tolerance < 0.0f || tolerance > 30.0f) 
		return NULL;
	if(!m_pAnnotsArray && !initAnnotArray())
		return NULL;

	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	FS_INT32 iAnnotCount = getAnnotCount();
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
	for (FS_INT32 iValidIndex = iAnnotCount-1; iValidIndex >=0 ; iValidIndex--)
	{
		FSAnnot* pAnnot = (FSAnnot*)m_pAnnotsArray->GetAt(iValidIndex);
		if (!pAnnot)
			continue;
		
		if(FSAnnot::e_annotPopup == pAnnot->getType() || FSAnnot_Util_ChectAnnotIsReply((CPDF_Dictionary*)pAnnot->getDict()) || FSAnnot_Util_CheckIsStateAnnot(pAnnot))
			continue;

		FX_BOOL bIn = FSANNOT_Util_CheckAnnotIsAtPoint(pAnnot, pos.x, pos.y, tolerance, matrix);
		if (bIn)
			return pAnnot;
	}
	return NULL;
}

FSAnnot* FSPDFPageImp::getAnnotAtPos(FSPointF pos, FS_FLOAT tolerance)
{
	return getAnnotAtDevicePos(NULL, pos, tolerance);
}

FSAnnot* FSPDFPageImp::addAnnot(FSM_ANNOTTYPE annotType, FSRectF rect)
{
    LogOut("FSPDFPageImp::addAnnot, begin annotType=%d", annotType);
	if (!m_pPage->m_pFormDict 
		|| FALSE == FSANNOT_Util_CheckAnnotIsSupport(annotType) 
		|| FSAnnot::e_annotWidget == annotType) //Not support to add widget annot (form control &signature).
    {
        LogOut("FSPDFPageImp::addAnnot, not Support Annot, return null. annotType=%d", annotType);
		return NULL;
    }
	
	if (!CHECKVALID_FLOATRECT_PDF(rect))
	{
		if (FSAnnot::e_annotNote == annotType
			|| FSAnnot::e_annotFreeText == annotType
			|| FSAnnot::e_annotSquare == annotType
			|| FSAnnot::e_annotCircle == annotType
			|| FSAnnot::e_annotFileAttachment == annotType
			|| FSAnnot::e_annotStamp == annotType
			|| FSAnnot::e_annotCaret == annotType
			|| FSAnnot::e_annotLink == annotType)
		{
             LogOut("FSPDFPageImp::addAnnot, some annots not Support annotType=%d", annotType);
				return NULL;
		}
		else
			rect.set(0, 0, 0, 0);
	}

	return addAnnotImp(annotType, rect, TRUE);
}

FS_BOOL FSPDFPageImp::removeAnnot(FSAnnot* annot)
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	if (!annot || !annot->getDict()
		|| !m_pAnnotsArray || 0 == m_pAnnotsArray->GetSize()
		) 
		return FALSE;

	FS_DWORD dwAnnotObjNum = annot->getDict()->getObjNum();
	FSPDFPage* pAnnotPage = annot->getPage();
	if (dwAnnotObjNum<1 
		|| !pAnnotPage || pAnnotPage->getIndex() != this->getIndex() 
		)
		return FALSE;

	CPDF_AnnotList annotList(m_pPage);
	FS_INT32 annotCount = m_pAnnotsArray->GetSize();
	for(int i=0; i < annotList.Count(); i++)
	{
		if (annot && i<annotCount)
		{
			if((FSAnnot*)m_pAnnotsArray->GetAt(i) == annot){
				m_pAnnotsArray->RemoveAt(i);
				annotCount--;
				SAFE_DELETE(annot);
			}
		}

		CPDF_Annot* pAnnot = annotList.GetAt(i);
		if(!pAnnot || !pAnnot->m_pAnnotDict) continue;
		if(dwAnnotObjNum == pAnnot->m_pAnnotDict->GetObjNum())
		{
			annotList.Remove(i);
			setModified();
			return TRUE;
		}
	}
	return FALSE;
}

FS_BOOL FSPDFPageImp::hasTransparency()
{
	if (!m_pPage)
		return FALSE;

	return m_pPage->BackgroundAlphaNeeded();
}

FSFormControl* FSPDFPageImp::addControl(CPDF_FormControl* pFormControl, FSRectF rect)
{
	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	CPDF_Dictionary* pAnnotDict = pFormControl->GetWidget();
	CPDF_Array* pAnnotArray = pPageDict->GetArray(FX_BSTRC("Annots"));
	if(!pAnnotArray)
	{
		pAnnotArray = FX_NEW CPDF_Array;
		if (!pAnnotArray) return NULL;
		pPageDict->SetAt(FX_BSTRC("Annots"), pAnnotArray);
	}
	pAnnotArray->AddReference(m_pPage->m_pDocument, pAnnotDict);
	CPDF_Rect pdfRect(rect.left, rect.bottom, rect.right, rect.top);
	pAnnotDict->SetAtRect(FX_BSTRC("Rect"), pdfRect);
    CFX_ByteString bsNM;
    bsNM.Format("%x", (FX_DWORD)(FX_INTPTR)(void*)pFormControl);
    pAnnotDict->SetAtString(FSANNOT_STRINGKEY_UNIQUEID,bsNM);
	return (FSFormControl*)createFSAnnot(pAnnotDict, -1);
}

FSFormControl* FSPDFPageImp::getControlAtPos(FSPointF pos, FS_FLOAT tolerance)
{
	FSAnnot* annot = getAnnotAtPos(pos, tolerance);
	if(!annot || annot->getType() != FSAnnot::e_annotWidget)
		return NULL;
	return (FSFormControl*)annot;
}

FSFormControl* FSPDFPageImp::getControlByDict(CPDF_Dictionary* dict)
{
	if(!m_pAnnotsArray && !initAnnotArray())
		return NULL;
		
	FS_INT32 annotCount = m_pAnnotsArray->GetSize();
	for(int i=0; i < annotCount; i++)
	{
		FSAnnot* pAnnot = (FSAnnot*)m_pAnnotsArray->GetAt(i);
		if(pAnnot->getType() == FSAnnot::e_annotWidget &&
			((CPDF_Dictionary*)pAnnot->getDict()) == dict)
			return (FSFormControl*)pAnnot;
	}
	return NULL;
}

FS_BOOL FSPDFPageImp::removeAnnotByObjNum(FX_DWORD objNum)
{
	if(!m_pAnnotsArray && !initAnnotArray())
		return NULL;
		
	FS_INT32 annotCount = m_pAnnotsArray->GetSize();
	for(int i=0; i < annotCount; i++)
	{
		FSAnnot* pAnnot = (FSAnnot*)m_pAnnotsArray->GetAt(i);
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		if(pAnnotDict && pAnnotDict->GetObjNum() == objNum)
			return removeAnnot(pAnnot);
	}
	return FALSE;
}

FS_RESULT FSPDFPageImp::DeviceToPagePoint(const CFX_Point &devicePoint, CFX_PointF &pagePoint)
{
	return 0;
}

FS_RESULT FSPDFPageImp::DeviceToPageRect(const CFX_Rect &deviceRect, CFX_FloatRect &pageRect)
{
	return 0;
}

FS_RESULT FSPDFPageImp::PageToDevicePoint(const CFX_PointF &pagePoint, CFX_Point &devicePoint)
{
	return 0;
}

FS_RESULT FSPDFPageImp::PageToDeviceRect(const CFX_FloatRect &pageRect, CFX_Rect &deviceRect)
{
	return 0;
}

FS_RESULT FSPDFPageImp::OnMouseMove(FS_DWORD nFlags, const CFX_PointF& point)
{
// 	FSCRT_LOCKOBJECT(lock, &m_lock)
// 	FXSYS_assert(m_viewer != NULL);

	//CFSPDF_PageView* pageView = NULL;
	IFSPDF_AnnotHandler* pHandler = NULL;
	FSPointF pt = {point.x, point.y};
	FSAnnot* annot = getControlAtPos(pt,0.0);

	FSFormFillerImp* pFormFiller =	getFormFiller();
	//FS_RESULT ret =	m_viewer->GetAnnotAndViewByDevicePoint(point, &annot, &pageView);
	if (pFormFiller->m_pHoverAnnot != annot && NULL != pFormFiller->m_pHoverAnnot) {
		//ret = m_viewer->GetPageViewByAnnot(m_pHoverAnnot, pageView);
		//if (ret != FSCRT_ERRCODE_SUCCESS) return ret;

		if(FSAnnot::e_annotWidget != pFormFiller->m_pHoverAnnot->getType())
			return FSCRT_ERRCODE_SUCCESS;

		pHandler = pFormFiller->GetWidgetHandler();

		if (pHandler) {
			FS_RESULT ret = pHandler->OnMouseExit(pFormFiller->m_pHoverAnnot, this);
			if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_UNSUPPORTED) return ret;
		}

		//pageView = NULL;
		pFormFiller->m_pHoverAnnot = NULL;
	}
	if (!annot) return FSCRT_ERRCODE_SUCCESS;

	if(FSAnnot::e_annotWidget != annot->getType())
		return FSCRT_ERRCODE_SUCCESS;
	//ret = m_viewer->GetAnnotHandlerByAnnot(annot, &pHandler);
	pHandler = pFormFiller->GetWidgetHandler();
	if (annot != pFormFiller->m_pHoverAnnot) {
		if (pHandler) {
			FS_RESULT ret = pHandler->OnMouseEnter(annot, this);
			if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_UNSUPPORTED) return ret;
		}
		pFormFiller->m_pHoverAnnot = annot;
	}

	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	return pHandler->OnMouseMove(annot, this, nFlags, point);
}
FS_RESULT FSPDFPageImp::OnLButtonDown(FS_DWORD nFlags, const CFX_PointF& point)
{
	FSPointF pt = {point.x, point.y};
	FSAnnot* annot = getControlAtPos(pt,0.0);
	if (!annot)
	{
		FSFormFillerImp* pFormFiller = getFormFiller();
		if(pFormFiller->m_pFocusAnnot)  {pFormFiller->SetFocusAnnot(NULL);}
		return FSCRT_ERRCODE_SUCCESS;
	}

	if(FSAnnot::e_annotWidget != annot->getType())
		return FSCRT_ERRCODE_SUCCESS;

	FSFormFillerImp* pFormFiller = getFormFiller();
	IFSPDF_AnnotHandler* pHandler = pFormFiller->GetWidgetHandler();
	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	FS_BOOL bReadOnly = FALSE;
	FS_RESULT ret = pFormFiller->ST_IsWidgetReadOnly(annot, &bReadOnly);
	if (bReadOnly)
	{
		pFormFiller->SetFocusAnnot(NULL);
		return FSCRT_ERRCODE_SUCCESS;
	}

	//if the hidden bit and readonly bit of widget's flag is set, form control is non interactive
	FS_INT32 flag = annot->getFlags();
	if (flag&FSPDF_ANNOTFLAG_READONLY || flag&FSPDF_ANNOTFLAG_HIDDEN || flag&FSPDF_ANNOTFLAG_NOVIEW)
	{
		pFormFiller->SetFocusAnnot(NULL);
		return FSCRT_ERRCODE_SUCCESS;
	}

	ret = pHandler->OnLButtonDown(annot, this, nFlags, point);
	if (ret != FSCRT_ERRCODE_SUCCESS) return ret;

	if (annot && annot != pFormFiller->m_pFocusAnnot) {pFormFiller->SetFocusAnnot(annot);}

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT FSPDFPageImp::OnLButtonUp(FS_DWORD nFlags, const CFX_PointF& point)
{
	FSPointF pt = {point.x, point.y};
	FSAnnot* annot = getControlAtPos(pt,0.0);
	FS_RESULT ret = annot?FSCRT_ERRCODE_SUCCESS:FSCRT_ERRCODE_ERROR;
	//CFSPDF_PageView* pageView = NULL;
	//FS_RESULT ret =	m_viewer->GetAnnotAndViewByDevicePoint(point, &annot, &pageView);
	FSFormFillerImp* pFormFiller = getFormFiller();
	if (ret != FSCRT_ERRCODE_SUCCESS)
	{
		if(pFormFiller->m_pFocusAnnot)  {pFormFiller->SetFocusAnnot(NULL);}
		return ret;
	}

	if(FSAnnot::e_annotWidget != annot->getType())
		return FSCRT_ERRCODE_SUCCESS;
	IFSPDF_AnnotHandler* pHandler = pFormFiller->GetWidgetHandler();
	//ret = m_viewer->GetAnnotHandlerByAnnot(annot, &pHandler);
	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	ret = pHandler->OnLButtonUp(annot, this, nFlags, point);
	if (ret == FSCRT_ERRCODE_SUCCESS) return ret;

	if (annot && annot != pFormFiller->m_pFocusAnnot) {pFormFiller->SetFocusAnnot(annot);}

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT FSPDFPageImp::OnLButtonDbClick(FS_DWORD nFlags, const CFX_PointF& point)
{
	FSPointF pt = {point.x, point.y};
	FSAnnot* annot = getControlAtPos(pt,0.0);
	//CFSPDF_PageView* pageView = NULL;
	FS_RESULT ret = annot?FSCRT_ERRCODE_SUCCESS:FSCRT_ERRCODE_ERROR;
	//FS_RESULT ret =	m_viewer->GetAnnotAndViewByDevicePoint(point, &annot, &pageView);
	FSFormFillerImp* pFormFiller = getFormFiller();
	if (ret != FSCRT_ERRCODE_SUCCESS)
	{
		if(pFormFiller->m_pFocusAnnot)  {pFormFiller->SetFocusAnnot(NULL);}
		return ret;
	}

	if(FSAnnot::e_annotWidget != annot->getType())
		return FSCRT_ERRCODE_SUCCESS;
	IFSPDF_AnnotHandler* pHandler = pFormFiller->GetWidgetHandler();
	//ret = m_viewer->GetAnnotHandlerByAnnot(annot, &pHandler);
	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	ret = pHandler->OnLButtonDblClk(annot, this, nFlags, point);
	if (ret != FSCRT_ERRCODE_SUCCESS) return ret;

	if (annot && annot != pFormFiller->m_pFocusAnnot) {pFormFiller->SetFocusAnnot(annot);}

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT FSPDFPageImp::OnKeyDown(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	FSFormFillerImp* pFormFiller = getFormFiller();
	FSAnnot* pAnnot = pFormFiller->GetFocusAnnot();
	if(!pAnnot) return FSCRT_ERRCODE_SUCCESS;

	IFSPDF_AnnotHandler* pHandler = pFormFiller->GetWidgetHandler();
	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	return pHandler->OnKeyDown(pAnnot, nKeyCode, nFlags);
}

FS_RESULT FSPDFPageImp::OnKeyUp(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	FSFormFillerImp* pFormFiller = getFormFiller();
	FSAnnot* pAnnot = pFormFiller->GetFocusAnnot();
	if(!pAnnot) return FSCRT_ERRCODE_SUCCESS;

	IFSPDF_AnnotHandler* pHandler = pFormFiller->GetWidgetHandler();
	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	return pHandler->OnKeyUp(pAnnot, nKeyCode, nFlags);
}

FS_RESULT FSPDFPageImp::OnChar(FS_DWORD nKeyCode, FS_DWORD nFlags)
{
	FSFormFillerImp* pFormFiller = getFormFiller();
	FSAnnot* pAnnot = pFormFiller->GetFocusAnnot();
	if(!pAnnot) return FSCRT_ERRCODE_SUCCESS;

	IFSPDF_AnnotHandler* pHandler = pFormFiller->GetWidgetHandler();
	if (!pHandler) return FSCRT_ERRCODE_UNSUPPORTED;

	return pHandler->OnChar(pAnnot, nKeyCode, nFlags);
}

FS_INT32	FSPDFPageImp::getWatermarkAnnotType(FS_INT32 annotIndex)
{
	if (!m_pPage || annotIndex<0) return WMANNOTTYPE_NO;

	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	CPDF_AnnotList pAnnotList(m_pPage);
	FS_INT32 annotCount = pAnnotList.Count();
	if (annotIndex>=annotCount) return WMANNOTTYPE_NO;

	CPDF_Annot* pAnnot = pAnnotList.GetAt(annotIndex);
	if (!pAnnot)
		return WMANNOTTYPE_NO;
	return FSCRT_Util_GetWatermarkAnnotType(pAnnot->m_pAnnotDict);
}

FS_BOOL FSPDFPageImp::parserAnnotsForFlatten(FS_BOOL isDisplay, FS_DWORD nUsage, CPDF_RectArray* pRectArray, CPDF_ObjectArray* pObjectArray,CFX_DWordArray* pRemoveAnnotArray)
{
	if (!pRectArray || !pObjectArray) return FALSE;

    getContentsRectForFlatten(pRectArray);

	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	if(!pPageDict)
		return FALSE;

	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	CPDF_Array* pAnnots = pPageDict->GetArray(FX_BSTRC("Annots"));
	if (!pAnnots) return FALSE;

	FX_DWORD dwSize = pAnnots->GetCount();
	for (FS_INT32 i = 0; i < (FS_INT32)dwSize; i++){
		CPDF_Object* pObj = pAnnots->GetElementValue(i);
		if (!pObj)
            continue;

		if (pObj->GetType() != PDFOBJ_DICTIONARY)
            continue;
		
	
		CPDF_Dictionary* pAnnotDic = (CPDF_Dictionary*)pObj;
		if (!isNeedFlattenAnnot(pAnnotDic,isDisplay, nUsage))
			continue;

		parserStreamForFlatten(pAnnotDic, pRectArray, pObjectArray);

		FS_DWORD dwAnnotObjNum = pAnnotDic->GetObjNum();
		pRemoveAnnotArray->Add(dwAnnotObjNum);

		//Its parent had been deleted, so it make no sense to exist.
		if (pAnnotDic->KeyExist(FX_BSTRC("Popup")))
		{
			CPDF_Dictionary* pPopup = pAnnotDic->GetDict(FX_BSTRC("Popup"));
			if (pPopup)
			{
				dwAnnotObjNum = pPopup->GetObjNum();
				pRemoveAnnotArray->Add(dwAnnotObjNum);
			}
		}
	}
	return TRUE;
}

FS_BOOL FSPDFPageImp::flattenIsValiableRect(CPDF_Rect rect, CPDF_Rect rcPage)
{
	if (rect.IsEmpty()) return FALSE;

	if (!rcPage.IsEmpty()) {
		if (rect.left - rcPage.left < -10.000001f ||
			rect.right - rcPage.right > 10.000001f ||
			rect.top - rcPage.top > 10.000001f ||
			rect.bottom - rcPage.bottom < -10.000001f)
			return FALSE;
	}

	return TRUE;
}

FS_BOOL FSPDFPageImp::getContentsRectForFlatten(CPDF_RectArray* pRectArray)
{
	if(!m_pPage || !pRectArray)
		return FALSE;

	CFX_AffineMatrix matrix = m_pPage->GetPageMatrix();
	FX_POSITION pos = m_pPage->GetFirstObjectPosition();

	while (pos){
		CPDF_PageObject* pPageObject = m_pPage->GetNextObject(pos);
		if (!pPageObject) continue;

		CPDF_Rect rc;
		rc.left = pPageObject->m_Left;
		rc.right = pPageObject->m_Right;
		rc.bottom = pPageObject->m_Bottom;
		rc.top = pPageObject->m_Top;

		if (flattenIsValiableRect(rc, m_pPage->m_pFormDict->GetRect(FX_BSTRC("MediaBox"))))
			pRectArray->Add(rc);
	}

	return TRUE;
}

void FSPDFPageImp::parserStreamForFlatten(CPDF_Dictionary* pStream, CPDF_RectArray* pRectArray,CPDF_ObjectArray* pObjectArray)
{
	if (!pStream) return;
	if(!pRectArray || !pObjectArray)
		return;

	CPDF_Rect rect;

	if (pStream->KeyExist(FX_BSTRC("Rect")))
		rect = pStream->GetRect(FX_BSTRC("Rect"));
	else if (pStream->KeyExist(FX_BSTRC("BBox")))
		rect = pStream->GetRect(FX_BSTRC("BBox"));

	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	if (flattenIsValiableRect(rect, pPageDict->GetRect(FX_BSTRC("MediaBox"))))
		pRectArray->Add(rect);

	pObjectArray->Add(pStream);
}

FS_FLOAT FSPDFPageImp::flattenGetMinMaxValue(const CPDF_RectArray& array, FS_INT32 type, FS_INT32 value)
{
	FX_INT32 iCount = array.GetSize();
	if (iCount == 0) return 0;

	FX_FLOAT fRet = 0;
	switch (value){
	case FSPDF_VALUE_LEFT:
		fRet = array[0].left;
		for (FX_INT32 i = 1; i < iCount; i++)
			fRet = FSPDF_FLATTEN_GETVALUE(type, fRet, array[i].left);
		return fRet;
	case FSPDF_VALUE_TOP:
		fRet = array[0].top;
		for (FX_INT32 i = 1; i < iCount; i++)
			fRet = FSPDF_FLATTEN_GETVALUE(type, fRet, array[i].top);
		return fRet;
	case FSPDF_VALUE_RIGHT:
		fRet = array[0].right;
		for (FX_INT32 i = 1; i < iCount; i++)
			fRet = FSPDF_FLATTEN_GETVALUE(type, fRet, array[i].right);
		return fRet;
	case FSPDF_VALUE_BOTTOM:
		fRet = array[0].bottom;
		for (FX_INT32 i = 1; i < iCount; i++)
			fRet = FSPDF_FLATTEN_GETVALUE(type, fRet, array[i].bottom);
		return fRet;
	default:
		return 0;
	}
}
CPDF_Rect FSPDFPageImp::flattenCalculateRect(const CPDF_RectArray& pRectArray)
{
	CPDF_Rect rcRet;

	rcRet.left = flattenGetMinMaxValue(pRectArray, FSPDF_TYPE_MIN, FSPDF_VALUE_LEFT);
	rcRet.top = flattenGetMinMaxValue(pRectArray, FSPDF_TYPE_MAX, FSPDF_VALUE_TOP);
	rcRet.right = flattenGetMinMaxValue(pRectArray, FSPDF_TYPE_MAX, FSPDF_VALUE_RIGHT);
	rcRet.bottom = flattenGetMinMaxValue(pRectArray, FSPDF_TYPE_MIN, FSPDF_VALUE_BOTTOM);

	return rcRet;
}

void FSPDFPageImp::annotsRectForFlatten(CPDF_RectArray &RectArray)
{
	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;

	CPDF_Rect rcMerger = flattenCalculateRect(RectArray);
	CPDF_Rect rcOriginalMB = pPageDict->GetRect(FX_BSTRC("MediaBox"));

	if (pPageDict->KeyExist(FX_BSTRC("CropBox")))
		rcOriginalMB = pPageDict->GetRect(FX_BSTRC("CropBox"));

	if (rcOriginalMB.IsEmpty()) 	
		rcOriginalMB = CPDF_Rect(0.0f, 0.0f, 612, 792);

	rcMerger.left = FX_MAX(rcMerger.left, rcOriginalMB.left);
	rcMerger.right = FX_MIN(rcMerger.right, rcOriginalMB.right);
	rcMerger.top = FX_MIN(rcMerger.top, rcOriginalMB.top);
	rcMerger.bottom = FX_MAX(rcMerger.bottom, rcOriginalMB.bottom);

	CPDF_Rect rcOriginalCB;
	if (pPageDict->KeyExist(FX_BSTRC("ArtBox")))
		rcOriginalCB = pPageDict->GetRect(FX_BSTRC("ArtBox"));
	else
		rcOriginalCB = rcOriginalMB;

	if (!rcOriginalMB.IsEmpty()) {
		pPageDict->SetAtRect(FX_BSTRC("MediaBox"), rcOriginalMB);
	}

	if (!rcOriginalCB.IsEmpty()) {
		pPageDict->SetAtRect(FX_BSTRC("ArtBox"), rcOriginalCB);
	}
}

CFX_AffineMatrix FSPDFPageImp::flattenGetMatrix(CPDF_Rect rcAnnot, CPDF_Rect rcStream, CFX_AffineMatrix matrix)
{
	if(rcStream.IsEmpty())
		return CFX_AffineMatrix();

	matrix.TransformRect(rcStream);
	rcStream.Normalize();

	FX_FLOAT a = rcAnnot.Width() / rcStream.Width();
	FX_FLOAT d = rcAnnot.Height() / rcStream.Height();

	FX_FLOAT e = rcAnnot.left - rcStream.left * a;
	FX_FLOAT f = rcAnnot.bottom - rcStream.bottom * d;
	return CFX_AffineMatrix(a, 0, 0, d, e, f);
}

FS_BOOL FSPDFPageImp::isFlattened(FS_BOOL isDisplay, FS_INT32 options)
{
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
}

FS_BOOL FSPDFPageImp::isNeedFlattenAnnot(CPDF_Dictionary * pAnnotDic, FS_BOOL isDisplay, FS_INT32 options)
{
	
	FS_INT32 iWMAnnotType = FSCRT_Util_GetWatermarkAnnotType(pAnnotDic);
	if (WMANNOTTYPE_RDKTRIAL == iWMAnnotType || WMANNOTTYPE_GSDKTRIAL == iWMAnnotType)
	{ // Ignore RDK and GSDK trial watermark
		return FALSE;
	}

	CFX_ByteString sSubtype = pAnnotDic->GetString(FX_BSTRC("Subtype"));
	FX_BOOL bStandardAnnot = FSPDF_Util_CheckAnnotIsStandard(sSubtype);

	if (sSubtype == FSPDF_ANNOTTYPE_POPUP)
		return FALSE;

	if (options & FSPDFPageDefines::e_flattenOptionNoAnnot)
	{
		if (sSubtype != FSPDF_ANNOTTYPE_WIDGET)
			return FALSE;
	}
	else if (options & FSPDFPageDefines::e_flattenOptionNoFormControl)
	{
		if (sSubtype == FSPDF_ANNOTTYPE_WIDGET)
			return FALSE;
	}

	FS_INT32 nAnnotFlag = pAnnotDic->GetInteger(FX_BSTRC("F"));
	if (nAnnotFlag & ANNOTFLAG_HIDDEN)
		return FALSE;


	if (TRUE == isDisplay)
	{
		// ANNOTFLAG_INVISIBLE have no effect for standard annotations, by PDF refenerce 8.4.2.
		if (FALSE == bStandardAnnot)
		{
			if (nAnnotFlag & ANNOTFLAG_INVISIBLE)
				return FALSE;
		}
		if (nAnnotFlag & ANNOTFLAG_NOVIEW)
			return FALSE;
	}
	else
	{
		if (!(nAnnotFlag & ANNOTFLAG_PRINT))
			return FALSE;

		// It make no sense to print text, file attachment, sound annotations by Adobe.
		if (sSubtype == FSPDF_ANNOTTYPE_NOTE || sSubtype == FSPDF_ANNOTTYPE_FILEATTACHMENT
			|| sSubtype == FSPDF_ANNOTTYPE_SOUND)
			return FALSE;
	}
	return TRUE;
}

void FSPDFPageImp::generateContentForFlatten(CPDF_ObjectArray &ObjectArray)
{
	CPDF_Page *pPage = m_pPage;
	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	CPDF_Document* pDocument = m_pPage->m_pDocument;
	if (!pPage) return;
	CPDF_Dictionary *pRes = NULL;

	CPDF_Object* pResObj = pPage->GetPageAttr("Resources");
	if (pResObj && PDFOBJ_DICTIONARY == pResObj->GetType())
		pRes = pResObj->GetDict();

	if (!pRes){
		pRes = FX_NEW CPDF_Dictionary;
		if (!pRes) return;//ST_OOM
		pPageDict->SetAt(FX_BSTRC("Resources"), pRes);
	}

	CPDF_Stream* pNewXObject = FX_NEW CPDF_Stream(NULL, 0, FX_NEW CPDF_Dictionary);
	if (!pNewXObject) return;//ST_OOM
	FX_DWORD dwObjNum = pDocument->AddIndirectObject(pNewXObject);
	CPDF_Dictionary* pPageXObject = pRes->GetDict(FX_BSTRC("XObject"));
	if (!pPageXObject){
		pPageXObject = FX_NEW CPDF_Dictionary;
		if (!pPageXObject) return;//ST_OOM
		pRes->SetAt(FX_BSTRC("XObject"), pPageXObject);
	}

	CFX_ByteString key = "";
	FS_INT32 nStreams = ObjectArray.GetSize();

	if (nStreams > 0){
		for (FS_INT32 iKey = 0; ; iKey++) {
			char sExtend[5] = {0};
#if (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
			_itoa_s(iKey, sExtend, 5, 10);
#else
			FXSYS_itoa(iKey, sExtend, 10);
#endif //_FX_WINAPI_PARTITION_APP_
			key = CFX_ByteString("FFT") + CFX_ByteString(sExtend);

			if (!pPageXObject->KeyExist(key)) break;
		}
	}

	setPageContentsForFlatten(key);

	CPDF_Dictionary* pNewXORes = NULL;
	if (!key.IsEmpty()){
		pPageXObject->SetAtReference(key, pDocument, dwObjNum);
		CPDF_Dictionary* pNewOXbjectDic = pNewXObject->GetDict();
		pNewXORes = FX_NEW CPDF_Dictionary;
		if (!pNewXORes) return;//ST_OOM;
		pNewOXbjectDic->SetAt(FX_BSTRC("Resources"), pNewXORes);
		pNewOXbjectDic->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
		pNewOXbjectDic->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
		pNewOXbjectDic->SetAtInteger(FX_BSTRC("FormType"), 1);
		pNewOXbjectDic->SetAtName(FX_BSTRC("Name"), FX_BSTRC("FRM"));

		CFX_ByteString keyWord;
		CPDF_Page tempPage;
		tempPage.Load(pDocument, pPageDict);
		if (pPageDict->KeyExist(FX_BSTRC("CropBox")))
		{
			keyWord = FX_BSTRC("CropBox");
		}else{
			keyWord = FX_BSTRC("MediaBox");
		}
		CPDF_Object* pObject = tempPage.GetPageAttr(keyWord); 
		CPDF_Rect rcBBox;
		if(pObject->GetType() == PDFOBJ_ARRAY)
		{
			CPDF_Array* pArray = (CPDF_Array*)pObject;
			rcBBox = pArray->GetRect();
		}
		pNewOXbjectDic->SetAtRect(FX_BSTRC("BBox"), rcBBox);
	}

	for (FS_INT32 i = 0; i < nStreams; i++){
		CPDF_Dictionary* pAnnotDic = ObjectArray.GetAt(i);
		if (!pAnnotDic) continue;

		CPDF_Rect rcAnnot = pAnnotDic->GetRect(FX_BSTRC("Rect"));
		rcAnnot.Normalize();

		//CFX_ByteString sAnnotState = pAnnotDic->GetString(FX_BSTRC("AS"));
		CPDF_Dictionary* pAnnotAP = pAnnotDic->GetDict(FX_BSTRC("AP"));
		if (!pAnnotAP) continue;

		// Get the AP stream or subdirectory
		CPDF_Object* psub = pAnnotAP->GetElementValue(FX_BSTRC("N"));
		if (psub == NULL) continue;

		CPDF_Stream* pStream = NULL;
		if (psub->GetType() == PDFOBJ_STREAM) {
			// The only one stream, use it
			pStream = (CPDF_Stream*)psub;
		} else if (psub->GetType() == PDFOBJ_DICTIONARY) {
			// Need to check the annotation status
			CFX_ByteString as = pAnnotDic->GetString("AS");
			if (as.IsEmpty()) {
				// Sometimes a buggy PDF doesn't have AS for form widget, we need to use field value to determine which one is active
				// TESTDOC: Bug #7999 - web.pdf
				CFX_ByteString value = pAnnotDic->GetString(FX_BSTRC("V"));
				if (value.IsEmpty())
					value = pAnnotDic->GetDict(FX_BSTRC("Parent"))->GetString(FX_BSTRC("V"));
				if (value.IsEmpty() || !((CPDF_Dictionary*)psub)->KeyExist(value))
					as = FX_BSTRC("Off");
				else
					as = value;
			}
			pStream = ((CPDF_Dictionary*)psub)->GetStream(as);
		}

		if (NULL == pStream) continue;

		CPDF_Dictionary* pAPDic = pStream->GetDict();
		CFX_AffineMatrix matrix = pAPDic->GetMatrix(FX_BSTRC("Matrix"));

		CPDF_Rect rcStream;
		if (pAPDic->KeyExist(FX_BSTRC("Rect")))
			rcStream = pAPDic->GetRect(FX_BSTRC("Rect"));
		else if (pAPDic->KeyExist(FX_BSTRC("BBox")))
			rcStream = pAPDic->GetRect(FX_BSTRC("BBox"));

		if (rcStream.IsEmpty()) continue;

		CPDF_Object* pObj = pStream;
		if (pObj){		
			CPDF_Dictionary* pObjDic = pObj->GetDict();
			if (pObjDic){
				pObjDic->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
				pObjDic->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
			}
		}

		CPDF_Dictionary* pXObject = pNewXORes->GetDict(FX_BSTRC("XObject"));
		if (!pXObject){
			pXObject = FX_NEW CPDF_Dictionary;
			if (!pXObject) return ; //ST_OOM
			pNewXORes->SetAt(FX_BSTRC("XObject"), pXObject);
		}

		CFX_ByteString sFormName;
		sFormName.Format("F%d", i);
		FX_DWORD dwObjNum = pDocument->AddIndirectObject(pObj);
		pXObject->SetAtReference(sFormName, pDocument, dwObjNum);

		CPDF_StreamAcc acc;
		acc.LoadAllData(pNewXObject);

		FX_LPCBYTE pData = acc.GetData();
		CFX_ByteString sStream1 = "q ";
		CFX_ByteString sStream(pData, acc.GetSize());
		CFX_ByteString sTemp;

		sStream1 += sStream;

		CFX_AffineMatrix m = flattenGetMatrix(rcAnnot, rcStream, matrix);
		sTemp.Format("q %f 0 0 %f %f %f cm /%s Do Q\n", m.a, m.d, m.e, m.f, (FX_LPCSTR)sFormName);
		//sStream += sTemp;
		sStream1 += sTemp;
		sStream1 += " Q";

		pNewXObject->SetData((FX_LPCBYTE)sStream1, sStream1.GetLength(), FALSE, FALSE);
	}
}

void FSPDFPageImp::setPageContentsForFlatten(CFX_ByteString key)
{
	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	CPDF_Document* pDocument = m_pPage->m_pDocument;

	CPDF_Object* pContentsObj = pPageDict->GetStream(FX_BSTRC("Contents"));
	if (!pContentsObj)
		pContentsObj = pPageDict->GetArray(FX_BSTRC("Contents"));

	if (!pContentsObj){
		//Create a new contents dictionary
		if (!key.IsEmpty()){
			CPDF_Stream* pNewContents = FX_NEW CPDF_Stream(NULL, 0, FX_NEW CPDF_Dictionary);
			if (!pNewContents) return;
			pPageDict->SetAtReference(FX_BSTRC("Contents"), pDocument, pDocument->AddIndirectObject(pNewContents));

			CFX_ByteString sStream;
			sStream.Format("q 1 0 0 1 0 0 cm /%s Do Q", (FX_LPCSTR)key);
			pNewContents->SetData((FX_LPCBYTE)sStream, sStream.GetLength(), FALSE, FALSE);
		}
		return;
	}

	FS_INT32 iType = pContentsObj->GetType();
	CPDF_Array* pContentsArray = NULL;

	switch(iType){
	case PDFOBJ_STREAM:{
		pContentsArray = FX_NEW CPDF_Array;
		if (!pContentsArray) return ;//ST_OOM
		CPDF_Stream* pContents = (CPDF_Stream*)pContentsObj;
		FX_DWORD dwObjNum = pDocument->AddIndirectObject(pContents);
		CPDF_StreamAcc acc;
		acc.LoadAllData(pContents);
		CFX_ByteString sStream = "q\n";
		CFX_ByteString sBody = CFX_ByteString((FX_LPCSTR)acc.GetData(), acc.GetSize());
		sStream = sStream + sBody + "\nQ";
		pContents->SetData((FX_LPCBYTE)sStream, sStream.GetLength(), FALSE, FALSE);
		pContentsArray->AddReference(pDocument, dwObjNum);
		break;
					   }
	case PDFOBJ_ARRAY:{
		pContentsArray = (CPDF_Array*)pContentsObj;
		break;
					  }
	default:
		break;
	}	

	if (!pContentsArray) return;

	FX_DWORD dwObjNum = pDocument->AddIndirectObject(pContentsArray);
	pPageDict->SetAtReference(FX_BSTRC("Contents"), pDocument, dwObjNum);

	FS_INT32 nArrSize = pContentsArray->GetCount();
	FS_BOOL bFirst = TRUE;
	FS_INT32 i = 0;
	for (i = 0; i < nArrSize; i++)
	{
		CPDF_Stream* pContentStream = pContentsArray->GetStream(i);
		if (pContentStream){
			CPDF_StreamAcc acc;
			acc.LoadAllData(pContentStream);
			CFX_ByteString bstrStream;
			if (bFirst) {
				bstrStream += "q ";
				bFirst = FALSE;
			}
			bstrStream += CFX_ByteStringC(acc.GetData(), acc.GetSize());
			bstrStream = bstrStream + ' ';

			pContentStream->SetData((FX_LPCBYTE)bstrStream, bstrStream.GetLength(), FALSE, FALSE);
		}
	}
	for (i = nArrSize - 1; i >= 0; i--) {
		CPDF_Stream* pContentStream = pContentsArray->GetStream(i);
		if (pContentStream){
			CPDF_StreamAcc acc;
			acc.LoadAllData(pContentStream);
			CFX_ByteString bstrStream = CFX_ByteStringC(acc.GetData(), acc.GetSize());
			bstrStream += " Q"; 
			pContentStream->SetData((FX_LPCBYTE)bstrStream, bstrStream.GetLength(), FALSE, FALSE);
			break;
		}
	}

	if (!key.IsEmpty()){
		CPDF_Stream* pNewContents = FX_NEW CPDF_Stream(NULL, 0, FX_NEW CPDF_Dictionary);
		if (!pNewContents) return ; //ST_OOM
		dwObjNum = pDocument->AddIndirectObject(pNewContents);
		pContentsArray->AddReference(pDocument, dwObjNum);

		CFX_ByteString sStream;
		sStream.Format("q 1 0 0 1 0 0 cm /%s Do Q", (FX_LPCSTR)key);
		pNewContents->SetData((FX_LPCBYTE)sStream, sStream.GetLength(), FALSE, FALSE);
	}
}

FS_BOOL FSPDFPageImp::removeAnnotForFlatten(CFX_DWordArray removeAnnotArray)
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	for(int i=0; i<removeAnnotArray.GetSize(); i++)
	{
		FS_BOOL ret = deleteAnnotByObjNum(removeAnnotArray.GetAt(i));
		if (!ret)
			return ret;
	}
	return TRUE;
}

FS_BOOL FSPDFPageImp::deleteAnnotByObjNum(FX_DWORD objNum)
{
	if (!m_pPage) 
		return FALSE;
	CPDF_Dictionary* pPageDict = m_pPage->m_pFormDict;
	if (!pPageDict)
		return FALSE;

	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);

	CPDF_Array* pAnnotsArray = pPageDict->GetArray(FX_BSTRC("Annots"));
	if(!pAnnotsArray || pAnnotsArray->GetCount() == 0)
		return FALSE;
	for (FS_INT32 j=(FS_INT32)pAnnotsArray->GetCount()-1 ; j >= 0; j--)
	{
		CPDF_Object* pAnnotDict = pAnnotsArray->GetElementValue(j);
		if (!pAnnotDict) return FALSE;

		if (((CPDF_Dictionary*)pAnnotDict)->GetObjNum() == objNum)
		{
			pAnnotsArray->RemoveAt(j);

			if(m_pAnnotsArray && m_pAnnotsArray->GetSize())
			{
                for(int i=m_pAnnotsArray->GetSize()-1; i>=0; i--)
                {
                    FSAnnot* annot = (FSAnnot*)m_pAnnotsArray->GetAt(i);
                    if(pAnnotDict == (CPDF_Dictionary*)annot->getDict())
                    {
                        delete annot;
                        m_pAnnotsArray->RemoveAt(i);
                    }
                }

			}
			break;
		}
	}
	return TRUE;

}
FS_BOOL	FSPDFPageImp::flatten(FS_BOOL isDisplay, FS_DWORD options)
{

	if (options > (FSPDFPageDefines::e_flattenOptionAll | FSPDFPageDefines::e_flattenOptionNoFormControl | FSPDFPageDefines::e_flattenOptionNoAnnot))
		return FALSE;

	if (isFlattened(isDisplay, options))
		return TRUE;

	if (!m_pPage) return FALSE;

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

	setModified();
	return TRUE;

}

FSAnnot*	FSPDFPageImp::getAnnotbyDict(CPDF_Dictionary* pAnnotDict)
{
	if (!pAnnotDict) return NULL;

	FS_INT32 nAnnotCount = getAnnotCount();
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		FSAnnot* pAnnot = getAnnot(i);
		if (pAnnot && pAnnot->getDict()->getObjNum() == pAnnotDict->GetObjNum())
			return pAnnot;
	}
	return NULL;
}

FS_BOOL	FSPDFPageImp::setAnnotGroup(FSMarkup** annotArray, FS_INT32 arrayCount, FS_INT32 headerIndex)
{
	// Judge the validation of input parameters.
	if (!annotArray || arrayCount<2 || headerIndex<0 || headerIndex>=arrayCount)
		return FALSE;
	for(FS_INT32 i=0; i<arrayCount; i++)
	{
		if (!annotArray[i] || annotArray[i]->getPage() != this || !annotArray[i]->isMarkup())
			return FALSE;
		// Check if a markup annotation has been added to annotArray more than once.
		for (FS_INT32 j = i + 1; j < arrayCount; j++)
		{
			if (annotArray[i] == annotArray[j])
				return FALSE;
		}
	}

	// Group input markup annotations
	// ATTENTION: assume that, the new group is Group-0, and a markup annot in annotArray is also in another group, Group-1.
	// This function will merge Group-1 to Group-0 with new group header.
	// For header
	// a. If it's not in any group yet, no change.
	// b. If it's already in another group and also be the header, no change.
	// c. If it's already in another group but not the header, change it to be the new header in its old group.
	// For other annotation in annotArray, also merge their old groups to the new one.

	// Prepare ref obj to new header.
	CPDF_Dictionary* pHeaderDict = (CPDF_Dictionary*)annotArray[headerIndex]->getDict();
	if (!pHeaderDict)
		return FALSE;
	FS_DWORD dwHeaderObjNum = pHeaderDict->GetObjNum();
	if (0 == dwHeaderObjNum)
		dwHeaderObjNum = m_pPage->m_pDocument->AddIndirectObject(pHeaderDict);
	CPDF_Reference* pHeaderRef = FX_NEW CPDF_Reference(m_pPage->m_pDocument, dwHeaderObjNum);
	if (!pHeaderRef) return FALSE;

	// Check all elements in annotArray to merge all the old groups to the new one.
	CFX_MapPtrToPtr mapNewElements;
	for (FS_INT32 i=0; i<arrayCount; i++)
		getOldGroupElements(annotArray[i], mapNewElements);

	// Set each element in mapNewElements:
	// for header, ensure no /RT and /IRT
	// for rest, set new relationship ship to new header.
	FX_POSITION pos = mapNewElements.GetStartPosition();
	while(pos)
	{
		FSMarkup* pMarkup = NULL;
		FSMarkup* pTemp = NULL;
		mapNewElements.GetNextAssoc(pos, (void*&)pMarkup, (void*&)pTemp);
		CPDF_Dictionary* pTempDict = (CPDF_Dictionary*)pMarkup->getDict();
		if (annotArray[headerIndex] == pMarkup)
		{
			//Update new header's dictionary: remove /RT and /IRT
			pHeaderDict->RemoveAt(FSANNOT_DICTKEY_IRT);
			pHeaderDict->RemoveAt(FSANNOT_NAMEKEY_RT);
		}
		else
		{
			// Set new relationship ship to new header: set /RT = "Group" and /IRT to new header.
			pTempDict->SetAtName(FSANNOT_NAMEKEY_RT, "Group");
			pTempDict->SetAt(FSANNOT_DICTKEY_IRT, pHeaderRef->Clone());
		}
	}
	mapNewElements.RemoveAll();
	pHeaderRef->Release();

	setModified();
	return TRUE;
}

FS_BOOL FSPDFPageImp::getOldGroupElements(FSMarkup* pMarkup, CFX_MapPtrToPtr& mapNewElements)
{
	if (!pMarkup || !pMarkup->isMarkup()) return FALSE;

	FS_INT32 oldCount = pMarkup->getGroupElementCount();
	if (oldCount<2)
	{// pMarkup is not in other group, or that group only has itself. So just add pMarkup to the map.
		mapNewElements.SetAt(pMarkup, pMarkup);
	}
	else
	{
		// Set all the elements in old group to map
		for (FS_INT32 i=0; i<oldCount; i++)
		{
			FSMarkup* pTemp = pMarkup->getGroupElement(i);
			if (pTemp)
				mapNewElements.SetAt(pTemp, pTemp);
		}
	}
	return TRUE;
}

FSAnnot* FSPDFPageImp::addAnnotImp(FSM_ANNOTTYPE annotType, FSRectF rect, FS_BOOL bNeedDefaultAP)
{
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	if (!m_pAnnotsArray) 
		getAnnotCount();
	if (!m_pAnnotsArray)
    {
        LogOut("FSPDFPageImp::addAnnotImp, m_pAnnotsArray is null. annotType=%d", annotType);
		return NULL;
    }
	CPDF_Dictionary* pAnnotDict = FSANNOT_UTIL_InsertAnnot(annotType, -1, rect, m_pPage->m_pFormDict, ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument());
	FSAnnot* pNewAnnot = createFSAnnot(pAnnotDict, -1);
     LogOut("FSPDFPageImp::addAnnotImp, pNewAnnot=%x", pNewAnnot);
	if (pNewAnnot && bNeedDefaultAP)
		FSANNOT_UTIL_SetDefaultProperties(pNewAnnot);
	if (pAnnotDict || pNewAnnot)
		setModified();
	return pNewAnnot;
}

FSSignature* FSPDFPageImp::getSignatureAtPos(FSPointF pos, FS_FLOAT tolerance)
{
	if (!m_pPage || !m_pFSPDFDoc 
		|| tolerance<0.0 || tolerance>30.0f) 
		return NULL;

	CPDF_Signature* pSig = ((FSPDFDocImp*)m_pFSPDFDoc)->getSignatureAtPos(m_pPage, pos, tolerance);
	if (!pSig) return NULL;

#if defined(ENABLE_SIGNATURE)
	return (FSSignature*)getAnnotbyDict(pSig->GetSignatureDict());
#else
	return NULL;
#endif
}

FSSignature* FSPDFPageImp::addSignature(FSRectF rect)
{
	if (!m_pPage || !m_pFSPDFDoc 
		|| !CHECKVALID_FLOATRECT_PDF(rect))
		return NULL;

#if defined(ENABLE_SIGNATURE)
	// Create a new CPDF_Signature
	CPDF_Signature* pNewSig = FX_NEW CPDF_Signature();
	if (!pNewSig) return NULL;
	// Add the new CPDF_Signature to document
	if (!((FSPDFDocImp*)m_pFSPDFDoc)->addSignature(pNewSig, m_pPage, rect))
	{
		delete pNewSig;
		return NULL;
	}
	// Create V dictionary in signature field dictionary.
	pNewSig->CreateSigVDict();
	// Add the new signature to annot array in FSPDFPageImp.
	FSCRT_LOCKOBJECT(lock, &m_lockAnnots);
	if (!m_pAnnotsArray) 
		getAnnotCount();
	if (!m_pAnnotsArray)
		return NULL;
	// Add new control in CPDF_InterForm for this new signature field. 
	FSFormImp* pForm = (FSFormImp*)((FSPDFDocImp*)m_pFSPDFDoc)->getForm();
	if(pForm && pForm->ST_GetSTInterForm())
		pForm->ST_GetSTInterForm()->m_pInterForm->AddControl(pNewSig->GetSignatureDict(), pNewSig->GetSignatureDict());

	FSSignature* pNewFSSig = createFSSignature(pNewSig, -1, TRUE, TRUE);
	if (pNewFSSig)
		setModified();
	return pNewFSSig;
#else
	return NULL;
#endif
}

FSSignature*	FSPDFPageImp::createFSSignature(CPDF_Dictionary* pSigFieldDict, FS_INT32 index, FS_BOOL bNewAdd, FS_BOOL bCreateVDict)
{
	if (!m_pFSPDFDoc || !pSigFieldDict || index<-1) return NULL;
	if (!FSANNOT_Util_IsSignatureField(pSigFieldDict)) return NULL;

#if defined(ENABLE_SIGNATURE)
	// Get the CPDF_Signature object.
	CPDF_SignatureEdit* pSigEdit = ((FSPDFDocImp*)m_pFSPDFDoc)->getSigantureEdit();
	if (!pSigEdit) return NULL;
	CPDF_Signature* pSig = NULL;
	FS_INT32 nSigCount = m_pFSPDFDoc->getSignatureCount();
	for (FS_INT32 i=0; i<nSigCount; i++)
	{
		pSig = pSigEdit->GetSignature(i);
		if (!pSig) continue;
		if (pSig->GetSignatureDict()->GetObjNum() == pSigFieldDict->GetObjNum())
			break;
	}
	if (!pSig) return NULL;
	return createFSSignature(pSig, index, bNewAdd, bCreateVDict);
#else
	return NULL;
#endif
}

FSSignature*	FSPDFPageImp::createFSSignature(CPDF_Signature* pSig, FS_INT32 index, FS_BOOL bNewAdd, FS_BOOL bCreateVDict)
{
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
}

