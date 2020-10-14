//#include "../../include/fs_pdfpage.h"
#include "fs_annot_imp.h"
#include "fs_annoticonprovider.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/commonImp.h"
#include "../inc/fs_pdfdocImp.h"


FSFileAttachmentImp::FSFileAttachmentImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	m_pFileSpec = NULL;
	FSPDFMarkupImp::init(pAnnotDict, page); 
}
FSFileAttachmentImp::~FSFileAttachmentImp()
{
	if(m_pFileSpec)
		m_pFileSpec->release();
}

FS_BOOL	FSFileAttachmentImp::move(FSRectF rect)
{
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
    if (!pAnnotDict || !CHECKVALID_FLOATRECT_PDF(rect)) return FALSE;
	CPDF_Rect rtNew(rect.left, rect.bottom, rect.right, rect.top);
	pAnnotDict->SetAtRect(FX_BSTRC("Rect"), rtNew);
	return resetAppearanceStream();
}

FS_BOOL FSFileAttachmentImp::resetAppearanceStream()
{
	if (!getPDFDict() || !getPage()) 
		return FALSE;
	CPDF_Page* pPDFPage = ((FSPDFPageImp*)getPage())->getPage();
	if (NULL == pPDFPage || NULL == pPDFPage->m_pDocument)
		return FALSE;

	CPDF_Document* pDoc = pPDFPage->m_pDocument;
	CFSPDF_AnnotIconProvider annotIconProvider(pDoc);
	return annotIconProvider.generateAPStream(this);
}

FS_BOOL FSFileAttachmentImp::setFileSpec(FSFileSpec* fileSpecification)
{
	if (!fileSpecification || !getDict()) return FALSE;

	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
	pAnnotDict->SetAtReference(FX_BSTRC("FS"), ((FSPDFDocImp*)((FSFileSpecImp*)fileSpecification)->getDoc())->getPDFDocument(),
		((FSFileSpecImp*)fileSpecification)->getDict());

	return TRUE;
}

FSFileSpec* FSFileAttachmentImp::getFileSpec()
{
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
	if (!pAnnotDict) return NULL;

	if (!pAnnotDict->KeyExist(FX_BSTRC("FS")))
	{
		m_pFileSpec = NULL;
		return NULL;
	}

	if (m_pFileSpec)
	{
		m_pFileSpec->release();
		m_pFileSpec = NULL;
	}

	CPDF_Object* pAttachObj = pAnnotDict->GetElementValue(FX_BSTRC("FS"));
	if(pAttachObj->GetDict())
	{
		FSPDFPage* page = getPage();
		FSPDFDoc* doc = NULL;
		if(page)
			doc = page->getDocument();
		m_pFileSpec =  FX_NEW FSFileSpecImp(doc, pAttachObj->GetDict());
	}

	return m_pFileSpec;
}

FSString FSFileAttachmentImp::getIconName() const
{
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
	if (!pAnnotDict) return FSString();
	if(!pAnnotDict->KeyExist(FX_BSTRC("Name")))
	{
		return FSString("PushPin");
	}
	CFX_ByteString bsIconName = pAnnotDict->GetString(FX_BSTRC("Name"));
	if (bsIconName.GetLength() == 0)
		bsIconName = "PushPin";
	return FSString(bsIconName);
}

void FSFileAttachmentImp::setIconName(const char* iconName)
{
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
	if (!pAnnotDict) return;
	if (NULL == iconName || strlen(iconName) == 0)
		pAnnotDict->SetAtName(FX_BSTRC("Name"), CFX_ByteString("PushPin"));
	else
		pAnnotDict->SetAtName(FX_BSTRC("Name"), CFX_ByteString(iconName));
}

