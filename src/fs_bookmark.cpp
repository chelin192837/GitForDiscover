#include "../include/common.h"
#include "../include/fs_bookmark.h"
#include "../include/fs_pdfdoc.h"
#include "../include/fs_pdfpage.h"
#include "../include/fs_pdfobject.h"

#include "fxcore.h"
#include "fs_util.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_bookmarkImp.h"
#include "inc/fs_actionImp.h"

/////////////////////////////////////
//class FSBookmarkImp
FSBookmarkImp::FSBookmarkImp()
	:m_pBookmarkDict(NULL)
	,m_pFSPDFDoc(NULL)
	,m_pBmParent(NULL)
	,m_pBmNextSibling(NULL)
	,m_pBmFirstChild(NULL)
	,m_pDestination(NULL)
//	,m_pAction(NULL)
{
}

FSBookmarkImp::~FSBookmarkImp()
{
	SAFE_DELETE(m_pBmFirstChild);
	SAFE_DELETE(m_pBmNextSibling);
//	SAFE_DELETE(m_pAction);

	if (m_pBmParent&& this == m_pBmParent->m_pBmFirstChild)
		m_pBmParent->m_pBmFirstChild = NULL;
	m_pBmParent = NULL;

	if (m_pDestination)
		m_pDestination->release();
	m_pDestination = NULL;
}

void FSBookmarkImp::init(FSPDFDoc* pDoc, CPDF_Dictionary* pBookmarkDict)
{
	m_pBookmarkDict = pBookmarkDict;
	m_pFSPDFDoc = pDoc;
}

FSBookmarkImp* FSBookmarkImp::getParent()
{
	if (!m_pBookmarkDict) return NULL;

	return m_pBmParent;
}

FSBookmarkImp* FSBookmarkImp::getFirstChild()
{
	if (!m_pBookmarkDict) return NULL;
	if (m_pBmFirstChild) return m_pBmFirstChild;

	CPDF_Dictionary* pChildBmDict = m_pBookmarkDict->GetDict("First");
	if (!pChildBmDict) return NULL;

	m_pBmFirstChild = FX_NEW FSBookmarkImp();
	if (!m_pBmFirstChild) return NULL;

	m_pBmFirstChild->init(m_pFSPDFDoc, pChildBmDict);
	m_pBmFirstChild->m_pBmParent = this;

	return m_pBmFirstChild;
}

FSBookmarkImp* FSBookmarkImp::getNextSibling()
{
	if (!m_pBookmarkDict) return NULL;
	if (m_pBmNextSibling) return m_pBmNextSibling;

	CPDF_Dictionary *pNextBmDict = m_pBookmarkDict->GetDict("Next");
	if (!pNextBmDict || isExisted(pNextBmDict)) return NULL;

	m_pBmNextSibling = FX_NEW FSBookmarkImp();
	if (!m_pBmNextSibling) return NULL;

	m_pBmNextSibling->init(m_pFSPDFDoc, pNextBmDict);
	m_pBmNextSibling->m_pBmParent = m_pBmParent;

	return m_pBmNextSibling;
}

FS_BOOL FSBookmarkImp::isExisted(CPDF_Dictionary* pBookmarkDict)
{
	if(!m_pBmParent)
		return FALSE;
	FSBookmarkImp* pBookmark = m_pBmParent->getFirstChild();
	while(pBookmark){
		if(pBookmark->m_pBookmarkDict == pBookmarkDict)
			return TRUE;
		pBookmark = pBookmark->m_pBmNextSibling;
	}
	return FALSE;
}

FS_BOOL FSBookmarkImp::getFinalGOTOAction(CPDF_Action pdfAct, CPDF_Action& gotoAction)
{
	int nSubActions = pdfAct.GetSubActionsCount();
	for(int i = nSubActions -1; i>=0; i--){
		if(getFinalGOTOAction(pdfAct.GetSubAction(i), gotoAction))
			return TRUE;
	}
	if(pdfAct.GetType() == CPDF_Action::GoTo){
		gotoAction = pdfAct;
		return TRUE;
	}
	return FALSE;
}

FSDestination* FSBookmarkImp::getDestination()
{
	if (!m_pBookmarkDict || !m_pFSPDFDoc || NULL == ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument()) 
		return NULL;
	if (m_pDestination) 
		return m_pDestination;
	CPDF_BookmarkEx bookmark(m_pBookmarkDict);
	CPDF_Document* pDoc = ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument();
	CPDF_Object* pDestObj = bookmark.GetDest(pDoc);
	if (NULL == pDestObj)
	{
		// If this bookmark is not directly associated with a dest, we try to get action
		CPDF_Action pdfAct = bookmark.GetAction();
		CPDF_Action gotoAction;
		if(getFinalGOTOAction(pdfAct, gotoAction)){
			// Currently, only support to get destination directly from GOTO action.
			if (gotoAction.GetType() == CPDF_Action::GoTo)
				pDestObj = gotoAction.GetDest(pDoc);
			else
				pDestObj = NULL;
		}
	}
	if (!pDestObj) return NULL;
	CPDF_Dest bmDest(pDestObj);
	m_pDestination = FSDestinationImp::createFromPDFDest(m_pFSPDFDoc, bmDest);
	return m_pDestination;
}

void FSBookmarkImp::setDestination(const FSDestination* dest) 
{
	FSDestinationImp* pDestImp = NULL;
	if (dest) pDestImp = (FSDestinationImp*)dest;
	if (!pDestImp || !pDestImp->getDestArray() || pDestImp->getPageIndex() < 0
		|| !m_pFSPDFDoc || !m_pBookmarkDict) 
		return;

	CPDF_Document* pDoc = ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument();
	FSPDFDictionary* pFSPageDict = (FSPDFDictionary*)pDoc->GetPage(pDestImp->getPageIndex());
	if (!pFSPageDict) 
		return;
	if (m_pDestination)
	{
		m_pDestination->release();
		m_pDestination = NULL;
	}
	m_pDestination = pDestImp->retain();
	CPDF_Array* pArrayDt = (CPDF_Array*)pDestImp->getDestArray()->Clone();
	CPDF_Object* pPageRef = pArrayDt->GetElement(0);
	if (pPageRef)
	{
		if (PDFOBJ_REFERENCE == pPageRef->GetType())
			((CPDF_Reference*)pPageRef)->SetRef(pDoc, pFSPageDict->getObjNum());
		else
			pArrayDt->InsertAt(0, FX_NEW CPDF_Reference(pDoc, pFSPageDict->getObjNum()));
	}
	else
		pArrayDt->AddReference(pDoc, pFSPageDict->getObjNum());

	CPDF_BookmarkEx bookmark(m_pBookmarkDict);
	bookmark.SetDest(pDoc, pArrayDt);
	((FSPDFDocImp*)m_pFSPDFDoc)->setModified();
}

FSString FSBookmarkImp::getTitle() const
{
	if (!m_pBookmarkDict) return NULL;

	CPDF_BookmarkEx bookmark(m_pBookmarkDict);
	CFX_WideString wsTitle = bookmark.GetTitle();
	FSString strTitle = FSString::createFromWideString(wsTitle);
	strTitle.toUTF8();

	return strTitle;
}

void FSBookmarkImp::setTitle(const char* title) 
{
	if (!m_pBookmarkDict || FSCRT_Util_IsStringEmpty(title)) return;

	CFX_WideString wsStrTitle;
	wsStrTitle = wsStrTitle.FromUTF8(title, FXSYS_strlen(title));

	CPDF_BookmarkEx bookmark(m_pBookmarkDict);
	((FSPDFDocImp*)m_pFSPDFDoc)->setModified();
	return bookmark.SetTitle(wsStrTitle);
}

FS_ARGB FSBookmarkImp::getColor() const
{
	if (!m_pBookmarkDict) return 0;

	CPDF_BookmarkEx bookmark(m_pBookmarkDict);
	return ArgbEncode(0xFF, bookmark.GetColorRef());
}

void FSBookmarkImp::setColor(FS_ARGB color)
{
	if (!m_pBookmarkDict) return;

	CPDF_BookmarkEx bookmarkEx(m_pBookmarkDict);
	bookmarkEx.SetColorRef((FS_ARGB)color);
	((FSPDFDocImp*)m_pFSPDFDoc)->setModified();
}

FS_DWORD FSBookmarkImp::getStyle() const
{
	if (!m_pBookmarkDict) return e_bookmarkStyleNormal;

	return m_pBookmarkDict->GetInteger("F");
}

void FSBookmarkImp::setStyle(FS_DWORD style)
{
	if (!m_pBookmarkDict) return;

	if (style < e_bookmarkStyleNormal || style > (e_bookmarkStyleNormal | e_bookmarkStyleItalic | e_bookmarkStyleBold))
		style = e_bookmarkStyleNormal;
	
	m_pBookmarkDict->SetAtInteger("F",style);
	((FSPDFDocImp*)m_pFSPDFDoc)->setModified();
}

FSBookmarkImp* FSBookmarkImp::getPrevSibling()
{
	if (!m_pBmParent)
		return NULL;

	FSBookmarkImp* prevSibling = m_pBmParent->getFirstChild();
	FSBookmarkImp* nextSibling = prevSibling;
	while(nextSibling)
	{
		if (nextSibling == this)
			return prevSibling;
		prevSibling = nextSibling;
		nextSibling = nextSibling->getNextSibling();
	}

	return NULL;
}

FSBookmarkImp* FSBookmarkImp::getLastChild()
{
	FSBookmarkImp* pLastChild = getFirstChild();
	FSBookmarkImp* pNextChild = pLastChild;

	while(pNextChild)
	{
		pLastChild = pNextChild;
		pNextChild = pNextChild->getNextSibling();
	}
	return pLastChild;
}

void FSBookmarkImp::insertPDFBookmark(FS_BOOKMARKPOSITION pos, CPDF_Dictionary* pInsertBookmarkDict)
{
	if (!pInsertBookmarkDict) return;

	CPDF_Document* pDoc = ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument();

	if (getParent() == NULL && m_pBookmarkDict == NULL)
	{
		CPDF_Dictionary* pDocRootDict = pDoc->GetRoot();
		if (!pDocRootDict) return;

		CPDF_Dictionary* pOutlinesDict = FX_NEW CPDF_Dictionary;
		if (!pOutlinesDict) return;

		FX_INT32 iOutlinesNum = pDoc->AddIndirectObject(pOutlinesDict);
		pDocRootDict->SetAtReference(FX_BSTRC("Outlines"), pDoc, iOutlinesNum);

		pOutlinesDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("Outlines"));
		pOutlinesDict->SetAtInteger(FX_BSTRC("Count"), 0);

		m_pBookmarkDict = pOutlinesDict;
	}

	CPDF_Dictionary* pParentDict = (pos == e_bookmarkPosFirstChild || pos == e_bookmarkPosLastChild) ? m_pBookmarkDict : getParent()->m_pBookmarkDict;
	if (pParentDict == NULL) return;

	pInsertBookmarkDict->SetAtReference(FX_BSTRC("Parent"), pDoc, pParentDict);

	// Get After and before dictionary
	CPDF_Dictionary* pBeforeDict = NULL;
	CPDF_Dictionary* pAfterDict = NULL;

	switch (pos){
	case e_bookmarkPosFirstChild:
	case e_bookmarkPosFirstSibling:
		pBeforeDict = pParentDict->GetDict(FX_BSTRC("First"));
		break;
	case e_bookmarkPosLastChild:
	case e_bookmarkPosLastSibling:
		pAfterDict = pParentDict->GetDict(FX_BSTRC("Last"));
		break;
	case e_bookmarkPosPrevSibling:
		pBeforeDict = m_pBookmarkDict;
		pAfterDict = pBeforeDict->GetDict(FX_BSTRC("Prev"));
		break;
	case e_bookmarkPosNextSibling:
		pAfterDict = m_pBookmarkDict;
		pBeforeDict = pAfterDict->GetDict(FX_BSTRC("Next"));
		break;
	default:
		break;
	}

	// Insert
	if (pBeforeDict){
		pInsertBookmarkDict->SetAtReference(FX_BSTRC("Next"), pDoc, pBeforeDict);
		pBeforeDict->SetAtReference(FX_BSTRC("Prev"), pDoc, pInsertBookmarkDict);
	}
	if (pAfterDict){
		pInsertBookmarkDict->SetAtReference(FX_BSTRC("Prev"), pDoc, pAfterDict);
		pAfterDict->SetAtReference(FX_BSTRC("Next"), pDoc, pInsertBookmarkDict);
	}	
	// set last and first if need
	if (!pBeforeDict) pParentDict->SetAtReference(FX_BSTRC("Last"), pDoc, pInsertBookmarkDict);
	if (!pAfterDict) pParentDict->SetAtReference(FX_BSTRC("First"), pDoc, pInsertBookmarkDict);

	//set count entry
	while(pParentDict){
		int iCount = pParentDict->GetInteger(FX_BSTRC("Count"));
		if (iCount >= 0)
			pParentDict->SetAtInteger(FX_BSTRC("Count"), ++iCount);
		else
			pParentDict->SetAtInteger(FX_BSTRC("Count"), --iCount);
		pParentDict = pParentDict->GetDict(FX_BSTRC("Parent"));
	}
}

void FSBookmarkImp::insertBookmarkToLastChild(FSBookmarkImp* insertBookmark)
{
	FSBookmarkImp* pLastChild = getLastChild();
	if (pLastChild == NULL)
		m_pBmFirstChild = insertBookmark;
	else
		pLastChild->m_pBmNextSibling = insertBookmark;
	
	insertBookmark->m_pBmParent = this;
	insertBookmark->m_pBmNextSibling = NULL;
}

void FSBookmarkImp::insertBookmarkToPrevSibling(FSBookmarkImp* insertBookmark)
{
	FSBookmarkImp* prevSiblingBookmark = getPrevSibling();
	if (prevSiblingBookmark != NULL && prevSiblingBookmark != this)
	{
		prevSiblingBookmark->m_pBmNextSibling = insertBookmark;
		insertBookmark->m_pBmNextSibling = this;
	}
	else if (prevSiblingBookmark == this)
	{
		insertBookmark->m_pBmNextSibling = m_pBmParent->m_pBmFirstChild;
		m_pBmParent->m_pBmFirstChild = insertBookmark;
	}
	else
		return;
	insertBookmark->m_pBmParent = m_pBmParent;
}

void FSBookmarkImp::insertFSBookmark(FS_BOOKMARKPOSITION pos, FSBookmarkImp* pBookmark)
{
	if (!pBookmark) return;

	switch (pos){
	case e_bookmarkPosFirstChild:
		pBookmark->m_pBmNextSibling = getFirstChild();
		m_pBmFirstChild = pBookmark;
		m_pBmFirstChild->m_pBmParent = this;
		break;
	case e_bookmarkPosFirstSibling:
		pBookmark->m_pBmNextSibling = m_pBmParent->getFirstChild();
		m_pBmParent->m_pBmFirstChild = pBookmark;
		pBookmark->m_pBmParent = m_pBmParent;
		break;
	case e_bookmarkPosLastChild:
		insertBookmarkToLastChild(pBookmark);
		break;
	case e_bookmarkPosLastSibling:
		m_pBmParent->insertBookmarkToLastChild(pBookmark);
		break;
	case e_bookmarkPosPrevSibling:
		insertBookmarkToPrevSibling(pBookmark);
		break;
	case e_bookmarkPosNextSibling:
		pBookmark->m_pBmNextSibling = getNextSibling();
		m_pBmNextSibling = pBookmark;
		pBookmark->m_pBmParent = m_pBmParent;
		break;
	default:
		break;
	}
}

void FSBookmarkImp::insertBookmark(FS_BOOKMARKPOSITION pos, FSBookmarkImp* insertBookmark)
{
	if (!insertBookmark || NULL == insertBookmark->getBookmarkDict())
		return;

	insertFSBookmark(pos, insertBookmark);
	insertPDFBookmark(pos, insertBookmark->getBookmarkDict());
}

FSBookmark*	FSBookmarkImp::insert(const char* title, FS_BOOKMARKPOSITION pos)
{
	if ((!m_pBookmarkDict && getParent() != NULL) || FSCRT_Util_IsStringEmpty(title)) return NULL;

	if (pos < e_bookmarkPosFirstChild  || pos > e_bookmarkPosLastSibling)
		return NULL;
	
	if (m_pFSPDFDoc->getFirstBookmark() == this && pos != e_bookmarkPosFirstChild && pos != e_bookmarkPosLastChild)
		return NULL;

	CPDF_Document* pDoc = ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument();
	if (!pDoc) return NULL;

	FSBookmarkImp* pBookmark = FX_NEW FSBookmarkImp();
	if (!pBookmark) return NULL;
	
	//create a new bookmark dictionary.
	CPDF_Dictionary* pNewBookmarkDict = FX_NEW CPDF_Dictionary;
	if (!pNewBookmarkDict) 
	{
		delete pBookmark;
		return NULL;
	}
	
	pDoc->AddIndirectObject(pNewBookmarkDict);
	//init
	pBookmark->init(m_pFSPDFDoc, pNewBookmarkDict);

	//insert bookmark.
	insertBookmark(pos, pBookmark);

	//set property.
	pBookmark->setTitle(title);
	pBookmark->setColor(0);
	pBookmark->setStyle(e_bookmarkStyleNormal);

	((FSPDFDocImp*)m_pFSPDFDoc)->setModified();

	return (FSBookmark*)pBookmark;
}

CPDF_Dictionary* FSBookmarkImp::getBookmarkDict()
{
	return m_pBookmarkDict;
}

FS_BOOL FSBookmarkImp::isDescendant(FSBookmark* bookmark)
{
	if (!bookmark || this == bookmark)
		return FALSE;

	FSBookmarkImp* pChild = getFirstChild();
	while(pChild)
	{
		if (pChild == (FSBookmarkImp*)bookmark)
			return TRUE;

		FSBookmarkImp* pNextSibling = pChild->getNextSibling();
		while(pNextSibling)
		{
			if (pNextSibling == (FSBookmarkImp*)bookmark || pNextSibling->isDescendant(bookmark))
				return TRUE;

			pNextSibling = pNextSibling->getNextSibling();
		}
		pChild = pChild->getFirstChild();
	}

	return FALSE;
}

//remove the child bookmark from the current bookmark.
FS_BOOL FSBookmarkImp::removeFSBookmark(FSBookmarkImp* bookmark)
{
	if (!bookmark || this == bookmark) return FALSE;

	FSBookmarkImp* parent = this;
	FSBookmarkImp* pFirstChild = parent->getFirstChild();

	while(pFirstChild)
	{
		if (pFirstChild == bookmark)
		{
			parent->m_pBmFirstChild = pFirstChild->getNextSibling();
            bookmark->m_pBmNextSibling = NULL;
			return TRUE;
		}
		else
		{
			FSBookmarkImp* pNextSibling = pFirstChild->getNextSibling();
			FSBookmarkImp* pPrevSibling = pFirstChild;
			while(pNextSibling)
			{
				if (pNextSibling == bookmark)
				{
					pPrevSibling->m_pBmNextSibling = pNextSibling->getNextSibling();
                    bookmark->m_pBmNextSibling = NULL;
					return TRUE;
				}
				if (bookmark->m_pBmParent != this && pNextSibling->removeFSBookmark(bookmark))
					return TRUE;

				pPrevSibling = pNextSibling;
				pNextSibling = pNextSibling->getNextSibling();
			}
		}
		if (bookmark->m_pBmParent == this)
			return FALSE;

		parent = pFirstChild;
		pFirstChild = pFirstChild->getFirstChild();
	}

	return FALSE;
}

//The current bookmark is removed from bookmark tree.
FS_BOOL	FSBookmarkImp::removeFromBMTree(FS_BOOL bRelease)
{
	FSPDFDocImp* pFSDocImp = (FSPDFDocImp*)m_pFSPDFDoc;
	CPDF_Document* pDoc = pFSDocImp->getPDFDocument();
	if (!pDoc) return FALSE;

	if (this == pFSDocImp->getFirstBookmark())
	{
		pDoc->GetRoot()->RemoveAt(FX_BSTRC("Outlines"));
		if (bRelease)
		{
			SAFE_DELETE(pFSDocImp->m_pBmRoot);
		}
	}
	else
	{
		if (!m_pBmParent->removeFSBookmark(this))
			return FALSE;

		CPDF_BookmarkTreeEx bookmarkTree(pDoc);
		bookmarkTree.RemoveItem(m_pBookmarkDict);

		//In FXCORE, if the bookmark root has no child, it would remove the outlines dictionary in catalog.
		if (m_pBmParent == ((FSPDFDocImp*)m_pFSPDFDoc)->m_pBmRoot && m_pBmParent->m_pBmFirstChild == NULL)
			m_pBmParent->m_pBookmarkDict = NULL;

		if (bRelease)
			delete this;
	}

	return TRUE;
}

FS_BOOL	FSBookmarkImp::moveTo(FSBookmark* destBookmark, FS_BOOKMARKPOSITION pos)
{
	if (!m_pBookmarkDict || !destBookmark || pos < e_bookmarkPosFirstChild  || pos > e_bookmarkPosLastSibling) 
		return FALSE;

	if (this == m_pFSPDFDoc->getFirstBookmark())
		return FALSE;
	
	if (m_pFSPDFDoc->getFirstBookmark() == destBookmark && pos != e_bookmarkPosFirstChild && pos != e_bookmarkPosLastChild)
		return FALSE;

	FSBookmarkImp* pBookmarkImp = (FSBookmarkImp*)destBookmark;
	if (!pBookmarkImp->m_pBookmarkDict)
		return FALSE;

	CPDF_Document* pDoc = ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument();
	if (!pDoc) return FALSE;

	//Verify whether destBookmark is current bookmark itself or any descendant of current bookmark.
	if (isDescendant(destBookmark)||this == destBookmark) 
		return FALSE;

	//remove from bookmark tree.
	if (FALSE == this->removeFromBMTree(FALSE))
		return FALSE;

	//insert bookmark to bookmark tree.
	pBookmarkImp->insertBookmark(pos, this);

	((FSPDFDocImp*)m_pFSPDFDoc)->setModified();
	return TRUE;
}
