#ifndef _FS_PDFTEXT_IMP_H_
#define _FS_PDFTEXT_IMP_H_

#include "../../include/fs_pdfpage.h"
//#include "../../include/fs_search.h"

class IPDF_TextPage;

class FSPDFTextSelectImp: public FSPDFTextSelect, public CFX_Object
{
public:
	FSPDFTextSelectImp();
	virtual void release();
	virtual ~FSPDFTextSelectImp();
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
};

class FSPDFTextLinkImp : public FSPDFTextLink, public CFX_Object
{
public:
	FSPDFTextLinkImp();
	virtual ~FSPDFTextLinkImp(){}
	virtual FSString getURI();
	virtual FS_INT32 getStartCharIndex();
	virtual FS_INT32 getEndCharIndex();
	virtual FS_INT32 getRectCount();
	virtual FSRectF getRect(int rectIndex);
	
	// Internal use
	void	init(IPDF_LinkExtract* pLinkExtract, FS_INT32 index);
protected:
	FSCRT_DEFINELOCK(m_lock);
	IPDF_LinkExtract*	m_pLinkExt;
	FS_INT32			m_linkIndex;
};

class FSPDFPageLinksImp : public FSPDFPageLinks, public CFX_Object
{
public:
	FSPDFPageLinksImp();
	virtual ~FSPDFPageLinksImp();
	void release();
	virtual FS_INT32 getTextLinkCount();
	virtual FSPDFTextLink* getTextLink(FS_INT32 index);
	virtual FS_INT32 getLinkAnnotCount();
	virtual FSLink* getLinkAnnot(FS_INT32 index);

	// Internal use
	FS_BOOL			init(FSPDFPage* pPage);
protected:
	FSCRT_DEFINELOCK(m_lock);
	FSPDFPage*			m_pPage;
	IPDF_LinkExtract*	m_pLinkExt;
	FSPDFTextLinkImp*	m_pTextLinks;
	FS_INT32			m_nTextLinks;
	FS_INT32			m_nLinkAnnots;
};


class FSPDFTextSearchImp : public FSPDFTextSearch, public CFX_Object
{
public:
	FSPDFTextSearchImp(FSPDFDoc* pPDFDoc, FSPause* pause);
	virtual void release();
	virtual ~FSPDFTextSearchImp();

	FS_BOOL	setKeyWords(const char* keyWords);
	FS_BOOL	setStartPage(FS_INT32 pageIndex);
	FS_BOOL	setFlag(FS_DWORD flag);

	FS_BOOL findNext();
	FS_BOOL findPrev();

	FS_INT32 getMatchRectCount();
	FSRectF getMatchRect(FS_INT32 index) const;
	FS_INT32 getMatchPageIndex() const;
	FSString getMatchSentence();
	FS_INT32 getMatchSentenceStartIndex();
	FS_INT32 getMatchStartCharIndex() const;
	FS_INT32 getMatchEndCharIndex() const;
private:
	// return value: 0 for false, 1 for true, 2 for pause.
	FS_INT32 initSearch(FS_BOOL bNext);
	void	 saveState(FS_BOOL bNext);
	void	 clear(FS_BOOL bClearSearch = FALSE);

private:
	FSCRT_DEFINELOCK(m_lock);
	FSPDFDoc*			m_pPDFDoc;
	CPDF_Page*			m_pCurrPDFPage;
	FS_INT32			m_ParsedPageIndex;
	IPDF_TextPage*		m_pCurrTextPage;
	FS_INT32			m_startPageIndex;
	FS_DWORD			m_searchFlag;
	char*				m_Pattern;
	FS_INT32			m_keyLen;
	FSPause*			m_pause;

	IPDF_TextPageFind*	m_pSearch;

 	FS_INT32			m_curPageIndex;
	CFX_ArrayTemplate<CFX_FloatRect>* m_pRects;
	FS_INT32			m_SentenceStart;
	FS_INT32			m_KeyStart;
	FSString			m_Sentence;
	FS_INT32			m_bFindNext;
};

#endif //_FS_PDFTEXT_IMP_H_
