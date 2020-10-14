#include "../../include/common.h"
#include "../../include/fs_pdfdoc.h"
#include "../../include/fs_pdfpage.h"
#include "../../include/fs_annot.h"
#include "../fxcore.h"
#include "../fs_util.h"
#include "../inc/fs_pdfdocImp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../annotation/fs_annot_util.h"
#include "inc/fs_watermark.h"
#include "inc/fs_wminfo.h"

///////////////////////////////////////////////////
// Class CFSPDF_Watermark
CFSPDF_Watermark::CFSPDF_Watermark(FSPDFDoc* document)
{
	m_iWatermarkType = FSPDF_WATERMARK_TYPE_INVALID;
	m_pWatermarkParam = NULL;
	m_pWatermarkInfo = NULL;
	m_pFSPDFDoc = document;
}

CFSPDF_Watermark::~CFSPDF_Watermark()
{
	Release();
}

FS_RESULT CFSPDF_Watermark::Release()
{
	if (m_pWatermarkInfo)
	{
		delete m_pWatermarkInfo;
		m_pWatermarkInfo = NULL;
	}
	if (m_pWatermarkParam)
	{
		delete m_pWatermarkParam;
		m_pWatermarkParam = NULL;
	}
	m_iWatermarkType = FSPDF_WATERMARK_TYPE_INVALID;

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSPDF_Watermark::CreateFromText(CFX_ByteString text, const FSPDF_WATERMARK_TEXTPROPERTIES* properties, FS_BOOL bSDKWatermark/* = FALSE*/)
{
	if (text.GetLength()<1) return FSCRT_ERRCODE_PARAM;
	//check parameters
	if (NULL == properties || properties->alignment < FSPDF_WATERMARK_TEXTALIGNMENT_LEFT || properties->alignment > FSPDF_WATERMARK_TEXTALIGNMENT_RIGHT
		|| NULL == properties->pPDFFont || properties->fontSize <= 0 || properties->fontStyle< FSPDF_WATERMARK_FONTSTYLE_NORMAL
		|| properties->fontStyle > FSPDF_WATERMARK_FONTSTYLE_UNDERLINE || properties->lineSpace <= 0)
	{
		return FSCRT_ERRCODE_PARAM;
	}
	if (!m_pFSPDFDoc) return FSCRT_ERRCODE_ERROR;
	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
	//release
	Release();

	//back up for rescue
	FSPDF_WATERMARK_TEXT_PARAM* pParam = new FSPDF_WATERMARK_TEXT_PARAM;//FX_Alloc(FSPDF_WATERMARK_TEXT_PARAM, 1); // FX_Alloc for FSPDF_WATERMARK_TEXT_PARAM will cause crash in ios. Need to be adjusted later.

	if (!pParam)
		return FSCRT_ERRCODE_OUTOFMEMORY;
	pParam->properties = *properties;

	//analyze each character and try to use the most suitable font for them.
	//Here, convert UTF-8 to UTF-16LE in all platform and will deal with 4-byte unicode(surrogate character).
	{
		//CFSCRT_LTFont* pDefaultFont = (CFSCRT_LTFont*)properties->pFont;
		//FSCRT_BSTR strDefaultFontName;
		//FSCRT_BStr_Init(&strDefaultFontName);
		//pDefaultFont->GetName(&strDefaultFontName);

		//FS_DWORD dwDefaultFontStyle;
		//pDefaultFont->GetStyle(&dwDefaultFontStyle);

		//CFSCRT_LTAllcator* pAllocator = FSCRT_GetLTAllocator();
		//if(!pAllocator)
		//{
		//	FSCRT_BStr_Clear(&strDefaultFontName);
		//	FSCRT_LTFree(pParam);
		//	return FSCRT_ERRCODE_OUTOFMEMORY;
		//}

		//CFSCRT_LTFontMgr* pFontMgr = FSCRT_GetLTEnvironment()->GetLTFontMgr();
		//if(!pFontMgr)
		//{
		//	FSCRT_BStr_Clear(&strDefaultFontName);
		//	FSCRT_LTFree(pParam);
		//	return FSCRT_ERRCODE_OUTOFMEMORY;
		//}

		//pParam->fontarray = FX_NewAtAllocator(pAllocator) FSPDF_WATERMARK_TEXTPIECE_FONTARRAY(pAllocator);
		pParam->wText = CFX_WideString::FromUTF8(text.GetBuffer(text.GetLength()), text.GetLength());
		//FS_DWORD nStartPos = -1;
		//CFSCRT_LTFont* pSegFont = NULL;
		//FS_INT32 iCurEmbType = FSPDF_WMCHAREMBTYPE_NONEED;
		//FS_INT32 iEmbType = FSPDF_WMCHAREMBTYPE_NONEED;
		//// 2015-11-12 by Yvonne : Adjust code to judge if a char needs to be embedded and what the embed type is.
		//for(FS_DWORD charIdx = 0; charIdx < pParam->dwUTF16LELen; charIdx ++)
		//{
		//	iCurEmbType = FSPDF_WMCHAREMBTYPE_NONEED;
		//	FS_WORD charItem = pParam->pUTF16LEWideString[charIdx];
		//	CFSCRT_LTFont* curCharFont = NULL;
		//	if (charIdx+1<pParam->dwUTF16LELen)
		//		iCurEmbType = ST_NOJMP_CharEmbedType(charItem, pParam->pUTF16LEWideString[charIdx+1]);
		//	if (FSPDF_WMCHAREMBTYPE_SURROGATE == iCurEmbType) //Currently, for a surrogate char(4-byte) it needs to be embedded with pDefaultFont.
		//		curCharFont = pDefaultFont->Retain();
		//	else
		//		pDefaultFont->GetLogicFont(charItem, &curCharFont);
		//	if (NULL == curCharFont)
		//		continue;
		//	if (nStartPos == -1 || curCharFont != pSegFont || iCurEmbType != iEmbType)
		//	{
		//		if(nStartPos != -1)
		//		{
		//			FSPDF_WATERMARK_TEXTPIECE_FONTINFO newInfo;
		//			newInfo.nStartPos = nStartPos; newInfo.nEndPos = charIdx; newInfo.pFont = pSegFont;
		//			newInfo.iEmbType = iEmbType;
		//			pParam->fontarray->Add(newInfo);
		//		}
		//		nStartPos = charIdx;
		//		pSegFont = curCharFont;
		//		iEmbType = iCurEmbType;
		//	}
		//	else
		//	{
		//		if(curCharFont) 
		//			curCharFont->Release();
		//	}
		//	if (FSPDF_WMCHAREMBTYPE_SURROGATE == iCurEmbType)//A surrogate char(4-byte) contains current char value and the next one, so just skip the next one.
		//		charIdx++;

		//}
		//if(nStartPos != -1)
		//{
		//	FSPDF_WATERMARK_TEXTPIECE_FONTINFO newInfo;
		//	newInfo.nStartPos = nStartPos; newInfo.nEndPos = pParam->dwUTF16LELen; newInfo.pFont = pSegFont;
		//	newInfo.iEmbType = iEmbType;
		//	pParam->fontarray->Add(newInfo);
		//}
		//FSCRT_BStr_Clear(&strDefaultFontName);
	}
	m_pWatermarkParam = pParam;

	// into the darkness
	//ret = FSCRT_ERRCODE_SUCCESS;
	//{
	//	CFSCRT_LTPDFDocument* pFSLTDoc = (CFSCRT_LTPDFDocument*)m_pFSPDFDoc;
	//	for(int i = 0, count = pParam->fontarray->GetSize(); i < count; i ++)
	//	{
	//		FSPDF_WATERMARK_TEXTPIECE_FONTINFO fontInfo = pParam->fontarray->GetAt(i);
	//		CFSCRT_LTFont* pFont = fontInfo.pFont;
	//		FSCRT_CheckObject(pFont);
	//		CFSCRT_LTPDFFonts* pdfFonts = pFSLTDoc->GetPDFFonts();
	//		FS_RESULT ret = pdfFonts->IsPDFFontAvailable(*pFont);
	//		if ( FSCRT_ERRCODE_NOTFOUND == ret)
	//			ret = pdfFonts->ST_AddPDFFont(*pFont); //should we rollback?
	//		else if (ret == FSCRT_ERRCODE_ERROR)
	//			ret = pdfFonts->ST_RecoverPDFFont(*pFont);
	//		if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_NOTFOUND)
	//				return ret;
	//	}
	//}
	if (ret == ST_CreateFromText(pParam->wText, /*((FSPDF_WATERMARK_TEXT_PARAM*)pParam)->fontarray,*/ properties, bSDKWatermark))
	{
		m_iWatermarkType  = FSPDF_WATERMARK_TYPE_TEXT;
	}
	return ret;
}

//FS_RESULT CFSPDF_Watermark::CreateFromImage(FSCRT_IMAGE image)
//{
//	if (!image) return FSCRT_ERRCODE_PARAM;
//
//	//release
//	Release();
//
//	//back up for rescue
//	FSPDF_WATERMARK_IMAGE_PARAM* pParam = (FSPDF_WATERMARK_IMAGE_PARAM*)FSCRT_LTAlloc(sizeof(FSPDF_WATERMARK_IMAGE_PARAM));
//	if (!pParam)
//		return FSCRT_ERRCODE_OUTOFMEMORY;
//	pParam->image = image;
//	m_pWatermarkParam = pParam;
//
//	//into the darkness
//	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
//	FSCRT_StartOOM();
//	FSCRT_StartSTMemory();
//	FSCRT_CheckObject(m_pParent);
//	FSCRT_CheckObject((CFSCRT_LTImage*)image);
//
//	FSCRT_LOCK(m_lock);
//	ret = ST_CreateFromImage(image);
//	if (ret == FSCRT_ERRCODE_SUCCESS) EnableAvailable();
//	FSCRT_UNLOCK(m_lock);
//	if (ret == FSCRT_ERRCODE_SUCCESS)
//	{
//		if(m_pParent) ret = ((CFSCRT_LTPDFDocument*)m_pParent)->AddRecoverObj(this);
//		if (FSCRT_ERRCODE_SUCCESS != ret) {
//			FSCRT_EndSTMemory();
//			return ret;
//		}
//
//		m_iWatermarkType  = FSPDF_WATERMARK_TYPE_IMAGE;
//	}
//	FSCRT_EndSTMemory();
//
//	if (ret != FSCRT_ERRCODE_ST_OOM) return ret;
//    Clear();
//	FSCRT_Recover(ret, m_pParent);
//	FSCRT_EndOOM();
//	
//	FSCRT_ReturnValue(FSCRT_ERRCODE_UNRECOVERABLE);
//}
//
//FS_RESULT CFSPDF_Watermark::CreateFromBitmap(FSCRT_BITMAP bitmap)
//{
//	if (!bitmap) return FSCRT_ERRCODE_PARAM;
//
//	//release
//	Release();
//
//	//back up for rescue
//	FSPDF_WATERMARK_BITMAP_PARAM* pParam = (FSPDF_WATERMARK_BITMAP_PARAM*)FSCRT_LTAlloc(sizeof(FSPDF_WATERMARK_BITMAP_PARAM));
//	if (!pParam)
//		return FSCRT_ERRCODE_OUTOFMEMORY;
//	pParam->bitmap = bitmap;
//	m_pWatermarkParam = pParam;
//
//	//into the darkness
//	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
//	FSCRT_StartOOM();
//	FSCRT_StartSTMemory();
//	FSCRT_CheckObject(m_pParent);
//	FSCRT_CheckObject(((CFSCRT_LTDIBitmap*)bitmap));
//
//	FSCRT_LOCK(m_lock);
//	ret = ST_CreateFromBitmap(bitmap);
//	if (ret == FSCRT_ERRCODE_SUCCESS) EnableAvailable();
//	FSCRT_UNLOCK(m_lock);
//	if (ret == FSCRT_ERRCODE_SUCCESS)
//	{
//		if(m_pParent) ret = ((CFSCRT_LTPDFDocument*)m_pParent)->AddRecoverObj(this);
//		if (FSCRT_ERRCODE_SUCCESS != ret) {
//			FSCRT_EndSTMemory();
//			return ret;
//		}
//		m_iWatermarkType  = FSPDF_WATERMARK_TYPE_BITMAP;
//	}
//	FSCRT_EndSTMemory();
//
//	if (ret != FSCRT_ERRCODE_ST_OOM) return ret;
//    Clear();
//	FSCRT_Recover(ret, m_pParent);
//	FSCRT_EndOOM();
//	
//	FSCRT_ReturnValue(FSCRT_ERRCODE_UNRECOVERABLE);
//}
//
//FS_RESULT CFSPDF_Watermark::CreateFromPage(FSCRT_PAGE page)
//{
//	CHECK_PDFPAGE(page)
//
//	FS_BOOL parsed = FALSE;
//	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
//	ret = ((CFSCRT_LTPDFPage*)page)->IsParsed(&parsed);
//	if (ret != FSCRT_ERRCODE_SUCCESS)
//		return ret;
//	if (!parsed)
//		return FSCRT_ERRCODE_NOTPARSED;
//	//release
//	Release();
//
//	//back up for rescue
//	FSPDF_WATERMARK_PAGE_PARAM* pParam = (FSPDF_WATERMARK_PAGE_PARAM*)FSCRT_LTAlloc(sizeof(FSPDF_WATERMARK_PAGE_PARAM));
//	if (!pParam)
//		return FSCRT_ERRCODE_OUTOFMEMORY;
//	pParam->page = page;
//	m_pWatermarkParam = pParam;
//
//	//into the darkness
//	FSCRT_StartOOM();
//	FSCRT_StartSTMemory();
//	FSCRT_CheckObject(m_pParent);
//	if (ret == FSCRT_ERRCODE_SUCCESS) 
//	{
//		if (!((CFSCRT_LTPDFPage*)page)->IsAvailable())
//		{
//			((CFSCRT_LTPDFPage*)page)->NeedRecoverStartParse();
//			((CFSCRT_LTPDFPage*)page)->NeedRecoverContinueParse();
//			FSCRT_RecoverObject(ret, (CFSCRT_LTPDFPage*)page);
//		}
//	}
//
//	FSCRT_LOCK(m_lock);
//	ret = ST_CreateFromPage(page);
//	if (ret == FSCRT_ERRCODE_SUCCESS) EnableAvailable();
//	FSCRT_UNLOCK(m_lock);
//	if (ret == FSCRT_ERRCODE_SUCCESS)
//	{
//		if(m_pParent) ret = ((CFSCRT_LTPDFDocument*)m_pParent)->AddRecoverObj(this);
//		if (FSCRT_ERRCODE_SUCCESS != ret) {
//			FSCRT_EndSTMemory();
//			return ret;
//		}
//		m_iWatermarkType = FSPDF_WATERMARK_TYPE_PAGE;
//	}
//	FSCRT_EndSTMemory();
//
//	if (ret != FSCRT_ERRCODE_ST_OOM) return ret;
//    Clear();
//	FSCRT_Recover(ret, m_pParent);
//	FSCRT_EndOOM();
//	
//	FSCRT_ReturnValue(FSCRT_ERRCODE_UNRECOVERABLE);
//}

//FSPDFPage*	CFSPDF_Watermark::GetPDFPage()
//{
//	if(GetType() != FSPDF_WATERMARK_TYPE_PAGE)
//		return NULL;
//	FSPDF_WATERMARK_PAGE_PARAM* pParam = (FSPDF_WATERMARK_PAGE_PARAM*)m_pWatermarkParam;
//	if(!pParam)
//		return NULL;
//	return (CFSCRT_LTPDFPage *)(pParam->page);
//}

FS_RESULT CFSPDF_Watermark::InsertToPage(FSPDFPage* page, FS_BOOL bSDKWatermark/* = FASLE*/)
{
	if (!page) 
		return FSCRT_ERRCODE_PARAM;
	if (FALSE == bSDKWatermark && FALSE == page->isParsed())
		return FSCRT_ERRCODE_NOTPARSED;
	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;

	/*CFX_ByteString xmlData;
	ret = RegenerateSettingsXML(xmlData);
	if (!FSCRT_SUCCEEDED(ret)) return ret;
	m_pWatermarkInfo->UpdateSettingsXML(xmlData);*/


	//CFSCRT_LTPDFAnnot* pAnnot = NULL;
	CPDF_Dictionary* pWMAnnotDict = NULL;
	if (m_settings.flags & FSPDF_WATERMARKFLAG_ASANNOT)
	{
		FSRectF rect;
		rect.set(0, 0, 0, 0);
		pWMAnnotDict = FSANNOT_UTIL_InsertAnnot(FSAnnot::e_annotWatermark,m_settings.flags & FSPDF_WATERMARKFLAG_ONTOP ? -1 : 0, rect, ((FSPDFPageImp*)page)->getPage()->m_pFormDict, ((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument(), bSDKWatermark);
		//ret = page->InsertAnnot(FSPDF_ANNOTTYPE_WATERMARK, m_settings.flags & FSPDF_WATERMARKFLAG_ONTOP ? -1 : 0, pAnnot, bSDKWatermark);
		if (NULL == pWMAnnotDict) return FSCRT_ERRCODE_ERROR;
	}
	FS_BOOL bRet = m_pWatermarkInfo->InsertIntoPage(&m_settings, ((FSPDFPageImp*)page)->getPage(), pWMAnnotDict);
	if (bRet)
		return FSCRT_ERRCODE_SUCCESS;
	else
		return FSCRT_ERRCODE_ERROR;
}

FS_RESULT CFSPDF_Watermark::RegenerateSettingsXML()
{
	CFX_ByteString xmlContent;
	xmlContent = "<\?xml version=\"1.0\" encoding=\"UTF-8\" \?>";
	xmlContent += "<WatermarkSettings version = \"8.0\">";

	//// sourceToXML
	//{
	//	if (m_iWatermarkType == FSPDF_WATERMARK_TYPE_IMAGE || 
	//		m_iWatermarkType == FSPDF_WATERMARK_TYPE_BITMAP || 
	//		m_iWatermarkType == FSPDF_WATERMARK_TYPE_PAGE)
	//	{
	//		xmlContent += "<SourceFile name=\"CurrentImage\" type=\"\"/>";
	//	}
	//}

	// text: font
	{
		if (m_iWatermarkType == FSPDF_WATERMARK_TYPE_TEXT)
		{
			
			CPDF_Font* pPDFFont = ((FSPDF_WATERMARK_TEXT_PARAM*)m_pWatermarkParam)->properties.pPDFFont;
			if(pPDFFont)
			{
				CFX_ByteString strFontName, strFontType;
				strFontName = pPDFFont->m_Font.GetPsName().UTF8Encode();
				switch(pPDFFont->GetFontType())
				{
				case FXFM_NORMAL_TRUETYPE:
					strFontType = "TrueType";
					break;
				case FXFM_NORMAL_TYPE1:
					strFontType = "Type1";
					break;
				case FXFM_NORMAL_CFF:
					strFontType = "CFF";
					break;
				case FXFM_NORMAL_BDF:
				case FXFM_NORMAL_PCF:
				case FXFM_NORMAL_TYPE42:
				case FXFM_NORMAL_CIDTYPE1:
				case FXFM_NORMAL_PFR:
				case FXFM_NORMAL_WINFNT	:
				default:
					strFontType = "Type0";
				}

				CFX_ByteString strXML;
				strXML.Format("<Font name=\"%s\" type=\"%s\" %s size=\"%f\"/>",
					(FX_LPCSTR)strFontName, 
					(FX_LPCSTR)strFontType, 
					((FSPDF_WATERMARK_TEXT_PARAM*)m_pWatermarkParam)->properties.fontStyle & FSPDF_WATERMARK_FONTSTYLE_UNDERLINE  ?  "underline = \"true\"" : "", 
					((FSPDF_WATERMARK_TEXT_PARAM*)m_pWatermarkParam)->properties.fontSize);

				xmlContent += strXML;
			}
		}
	}
	// text: text
	{
		if (m_iWatermarkType == FSPDF_WATERMARK_TYPE_TEXT)
		{
			CXML_Element xmlText("", "");
			CFX_WideString strContent = ((FSPDF_WATERMARK_TEXT_PARAM*)m_pWatermarkParam)->wText;
			xmlText.AddChildContent(strContent);
			CFX_ByteString strXML = xmlText.OutputStream();
			strXML = strXML.Mid(2, strXML.GetLength() - 7 );

			xmlContent+= strXML;
		}
	}
	// scale
	{
		CFX_ByteString strXML;
		strXML.Format("<Scale value=\"%.2f\"/>", FX_MIN(m_settings.scaleX, m_settings.scaleY)); /* TODO: Verify about this */
		xmlContent+= strXML;
	}
	// rotation
	{
		CFX_ByteString strXML;

		strXML.Format("<Rotation value=\"%.2f\"/>", m_settings.rotation);
		xmlContent+= strXML;
	}
	// opacity
	{
		CFX_ByteString strXML;

		strXML.Format("<Opacity value=\"%.2f\"/>", m_settings.opacity / 100.0f);
		xmlContent+= strXML;
	}
	// location
	{
		CFX_ByteString strXML;

		strXML.Format("<Location ontop=\"%d\"/>", m_settings.flags & FSPDF_WATERMARKFLAG_ONTOP ? 1 : 0);
		xmlContent+= strXML;
	}
	// text: color;  other: 0 (for compatibility)
	{
		FS_ARGB textcolor = FSCRT_ARGB_Encode(0,0,0,0);
		if (m_iWatermarkType == FSPDF_WATERMARK_TYPE_TEXT)
		{
			textcolor = ((FSPDF_WATERMARK_TEXT_PARAM*)m_pWatermarkParam)->properties.color;
		}

		CFX_ByteString strXML;

		strXML.Format("<Color b=\"%f\" r=\"%f\" g=\"%f\"/>"
			, FSCRT_ARGB_GetBValue(textcolor) / 255.0f
			, FSCRT_ARGB_GetRValue(textcolor) / 255.0f
			, FSCRT_ARGB_GetGValue(textcolor) / 255.0f);

		xmlContent+= strXML;
	}
	// source position
	{
		CFX_ByteString strXML;

		strXML.Format("<Alignment unit=\"%d\" horizvalue=\"%f\" vertvalue=\"%f\" horizalign=\"%d\" vertalign=\"%d\"/>", 
			0 /*unit, currently always 0 (points)*/, 
			((2	==	m_settings.position % 3)/*RIGHT*/	? (-m_settings.offsetX)	: m_settings.offsetX),
			((0	==	m_settings.position / 3)/*TOP*/		? (-m_settings.offsetY)	: m_settings.offsetY),  /*TODO: CHECK THIS PHANTOM LOGIC, might be wrong*/
			m_settings.position % 3, m_settings.position / 3);

		xmlContent+= strXML;
	}
	// appearance
	{
		CFX_ByteString strXML;
		strXML.Format("<Appearance onscreen=\"%d\" onprint=\"%d\" fixedprint=\"%d\"/>",
			m_settings.flags & FSPDF_WATERMARKFLAG_INVISIBLE ? 0 : 1, 
			m_settings.flags & FSPDF_WATERMARKFLAG_NOPRINT ? 0 : 1,
			/*TODO: verify*/ m_settings.flags & FSPDF_WATERMARKFLAG_ASANNOT ? 1 : 0);
		xmlContent+= strXML;
	}
	// page range
	{
		//We'll generated fixed range (all).
		xmlContent += "<PageRange end=\"-1\" start=\"-1\" even=\"1\" odd=\"1\"/>";
	}
	xmlContent += "</WatermarkSettings>";

	m_pWatermarkInfo->UpdateSettingsXML(xmlContent);
	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT CFSPDF_Watermark::UpdateSettings(const FSPDF_WATERMARK_SETTINGS* settings)
{
	//FSCRT_LOCKOBJECT(lock, &m_lock);

	m_settings = *settings;
	return FSCRT_ERRCODE_SUCCESS;
}

//FS_RESULT CFSPDF_Watermark::RemoveAnnotWMFromPage(CFSCRT_LTPDFPage* pPage)
//{
//	if (!pPage) return FSCRT_ERRCODE_PARAM;
//
//	if (pPage->IsAnnotsLoaded())
//	{
//		FSCRT_BSTRC(strWMFilter, FSPDF_ANNOTTYPE_WATERMARK);
//		FS_INT32 annotcount = 0;
//		FS_INT32 i = 0;
//		FS_RESULT ret = pPage->CountAnnotsByFilter(&strWMFilter, &annotcount);
//		if(ret == FSCRT_ERRCODE_NOTFOUND) ret = FSCRT_ERRCODE_SUCCESS;
//		if (ret != FSCRT_ERRCODE_SUCCESS) return ret;
//
//		for(i = annotcount - 1 ; i >= 0; i--)
//		{
//			FSCRT_ANNOT annot = NULL;
//			ret = pPage->GetAnnotByFilter(&strWMFilter, i, &annot);
//			if (ret != FSCRT_ERRCODE_SUCCESS) return ret;
//			ret = pPage->DeleteAnnot((CFSCRT_LTPDFAnnot*)annot);
//			if (ret != FSCRT_ERRCODE_SUCCESS) return ret;
//		}
//	}
//	else
//	{
//		return pPage->RemoveWatermarkAnnot(0, TRUE);
//	}
//
//	return FSCRT_ERRCODE_SUCCESS;
//}

//FS_RESULT CFSPDF_Watermark::CountAnnotWMFromPage(CFSCRT_LTPDFPage* pPage, FS_INT32* count)
//{
//	if (!pPage) 
//		return FSCRT_ERRCODE_PARAM;
//
//	FS_RESULT ret = FSCRT_ERRCODE_ERROR;
//	if (pPage->IsAnnotsLoaded())
//	{
//		FSCRT_BSTRC(strWMFilter, FSPDF_ANNOTTYPE_WATERMARK);
//		FS_INT32 annotcount = 0;
//		FS_INT32 i = 0;
//
//		ret = pPage->CountAnnotsByFilter(&strWMFilter, &annotcount);
//		if(ret == FSCRT_ERRCODE_NOTFOUND) ret = FSCRT_ERRCODE_SUCCESS;
//		if (ret != FSCRT_ERRCODE_SUCCESS) 
//			return ret;
//		else
//			*count = annotcount;
//	}
//	else
//		ret = pPage->CountWatermarkAnnots(count);
//
//	return ret;
//}
//
//FS_RESULT CFSPDF_Watermark::ST_CountPageObjWMFromPage(CFSCRT_LTPDFPage* pPage, FS_INT32* count)
//{
//	FSCRT_LOCKOBJECT(lock, &pPage->m_lock);
//
//	FSCRT_EXCEPTION_STRAT
//
//    *count = 0;
//
//	CPDF_Page* objects = pPage->m_pSTPage;
//
//	FX_POSITION pos = objects->GetFirstObjectPosition();
//	while(pos)
//	{
//		CPDF_PageObject* pPageObject = objects->GetNextObject(pos);
//		if (!pPageObject || pPageObject->m_Type != FSPDF_PAGEOBJECT_FORMXOBJECT)
//			continue;
//
//		CPDF_FormObject* pFormObj = (CPDF_FormObject*)pPageObject;
//		if (!pFormObj || !pFormObj->m_pForm || !pFormObj->m_pForm->m_pFormDict) 
//			continue;
//
//		CPDF_Dictionary* pPieceInfo = pFormObj->m_pForm->m_pFormDict->GetDict(FX_BSTRC("PieceInfo"));
//		if (!pPieceInfo)
//			continue;
//
//		CPDF_Dictionary* pACTInfo  = pPieceInfo->GetDict(FX_BSTRC("ADBE_CompoundType"));
//		if (!pACTInfo) 
//			continue;
//
//		if (pACTInfo->GetString(FX_BSTRC("Private")) != FX_BSTRC("Watermark")) 
//			continue;
//		else
//			(*count)++;
//	}
//
//	return FSCRT_ERRCODE_SUCCESS;
//
//	FSCRT_EXCEPTION_END
//}
//
//FS_RESULT CFSPDF_Watermark::CountPageObjWMFromPage(CFSCRT_LTPDFPage* pPage, FS_INT32* count)
//{
//	if (!pPage) 
//		return FSCRT_ERRCODE_PARAM;
//
//	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
//	FSCRT_StartOOM();
//	FSCRT_StartSTMemory();
//	FSCRT_CheckObject(pPage);
//
//	ret = ST_CountPageObjWMFromPage(pPage, count);
//
//	FSCRT_EndSTMemory();
//
//	if (ret != FSCRT_ERRCODE_ST_OOM) 
//		return ret;
//
//	FSCRT_Recover(ret, pPage);
//	FSCRT_EndOOM();
//
//	return FSCRT_ERRCODE_UNRECOVERABLE;
//}

//FS_RESULT CFSPDF_Watermark::ST_RemovePageObjWMFromPage(CFSCRT_LTPDFPage* pPage)
//{
//	FSCRT_LOCKOBJECT(lock, &pPage->m_lock);
//
//	FSCRT_EXCEPTION_STRAT
//
//	FSPDF_PAGEOBJECTS objects = (FSPDF_PAGEOBJECTS)pPage->m_pSTPage;;
//
//	FX_POSITION pos = ((CPDF_PageObjects*)objects)->GetFirstObjectPosition();
//	while(pos)
//	{
//		FX_POSITION tempPos = pos;
//		CPDF_PageObject* pPageObject = ((CPDF_PageObjects*)objects)->GetNextObject(pos);
//		if (!pPageObject || pPageObject->m_Type != FSPDF_PAGEOBJECT_FORMXOBJECT) continue;
//
//		CPDF_FormObject* pFormObj = (CPDF_FormObject*)pPageObject;
//		if (!pFormObj || !pFormObj->m_pForm || !pFormObj->m_pForm->m_pFormDict) continue;
//
//		CPDF_Dictionary* pPieceInfo = pFormObj->m_pForm->m_pFormDict->GetDict(FX_BSTRC("PieceInfo"));
//		if (!pPieceInfo) continue;
//
//		CPDF_Dictionary* pACTInfo  = pPieceInfo->GetDict(FX_BSTRC("ADBE_CompoundType"));
//		if (!pACTInfo) continue;
//
//		if (pACTInfo->GetString(FX_BSTRC("Private")) != FX_BSTRC("Watermark")) continue;
//		// this is a watermark. remove it.
//		((CPDF_PageObjects*)objects)->RemoveObject(tempPos);
//	}
//
//	CPDF_Dictionary* pFormDict = ((CPDF_PageObjects*)objects)->m_pFormDict;
//	if (!pFormDict) return FSCRT_ERRCODE_PARAM;
//	if (!pFormDict->KeyExist(FX_BSTRC("Resources")))
//		pFormDict->SetAt(FX_BSTRC("Resources"), CPDF_Dictionary::Create());
//
//	CPDF_ContentGenerator CG((CPDF_Page*)objects);
//	CG.GenerateContent();
//
//	return FSCRT_ERRCODE_SUCCESS;
//
//	FSCRT_EXCEPTION_END
//}
//
//FS_RESULT CFSPDF_Watermark::RemovePageObjWMFromPage(CFSCRT_LTPDFPage* pPage)
//{
//	if (!pPage) return FSCRT_ERRCODE_PARAM;
//
//	FS_RESULT ret = FSCRT_ERRCODE_SUCCESS;
//	FSCRT_StartOOM();
//	FSCRT_StartSTMemory();
//	FSCRT_CheckObject(pPage);
//
//	ret = ST_RemovePageObjWMFromPage(pPage);
//
//	FSCRT_EndSTMemory();
//
//	if (ret != FSCRT_ERRCODE_ST_OOM) return ret;
//
//	FSCRT_Recover(ret, pPage);
//	FSCRT_EndOOM();
//	
//	FSCRT_ReturnValue(FSCRT_ERRCODE_UNRECOVERABLE);
//}
/*const FS_WORD* pUTF16LEWideString, FS_DWORD dwUTF16LELen, const FSPDF_WATERMARK_TEXTPIECE_FONTARRAY* fontarray,*/ 
FS_RESULT CFSPDF_Watermark::ST_CreateFromText(CFX_WideString wsText, const FSPDF_WATERMARK_TEXTPROPERTIES* properties, FS_BOOL bSDKWatermark/* = FALSE*/)
{
	//FSCRT_LOCKOBJECT(lock, &m_pParent->m_lock);

	m_pWatermarkInfo = FX_NEW CFSPDF_WatermarkInfo();
	if (!m_pWatermarkInfo) return FSCRT_ERRCODE_ERROR;
	
	//CFSCRT_LTPDFDocument* pLTPDFDoc = (CFSCRT_LTPDFDocument*)m_pParent;
	//CFSCRT_LTFont* pLTFont = (CFSCRT_LTFont*)properties->font;
	//CFSCRT_LTPDFFonts* pdfFonts = pLTPDFDoc->GetPDFFonts();
	//FS_RESULT ret = pdfFonts->IsPDFFontAvailable(*pLTFont);
	//if ( FSCRT_ERRCODE_NOTFOUND == ret)
	//	ret = pdfFonts->ST_AddPDFFont(*pLTFont); //should we rollback?
	//else if (ret == FSCRT_ERRCODE_ERROR)
	//	ret = pdfFonts->ST_RecoverPDFFont(*pLTFont);
	//if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_NOTFOUND)
	//	return ret;
	// update color with alpha info in setting
	FS_ARGB color = properties->color;
	FX_BYTE a = (FX_BYTE) 0xff * m_settings.opacity / 100;
	color = FSCRT_ARGB_Encode(a, FXARGB_R(color), FXARGB_G(color), FXARGB_B(color));

	m_pWatermarkInfo->CreateWatermark(((FSPDFDocImp*)m_pFSPDFDoc)->getPDFDocument(), wsText,/* pdfFonts, fontarray, */properties->pPDFFont, properties->fontSize, color, 
		properties->fontStyle & FSPDF_WATERMARK_FONTSTYLE_UNDERLINE ? TRUE : FALSE, properties->lineSpace, properties->alignment, bSDKWatermark);
	return FSCRT_ERRCODE_SUCCESS;
}

//FS_RESULT CFSPDF_Watermark::ST_CreateFromImage(FSCRT_IMAGE image)
//{
//	FSCRT_LOCKOBJECT(lock, &((CFSCRT_LTImage*)image)->m_lock);
//	FSCRT_LOCKOBJECT(lock2, &m_pParent->m_lock);
//
//	FSCRT_EXCEPTION_STRAT
//
//	m_pWatermarkInfo = FX_NEW CFSPDF_WatermarkInfo();
//	if (!m_pWatermarkInfo) return FSCRT_ERRCODE_ST_OOM;
//
//	m_pWatermarkInfo->CreateWatermark(((CFSCRT_LTPDFDocument*)m_pParent)->m_pSTDoc, ((CFSCRT_LTImage*)image)->m_pImage, ((CFSCRT_LTImage*)image)->m_hImage);
//
//	return FSCRT_ERRCODE_SUCCESS;
//
//	FSCRT_EXCEPTION_END
//}
//
//FS_RESULT CFSPDF_Watermark::ST_CreateFromBitmap(FSCRT_BITMAP bitmap)
//{
//	FSCRT_LOCKOBJECT(lock, &((CFSCRT_LTDIBitmap*)bitmap)->m_lock);
//	FSCRT_LOCKOBJECT(lock2, &m_pParent->m_lock);
//
//	FSCRT_EXCEPTION_STRAT
//
//	m_pWatermarkInfo = FX_NEW CFSPDF_WatermarkInfo();
//	if (!m_pWatermarkInfo) return FSCRT_ERRCODE_ST_OOM;
//	//Support 565 format?
//	m_pWatermarkInfo->CreateWatermark(((CFSCRT_LTPDFDocument*)m_pParent)->m_pSTDoc, (CFX_DIBitmap*)((CFSCRT_LTDIBitmap*)bitmap)->GetBitmap());
//
//	return FSCRT_ERRCODE_SUCCESS;
//
//	FSCRT_EXCEPTION_END
//}
//
//FS_RESULT CFSPDF_Watermark::ST_CreateFromPage(FSCRT_PAGE page)
//{
//	FSCRT_LOCKOBJECT(lock, &((CFSCRT_LTPDFPage*)page)->m_lock);
//	FSCRT_LOCKOBJECT(lock2, &m_pParent->m_lock);
//
//	FSCRT_EXCEPTION_STRAT
//
//	m_pWatermarkInfo = FX_NEW CFSPDF_WatermarkInfo();
//	if (!m_pWatermarkInfo) return FSCRT_ERRCODE_ST_OOM;
//
//	CPDF_ObjectArray ObjectArray;
//	CPDF_RectArray	 RectArray;
//	CFX_DWordArray	 removeAnnotArray;
//	CPDF_Document* pDocument = ((CFSCRT_LTPDFPage*)page)->m_pSTPage->m_pDocument;
//	CPDF_Dictionary* pPageDict = ((CFSCRT_LTPDFPage*)page)->m_pSTPage->m_pFormDict;
//	FS_RESULT ret = ((CFSCRT_LTPDFPage*)page)->ST_NOJMP_ParserAnnotsForFlatten(((CFSCRT_LTPDFPage*)page)->m_pSTPage, FSPDF_FLATTENFLAG_DISPLAY, &RectArray, &ObjectArray, &removeAnnotArray);
//	if (ret != FSCRT_ERRCODE_SUCCESS) {
//		return ret;
//	}
//	FS_BOOL isFinished = m_pWatermarkInfo->CreateWatermark(((CFSCRT_LTPDFDocument*)m_pParent)->m_pSTDoc, ((CFSCRT_LTPDFPage*)page)->m_pSTPage, &ObjectArray);
//	if (isFinished != TRUE) {
//		return FSCRT_ERRCODE_ERROR;
//	}
//	return FSCRT_ERRCODE_SUCCESS;
//
//	FSCRT_EXCEPTION_END
//}

FS_RESULT CFSPDF_Watermark::GetSize(FS_FLOAT* width, FS_FLOAT* height)
{
	if (NULL == width || NULL == height)
		return FSCRT_ERRCODE_PARAM;
	FS_BOOL bRet = m_pWatermarkInfo->GetWaterMarkSize(width, height);
	if (bRet)
		return FSCRT_ERRCODE_SUCCESS;
	else
		return FSCRT_ERRCODE_ERROR;
}


