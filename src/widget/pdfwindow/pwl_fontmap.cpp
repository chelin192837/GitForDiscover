//#include "../../../../fs_precompilers.h"
//#if defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)

#include "include/pdfwindow/pdfwindow.h"
#include "include/pdfwindow/pwl_wnd.h"
#include "include/pdfwindow/pwl_fontmap.h"

#if _FXM_PLATFORM_ == _FXM_PLATFORM_ANDROID_
#include <dlfcn.h>
#endif

#define DEFAULT_FONT_NAME			"Helvetica"

/* ------------------------------ CPWL_FontMap ------------------------------ */

CPWL_FontMap::CPWL_FontMap(IFX_SystemHandler* pSystemHandler) : 	
	m_pPDFDoc(NULL),
	m_pSystemHandler(pSystemHandler)
{
	ASSERT(m_pSystemHandler != NULL);
}

CPWL_FontMap::~CPWL_FontMap()
{
	if (m_pPDFDoc)
	{
		delete m_pPDFDoc;
		m_pPDFDoc = NULL;
	}

	Empty();
}

void CPWL_FontMap::SetSystemHandler(IFX_SystemHandler* pSystemHandler)
{
	m_pSystemHandler = pSystemHandler;
}

CPDF_Document* CPWL_FontMap::GetDocument()
{
	if (!m_pPDFDoc)
	{
		if (CPDF_ModuleMgr* pMgr = CPDF_ModuleMgr::Get())
		{
			m_pPDFDoc = FX_NEW CPDF_Document;
			if (!m_pPDFDoc)	return m_pPDFDoc;

			m_pPDFDoc->CreateNewDoc();
		}
	}

	return m_pPDFDoc;
}

CPDF_Font* CPWL_FontMap::GetPDFFont(FX_INT32 nFontIndex)
{
	if (nFontIndex >=0 && nFontIndex < m_aData.GetSize())
	{
		if (CPWL_FontMap_Data* pData = m_aData.GetAt(nFontIndex))
		{
			return pData->pFont;
		}
	}

	return NULL;
}

CFX_ByteString CPWL_FontMap::GetPDFFontAlias(FX_INT32 nFontIndex)
{
	if (nFontIndex >=0 && nFontIndex < m_aData.GetSize())
	{
		if (CPWL_FontMap_Data* pData = m_aData.GetAt(nFontIndex))
		{
			return pData->sFontName;
		}
	}

	return "";
}

FX_BOOL CPWL_FontMap::KnowWord(FX_INT32 nFontIndex, FX_WORD word)
{
	if (nFontIndex >=0 && nFontIndex < m_aData.GetSize())
	{
		if (CPWL_FontMap_Data* pData = m_aData.GetAt(nFontIndex))
		{	
			if(pData->pFont && pData->pFont->IsEmbedded())
				return FALSE;
			return CharCodeFromUnicode(nFontIndex, word) >= 0;
		}
	}

	return FALSE;
}

FX_INT32 CPWL_FontMap::GetWordFontIndex(FX_WORD word, FX_INT32 nCharset, FX_INT32 nFontIndex)
{
	if (nFontIndex > 0)
	{
		if (KnowWord(nFontIndex, word))
			return nFontIndex;
	}
	if (const CPWL_FontMap_Data* pData = GetFontMapData(0))
	{
		if (KnowWord(0, word))
		{
			return 0;
		}
	}

	FX_INT32 nNewFontIndex = -1;

	nNewFontIndex = this->GetFontIndex(GetNativeFontName(nCharset), nCharset, TRUE);
	if (nNewFontIndex >= 0)
	{
		if (KnowWord(nNewFontIndex, word))
			return nNewFontIndex;
	}

	nNewFontIndex = this->GetFontIndex("Arial Unicode MS", DEFAULT_CHARSET, FALSE);
	if (nNewFontIndex >= 0)
	{
		if (KnowWord(nNewFontIndex, word))
		return nNewFontIndex;
	}

	return -1;
}

FX_INT32 CPWL_FontMap::CharCodeFromUnicode(FX_INT32 nFontIndex, FX_WORD word)
{
	if (CPWL_FontMap_Data* pData = m_aData.GetAt(nFontIndex))
	{
		if (pData->pFont)
		{
			if (pData->pFont->IsUnicodeCompatible())
			{
				int nCharCode = pData->pFont->CharCodeFromUnicode(word);
				if(pData->pFont->GlyphFromCharCode(nCharCode) == -1)
					return -1;
				return nCharCode;
			}
			else
			{
				if (word < 0xFF)
					return word;
			}
		}
	}

	return -1;
}

CFX_ByteString CPWL_FontMap::GetNativeFontName(FX_INT32 nCharset)
{
	//searching native font is slow, so we must save time
	for (FX_INT32 i=0,sz=m_aNativeFont.GetSize(); i<sz; i++)
	{
		if (CPWL_FontMap_Native* pData = m_aNativeFont.GetAt(i))
		{
			if (pData->nCharset == nCharset)
				return pData->sFontName;
		}
	}

	CFX_ByteString sNew = GetNativeFont(nCharset);

	if (!sNew.IsEmpty())
	{
		CPWL_FontMap_Native* pNewData = new CPWL_FontMap_Native;
		pNewData->nCharset = nCharset;
		pNewData->sFontName = sNew;

		m_aNativeFont.Add(pNewData);
	}

	return sNew;
}

void CPWL_FontMap::Empty()
{
	{
		for (FX_INT32 i=0, sz=m_aData.GetSize(); i<sz; i++)
			delete m_aData.GetAt(i);

		m_aData.RemoveAll();
	}
	{
		for (FX_INT32 i=0, sz=m_aNativeFont.GetSize(); i<sz; i++)
			delete m_aNativeFont.GetAt(i);

		m_aNativeFont.RemoveAll();
	}
}

void CPWL_FontMap::Initial(FX_LPCSTR fontname)
{
	CFX_ByteString sFontName = fontname;

	if (sFontName.IsEmpty())
		sFontName = DEFAULT_FONT_NAME;

	GetFontIndex(sFontName, ANSI_CHARSET, FALSE);

	//GetFontIndex(this->GetNativeFontName(nCharset), nCharset);
}
	

/*
List of currently supported standard fonts:
Courier, Courier-Bold, Courier-BoldOblique, Courier-Oblique
Helvetica, Helvetica-Bold, Helvetica-BoldOblique, Helvetica-Oblique
Times-Roman, Times-Bold, Times-Italic, Times-BoldItalic
Symbol, ZapfDingbats
*/

const char* g_sDEStandardFontName[] = {"Courier", "Courier-Bold", "Courier-BoldOblique", "Courier-Oblique",
	"Helvetica", "Helvetica-Bold", "Helvetica-BoldOblique", "Helvetica-Oblique",
	"Times-Roman", "Times-Bold", "Times-Italic", "Times-BoldItalic",
	"Symbol", "ZapfDingbats"};

FX_BOOL	CPWL_FontMap::IsStandardFont(const CFX_ByteString& sFontName)
{
	for (FX_INT32 i=0; i<14; i++)
	{
		if (sFontName == g_sDEStandardFontName[i])
			return TRUE;
	}

	return FALSE;
}

FX_INT32 CPWL_FontMap::FindFont(const CFX_ByteString& sFontName, FX_INT32 nCharset)
{
	for (FX_INT32 i=0,sz=m_aData.GetSize(); i<sz; i++)
	{
		if (CPWL_FontMap_Data* pData = m_aData.GetAt(i))
		{
			if (nCharset == DEFAULT_CHARSET || nCharset == pData->nCharset)
			{
				if (sFontName.IsEmpty() || pData->sFontName == sFontName)
					return i;
			}
		}
	}

	return -1;
}

FX_INT32 CPWL_FontMap::GetFontIndex(const CFX_ByteString& sFontName, FX_INT32 nCharset, FX_BOOL bFind)
{
	FX_INT32 nFontIndex = FindFont(EncodeFontAlias(sFontName, nCharset), nCharset);
	if (nFontIndex >= 0) return nFontIndex;

//	nFontIndex = FindFont("", nCharset);
//	if (nFontIndex >= 0) return nFontIndex;

	CFX_ByteString sAlias;
	CPDF_Font* pFont = NULL;

	if (bFind)
		pFont = FindFontSameCharset(sAlias, nCharset);

	if (!pFont)
	{
		CFX_ByteString sTemp = sFontName;
		pFont = AddFontToDocument(GetDocument(), sTemp, nCharset);

		/*
		if (FindFont(sAlias))
		{
			sAlias = EncodeFontAlias(sTemp, nCharset);
		}
		else
		*/
		{
			sAlias = EncodeFontAlias(sTemp, nCharset);
		}		
	}

	AddedFont(pFont, sAlias);

	return AddFontData(pFont, sAlias, nCharset);
}

FX_INT32 CPWL_FontMap::GetPWLFontIndex(FX_WORD word, FX_INT32 nCharset)
{
	FX_INT32 nFind = -1;

	for (FX_INT32 i=0,sz=m_aData.GetSize(); i<sz; i++)
	{
		if (CPWL_FontMap_Data* pData = m_aData.GetAt(i))
		{
			if (pData->nCharset == nCharset)
			{
				nFind = i;
				break;
			}
		}
	}

	CPDF_Font* pNewFont = GetPDFFont(nFind);

	if (!pNewFont) return -1;

	/*
	if (CPDF_Font* pFont = GetPDFFont(nFind))
	{
		PWLFont.AddWordToFontDict(pFontDict, word);
	}
	*/

// #ifdef FOXIT_CHROME_BUILD
//   CFX_ByteString sAlias = EncodeFontAlias("Arial_Chrome", nCharset);
// #else
	CFX_ByteString sAlias = EncodeFontAlias("Arial_Foxit", nCharset);
//#endif
	AddedFont(pNewFont, sAlias);

	return AddFontData(pNewFont, sAlias, nCharset);
}

CPDF_Font* CPWL_FontMap::FindFontSameCharset(CFX_ByteString& sFontAlias, FX_INT32 nCharset)
{
	return NULL;
}

FX_INT32 CPWL_FontMap::AddFontData(CPDF_Font* pFont, const CFX_ByteString& sFontAlias, FX_INT32 nCharset)
{
	CPWL_FontMap_Data* pNewData = new CPWL_FontMap_Data;
	pNewData->pFont = pFont;
	pNewData->sFontName = sFontAlias;
	pNewData->nCharset = nCharset;

	m_aData.Add(pNewData);

	return m_aData.GetSize() -1;
}

void CPWL_FontMap::AddedFont(CPDF_Font* pFont, CFX_ByteString& sFontAlias)
{
}

CFX_ByteString CPWL_FontMap::GetFontName(FX_INT32 nFontIndex)
{
	if (nFontIndex >=0 && nFontIndex < m_aData.GetSize())
	{
		if (CPWL_FontMap_Data* pData = m_aData.GetAt(nFontIndex))
		{
			return pData->sFontName;
		}
	}

	return "";
}

CFX_ByteString CPWL_FontMap::GetNativeFont(FX_INT32 nCharset)
{
	CFX_ByteString sFontName;

	if (nCharset == DEFAULT_CHARSET)
		nCharset = GetNativeCharset();

	sFontName = GetDefaultFontByCharset(nCharset);

	if (m_pSystemHandler)
	{
		if (m_pSystemHandler->FindNativeTrueTypeFont(nCharset, sFontName))
			return sFontName;

		sFontName = m_pSystemHandler->GetNativeTrueTypeFont(nCharset);
	}

	return sFontName;
}

CPDF_Font* CPWL_FontMap::AddFontToDocument(CPDF_Document* pDoc, CFX_ByteString& sFontName, FX_BYTE nCharset)
{
	if (IsStandardFont(sFontName))
		return AddStandardFont(pDoc, sFontName);
	else
		return AddSystemFont(pDoc, sFontName, nCharset);
}

CPDF_Font* CPWL_FontMap::AddStandardFont(CPDF_Document* pDoc, CFX_ByteString& sFontName)
{
	if (!pDoc) return NULL;

	CPDF_Font* pFont = NULL;

	if (sFontName == "ZapfDingbats")
		pFont = pDoc->AddStandardFont(sFontName, NULL);
	else
	{
		CPDF_FontEncoding fe(PDFFONT_ENCODING_WINANSI);
		pFont = pDoc->AddStandardFont(sFontName, &fe);
	}

	return pFont;
}

CPDF_Font* CPWL_FontMap::AddSystemFont(CPDF_Document* pDoc, CFX_ByteString& sFontName, FX_BYTE nCharset)
{
	if (!pDoc) return NULL;

	if (sFontName.IsEmpty()) sFontName = GetNativeFont(nCharset);
	if (nCharset == DEFAULT_CHARSET) nCharset = GetNativeCharset();

	if (m_pSystemHandler)
		return m_pSystemHandler->AddNativeTrueTypeFontToPDF(pDoc, sFontName, nCharset);
	
	return NULL;
}

CFX_ByteString CPWL_FontMap::EncodeFontAlias(const CFX_ByteString& sFontName, FX_INT32 nCharset)
{
	CFX_ByteString sPostfix;
	sPostfix.Format("_%02X", nCharset);
	return EncodeFontAlias(sFontName) + sPostfix;
}

CFX_ByteString CPWL_FontMap::EncodeFontAlias(const CFX_ByteString& sFontName)
{
	CFX_ByteString sRet = sFontName;
	sRet.Remove(' ');
	return sRet;
}

FX_INT32 CPWL_FontMap::GetFontMapCount() const
{
	return m_aData.GetSize();
}

const CPWL_FontMap_Data* CPWL_FontMap::GetFontMapData(FX_INT32 nIndex) const
{
	if (nIndex >=0 && nIndex < m_aData.GetSize())
	{
		return m_aData.GetAt(nIndex);
	}

	return NULL;
}

FX_INT32 CPWL_FontMap::GetNativeCharset()
{
	FX_BYTE nCharset = ANSI_CHARSET;
	FX_INT32 iCodePage = FXSYS_GetACP();
	switch (iCodePage)
	{
	case 932://Japan
		nCharset = SHIFTJIS_CHARSET;
		break;
	case 936://Chinese (PRC, Singapore)
		nCharset = GB2312_CHARSET;
		break;
	case 950://Chinese (Taiwan; Hong Kong SAR, PRC)
		nCharset = CHINESEBIG5_CHARSET;
		break;
	case 1252://Windows 3.1 Latin 1 (US, Western Europe)
		nCharset = ANSI_CHARSET;
		break;
	case 874://Thai
		nCharset = THAI_CHARSET;
		break;
	case 949://Korean
		nCharset = HANGUL_CHARSET;
		break;
	case 1200://Unicode (BMP of ISO 10646)
		nCharset = ANSI_CHARSET;
		break;
	case 1250://Windows 3.1 Eastern European
		nCharset = EASTEUROPE_CHARSET;
		break;
	case 1251://Windows 3.1 Cyrillic
		nCharset = RUSSIAN_CHARSET;
		break;
	case 1253://Windows 3.1 Greek
		nCharset = GREEK_CHARSET;
		break;
	case 1254://Windows 3.1 Turkish
		nCharset = TURKISH_CHARSET;
		break;
	case 1255://Hebrew
		nCharset = HEBREW_CHARSET;
		break;
	case 1256://Arabic
		nCharset = ARABIC_CHARSET;
		break;
	case 1257://Baltic
		nCharset = BALTIC_CHARSET;
		break;
	case 1258://Vietnamese
		nCharset = VIETNAMESE_CHARSET;
		break;
	case 1361://Korean(Johab)
		nCharset = JOHAB_CHARSET;
		break;
	}
	return nCharset;
}

CFX_ByteString CPWL_FontMap::GetDefaultFontByCharset(FX_INT32 nCharset)
{
	switch (nCharset)
	{
	case ANSI_CHARSET:
		return "Helvetica";
	case GB2312_CHARSET:
#if _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_
		return "STHeitiSC-Light";
#else
		return "SimSun";
#endif
	case CHINESEBIG5_CHARSET:
		return "MingLiU";
	case SHIFTJIS_CHARSET:
#if _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_
		return "AppleGothic";
#else
		return "MS Gothic";
#endif
#if _FX_OS_ == _FX_MACOSX_ || _FX_OS_ == _FX_IOS_
    case HANGUL_CHARSET:
        return "AppleGothic";
#else
	case HANGUL_CHARSET:
		return "Batang";
#endif
	case RUSSIAN_CHARSET:
		return "Arial";
	case EASTEUROPE_CHARSET:
		return "Tahoma";
	case ARABIC_CHARSET:
	case HEBREW_CHARSET:
	case GREEK_CHARSET:
		return "Arial";
    case THAI_CHARSET:
        return "Thonburi";
	}

	return "Arial";
}

FS_INT32 getACP()
{
#if _FXM_PLATFORM_ == _FXM_PLATFORM_WINDOWS_
	return FXSYS_GetACP();
#elif _FXM_PLATFORM_ == _FXM_PLATFORM_ANDROID_
	char lang[512] = "";
	char country[512] = "";

	typedef int (*CAC_FUNC)(const char *name, char *value); 
	CAC_FUNC cac_func = NULL;

	void*  lib =dlopen("libc.so", RTLD_NOW | RTLD_GLOBAL);
	if (lib == NULL)
		return 0;

	*(void **) (&cac_func) = dlsym(lib, "__system_property_get");
	if (cac_func == NULL)
	{
		dlclose(lib);
		return 0;
	}

	cac_func("persist.sys.language", lang);
	cac_func("persist.sys.country", country);
	dlclose(lib);

	if (strncmp(lang, "zh",2) == 0)
	{
		if (strncmp(country, "TW",2) == 0 || strncmp(country, "HK",2) == 0 )
			return 950;
		return 936;
	}
	if (strcmp(lang, "ko") == 0)
		return 949;
	if (strcmp(lang, "ja") == 0)
		return 932;
	return 0;
#elif _FXM_PLATFORM_ == _FXM_PLATFORM_LINUX_
	char *lang = getenv("LANG");
	if (strncmp(lang, "zh_HK",5) == 0 || strncmp(lang, "zh_TW", 5) == 0)
		return 950;
	if (strncmp(lang, "zh_CN", 5) == 0)
		return 936;
	if (strncmp(lang, "ja_JP", 5) == 0)
		return 932;
	if (strncmp(lang, "ko_KR", 5) == 0)
		return 949;
#elif _FXM_PLATFORM_ == _FXM_PLATFORM_APPLE_
	CFArrayRef langs = CFLocaleCopyPreferredLanguages();
	CFStringRef langCode = (CFStringRef)CFArrayGetValueAtIndex(langs, 0);
	// zh-Hant traditional chinese: 950
	// zh-Hans simplize chinese: 936
	// ja japanese: 932
	// ko korea: 949
	CFStringRef targets[4] = {CFSTR("zh-Hant"),CFSTR("zh-Hans"),CFSTR("ja"),CFSTR("ko")};
	for (CFIndex i = 0 ; i < CFArrayGetCount(langs); i++)
	{
		CFStringRef lang = (CFStringRef) CFArrayGetValueAtIndex(langs, i);
		for (CFIndex j = 0; j<4; j++)
		{
			CFComparisonResult result = CFStringCompare(lang, targets[j], kCFCompareCaseInsensitive);
			if (result == kCFCompareEqualTo) {
				switch(j)
				{
				case 0:
					return 950;
				case 1:
					return 936;
				case 2:
					return 932;
				case 3:
					return 949;
				}
			}
		}
	}
#endif
	return 0;
}

FX_INT32 CPWL_FontMap::CharSetFromUnicode(FX_WORD word, FX_INT32 nOldCharset)
{
	//to avoid CJK Font to show ASCII
	if (word < 0x7F) return ANSI_CHARSET;

	int nACP = getACP();
	switch (nACP)
	{
	case 932:
	case 936:
	case 950:
	case 949:
		if ((word >= 0x2E80 && word <= 0x2EFF) ||
			(word >= 0x3000 && word <= 0x303F) ||
			(word >= 0x3200 && word <= 0x32FF) ||
			(word >= 0x3300 && word <= 0x33FF) ||
			(word >= 0x3400 && word <= 0x4DB5) ||
			(word >= 0x4E00 && word <= 0x9FFF) ||
			(word >= 0xF900 && word <= 0xFAFF) ||
			(word >= 0xFE30 && word <= 0xFE4F) ||
			(word >= 0x20000 && word <= 0x2A6D6) ||
			(word >= 0x2F800 && word <= 0x2FA1F)||
			(word >= 0xFF00 && word <= 0xFF5E))
		{
			switch (nACP)
			{
			case 932:
				return SHIFTJIS_CHARSET;
			case 936:
				return GB2312_CHARSET;
			case 950:
				return CHINESEBIG5_CHARSET;
			case 949:
				return HANGUL_CHARSET;
			}
		}
		break;
	}

	//find new charset
	if (word == 0x20A9)
		return HANGUL_CHARSET;
	if ((word >= 0x4E00 && word <= 0x9FA5) || 
		(word >= 0xE7C7 && word <= 0xE7F3) ||
		(word >= 0x3000 && word <= 0x303F) ||
		(word >= 0x2000 && word <= 0x206F) ||
		(word >= 0xff01 && word <= 0xff5e)		// full-width characters, see:http://baike.baidu.com/link?url=534iEzWOdyEm3zllzezoi38p3_yO-NXS9h2Df_EgKLKe8a3jzqYjzssv1ae5twrHLbMf_oNm9MuUeWe7Zhh_wq
		)
	{
		return GB2312_CHARSET;
	}

	if (((word >= 0x3040) && (word <= 0x309F)) ||
		((word >= 0x30A0) && (word <= 0x30FF)) ||
		((word >= 0x31F0) && (word <= 0x31FF)) ||
		((word >= 0xFF5F) && (word <= 0xFFEF)) )
	{
		return SHIFTJIS_CHARSET;
	}

	if (((word >= 0xAC00) && (word <= 0xD7AF)) ||
		((word >= 0x1100) && (word <= 0x11FF)) ||
		((word >= 0x3130) && (word <= 0x318F)))
	{
		return HANGUL_CHARSET;
	}

	if (word >= 0x0E00 && word <= 0x0E7F)
		return THAI_CHARSET;

	if ((word >= 0x0370 && word <= 0x03FF) ||
		(word >= 0x1F00 && word <= 0x1FFF))
		return GREEK_CHARSET;

	if ((word >= 0x0600 && word <= 0x06FF) ||
		(word >= 0xFB50 && word <= 0xFEFC))
		return ARABIC_CHARSET;

	if (word >= 0x0590 && word <= 0x05FF)
		return HEBREW_CHARSET;

	if (word >= 0x0400 && word <= 0x04FF)
		return RUSSIAN_CHARSET;

	if (word == 0x11E || word == 0x11F || word == 0x130 || word == 0x131 || word == 0x15E || word == 0x15F)
		return TURKISH_CHARSET;

	if (word >= 0x0100 && word <= 0x024F)
		return EASTEUROPE_CHARSET;

	if (word >= 0x1E00 && word <= 0x1EFF)
		return VIETNAMESE_CHARSET;

	return DEFAULT_CHARSET;
}

/* ------------------------ CPWL_DocFontMap ------------------------ */

CPWL_DocFontMap::CPWL_DocFontMap(IFX_SystemHandler* pSystemHandler, CPDF_Document* pAttachedDoc) 
	: CPWL_FontMap(pSystemHandler),
	m_pAttachedDoc(pAttachedDoc)
{
}

CPWL_DocFontMap::~CPWL_DocFontMap()
{
}

CPDF_Document* CPWL_DocFontMap::GetDocument()
{
	return m_pAttachedDoc;
}

//#endif //defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)
