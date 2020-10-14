#include "../include/fs_pdfdoc.h"
#include "../include/fs_pdfpage.h"
#include "../include/fs_search.h"

#include "fxcore.h"
#include "fs_file.h"
#include "fs_util.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_pdftextImp.h"
#include "inc/fs_pdfdocImp.h"

extern FSPDFTextSearch* createOFDTextSearch(FSPDFDoc* pPDFDoc, FSPause* pause);
extern FSPDFTextSelect* createOFDTextSelect(FSPDFPage* pPage);

////////////////////////////////////////////////////////////////////////////
// Class FSPDFTextSearch
FSPDFTextSearch* FSPDFTextSearch::create(FSPDFDoc* pPDFDoc, FSPause* pause)
{
	if(!pPDFDoc)
		return NULL;
//	if (dynamic_cast<FSPDFDocImp*>(pPDFDoc)) {
	if ((FSPDFDocImp*)(pPDFDoc)) {
		return FX_NEW FSPDFTextSearchImp(pPDFDoc, pause);
	} else {
		return createOFDTextSearch(pPDFDoc, pause);
	}
}

static FS_BOOL _IsSentenceSeparators(FX_WCHAR ch)
{
	CFX_WideString separators = CFX_WideStringC(L".?!:;。？！：；");
	return (separators.Find(ch) > -1);
}

static FS_BOOL _IsPunctuationSeparators(FX_WCHAR ch)
{
	CFX_WideString separators = CFX_WideStringC(L",.?!:;`~ ，、。？！：；｀～ ");
	return (separators.Find(ch) > -1);
}

static FX_DWORD UnicodeFromCharInfo(FPDF_CHAR_INFO& charinfo)
{
	FX_DWORD unicode = 0;
	if (charinfo.m_Unicode.GetSize() > 0) {
		unicode = charinfo.m_Unicode[0];
	}
	return unicode;
}

#define FS_PATTERNPOS_MAX	36
#define FS_SENTENCELENGTH_MIN	50
#define FS_SENTENCELENGTH_MAX	70
static FSString _GetMatchSentence(IPDF_TextPage* pTextPage, FS_INT32 keyLen, FS_INT32 pos, FS_INT32& keyStart)
{
	int i, startpos = 0, endpos = pTextPage->CountChars() - 1;
	FPDF_CHAR_INFO charinfo;

	for (i = pos - 1; i >= 0; i --) {
		pTextPage->GetCharInfo(i, charinfo);
		if (_IsSentenceSeparators(UnicodeFromCharInfo(charinfo))) {
			startpos = i + 1;
			break;
		}
	}

	if (pos - startpos > FS_PATTERNPOS_MAX) {
		startpos = pos - FS_PATTERNPOS_MAX;
		for (i = startpos; i < pos; i ++) {
			pTextPage->GetCharInfo(i, charinfo);
			if (UnicodeFromCharInfo(charinfo) <= 0x20) {
				pTextPage->GetCharInfo(i + 1, charinfo);
				if (UnicodeFromCharInfo(charinfo) > 0x20 && !_IsPunctuationSeparators(UnicodeFromCharInfo(charinfo))) {
					startpos = i + 1;
					break;
				}
			}
			if (i == pos - 1) {
				startpos = pos;
				break;
			}
		}
	}

	for (i = startpos; i <= pos; i ++) {
		pTextPage->GetCharInfo(i, charinfo);
		if (UnicodeFromCharInfo(charinfo) <= 0x20)
			startpos ++;
		else
			break;
	}

	for (i = pos + keyLen; i <= endpos; i ++) {
		pTextPage->GetCharInfo(i, charinfo);
		if ((_IsSentenceSeparators(UnicodeFromCharInfo(charinfo)) && i - startpos > FS_SENTENCELENGTH_MIN) || i == endpos) {
			endpos = i;
			break;
		}
	}
	FS_INT32 sentenceLen = endpos - startpos +1;
	if (sentenceLen > FS_SENTENCELENGTH_MAX)
	{
		sentenceLen = FS_SENTENCELENGTH_MAX;
	}
	CFX_WideString sentence = pTextPage->GetPageText(startpos, sentenceLen);
	FX_LPWSTR wStr = sentence.GetBuffer(sentence.GetLength());
	for(i=sentence.GetLength()-1; i>=0; i--){
		switch (sentence.GetAt(i))
		{
			case '\t':
			case '\n':
			case '\r':
				{
					wStr[i] = ' ';
					break;

				}
		}
	}

//	SentenceStart = startpos;

// 	CFX_WideString str1;
// 	CFX_WideString str2;
 	keyStart = pos - startpos;
// 	for (int i = 0; i < keyLen; i ++) {
// 		str1 = pattern[i];
// 		str2 = sentence[keyStart + i];
// 		while (str1.CompareNoCase(str2) != 0 && sentence.GetLength() > 0) {
// 			sentence.Delete(keyStart + i, 1);
// 			str2 = sentence[keyStart + i];
// 		}
// 	}
	FSString str = FSString::createFromWideString((const wchar_t *)wStr);
	str.toUTF8();
	return str;
}

////////////////////////////////////////////
// Class FSPDFTextSelectImp
FSPDFTextSelectImp::FSPDFTextSelectImp()
	: m_pFSPDFPage(NULL)
{
}
void FSPDFTextSelectImp::release()
{
	delete this;
}
FSPDFTextSelectImp::~FSPDFTextSelectImp()
{
}

FSPDFTextSelect* FSPDFTextSelect::create(FSPDFPage* pPage)
{
//	if (dynamic_cast<FSPDFPageImp*>(pPage)) {
	if ((FSPDFPageImp*)(pPage)) {
		if(!pPage || !pPage->isParsed())
			return NULL;
		FSPDFTextSelectImp* pTextPage = FX_NEW FSPDFTextSelectImp();
		if(pTextPage && pTextPage->init(pPage))
			return pTextPage;
		SAFE_DELETE(pTextPage);
		return NULL;
	} else {
		return createOFDTextSelect(pPage);
	}
}

FS_BOOL FSPDFTextSelectImp::init(FSPDFPage* pPage)
{
	if(pPage && ((FSPDFPageImp*)pPage)->getTextPage()){
		m_pFSPDFPage = pPage;
		return TRUE;
	}
	return FALSE;
}

FSPDFPage* FSPDFTextSelectImp::getPage() const
{
	return m_pFSPDFPage;
}

//Text extraction and selection.
FS_INT32 FSPDFTextSelectImp::getCharCount() const
{
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage)
		return 0;
	return pTextPage->CountChars();
}

FSString FSPDFTextSelectImp::getChars(FS_INT32 startIndex, FS_INT32 count) const
{
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || startIndex < 0 || startIndex>=getCharCount() || count < -1)
		return FSString();
	CFX_WideString wcsText = pTextPage->GetPageText(startIndex,count);
	FSString str = FSString::createFromWideString(wcsText);
	str.toUTF8();
	return str;
}

FSString FSPDFTextSelectImp::getTextInRect(FSRectF rect) const
{
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || FALSE == CHECKVALID_FLOATRECT_PDF(rect))
		return FSString();
	CFX_FloatRect fxRect(rect.left, rect.bottom, rect.right, rect.top);
	CFX_WideString wstr = pTextPage->GetTextByRect(fxRect);
	FSString str = FSString::createFromWideString(wstr);
	str.toUTF8();
	return str;
}

FS_INT32 FSPDFTextSelectImp::getIndexAtPos(FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance) const
{
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || tolerance<0)
		return -1;
	int ret = pTextPage->GetIndexAtPos(x, y, tolerance, tolerance);
	return ret < 0 ? -1: ret;
}

FS_BOOL FSPDFTextSelectImp::getWordAtPos(FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, FS_INT32& startIndex, FS_INT32& count) const
{
	startIndex = 0;
	count = 0;
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	if (!pTextPage || tolerance<0)
		return FALSE;
	FS_INT32 charIndex = getIndexAtPos(x, y, tolerance);
	if(charIndex == -1)
		return FALSE;
	FPDF_CHAR_INFO charInfo;
	pTextPage->GetCharInfo(charIndex, charInfo);
	if(_IsPunctuationSeparators(UnicodeFromCharInfo(charInfo))){
		startIndex = charIndex;
		count = 1;
		return TRUE;
	}
	for(int i=charIndex-1; i>=0; i--){
		pTextPage->GetCharInfo(i, charInfo);
		if(charInfo.m_Unicode[0] == '\r' || charInfo.m_Unicode[0] == '\n' || charInfo.m_Unicode[0] > 255 || _IsPunctuationSeparators(UnicodeFromCharInfo(charInfo))){
			startIndex = i+1;
			break;
		}
	}
	if(startIndex < 0)
		startIndex = 0;
	FS_INT32 nChars = pTextPage->CountChars();
	for(int i=charIndex+1; i<nChars; i++){
		pTextPage->GetCharInfo(i, charInfo);
		FS_INT32 ch =  charInfo.m_Unicode[0];
		if(ch !=0 && (ch == '\r' || ch == '\n' || ch > 255 || _IsPunctuationSeparators(UnicodeFromCharInfo(charInfo)))){
			count = i - startIndex;
			break;
		}
	}
	return TRUE;
}

FS_INT32 FSPDFTextSelectImp::getTextRectCount(FS_INT32 start, FS_INT32 nCount)
{
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || start<0 || start>=getCharCount() || nCount < -1)
		return 0;
	FPDF_CHAR_INFO info;
	pTextPage->GetCharInfo(start, info);
	return pTextPage->CountRects(start, nCount);
}

FSRectF FSPDFTextSelectImp::getTextRect(FS_INT32 rectIndex) const
{
	FSRectF rectF;
	rectF.set(0, 0, 0, 0);
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	// We don't do judgment for the upper limit of rectIndex, because we cannot get rectCount easily here.
	// We cannot store rectCount in FSPDFTextSelectImp either because pTextPage may be changed when page is re-parsed.
	// The judgment for upper limit of rectIndex will be done by fxcore API IPDF_TextPage::GetRect().
	if(!pTextPage || rectIndex<0)
		return rectF;
	FX_FLOAT left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	pTextPage->GetRect(rectIndex, left, top, right,bottom) ;
	rectF.set(left, bottom, right, top);
	return rectF;
}

FS_ROTATION FSPDFTextSelectImp::getBaselineRotation(FS_INT32 rectIndex)
{
	IPDF_TextPage* pTextPage = ((FSPDFPageImp*)m_pFSPDFPage)->getTextPage();
	// We don't do judgment for the upper limit of rectIndex, because we cannot get rectCount easily here.
	// We cannot store rectCount in FSPDFTextSelectImp either because pTextPage may be changed when page is re-parsed.
	// The judgment for upper limit of rectIndex will be done by fxcore API IPDF_TextPage::GetBaselineRotate().
	if(!pTextPage || rectIndex<0)
		return FSCommonDefines::e_rotationUnknown;

	int Rotate;
	FX_BOOL ret = pTextPage->GetBaselineRotate(rectIndex, Rotate);
	if(ret == FALSE)
		return FSCommonDefines::e_rotationUnknown;
	Rotate = Rotate % 360;
	if(Rotate > 360 || Rotate < 20)
		return FSCommonDefines::e_rotation0;
	if(Rotate > 70 && Rotate < 110)
		return FSCommonDefines::e_rotation90;
	if(Rotate > 160 && Rotate < 200)
		return FSCommonDefines::e_rotation180;
	if(Rotate > 250 && Rotate < 290)
		return FSCommonDefines::e_rotation270;
	return FSCommonDefines::e_rotationUnknown;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class FSPDFTextSearchImp
FSPDFTextSearchImp::FSPDFTextSearchImp(FSPDFDoc* pPDFDoc, FSPause* pause)
	: m_startPageIndex(0)
	, m_pPDFDoc(pPDFDoc)
	, m_pause(pause)
	, m_Pattern(NULL)
//	, m_bParser(FALSE)
	, m_pRects(NULL)
	, m_pSearch(NULL)
	, m_curPageIndex(-1)
	, m_bFindNext(-1)
	, m_KeyStart(-1)
	, m_pCurrPDFPage(NULL)
	, m_searchFlag(e_searchNormal)
	, m_ParsedPageIndex(-1)
	, m_pCurrTextPage(NULL)
{
}

void FSPDFTextSearchImp::release()
{
	delete this;
}

FSPDFTextSearchImp::~FSPDFTextSearchImp()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (m_Pattern)
	{
		FX_Free(m_Pattern);
		m_Pattern = NULL;
	}
	SAFE_DELETE(m_pRects);
	SAFE_DELETE(m_pSearch);
	SAFE_DELETE(m_pCurrTextPage);
	SAFE_DELETE(m_pCurrPDFPage);
}

FS_BOOL FSPDFTextSearchImp::setKeyWords(const char* keyWords)
{
	if(!keyWords)
		return FALSE;
	int len = FXSYS_strlen(keyWords);
	if(len == 0)
		return FALSE;
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (m_Pattern) {
		FX_Free(m_Pattern);
		m_Pattern = NULL;
	}
	m_Pattern = (char*)FX_Alloc(char,len+1);
	if(!m_Pattern)
		return FALSE;
	FXSYS_memcpy(m_Pattern, keyWords, sizeof(char) * len);
	m_Pattern[len] = 0;
	SAFE_DELETE(m_pSearch);
	m_bFindNext = -1;
	return TRUE;
}

FS_BOOL FSPDFTextSearchImp::setFlag(FS_DWORD flag)
{
	if(flag < e_searchNormal || flag > (e_searchNormal|e_searchMatchCase|e_searchMatchWholeWord|e_searchConsecutive)) 
		return FALSE;
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_searchFlag = flag;
	SAFE_DELETE(m_pSearch);
	m_bFindNext = -1;
	return TRUE;
}

FS_BOOL FSPDFTextSearchImp::setStartPage(FS_INT32 pageIndex)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(pageIndex<0 || pageIndex >= m_pPDFDoc->getPageCount())
		return FALSE;
	m_startPageIndex = pageIndex;
	SAFE_DELETE(m_pSearch);
	m_bFindNext = -1;
	return TRUE;
}

FS_INT32 FSPDFTextSearchImp::initSearch(FS_BOOL bNext)
{
	FS_INT32 startCharIndex;
	if(bNext)
		startCharIndex = 0;
	else
		startCharIndex = - 1;

	FSCRT_LOCKOBJECT(lock, &((FSPDFDocImp*)m_pPDFDoc)->m_lockPages);
	FSCRT_LOCKOBJECT(lock1, &m_lock);
	m_bFindNext = bNext;
	if(m_pCurrPDFPage && m_ParsedPageIndex != m_curPageIndex){
		SAFE_DELETE(m_pCurrTextPage);
		SAFE_DELETE(m_pCurrPDFPage);
	}
	if(!m_pCurrPDFPage || m_ParsedPageIndex != m_curPageIndex){
		CPDF_Document* pPDFDocument = ((FSPDFDocImp*)m_pPDFDoc)->getPDFDocument();
		CPDF_Dictionary* pageDic = pPDFDocument->GetPage(m_curPageIndex);
		if(!pageDic)
			return FALSE;
		m_pCurrPDFPage = FX_NEW CPDF_Page();
		if(!m_pCurrPDFPage) return FALSE;
		m_pCurrPDFPage->Load(pPDFDocument, pageDic);
		CPDF_ParseOptions parseOption;
		parseOption.m_bTextOnly = TRUE;
		m_pCurrPDFPage->StartParse(&parseOption, FALSE);
		m_pCurrPDFPage->ContinueParse(NULL);
		m_ParsedPageIndex = m_curPageIndex;
		CPDFText_ParseOptions opt;
		m_pCurrTextPage = IPDF_TextPage::CreateTextPage(m_pCurrPDFPage, opt);
		if(!m_pCurrTextPage){
			SAFE_DELETE(m_pCurrPDFPage);
			return FALSE;
		}
		m_pCurrTextPage->ParseTextPage();
	}
	if (m_pRects && m_pRects->GetSize() > 0) {
		m_pRects->RemoveAll();
	}
	SAFE_DELETE(m_pSearch);
	m_pSearch = IPDF_TextPageFind::CreatePageFind(m_pCurrTextPage);
	if(!m_pSearch)
		return FALSE;

	CFX_WideString wstr = CFX_WideString::FromUTF8(m_Pattern);
	m_keyLen = wstr.GetLength();
	FS_INT32 searchFlags = 0;
	if(m_searchFlag & e_searchMatchCase)
		searchFlags |= FPDFTEXT_MATCHCASE;
	if(m_searchFlag & e_searchMatchWholeWord)
		searchFlags |= FPDFTEXT_MATCHWHOLEWORD;
	if(m_searchFlag & e_searchConsecutive)
		searchFlags |= FPDFTEXT_CONSECUTIVE;
	return m_pSearch->FindFirst(wstr, searchFlags, startCharIndex);
}


void FSPDFTextSearchImp::saveState(FS_BOOL bNext)
{
	m_startPageIndex = m_curPageIndex;
	m_bFindNext = bNext;
}

void FSPDFTextSearchImp::clear(FS_BOOL bClearSearch)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_Sentence.isEmpty())
		m_Sentence.clear();
	if (m_pRects && m_pRects->GetSize() > 0) {
		m_pRects->RemoveAll();
	}
	if(bClearSearch){
		SAFE_DELETE(m_pSearch);
//		m_curPageIndex = -1;
	}
}

FS_BOOL FSPDFTextSearchImp::findNext()
{
	FSCRT_LOCKOBJECT(lock, &((FSPDFDocImp*)m_pPDFDoc)->m_lockPages);
	FSCRT_LOCKOBJECT(lock1, &m_lock);
	if (FSCRT_Util_IsStringEmpty(m_Pattern)) return FALSE;
	clear();
	if(m_pSearch && m_pSearch->FindNext()){
		saveState(TRUE);
		return TRUE;
	}
	FS_INT32 endPage =  m_pPDFDoc->getPageCount();
	if( (m_bFindNext == -1 || m_curPageIndex < m_startPageIndex || m_curPageIndex >= endPage))
		m_curPageIndex = m_startPageIndex-1;
	while(m_curPageIndex < endPage){
		m_curPageIndex ++;
		if(m_bFindNext != -1 && m_pause && m_pause->needPauseNow()){
			clear(TRUE);
			m_bFindNext = -1;
			return FALSE;
		}
		if(!initSearch(TRUE))
			continue;
		if(m_pSearch->FindNext()){
			saveState(TRUE);
			return TRUE;
		}
	}
	clear(TRUE);
	m_bFindNext = -1;
	return FALSE;
}

FS_BOOL FSPDFTextSearchImp::findPrev()
{
	FSCRT_LOCKOBJECT(lock, &((FSPDFDocImp*)m_pPDFDoc)->m_lockPages);
	FSCRT_LOCKOBJECT(lock1, &m_lock);
	if (FSCRT_Util_IsStringEmpty(m_Pattern)) return FALSE;
	clear();
	if(m_pSearch && m_pSearch->FindPrev()){
		saveState(FALSE);
		return TRUE;
	}
	if((m_bFindNext == -1 || m_curPageIndex < 0 || m_curPageIndex >= m_pPDFDoc->getPageCount()))
		m_curPageIndex = m_startPageIndex+1;
	while(m_curPageIndex >= 0){
		m_curPageIndex --;
		if(m_bFindNext != -1 && m_pause && m_pause->needPauseNow()){
			clear(TRUE);
			m_bFindNext = -1;
			return FALSE;
		}
		if(!initSearch(FALSE))
			continue;
		if(m_pSearch->FindPrev()){
			saveState(FALSE);
			return TRUE;
		}
	}
	clear(TRUE);
	m_bFindNext = -1;
	return FALSE;
}

FS_INT32 FSPDFTextSearchImp::getMatchRectCount()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_pSearch || m_bFindNext == -1)
		return 0;

	if (!m_pRects)
		m_pRects = FX_NEW CFX_RectArray();
	m_pSearch->GetRectArray(*m_pRects);
	return m_pRects->GetSize();
}

FSRectF FSPDFTextSearchImp::getMatchRect(FS_INT32 index) const
{
	FSRectF retRect;
	retRect.set(0, 0, 0, 0);
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if (!m_pRects || index<0 || index>=m_pRects->GetSize() || m_bFindNext == -1)
		return retRect;
	CFX_FloatRect fxRect = m_pRects->GetAt(index);
	retRect.set(fxRect.left, fxRect.bottom, fxRect.right, fxRect.top);
	return retRect;
}

FS_INT32 FSPDFTextSearchImp::getMatchPageIndex() const
{
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if(m_bFindNext == -1)
		return -1;
	return m_startPageIndex;
}

FSString FSPDFTextSearchImp::getMatchSentence()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_pCurrPDFPage || !m_pCurrTextPage || !m_pSearch || m_bFindNext == -1)
		return FSString();
	if(m_Sentence.isEmpty())
		m_Sentence = _GetMatchSentence(m_pCurrTextPage, m_keyLen, m_pSearch->GetCurOrder(), m_KeyStart);
	return m_Sentence;
}

FS_INT32 FSPDFTextSearchImp::getMatchSentenceStartIndex()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_pCurrPDFPage || !m_pCurrTextPage || !m_pSearch || m_bFindNext == -1)
		return -1;
	if(m_Sentence.isEmpty())
		m_Sentence = _GetMatchSentence(m_pCurrTextPage, m_keyLen, m_pSearch->GetCurOrder(), m_KeyStart);
	return m_KeyStart;
}

FS_INT32 FSPDFTextSearchImp::getMatchStartCharIndex() const
{
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if(!m_pSearch || m_bFindNext == -1)
		return -1;
	return m_pSearch->GetCurOrder();
}

FS_INT32 FSPDFTextSearchImp::getMatchEndCharIndex() const
{
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if(!m_pSearch || m_bFindNext == -1)
		return -1;
	return m_pSearch->GetCurOrder() + m_pSearch->GetMatchedCount() - 1;
}

/////////////////////////////////////////////////////////////////
// FSPDFLinkExtracImp
FSPDFPageLinks* FSPDFPageLinks::create(FSPDFPage* pPage)
{
	if(!pPage || !pPage->isParsed())
		return NULL;
	FSPDFPageLinksImp* pLinkExtract = FX_NEW FSPDFPageLinksImp;
	if(!pLinkExtract)
		return NULL;
	if(pLinkExtract->init(pPage))
		return pLinkExtract;
	delete pLinkExtract;
	return NULL;
}
FSPDFPageLinksImp::FSPDFPageLinksImp()
	:m_pLinkExt(NULL)
	, m_pPage(NULL)
	, m_pTextLinks(NULL)
	, m_nLinkAnnots(-1)
	, m_nTextLinks(0)
{}

void FSPDFPageLinksImp::release()
{
	delete this;
}

FSPDFPageLinksImp::~FSPDFPageLinksImp()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	SAFE_DELETE(m_pLinkExt);
	if(m_pTextLinks){
		delete[]m_pTextLinks;
		m_pTextLinks = NULL;
	}
}

FS_BOOL FSPDFPageLinksImp::init(FSPDFPage* pPage)
{
	FSCRT_LOCKOBJECT(lock1, &m_lock);
	FSPDFPageImp* pPageImp = (FSPDFPageImp*)pPage;
	FSCRT_LOCKOBJECT(lock, &pPageImp->m_lock);
	IPDF_TextPage* pText = pPageImp->getTextPage();
	if(!pText)
		return FALSE;
	m_pPage = pPage;
	m_pLinkExt = IPDF_LinkExtract::CreateLinkExtract();
	if(!m_pLinkExt)
		return FALSE;
	if(m_pLinkExt->ExtractLinks(pText))
	{
		FSCRT_LOCKOBJECT(lock, &m_lock);
		m_nTextLinks = m_pLinkExt->CountLinks();
		if(!m_pTextLinks)
			m_pTextLinks = FX_NEW FSPDFTextLinkImp[m_nTextLinks];
		if(m_pTextLinks){
			for(int i=0; i<m_nTextLinks; i++)
				m_pTextLinks[i].init(m_pLinkExt, i);
			return TRUE;
		}
	}
	SAFE_DELETE(m_pLinkExt);
	m_nTextLinks = 0;
	return TRUE;
}
/** Get the count of links in the page. Return -1 for error. */
FS_INT32 FSPDFPageLinksImp::getTextLinkCount()
{	
	FSCRT_LOCKOBJECT(lock, &m_lock);
	return m_nTextLinks;
}

FSPDFTextLink* FSPDFPageLinksImp::getTextLink(int index)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(index < 0 || !m_pTextLinks || index >= m_nTextLinks)
		return NULL;
	return &(m_pTextLinks[index]);
}

int FSPDFPageLinksImp::getLinkAnnotCount()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FSCRT_LOCKOBJECT(lock1, &((FSPDFPageImp*)m_pPage)->m_lockAnnots);
	if(m_nLinkAnnots != -1)
		return m_nLinkAnnots;
	m_nLinkAnnots = 0;
	FS_INT32 nAnnots = m_pPage->getAnnotCount();
	for(int i=0; i<nAnnots; i++){
		FSAnnot* pAnnot = m_pPage->getAnnot(i);
		if(pAnnot->getType() == FSAnnot::e_annotLink)
			m_nLinkAnnots++;
	}
	return m_nLinkAnnots;
}

FSLink* FSPDFPageLinksImp::getLinkAnnot(int index)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FSCRT_LOCKOBJECT(lock1, &((FSPDFPageImp*)m_pPage)->m_lockAnnots);
	if(index < 0 || index >= m_nLinkAnnots)
		return NULL;
	FS_INT32 nAnnots = m_pPage->getAnnotCount();
	FS_INT32 iLinkAnnots = 0;
	for(int i=0; i<nAnnots; i++){
		FSAnnot* pAnnot = m_pPage->getAnnot(i);
		if(pAnnot->getType() == FSAnnot::e_annotLink && iLinkAnnots++ == index)
			return (FSLink*)pAnnot;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////
// FSPDFTextLinkImp
FSPDFTextLinkImp::FSPDFTextLinkImp()
	: m_pLinkExt(NULL)
	, m_linkIndex(-1)
{
}

void FSPDFTextLinkImp::init(IPDF_LinkExtract* pLinkExtract, FS_INT32 index)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_pLinkExt = pLinkExtract;
	m_linkIndex = index;
}

/** Get the linked URI for specified link. */
FSString FSPDFTextLinkImp::getURI()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFX_WideString wcsText = m_pLinkExt->GetURL(m_linkIndex);
	FSString str = FSString::createFromWideString(wcsText);
	str.toUTF8();
	return str;
}
/** Get the specified link start index in page context.*/
FS_INT32 FSPDFTextLinkImp::getStartCharIndex()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	int start, count;
	m_pLinkExt->GetBoundedSegment(m_linkIndex, start, count);
	return start;
}

/** Get the specified link end index in page context.*/
FS_INT32 FSPDFTextLinkImp::getEndCharIndex()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	int start, count;
	m_pLinkExt->GetBoundedSegment(m_linkIndex, start, count);
	return start + count - 1;
}

/** Get the count of link's rectangle. */
FS_INT32 FSPDFTextLinkImp::getRectCount()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	CFX_RectArray re;
	m_pLinkExt->GetRects(m_linkIndex, re);
	return re.GetSize();
}
/** Get the rectangle array for specified link. */
FSRectF FSPDFTextLinkImp::getRect(FS_INT32 rectIndex)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	FSRectF fsRect;
	fsRect.set(0,0,0,0);
	if(rectIndex < 0)
		return fsRect;
	CFX_RectArray re;
	m_pLinkExt->GetRects(m_linkIndex, re);
	if(rectIndex >= re.GetSize())
		return fsRect;
	CFX_FloatRect fRe = re.GetAt(rectIndex);
	fsRect.set(fRe.left,fRe.bottom, fRe.right, fRe.top);
	return fsRect;
}

