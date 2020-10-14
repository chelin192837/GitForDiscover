#ifndef _FR_OFDSEARCHIMP_H_
#define _FR_OFDSEARCHIMP_H_

#include "../../include/common.h"
#include "../../include/fs_pdfpage.h"
#include "../../include/fs_search.h"

class FR_OFDTextSearchImp : public FSPDFTextSearch, public CFX_Object
{
public:
	FR_OFDTextSearchImp(FSPDFDoc* pPDFDoc, FSPause* pause);
	virtual ~FR_OFDTextSearchImp();

public:
#ifndef _SWIG_IOS_
	virtual void		release();
#endif

	virtual FS_BOOL		setKeyWords(const char* keyWords);
	virtual FS_BOOL		setStartPage(FS_INT32 pageIndex);
	virtual FS_BOOL		setFlag(FS_DWORD flag);
	virtual FS_BOOL		findNext();
	virtual FS_BOOL		findPrev();

	virtual FS_INT32	getMatchRectCount();
	virtual FSRectF		getMatchRect(FS_INT32 index) const;
	virtual FS_INT32	getMatchPageIndex() const;

	virtual FSString getMatchSentence();
	virtual FS_INT32 getMatchSentenceStartIndex();
	virtual FS_INT32 getMatchStartCharIndex() const;
	virtual FS_INT32 getMatchEndCharIndex() const;


private:
	// return value: 0 for false, 1 for true, 2 for pause.
	FS_INT32 initSearch(FS_BOOL bNext);
	void	 saveState(FS_BOOL bNext);
	void	 clear(FS_BOOL bClearSearch = FALSE);

public:
	FSCRT_DEFINELOCK(m_lock);
	FSPDFDoc*			m_pPDFDoc;
//	CPDF_Page*			m_pCurrPDFPage;
	//IOFD_Page*			m_pCurrPDFPage;
	FSPDFPage*			m_pCurrPDFPage;
	FS_INT32			m_ParsedPageIndex;
//	IPDF_TextPage*		m_pCurrTextPage;
	IOFD_TextPage*		m_pCurrTextPage;
	FS_INT32			m_startPageIndex;
	FS_DWORD			m_searchFlag;
	char*				m_Pattern;
	FS_INT32			m_keyLen;
	FSPause*			m_pause;

//	IPDF_TextPageFind*	m_pSearch;
	IOFD_TextPageFind*	m_pSearch;

 	FS_INT32			m_curPageIndex;
	CFX_ArrayTemplate<CFX_FloatRect>* m_pRects;
	FS_INT32			m_SentenceStart;
	FS_INT32			m_KeyStart;
	FSString			m_Sentence;
	FS_INT32			m_bFindNext;
};

class FR_OFDTextSelectImp: public FSPDFTextSelect, public CFX_Object
{
public:
	FR_OFDTextSelectImp();
	virtual void release();
	virtual ~FR_OFDTextSelectImp();
	virtual FSPDFPage*		getPage() const;
	virtual FS_INT32		getCharCount() const;
	virtual FSString		getChars(FS_INT32 startIndex, FS_INT32 count) const;
	virtual FS_INT32		getIndexAtPos(FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance) const;
	virtual FSString		getTextInRect(FSRectF rect) const;
	virtual FS_BOOL			getWordAtPos(FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, FS_INT32& startIndex, FS_INT32& count) const; 
	virtual FS_INT32		getTextRectCount(FS_INT32 start, FS_INT32 nCount);
	virtual FSRectF			getTextRect(FS_INT32 rectIndex) const;
	virtual FS_ROTATION		getBaselineRotation(FS_INT32 rectIndex);

	// Internal use
	FS_BOOL				init(FSPDFPage* pPage);
private:
	FSPDFPage* m_pFSPDFPage;
	IOFD_TextPageSelect* m_pTextSelect;
	CFX_ArrayTemplate<CFX_RectF>	m_TextRect;
};



#endif