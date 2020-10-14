//#include "../../include/fs_pdfpage.h"
#include "fs_annot_imp.h"
#include "fs_annoticonprovider.h"
#include "../inc/fs_pdfpageImp.h"


FSPDFNoteImp::FSPDFNoteImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFNoteImp::move(FSRectF rect)
{
	return FSPDFAnnotImp::moveImp((FSNote*)this, rect, TRUE);
}

FSMarkup* FSPDFNoteImp::getReplyTo()
{
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
	if (!FSAnnot_Util_ChectAnnotIsReply(pAnnotDict))
	{
		return NULL;
	}
	CPDF_Dictionary* pIRT = pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT);
	if(!pIRT)
		return NULL;
	FSPDFPageImp* pFSPDFPage = (FSPDFPageImp*)getPage();
	CFX_ArrayTemplate<void*>* pArray = pFSPDFPage->getAnnotArray();
	FS_INT32 annotCount = pArray->GetSize();
	for(int i=0; i<annotCount; i++){
		FSAnnot* pCurAnnot = (FSAnnot*)pArray->GetAt(i);

		if(pCurAnnot && (CPDF_Dictionary*)pCurAnnot->getDict() == pIRT)
		{
			if(pCurAnnot->isMarkup())
				return (FSMarkup *)pCurAnnot;
			return NULL;
		}
	}
	return NULL;
}

//////////////
// Methods inherited from FSNote
FS_BOOL FSPDFNoteImp::getOpenStatus() const
{
	return getBoolean(FSANNOT_BOOLEANKEY_OPEN);
}

void FSPDFNoteImp::setOpenStatus(FS_BOOL status)
{
	setBoolean(FSANNOT_BOOLEANKEY_OPEN, status);
}
	
FSString FSPDFNoteImp::getIconName() const
{
	return getName(FSANNOT_NAMEKEY_ICONNAME);
}

void FSPDFNoteImp::setIconName(const char* iconName)
{
	setName(FSANNOT_NAMEKEY_ICONNAME, iconName);
}

FS_BOOL FSPDFNoteImp::resetAppearanceStream()
{
	if (!getPDFDict() || !getPage()) 
		return FALSE;
	CPDF_Page* pPDFPage = ((FSPDFPageImp*)getPage())->getPage();
	if (NULL == pPDFPage || NULL == pPDFPage->m_pDocument)
		return FALSE;

	// If current note annot is a reply, do not reset ap.
	if (isReply())
		return TRUE;

	CPDF_Document* pDoc = pPDFPage->m_pDocument;
	CFSPDF_AnnotIconProvider annotIconProvider(pDoc);
	return annotIconProvider.generateAPStream(this);
}

FS_BOOL		FSPDFNoteImp::isStateAnnot()
{
	return FSAnnot_Util_CheckIsStateAnnot((FSNote*)this);
}

FSM_ANNOTSTATEMODEL	FSPDFNoteImp::getStateModel()
{
	if (!isStateAnnot()) return (FSM_ANNOTSTATEMODEL)0;

	CFX_ByteString bsModel = getStringImpl(FSANNOT_STRINGKEY_STATEMODEL);
	return FSANNOT_Util_ConvertStateModelToEnum(bsModel);
}

FSM_ANNOTSTATE		FSPDFNoteImp::getState()
{
	if (!isStateAnnot()) return (FSM_ANNOTSTATE)0;

	CFX_ByteString bsState = getStringImpl(FSANNOT_STRINGKEY_STATE);
	return FSANNOT_Util_ConvertStateToEnum(bsState);
}

void	FSPDFNoteImp::setState(FSM_ANNOTSTATE state)
{
	FSM_ANNOTSTATEMODEL stateModel = getStateModel();
	if (!FSANNOT_Util_IsStateValueValid(stateModel, state))
		return ;

	CFX_ByteString bsState = FSANNOT_Util_ConvertStateFromEnum(state);
	setString(FSANNOT_STRINGKEY_STATE, bsState);
	setModified();
}

FS_BOOL FSPDFNoteImp::isReply()
{
	return FSAnnot_Util_ChectAnnotIsReply(getPDFDict());
}