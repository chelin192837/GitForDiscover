//#include "../inc/fpdf_define.h"
//#include "../widget/pdfwindow/include/fxedit/fx_edit.h"
#include "../../include/common.h"

#include "../fxcore.h"

#include "fpdf_richtext.h"

FS_DWORD FSCRT_HexWideStringToInteger(CFX_WideString wsString)
{
	wsString.MakeUpper();

	FS_DWORD result = 0;
	FX_INT32 bits = 0;
	FX_INT32 len = wsString.GetLength();
	for (FX_INT32 i = 0; i < len; i++)
	{
		FS_WORD bit = 0;
		FX_WCHAR wchHex = wsString.GetAt(i);

		switch(wchHex)
		{
		case L'A':
			bit = 10;
			break;
		case L'B':
			bit = 11;
			break;
		case L'C':
			bit = 12;
			break;
		case L'D':
			bit = 13;
			break;
		case L'E':
			bit = 14;
			break;
		case L'F':
			bit = 15;
			break;
		default:
			{
				CFX_WideString wsTmp = FX_WSTR(wchHex);
				bit = FXSYS_wtoi(wsTmp);
			}
			break;
		}
		FS_DWORD temp = FXSYS_pow(16, wsString.GetLength() - i - 1);
		result += bit * temp;
	}
	return result;
}

static CFX_WideString GetKeywordValue(const CFX_WideString &str)
{
	CFX_WideString wsKeyword;
	FX_INT32 i = str.Find(L":");
	if (i != -1)
	{
		wsKeyword = str.Right(str.GetLength() - i - 1);
		wsKeyword.TrimLeft(L"'\" \t\r\n");
		wsKeyword.TrimRight(L"'\" \t\r\n");
	}
	return wsKeyword;
}

static FX_FLOAT GetDecimalValue(const CFX_WideString &str)
{
	if (str.IsEmpty()) return 0.0f;
	return (FX_FLOAT)GetKeywordValue(str).GetFloat();
}

static FX_COLORREF GetRGBValue(const CFX_WideString &str)
{
	FX_COLORREF color = 0;
	CFX_WideString wsColor = GetKeywordValue(str);
	wsColor.Delete(0);
	color = (FX_COLORREF)FSCRT_HexWideStringToInteger(wsColor);

	//color = (color & 0x0000ff00) | (color << 16 & 0x00ffffff) | (color >> 16);
	return color;
}

static void GetListValue(const CFX_WideString &str, CFX_WideStringArray &values)
{
	CFX_WideString wsList = GetKeywordValue(str);
	if (wsList.IsEmpty()) return;

	CFX_WideString wsSeps = L",";

	CFX_WideString wsToken = wsList;
	FX_STRSIZE pos = wsList.Find(wsSeps);

	if (pos != -1)
		wsToken = wsList.Left(pos);

	if (!wsToken.IsEmpty())
		values.RemoveAll();

	while (!wsToken.IsEmpty())
	{
		values.Add(wsToken);

		if (-1 == pos) break;

		wsList = wsList.Right(wsList.GetLength() - pos - 1);
		if (wsList.IsEmpty()) break;

		wsToken = wsList;

		pos = wsList.Find(wsSeps);

		if (pos != -1)
			wsToken = wsList.Left(pos);
	}
	wsToken.ReleaseBuffer();
	wsList.ReleaseBuffer();
}

static FX_BOOL IsProperty(const CFX_WideString &str, CFX_WideString prt)
{
	FX_INT32 len = prt.GetLength();
	if (str.GetLength() >= len)
		return str.Left(len).CompareNoCase(prt) == 0;
	else
		return FALSE;
}

////////////////////////////////////////////////
//CPDF_RichTextXML

CPDF_RichTextXML::CPDF_RichTextXML()
			:m_pXMLBuf(NULL)
			, m_wsXFASpecVer(L"2.4")
			, m_wsXFAAPIVer(L"Acroform:2.7.0.0")
			, m_bCurrentRTL(FALSE)
			, m_Beginalign(0)
{}

CPDF_RichTextXML::~CPDF_RichTextXML()
{
	if (m_pXMLBuf)
		FX_Free(m_pXMLBuf);

	m_pXMLBuf = NULL;
	ClearTextBlocks();
}

void CPDF_RichTextXML::ClearTextBlocks()
{
	for (FX_INT32 i = 0; i < m_TextBlocks.GetSize(); i++)
	{
		if (m_TextBlocks[i])
		{
			delete m_TextBlocks[i];
			m_TextBlocks[i] = NULL;
		}
	}
	m_TextBlocks.RemoveAll();
}

void CPDF_RichTextXML::FillStyle(CFX_WideString wsStyle, STYLE &style)
{
	if (wsStyle.IsEmpty())
		return;

	CFX_WideString wsSeps = L";";

	FX_STRSIZE pos = wsStyle.Find(wsSeps);
	CFX_WideString wsToken = wsStyle;
	if (-1 != pos) 
		wsToken = wsStyle.Left(pos);

	while (!wsToken.IsEmpty())
	{
		CFX_WideString wsProperty = wsToken;
		wsProperty.TrimLeft();
		wsProperty.TrimRight();

		CFX_WideString wsValue = GetKeywordValue(wsProperty);
		if (IsProperty(wsProperty, L"text-align"))
		{
			if (!wsValue.IsEmpty())
				style.wsTextAlign = wsValue;

			style.nPropertyUsed |= STYLE::TEXT_ALIGN_FLAG;
		}
		else if (IsProperty(wsProperty, L"vertical-align"))
		{
			if(0 == m_wsXFASpecVer.CompareNoCase(L"2.0.2"))
			{
				style.fVerticalAlign = 0.0f;

				if (wsValue.Find(L"+") != -1)
					style.nScriptType = 1;
				else if (wsValue.Find(L"-") != -1)
					style.nScriptType = 2;
				else
					style.nScriptType = 0;
			}
			else
			{
				style.fVerticalAlign = GetDecimalValue(wsProperty);

				if (wsValue.CompareNoCase(L"+0.0pt") == 0)
					style.nScriptType = 1;
				else if (wsValue.CompareNoCase(L"-0.0pt") == 0)
					style.nScriptType = 2;
				else
					style.nScriptType = 0;
			}
			style.nPropertyUsed |= STYLE::VERT_ALIGN_FLAG;
			style.nPropertyUsed |= STYLE::SCRIPT_TYPE_FLAG;
		}
		else if (IsProperty(wsProperty, L"font-size"))
		{
			wsProperty.TrimRight(L"pt");
			style.fFontSize = GetDecimalValue(wsProperty);
			style.nPropertyUsed |= STYLE::FONT_SIZE_FLAG;
		}
		else if (IsProperty(wsProperty, L"font-style"))
		{
			if (!wsValue.IsEmpty())
				style.wsFontStyle = wsValue;

			style.nPropertyUsed |= STYLE::FONT_STYLE_FLAG;
		}
		else if (IsProperty(wsProperty, L"font-weight"))
		{
			if (!wsValue.IsEmpty())
				style.wsFontWeight = wsValue;

			style.nPropertyUsed |= STYLE::FONT_WEIGHT_FLAG;
		}
		else if (IsProperty(wsProperty, L"font-family"))
		{
			GetListValue(wsProperty, style.wsArrayFontFamily);
			style.nPropertyUsed |= STYLE::FONT_FAMILY_FLAG;
		}
		else if (IsProperty(wsProperty, L"font"))
		{
			CFX_WideString wsFontFamily;
			if(GetFontNameAndFontSize(wsValue, wsFontFamily, style.fFontSize) == STYLE::FONT_SIZE_FLAG)
				style.nPropertyUsed |= STYLE::FONT_SIZE_FLAG;

			if (!wsFontFamily.IsEmpty())
			{
				style.wsArrayFontFamily.Add(wsFontFamily);
				style.nPropertyUsed |= STYLE::FONT_FAMILY_FLAG;
			}
		}
		else if (IsProperty(wsProperty, L"color"))
		{
			style.color = GetRGBValue(wsProperty);
			style.nPropertyUsed |= STYLE::TEXT_COLOR_FLAG;
		}
		else if (IsProperty(wsProperty, L"text-decoration"))
		{
			if (!wsValue.IsEmpty())
				style.wsTextDecoration = wsValue;
			style.nPropertyUsed |= STYLE::TEXT_DECORATION_FLAG;
		}
		else if (IsProperty(wsProperty, L"font-stretch"))
		{
			if (!wsValue.IsEmpty())
				style.wsFontStretch = wsValue;
			style.nPropertyUsed |= STYLE::FONT_STRETCH_FLAG;
		}
		else if (IsProperty(wsProperty, L"line-height"))
		{
			style.fLineHeight = GetDecimalValue(wsProperty);
			style.nPropertyUsed |= STYLE::LINE_HEIGHT_FLAG;
		}
		else if (IsProperty(wsProperty, L"letter-spacing"))
		{
			style.fLetterSpacing = GetDecimalValue(wsProperty);
			style.nPropertyUsed |= STYLE::TEXT_LETTERSPACE_FLAG;
		}
		else if (IsProperty(wsProperty, L"xfa-font-horizontal-scale"))
		{
			style.fXFAFontHorizontalScale = GetDecimalValue(wsProperty);
			style.nPropertyUsed |= STYLE::TEXT_HORIZSCALE_FLAG;
		}

		if (pos != -1)
			wsStyle = wsStyle.Right(wsStyle.GetLength() - pos - 1);
		else
			break;

		if (wsStyle.IsEmpty()) break;

		wsToken = wsStyle;

		pos = wsStyle.Find(wsSeps);
		if (-1 != pos) 
			wsToken = wsStyle.Left(pos);
	}

	if (style.wsArrayFontFamily.GetSize() == 0)
		style.wsArrayFontFamily.Add(L"Helvetica");
}

FS_DWORD CPDF_RichTextXML::GetFontSize(const CFX_WideString &wsCSS, FX_FLOAT &fFontSize, FX_INT32 &nSta, FX_INT32 &nEnd)
{
	FX_INT32 iPTPos = wsCSS.Find(L"pt", nSta);

	FX_WCHAR wOneCh;
	if (iPTPos != -1)
	{
		wOneCh = wsCSS[iPTPos -1];

		if (!(wOneCh >= '0' && wOneCh <= '9' || wOneCh == '.'))
		{
			nSta += 2;
			return GetFontSize(wsCSS,fFontSize,nSta,nEnd);
		}

		nEnd = iPTPos + 1;
		CFX_WideString wsFontSize;
		while(iPTPos--)
		{
			FX_WCHAR ch = wsCSS[iPTPos];
			CFX_WideString wsCh = ch;
			if (ch >= '0' && ch <= '9' || ch == '.')
				wsFontSize = wsCh + wsFontSize;
			else
				break;
		}
		nSta = iPTPos + 1;
		fFontSize = wsFontSize.GetFloat();
		return STYLE::FONT_SIZE_FLAG;
	}

	return 0;
}

void CPDF_RichTextXML::GetFontName(const CFX_WideString &wsCSS, CFX_WideString & wsFontName)
{
	FX_BOOL bStart = FALSE, bQuote = FALSE, bBlank = FALSE;
	CFX_WideString wsFontFamily;
	for (int i = 0; i < wsCSS.GetLength(); i++)
	{
		FX_WCHAR ch = wsCSS[i];
		if (!bStart && (ch == ' ' || ch =='\t') )
			continue;

		if (ch == '\'')
		{
			if (!bStart)
			{
				bStart = TRUE;
				bQuote = TRUE;
				continue;
			}
			else
				break;
		}
		else
			bStart = TRUE;

		if (ch == ',')
			break;

		if (ch == ' ' && !bQuote)
			bBlank = TRUE;
		
		if(bBlank && (ch >= '0' && ch <= '9'))
			break;

		if ((ch == '\t') && !bQuote)
			break;
		wsFontFamily += ch;
	}
	wsFontName = wsFontFamily;
}

FX_INT32 CPDF_RichTextXML::GetFontNameAndFontSize(const CFX_WideString &wsCSS, CFX_WideString &wsFontName, FX_FLOAT &fFontSize)
{
	FX_INT32 iStat = 0, iEnd = 0;

	FX_INT32 iRet = GetFontSize(wsCSS, fFontSize, iStat, iEnd);

	CFX_WideString wsCSSTmp = wsCSS;
	wsCSSTmp.Delete(iStat, iEnd - iStat + 1);

	GetFontName(wsCSSTmp, wsFontName);

	return iRet;
}

FS_BOOL CPDF_RichTextXML::FillStyle(CXML_Element* pXML, STYLE &style)
{
	if (!pXML->HasAttr("style"))
		return FALSE;

	CFX_WideString wsStyle = pXML->GetAttrValue("style");
	FillStyle(wsStyle, style);
	return TRUE;
}

void CPDF_RichTextXML::ParseXML2Text(CXML_Element* pXML, STYLE* pDefStyle, STYLE* pHanderStyle)
{
	STYLE style;
	if(pDefStyle) style = *pDefStyle;

	FillStyle(pXML, style);

	FX_BOOL bBody = FALSE, bPara = FALSE;

	//XHTML elements tag
	CFX_ByteString bsTagName = pXML->GetTagName();
	if (!bsTagName.IsEmpty())
	{	
		if (bsTagName.EqualNoCase("body"))
		{
			bBody = TRUE;
		}
		else if (bsTagName.EqualNoCase("p"))
		{
			TEXT_BLOCK* pTextBlock = FX_NEW TEXT_BLOCK;
			if (!pTextBlock) return;

			CFX_WideString wsValue = pXML->GetAttrValue("dir");
			if (!wsValue.IsEmpty())
			{
				if (wsValue.CompareNoCase(L"rtl") == 0)
				{
					style.bRTL = TRUE;
					m_bCurrentRTL = TRUE;
				}
			}
			if (style.wsTextAlign == L"center")
				m_Beginalign = 1;
			
			pTextBlock->style = style;
			pTextBlock->text = L"\n";
			pTextBlock->p = PARAGRAPH_START;

			m_TextBlocks.Add(pTextBlock);
			bPara = TRUE;
			pHanderStyle = &pTextBlock->style;
		}
		else if (bsTagName.EqualNoCase("i"))
			style.wsFontStyle = L"italic";
		else if (bsTagName.EqualNoCase("b"))
			style.wsFontWeight = L"bold";
		else if (bsTagName.EqualNoCase("span"))
		{
			;//Nothing
		}
	}

	for (FX_INT32 iChild = 0, cChild = pXML->CountChildren(); iChild < cChild; iChild++)
	{
		CXML_Element::ChildType eType = pXML->GetChildType(iChild);
		if(eType == CXML_Element::Content)
		{
			CFX_WideString wsContent = pXML->GetContent(iChild);
			CFX_WideString wsContentOld = wsContent;
			wsContent.TrimLeft(L"\r\n");
			wsContent.TrimRight(L"\r\n");

			if (!wsContent.IsEmpty())
			{
				TEXT_BLOCK* pTextBlock = FX_NEW TEXT_BLOCK;
				if (!pTextBlock) return;

				pTextBlock->style = style;

				CFX_WideString htmlSpace ((wchar_t)160);
				wsContent.Replace(htmlSpace, (FX_LPCWSTR)L" ");

				pTextBlock->text = wsContent;
				m_TextBlocks.Add(pTextBlock);
			}
			else
			{
				if((wsContentOld.Find(L"\n") != -1) || (wsContentOld.Find(L"\r") != -1))
				{
					if (pHanderStyle && iChild == 0)
					{
						pHanderStyle->wsTextAlign = style.wsTextAlign;
						pHanderStyle->fVerticalAlign = style.fVerticalAlign;
						pHanderStyle->fFontSize	= style.fFontSize;
						pHanderStyle->wsFontStyle = style.wsFontStyle;
						pHanderStyle->wsFontWeight = style.wsFontWeight;
						pHanderStyle->wsArrayFontFamily.Copy(style.wsArrayFontFamily);
						pHanderStyle->color	= style.color;
						pHanderStyle->wsTextDecoration = style.wsTextDecoration;
						pHanderStyle->wsFontStretch	= style.wsFontStretch;
						pHanderStyle->fLineHeight = style.fLineHeight;
						pHanderStyle->fLetterSpacing = style.fLetterSpacing;
						pHanderStyle->fXFAFontHorizontalScale = style.fXFAFontHorizontalScale;
						pHanderStyle->nScriptType = style.nScriptType;
						pHanderStyle->nPropertyUsed	= style.nPropertyUsed;
						pHanderStyle->bRTL = style.bRTL;

						pHanderStyle = NULL;
					}
				}
			}

		}
		else if (eType == CXML_Element::Element)
		{
			CXML_Element* pChild = pXML->GetElement(iChild);
			ParseXML2Text(pChild, &style,pHanderStyle);
		}
	}

	if (bPara)
	{
		TEXT_BLOCK* pTextBlock = FX_NEW TEXT_BLOCK;
		if (!pTextBlock) return;

		pTextBlock->style = style;
		pTextBlock->text = L"\n";
		pTextBlock->p = PARAGRAPH_END;
		m_TextBlocks.Add(pTextBlock);
	}
	if (bBody || bPara)
	{
		if (m_TextBlocks.GetSize() > 0)
		{
			if (m_TextBlocks[0]->p == PARAGRAPH_START)
			{
				delete m_TextBlocks[0];
				m_TextBlocks.RemoveAt(0);
			}
		}

		if (m_TextBlocks.GetSize() > 0)
		{
			if (m_TextBlocks[m_TextBlocks.GetSize() - 1]->p == PARAGRAPH_END)
			{
				delete m_TextBlocks[m_TextBlocks.GetSize() - 1];
				m_TextBlocks.RemoveAt(m_TextBlocks.GetSize() - 1);
			}
		}
	}
	if (bBody)
	{
		FX_INT32 i = 0;
		for (i = 0; i < m_TextBlocks.GetSize() - 1; i++)
		{
			if (m_TextBlocks[i]->p == PARAGRAPH_END)
			{
				delete m_TextBlocks[i];
				m_TextBlocks.RemoveAt(i);
			}
		}
		for (i = 0; i < m_TextBlocks.GetSize() - 1; i++)
		{
			TEXT_BLOCK* piCur = m_TextBlocks[i];
			if (piCur->p != 0 || piCur->text == L"\n")
			{
				TEXT_BLOCK* piNext = m_TextBlocks[i + 1];
				
				if ((piNext->style.nPropertyUsed & STYLE::LINE_HEIGHT_FLAG)&&
					(!(piCur->style.nPropertyUsed & STYLE::LINE_HEIGHT_FLAG) || piNext->p == 0))
				{
					piCur->style.fLineHeight = piNext->style.fLineHeight;
					piCur->style.nPropertyUsed |= STYLE::LINE_HEIGHT_FLAG;
				}

				if ((piNext->style.nPropertyUsed & STYLE::FONT_SIZE_FLAG)&&
					(!(piCur->style.nPropertyUsed & STYLE::FONT_SIZE_FLAG) || piNext->p == 0))
				{
					piCur->style.fFontSize = piNext->style.fFontSize;
					piCur->style.nPropertyUsed |= STYLE::FONT_SIZE_FLAG;
				}

				if ((piNext->style.nPropertyUsed & STYLE::FONT_FAMILY_FLAG)&&
					(!(piCur->style.nPropertyUsed & STYLE::FONT_FAMILY_FLAG) || piNext->p == 0))
				{
					piCur->style.wsArrayFontFamily.Copy(piNext->style.wsArrayFontFamily);
					piCur->style.nPropertyUsed |= STYLE::FONT_FAMILY_FLAG;
				}
			}
		}
	}
}

void CPDF_RichTextXML::ParseXML2Text()
{
	ClearTextBlocks();

	STYLE *pHanderstyle = NULL;

	CXML_Element* pXML = CXML_Element::Parse(m_pXMLBuf, m_bufSize, TRUE);
	if (pXML)
	{
		CFX_ByteString bsTagName = pXML->GetTagName();
		if (bsTagName.EqualNoCase("body"))
		{
			if (pXML->HasAttr("xfa:spec"))
			{
				CFX_WideString wsSpec =	pXML->GetAttrValue("xfa:spec");
				if (!wsSpec.IsEmpty())
					m_wsXFASpecVer = wsSpec;
			}
			if (pXML->HasAttr("xfa:APIVersion"))
			{
				CFX_WideString wsAPISpec =	pXML->GetAttrValue("xfa:APIVersion");
				if (!wsAPISpec.IsEmpty())
					m_wsXFAAPIVer = wsAPISpec;
			}
		}
		ParseXML2Text(pXML, &m_defaultStyle, pHanderstyle);
		delete pXML;
	}
	else
	{
		TEXT_BLOCK* pSingle = FX_NEW TEXT_BLOCK;
		if (!pSingle) return;

		CFX_ByteString bsText((FX_LPCBYTE)m_pXMLBuf, m_bufSize);
		pSingle->style = m_defaultStyle;
		if (m_defaultStyle.wsArrayFontFamily.GetSize() == 0)
			pSingle->style.wsArrayFontFamily.Add(L"Helvetica");

		pSingle->text = bsText.UTF8Decode();
		m_TextBlocks.Add(pSingle);
	}
	
}

FS_BOOL CPDF_RichTextXML::SetXML(CFX_WideString wsXMLString, CFX_WideString wsDS /* = L"" */)
{
	if (wsXMLString.IsEmpty() && wsDS.IsEmpty())
		return FALSE;
	if (m_pXMLBuf)
	{
		FX_Free(m_pXMLBuf);
		m_pXMLBuf = NULL;
	}
	m_TextBlocks.RemoveAll();
	if (!wsDS.IsEmpty())
		FillStyle(wsDS, m_defaultStyle);
	
	if (wsXMLString.IsEmpty())
		return TRUE;
	CFX_WideString wsContent = wsXMLString;
	CFX_ByteString wsUTF8 = wsContent.UTF8Encode();

	FX_INT32 len = wsUTF8.GetLength();

	m_pXMLBuf = FX_Alloc(FX_BYTE, len + 1);
	if (!m_pXMLBuf) 
		return FALSE;

	FXSYS_memcpy(m_pXMLBuf, wsUTF8.GetBuffer(len), len);
	m_pXMLBuf[len] = 0;

	m_bufSize = len + 1;
	ParseXML2Text();
	return TRUE;
}

FS_BOOL	CPDF_RichTextXML::getFontName(FS_INT32 index, CFX_WideString& fontName)
{
	FS_INT32 nFontCount = 0;
	if (m_TextBlocks.GetSize()>0)
		nFontCount = m_TextBlocks.GetAt(0)->style.wsArrayFontFamily.GetSize();
	else
		nFontCount = m_defaultStyle.wsArrayFontFamily.GetSize();

	if (index<0 || index>=nFontCount) return FALSE;

	if (m_TextBlocks.GetSize()>0)
		fontName = *(m_TextBlocks.GetAt(0)->style.wsArrayFontFamily.GetDataPtr(index));
	else if (m_defaultStyle.nPropertyUsed & STYLE::FONT_FAMILY_FLAG)
		fontName = *(m_defaultStyle.wsArrayFontFamily.GetDataPtr(index));
	else
		return FALSE;
	return TRUE;
}

FS_BOOL	CPDF_RichTextXML::getFontSize(FS_FLOAT& fFontSize)
{
	if (m_TextBlocks.GetSize()>0)
		fFontSize = m_TextBlocks.GetAt(0)->style.fFontSize;
	else if (m_defaultStyle.nPropertyUsed & STYLE::FONT_SIZE_FLAG)
		fFontSize = m_defaultStyle.fFontSize;
	else
		return FALSE;
	return TRUE;
}

FS_BOOL	CPDF_RichTextXML::getColor(FS_ARGB& color)
{
	if (m_TextBlocks.GetSize()>0)
		color = 0xFF000000 | m_TextBlocks.GetAt(0)->style.color;
	else if (m_defaultStyle.nPropertyUsed & STYLE::TEXT_COLOR_FLAG)
		color = 0xFF000000 | m_defaultStyle.color;
	else
		return FALSE;
	return TRUE;
}

//void CPDF_RichTextXML::XMLToFXEdit(IFX_Edit* pEdit, IFX_Edit_FontMap* pFontProv, FX_BOOL bAutoSetAgin)
//{
//	IPDF_VariableText* pVT = pEdit->GetVariableText();
//
////1. set font size
//	pEdit->SetFontSize(m_defaultStyle.fFontSize);
//	
//	CPVT_Section sp;
//	CPVT_WordPlace old = pVT->GetIterator()->GetAt();
//	pVT->GetIterator()->SetAt(0);
//	pVT->GetIterator()->GetSection(sp);
//
//	sp.WordProps.fFontSize = m_defaultStyle.fFontSize;
//
//	pVT->GetIterator()->SetSection(sp);
//	pVT->GetIterator()->SetAt(old);
//
////2. set font properties.
//	for (FX_INT32 iText = 0, cText = m_TextBlocks.GetSize(); iText < cText; iText++)
//	{
//		TEXT_BLOCK* tb = m_TextBlocks[iText];
//
//		FX_BOOL bItalic	= tb->style.wsFontStyle.CompareNoCase(L"italic") == 0;
//		FX_BOOL bBord = tb->style.wsFontWeight.CompareNoCase(L"bold") == 0||
//			tb->style.wsFontWeight.CompareNoCase(L"700") == 0;
//
//		FX_INT32 nFontIndex = -1, nCharSet = DEFAULT_CHARSET; 
//		FX_FLOAT fFontHeight = 0, fLineSpace = 0;
//
//		nCharSet = pFontProv->CharSetFromUnicode(tb->text.GetAt(0), 0);
//		nFontIndex = pFontProv->GetWordFontIndex(tb->text.GetAt(0), nCharSet, -1);
//
//		if (nFontIndex == -1)
//			nFontIndex = 0;
//
//		CFX_Font* pFont = &pFontProv->GetPDFFont(nFontIndex)->m_Font;
//		if (pFont == NULL) return;
//
//		fFontHeight = FX_FLOAT(pFont->GetAscent() - pFont->GetDescent()) * tb->style.fFontSize / 1000.0f;
//		fLineSpace = tb->style.fLineHeight> fFontHeight ? tb->style.fLineHeight - fFontHeight : 0;
//
//		FX_INT32 nWordStyle = 0;
//		if (bItalic)
//			nWordStyle |= PVTWORD_STYLE_ITALIC;
//	
//		if (bBord)
//			nWordStyle |= PVTWORD_STYLE_BOLD;
//
//		if (tb->style.wsTextDecoration.Find(L"underline") != -1)
//			nWordStyle |= PVTWORD_STYLE_UNDERLINE;
//		if (tb->style.wsTextDecoration.Find(L"line-through") != -1)
//			nWordStyle |= PVTWORD_STYLE_CROSSOUT;
//		if (tb->style.wsTextDecoration.Find(L"double word") != -1||
//			tb->style.wsTextDecoration.Find(L"word") != -1||			
//			tb->style.wsTextDecoration.Find(L"double") != -1)	
//			nWordStyle |= PVTWORD_STYLE_UNDERLINE;					
//
//		FX_INT32 nAlignment = 0;
//		if (tb->style.wsTextAlign.CompareNoCase(L"left") == 0 )
//			nAlignment = 0;
//		else if (tb->style.wsTextAlign.CompareNoCase(L"center") == 0)
//			nAlignment = 1;
//		else if (tb->style.wsTextAlign.CompareNoCase(L"right") == 0)
//			nAlignment = 2;
//	
//		CPVT_SecProps sp(fLineSpace, 0.0f, nAlignment);
//		CPVT_WordProps wp(nFontIndex,
//			tb->style.fFontSize, 
//			tb->style.color, 
//			tb->style.nScriptType, 
//			nWordStyle, 
//			tb->style.fLetterSpacing, 
//			(FX_INT32)tb->style.fXFAFontHorizontalScale);
//
//			if (tb->p != 0 || tb->text == L"\n" || iText == 0)
//				pEdit->InsertText(tb->text, nCharSet, &sp, &wp);
//			else
//				pEdit->InsertText(tb->text, nCharSet, NULL, &wp);
//	}
//	
//	if (m_TextBlocks.GetSize() <= 0)
//	{
//		pEdit->EnableUndo(FALSE);
//		if (bAutoSetAgin && m_Beginalign != 1)
//			pEdit->SetRichTextAlignment(m_bCurrentRTL ? 2:0);	
//	}
//
//	pEdit->Paint();
//}