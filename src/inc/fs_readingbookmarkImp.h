#ifndef _FS_READINGBOOKMARK_IMP_H_
#define _FS_READINGBOOKMARK_IMP_H_

#include "../../include/fs_readingbookmark.h"

//<pdfx:FoxitReadingBookmark> 
#define FSREADINGBOOKMARK_ROOTSPACE							"FoxitReadingBookmark"
//<FoxitReadingBookmark:Bookmark>
#define FSREADINGBOOKMARK_ROOTTAG							"Bookmark"
//<Bookmark:Title>
#define FSREADINGBOOKMARK_CHILDPARAMTAG_TITLE				"Title"
//<Bookmark:PageIndex>
#define FSREADINGBOOKMARK_CHILDPARAMTAG_PAGEINDEX			"PageIndex"
//<Bookmark:CreateDateTime>
#define FSREADINGBOOKMARK_CHILDPARAMTAG_CREATIONDATETIME	"CreateDateTime"
//<Bookmark:ModifyDateTime>
#define FSREADINGBOOKMARK_CHILDPARAMTAG_MODIFIEDDATETIME	"ModifyDateTime"

class FSReadingBookmarkImp : public FSReadingBookmark, public CFX_Object
{
public:
	virtual ~FSReadingBookmarkImp();

	virtual FSString	getTitle() const;
	virtual void		setTitle(const char* title);
	virtual FS_INT32	getPageIndex() const;
	virtual void		setPageIndex(FS_INT32 index);
	virtual FSDateTime	getDateTime(FS_BOOL isCreationDate) const;
	virtual void		setDateTime(FSDateTime dateTime, FS_BOOL isCreationDate);

	FSReadingBookmarkImp(CXML_Element* pReadingBookmarkElement, FSPDFDocImp* pDoc);
private:
	//<FoxitReadingBookmark:Bookmark>
	CXML_Element*	m_pReadingBookmarkElement;
	FSPDFDocImp*	m_pDoc;
};
#endif //_FS_READINGBOOKMARK_IMP_H_