#include "fs_annot_imp.h"
#include "fs_annot_util.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/fs_pdfdocImp.h"

FSPDFMarkupImp::FSPDFMarkupImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	init(pAnnotDict, page);
}

void FSPDFMarkupImp::init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFAnnotImp::init(pAnnotDict, page);
}

//////////////
// Methods inherited from FSAnnot


FSPopup*	FSPDFMarkupImp::getPopup()
{
	CPDF_Dictionary* pPopupDict = getSubDict(FSANNOT_DICTKEY_POPUP);
	if (!pPopupDict) return NULL;
	FSAnnot* pAnnot = ((FSPDFPageImp*)getPage())->getAnnotbyDict(pPopupDict);
	if (pAnnot->getType() != FSAnnot::e_annotPopup)
		return NULL;
	return (FSPopup*)pAnnot;
}

void	FSPDFMarkupImp::setPopup(FSPopup* popup)
{
	FSM_ANNOTTYPE annotType = getType();
	if (!popup  || !popup->getDict()
		|| FSAnnot::e_annotFreeText == annotType || FSAnnot::e_annotSound == annotType
		|| !getDict() || !getPage()) 
		return ;

	CPDF_Dictionary* pCurrentDict = (CPDF_Dictionary*)getDict();
	CPDF_Dictionary* pNewPopupDict = (CPDF_Dictionary*)popup->getDict();
	//Check if current annot already has a pop-up.
	FSPopup* pOldPopup = getPopup();
	if (pOldPopup)
	{	
		// Remove old popup from page
		((CPDF_Dictionary*)pOldPopup->getDict())->RemoveAt(FSANNOT_DICTKEY_POPUPPARENT);
		if (!getPage()->removeAnnot(pOldPopup))
			return ; // TODO: return??
		// Remove relationship in current markup annotation
		pCurrentDict->RemoveAt(FSANNOT_DICTKEY_POPUP);
	}

	//Check if input popup already has a parent.
	CPDF_Dictionary* pOldParentDict = pNewPopupDict->GetDict(FSANNOT_DICTKEY_POPUPPARENT);
	if (pOldParentDict)
	{	// Remove the relationship between input popup and its old parent.
		pOldParentDict->RemoveAt(FSANNOT_DICTKEY_POPUP);
		pNewPopupDict->RemoveAt(FSANNOT_DICTKEY_POPUPPARENT);
	}

	// Build the relationship between input popup and current markup annotation.
	CPDF_Document* pDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
	pCurrentDict->SetAtReference(FSANNOT_DICTKEY_POPUP, pDoc, pNewPopupDict);
	pNewPopupDict->SetAtReference(FSANNOT_DICTKEY_POPUPPARENT, pDoc, pCurrentDict);
	setModified();
}

/////////////
// Methods inherited from FSMarkup
FSString FSPDFMarkupImp::getTitle() const
{
	return getString(FSANNOT_STRINGKEY_TITLE);
}

void FSPDFMarkupImp::setTitle(const char* title)
{
	return setString(FSANNOT_STRINGKEY_TITLE, title);
}

FSString FSPDFMarkupImp::getSubject() const
{
	return getString(FSANNOT_STRINGKEY_SUBJECT);
}

void FSPDFMarkupImp::setSubject(const char* subject)
{
	return setString(FSANNOT_STRINGKEY_SUBJECT, subject);
}

FS_FLOAT FSPDFMarkupImp::getOpacity() const
{
	if (hasProperty(FSANNOT_FLOATKEY_OPACITY))
		return getFloat(FSANNOT_FLOATKEY_OPACITY);
	else
		return 1.0f;
}

void FSPDFMarkupImp::setOpacity(FS_FLOAT opacity)
{
	if(opacity <0.0 || opacity> 1.0)
		return;
	return setFloat(FSANNOT_FLOATKEY_OPACITY, opacity);
}

FSString FSPDFMarkupImp::getIntent() const
{
	return getName(FSANNOT_NAMEKEY_INTENT);
}

void FSPDFMarkupImp::setIntent(const char* intent)
{
	switch(getType())
	{
	case FSAnnot::e_annotPolygon:
		{
			if (0 == FXSYS_strcmp("PolygonDimension", intent))
				return;
		}
		break;
	case FSAnnot::e_annotPolyLine:
		{
			if (0 == FXSYS_strcmp("PolyLineDimension", intent))
				return;
		}
		break;
	case FSAnnot::e_annotLine:
		{
			if (0 == FXSYS_strcmp("LineDimension", intent))
				return;
		}
		break;
	}
	return setName(FSANNOT_NAMEKEY_INTENT, intent);
}

FSDateTime FSPDFMarkupImp::getCreationDateTime() const
{
	return getDateTime(FSANNOT_DATETIMEKEY_CREATION);
}

void FSPDFMarkupImp::setCreationDateTime(FSDateTime dateTime)
{
	return setDateTime(FSANNOT_DATETIMEKEY_CREATION, dateTime);
}

#define FSANNOT_REPLYIMPL_COUNT			0
#define FSANNOT_REPLYIMPL_GET			1
FS_BOOL FSPDFMarkupImp::replyImpl(FS_INT32 index, FS_INT32 implType, FSNote* &pNote, FS_INT32& count) const
{
	if (!getPDFDict()) 
		return FALSE;

	FSPDFPageImp* pFSPDFPage = (FSPDFPageImp*)getPage();
	if (!pFSPDFPage || !pFSPDFPage->getPage()) return FALSE;
	FSCRT_LOCKOBJECT(lock, &pFSPDFPage->m_lock);
	CPDF_AnnotList annotList(pFSPDFPage->getPage());

	if (FSANNOT_REPLYIMPL_GET == implType)
	{
		if (index < 0 || index >= annotList.Count())
			return FALSE;
	}

	FS_INT32 iReplyCount = 0;
	for (FS_INT32 i = 0; i < annotList.Count(); i++)
	{
		CPDF_Annot* pAnnot = annotList.GetAt(i);
		if (!pAnnot || !pAnnot->m_pAnnotDict) continue;

		CPDF_Dictionary* pAnnotDict = pAnnot->m_pAnnotDict;

		if (FALSE == pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE).Equal(FSPDF_ANNOTTYPE_NOTE) 
			|| !FSAnnot_Util_ChectAnnotIsReply(pAnnotDict) || pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT) != getPDFDict())
			continue;
		else
			iReplyCount++;

		switch(implType)
		{
		case FSANNOT_REPLYIMPL_GET:
			if (index == iReplyCount - 1)
			{
				pNote = getReplyImpl(pAnnotDict, annotList, i);
				return (pNote != NULL);
			}
			break;
		default:
			break;
		}
	}

	if (FSANNOT_REPLYIMPL_COUNT == implType)
		count = iReplyCount;

	return TRUE;
}

FSNote* FSPDFMarkupImp::getReplyImpl(CPDF_Dictionary* pReplyAnnotDict, CPDF_AnnotList& annotList, FS_INT32 indexInAnnotList) const
{
	if (!pReplyAnnotDict) 
		return NULL;

	// Page has been locked in outside calling from replyImpl().
	FSPDFPageImp* pFSPDFPage = (FSPDFPageImp*)getPage();
	FSCRT_LOCKOBJECT(lock, &pFSPDFPage->m_lockAnnots);
	if (!pFSPDFPage || !pFSPDFPage->getAnnotArray()) return FALSE;
	if (!pReplyAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE).Equal(FSPDF_ANNOTTYPE_NOTE))
		return NULL;

	CFX_ArrayTemplate<void*>* pFSAnnotArray = pFSPDFPage->getAnnotArray();

	FSPDFNoteImp* pNote = NULL;
	FSAnnot* annot = (FSAnnot*)pFSAnnotArray->GetAt(indexInAnnotList);
	if (annot) 
		return (FSNote*)annot;

	pNote = FX_NEW FSPDFNoteImp(pReplyAnnotDict, pFSPDFPage);
	if (!pNote) return NULL;
	pFSAnnotArray->SetAt(indexInAnnotList, pNote);
	return pNote;
}

FS_BOOL FSPDFMarkupImp::removeReplyImpl(FS_INT32 replyIndex, FS_BOOL bRemoveAll)
{
	if (!getPDFDict()) 
		return FALSE;

	FSPDFPageImp* pFSPDFPage = (FSPDFPageImp*)getPage();
	if (!pFSPDFPage || !pFSPDFPage->getPage() || !pFSPDFPage->getAnnotArray()) return FALSE;
	FSCRT_LOCKOBJECT(lock, &pFSPDFPage->m_lock);
	CPDF_AnnotList annotList(pFSPDFPage->getPage());

	if (FALSE == bRemoveAll && (replyIndex < 0 || replyIndex >= annotList.Count()) )
		return FALSE;

	FS_INT32 iReplyCount = 0;
	FS_INT32 i=0, iValidIndex=0;//iValidIndex represents the index used in pFSAnnotArray, excluding SDK trial watermark annotations.
	FS_INT32 iCurrentCount = annotList.Count();
	if (i>=iCurrentCount) return FALSE;
	CPDF_AnnotList* pTempAnnotList = NULL;
	pTempAnnotList = FX_NEW CPDF_AnnotList(pFSPDFPage->getPage());
	do {
		FS_INT32 iCurrentIndex = i;
		CPDF_Annot* pAnnot = pTempAnnotList->GetAt(i++);
		if (!pAnnot || !pAnnot->m_pAnnotDict)
		{
			iValidIndex++;
			continue;
		}
		CPDF_Dictionary* pAnnotDict = pAnnot->m_pAnnotDict;
		FS_INT32 iWMAnnotType = FSCRT_Util_GetWatermarkAnnotType(pAnnot->m_pAnnotDict);
		if (WMANNOTTYPE_RDKTRIAL == iWMAnnotType || WMANNOTTYPE_GSDKTRIAL == iWMAnnotType)
		{// Ignore RDK and GSDK trial watermark and correct iValidIndex for pFSAnnotArray.
			iValidIndex--;
			continue;
		}
		if (FALSE == pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE).Equal(FSPDF_ANNOTTYPE_NOTE) 
			|| !FSAnnot_Util_ChectAnnotIsReply(pAnnotDict) || pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() != getPDFDict()->GetObjNum())
		{// pAnnotDict doesn't represents a reply note annot or it's not a reply to current annotation.
			iValidIndex++;
			continue;
		}
		else
			iReplyCount++;

		if (TRUE == bRemoveAll || (FALSE == bRemoveAll && replyIndex == iReplyCount - 1) )
		{
			CFX_ArrayTemplate<void*>* pFSAnnotArray = pFSPDFPage->getAnnotArray();

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Remove current reply annot's reference from annotlist first. Also remove it from pFSAnnotArray.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Why to remove current reply annot first and then remove its replies? Can dictionary of current reply annot still be valid after pTempAnnotList->Remove()?
			// ANSWER:
			// In current code flow, if to remove sub replies first, iCurrentIndex of current reply annot may become unreliable.
			// Because index of any of its sub-reply annot may be smaller than current reply, if this kind of sub reply is removed first, iCurrentIndex becomes unreliable.
			// So, remove current reply annot first.
			// According to the test result, pTempAnnotList->Remove() just removes the reference object of current reply annot in /Annots, 
			// not remove the dictionary of current reply annot.
			// So, we can remove current reply annot's reference from annotlist first and then use its dictionary to create a temp FSPDFNoteImp object to remove its replies.
			pTempAnnotList->Remove(iCurrentIndex);
			FSAnnot* annot = (FSAnnot*)pFSAnnotArray->GetAt(iValidIndex);
			if (annot)
			{
				delete annot;
				pFSAnnotArray->RemoveAt(iValidIndex);
			}
			//FS_INT32 iTemp = pTempAnnotList->Count(); // Just for check count.
			delete pTempAnnotList; 
			pTempAnnotList = NULL;
			// Use dictionary of current reply annotation to check if it has any sub reply. If yes, remove them.
			FSPDFNoteImp* pReplyAnnot = FX_NEW FSPDFNoteImp(pAnnotDict, getPage());
			if (NULL == pReplyAnnot) 
				return FALSE;
			FS_INT32 iSubReplyCount = pReplyAnnot->getReplyCount();
			if (iSubReplyCount>0)
				pReplyAnnot->removeAllReplies();
			delete pReplyAnnot;

			i=0; iValidIndex=0;
			pTempAnnotList = FX_NEW CPDF_AnnotList(pFSPDFPage->getPage());
			iCurrentCount = pTempAnnotList->Count();
		}
		else
			iValidIndex++;
	}
	while(i<iCurrentCount);
	if (pTempAnnotList) delete pTempAnnotList;
	if (FALSE == bRemoveAll && replyIndex >= iReplyCount) return FALSE;
	setModified();
	return TRUE;
}

FS_INT32 FSPDFMarkupImp::getReplyCount()
{
	if (!getPDFDict()) return 0;

	FS_INT32 count;
	FSNote* pNote;
	replyImpl(0, FSANNOT_REPLYIMPL_COUNT, pNote, count);

	return count;
}

FSNote* FSPDFMarkupImp::getReply(FS_INT32 index) const
{
	if (!getPDFDict()) return NULL;

	FSNote* pAnnotNote = NULL;
	FS_INT32 count = 0;
	replyImpl(index, FSANNOT_REPLYIMPL_GET, pAnnotNote, count);

	return pAnnotNote;
}

FSNote* FSPDFMarkupImp::addReply()
{
	if (!getPDFDict() || !getPage())
		return NULL;
	FSPDFPageImp* pFSPDFPage = (FSPDFPageImp*)getPage();
	CPDF_Page* pPDFPage = pFSPDFPage->getPage();
	FSCRT_LOCKOBJECT(lock, &pFSPDFPage->m_lockAnnots);
	if (NULL == pPDFPage || NULL == pFSPDFPage->getAnnotArray()) 
		return NULL;

	CPDF_AnnotList annotList(pPDFPage);
	FSRectF rectF;
	rectF.set(0, 0, 0, 0);
	CPDF_Dictionary* pAnnotDict = FSANNOT_UTIL_InsertAnnot(FSAnnot::e_annotNote, annotList.Count(), rectF, pPDFPage->m_pFormDict, pPDFPage->m_pDocument);
	if (!pAnnotDict) return NULL;

	pAnnotDict->SetAtInteger(FSANNOT_INTEGERKEY_FLAGS, 0);
	pAnnotDict->SetAtName(FSANNOT_NAMEKEY_RT, FX_BSTRC("R"));
	pAnnotDict->SetAtReference(FSANNOT_DICTKEY_IRT, pPDFPage->m_pDocument, getPDFDict());

	FSPDFNoteImp* pNote = FX_NEW FSPDFNoteImp(pAnnotDict, pFSPDFPage);
	if (!pNote) return NULL;
	pFSPDFPage->getAnnotArray()->Add(pNote);
	return pNote;
}

FS_BOOL FSPDFMarkupImp::removeReply(FS_INT32 index)
{
	return removeReplyImpl(index, FALSE);
}

FS_BOOL FSPDFMarkupImp::removeAllReplies()
{
	return removeReplyImpl(0, TRUE);
}

FS_BOOL		FSPDFMarkupImp::isGrouped()
{
	// Check if current markup annot is a member of a group, not the header.
	if(hasProperty(FSANNOT_NAMEKEY_RT))
	{
		CFX_ByteString bsRT = getStringImpl(FSANNOT_NAMEKEY_RT);
		if (bsRT.Equal("Group"))
			return TRUE;
		else // TODO: need to check if other case means current markup annot doesn't belong to a group.
			return FALSE;
	}
	
	// Check if current markup annot is the header of a group.
	return isGroupHeader();
}

FSMarkup*	FSPDFMarkupImp::getGroupHeader()
{
	CPDF_Dictionary* pHeaderDict = getGroupHeaderDict();
	if (!pHeaderDict) return NULL;
	FSAnnot* pAnnot = ((FSPDFPageImp*)getPage())->getAnnotbyDict(pHeaderDict);
	if (pAnnot->isMarkup())
		return (FSMarkup*)pAnnot;
	return NULL;
}

FS_INT32	FSPDFMarkupImp::getGroupElementCount()
{
	CPDF_Dictionary* pHeaderDict = getGroupHeaderDict();
	if (!getPage() || !pHeaderDict) return 0;

	FS_DWORD dwHeaderObjNum = pHeaderDict->GetObjNum();
	FSPDFPage* pPage = getPage();
	FS_INT32 nAnnotCount = pPage->getAnnotCount();
	FS_INT32 nGroupElementCount = 0;
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		FSAnnot* pAnnot = pPage->getAnnot(i);
		if (!pAnnot || !pAnnot->isMarkup()) 
			continue;

		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		if (pAnnotDict)
		{
			// pAnnot is just the header
			if (pAnnotDict->GetObjNum() == dwHeaderObjNum)
				nGroupElementCount++;
			else
			{// pAnnot is not the header and then check if it belongs to the group
				CFX_ByteString bsRT = pAnnotDict->GetString(FSANNOT_NAMEKEY_RT);
				if (pAnnotDict->KeyExist(FSANNOT_DICTKEY_IRT))
				{
					FX_DWORD objNum = pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum();
					if (objNum == dwHeaderObjNum && bsRT.Equal("Group"))
						nGroupElementCount++;
				}
			}
		}
	}
	// There should be at least 2 elements in a group.
	if (nGroupElementCount<2)
		nGroupElementCount = 0;
	return nGroupElementCount;
}

FSMarkup*	FSPDFMarkupImp::getGroupElement(FS_INT32 index)
{
	if (index<0 || index>=getGroupElementCount()) return NULL;
	CPDF_Dictionary* pHeaderDict = getGroupHeaderDict();
	if (!getPage() || !pHeaderDict) return 0;

	FS_DWORD dwHeaderObjNum = pHeaderDict->GetObjNum();
	FSPDFPage* pPage = getPage();
	FS_INT32 nAnnotCount = pPage->getAnnotCount();
	FS_INT32 nGroupElementIndex = 0;
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		FSAnnot* pAnnot = pPage->getAnnot(i);
		if (!pAnnot || !pAnnot->isMarkup()) 
			continue;

		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		if (pAnnotDict)
		{
			// pAnnot is just the header
			if (pAnnotDict->GetObjNum() == dwHeaderObjNum)
			{
				if (nGroupElementIndex == index)
					return (FSMarkup*)pAnnot;
				nGroupElementIndex++;
			}
			else
			{// pAnnot is not the header and then check if it belongs to the group
				CFX_ByteString bsRT = pAnnotDict->GetString(FSANNOT_NAMEKEY_RT);
				if (pAnnotDict->KeyExist(FSANNOT_DICTKEY_IRT) 
					&& pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() == dwHeaderObjNum
					&& bsRT.Equal("Group"))
				{
					if (nGroupElementIndex == index)
						return (FSMarkup*)pAnnot;
					nGroupElementIndex++;
				}
			}
		}
	}
	return NULL;
}

FS_BOOL		FSPDFMarkupImp::ungroup()
{
	CPDF_Dictionary* pHeaderDict = getGroupHeaderDict();
	//Not in a group
	if (!pHeaderDict || !getPage()) 
		return FALSE;
	else if (this->getDict()->getObjNum() == pHeaderDict->GetObjNum()) 
	{// Current markup is just the header
		// ungroup the whole group
		FS_DWORD dwHeaderObjNum = pHeaderDict->GetObjNum();
		FSPDFPage* pPage = getPage();
		FS_INT32 nAnnotCount = pPage->getAnnotCount();
		for (FS_INT32 i=0; i<nAnnotCount; i++)
		{
			FSAnnot* pAnnot = pPage->getAnnot(i);
			if (!pAnnot || !pAnnot->isMarkup())
				continue;
			CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
			if (pAnnotDict)
			{
				// pAnnot is just the header
				if (pAnnotDict->GetObjNum() == dwHeaderObjNum)
					continue;
				else
				{// pAnnot is not the header and then check if it belongs to the group
					CFX_ByteString bsRT = pAnnotDict->GetString(FSANNOT_NAMEKEY_RT);
					if (pAnnotDict->KeyExist(FSANNOT_DICTKEY_IRT) 
						&& pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() == dwHeaderObjNum
						&& bsRT.Equal("Group"))
					{
						pAnnotDict->RemoveAt(FSANNOT_NAMEKEY_RT);
						pAnnotDict->RemoveAt(FSANNOT_DICTKEY_IRT);
					}
				}
			}
		}
	}
	else
	{// Current markup is in a group, not the header.
		// ungroup current markup from the group.
		getDict()->removeAt(FSANNOT_NAMEKEY_RT);
		getDict()->removeAt(FSANNOT_DICTKEY_IRT);
	}

	setModified();
	return TRUE;
}

FS_INT32	FSPDFMarkupImp::getStateAnnotCount(FSM_ANNOTSTATEMODEL stateModel)
{
	if (!getDict() || !getPage()) return 0;
	//PDF Reference 1.7 P621
	//"Additional state changes are made by adding text annotations in reply to the previous reply for a given user"
	// That means, if there are several states (in the same model) for a markup annot, they should be like this:
	// org markup <-- state1 <-- state2 <-- state3...
	CFX_ByteString bsModel = FSANNOT_Util_ConvertStateModelFromEnum(stateModel);
	FS_DWORD dwCurrentObjNum = getDict()->getObjNum();
	FSPDFPage* pPage = getPage();
	FS_INT32 nAnnotCount = pPage->getAnnotCount();
	FS_INT32 nStateAnnotCount = 0;
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		FSAnnot* pAnnot = pPage->getAnnot(i);
		if (!pAnnot || !pAnnot->isMarkup() || !FSAnnot_Util_CheckIsStateAnnot(pAnnot)) 
			continue;
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		// Check if this pAnnot is the state of "current annot"
		if ( pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() == dwCurrentObjNum
			&& pAnnotDict->GetString(FSANNOT_STRINGKEY_STATEMODEL).Equal(bsModel))
		{
			// If yes, change this pAnnot to be the "current annot" and find its state.
			dwCurrentObjNum = pAnnotDict->GetObjNum();
			nStateAnnotCount++;
		}
	}
	return nStateAnnotCount;
}

FSNote*		FSPDFMarkupImp::getStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FS_INT32 index)
{
	if (index<0) return NULL;
	return getStateAnnotImp(stateModel, index);
}

FSNote*		FSPDFMarkupImp::addStateAnnot(FSM_ANNOTSTATEMODEL stateModel, FSM_ANNOTSTATE state)
{
	if (!FSANNOT_Util_IsStateValueValid(stateModel, state) || !getPage())
		return NULL;
	//PDF Reference 1.7 P621
	//"Additional state changes are made by adding text annotations in reply to the previous reply for a given user"
	// That means, if there are several states (in the same model) for a markup annot, they should be like this:
	// org markup <-- state1 <-- state2 <-- state3...

	//statemode = marked, only one
	CFX_ByteString bsModel = FSANNOT_Util_ConvertStateModelFromEnum(stateModel);
	if (bsModel.Equal("Marked") && getStateAnnotCount(stateModel) != 0) {
		FSNote* pStateAnnot = getStateAnnotImp(stateModel, 0);
		if (pStateAnnot == NULL) {
			return NULL;
		}
		pStateAnnot->setState(state);
		return pStateAnnot;
	}

	// First get the object number of the parent of new state.
	FS_DWORD dwStateParentObjNum = 0;
	// Try to get the last state
	FSNote* pLastState = getStateAnnotImp(stateModel, -1);
	if (pLastState)
	{// If last state exists, it should be the parent of new state.
		dwStateParentObjNum = pLastState->getDict()->getObjNum();
	}
	else
	{// If no last state, current markup should be the parent of new state.
		dwStateParentObjNum = getDict()->getObjNum();
	}
	CPDF_Document* pDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
	CPDF_Reference* pIRTRef = FX_NEW CPDF_Reference(pDoc, dwStateParentObjNum);
	if (!pIRTRef) return NULL;

	// Second, creat a new note annot as state annot:
	// empty rect, flag = 30, border color = [0 0 0], no border info, and also set state model/state/IRT.
	FSRectF emptyRect;
	emptyRect.set(0, 0, 0, 0);
	FSAnnot* pNewAnnot = ((FSPDFPageImp*)getPage())->addAnnotImp(FSAnnot::e_annotNote, emptyRect, FALSE);
	if (!pNewAnnot) {
		pIRTRef->Release();
		return NULL;
	}
	FSNote* pStateAnnot = (FSNote*)pNewAnnot;
	//flag = 30, border color = [0 0 0]
	pStateAnnot->setFlags(FSAnnot::e_annotFlagHidden|FSAnnot::e_annotFlagPrint|FSAnnot::e_annotFlagNoZoom|FSAnnot::e_annotFlagNoRotate);
	pStateAnnot->setBorderColor(0xFF000000);
	// Set state model/state/IRT
	((FSPDFNoteImp*)pStateAnnot)->setString(FSANNOT_STRINGKEY_STATEMODEL, bsModel);
	CFX_ByteString bsState = FSANNOT_Util_ConvertStateFromEnum(state);
	((FSPDFNoteImp*)pStateAnnot)->setString(FSANNOT_STRINGKEY_STATE, bsState);
	((CPDF_Dictionary*)pStateAnnot->getDict())->SetAt(FSANNOT_DICTKEY_IRT, pIRTRef);
	// TODO: Need resetap??
	setModified();
	return pStateAnnot;
}

FS_BOOL		FSPDFMarkupImp::removeAllStateAnnots()
{
	if (!getDict() || !getPage()) return FALSE;
	//PDF Reference 1.7 P621
	//"Additional state changes are made by adding text annotations in reply to the previous reply for a given user"
	// That means, if there are several states (in the same model) for a markup annot, they should be like this:
	// org markup <-- state1 <-- state2 <-- state3...

	FS_DWORD dwCurObjNum_MarkedModel = getDict()->getObjNum();
	FS_DWORD dwCurObjNum_ReviewModel = getDict()->getObjNum();
	FSPDFPage* pPage = getPage();
	FS_INT32 nAnnotCount = pPage->getAnnotCount();
	CFX_ArrayTemplate<FSNote*>* pStateAnnots = FX_NEW CFX_ArrayTemplate<FSNote*>();
	if (!pStateAnnots) return FALSE;
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		FSAnnot* pAnnot = pPage->getAnnot(i);
		if (!pAnnot || !pAnnot->isMarkup() || !FSAnnot_Util_CheckIsStateAnnot(pAnnot)) 
			continue;
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		// Check if this pAnnot is the state of "current annot"
		if ( pAnnotDict->GetString(FSANNOT_STRINGKEY_STATEMODEL).Equal("Marked")
			&& pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() == dwCurObjNum_MarkedModel)
		{// For Marked state model
			// change this pAnnot to be the "current annot" and find its state.
			dwCurObjNum_MarkedModel = pAnnotDict->GetObjNum();
			pStateAnnots->Add((FSNote*)pAnnot);
		}
		else if (pAnnotDict->GetString(FSANNOT_STRINGKEY_STATEMODEL).Equal("Review")
				 && pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() == dwCurObjNum_ReviewModel)
		{// For Review state model
			// change this pAnnot to be the "current annot" and find its state.
			dwCurObjNum_ReviewModel = pAnnotDict->GetObjNum();
			pStateAnnots->Add((FSNote*)pAnnot);
		}
	}
	if (pStateAnnots->GetSize()>0)
	{
		// Remove all the state annotations. If state annot has a popup, remove the popup as well.
		for (FS_INT32 i=0; i<pStateAnnots->GetSize(); i++)
		{
			FSNote* pStateAnnot = pStateAnnots->GetAt(i);
			CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pStateAnnot->getDict();
			pAnnotDict->RemoveAt(FSANNOT_DICTKEY_IRT);
			pAnnotDict->RemoveAt(FSANNOT_STRINGKEY_STATE);
			pAnnotDict->RemoveAt(FSANNOT_STRINGKEY_STATEMODEL);
			FSPopup* pPopup = pStateAnnot->getPopup();
			pPage->removeAnnot(pStateAnnot);
			if (pPopup)
				pPage->removeAnnot(pPopup);
		}
		setModified();
		pStateAnnots->RemoveAll();
	}
	SAFE_DELETE(pStateAnnots);
	return TRUE;
}

FS_BOOL FSPDFMarkupImp::isGroupHeader()
{
	// To find one other annotation, whose IRT = current annot and whose RT is "Group".
	// If find such annotation, that means current annot is the group header.
	// If find none, that means current annot is not the group header.
	FSPDFPage* pPage = getPage();
	if (!pPage || !getDict()) return FALSE;

	FS_DWORD dwCurrentAnnotObjNum = getDict()->getObjNum();
	FS_INT32 nAnnotCount = pPage->getAnnotCount();
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		FSAnnot* pAnnot = pPage->getAnnot(i);
		if (!pAnnot || !pAnnot->isMarkup()) 
			continue;

		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		if (!pAnnotDict) continue;
		if (pAnnotDict->KeyExist(FSANNOT_DICTKEY_IRT))
		{
			CPDF_Dictionary* pIRTDict = pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT);
			if(pIRTDict)
			{
				FS_DWORD dwIRTObjNum = pIRTDict->GetObjNum();
				CFX_ByteString bsRT = pAnnotDict->GetString(FSANNOT_NAMEKEY_RT);
				if (dwIRTObjNum == dwCurrentAnnotObjNum && bsRT.Equal("Group"))
					return TRUE;
			}
		}
	}
	return FALSE;
}

CPDF_Dictionary* FSPDFMarkupImp::getGroupHeaderDict()
{
	if (!getPage()) return NULL;
	// Check if in a group.
	if (!isGrouped())
		return NULL;
	// Check if current markup annot is just the group header
	if (isGroupHeader())
		return (CPDF_Dictionary*)getDict();

	return getSubDict(FSANNOT_DICTKEY_IRT);
}

// index=-1 means to get the last state annot.
FSNote*	FSPDFMarkupImp::getStateAnnotImp(FSM_ANNOTSTATEMODEL model, FS_INT32 index)
{
	if (!getDict() || !getPage()) return NULL;
	//PDF Reference 1.7 P621
	//"Additional state changes are made by adding text annotations in reply to the previous reply for a given user"
	// That means, if there are several states (in the same model) for a markup annot, they should be like this:
	// org markup <-- state1 <-- state2 <-- state3...
	FS_BOOL bLast = (-1 == index);
	CFX_ByteString bsModel = FSANNOT_Util_ConvertStateModelFromEnum(model);
	FS_DWORD dwCurrentObjNum = getDict()->getObjNum();
	FSPDFPage* pPage = getPage();
	FS_INT32 nAnnotCount = pPage->getAnnotCount();
	FS_INT32 nStateAnnotCount = 0;
	FSAnnot* pAnnot = NULL;
	for (FS_INT32 i=0; i<nAnnotCount; i++)
	{
		pAnnot = pPage->getAnnot(i);
		if (!pAnnot || !pAnnot->isMarkup() || !FSAnnot_Util_CheckIsStateAnnot(pAnnot) ) 
			continue;
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		// Check if this pAnnot is the state of "current annot"
		if (pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT)->GetObjNum() == dwCurrentObjNum
			&& pAnnotDict->GetString(FSANNOT_STRINGKEY_STATEMODEL).Equal(bsModel))
		{
			nStateAnnotCount++;
			if (!bLast && index == nStateAnnotCount-1)
				break; 
			// If yes, change this pAnnot to be the "current annot" and find its state.
			dwCurrentObjNum = pAnnotDict->GetObjNum();
		}
	}
	if (nStateAnnotCount>0 && index<nStateAnnotCount)
		return (FSNote*)pAnnot;
	return NULL;
}