#include "fr_include.h"
#include "../fs_util.h"
#include "fr_ofdpageImp.h"
#include "fr_ofddocImp.h"
#include "fr_ofdpageImp.h"
#include "fr_ofdsearchImp.h"

static void		safeDelete(IOFD_TextPage*& page)
{
	if (page) {
		page->Release();
		page = NULL;
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

static FX_DWORD UnicodeFromCharInfo(OFD_CHAR_INFO& charinfo)
{
	FX_DWORD unicode = 0;
//	if (charinfo.m_Unicode.GetSize() > 0) {
//		unicode = charinfo.m_Unicode[0];
//	}
	unicode = charinfo.m_dwUnicode;
	return unicode;
}


#define FS_PATTERNPOS_MAX	36
#define FS_SENTENCELENGTH_MIN	50
#define FS_SENTENCELENGTH_MAX	70
static FSString _GetMatchSentence(IOFD_TextPage* pTextPage, FS_INT32 keyLen, FS_INT32 pos, FS_INT32& keyStart)
{
	int i, startpos = 0, endpos = pTextPage->CountChars() - 1;
	OFD_CHAR_INFO charinfo;

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


FSPDFTextSearch* createOFDTextSearch(FSPDFDoc* pPDFDoc, FSPause* pause)
{
	return new FR_OFDTextSearchImp(pPDFDoc, pause);
}

//class FR_OFDTextSearchImp : public FSPDFTextSearch, public CFX_Object
FR_OFDTextSearchImp::FR_OFDTextSearchImp(FSPDFDoc* pPDFDoc, FSPause* pause)
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

FR_OFDTextSearchImp::~FR_OFDTextSearchImp()
{

}

#ifndef _SWIG_IOS_
void		FR_OFDTextSearchImp::release()
{

}
#endif

FS_BOOL		FR_OFDTextSearchImp::setKeyWords(const char* keyWords)
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

FS_BOOL		FR_OFDTextSearchImp::setStartPage(FS_INT32 pageIndex)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(pageIndex<0 || pageIndex >= m_pPDFDoc->getPageCount())
		return FALSE;
	m_startPageIndex = pageIndex;
	SAFE_DELETE(m_pSearch);
	m_bFindNext = -1;
	return TRUE;
}

FS_BOOL		FR_OFDTextSearchImp::setFlag(FS_DWORD flag)
{
	if(flag < e_searchNormal || flag > (e_searchNormal|e_searchMatchCase|e_searchMatchWholeWord|e_searchConsecutive)) 
		return FALSE;
	FSCRT_LOCKOBJECT(lock, &m_lock);
	m_searchFlag = flag;
	SAFE_DELETE(m_pSearch);
	m_bFindNext = -1;
	return TRUE;
}

FS_BOOL		FR_OFDTextSearchImp::findNext()
{
	FSCRT_LOCKOBJECT(lock, &((FR_OFDDocImp*)m_pPDFDoc)->m_lockPages);
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

FS_BOOL		FR_OFDTextSearchImp::findPrev()
{
	FSCRT_LOCKOBJECT(lock, &((FR_OFDDocImp*)m_pPDFDoc)->m_lockPages);
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

FS_INT32	FR_OFDTextSearchImp::getMatchRectCount()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_pSearch || m_bFindNext == -1)
		return 0;

	if (!m_pRects)
		m_pRects = FX_NEW CFX_RectArray();
	m_pSearch->GetRectArray(*m_pRects);
	return m_pRects->GetSize();
}

FSRectF		FR_OFDTextSearchImp::getMatchRect(FS_INT32 index) const
{
	FSRectF retRect;
	retRect.set(0, 0, 0, 0);
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if (!m_pRects || index<0 || index>=m_pRects->GetSize() || m_bFindNext == -1)
		return retRect;
	CFX_FloatRect fxRect = m_pRects->GetAt(index);
	CFX_Matrix matrix = ((FR_OFDPageImp*)m_pCurrPDFPage)->getFormatMatrix();
	matrix.TransformRect(fxRect);
	retRect.set(fxRect.left, fxRect.bottom, fxRect.right, fxRect.top);
	return retRect;
}

FS_INT32	FR_OFDTextSearchImp::getMatchPageIndex() const
{
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if(m_bFindNext == -1)
		return -1;
	return m_startPageIndex;
}

FSString FR_OFDTextSearchImp::getMatchSentence()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_pCurrPDFPage || !m_pCurrTextPage || !m_pSearch || m_bFindNext == -1)
		return FSString();
	if(m_Sentence.isEmpty())
		m_Sentence = _GetMatchSentence(m_pCurrTextPage, m_keyLen, m_pSearch->GetCurPos(), m_KeyStart);
	return m_Sentence;
}

FS_INT32 FR_OFDTextSearchImp::getMatchSentenceStartIndex()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if(!m_pCurrPDFPage || !m_pCurrTextPage || !m_pSearch || m_bFindNext == -1)
		return -1;
	if(m_Sentence.isEmpty())
		m_Sentence = _GetMatchSentence(m_pCurrTextPage, m_keyLen, m_pSearch->GetCurPos(), m_KeyStart);
	return m_KeyStart;
}

FS_INT32 FR_OFDTextSearchImp::getMatchStartCharIndex() const
{
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if(!m_pSearch || m_bFindNext == -1)
		return -1;
	return m_pSearch->GetCurPos();
}

FS_INT32 FR_OFDTextSearchImp::getMatchEndCharIndex() const
{
	FSCRT_LOCKOBJECT(lock, (CFSCRT_Lock*)&m_lock);
	if(!m_pSearch || m_bFindNext == -1)
		return -1;
	return m_pSearch->GetCurPos() + m_keyLen - 1;;
}

#define MAX_INT32		0x7fffffff

FS_INT32 FR_OFDTextSearchImp::initSearch(FS_BOOL bNext)
{
	FS_INT32 startCharIndex;
	if (bNext) {
		startCharIndex = 0;
	} else {
		//startCharIndex = - 1;
		startCharIndex = MAX_INT32;
	}

	FSCRT_LOCKOBJECT(lock, &((FR_OFDDocImp*)m_pPDFDoc)->m_lockPages);
	FSCRT_LOCKOBJECT(lock1, &m_lock);
	m_bFindNext = bNext;
	if (m_pCurrPDFPage && m_ParsedPageIndex != m_curPageIndex){
		safeDelete(m_pCurrTextPage);
		//safeDelete(m_pCurrPDFPage);
		m_pPDFDoc->closePage(m_pCurrPDFPage);
		m_pCurrPDFPage = NULL;
	}
	if (!m_pCurrPDFPage || m_ParsedPageIndex != m_curPageIndex){
		m_pCurrPDFPage = m_pPDFDoc->getPage(m_curPageIndex);
		if (!m_pCurrPDFPage) {
			return FALSE;
		}
		IOFD_Page* page = ((FR_OFDPageImp*)m_pCurrPDFPage)->getPage();
		/*IOFD_Document* pPDFDocument = ((FR_OFDDocImp*)m_pPDFDoc)->getOFDDocument();
		if (!pPDFDocument)
			return FALSE;

		m_pCurrPDFPage = pPDFDocument->GetPage(m_curPageIndex);
		if (!m_pCurrPDFPage || !m_pCurrPDFPage->LoadPage()) {
			return FALSE;
		}
		*/
		m_ParsedPageIndex = m_curPageIndex;
		m_pCurrTextPage = IOFD_TextPage::CreateTextPage(page);
		if (!m_pCurrTextPage) {
			//safeDelete(m_pCurrPDFPage);
			m_pPDFDoc->closePage(m_pCurrPDFPage);
			m_pCurrPDFPage = NULL;
			return FALSE;
		}
		//m_pCurrTextPage->ParseTextPage();
	}
	if (m_pRects && m_pRects->GetSize() > 0) {
		m_pRects->RemoveAll();
	}
	SAFE_DELETE(m_pSearch);
	m_pSearch = IOFD_TextPageFind::CreateTextFind(((FR_OFDPageImp*)m_pCurrPDFPage)->getPage());
	if(!m_pSearch)
		return FALSE;

	CFX_WideString wstr = CFX_WideString::FromUTF8(m_Pattern);
	m_keyLen = wstr.GetLength();
	FS_INT32 searchFlags = 0;
	if (m_searchFlag & e_searchMatchCase) {
		searchFlags |= FOFDTEXT_MATCHCASE;
	}
	if (m_searchFlag & e_searchMatchWholeWord) {
		searchFlags |= FOFDTEXT_MATCHWHOLEWORD;
	}
	if (m_searchFlag & e_searchConsecutive) {
		// note by gm, no this flag in ofd
		//searchFlags |= FPDFTEXT_CONSECUTIVE;
	}
	return m_pSearch->FindFirst(wstr, searchFlags, startCharIndex);
}


void FR_OFDTextSearchImp::saveState(FS_BOOL bNext)
{
	m_startPageIndex = m_curPageIndex;
	m_bFindNext = bNext;
}

void FR_OFDTextSearchImp::clear(FS_BOOL bClearSearch)
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


FSPDFTextSelect* createOFDTextSelect(FSPDFPage* pPage)
{
	if(!pPage || !pPage->isParsed())
		return NULL;
	FR_OFDTextSelectImp* pTextPage = FX_NEW FR_OFDTextSelectImp();
	if(pTextPage && pTextPage->init(pPage))
		return pTextPage;
	SAFE_DELETE(pTextPage);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////
// Class FR_OFDTextSelectImp
FR_OFDTextSelectImp::FR_OFDTextSelectImp()
	: m_pFSPDFPage(NULL)
	, m_pTextSelect(NULL)
{

}

void FR_OFDTextSelectImp::release()
{
	delete this;
}

FR_OFDTextSelectImp::~FR_OFDTextSelectImp()
{
	if (m_pTextSelect) {
		m_pTextSelect->Release();
		m_pTextSelect = NULL;
	}
}

FS_BOOL FR_OFDTextSelectImp::init(FSPDFPage* pPage)
{
	if(pPage && ((FR_OFDPageImp*)pPage)->getTextPage()){
		m_pFSPDFPage = pPage;
		IOFD_Page* page = ((FR_OFDPageImp*)m_pFSPDFPage)->getPage();
		m_pTextSelect = IOFD_TextPageSelect::CreateTextSelect(page);
		return TRUE;
	}
	return FALSE;
}

FSPDFPage* FR_OFDTextSelectImp::getPage() const
{
	return m_pFSPDFPage;
}

//Text extraction and selection.
FS_INT32 FR_OFDTextSelectImp::getCharCount() const
{
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage)
		return 0;
	return pTextPage->CountChars();
}

FSString FR_OFDTextSelectImp::getChars(FS_INT32 startIndex, FS_INT32 count) const
{
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || startIndex < 0 || startIndex>=getCharCount() || count < -1)
		return FSString();
	CFX_WideString wcsText = pTextPage->GetPageText(startIndex,count);
	FSString str = FSString::createFromWideString(wcsText);
	str.toUTF8();
	return str;
}

FSString FR_OFDTextSelectImp::getTextInRect(FSRectF rect) const
{
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || FALSE == CHECKVALID_FLOATRECT_PDF(rect))
		return FSString();

	CFX_FloatRect fxRect(rect.left, rect.bottom, rect.right, rect.top);
	CFX_Matrix ofd_2_pdf = ((FR_OFDPageImp*)m_pFSPDFPage)->getFormatMatrix();
	CFX_Matrix pdf_2_ofd;
	pdf_2_ofd.SetReverse(ofd_2_pdf);
	pdf_2_ofd.TransformRect(fxRect);
	CFX_PointF pt;
	pt.Set(fxRect.left, fxRect.top);
	m_pTextSelect->SetStartPoint(pt);
	pt.Set(fxRect.right, fxRect.bottom);
	m_pTextSelect->SetEndPoint(pt);
	m_pTextSelect->SetSelType(OFD_TEXTSELTYPE_RECTBOX);
	CFX_WideString wstr = m_pTextSelect->GetSelectedText();
	FSString str = FSString::createFromWideString(wstr);
	str.toUTF8();
	return str;
}

FS_INT32 FR_OFDTextSelectImp::getIndexAtPos(FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance) const
{
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || !m_pTextSelect || tolerance<0)
		return -1;

	CFX_Matrix matrix = ((FR_OFDPageImp*)m_pFSPDFPage)->getFormatMatrix();
	CFX_Matrix matrix2;
	matrix2.SetReverse(matrix);
	matrix2.Transform(x, y);
	tolerance = matrix2.TransformDistance(tolerance);
	int size = pTextPage->CountChars();
	OFD_CHAR_INFO info;
	for (int i=0; i<size; ++i) {
		if (!pTextPage->GetCharInfo(i, info)) {
			continue;
		}
		CFX_RectF rect = info.m_fCharBox;
		info.m_Matrix.TransformRect(rect);
		if (rect.Contains(x, y)) {
			return i;
		}
	}

	for (int i=0; i<size; ++i) {
		if (!pTextPage->GetCharInfo(i, info)) {
			continue;
		}
		CFX_RectF rect = info.m_fCharBox;

		info.m_Matrix.TransformRect(rect);
		rect.Inflate(tolerance, tolerance);
		if (rect.Contains(x, y)) {
			return i;
		}
	}

	return -1;
//	int ret = pTextPage->GetIndexAtPos(x, y, tolerance, tolerance);
//	return ret < 0 ? -1: ret;
}

FS_BOOL FR_OFDTextSelectImp::getWordAtPos(FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, FS_INT32& startIndex, FS_INT32& count) const
{
	startIndex = 0;
	count = 0;
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	if (!pTextPage || tolerance<0)
		return FALSE;
	// need to transform with matrix
	FS_INT32 charIndex = getIndexAtPos(x, y, tolerance);
	if(charIndex == -1)
		return FALSE;
	OFD_CHAR_INFO charInfo;
	pTextPage->GetCharInfo(charIndex, charInfo);
	if(_IsPunctuationSeparators(UnicodeFromCharInfo(charInfo))){
		startIndex = charIndex;
		count = 1;
		return TRUE;
	}
	for(int i=charIndex-1; i>=0; i--){
		pTextPage->GetCharInfo(i, charInfo);
		if(charInfo.m_dwUnicode == '\r' || charInfo.m_dwUnicode == '\n' || charInfo.m_dwUnicode > 255 || _IsPunctuationSeparators(UnicodeFromCharInfo(charInfo))){
			startIndex = i+1;
			break;
		}
	}
	if(startIndex < 0)
		startIndex = 0;
	FS_INT32 nChars = pTextPage->CountChars();
	for(int i=charIndex+1; i<nChars; i++){
		pTextPage->GetCharInfo(i, charInfo);
		FS_INT32 ch =  charInfo.m_dwUnicode;
		if(ch !=0 && (ch == '\r' || ch == '\n' || ch > 255 || _IsPunctuationSeparators(UnicodeFromCharInfo(charInfo)))){
			count = i - startIndex;
			break;
		}
	}
	return TRUE;
}

FS_INT32 FR_OFDTextSelectImp::getTextRectCount(FS_INT32 start, FS_INT32 nCount)
{
	m_TextRect.RemoveAll();
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	if(!pTextPage || start<0 || start>=getCharCount() || nCount < -1)
		return 0;

	//return pTextPage->CountRects(start, nCount);

	CFX_Matrix ofd_2_pdf = ((FR_OFDPageImp*)m_pFSPDFPage)->getFormatMatrix();
	int charCount = getCharCount();
	for (int i=0; i<nCount;++i)
	{
		if (start+i>=charCount){
			break;
		}

		OFD_CHAR_INFO info;
		pTextPage->GetCharInfo(start+i, info);
		CFX_RectF result = info.m_fCharBox;
		info.m_Matrix.TransformRect(result);
		ofd_2_pdf.TransformRect(result);
		m_TextRect.Add(result);
	}
	//return (start+nCount>charCount) ? charCount-start : nCount;
	return m_TextRect.GetSize();
}

FSRectF FR_OFDTextSelectImp::getTextRect(FS_INT32 rectIndex) const
{
	FSRectF rectF;
	rectF.set(0, 0, 0, 0);
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	// We don't do judgment for the upper limit of rectIndex, because we cannot get rectCount easily here.
	// We cannot store rectCount in FR_OFDTextSelectImp either because pTextPage may be changed when page is re-parsed.
	// The judgment for upper limit of rectIndex will be done by fxcore API IPDF_TextPage::GetRect().
	if(!pTextPage || rectIndex<0)
		return rectF;

	if (rectIndex>=m_TextRect.GetSize()) {
		return rectF;
	}

	CFX_RectF result = m_TextRect.GetAt(rectIndex);
	/*
	FX_FLOAT left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	//pTextPage->GetRect(rectIndex, left, top, right,bottom) ;
	//rectF.set(left, bottom, right, top);
	OFD_CHAR_INFO info;
	pTextPage->GetCharInfo(rectIndex, info);
	CFX_RectF result = info.m_fCharBox;
	info.m_Matrix.TransformRect(result);
	// */
	rectF.set(result.left, result.bottom(), result.right(), result.top);
	return rectF;
}

FS_ROTATION FR_OFDTextSelectImp::getBaselineRotation(FS_INT32 rectIndex)
{
	IOFD_TextPage* pTextPage = ((FR_OFDPageImp*)m_pFSPDFPage)->getTextPage();
	// We don't do judgment for the upper limit of rectIndex, because we cannot get rectCount easily here.
	// We cannot store rectCount in FR_OFDTextSelectImp either because pTextPage may be changed when page is re-parsed.
	// The judgment for upper limit of rectIndex will be done by fxcore API IPDF_TextPage::GetBaselineRotate().
	if(!pTextPage || rectIndex<0)
		return FSCommonDefines::e_rotationUnknown;
	
	return FSCommonDefines::e_rotation0;
	/*
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
	// */
}
