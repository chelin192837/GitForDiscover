#include "../../include/common.h"

#include "../fxcore.h"
#include "inc/fs_wmtext.h"

//*****************************************************************************
//* text watermark info
//*****************************************************************************
CFS_TextWatermark::CFS_TextWatermark()
				: m_pRtfBreak(NULL)
//				, m_pFont(NULL)
				, m_pFontMgr(NULL)
				, m_pWidths(NULL)
				, m_iChars(0)
				, m_pCharPos(NULL)
				, m_iCount(0)
				, m_bUnderline(FALSE)
				, m_color(0)
				, m_fFontSize(24)
				, m_fLinePos(0)
				, m_fLineStart(0)
				, m_Fonts(16)
				, m_blockAlignment(FX_RTFLINEALIGNMENT_Center)
				, m_fMaxLineLength(0)
{

}

CFS_TextWatermark::~CFS_TextWatermark()
{
	if (m_pWidths)		FX_Free(m_pWidths);
	if (m_pCharPos)		FX_Free(m_pCharPos);
//	if (m_pFont)		m_pFont->Release();
	if (m_pFontMgr)		m_pFontMgr->Release();
	if (m_pRtfBreak)	m_pRtfBreak->Release();
	m_Fonts.RemoveAll();
}

void CFS_TextWatermark::Init(FX_FLOAT fFontSize, FX_ARGB color, FX_BOOL bUnderline,FX_FLOAT linespace,FX_DWORD blockAlignment)
{
	if (fFontSize > 1) m_fFontSize = fFontSize;
	m_color = color;
	m_bUnderline = bUnderline;
	m_linespace = linespace;

	m_blockAlignment = blockAlignment;

//	if (m_pFont) m_pFont->Release();

	m_pFontMgr = IFX_FontMgr::Create(FX_GetDefFontEnumerator());
//	m_pFont = IFX_Font::LoadFont(pFont, m_pFontMgr);

//	FXSYS_assert(m_pFont);

	if (m_pRtfBreak)
		m_pRtfBreak->Reset();
	else
		m_pRtfBreak = CreateRTFBreak();
//	m_pTxtBreak->SetFont(m_pFont);
}

IFX_RTFBreak* CFS_TextWatermark::CreateRTFBreak()
{
	IFX_RTFBreak *pRTFBreak = IFX_RTFBreak::Create(0);
	if (!pRTFBreak) return NULL;

	pRTFBreak->SetLayoutStyles(FX_RTFLAYOUTSTYLE_ExpandTab);
	if(m_fFontSize < 106)    // because 107*1000*20000 > 0x7fffffff 
		pRTFBreak->SetLineWidth(0, m_fFontSize * 1000);
	else
		pRTFBreak->SetLineWidth(0, 106 * 1000);

	pRTFBreak->SetFontSize(m_fFontSize);
//	pTxtBreak->SetFont(m_pFont);

	return pRTFBreak;
}

FX_BOOL CFS_TextWatermark::FillText(CPDF_Form *pForm, CFX_WideString wText, /*IFX_FontEx* pFontEx, */CPDF_Font* pPDFFont)
{
	if (!pForm || wText.GetLength()<1) return FALSE;
	if (!m_pRtfBreak) return FALSE; 
	FS_FLOAT bstart = 0.0f;

	FX_BOOL bSavePoint = TRUE;
	CPDF_Point ptStart(0, 0);
	FX_FLOAT fWidth = 0;
	FX_DWORD dwStatus;
	FX_INT32 iLength = wText.GetLength();
	m_pRtfBreak->SetLineBreakChar(0x0a);
	
	//int fontarrayindex = -1, fontarraysize = fontarray->GetSize();
	//if(!fontarraysize) return FALSE;
	IFX_Font* pIFXFont = NULL;
	//CPDF_Font* pPDFFont;
	CFX_PtrList pIFXFontList;
	FX_INT32	iLineObjCount = 0;
	FX_BOOL  breakFlag = FALSE;
	CFX_DWordArray actCharCodeArray;
	//CFX_CMapDWordToDWord* pTempReplaceCharCode = FX_NEW CFX_CMapDWordToDWord;
	//if (NULL == pTempReplaceCharCode) return FALSE;
	for (FX_INT32 i = 0, mapIndex = 0; i < iLength; i++)
	{
		//while(fontarrayindex < fontarraysize && (fontarrayindex < 0 || i >= (int)fontarray->GetAt(fontarrayindex).nEndPos))
		//	fontarrayindex++;
		//if (fontarrayindex < fontarraysize)
		//{
		//	if(i == (int)fontarray->GetAt(fontarrayindex).nStartPos)
		//	{
		//		FSPDF_WATERMARK_TEXTPIECE_FONTINFO fontInfo = fontarray->GetAt(fontarrayindex);
		//		CFSCRT_LTFont* pLTFont = fontInfo.pFont;

		//		FS_RESULT ret = pdfFonts->IsPDFFontAvailable(*pLTFont);
		//		if ( FSCRT_ERRCODE_NOTFOUND == ret)
		//			ret = pdfFonts->ST_AddPDFFont(*pLTFont); //should we rollback?
		//		else if (ret == FSCRT_ERRCODE_ERROR)
		//			ret = pdfFonts->ST_RecoverPDFFont(*pLTFont);
		//		if (ret != FSCRT_ERRCODE_SUCCESS && ret != FSCRT_ERRCODE_NOTFOUND)
		//		{
		//			m_pRtfBreak->Reset();
		//			delete pTempReplaceCharCode;
		//			return FALSE;
		//		}
		//		ret = pdfFonts->ST_GetPDFFont(*pLTFont, pPDFFont);
		//		if (ret != FSCRT_ERRCODE_SUCCESS)
		//		{
		//			m_pRtfBreak->Reset();
		//			delete pTempReplaceCharCode;
		//			return FALSE;
		//		}

		//		FS_BOOL bAddCharcode = TRUE;
		//		if (FSPDF_WMCHAREMBTYPE_SURROGATE == fontInfo.iEmbType)
		//		{// If current text piece are surrogate chars, embed the font, 
		//		 // prepare replaced char for each surrogate chars, store the actual charcode after embedding and get the embedded CPDF_Font.
		//			// Embed the font and unicodes of surrogate chars.
		//			IFX_FMEmbFont* pFXEmbFont = FXFM_CreateEmbFont(pForm->m_pDocument,fontInfo.pFont->m_pSTFont->GetFont());
		//			if (pFXEmbFont)
		//			{
		//				CFX_DWordArray tempCharcodes;
		//				for (int j=fontInfo.nStartPos; j<fontInfo.nEndPos; j++)
		//					tempCharcodes.Add(pUTF16LEWideString[j]);
		//				FX_DWORD nCharCodes = tempCharcodes.GetSize();
		//				FX_DWORD* pGlyph = (FX_DWORD*)FX_Alloc(FX_DWORD, nCharCodes);
		//				if (!pGlyph)
		//				{
		//					m_pRtfBreak->Reset();
		//					pFXEmbFont->Release();
		//					delete pTempReplaceCharCode;
		//					return FALSE;
		//				}
		//				FX_DWORD nEmbCharCodes = pFXEmbFont->AddUnicodes(tempCharcodes.GetData(),nCharCodes, pGlyph, nCharCodes);
		//				tempCharcodes.RemoveAll();
		//				// Store the embedded charcode for surrogate char, and prepare replaced char for each surrogate chars.
		//				CPDF_Font* pEMBPDFFont = pFXEmbFont->GetPDFFont();
		//				for (int j=fontInfo.nStartPos, k=0; j<fontInfo.nEndPos; j=j+2, k++)
		//				{
		//					if (nEmbCharCodes>0)
		//					{
		//						if (k>=nEmbCharCodes) break;
		//						//Store the embedded charcode for surrogate char.
		//						actCharCodeArray.Add(pGlyph[k]);
		//						//Find a replace char in original CPDF_Font, and this char will be appended to m_pRtfBreak -- just for getting right position info for surrogate char.
		//						// ATTENTION: during this loop pPDFFont is still the original CPDF_Font -- no-embed one.
		//						FX_RECT charBBox;
		//						pEMBPDFFont->GetCharBBox(pGlyph[k],charBBox);
		//						FX_INT32 iExpectCharWidth = charBBox.Width();
		//						FX_INT32 iTolerrance = 1;
		//						for (FX_WCHAR wch = 0; wch<=0xD7FF; wch++)
		//						{
		//							//Use original CPDF_Font to find a replace char.
		//							FX_RECT tempBBox;
		//							FX_DWORD iCharcode = pPDFFont->CharCodeFromUnicode(wch);
		//							pPDFFont->GetCharBBox(iCharcode, tempBBox);
		//							FX_INT32 tempWidth = tempBBox.Width();
		//							if (tempWidth>= iExpectCharWidth-iTolerrance && tempWidth<=iExpectCharWidth+iTolerrance)
		//							{
		//								// If find a character for replacing, this character needs to be embedded as well.
		//								pTempReplaceCharCode->SetAt(j, wch);
		//								tempCharcodes.Add(iCharcode);
		//								FX_DWORD nCharCodes2 = tempCharcodes.GetSize();
		//								FX_DWORD* pGlyph2 = (FX_DWORD*)FX_Alloc(FX_DWORD, nCharCodes2);
		//								if (!pGlyph2)
		//								{
		//									m_pRtfBreak->Reset();
		//									FX_Free(pGlyph);
		//									pFXEmbFont->Release();
		//									delete pTempReplaceCharCode;
		//									return FALSE;
		//								}
		//								FX_DWORD nEmbCharCodes = pFXEmbFont->AddUnicodes(tempCharcodes.GetData(),nCharCodes2, pGlyph2, nCharCodes2);
		//								tempCharcodes.RemoveAll();
		//								FX_Free(pGlyph2);
		//								break;
		//							}
		//						}
		//					}
		//					else if (0 == nEmbCharCodes)
		//					{
		//						actCharCodeArray.Add(pUTF16LEWideString[j]);
		//						actCharCodeArray.Add(pUTF16LEWideString[j+1]);
		//					}
		//				}
		//				FX_Free(pGlyph);
		//				pPDFFont = pFXEmbFont->GetPDFFont();
		//				pFXEmbFont->Release();
		//				bAddCharcode = FALSE;
		//			}
 	//			}
		//		if (bAddCharcode)
		//		{
		//			for (int j=fontInfo.nStartPos; j<fontInfo.nEndPos; j++)
		//			{
		//				FX_DWORD dwCharcode = pPDFFont->CharCodeFromUnicode(pUTF16LEWideString[j]);
		//				actCharCodeArray.Add(dwCharcode);
		//			}
		//		}

				//pIFXFont = IFX_Font::LoadFont(&pPDFFont->m_Font, m_pFontMgr);
				////Looks like it's not supported on the ios platform.
				//if(!pIFXFont)
				//{
				//	m_pRtfBreak->Reset();
				//	delete pTempReplaceCharCode;
				//	return FALSE;
				//}
		//if (0 == i)
		//{
			FX_DWORD dwCharcode = pPDFFont->CharCodeFromUnicode(wText[i]);
			actCharCodeArray.Add(dwCharcode);
			pIFXFont = IFX_Font::LoadFont(&pPDFFont->m_Font,m_pFontMgr);
			m_pRtfBreak->SetFont(pIFXFont);

			if(breakFlag)
			{
				FS_INT32 count = pIFXFontList.GetCount();
				if (count > 0)
				{
					for (FS_INT32 j = 0; j != count; j++)
					{
						FX_POSITION posTmp = pIFXFontList.GetHeadPosition();
						if (j % 2 == 0)
							((IFX_Font*)pIFXFontList.GetAt(posTmp))->Release();
						pIFXFontList.RemoveAt(posTmp);
					}
				}
			}
			pIFXFontList.AddTail(pIFXFont);
			pIFXFontList.AddTail(pPDFFont);
		//}
			//}
		//}

		if(!pIFXFont) continue;

		FX_WORD wAppendChar = wText[i];
		FX_DWORD dwValue;
		//if (pTempReplaceCharCode->Lookup(i, dwValue))
		//{
		//	wAppendChar = dwValue;
		//	i++;
		//}
		if ((dwStatus = m_pRtfBreak->AppendChar(wAppendChar)) > FX_RTFBREAK_PieceBreak)
		{
			FX_POSITION pos = pIFXFontList.GetTailPosition();
			CPDF_Font* pPDFFontPrev = (CPDF_Font*)pIFXFontList.GetPrev(pos);
			IFX_Font* pIFXFontPrev = (IFX_Font*)pIFXFontList.GetPrev(pos);
			pIFXFontList.AddTail(pIFXFontPrev->Retain());
			pIFXFontList.AddTail(pPDFFontPrev);

			RetrievePieces(pForm, actCharCodeArray, dwStatus, fWidth, bSavePoint, ptStart, &pIFXFontList, iLineObjCount);
			breakFlag = TRUE;
		}
		else
			breakFlag = FALSE;
	}	
	//delete pTempReplaceCharCode;

	if ((dwStatus = m_pRtfBreak->EndBreak(FX_RTFBREAK_ParagraphBreak)) > FX_RTFBREAK_PieceBreak)
		RetrievePieces(pForm, actCharCodeArray, dwStatus, fWidth, bSavePoint, ptStart, &pIFXFontList, iLineObjCount);

	m_fLinePos += FX_MAX(FX_MIN(m_fFontSize, 5) * 0.1f, 1.0f); //add underline 
	FX_POSITION pos = pForm->GetFirstObjectPosition();

	FX_FLOAT lineLength = 0;
	int index = 0;
	int postion = 0;
	while (pos)
	{
		CPDF_PageObject *pObj = pForm->GetNextObject(pos);
		int size = m_objInLine.GetSize();

		if(size > index )
		{
			if ( postion < m_objInLine.GetAt(index))
			{
				lineLength = m_flineLength.GetAt(index);
			}

			if (postion == m_objInLine.GetAt(index)-1)
			{
				index++;
				postion = -1;
				bstart -=FX_MAX(FX_MIN(m_fFontSize, 5) * 0.1f, 1.0f); //add underline
			}
			
			postion++;
		}

		FX_FLOAT di = 0;
		switch(m_blockAlignment)
		{
		case FX_RTFLINEALIGNMENT_Left:
			di = 0;
			break;
		case FX_RTFLINEALIGNMENT_Right:
			if ( lineLength < m_fMaxLineLength )
			{
				di = m_fMaxLineLength - lineLength;
			}
			break;
		default:
			if ( lineLength < m_fMaxLineLength )
			{
				di = m_fMaxLineLength - lineLength;
				di /= 2.0;
			}
			break;
		}

		switch (pObj->m_Type)
		{
		case PDFPAGE_TEXT:
			{
				CPDF_TextObject *pTextObj = (CPDF_TextObject*)pObj;
				FX_FLOAT fPosY = pTextObj->GetPosY();
				CFX_AffineMatrix mt;
				pTextObj->GetTextMatrix(&mt);
				FX_RECT rt = pTextObj->GetBBox(&mt);
//				FX_FLOAT di = 0;

// 				if ( lineLength < m_fMaxLineLength )
// 				{
// 					di = m_fMaxLineLength - lineLength;
// 					di /= 2.0;
// 				}
				
				FX_FLOAT fPosX = pTextObj->GetPosX() + di; 
				pTextObj->SetPosition(fPosX, m_fLinePos - fPosY);
				//pTextObj->SetPosition(pTextObj->GetPosX(), m_fLinePos - fPosY);
			}
			break;
		case PDFPAGE_PATH:
			{
				CPDF_PathObject *pPathObj = (CPDF_PathObject*)pObj;
				FX_INT32 iFlags[2] = { FXPT_MOVETO, FXPT_LINETO };
//				FX_FLOAT di = 0;
// 				if ( lineLength < m_fMaxLineLength )
// 				{
// 					di = m_fMaxLineLength - lineLength;
// 					di /= 2.0;
// 				}

				for (FX_INT32 i = 0; i < 2; i++)
				{
					FX_FLOAT x = pPathObj->m_Path.GetPointX(i);
					FX_FLOAT y = pPathObj->m_Path.GetPointY(i);
					CPDF_PathData * pPathData = pPathObj->m_Path.GetModify();

					pPathData->SetPoint(i, x +di, m_fLinePos - y, iFlags[i]);
				}
			}
			break;
		}
	}

	CPDF_Rect rtBBox(0, bstart, fWidth, m_fLinePos);
	pForm->m_pFormDict->SetAtRect(FX_BSTRC("BBox"), rtBBox);
	
	CPDF_ContentGenerator cg(pForm);
	cg.GenerateContent();

	return TRUE;
}

FX_BOOL	FSCRT_ST_WM_CharNeedEmbed(FX_DWORD charcode, FX_BSTR bsFontEncoding)
{
	if ((FX_DWORD)-1 == charcode) //unsupported character
		return FALSE;
	
	if (0 == FXSYS_memcmp(FONTENCODING_UNIJIS_UCS2_H, bsFontEncoding.GetCStr(), bsFontEncoding.GetLength()))
	{
		const FX_DWORD cdwCheckCount = sizeof(g_Charcode_UniJISUCS2H)/sizeof(g_Charcode_UniJISUCS2H[0]);
		FX_DWORD dwLowIndex = 0;
		FX_DWORD dwHighIndex = cdwCheckCount - 1;
		FX_DWORD dwMidIndex = 0;
		while (dwLowIndex<dwHighIndex)
		{
			if (charcode == g_Charcode_UniJISUCS2H[dwLowIndex] || charcode == g_Charcode_UniJISUCS2H[dwHighIndex])
				return TRUE;

			dwMidIndex = (dwHighIndex + dwLowIndex)/2;
			FX_DWORD dwMidValue = g_Charcode_UniJISUCS2H[dwMidIndex];
			if (dwMidIndex == dwHighIndex || dwMidIndex == dwLowIndex) break;
			if (charcode > dwMidValue)
				dwLowIndex = dwMidIndex + 1;
			else if (charcode < dwMidValue)
				dwHighIndex = dwMidIndex - 1;
			else //wch1 == dwMidValue
				return TRUE;
		}
	}
	return FALSE;
}

void CFS_TextWatermark::RetrievePieces(CPDF_Form *pForm, CFX_DWordArray& actCharCodeArray, FX_DWORD dwStatus, FX_FLOAT &fMaxWidth, FX_BOOL &bSavePoint, CPDF_Point &pt, CFX_PtrList* pIFXFontList, FX_INT32& iLineObjCount)
{
	if (dwStatus <= FX_TXTBREAK_PieceBreak) return;

	if(m_linespace <=0) m_linespace = 1.5f;
	else
		if(m_linespace >10.0) m_linespace = 10.0f;
	FX_FLOAT fLineHeight = m_fFontSize * m_linespace;

	CFX_RectF rtPiece;
	FX_INT32 iPieces = m_pRtfBreak->CountBreakPieces();

	if (pIFXFontList->GetCount() < iPieces*2) return;
	FX_FLOAT fThisLineMaxLength = 0;
	for (FX_INT32 i = 0; i < iPieces; i++)
	{
		const CFX_RTFPiece *pPiece = m_pRtfBreak->GetBreakPiece(i);
		FX_INT32 iChars = pPiece->GetLength();
		if (!m_pWidths)
			m_pWidths = FX_Alloc(FX_INT32, iChars * sizeof(FX_INT32));
		else if (m_iChars < iChars)
			m_pWidths = FX_Realloc(FX_INT32, m_pWidths, iChars * sizeof(FX_INT32));
		if (m_iChars < iChars) m_iChars = iChars;
		pPiece->GetWidths(m_pWidths);

		CFX_WideString wsText;
		pPiece->GetString(wsText);

		rtPiece.left = pPiece->m_iStartPos / 20000.0f;
		rtPiece.top = m_fLinePos;
		rtPiece.width = pPiece->m_iWidth / 20000.0f;
		rtPiece.height = fLineHeight;

		fThisLineMaxLength += rtPiece.width;
		
		if ( m_fMaxLineLength < rtPiece.width )
		{
			m_fMaxLineLength = rtPiece.width;
		}

		FX_POSITION pos;
		FX_RTFTEXTOBJ tr;
		tr.fFontSize = m_fFontSize;
		tr.iLength = iChars;
		
		pos = pIFXFontList->GetHeadPosition();    //pophead
		tr.pFont = (IFX_Font*)pIFXFontList->GetAt(pos);
		pIFXFontList->RemoveAt(pos);

		tr.pRect = &rtPiece;
		tr.pStr = wsText;
		tr.pWidths = m_pWidths;
		//tr.bSkipSpace = FALSE;
		FX_INT32 iCount = m_pRtfBreak->GetDisplayPos(&tr, NULL);

		if (!m_pCharPos)
			m_pCharPos = FX_Alloc(FXTEXT_CHARPOS, iCount * sizeof(FXTEXT_CHARPOS));
		else if (m_iCount < iCount) 
			m_pCharPos = FX_Realloc(FXTEXT_CHARPOS, m_pCharPos, iCount * sizeof(FXTEXT_CHARPOS));
		if (m_iCount < iCount) 
			m_iCount = iCount;

		CFX_WideString wsForm;
		m_pRtfBreak->GetDisplayPos(&tr, m_pCharPos, FALSE, &wsForm);

		tr.pFont->Release();

		pos = pIFXFontList->GetHeadPosition();    //pophead
		if (iCount>0)
		{
			CPDF_Font* pPDFFont = (CPDF_Font*)pIFXFontList->GetAt(pos);
			CFX_ByteString bsFontEncoding = pPDFFont->GetFontDict()->GetString("Encoding", "");
			// Check each character in current piece to see if it needs to be embedded.
			CFX_DWordArray charcodeArray, embcharcodeArray;
			FX_INT32 iCommonCharCount = 0, iEmbCharCount = 0;
			FXTEXT_CHARPOS* pCommonCharPos = FX_Alloc(FXTEXT_CHARPOS, iCount * sizeof(FXTEXT_CHARPOS));
			if (NULL == pCommonCharPos)
			{
				return ;
			}
			FXTEXT_CHARPOS* pEmbCharPos = FX_Alloc(FXTEXT_CHARPOS, iCount * sizeof(FXTEXT_CHARPOS));
			if (NULL == pEmbCharPos)
			{
				FX_Free(pCommonCharPos);
				return ;
			}
			for (FX_DWORD j=0, iPos=0; j<iChars; j++ )
			{
				FX_DWORD dwCharcode = actCharCodeArray.GetAt(j);
				CFX_WideString wTemp = pPDFFont->UnicodeFromCharCode(dwCharcode);
				if (0x0020 == dwCharcode || 0x000A == dwCharcode || 0x000D == dwCharcode) continue;
				if (0 == bsFontEncoding.Compare(FONTENCODING_UNIJIS_UCS2_H) 
					&& 1 == wTemp.GetLength() && FSCRT_ST_WM_CharNeedEmbed(dwCharcode, FONTENCODING_UNIJIS_UCS2_H))
				{
					embcharcodeArray.Add(dwCharcode);
					FXSYS_memcpy(&pEmbCharPos[iEmbCharCount], &m_pCharPos[iPos], sizeof(FXTEXT_CHARPOS));
					iEmbCharCount++;
				}
				else
				{
					charcodeArray.Add(dwCharcode);
					FXSYS_memcpy(&pCommonCharPos[iCommonCharCount], &m_pCharPos[iPos], sizeof(FXTEXT_CHARPOS));
					iCommonCharCount++;
				}
				iPos++;
			}
			// Add common characters
			FXSYS_memcpy(m_pCharPos, pCommonCharPos, sizeof(FXTEXT_CHARPOS)*iCommonCharCount);
			AddTextToForm(pForm, &charcodeArray, pPDFFont, iLineObjCount);
			FX_Free(pCommonCharPos);

			// Add characters which need to be embedded first.
			IFX_FMEmbFont* pFXEmbFont = FXFM_CreateEmbFont(pForm->m_pDocument, &(pPDFFont->m_Font));
			if (pFXEmbFont)
			{
				FX_DWORD successCount = pFXEmbFont->AddUnicodes(embcharcodeArray.GetData(), embcharcodeArray.GetSize());
				if (successCount>0)
				{
					CPDF_Font* pEMBPDFFont = pFXEmbFont->GetPDFFont();
					for (FX_INT32 k=0; k<iEmbCharCount; k++)
					{
						CFX_WideString wString = pPDFFont->UnicodeFromCharCode(embcharcodeArray.GetAt(k));
						FX_INT32 iLen = wString.GetLength();
						if (1 == iLen)
						{
							FX_DWORD embCharCode = pEMBPDFFont->CharCodeFromUnicode(wString.GetAt(0));
							embcharcodeArray.SetAt(k, embCharCode);
						}
					}
					pPDFFont = pFXEmbFont->GetPDFFont();
				}
				pFXEmbFont->Release();
			}
			FXSYS_memcpy(m_pCharPos, pEmbCharPos, sizeof(FXTEXT_CHARPOS)*iEmbCharCount);
			AddTextToForm(pForm, &embcharcodeArray, pPDFFont, iLineObjCount);
			FX_Free(pEmbCharPos);
		}
		// If iCount = 0, that means m_pRtfBreak cannot prepare the position of characters in current piece.
		// But it's hard to calculate the position in SDK especially when the current characters are the first ones of all.
		// So, currently, not deal with this case when iCount=0 -- the result is that if iCount=0, characters in current piece will all be ignored.
		pIFXFontList->RemoveAt(pos);
		actCharCodeArray.RemoveAt(0, iChars);

		m_fLineStart += rtPiece.width;
		if (m_bUnderline && bSavePoint)
		{
			pt.x = rtPiece.left;
			pt.y = rtPiece.bottom();
			bSavePoint = FALSE;
		}
	}
	if(fThisLineMaxLength)
		m_flineLength.Add(fThisLineMaxLength);
	m_pRtfBreak->ClearBreakPieces();
	fMaxWidth = FX_MAX(fMaxWidth, m_fLineStart);
	if (dwStatus == FX_RTFBREAK_ParagraphBreak)
	{
		m_objInLine.Add(iLineObjCount);
		iLineObjCount = 0;

		m_fLinePos += fLineHeight;
		m_fLineStart = 0;
		if (m_bUnderline && m_pCharPos)
		{
			CPDF_Point ptEnd(rtPiece.right(), rtPiece.bottom());

			CPDF_Point pEnd = ptEnd;
			CPDF_Point pBegin = pt;
			float linewidth = FX_MIN(m_fFontSize, 5) * 0.1f;
			pEnd.y = (m_pCharPos->m_OriginY + linewidth);
			pBegin.y =(m_pCharPos->m_OriginY + linewidth);
			AddLineToForm(pForm, pBegin, pEnd);
		}
		bSavePoint = TRUE;
		m_pRtfBreak->Reset();
	}
}

void CFS_TextWatermark::AddTextToForm(CPDF_Form *pForm, const CFX_DWordArray* charcodeArray, CPDF_Font* pPDFFont, FX_INT32& iLineObjCount)
{
	if (m_pCharPos == NULL) return;

//	CPDF_Font* pFont = GetPDFFont(pForm->m_pDocument, m_pFont);

	FX_FLOAT rgb[3];
	rgb[0] = FXARGB_R(m_color) / 255.0f;
	rgb[1] = FXARGB_G(m_color) / 255.0f;
	rgb[2] = FXARGB_B(m_color) / 255.0f;
	
	CPDF_ColorSpace *pCS = CPDF_ColorSpace::GetStockCS(PDFCS_DEVICERGB);
	FX_POSITION lastPos = pForm->GetLastObjectPosition();
	FX_FLOAT fPos = 0;

	FX_INT32 iCount = charcodeArray->GetSize();
	FX_INT32 deltaX = 0;
	FX_INT32 iCalcDelta = 0;
	FX_INT32 iActAddedCount = 0;
	for (FX_INT32 i = 0, j=0; i < iCount;i++)
	{
		FX_DWORD dwCharCode = charcodeArray->GetAt(i);
		if ((FX_DWORD)-1 == dwCharCode) //unsupported character
			continue;
		//Followings don't have position info:
		// space, \r, \n
		if (0x20 == dwCharCode || 0x0A == dwCharCode || 0x0D == dwCharCode) 
			continue;

		CPDF_TextObject *pTextObj = FX_NEW CPDF_TextObject;
		CPDF_TextStateData *pTextData = pTextObj->m_TextState.GetModify();
		pTextData->m_pFont = pForm->m_pDocument->LoadFont(pPDFFont->GetFontDict());
		pTextData->m_FontSize = m_fFontSize;
		pTextObj->m_ColorState.SetFillColor(pCS, rgb, 3);
		pTextObj->m_ColorState.SetStrokeColor(pCS, rgb, 3);
		pTextObj->SetData(1, &dwCharCode, &fPos, m_pCharPos[j].m_OriginX, m_pCharPos[j].m_OriginY);
		iActAddedCount = ++j;
		lastPos = pForm->InsertObject(lastPos, pTextObj);
	}
	iLineObjCount += iActAddedCount;
}

void CFS_TextWatermark::AddLineToForm(CPDF_Form *pForm, const CPDF_Point &pt1, const CPDF_Point &pt2)
{
	
	CPDF_PathObject *pPathObj = FX_NEW CPDF_PathObject;
	CPDF_PathData* pPathData = pPathObj->m_Path.GetModify();
	pPathData->SetPointCount(2);
	pPathData->SetPoint(0, pt1.x, pt1.y, FXPT_MOVETO);
	pPathData->SetPoint(1, pt2.x, pt2.y, FXPT_LINETO);

	FX_FLOAT rgb[3];
	FS_FLOAT alpha = (FS_FLOAT) (FXARGB_A(m_color) / 255.0f);
	rgb[0] = FXARGB_R(m_color) / 255.0f;
	rgb[1] = FXARGB_G(m_color) / 255.0f;
	rgb[2] = FXARGB_B(m_color) / 255.0f;
	CPDF_ColorSpace *pCS = CPDF_ColorSpace::GetStockCS(PDFCS_DEVICERGB);
	pPathObj->m_ColorState.SetStrokeColor(pCS, rgb, 3);
	CPDF_GeneralStateData *pGeneralStateData = pPathObj->m_GeneralState.GetModify();
	pGeneralStateData->m_StrokeAlpha = pGeneralStateData->m_FillAlpha = alpha;
	pPathObj->m_FillType = 0;
	pPathObj->m_bStroke = TRUE;
	pPathObj->m_Matrix.SetIdentity();
	CFX_GraphStateData *pGraph = pPathObj->m_GraphState.GetModify();
	
	/*CPDF_PathObject *pPathObj = FX_NEW CPDF_PathObject;
	CPDF_PathData* pPathData = pPathObj->m_Path.GetModify();
	pPathData->SetPointCount(2);
	pPathData->SetPoint(0, pt1.x, pt1.y, FXPT_MOVETO);
	pPathData->SetPoint(1, pt2.x, pt2.y, FXPT_LINETO);

	FX_FLOAT rgb[3];
	rgb[0] = FXARGB_R(m_color) / 255.0f;
	rgb[1] = FXARGB_G(m_color) / 255.0f;
	rgb[2] = FXARGB_B(m_color) / 255.0f;
	CPDF_ColorSpace *pCS = CPDF_ColorSpace::GetStockCS(PDFCS_DEVICERGB);
	pPathObj->m_ColorState.SetStrokeColor(pCS, rgb, 3);

	CFX_GraphStateData *pGraph = pPathObj->m_GraphState.GetModify();
	*/

	pGraph->m_LineWidth = 1;

	pPathObj->CalcBoundingBox();

	m_objInLine.Add(1);

	m_flineLength.Add(pt2.x-pt1.x);

	pForm->InsertObject(pForm->GetLastObjectPosition(), pPathObj);
}

