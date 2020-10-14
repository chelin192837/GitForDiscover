#include "../include/common.h"
#include "../include/fs_action.h"
#include "../include/fs_pdfpage.h"
#include "../include/fs_pdfobject.h"
#include "fs_util.h"
#include "inc/fs_actionImp.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_log.h"

#include "ofd/fr_ofdactionImp.h"

extern bool isOFDPage(FSPDFPage* page);

/////////////////////////////
//class FSDestination
FSDestination *FSDestination::createXYZ(FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT zoomFactor)
{
	FSRDK_LOG_FUNC(WSTR_FORMAT("FSDestination::createXYZ(%d, %f, %f, %f)"), page, left, top, zoomFactor);
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomXYZ, page, left, top, 0, 0, zoomFactor);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomXYZ, page, left, top, 0, 0, zoomFactor);
}

FSDestination *FSDestination::createFitPage(FSPDFPage* page)
{
	FSRDK_LOG_FUNC(WSTR_FORMAT("FSDestination::createFitPage(%d)"), page);
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitPage, page, 0, 0, 0, 0, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitPage, page, 0, 0, 0, 0, 0);
}

FSDestination *FSDestination::createFitHorz(FSPDFPage* page, FS_FLOAT top)
{
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitHorz, page, 0, top, 0, 0, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitHorz, page, 0, top, 0, 0, 0);
}

FSDestination *FSDestination::createFitVert(FSPDFPage* page, FS_FLOAT left)
{
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitVert, page, left, 0, 0, 0, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitVert, page, left, 0, 0, 0, 0);
}

FSDestination *FSDestination::createFitRect(FSPDFPage* page, FS_FLOAT left, FS_FLOAT bottom, FS_FLOAT right, FS_FLOAT top)
{
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitRect, page, left, top, right, bottom, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitRect, page, left, top, right, bottom, 0);
}

FSDestination *FSDestination::createFitBBox(FSPDFPage* page)
{
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitBBox, page, 0, 0, 0, 0, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitBBox, page, 0, 0, 0, 0, 0);
}

FSDestination *FSDestination::createFitBHorz(FSPDFPage* page, FS_FLOAT top)
{
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitBHorz, page, 0, top, 0, 0, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitBHorz, page, 0, top, 0, 0, 0);
}

FSDestination *FSDestination::createFitBVert(FSPDFPage* page, FS_FLOAT left)
{
	if (isOFDPage(page)) {
		return FR_OFDDestImp::create(FSCommonDefines::e_zoomFitBVert, page, left, 0, 0, 0, 0);
	}
	return FSDestinationImp::create(FSCommonDefines::e_zoomFitBVert, page, left, 0, 0, 0, 0);
}

////////////////////////////
// Class FSDestinationImp
FSDestinationImp* FSDestinationImp::create(FS_ZOOMMODE zoomMode, FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom, FS_FLOAT zoomFactor)
{
	if (!page) return NULL;

	FSDestinationImp* pDest = FX_NEW FSDestinationImp();
	if (!pDest) return NULL;

	FSPDFDictionary* pPageDict = page->getDict();
	if (!pPageDict) {
		pDest->release();
		return NULL;
	}
	FS_DWORD pageObjNum = pPageDict->getObjNum();
	FSPDFDocImp* pDocImp = (FSPDFDocImp*)((FSPDFPageImp*)page)->getDocument();
	if (!pDocImp) {
		pDest->release();
		return NULL;
	}

	CPDF_Array* pDestArray = CPDF_Array::Create();	
	if (!pDestArray)
	{
		pDest->release();
		return NULL;
	}
	pDestArray->Add(CPDF_Reference::Create(pDocImp->getPDFDocument(), pageObjNum));
	switch(zoomMode)
	{
	case FSCommonDefines::e_zoomXYZ:	
		pDestArray->Add(FX_NEW CPDF_Name("XYZ"));
		pDestArray->AddNumber(left);
		pDestArray->AddNumber(top);
		pDestArray->AddNumber(zoomFactor);
		break;
	case FSCommonDefines::e_zoomFitPage:
		pDestArray->Add(FX_NEW CPDF_Name("Fit"));
		break;
	case FSCommonDefines::e_zoomFitHorz:
		pDestArray->Add(FX_NEW CPDF_Name("FitH"));
		pDestArray->AddNumber(top);
		break;
	case FSCommonDefines::e_zoomFitVert:
		pDestArray->Add(FX_NEW CPDF_Name("FitV"));
		pDestArray->AddNumber(left);
		break;
	case FSCommonDefines::e_zoomFitRect:
		pDestArray->Add(FX_NEW CPDF_Name("FitR"));
		pDestArray->AddNumber(left);
		pDestArray->AddNumber(bottom);
		pDestArray->AddNumber(right);
		pDestArray->AddNumber(top);
		break;
	case FSCommonDefines::e_zoomFitBBox:
		pDestArray->Add(FX_NEW CPDF_Name("FitB"));
		break;
	case FSCommonDefines::e_zoomFitBHorz:
		pDestArray->Add(FX_NEW CPDF_Name("FitBH"));
		pDestArray->AddNumber(top);
		break;
	case FSCommonDefines::e_zoomFitBVert:
		pDestArray->Add(FX_NEW CPDF_Name("FitBV"));
		pDestArray->AddNumber(left);
		break;
	}

	pDest->m_pDestArray = pDestArray;
	pDest->m_pageIndex = page->getIndex();
	return pDest;
}

FSDestination* FSDestinationImp::createFromPDFDest(FSPDFDoc* pFSPDFDoc, CPDF_Dest pdfDest)
{
	if (!pFSPDFDoc || !((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument() || !pdfDest.m_pObj)
		return NULL;
	FSDestination* pRetDest = NULL;
	CPDF_Document* pDoc = ((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument();
	CPDF_Dest actDest = pdfDest;
	FS_INT32 objType = pdfDest.m_pObj->GetType();
	switch(objType)
	{
	case PDFOBJ_NAME:
	case PDFOBJ_STRING:
		{
			CFX_ByteString strName = pdfDest.GetRemoteName();
			CPDF_NameTree nameTree(pDoc, FX_BSTRC("Dests"));
			CPDF_Array* nameDest = nameTree.LookupNamedDest(pDoc, strName);
			if (!nameDest) return NULL;
			actDest = CPDF_Dest(nameDest);
		}
	case PDFOBJ_ARRAY:
		break;
	default:
		return NULL;
	}

	FS_INT32 iPageIndex = actDest.GetPageIndex(((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument());
	// In FSDestination, FSPDFPageImp::retain() will be called.
	// So here, no need to retain the page.
	FSPDFPage* pFSPDFPage = ((FSPDFDocImp*)pFSPDFDoc)->getPageImp(iPageIndex, FALSE);
	if (!pFSPDFPage) return NULL;
	FS_INT32 iZoomMode = actDest.GetZoomMode();
	switch(iZoomMode)
	{
	case PDFZOOM_XYZ:
		pRetDest = FSDestination::createXYZ(pFSPDFPage, actDest.GetParam(0), actDest.GetParam(1), actDest.GetParam(2));
		break;
	case PDFZOOM_FITPAGE:
		pRetDest = FSDestination::createFitPage(pFSPDFPage);
		break;
	case PDFZOOM_FITHORZ:
		pRetDest = FSDestination::createFitHorz(pFSPDFPage, actDest.GetParam(0));
		break;
	case PDFZOOM_FITVERT:
		pRetDest = FSDestination::createFitVert(pFSPDFPage, actDest.GetParam(0));
		break;
	case PDFZOOM_FITRECT:
		pRetDest = FSDestination::createFitRect(pFSPDFPage, actDest.GetParam(0), actDest.GetParam(1), actDest.GetParam(2), actDest.GetParam(3));
		break;
	case PDFZOOM_FITBBOX:
		pRetDest = FSDestination::createFitBBox(pFSPDFPage);
		break;
	case PDFZOOM_FITBHORZ:
		pRetDest = FSDestination::createFitBHorz(pFSPDFPage, actDest.GetParam(0));
		break;
	case PDFZOOM_FITBVERT:
		pRetDest = FSDestination::createFitBVert(pFSPDFPage, actDest.GetParam(0));
		break;
	}
	return pRetDest;
}

FSDestinationImp::FSDestinationImp()
	:m_pDestArray(NULL)
	,m_pageIndex(-1)
	,m_nCountRef(1)
{
}

FSDestinationImp::~FSDestinationImp()
{
    if (m_pDestArray)
    {
        m_pDestArray->Release();
        m_pDestArray = NULL;
    }
}

FSDestinationImp* FSDestinationImp::retain()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_nCountRef++;
	return this;
}

void FSDestinationImp::release()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		m_nCountRef --;	
		if (m_nCountRef != 0) {
			return ;
		}
	}
	delete this;
}

FS_INT32 FSDestinationImp::getPageIndex() const
{
	return m_pageIndex;
}

FS_ZOOMMODE FSDestinationImp::getZoomMode() const
{
	if(!m_pDestArray) 
		return (FS_ZOOMMODE)0;
	static const char* s_modes[] = {"","XYZ","Fit","FitH","FitV","FitR","FitB","FitBH","FitBV"};
	CFX_ByteString bsMode = m_pDestArray->GetString(1);
	if(bsMode.IsEmpty()) 
		return (FS_ZOOMMODE)0;
	for(int i=0; i<sizeof(s_modes)/sizeof(const char*); i++)
	{
		if(bsMode.Equal(CFX_ByteString(s_modes[i])))
			return (FS_ZOOMMODE)i;
	}
	return (FS_ZOOMMODE)0;
}

FS_FLOAT FSDestinationImp::getLeft() const
{
	if(!m_pDestArray) return 0.0f;
	FS_ZOOMMODE zoomMode = getZoomMode();
	switch(zoomMode)
	{
		case FSCommonDefines::e_zoomXYZ:
		case FSCommonDefines::e_zoomFitVert:
		case FSCommonDefines::e_zoomFitRect:
		case FSCommonDefines::e_zoomFitBVert:
			return m_pDestArray->GetFloat(2);
		default:
			return 0.0f;
	}
}

FS_FLOAT FSDestinationImp::getTop() const
{
	if(!m_pDestArray) return 0.0f;
	FS_ZOOMMODE zoomMode = getZoomMode();
	switch(zoomMode)
	{
	case FSCommonDefines::e_zoomXYZ:
		return m_pDestArray->GetFloat(3);
	case FSCommonDefines::e_zoomFitHorz:
	case FSCommonDefines::e_zoomFitBHorz:
		return m_pDestArray->GetFloat(2);
	case FSCommonDefines::e_zoomFitRect:
		return m_pDestArray->GetFloat(5);
	default:
		return 0.0f;
	}
}

FS_FLOAT FSDestinationImp::getRight() const
{
	if(!m_pDestArray) return 0.0f;
	FS_ZOOMMODE zoomMode = getZoomMode();
	switch(zoomMode)
	{
	case FSCommonDefines::e_zoomFitRect:
		return m_pDestArray->GetFloat(4);
	default:
		return 0.0f;
	}
}

FS_FLOAT FSDestinationImp::getBottom() const
{
	if(!m_pDestArray) return 0.0f;
	FS_ZOOMMODE zoomMode = getZoomMode();
	switch(zoomMode)
	{
	case FSCommonDefines::e_zoomFitRect:
		return m_pDestArray->GetFloat(3);
	default:
		return 0.0f;
	}
}

FS_FLOAT FSDestinationImp::getZoomFactor() const
{
	if(!m_pDestArray) return 0.0f;
	FS_ZOOMMODE zoomMode = getZoomMode();
	switch(zoomMode)
	{
	case FSCommonDefines::e_zoomXYZ:
		return m_pDestArray->GetFloat(4);
	default:
		return 0.0f;
	}
}

///////////////////////////////////////////
// Class FSAction
FSAction* FSAction::create(FSPDFDoc* doc, FS_ACTIONTYPE actionType)
{
	if (!doc) 
		return NULL;

	CPDF_Action newAct = CPDF_Action::CreateAction(((FSPDFDocImp*)doc)->getPDFDocument(), (CPDF_Action::ActionType)actionType);
	if (!newAct)
		return NULL;
	return FSActionImp::create(doc, newAct.m_pDict, FALSE);
}

///////////////////////////////////////////
// Class FSActionImp
FSAction* FSActionImp::create(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict, FS_BOOL bCreateUnsupport)
{
	if (!pFSPDFDoc || !pActDict) return NULL;

	CPDF_Action action(pActDict);
	CPDF_Action::ActionType actType = action.GetType();
	switch(actType)
	{
	case CPDF_Action::GoTo:
		{
			FSGotoActionImp* pNewGotoAct = FX_NEW FSGotoActionImp();
			if (pNewGotoAct)
				pNewGotoAct->init(pFSPDFDoc, pActDict);
			return (FSGotoAction*)pNewGotoAct;
		}
	case CPDF_Action::URI:
		{
			FSURIActionImp* pNewURIAct = FX_NEW FSURIActionImp();
			if (pNewURIAct)
				pNewURIAct->init(pFSPDFDoc, pActDict);
			return (FSURIAction*)pNewURIAct;
		}
	default:
		{
			if (bCreateUnsupport)
			{
				FSActionImp* pNewAct = FX_NEW FSActionImp();
				if (pNewAct)
					pNewAct->init(pFSPDFDoc, pActDict);
				return (FSAction*)pNewAct;
			}
			else
				return NULL;
		}
	}
}

FSAction*	FSActionImp::retain(FSAction* pOrgAct)
{// This can only be used for supported types -- which can be used in FSAction::create().
	switch(pOrgAct->getType())
	{
	case FSActionDefines::e_actionTypeGoto:
		return ((FSGotoActionImp*)(FSGotoAction*)pOrgAct)->retain();
	case FSActionDefines::e_actionTypeURI:
		return ((FSURIActionImp*)(FSURIAction*)pOrgAct)->retain();
	default:
		return NULL;
	}
}

FSActionImp::FSActionImp()
	:m_pActiontDict(NULL)
	,m_pFSPDFDoc(NULL)
	,m_pSubActionArray(NULL)
	,m_nCountRef(1)
{

}

FSActionImp::~FSActionImp()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
		if (m_pSubActionArray) 
		{
			for (FS_INT32 i = 0; i < m_pSubActionArray->GetSize(); i++)
			{
				FSAction* pAct = (FSAction*)m_pSubActionArray->GetAt(i);
				if (pAct)
					pAct->release();
			}
			m_pSubActionArray->RemoveAll();

			SAFE_DELETE(m_pSubActionArray);
		}
	}

	m_pFSPDFDoc = NULL;
	m_pActiontDict = NULL;
}

void	FSActionImp::init(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict)
{
	m_pFSPDFDoc = pFSPDFDoc;
	m_pActiontDict = pActDict;
}

FS_BOOL	FSActionImp::initSubActionArray()
{
	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	if (m_pSubActionArray)
		return TRUE;
	m_pSubActionArray = FX_NEW CFX_PtrArray();
	if (!m_pSubActionArray)
		return FALSE;

	CPDF_Action action(m_pActiontDict);
	FS_INT32 iSubActCount = action.GetSubActionsCount();
	if (iSubActCount>0)
		m_pSubActionArray->SetSize(iSubActCount);
	return TRUE;
}

FS_ACTIONTYPE FSActionImp::getType()
{
	if (!m_pActiontDict) return e_actionTypeUnknown;

	CPDF_Action action(m_pActiontDict);
	return (FS_ACTIONTYPE)action.GetType();
}

FS_INT32	FSActionImp::getSubActionCount()
{
	if (!m_pActiontDict || !initSubActionArray() ) return 0;

	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	return m_pSubActionArray->GetSize();
}

FSAction* FSActionImp::getSubAction(FS_INT32 index)
{
	if (!m_pActiontDict) return NULL;

	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	if (index<0 || index>=getSubActionCount() || !m_pSubActionArray) return NULL;

	FSAction* pGetAct = (FSAction*)m_pSubActionArray->GetAt(index);
	if (pGetAct) return pGetAct;

	CPDF_Action action(m_pActiontDict);
	CPDF_Action subAct = action.GetSubAction(index);
	pGetAct = FSActionImp::create(m_pFSPDFDoc, subAct.m_pDict, TRUE);
	m_pSubActionArray->SetAt(index, pGetAct);
	return pGetAct;
}

void FSActionImp::release()
{
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		m_nCountRef--;
		if (m_nCountRef != 0)
			return;
	}
    delete this;
}

void FSActionImp::setSubAction(FS_INT32 index, FSAction* subAction)
{
	if (!m_pActiontDict || !subAction || !m_pFSPDFDoc) return;
	FS_ACTIONTYPE actType = subAction->getType();
	if (FSActionDefines::e_actionTypeGoto != actType && FSActionDefines::e_actionTypeURI != actType)
		return ;
	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	if (index<0 || index>=getSubActionCount() || !m_pSubActionArray)
		return ;

	CPDF_Object* pNext = m_pActiontDict->GetElementValue("Next");
	if (!pNext) return ;
	FSAction* pTempAct = FSActionImp::retain(subAction);
	if (!pTempAct) return;
	CPDF_Dictionary* pNewSubActDict = NULL;
	switch(subAction->getType())
	{
	case FSActionDefines::e_actionTypeGoto:
		pNewSubActDict = ((FSGotoActionImp*)(FSGotoAction*)subAction)->getActionImp()->getActDict();
		break;
	case FSActionDefines::e_actionTypeURI:
		pNewSubActDict = ((FSURIActionImp*)(FSURIAction*)subAction)->getActionImp()->getActDict();
		break;
	}
	if (!pNewSubActDict)
	{
		if (pTempAct)
			pTempAct->release();
		return ;
	}
	CPDF_Document* pDoc = ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument();
	CPDF_Reference* pNewSubActRef = CPDF_Reference::Create(pDoc, pDoc->AddIndirectObject(pNewSubActDict));
	if (!pNewSubActRef) return ;
	FS_INT32 iObjType = pNext->GetType();
	if (PDFOBJ_ARRAY == iObjType)
	{
		CPDF_Array* pNextArray = (CPDF_Array*)pNext;
		pNextArray->SetAt(index, pNewSubActRef);
	}
	else
	{// PDFOBJ_DICTIONARY
		m_pActiontDict->SetAt("Next", pNewSubActRef);
	}
	FSAction* pOldAct = (FSAction*)m_pSubActionArray->GetAt(index);
	if (pOldAct)
		pOldAct->release();
	m_pSubActionArray->SetAt(index, pTempAct);
	setModified();//TODO: need?
	return ;
}

FS_BOOL FSActionImp::insertSubAction(FS_INT32 index, FSAction* subAction)
{
	if (!m_pActiontDict || !subAction) return FALSE;
	FS_ACTIONTYPE actType = subAction->getType();
	if (FSActionDefines::e_actionTypeGoto != actType && FSActionDefines::e_actionTypeURI != actType)
		return FALSE;
	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	if (index<0) index = 0;
	FS_INT32 nSubActCount = getSubActionCount();
	if (!m_pSubActionArray) return NULL;
	if (index>=nSubActCount) index = nSubActCount;
	CPDF_Dictionary* pNewSubDict = NULL;
	switch(actType)
	{
	case FSActionDefines::e_actionTypeGoto:
		pNewSubDict = ((FSGotoActionImp*)(FSGotoAction*)subAction)->getActionImp()->getActDict();
		break;
	case FSActionDefines::e_actionTypeURI:
		pNewSubDict = ((FSURIActionImp*)(FSURIAction*)subAction)->getActionImp()->getActDict();
		break;
	default:
		return FALSE;
	}
	CPDF_Action newSubAction(pNewSubDict);
	CPDF_Action action(m_pActiontDict);
	action.InsertSubAction(index, ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument(), newSubAction);
	FSAction* pTempAct = FSActionImp::retain(subAction);
	if (!pTempAct)
	{
		action.RemoveSubAction(index);
		return FALSE;
	}
	if (index<nSubActCount)
		m_pSubActionArray->InsertAt(index, pTempAct);
	else
		m_pSubActionArray->Add(pTempAct);
	setModified(); //TODO: need?
	return TRUE;
}

FS_BOOL FSActionImp::removeSubAction(FS_INT32 index)
{
	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	if (!m_pActiontDict
		|| index<0 || index>=getSubActionCount() ||	!m_pSubActionArray) 
		return FALSE;

	CPDF_Action action(m_pActiontDict);
	action.RemoveSubAction(index);
	FSAction* pSubAct = (FSAction*)m_pSubActionArray->GetAt(index);
	if (pSubAct)
		pSubAct->release();
	m_pSubActionArray->RemoveAt(index);
	setModified(); //TODO: need?
	return TRUE;
}

FS_BOOL FSActionImp::removeAllSubActions()
{
	FSCRT_LOCKOBJECT(lock, &m_lockSubActArray);
	if (!m_pActiontDict || !initSubActionArray()) return FALSE;

	CPDF_Action action(m_pActiontDict);
	action.RemoveAllSubActions();
	// Release and remove all the sub actions in m_pSubActionArray
	for (FS_INT32 i = 0; i < m_pSubActionArray->GetSize(); i++)
	{
		FSAction* pAct = (FSAction*)m_pSubActionArray->GetAt(i);
		if (pAct)
			pAct->release();
	}
	m_pSubActionArray->RemoveAll();

	setModified(); //TODO: need?
	return TRUE;
}

////////////////////////////////////////////
// Class FSGotoActionImp
FSGotoAction*	FSGotoActionImp::createFromDestination(FSPDFDoc* pFSPDFDoc, CPDF_Object* pDestObj)
{
	if (!pFSPDFDoc || !((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument() || !pDestObj) return NULL;

	FSGotoActionImp* pRetAct = NULL;
	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument();
	CPDF_Dest pdfDest(pDestObj);
	FS_INT32 objType = pDestObj->GetType();
	switch(objType)
	{
	case PDFOBJ_NAME:
	case PDFOBJ_STRING:
		{
			CFX_ByteString strName = pdfDest.GetRemoteName();
			CPDF_NameTree nameTree(pPDFDoc, FX_BSTRC("Dests"));
			CPDF_Array* nameDest = nameTree.LookupNamedDest(pPDFDoc, strName);
			if (!nameDest) return NULL;
			pRetAct = FX_NEW FSGotoActionImp();
			if (!pRetAct->init(pFSPDFDoc, CPDF_Dest(nameDest)))
			{
				delete pRetAct;
				pRetAct = NULL;
			}
		}
	case PDFOBJ_ARRAY:
		{
			pRetAct = FX_NEW FSGotoActionImp();
			if (!pRetAct->init(pFSPDFDoc, pdfDest))
			{
				delete pRetAct;
				pRetAct = NULL;
			}
		}
	}
	return pRetAct;
}

FSGotoActionImp::FSGotoActionImp()
	:m_pDestination(NULL)
{

}

FSGotoActionImp::~FSGotoActionImp()
{
	if (m_pDestination)
	{
		m_pDestination->release();
		m_pDestination = NULL;
	}
}

void	FSGotoActionImp::init(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict)
{
	FSActionImp::init(pFSPDFDoc, pActDict);
}

FS_BOOL	FSGotoActionImp::init(FSPDFDoc* pFSPDFDoc, CPDF_Dest pdfDest)
{
	m_pDestination = FSDestinationImp::createFromPDFDest(pFSPDFDoc, pdfDest);
	if (m_pDestination)
		return TRUE;
	else
		return FALSE;
}

FS_ACTIONTYPE FSGotoActionImp::getType()
{
	return e_actionTypeGoto;
}


//void FSGotoActionImp::release()
//{
//	{
//		FSCRT_LOCKOBJECT(lock, &m_lock);
//		m_nCountRef--;
//		if (m_nCountRef != 0)
//			return ;
//	}
//	delete this;
//}

FSDestination* FSGotoActionImp::getDestination()
{
	// If goto action is created from destination internally, m_pDestination must be valid.
	if (m_pDestination)
		return m_pDestination;

	// Here, m_pDestination is NULL: that means goto action is created from FSActionImp::create(), and action dictionary should be valid.
	if (!getActDict() || !getFSPDFDocument()) 
		return NULL;
	FSPDFDoc* pFSPDFDoc = getFSPDFDocument();
	CPDF_Document* pPDFDoc = ((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument();
	CPDF_Action action(getActDict());
	CPDF_Dest pdfDest = action.GetDest(pPDFDoc);
	m_pDestination = FSDestinationImp::createFromPDFDest(pFSPDFDoc, pdfDest);
	return m_pDestination;
}

void	FSGotoActionImp::setDestination(FSDestination* dest)
{
	if (!getActDict() 
		|| !dest || !((FSDestinationImp*)dest)->getDestArray())
		return ;

	CPDF_Action action(getActDict());
	CPDF_Dest newDest(((FSDestinationImp*)dest)->getDestArray());
	action.SetDest(newDest);
	if (m_pDestination)
		m_pDestination->release();
	m_pDestination = ((FSDestinationImp*)dest)->retain();
	setModified();// TODO: need??
}

FSGotoAction*	FSGotoActionImp::retain()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_nCountRef++;
	return this;
}

////////////////////////////////////////////
// Class FSURIActionImp
FSURIActionImp::FSURIActionImp()
{

}

FSURIActionImp::~FSURIActionImp()
{

}

void	FSURIActionImp::init(FSPDFDoc* pFSPDFDoc, CPDF_Dictionary* pActDict)
{
	FSActionImp::init(pFSPDFDoc, pActDict);
}

FS_ACTIONTYPE	FSURIActionImp::getType()
{
	return e_actionTypeURI;
}

//void FSURIActionImp::release()
//{
//	{
//		FSCRT_LOCKOBJECT(lock, &m_lock);
//		m_nCountRef--;
//		if (m_nCountRef != 0)
//			return ;
//	}
//	delete this;
//}


FSString	FSURIActionImp::getURI()
{
	if (!getActDict()) return FSString();

	CPDF_Action action(getActDict());
	FSPDFDoc* pFSPDFDoc = getFSPDFDocument();
	CFX_ByteString bsURI = action.GetURI(((FSPDFDocImp*)pFSPDFDoc)->getPDFDocument());
	FSString retStr(bsURI.GetBuffer(bsURI.GetLength()), bsURI.GetLength());
	bsURI.ReleaseBuffer();
	return retStr;
}

void	FSURIActionImp::setURI(const char* uri)
{
	if (!getActDict() 
		|| !uri || FXSYS_strlen(uri)<1) 
		return;

	CFX_ByteString bsURI(uri, FXSYS_strlen(uri));
	CPDF_Action action(getActDict());
	action.SetURI(bsURI);
	setModified(); //TODO: need?
}

FS_BOOL		FSURIActionImp::isTrackPosition()
{
	if (!getActDict()) return FALSE;

	CPDF_Action action(getActDict());
	return action.GetMouseMap();
}

void	FSURIActionImp::setTrackPositionFlag(FS_BOOL isTrackPosition)
{
	if (!getActDict()) return;

	CPDF_Action action(getActDict());
	action.SetMouseMap(isTrackPosition);
	setModified(); //TODO: need?
}

FSURIAction*	FSURIActionImp::retain()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_nCountRef++;
	return this;
}