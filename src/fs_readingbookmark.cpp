#include "../include/fs_readingbookmark.h"

#include "fxcore.h"
#include "fs_util.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_readingbookmarkImp.h"

/////////////////////////////////////
//class FSReadingBookmarkImp
FSReadingBookmarkImp::FSReadingBookmarkImp(CXML_Element* pReadingBookmarkElement, FSPDFDocImp* pDoc)
	:m_pReadingBookmarkElement(pReadingBookmarkElement)
	,m_pDoc(pDoc)
{}

FSReadingBookmarkImp::~FSReadingBookmarkImp()
{}

//<Bookmark:Title>
FSString FSReadingBookmarkImp::getTitle() const
{
	if (!m_pReadingBookmarkElement) return FSString();

	CXML_Element* pTitleElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_TITLE);
	if (!pTitleElement) return FSString();

	CFX_WideString wsTitle = pTitleElement->GetContent(0);

	FSString strTitle = FSString::createFromWideString(wsTitle);
	strTitle.toUTF8();

	return strTitle;
}

void FSReadingBookmarkImp::setTitle(const char* title)
{
	if (!m_pDoc || !m_pReadingBookmarkElement || FSCRT_Util_IsStringEmpty(title)) return;
	CFX_WideString wsTitle = CFX_WideString::FromUTF8(title, FXSYS_strlen(title));

	CXML_Element* pTitleElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_TITLE);
	if (!pTitleElement)
	{
		pTitleElement = FX_NEW CXML_Element(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_TITLE);
		if (!pTitleElement) return;
		m_pReadingBookmarkElement->InsertChildElement(0, pTitleElement);
	}
	pTitleElement->RemoveChildren();
	pTitleElement->AddChildContent(wsTitle);

	m_pDoc->updateMetadataStream();
	m_pDoc->setModified();
}

//<Bookmark:PageIndex>
FS_INT32 FSReadingBookmarkImp::getPageIndex() const
{
	if (!m_pReadingBookmarkElement) return -1;

	CXML_Element* pPageIndexElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_PAGEINDEX);
	if (!pPageIndexElement) -1;

	CFX_WideString wsPageIndex = pPageIndexElement->GetContent(0);

	return wsPageIndex.GetInteger();
}

void FSReadingBookmarkImp::setPageIndex(FS_INT32 index)
{
	if (!m_pDoc || !m_pReadingBookmarkElement || index < 0) return;

	FS_INT32 pageCount = 0;
	m_pDoc->GetPageCount(&pageCount);

	if (index >= pageCount) return;

	CFX_ByteString bsPageIndex;
	bsPageIndex.Format("%d", index);
	
	CFX_WideString wsPageIndex;
	wsPageIndex.ConvertFrom(bsPageIndex);

	CXML_Element* pPageIndexElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_PAGEINDEX);
	if (!pPageIndexElement)
	{
		pPageIndexElement = FX_NEW CXML_Element(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_PAGEINDEX);
		if (!pPageIndexElement) return;
		m_pReadingBookmarkElement->AddChildElement(pPageIndexElement);
	}
	pPageIndexElement->RemoveChildren();
	pPageIndexElement->AddChildContent(wsPageIndex);

	m_pDoc->updateMetadataStream();
	m_pDoc->setModified();
}

FSDateTime FSReadingBookmarkImp::getDateTime(FS_BOOL isCreationDate) const
{
	FSDateTime dateTime;
	dateTime.set(0, 0, 0, 0, 0, 0, 0, 0, 0);

	if (!m_pReadingBookmarkElement) return dateTime;

	CXML_Element* pDateTimeElement = NULL;
	if (isCreationDate)
		pDateTimeElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_CREATIONDATETIME);
	else
		pDateTimeElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, FSREADINGBOOKMARK_CHILDPARAMTAG_MODIFIEDDATETIME);

	if (!pDateTimeElement) return dateTime;

	CFX_WideString wsDateTime = pDateTimeElement->GetContent(0);
	if (wsDateTime.IsEmpty()) return dateTime;

	CFSPDF_DateTime fsPDFDateTime;
	if (fsPDFDateTime.ParserPDFXMPDateTimeString(wsDateTime))
	{
		dateTime = fsPDFDateTime.ToSystemTime();
		dateTime.UTHourOffset = fsPDFDateTime.m_fsDatetime.UTHourOffset;
		dateTime.UTMinuteOffset = fsPDFDateTime.m_fsDatetime.UTMinuteOffset;
	}

	return dateTime;
}

void FSReadingBookmarkImp::setDateTime(FSDateTime dateTime, FS_BOOL isCreationDate)
{
	if (!m_pDoc || !m_pReadingBookmarkElement || !FSCRT_Util_CheckDateTime(dateTime)) return;
	
	CFSPDF_DateTime fsPDFDateTime(dateTime);
	CFX_WideString wsDateTime = fsPDFDateTime.ToXMPDateTimeString();
	if (wsDateTime.IsEmpty()) return;

	CXML_Element* pDateTimeElement = NULL;
	CFX_ByteString bsTagName;
	if (isCreationDate)
		bsTagName = FSREADINGBOOKMARK_CHILDPARAMTAG_CREATIONDATETIME;
	else
		bsTagName = FSREADINGBOOKMARK_CHILDPARAMTAG_MODIFIEDDATETIME;

	pDateTimeElement = m_pReadingBookmarkElement->GetElement(FSREADINGBOOKMARK_ROOTTAG, bsTagName);
	if (!pDateTimeElement)
	{
		pDateTimeElement = FX_NEW CXML_Element(FSREADINGBOOKMARK_ROOTTAG, bsTagName);
		if (!pDateTimeElement) return;
		m_pReadingBookmarkElement->AddChildElement(pDateTimeElement);
	}
	pDateTimeElement->RemoveChildren();
	pDateTimeElement->AddChildContent(wsDateTime);

	m_pDoc->updateMetadataStream();
	m_pDoc->setModified();
}