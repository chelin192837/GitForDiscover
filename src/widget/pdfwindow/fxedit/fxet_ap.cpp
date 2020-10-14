#include "../../../inc/fs_precompilers.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)

#include "../include/fxedit/fxet_stub.h"
#include "../include/fxedit/fx_edit.h"
#include "../include/fxedit/fxet_edit.h"

CFX_ByteString GetPDFWordString(IFX_Edit_FontMap * pFontMap, FX_INT32 nFontIndex, FX_WORD Word, FX_WORD SubWord) 
{

	CFX_ByteString sWord;

	if (!pFontMap)
	{
		return sWord;
	}

	if (CPDF_Font * pPDFFont = pFontMap->GetPDFFont(nFontIndex))
	{
		if (SubWord > 0)
		{
			Word = SubWord;		
		}
		else
		{
			FX_DWORD dwCharCode = -1;

			if (pPDFFont->IsUnicodeCompatible())
				dwCharCode = pPDFFont->CharCodeFromUnicode(Word);
			else
				dwCharCode = pFontMap->CharCodeFromUnicode(nFontIndex, Word);

			if (dwCharCode > 0 )
			{
				pPDFFont->AppendChar(sWord, dwCharCode);
				return sWord;
			}
		}

		pPDFFont->AppendChar(sWord, Word);
	}

	return sWord;
}

static CFX_ByteString GetPDFRenderString(const CFX_ByteString & strWords, FX_BOOL bIsEmbedded = FALSE)
{
	if (strWords.GetLength() > 0)
		return PDF_EncodeString(strWords, bIsEmbedded) + " Tj\n";

	return "";
}

static CFX_ByteString GetFontSetString(IFX_Edit_FontMap * pFontMap, FX_INT32 nFontIndex, FX_FLOAT fFontSize)
{
	CFX_ByteTextBuf sRet;

	if (pFontMap)
	{
		CFX_ByteString sFontAlias = pFontMap->GetPDFFontAlias(nFontIndex);

		if (sFontAlias.GetLength() > 0 && fFontSize > 0 )
			sRet << "/" << sFontAlias << " " << fFontSize << " Tf\n";
	}

	return sRet.GetByteString();
}

CFX_ByteString IFX_Edit::GetEditAppearanceStream(IFX_Edit* pEdit, const CPDF_Point & ptOffset, 
												 const CPVT_WordRange * pRange /* = NULL*/, FX_BOOL bContinuous/* = TRUE*/, FX_WORD SubWord/* = 0*/)
{
	CFX_ByteTextBuf sEditStream, sWords;

	CPDF_Point ptOld(0.0f,0.0f),ptNew(0.0f,0.0f);
	FX_INT32 nCurFontIndex = -1;

	if (IFX_Edit_Iterator* pIterator = pEdit->GetIterator())
	{
		if (pRange)
			pIterator->SetAt(pRange->BeginPos);
		else
			pIterator->SetAt(0);

		CPVT_WordPlace oldplace;

		while (pIterator->NextWord())
		{
			CPVT_WordPlace place = pIterator->GetAt();

			if (pRange && place.WordCmp(pRange->EndPos) > 0) break;

			if (bContinuous)			
			{
				if (place.LineCmp(oldplace) != 0)
				{
					if (sWords.GetSize() > 0)
					{
						sEditStream << GetPDFRenderString(sWords.GetByteString());
						sWords.Clear();
					}

					CPVT_Word word;
					if (pIterator->GetWord(word))
					{
						ptNew = CPDF_Point(word.ptWord.x + ptOffset.x, word.ptWord.y + ptOffset.y);
					}
					else
					{
						CPVT_Line line;
						pIterator->GetLine(line);
						ptNew = CPDF_Point(line.ptLine.x + ptOffset.x, line.ptLine.y + ptOffset.y);
					}

					if (ptNew.x != ptOld.x || ptNew.y != ptOld.y)
					{
						sEditStream << ptNew.x - ptOld.x << " " << ptNew.y - ptOld.y << " Td\n";

						ptOld = ptNew;
					}
				}

				CPVT_Word word;
				if (pIterator->GetWord(word))
				{	
					if (word.nFontIndex != nCurFontIndex)
					{
						if (sWords.GetSize() > 0)
						{
							sEditStream << GetPDFRenderString(sWords.GetByteString());
							sWords.Clear();
						}
						sEditStream << GetFontSetString(pEdit->GetFontMap(),word.nFontIndex,word.fFontSize);
						nCurFontIndex = word.nFontIndex;
					}

					sWords << GetPDFWordString(pEdit->GetFontMap(),nCurFontIndex,word.Word,SubWord);
				}

				oldplace = place;
			}
			else
			{
				CPVT_Word word;
				if (pIterator->GetWord(word))
				{
					ptNew = CPDF_Point(word.ptWord.x + ptOffset.x, word.ptWord.y + ptOffset.y);

					if (ptNew.x != ptOld.x || ptNew.y != ptOld.y)
					{
						sEditStream << ptNew.x - ptOld.x << " " << ptNew.y - ptOld.y << " Td\n";
						ptOld = ptNew;
					}	

					if (word.nFontIndex != nCurFontIndex)
					{
						sEditStream << GetFontSetString(pEdit->GetFontMap(),word.nFontIndex,word.fFontSize);
						nCurFontIndex = word.nFontIndex;
					}

					sEditStream << GetPDFRenderString(GetPDFWordString(pEdit->GetFontMap(),nCurFontIndex,word.Word,SubWord));
				}
			}
		}

		if (sWords.GetSize() > 0)
		{
			sEditStream << GetPDFRenderString(sWords.GetByteString());
			sWords.Clear();
		}
	}

	CFX_ByteTextBuf sAppStream;
	if (sEditStream.GetSize() > 0)
	{
		FX_INT32 nHorzScale = pEdit->GetHorzScale();
		if (nHorzScale != 100)
		{
			sAppStream << nHorzScale << " Tz\n";
		}

		FX_FLOAT fCharSpace = pEdit->GetCharSpace();
		if (!FX_EDIT_IsFloatZero(fCharSpace))
		{
			sAppStream << fCharSpace << " Tc\n";
		}

		sAppStream << sEditStream;
	}	

	return sAppStream.GetByteString();
}

static FX_BOOL AddUnline(CFX_RectArray& underLines, CFX_FloatRect& rcUnderline, FX_BOOL bVer)
{
	if ((FALSE == bVer) ? (rcUnderline.left != rcUnderline.right) : (rcUnderline.top != rcUnderline.bottom))
	{
		underLines.Add(rcUnderline);
		rcUnderline.InitRect(0 ,0);
		return TRUE;
	}
	return FALSE;
}

static CFX_ByteString GetUnderlineOrCrossoutString(CPDF_Rect rect, FX_COLORREF color, FX_COLORREF old)
{
	CFX_ByteString bsUnderline;
	FX_FLOAT fY = (rect.top + rect.bottom) / 2;
	bsUnderline.Format("%f w\n%f %f m\n%f %f l\nS\n", FXSYS_fabs(rect.top - rect.bottom), rect.left, fY, rect.right, fY);
	if (color != old)
	{
		CFX_ByteString bsColor;
		bsColor.Format(" %f %f %f RG\n", ((FX_FLOAT)FXSYS_GetRValue(color)) / 255.0f, ((FX_FLOAT)FXSYS_GetGValue(color)) / 255.0f, ((FX_FLOAT)FXSYS_GetBValue(color)) / 255.0f);
		bsUnderline = bsColor + bsUnderline;
	}
	return bsUnderline;
}

static CFX_ByteString GetColorString(FX_COLORREF color)
{
	CFX_ByteString bsColor;
	bsColor.Format(" %f %f %f rg\n", ((FX_FLOAT)FXSYS_GetRValue(color)) / 255.0f, ((FX_FLOAT)FXSYS_GetGValue(color)) / 255.0f, ((FX_FLOAT)FXSYS_GetBValue(color)) / 255.0f);
	return bsColor;
}

CPDF_Font* GetPDFfont(const CFX_DWordArray& strWords, IFX_Edit_FontMap* pFontMap, FX_INT32 nCurFontIndex, CFX_EditFontArray* fontindexlist, FX_BOOL* pIsEmbedded=NULL)
{
	if (strWords.GetSize() <=0 )
		return NULL;

	CPDF_Font* pPDFFont = pFontMap->GetPDFFont(nCurFontIndex);
	if (fontindexlist)
	{
		FX_BOOL bFind = 0;
		for (FX_INT32 i = 0;i < fontindexlist->GetSize();i++)
		{
			if (fontindexlist->GetDataPtr(i)->pFont == pPDFFont)
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			EDIT_FONT_DATA tmpData;
			tmpData.fontAName = pFontMap->GetPDFFontAlias(nCurFontIndex);
			tmpData.pFont = pPDFFont;
			fontindexlist->Add(tmpData);
		}
	}
	if (pIsEmbedded)
	{
		CPDF_Font* pPDFFontTmp = pFontMap->GetPDFFont(nCurFontIndex);
		if (pPDFFontTmp)
			*pIsEmbedded = pPDFFontTmp->IsEmbedded();
	}

	return pPDFFont;
}

CFX_ByteString GetWordString(const CFX_DWordArray& strWords,CPDF_Font* pPDFFont)
{
	if(strWords.GetSize() <= 0)
		return "";
	CFX_ByteString bsWord;
	FX_BOOL bUnicodeFont = (pPDFFont->IsUnicodeCompatible() || pPDFFont->IsEmbedded());
	for(FX_INT32 i = 0;i < strWords.GetSize(); i++)
	{
		FX_DWORD word = strWords.GetAt(i);
		if(bUnicodeFont)
		{
			FX_DWORD charcode = pPDFFont->CharCodeFromUnicode((FX_WCHAR)word);
			if(charcode < 0)
				continue;
			CFX_ByteString bsTemp;
			pPDFFont->AppendChar(bsTemp,charcode);
			bsWord += bsTemp;
		}
		else
			bsWord += (FX_CHAR)word;
	}
	return bsWord;
}

extern CPDF_Rect GetUnderLineRect(const CPVT_Word& word);
extern CPDF_Rect GetCrossoutRect(const CPVT_Word& word);

void GetUnderLinesOrCrossoutByLine(const CPVT_Word& word, CFX_FloatRect& lineRect, FX_BOOL bVer, FX_INT32 nWordtype)
{
	CPDF_Rect rc;
	switch(nWordtype)
	{
	case PVTWORD_STYLE_UNDERLINE:
		rc = GetUnderLineRect(word);
		break;
	case PVTWORD_STYLE_CROSSOUT:
		rc = GetCrossoutRect(word);
		break;
	default:
		break;
	}

	FX_FLOAT left = lineRect.left;
	FX_FLOAT right = lineRect.right;
	if (bVer)
	{
		left = lineRect.top;
		right = lineRect.bottom;
	}
	if (left == right)
		lineRect = rc;
	else
		lineRect.Union(rc);
}

CFX_ByteString IFX_Edit::GetRichEditAppearanceStream(IFX_Edit* pEdit, const CPDF_Point & ptOffset, 
	const CPVT_WordRange * pRange, FX_BOOL bContinuous, CFX_EditFontArray* fontindexlist)
{
	CFX_ByteTextBuf bsEditStream;

	CPDF_Point ptOld(0.0f,0.0f), ptNew(0.0f,0.0f);

	FX_INT32 nCurFontIndex = -1, nCurBidiOrder = -1;
	FX_FLOAT fCurFontSize = 0;

	CPVT_WordProps wp;
	CFX_RectArray underlines, crossouts;
	CFX_ArrayTemplate<FX_COLORREF> underlineColors, crossoutColors;

	CFX_DWordArray wsWords;

	CPDF_Font* pPDFFont = NULL;
	FX_BOOL bIsEmbedded = FALSE;

	if (IFX_Edit_Iterator* pIterator = pEdit->GetIterator())
	{
		if (pRange)
			pIterator->SetAt(pRange->BeginPos);
		else
			pIterator->SetAt(0);

		CPVT_WordPlace oldplace;

		CPDF_Rect rcUnderline(0, 0, 0, 0), rcCrossout(0, 0, 0, 0);
		FX_COLORREF rgbUnderline, rgbCrossout;

		while (pIterator->NextWord())
		{
			CPVT_WordPlace place = pIterator->GetAt();
			if (pRange && place.WordCmp(pRange->EndPos) > 0) break;

			if (bContinuous)			
			{
				CPVT_Word word;
				FX_BOOL bGetWord = pIterator->GetWord(word);

				word.WordProps.fFontSize = word.fFontSize;	

				if (place.LineCmp(oldplace) != 0 || word.WordProps.nScriptType != wp.nScriptType || word.WordProps.fCharSpace != wp.fCharSpace 
					|| word.WordProps.nHorzScale != wp.nHorzScale || word.WordProps.dwWordColor != wp.dwWordColor
					|| word.WordProps.nFontIndex != nCurFontIndex|| word.WordProps.fFontSize != fCurFontSize )
				{
					if (wsWords.GetSize() > 0)
					{	
						pPDFFont = GetPDFfont(wsWords, pEdit->GetFontMap(), nCurFontIndex, fontindexlist, &bIsEmbedded);
						bsEditStream << GetPDFRenderString(GetWordString(wsWords,pPDFFont), bIsEmbedded);
						wsWords.RemoveAll();

						if (AddUnline(underlines,rcUnderline,FALSE))
						{
							underlineColors.Add(rgbUnderline);
							rgbUnderline = 0;
						}

						if (!rcCrossout.IsEmpty())
						{
							crossouts.Add(rcCrossout);
							rcCrossout.InitRect(0, 0);
							crossoutColors.Add(rgbCrossout);
							rgbCrossout = 0;
						}
					}
					if (place.SecCmp(oldplace))
					{
						nCurFontIndex = wp.nFontIndex;
						fCurFontSize = wp.fFontSize;
					}

					if (bGetWord)
					{
						if (word.WordProps.nFontIndex != nCurFontIndex || word.WordProps.fFontSize != fCurFontSize)
						{
							bsEditStream << GetFontSetString(pEdit->GetFontMap(), word.nFontIndex, word.fFontSize);
						}

						if (word.WordProps.dwWordColor != wp.dwWordColor)
						{
							bsEditStream << GetColorString(word.WordProps.dwWordColor);
						}

						ptNew = CPDF_Point(word.ptWord.x + ptOffset.x, word.ptWord.y + ptOffset.y);

						if (word.WordProps.nHorzScale != wp.nHorzScale)
							bsEditStream << word.WordProps.nHorzScale<<" Tz\n";

						if (word.WordProps.fCharSpace != wp.fCharSpace)
							bsEditStream << word.WordProps.fCharSpace<<" Tc\n";
					}
					else
					{
						CPVT_Line line;
						pIterator->GetLine(line);
						ptNew = CPDF_Point(line.ptLine.x + ptOffset.x, line.ptLine.y + ptOffset.y);
					}
					bsEditStream << ptNew.x - ptOld.x << " " << ptNew.y - ptOld.y << " Td\n";

					ptOld = ptNew;
				}

				if (bGetWord)
				{
					if ((word.WordProps.nWordStyle & PVTWORD_STYLE_UNDERLINE) != (wp.nWordStyle & PVTWORD_STYLE_UNDERLINE))
					{
						if (AddUnline(underlines,rcUnderline,FALSE))
						{
							underlineColors.Add(rgbUnderline);
							rgbUnderline = 0;
						}
					}
					if ((word.WordProps.nWordStyle & PVTWORD_STYLE_CROSSOUT) != (wp.nWordStyle & PVTWORD_STYLE_CROSSOUT))
					{
						if (!rcCrossout.IsEmpty())
						{
							crossouts.Add(rcCrossout);
							rcCrossout.InitRect(0, 0);
							crossoutColors.Add(rgbCrossout);
							rgbCrossout = 0;
						}
					}
					wsWords.Add(word.Word);
					if (word.WordProps.nWordStyle & PVTWORD_STYLE_UNDERLINE)
					{
						GetUnderLinesOrCrossoutByLine(word,rcUnderline, FALSE, PVTWORD_STYLE_UNDERLINE);
						rgbUnderline = word.WordProps.dwWordColor;
					}

					if (word.WordProps.nWordStyle & PVTWORD_STYLE_CROSSOUT)
					{
						GetUnderLinesOrCrossoutByLine(word,rcCrossout, FALSE, PVTWORD_STYLE_CROSSOUT);
						rgbCrossout = word.WordProps.dwWordColor;
					}

					nCurFontIndex = word.WordProps.nFontIndex;
					fCurFontSize = word.WordProps.fFontSize;
					wp = word.WordProps;
				}

				oldplace = place;
			}
			else
			{
				CPVT_Word word;
				if (pIterator->GetWord(word))
				{
					if (word.WordProps.dwWordColor != wp.dwWordColor)
					{
						bsEditStream << GetColorString(wp.dwWordColor);
						wp = word.WordProps;
					}

					ptNew = CPDF_Point(word.ptWord.x + ptOffset.x, word.ptWord.y + ptOffset.y);

					if (ptNew.x != ptOld.x || ptNew.y != ptOld.y)
					{
						bsEditStream << ptNew.x - ptOld.x << " " << ptNew.y - ptOld.y << " Td\n";
						ptOld = ptNew;
					}	

					if (word.nFontIndex != nCurFontIndex)
					{
						bsEditStream << GetFontSetString(pEdit->GetFontMap(), word.nFontIndex, word.fFontSize);
						nCurFontIndex = word.nFontIndex;
						fCurFontSize = word.fFontSize;
					}
					if (wsWords.GetSize() > 0)
					{
						pPDFFont = GetPDFfont(wsWords,pEdit->GetFontMap(), nCurFontIndex, fontindexlist, &bIsEmbedded);
						bsEditStream << GetPDFRenderString(GetWordString(wsWords,pPDFFont), bIsEmbedded);
						wsWords.RemoveAll();
					}
				}
			}
		}

		if (wsWords.GetSize() > 0)
		{
			pPDFFont = GetPDFfont(wsWords,pEdit->GetFontMap(),nCurFontIndex,fontindexlist,&bIsEmbedded);
			CFX_ByteString bsWord = GetWordString(wsWords, pPDFFont);
			bsEditStream << GetPDFRenderString(bsWord, bIsEmbedded);
			wsWords.RemoveAll();

			if (AddUnline(underlines,rcUnderline,FALSE))
			{
				underlineColors.Add(rgbUnderline);
				rgbUnderline = 0;
			}

			if (!rcCrossout.IsEmpty())
			{
				crossouts.Add(rcCrossout);
				rcCrossout.InitRect(0, 0);
				crossoutColors.Add(rgbCrossout);
				rgbCrossout = 0;
			}
		}
	}

	CFX_ByteTextBuf bsAppStream;
	if (bsEditStream.GetSize() > 0)
		bsAppStream << bsEditStream;

	for (FX_INT32 nUnderline = 0; nUnderline < underlines.GetSize(); nUnderline++)
		bsAppStream << GetUnderlineOrCrossoutString(underlines.GetAt(nUnderline), underlineColors[nUnderline], nUnderline == 0 ? underlineColors[nUnderline] + 255 : underlineColors[nUnderline - 1]);

	for (FX_INT32 nCrossout = 0; nCrossout < crossouts.GetSize(); nCrossout++)
		bsAppStream << GetUnderlineOrCrossoutString(crossouts.GetAt(nCrossout), crossoutColors[nCrossout], nCrossout == 0 ? crossoutColors[nCrossout] + 255 : crossoutColors[nCrossout - 1]);

	return bsAppStream.GetByteString();
}

CFX_ByteString IFX_Edit::GetSelectAppearanceStream(IFX_Edit* pEdit, const CPDF_Point & ptOffset, 
							const CPVT_WordRange * pRange /*= NULL*/)
{
	CFX_ByteTextBuf sRet;

	if (pRange && pRange->IsExist())
	{
		if (IFX_Edit_Iterator* pIterator = pEdit->GetIterator())
		{
			pIterator->SetAt(pRange->BeginPos);
			
			while (pIterator->NextWord())
			{
				CPVT_WordPlace place = pIterator->GetAt();

				if (pRange && place.WordCmp(pRange->EndPos) > 0) break;				

				CPVT_Word word;
				CPVT_Line line;
				if (pIterator->GetWord(word) && pIterator->GetLine(line))
				{			
					CPDF_Rect rcWordSel = CPDF_Rect(word.ptWord.x,line.ptLine.y + line.fLineDescent,
							word.ptWord.x+word.fWidth,line.ptLine.y + line.fLineAscent);

					sRet << word.ptWord.x + ptOffset.x << " " << line.ptLine.y + line.fLineDescent
						<< " " << word.fWidth << " " << line.fLineAscent - line.fLineDescent << " re\nf\n";				
				}
			}
		}
	}

	return sRet.GetByteString();
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)