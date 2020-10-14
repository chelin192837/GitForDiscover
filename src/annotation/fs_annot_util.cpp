#include "fs_annot_util.h"
#include "fs_annot_imp.h"
#include "../fs_iconprovider.h"
#include "fs_annoticonprovider.h"
#include "../inc/fs_signatureImp.h"

CPDF_Rect FSAnnot_Util_ClipPageRect(CPDF_Rect rcPage, CPDF_Rect rcChild, FX_BOOL bCut)
{
	CPDF_Rect rcRet = rcChild;
	rcRet.Normalize();

	if (rcPage.Width() < rcChild.Width())
		bCut = TRUE;
	if (rcPage.Height() < rcChild.Height())
		bCut = TRUE;

	if (rcRet.left < rcPage.left)
	{
		rcRet.left = rcPage.left;
		if (!bCut)
			rcRet.right = rcPage.left + rcChild.Width();
	}
	else if (rcRet.right > rcPage.right)
	{
		rcRet.right = rcPage.right;
		if (!bCut)
			rcRet.left = rcPage.right - rcChild.Width();
	}

	if (rcRet.top > rcPage.top)
	{
		rcRet.top = rcPage.top;
		if (!bCut)
			rcRet.bottom = rcPage.top - rcChild.Height();
	}
	else if (rcRet.bottom < rcPage.bottom)
	{
		rcRet.bottom = rcPage.bottom;
		if (!bCut)
			rcRet.top = rcPage.bottom + rcChild.Height();
	}

	return rcRet;
}

FSM_ANNOTTYPE FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(CFX_ByteString bsType)
{
	if (bsType.IsEmpty()) return FSAnnot::e_annotUnknownType;

	if (bsType.Equal(FSPDF_ANNOTTYPE_NOTE))
		return FSAnnot::e_annotNote;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_LINK))
		return FSAnnot::e_annotLink;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_FREETEXT))
		return FSAnnot::e_annotFreeText;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_LINE))
		return FSAnnot::e_annotLine;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_SQUARE))
		return FSAnnot::e_annotSquare;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_CIRCLE))
		return FSAnnot::e_annotCircle;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_POLYGON))
		return FSAnnot::e_annotPolygon;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_POLYLINE))
		return FSAnnot::e_annotPolyLine;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_HIGHLIGHT))
		return FSAnnot::e_annotHighlight;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_UNDERLINE))
		return FSAnnot::e_annotUnderline;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_SQUIGGLY))
		return FSAnnot::e_annotSquiggly;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_STRIKEOUT))
		return FSAnnot::e_annotStrikeOut;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_STAMP))
		return FSAnnot::e_annotStamp;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_CARET))
		return FSAnnot::e_annotCaret;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_INK))
		return FSAnnot::e_annotInk;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_FILEATTACHMENT))
		return FSAnnot::e_annotFileAttachment;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_SOUND))
		return FSAnnot::e_annotSound;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_MOVIE))
		return FSAnnot::e_annotMovie;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_WIDGET))
		return FSAnnot::e_annotWidget;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_SCREEN))
		return FSAnnot::e_annotScreen;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_PRINTERMARK))
		return FSAnnot::e_annotPrinterMark;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_TRAPNET))
		return FSAnnot::e_annotTrapNet;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_WATERMARK))
		return FSAnnot::e_annotWatermark;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_PSI))
		return FSAnnot::e_annotPSInk;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_3D))
		return FSAnnot::e_annot3D;
	else if (bsType.Equal(FSPDF_ANNOTTYPE_POPUP))
		return FSAnnot::e_annotPopup;

	return FSAnnot::e_annotUnknownType;
}

CFX_ByteString FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(FSM_ANNOTTYPE annotType)
{
	switch(annotType)
	{
	case FSAnnot::e_annotNote:
		return FX_BSTRC(FSPDF_ANNOTTYPE_NOTE);
	case FSAnnot::e_annotLink:
		return FX_BSTRC(FSPDF_ANNOTTYPE_LINK);
	case FSAnnot::e_annotFreeText:
		return FX_BSTRC(FSPDF_ANNOTTYPE_FREETEXT);
	case FSAnnot::e_annotLine:
		return FX_BSTRC(FSPDF_ANNOTTYPE_LINE);
	case FSAnnot::e_annotSquare:
		return FX_BSTRC(FSPDF_ANNOTTYPE_SQUARE);
	case FSAnnot::e_annotCircle:
		return FX_BSTRC(FSPDF_ANNOTTYPE_CIRCLE);
	case FSAnnot::e_annotPolygon:
		return FX_BSTRC(FSPDF_ANNOTTYPE_POLYGON);
	case FSAnnot::e_annotPolyLine:
		return FX_BSTRC(FSPDF_ANNOTTYPE_POLYLINE);
	case FSAnnot::e_annotHighlight:
		return FX_BSTRC(FSPDF_ANNOTTYPE_HIGHLIGHT);
	case FSAnnot::e_annotUnderline:
		return FX_BSTRC(FSPDF_ANNOTTYPE_UNDERLINE);
	case FSAnnot::e_annotSquiggly:
		return FX_BSTRC(FSPDF_ANNOTTYPE_SQUIGGLY);
	case FSAnnot::e_annotStrikeOut:
		return FX_BSTRC(FSPDF_ANNOTTYPE_STRIKEOUT);
	case FSAnnot::e_annotStamp:
		return FX_BSTRC(FSPDF_ANNOTTYPE_STAMP);
	case FSAnnot::e_annotCaret:
		return FX_BSTRC(FSPDF_ANNOTTYPE_CARET);
	case FSAnnot::e_annotInk:
		return FX_BSTRC(FSPDF_ANNOTTYPE_INK);
	case FSAnnot::e_annotFileAttachment:
		return FX_BSTRC(FSPDF_ANNOTTYPE_FILEATTACHMENT);
	case FSAnnot::e_annotSound:
		return FX_BSTRC(FSPDF_ANNOTTYPE_SOUND);
	case FSAnnot::e_annotMovie:
		return FX_BSTRC(FSPDF_ANNOTTYPE_MOVIE);
	case FSAnnot::e_annotWidget:
		return FX_BSTRC(FSPDF_ANNOTTYPE_WIDGET);
	case FSAnnot::e_annotScreen:
		return FX_BSTRC(FSPDF_ANNOTTYPE_SCREEN);
	case FSAnnot::e_annotPrinterMark:
		return FX_BSTRC(FSPDF_ANNOTTYPE_PRINTERMARK);
	case FSAnnot::e_annotTrapNet:
		return FX_BSTRC(FSPDF_ANNOTTYPE_TRAPNET);
	case FSAnnot::e_annotWatermark:
		return FX_BSTRC(FSPDF_ANNOTTYPE_WATERMARK);
	case FSAnnot::e_annotPSInk:
		return FX_BSTRC(FSPDF_ANNOTTYPE_PSI);
	case FSAnnot::e_annot3D:
		return FX_BSTRC(FSPDF_ANNOTTYPE_3D);
	case FSAnnot::e_annotPopup:
		return FX_BSTRC(FSPDF_ANNOTTYPE_POPUP);
	default:
		return FX_BSTRC("");
	}
		
	return FX_BSTRC("");
}

FSM_ANNOTSTATEMODEL FSANNOT_Util_ConvertStateModelToEnum(CFX_ByteString bsModel)
{
	FSM_ANNOTSTATEMODEL retModel = (FSM_ANNOTSTATEMODEL)0;
	if (bsModel.Equal("Marked"))
		retModel = FSAnnot::e_annotStateModelMarked;
	else if (bsModel.Equal("Review"))
		retModel = FSAnnot::e_annotStateModelReview;
	return retModel;
}

CFX_ByteString FSANNOT_Util_ConvertStateModelFromEnum(FSM_ANNOTSTATEMODEL model)
{
	switch(model)
	{
	case FSAnnot::e_annotStateModelMarked:
		return "Marked";
	case FSAnnot::e_annotStateModelReview:
		return "Review";
	default:
		return "";
	}
}

FSM_ANNOTSTATE FSANNOT_Util_ConvertStateToEnum(CFX_ByteString bsState)
{
	FSM_ANNOTSTATE retState = (FSM_ANNOTSTATE)0;
	if (bsState.Equal("Marked"))
		retState = FSAnnot::e_annotStateMarked;
	else if (bsState.Equal("Unmarked"))
		retState = FSAnnot::e_annotStateUnmarked;
	else if (bsState.Equal("Accepted"))
		retState = FSAnnot::e_annotStateAccepted;
	else if (bsState.Equal("Rejected"))
		retState = FSAnnot::e_annotStateRejected;
	else if (bsState.Equal("Cancelled"))
		retState = FSAnnot::e_annotStateCancelled;
	else if (bsState.Equal("Completed"))
		retState = FSAnnot::e_annotStateCompleted;
	else if (bsState.Equal("None"))
		retState = FSAnnot::e_annotStateNone;
	return retState;
}

CFX_ByteString FSANNOT_Util_ConvertStateFromEnum(FSM_ANNOTSTATE state)
{
	switch(state)
	{
	case FSAnnot::e_annotStateMarked:
		return "Marked";
	case FSAnnot::e_annotStateUnmarked:
		return "Unmarked";
	case FSAnnot::e_annotStateAccepted:
		return "Accepted";
	case FSAnnot::e_annotStateRejected:
		return "Rejected";
	case FSAnnot::e_annotStateCancelled:
		return "Cancelled";
	case FSAnnot::e_annotStateCompleted:
		return "Completed";
	case FSAnnot::e_annotStateNone:
		return "None";
	default:
		return "";
	}
}

FS_BOOL FSANNOT_Util_CheckAnnotIsMarkup(FSM_ANNOTTYPE annotType)
{
	switch(annotType){
	case FSAnnot::e_annotNote:
	case FSAnnot::e_annotCaret:
	case FSAnnot::e_annotCircle:
	case FSAnnot::e_annotFileAttachment:
	case FSAnnot::e_annotFreeText:
	case FSAnnot::e_annotHighlight:
	case FSAnnot::e_annotInk:
	case FSAnnot::e_annotLine:
	case FSAnnot::e_annotPolygon:
	case FSAnnot::e_annotPolyLine:
	case FSAnnot::e_annotSound:
	case FSAnnot::e_annotSquare:
	case FSAnnot::e_annotSquiggly:
	case FSAnnot::e_annotStamp:
	case FSAnnot::e_annotStrikeOut:
	case FSAnnot::e_annotUnderline:
		return TRUE;
	}
	return FALSE;
}

FS_BOOL FSANNOT_Util_CheckAnnotIsMarkup(CFX_ByteString bsAnnotType)
{
	const char* annotType[] = {FSPDF_ANNOTTYPE_NOTE,FSPDF_ANNOTTYPE_CIRCLE,
							FSPDF_ANNOTTYPE_CARET, FSPDF_ANNOTTYPE_FILEATTACHMENT,
							FSPDF_ANNOTTYPE_FREETEXT, FSPDF_ANNOTTYPE_HIGHLIGHT,FSPDF_ANNOTTYPE_UNDERLINE,
							FSPDF_ANNOTTYPE_INK, FSPDF_ANNOTTYPE_LINE, FSPDF_ANNOTTYPE_POLYGON,
							FSPDF_ANNOTTYPE_POLYLINE, FSPDF_ANNOTTYPE_SOUND, FSPDF_ANNOTTYPE_SQUARE,
							FSPDF_ANNOTTYPE_SQUIGGLY, FSPDF_ANNOTTYPE_STAMP, FSPDF_ANNOTTYPE_STRIKEOUT};
	for (FS_INT32 i = 0; i < sizeof(annotType) / sizeof(annotType[0]); i++)
	{
		if (bsAnnotType.Equal(annotType[i]))
			return TRUE;
	}

	return FALSE;
}

FS_BOOL FSANNOT_Util_CheckAnnotIsSupport(CFX_ByteString bsAnnotType)
{
	const char* annotType[] = {	FSPDF_ANNOTTYPE_NOTE, FSPDF_ANNOTTYPE_STRIKEOUT, FSPDF_ANNOTTYPE_SQUIGGLY, 
								FSPDF_ANNOTTYPE_UNDERLINE, FSPDF_ANNOTTYPE_HIGHLIGHT, FSPDF_ANNOTTYPE_LINK, 
								FSPDF_ANNOTTYPE_SQUARE, FSPDF_ANNOTTYPE_CIRCLE, FSPDF_ANNOTTYPE_FREETEXT, 
								FSPDF_ANNOTTYPE_LINE, FSPDF_ANNOTTYPE_STAMP, FSPDF_ANNOTTYPE_CARET, 
								FSPDF_ANNOTTYPE_INK, FSPDF_ANNOTTYPE_FILEATTACHMENT,FSPDF_ANNOTTYPE_WIDGET, FSPDF_ANNOTTYPE_POLYGON,
								FSPDF_ANNOTTYPE_POLYLINE, FSPDF_ANNOTTYPE_POPUP};
	for (int i = 0; i < sizeof(annotType) / sizeof(annotType[0]); i++)
	{
		if (bsAnnotType.Equal(annotType[i]))
			return TRUE;
	}
	return FALSE;
}

FS_BOOL FSANNOT_Util_CheckAnnotIsSupport(FSM_ANNOTTYPE annotType)
{
	CFX_ByteString bsAnnotType = FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(annotType);
	return FSANNOT_Util_CheckAnnotIsSupport(bsAnnotType);
}

FS_BOOL FSANNOT_Util_IsBorderInfoValid(FSBorderInfo borderInfo)
{
	if (borderInfo.width<0
		|| borderInfo.style<FSAnnot::e_borderStyleSolid || borderInfo.style>FSAnnot::e_borderStyleCloudy
		)
		return FALSE;
	return TRUE;
}

FS_BOOL FSANNOT_Util_ColorToAPStream(FX_ARGB color, FS_BOOL bFill, CFX_ByteTextBuf & bufContent)
{
	if (FXARGB_A(color) <= 0) return FALSE;
	bufContent << (FS_FLOAT)FXARGB_R(color) / 255.0f << FX_BSTRC(" ");
	bufContent << (FS_FLOAT)FXARGB_G(color) / 255.0f << FX_BSTRC(" ");
	bufContent << (FS_FLOAT)FXARGB_B(color) / 255.0f << (bFill ? FX_BSTRC(" rg\n") : FX_BSTRC(" RG\n"));
	return TRUE;
}

void FSANNOT_Util_DashToAppStream(FX_FLOAT fDashOffset, const CFX_FloatArray& dashPattern, CFX_ByteTextBuf& csTemp)
{
	csTemp << FX_BSTRC("[");
	// 	if (FXSYS_fabs(fDashOffset) >= FSPDFANNOT_MIN)
	// 		csTemp << FX_BSTRC("[");
	for (FX_INT32 i = 0; i < dashPattern.GetSize(); i ++)
	{
		if (i > 0) csTemp << FX_BSTRC(" ");
		csTemp << dashPattern[i];
	}
	csTemp << FX_BSTRC("]");
	// 	if (FXSYS_fabs(fDashOffset) >= FSPDFANNOT_MIN)
	// 		csTemp << fDashOffset << FX_BSTRC("]");
	csTemp << fDashOffset;
	csTemp << FX_BSTRC(" d\n");
}

FSAnnot* FSANNOT_UTIL_InsertAnnot(int annotIndex, CPDF_Dictionary* pAnnotDict, FSPDFPage* pPage)
{
	if(!pPage || !pAnnotDict) return NULL;
	FSAnnot* pAnnot = NULL;
	if (pAnnotDict->KeyExist(FSANNOT_NAMEKEY_SUBTYPE)) {
		CFX_ByteString csType = pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE);
		FSM_ANNOTTYPE annotType = FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(csType);
		switch(annotType)
		{
		case FSAnnot::e_annotNote:
			pAnnot = (FSNote*)FX_NEW FSPDFNoteImp(pAnnotDict, pPage);
			break;
		case  FSAnnot::e_annotUnderline:
			pAnnot = (FSUnderline*)FX_NEW FSPDFUnderlineImp(pAnnotDict, pPage);
			break;
		case  FSAnnot::e_annotHighlight:
			pAnnot = (FSHighlight*)FX_NEW FSPDFHighlightImp(pAnnotDict, pPage);
			break;
		case  FSAnnot::e_annotSquiggly:
			pAnnot = (FSSquiggly*)FX_NEW FSPDFSquigglyImp(pAnnotDict, pPage);
			break;
		case  FSAnnot::e_annotStrikeOut:
			pAnnot = (FSStrikeOut*)FX_NEW FSPDFStrikeOutImp(pAnnotDict, pPage);
			break;
		case FSAnnot::e_annotLink:
			pAnnot = (FSLink*)FX_NEW FSPDFLinkImp(pAnnotDict, pPage);
			break;
		default:
			return NULL;
		}
	}
	CPDF_Page* pPDFPage = (CPDF_Page*)pPage;
	pPDFPage->m_pDocument->AddIndirectObject(pAnnotDict);

	CPDF_Dictionary* pPageDict = pPDFPage->m_pFormDict;
	CPDF_Array* pAnnotArray = pPageDict->GetArray(FX_BSTRC("Annots"));
	if(!pAnnotArray)
	{
		pAnnotArray = FX_NEW CPDF_Array;
		if (!pAnnotArray) {
			SAFE_DELETE(pAnnot);
			return NULL;
		}
		pPageDict->SetAt(FX_BSTRC("Annots"), pAnnotArray);
	}
	if(annotIndex<0 || annotIndex>(FS_INT32)pAnnotArray->GetCount()) annotIndex = pAnnotArray->GetCount();
	CPDF_Reference* pRef = FX_NEW CPDF_Reference((CPDF_Document*)(pPage->getDocument()), pAnnotDict->GetObjNum());
	if(!pRef){
		SAFE_DELETE(pAnnot);
		return NULL;
	}
	pAnnotArray->InsertAt(annotIndex, pRef, (CPDF_Document*)(pPage->getDocument()));
	return pAnnot;
}

void	FSANNOT_UTIL_SetDefaultProperties(FSAnnot* pAnnot)
{// All the default value of properties are based on Phantom 8.0.
	if (!pAnnot) return;
	FS_DWORD dwColor = 0xFF000000;
	FS_FLOAT fDashes[16] = {-1};
	FSBorderInfo borderInfo;
	borderInfo.set(1.0, FSAnnot::e_borderStyleSolid, 0, 0, fDashes);
	switch(pAnnot->getType())
	{
	case FSAnnot::e_annotNote:
		{
			dwColor = 0xFFFFFF00;
			pAnnot->setBorderColor(dwColor);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
			((FSNote*)pAnnot)->setIconName("Comment");
		}
		break;
	case FSAnnot::e_annotHighlight:
		{
			dwColor = 0xFFFFED00;
			pAnnot->setBorderColor(dwColor);
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotUnderline:
		{
			dwColor = 0xFF339E00;
			pAnnot->setBorderColor(dwColor);
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotStrikeOut:
		{
			dwColor = 0xFFF94244;
			pAnnot->setBorderColor(dwColor);
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotSquiggly:
		{
			dwColor = 0xFF4899FF;
			pAnnot->setBorderColor(dwColor);
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotLink:
		{
			pAnnot->setBorderInfo(borderInfo);
			dwColor = 0xFFFF0000;
			pAnnot->setBorderColor(dwColor);
			((FSPDFLinkImp*)(FSLink*)pAnnot)->setHighlightingMode(FSAnnot::e_highlightingModeNone);
		}
		break;
	case FSAnnot::e_annotSquare:
	case FSAnnot::e_annotCircle:
		{
			dwColor = 0xFFFF0000;
			borderInfo.width = 2.0f;
			pAnnot->setBorderInfo(borderInfo);
			pAnnot->setBorderColor(dwColor);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotFreeText:
		{
			dwColor = 0xFFFF0000;
			pAnnot->setBorderColor(dwColor);
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
			((FSFreeText*)pAnnot)->setAlignment(0);
			FSDefaultAppearance da;
			da.flags = FSCommonDefines::e_defaultAPFont | FSCommonDefines::e_defaultAPFontSize | FSCommonDefines::e_defaultAPTextColor;
			da.font = FSFont::createStandard(FSCommonDefines::e_fontStandardIDHelvetica);
			da.fontSize = 12;
			da.textColor = 0xFF0000FF;
			((FSFreeText*)pAnnot)->setDefaultAppearance(da);
			da.font->release();
		}
		break;
	case FSAnnot::e_annotStamp:
		{
			((FSMarkup*)pAnnot)->setOpacity(1.0);
			((FSStamp*)pAnnot)->setIconName("Draft");
		}
		break;
	case FSAnnot::e_annotCaret:
		{
			dwColor = 0xFFA7ABE8;
			pAnnot->setBorderColor(dwColor);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
			// TODO: need to set RD??
		}
		break;
	case FSAnnot::e_annotLine:
		{
			dwColor = 0xFFFF0000;
			pAnnot->setBorderColor(dwColor);
			borderInfo.width = 2.0;
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
			((FSLine*)pAnnot)->enableCaption(FALSE);
			((FSLine*)pAnnot)->setLineStartingStyle("None");
			((FSLine*)pAnnot)->setLineEndingStyle("None");
		}
		break;
	case FSAnnot::e_annotInk:
		{
			dwColor = 0xFFFF0000;
			pAnnot->setBorderColor(dwColor);
			borderInfo.width = 2.0;
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotPolygon:
	case FSAnnot::e_annotPolyLine:
		{
			dwColor = 0xFFFF0000;
			pAnnot->setBorderColor(dwColor);
			borderInfo.width = 2.0;
			pAnnot->setBorderInfo(borderInfo);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
		}
		break;
	case FSAnnot::e_annotFileAttachment:
		{
			dwColor = 0xFFFF0000;
			pAnnot->setBorderColor(dwColor);
			((FSMarkup*)pAnnot)->setOpacity(1.0);
			((FSFileAttachment*)pAnnot)->setIconName("PushPin");
		}
		break;
	default: //Here, unsupport type
		return ;
	}
}

// annotIndex = -1 means to insert annot to the last.
CPDF_Dictionary* FSANNOT_UTIL_InsertAnnot(FSM_ANNOTTYPE annotType, const FS_INT32 annotIndex, const FSRectF annotRect, CPDF_Dictionary* pPageDict, CPDF_Document* pDoc, FS_BOOL bRDKTrialWM /*= FALSE*/)
{
	CPDF_Array* pAnnotArray = pPageDict->GetArray(FX_BSTRC("Annots"));
	if(!pAnnotArray)
	{
		pAnnotArray = FX_NEW CPDF_Array;
		if (!pAnnotArray) return NULL;
		pPageDict->SetAt(FX_BSTRC("Annots"), pAnnotArray);
	}
	FS_INT32 iInsertIndex = annotIndex;
	if (iInsertIndex<0 || iInsertIndex>=pAnnotArray->GetCount())
	{
		iInsertIndex = pAnnotArray->GetCount();
		// Check if current last annot is RDK trial watermark annot.
		// If yes, new annot should be inserted in front of  RDK trial watermark annot.
		if (FALSE == bRDKTrialWM && iInsertIndex>0)
		{
			CPDF_Dictionary* pLastAnnotDict = pAnnotArray->GetDict(iInsertIndex-1);
			if (WMANNOTTYPE_RDKTRIAL == FSCRT_Util_GetWatermarkAnnotType(pLastAnnotDict))
				iInsertIndex--;
		}
	}

	CPDF_Dictionary* pAnnotDict = FX_NEW CPDF_Dictionary;
	if(!pAnnotDict) return NULL;
	pAnnotDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("Annot"));
	CFX_FloatRect fxRectF(annotRect.left, annotRect.bottom, annotRect.right, annotRect.top);
	pAnnotDict->SetAtRect(FSANNOT_RECTKEY_RECT, fxRectF);
	CFX_ByteString strAnnotType = FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(annotType);
	pAnnotDict->SetAtName(FX_BSTRC("Subtype"), strAnnotType);
	pAnnotDict->SetAtReference(FX_BSTRC("P"), pDoc, pPageDict);
	//pAnnotDict->SetAtInteger(FX_BSTRC("Rotate"), pPageDict->GetInteger(FX_BSTRC("Rotate"), 0));
	pDoc->AddIndirectObject(pAnnotDict);

	CPDF_Reference* pRef = FX_NEW CPDF_Reference(pDoc, pAnnotDict->GetObjNum());
	if(!pRef) return NULL;

	pAnnotArray->InsertAt(iInsertIndex, pRef, pDoc);

	return pAnnotDict;
}

FX_BOOL FSANNOT_Util_PolygonIsPtIn(FX_FLOAT x, FX_FLOAT y, CFS_Vector* pts, int ptnum)
{
	if (pts == NULL || ptnum < 3) return FALSE;

	FX_FLOAT startx, starty, endx, endy;
	int cross_num = 0;
	for (int i = 0; i < ptnum; i ++) {
		startx = pts[i].x;
		starty = pts[i].y;
		endx = pts[(i+1) % ptnum].x;
		endy = pts[(i+1) % ptnum].y;

		if (starty < endy) {
			FX_FLOAT temp = startx;
			startx = endx;
			endx = temp;
			temp = starty;
			starty = endy;
			endy = temp;
		}

		if (FXSYS_fabs(endx - startx) < FSPDF_FLOAT_MIN && 
			FXSYS_fabs(endy - starty) < FSPDF_FLOAT_MIN) 
			continue;

		FX_FLOAT v_y, h_x;
		if (FXSYS_fabs(endx - startx) < FSPDF_FLOAT_MIN)
			v_y = FSPDF_FLOAT_MIN;
		else
			v_y = (endy - starty) * (x - startx) / FSPDF_FDIVVAL(endx - startx) + starty;
		/*if (FXSYS_fabs(endy - starty) < FSPDF_FLOAT_MIN)
			h_x = FSPDF_FLOAT_MIN;
		else
			h_x = (y - starty) * (endx - startx) / FSPDF_FDIVVAL(endy - starty) + startx;*/

		if (y > starty && y > endy || y < starty && y < endy)
			continue;
		if (x > startx && x > endx)
			continue;
		if (y > v_y && endx > startx || y < v_y && endx < startx)
			continue;		
		cross_num ++;
	}

	if (cross_num % 2 == 1)
		return TRUE;
	else if (cross_num % 2 == 0)
		return FALSE;

	return FALSE;
}

FX_BOOL FSANNOT_Util_TextMarkupIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, FS_FLOAT tolerance)
{
	if (!pAnnotDict) return FALSE;

	CPDF_Array* pQuadArray = pAnnotDict->GetArray(FSANNOT_ARRAYKEY_QUADPOINTS);
	if (!pQuadArray) return FALSE;

	int count = pQuadArray->GetCount() / 8;
	for (int i = 0; i < count; i ++) {
		int index = i * 8;
		CFS_Vector pts[4];
		pts[0].x = (FX_FLOAT)(pQuadArray->GetNumber(index) - tolerance);
		pts[0].y = (FX_FLOAT)(pQuadArray->GetNumber(index+1) + tolerance);
		pts[1].x = (FX_FLOAT)(pQuadArray->GetNumber(index+2) + tolerance);
		pts[1].y = (FX_FLOAT)(pQuadArray->GetNumber(index+3) + tolerance);
		pts[2].x = (FX_FLOAT)(pQuadArray->GetNumber(index+6) + tolerance);
		pts[2].y = (FX_FLOAT)(pQuadArray->GetNumber(index+7) - tolerance);
		pts[3].x = (FX_FLOAT)(pQuadArray->GetNumber(index+4) - tolerance);
		pts[3].y = (FX_FLOAT)(pQuadArray->GetNumber(index+5) - tolerance);
		if (CFS_Vector::IsRect(pts)){
			CFX_FloatRect floatRect(pts[3].x, pts[3].y, pts[1].x, pts[1].y);
            floatRect.Normalize();
			if (floatRect.Contains(x, y))
				return TRUE;
		}
		else if (FSANNOT_Util_PolygonIsPtIn(x, y, pts, 4))
			return TRUE;
	}
	return FALSE;
}

CPDF_Rect FSANNOT_Util_DeflateRectangle(CPDF_Rect crRect, FX_FLOAT fDeflateX, FX_FLOAT fDeflateY)
{
	CPDF_Rect crNew(crRect.left + fDeflateX,
		crRect.bottom + fDeflateY,
		crRect.right - fDeflateX,
		crRect.top - fDeflateY);
	crNew.Normalize();
	return crNew;
}

CPDF_Rect FSANNOT_Util_DeflateRectangle(CPDF_Rect crRect, FX_FLOAT fDeflateLeft, FX_FLOAT fDeflateBottom, FX_FLOAT fDeflateRight, FX_FLOAT fDeflateTop)
{
	CPDF_Rect crNew(crRect.left + fDeflateLeft,
		crRect.bottom + fDeflateBottom,
		crRect.right - fDeflateRight,
		crRect.top - fDeflateTop);
	crNew.Normalize();
	return crNew;
}

FX_BOOL FSANNOT_Util_EclipseIsPtIn(FX_FLOAT x, FX_FLOAT y, CPDF_Rect crRect)
{
	if (x > crRect.right || x < crRect.left) return FALSE;
	if (y > crRect.top || y < crRect.bottom) return FALSE;

	FX_FLOAT a = (crRect.right - crRect.left) / 2.0f;
	FX_FLOAT b = (crRect.top - crRect.bottom) / 2.0f;
	FX_FLOAT xCenter = (crRect.left + crRect.right) / 2.0f;
	FX_FLOAT yCenter = (crRect.bottom + crRect.top) / 2.0f;
	FX_FLOAT y1, y2;
	y1 = (FX_FLOAT)FXSYS_sqrt(1.0f - (x - xCenter) * (x - xCenter) / (a * a)) * b;
	y2 = -y1;
	y1 += yCenter;
	y2 += yCenter;
	return (y <= y1 && y >= y2);
}

FS_INT32 FSANNOT_Util_GetBorderThickness(CPDF_Dictionary* pAnnotDict)
{
	if (!pAnnotDict) return -1;
	
	int iBorderWidth = 1;
	
	CPDF_Dictionary* pBS = pAnnotDict->GetDict(FX_BSTRC("BS"));
	if (pBS)
	{
		iBorderWidth = pBS->GetNumber(FX_BSTRC("W"));
	}
	else
	{
		CPDF_Array* pBorder = pAnnotDict->GetArray(FX_BSTRC("Border"));
		if (pBorder) 
			iBorderWidth = pBorder->GetNumber(2);
	}

	if (iBorderWidth < 1) iBorderWidth = 1;

	return iBorderWidth;
}

FX_BOOL FSANNOT_Util_SquareCirleIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, FS_FLOAT tolerance)
{
	if (!pAnnotDict) return FALSE;
	CFX_ByteString bstrAnnotType = pAnnotDict->GetString(FX_BSTR("Subtype"));
	if (bstrAnnotType.IsEmpty()) return FALSE;
	CPDF_Rect crRect = pAnnotDict->GetRect(FX_BSTR("Rect"));
	CPDF_Rect crRD;
	if (pAnnotDict->KeyExist("RD"))
		crRD = pAnnotDict->GetRect(FX_BSTR("RD"));

	CPDF_Rect crInnerRect = FSANNOT_Util_DeflateRectangle(crRect, crRD.left, crRD.bottom, crRD.right, crRD.top);
	CPDF_Rect crSquare = FSANNOT_Util_DeflateRectangle(crInnerRect, -tolerance, -tolerance);
	if (bstrAnnotType.EqualNoCase("Square"))
	{
		if (crSquare.Contains(x, y) == FALSE)
			return FALSE;
	}
	else if(bstrAnnotType.EqualNoCase("Circle"))
	{
		if (crSquare.Contains(x, y) == FALSE)
			return FALSE;
	}

	FS_INT32 iBorderWidth = FSANNOT_Util_GetBorderThickness(pAnnotDict);
	iBorderWidth += (FS_INT32)tolerance;
	if (bstrAnnotType.EqualNoCase("Square"))
	{
		CPDF_Rect crSquare1 = FSANNOT_Util_DeflateRectangle(crInnerRect, (FX_FLOAT)(-iBorderWidth), (FX_FLOAT)(-iBorderWidth));
		CPDF_Rect crSquare2 = FSANNOT_Util_DeflateRectangle(crInnerRect, (FX_FLOAT)(iBorderWidth), (FX_FLOAT)(iBorderWidth));

		if (crSquare1.Contains(x, y))
		{
			if (!pAnnotDict->KeyExist("IC"))
			{
				if(crInnerRect.Width() <= 2*iBorderWidth || crInnerRect.Height() <= 2*iBorderWidth)
					return TRUE;
				if (crSquare2.Contains(x, y) == FALSE)
				{
					return TRUE;
				}
			}
			else
				return TRUE;
		}
	}
	else if(bstrAnnotType.EqualNoCase("Circle"))
	{
		CPDF_Rect crSquare1 = FSANNOT_Util_DeflateRectangle(crInnerRect, (FX_FLOAT)(iBorderWidth), (FX_FLOAT)(iBorderWidth));
		CPDF_Rect crSquare2 = FSANNOT_Util_DeflateRectangle(crInnerRect, (FX_FLOAT)(-iBorderWidth), (FX_FLOAT)(-iBorderWidth));
		if (FSANNOT_Util_EclipseIsPtIn(x, y, crSquare2)) {
			if (!pAnnotDict->KeyExist("IC"))
			{
				if (crInnerRect.Width() < iBorderWidth || crInnerRect.Height() < iBorderWidth)
					return TRUE;
				if(FSANNOT_Util_EclipseIsPtIn(x, y, crSquare1) == FALSE)
				{
					return TRUE;
				}
			}
			else
			{
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

FX_FLOAT FSANNOT_Util_Length(CPDF_Point& point)
{
	return FXSYS_sqrt(point.x * point.x + point.y * point.y); 
}

CPDF_Point FSANNOT_Util_Rotate(CPDF_Point& point, double angle)
{
	CPDF_Point p(0.0f,0.0f);
	double cosValue = FXSYS_cos((FX_FLOAT)angle);
	double sinValue = FXSYS_sin((FX_FLOAT)angle);
	p.x = (FX_FLOAT)((double)point.x * cosValue - (double)point.y * sinValue);
	p.y = (FX_FLOAT)((double)point.x * sinValue + (double)point.y * cosValue);
	return p;
}

void FSANNOT_Util_Normalize(CPDF_Point& point)
{
	double dLen = FSANNOT_Util_Length(point);
	point.x = (FX_FLOAT)((double)point.x / dLen);
	point.y = (FX_FLOAT)((double)point.y / dLen);
}

CPDF_Point FSANNOT_Util_LineGetLLStartingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2)
{
	if (!pAnnotDict) return CPDF_Point(0, 0);

	CPDF_Point p3 = CPDF_Point(0.0f, 0.0f);
	p3.x = p2.x - p1.x;
	p3.y = p2.y - p1.y;

	if (FSANNOT_Util_Length(p3) > 0.0f)
		FSANNOT_Util_Normalize(p3);

	FX_FLOAT ll = 0.0f;
	if(pAnnotDict->KeyExist("LL")) 
		ll = pAnnotDict->GetNumber("LL");

	if (ll >= 0.0f)
		p3 = FSANNOT_Util_Rotate(p3, FSPDFANNOTUTIL_PI / 2.0f);
	else
		p3 = FSANNOT_Util_Rotate(p3, -FSPDFANNOTUTIL_PI / 2.0f);

	p3.x *= (FX_FLOAT)FXSYS_fabs(ll);
	p3.y *= (FX_FLOAT)FXSYS_fabs(ll);
	CPDF_Point p4 = CPDF_Point(p1.x + p3.x, p1.y + p3.y);
	return p4;
}

CPDF_Point FSANNOT_Util_LineGetLLEndingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2)
{
	if (!pAnnotDict) return CPDF_Point(0, 0);

	CPDF_Point p3 = CPDF_Point(0.0f,0.0f);
	p3.x = p2.x - p1.x;
	p3.y = p2.y - p1.y;
	if (FSANNOT_Util_Length(p3) > 0.0f) 
		FSANNOT_Util_Normalize(p3);

	FX_FLOAT ll = 0.0f;
	if(pAnnotDict->KeyExist("LL")) 
		ll = pAnnotDict->GetNumber("LL");

	if (ll >= 0.0f)
		p3 = FSANNOT_Util_Rotate(p3, FSPDFANNOTUTIL_PI / 2.0f);
	else
		p3 = FSANNOT_Util_Rotate(p3, -FSPDFANNOTUTIL_PI / 2.0f);

	p3.x *= (FX_FLOAT)FXSYS_fabs(ll);
	p3.y *= (FX_FLOAT)FXSYS_fabs(ll);
	CPDF_Point p4 = CPDF_Point(p2.x + p3.x, p2.y + p3.y);
	return p4;
}

CPDF_Point FSANNOT_Util_LineGetLLEStartingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2)
{
	if (!pAnnotDict) return CPDF_Point(0, 0);

	CPDF_Point p3 = CPDF_Point(0.0f,0.0f);
	p3.x = p2.x - p1.x;
	p3.y = p2.y - p1.y;
	if (FSANNOT_Util_Length(p3) > 0.0f) 
		FSANNOT_Util_Normalize(p3);

	FX_FLOAT ll = 0.0f;
	if(pAnnotDict->KeyExist("LL")) 
		ll = pAnnotDict->GetNumber("LL");

	FX_FLOAT lle= 0.0f;
	if(pAnnotDict->KeyExist("LLE"))
		lle = pAnnotDict->GetNumber("LLE");

	if (ll >= 0.0f)
		p3 = FSANNOT_Util_Rotate(p3, FSPDFANNOTUTIL_PI / 2.0f);
	else
		p3 = FSANNOT_Util_Rotate(p3, -FSPDFANNOTUTIL_PI / 2.0f);

	p3.x *= (lle + (FX_FLOAT)FXSYS_fabs(ll));
	p3.y *= (lle + (FX_FLOAT)FXSYS_fabs(ll));
	CPDF_Point p4 = CPDF_Point(p1.x + p3.x,p1.y + p3.y);
	return p4;
}

CPDF_Point FSANNOT_Util_LineGetLLEEndingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2)
{
	if (!pAnnotDict) return CPDF_Point(0, 0);
	
	CPDF_Point p3 = CPDF_Point(0.0f,0.0f);

	p3.x = p2.x - p1.x;
	p3.y = p2.y - p1.y;
	if (FSANNOT_Util_Length(p3) > 0.0f) 
		FSANNOT_Util_Normalize(p3);

	FX_FLOAT ll = 0.0f;
	if(pAnnotDict->KeyExist("LL")) 
		ll = pAnnotDict->GetNumber("LL");

	FX_FLOAT lle= 0.0f;
	if(pAnnotDict->KeyExist("LLE"))
		lle = pAnnotDict->GetNumber("LLE");

	if (ll >= 0.0f)
		p3 = FSANNOT_Util_Rotate(p3, FSPDFANNOTUTIL_PI / 2.0f);
	else
		p3 = FSANNOT_Util_Rotate(p3, -FSPDFANNOTUTIL_PI / 2.0f);

	p3.x *= (lle + (FX_FLOAT)FXSYS_fabs(ll));
	p3.y *= (lle + (FX_FLOAT)FXSYS_fabs(ll));
	CPDF_Point p4 = CPDF_Point(p2.x + p3.x,p2.y + p3.y);
	return p4;
}

FX_FLOAT FSANNOT_Util_DistancePointToLine(CPDF_Point point, CPDF_Point linePoint1, CPDF_Point linePoint2)
{
	CPDF_Point vector(0.0f,0.0f);
	vector.x = linePoint1.x - linePoint2.x;
	vector.y = linePoint1.y - linePoint2.y;

	if (FSANNOT_Util_Length(vector) <= FSPDFANNOTUTIL_MIN)
	{
		vector.x = point.x - linePoint1.x;
		vector.y = point.y - linePoint1.y;
		return (FX_FLOAT)FSANNOT_Util_Length(vector);
	}

	if (FXSYS_fabs(linePoint1.x - linePoint2.x) <= FSPDFANNOTUTIL_MIN)
		return (FX_FLOAT)FXSYS_fabs(point.x - linePoint1.x);

	FX_FLOAT a, b, d;
	a = vector.y / vector.x;
	b = (linePoint1.x * linePoint2.y - linePoint2.x * linePoint1.y) / vector.x;
	d = (FX_FLOAT)FXSYS_fabs(a * point.x - point.y + b) / (FX_FLOAT)FXSYS_sqrt(a * a + 1.0f);
	return d;
}

FX_BOOL FSANNOT_Util_LineIsPtIn(CPDF_Point& pBegin, CPDF_Point& pEnd, CPDF_Rect& crRect, FX_FLOAT magnifySize, FX_FLOAT fx, FX_FLOAT fy)
{
	FX_FLOAT d;

	crRect.left = FX_MIN(pBegin.x, pEnd.x);
	crRect.right = FX_MAX(pBegin.x, pEnd.x);
	crRect.bottom = FX_MIN(pBegin.y, pEnd.y);
	crRect.top = FX_MAX(pBegin.y, pEnd.y);
	crRect = FSANNOT_Util_DeflateRectangle(crRect, -magnifySize, -magnifySize);
	if (crRect.Contains(fx, fy))
	{
		d = FSANNOT_Util_DistancePointToLine(CPDF_Point(fx, fy), pBegin, pEnd);
		if (d <= magnifySize) return TRUE;
	}

	return FALSE;
}

FX_BOOL FSANNOT_Util_LineIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, FS_FLOAT tolerance)
{
	if (!pAnnotDict) return FALSE;
	CPDF_Rect crRect = pAnnotDict->GetRect(FX_BSTR("Rect"));
	if (crRect.Contains(x, y) == FALSE)
		return FALSE;

	CPDF_Array* pArray = NULL;
	if(pAnnotDict->KeyExist("L")) 
		pArray = pAnnotDict->GetArray("L");

	CPDF_Point pBegin(0.0f, 0.0f);
	if (pArray != NULL)
	{
		FX_DWORD dwCount = pArray->GetCount();
		if (dwCount > 0)
			pBegin.x = pArray->GetNumber(0);
		if (dwCount > 1)
			pBegin.y = pArray->GetNumber(1);
	}

	CPDF_Point pEnd(0.0f, 0.0f);
	if (pArray != NULL)
	{
		FX_DWORD dwCount = pArray->GetCount();
		if (dwCount > 2)
			pEnd.x = pArray->GetNumber(2);
		if (dwCount > 3)
			pEnd.y = pArray->GetNumber(3);
	}

	CPDF_Point p1 = pBegin;
	CPDF_Point p2 = pEnd;
	CPDF_Point p3 = FSANNOT_Util_LineGetLLStartingPoint(pAnnotDict, p1, p2);
	CPDF_Point p4 = FSANNOT_Util_LineGetLLEndingPoint(pAnnotDict, p1, p2);
	CPDF_Point p5 = FSANNOT_Util_LineGetLLEStartingPoint(pAnnotDict, p1, p2);
	CPDF_Point p6 = FSANNOT_Util_LineGetLLEEndingPoint(pAnnotDict, p1, p2);

	FS_INT32 iBorderWidth = FSANNOT_Util_GetBorderThickness(pAnnotDict);

	if (iBorderWidth < 1) iBorderWidth = 1;
	iBorderWidth += (FS_INT32)tolerance;

	FX_BOOL bIn1 = FSANNOT_Util_LineIsPtIn(p1,p2,crRect,(FX_FLOAT)iBorderWidth,x,y);
	FX_BOOL bIn2 = FSANNOT_Util_LineIsPtIn(p3,p4,crRect,(FX_FLOAT)iBorderWidth,x,y);
	FX_BOOL bIn3 = FSANNOT_Util_LineIsPtIn(p5,p6,crRect,(FX_FLOAT)iBorderWidth,x,y);

	if(bIn1 || bIn2 || bIn3)
		return TRUE;

	return FALSE;
}

FX_BOOL FSANNOT_Util_InkIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, int* lineindex, FS_FLOAT tolerance)
{
	if (!pAnnotDict) return FALSE;

	int iBorderWidth = FSANNOT_Util_GetBorderThickness(pAnnotDict);

	if (iBorderWidth < 1) iBorderWidth = 1;

	int nExpand = (int)(iBorderWidth + 1)/2;
	if (nExpand < 1) nExpand = 1;
	nExpand += 3;

	nExpand += (int)tolerance;

	CPDF_Array* pInkList = pAnnotDict->GetArray("InkList");
	if (!pInkList) return FALSE;

	for (unsigned int i = 0; i < pInkList->GetCount(); i ++) {
		CPDF_Array* pLineArray = pInkList->GetArray(i);
		if (!pLineArray) continue;

		CFS_Vector pt0(pLineArray->GetNumber(0), pLineArray->GetNumber(1));
		for (int j = 0; j < (int)pLineArray->GetCount() - 1; j += 2) {
			CFS_Vector pt1(pLineArray->GetNumber(j), pLineArray->GetNumber(j+1));
			CFX_FloatRect rt;
			rt.left = FX_MIN(pt0.x, pt1.x) - nExpand;
			rt.bottom = FX_MIN(pt0.y, pt1.y) - nExpand;
			rt.right = FX_MAX(pt0.x, pt1.x) + nExpand;
			rt.top = FX_MAX(pt0.y, pt1.y) + nExpand;
			if (rt.Contains(x, y)) {
				FX_FLOAT distance = 0.0f;
				CFS_Vector pt(x, y);
				CFS_Vector vector = pt0 - pt1;
				if (vector.Length() <= FSPDF_FLOAT_MIN) {
					vector = pt - pt1;
					distance = vector.Length();
				} else if (FXSYS_fabs(pt0.x - pt1.x) <= FSPDF_FLOAT_MIN) {
					distance = FXSYS_fabs(pt.x - pt0.x);
				} else {
					FX_FLOAT a, b;
					a = vector.y / FSPDF_FDIVVAL(vector.x);
					b = (pt0.x * pt1.y - pt1.x * pt0.y) / FSPDF_FDIVVAL(vector.x);
					distance = FXSYS_fabs(a * pt.x - pt.y + b) / FXSYS_sqrt(a * a + 1.0f);
				}
				if (distance <= nExpand) {
					*lineindex = i;
					return TRUE;
				}
			}
			pt0 = pt1;
		}
	}

	return FALSE;
}

FX_BOOL FSANNOT_Util_IsPointInRect(CPDF_Dictionary* pAnnotDict, FSM_ANNOTTYPE annotType, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, FS_INT32 pageRotate, const FSMatrix* pSrcMatrix)
{
	if(!pAnnotDict) return FALSE;

	FS_FLOAT fIconWidth = 0;
	FS_FLOAT fIconHeight = 0;

	CPDF_Rect annotRect = pAnnotDict->GetRect(FSANNOT_RECTKEY_RECT);
	CFX_ByteString bsIconName = pAnnotDict->GetString(FSANNOT_NAMEKEY_ICONNAME);
	if (!pSrcMatrix 
		|| FALSE == CFSPDF_AnnotIconProvider::getDisplaySize(annotType, bsIconName, fIconWidth, fIconHeight)
		|| !(FSAnnot::e_annotNote == annotType || FSAnnot::e_annotFileAttachment == annotType) 
		)
	{
		annotRect.Inflate((FX_FLOAT)( tolerance), (FX_FLOAT)( tolerance));
		if(annotRect.Contains(x,y))
			return TRUE;
		else
			return FALSE;
	}

	CPDF_Matrix matrix(pSrcMatrix->a, pSrcMatrix->b, pSrcMatrix->c, pSrcMatrix->d, pSrcMatrix->e, pSrcMatrix->f);

	FX_FLOAT scale_x = 1.0f, scale_y = 1.0f;
	if (fIconWidth != 0 && fIconHeight != 0) 
	{
		FX_FLOAT fWidth = annotRect.Width();
		FX_FLOAT fHeight = annotRect.Height();

		scale_x = fIconWidth / fWidth;
		scale_y = fIconHeight / fHeight;
	}
	else
		return FALSE;
	float factor_x = 0;
	float factor_y = 0;
	if(matrix.Is90Rotated())
	{
		factor_x = 1.0f/FXSYS_fabs(matrix.c) * scale_y;
		factor_y = 1.0f/FXSYS_fabs(matrix.b) * scale_x;
	}
	else
	{
		factor_x = 1.0f/FXSYS_fabs(matrix.a) * scale_x;
		factor_y = 1.0f/FXSYS_fabs(matrix.d) * scale_y;
	}
	float x1, x2;
	float y1, y2;
	switch(pageRotate)
	{
	case 0:
		{
			x1 = x2 = annotRect.left;
			y1 = y2 = annotRect.top;
		}
		break;
	case 90:
		{
			x1 = x2 = annotRect.left;
			y1 = y2 = annotRect.bottom;
		}
		break;
	case 180:
		{
			x1 = x2 = annotRect.right;
			y1 = y2 = annotRect.bottom;
		}
		break;
	case 270:
		{
			x1 = x2 = annotRect.right;
			y1 = y2 = annotRect.top;
		}
		break;
	}
	matrix.TransformPoint(x1, y1);
	matrix.Scale(factor_x, factor_y);
	matrix.TransformPoint(x2, y2);
	matrix.Translate(x1-x2, y1-y2);

	annotRect.Transform(&matrix);
	FX_RECT annotDeviceRect = annotRect.GetClosestRect();

	CFX_Matrix srcMatrix(pSrcMatrix->a, pSrcMatrix->b, pSrcMatrix->c, pSrcMatrix->d, pSrcMatrix->e, pSrcMatrix->f);
	srcMatrix.TransformPoint(x, y);
	int transTolerance = (int)srcMatrix.TransformDistance(tolerance);

	annotDeviceRect.left -= transTolerance;
	annotDeviceRect.right += transTolerance;
	annotDeviceRect.top -= transTolerance;
	annotDeviceRect.bottom += transTolerance;

	if(annotDeviceRect.Contains((int)x,(int)y))
		return TRUE;
	else
		return FALSE;
}

FX_BOOL	FSANNOT_Util_CheckAnnotIsAtPoint(FSAnnot* pAnnot, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, const FSMatrix* matrix)
{
	if (!pAnnot) return FALSE;
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
	if (!pAnnotDict) return FALSE;

	FS_FLOAT fx=x, fy=y,ft=tolerance;
	if(matrix){
		CFX_Matrix m(matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);
		CFX_Matrix rm;
		rm.SetReverse(m);
		rm.TransformPoint(fx, fy);
		ft = rm.TransformDistance(ft);
	}
	FSM_ANNOTTYPE annotType = pAnnot->getType();
	switch (annotType)
	{
	case FSAnnot::e_annotHighlight:
	case FSAnnot::e_annotUnderline:
	case FSAnnot::e_annotSquiggly:
	case FSAnnot::e_annotStrikeOut:
		return FSANNOT_Util_TextMarkupIsPointIn(pAnnotDict, fx, fy, ft);
	case FSAnnot::e_annotSquare:
	case FSAnnot::e_annotCircle:
		return FSANNOT_Util_SquareCirleIsPointIn(pAnnotDict, fx, fy, ft);
	case FSAnnot::e_annotLine:
		return FSANNOT_Util_LineIsPointIn(pAnnotDict, fx, fy, ft);
	case FSAnnot::e_annotInk:
		{
			int lineIndex = 0;
			return FSANNOT_Util_InkIsPointIn(pAnnotDict, fx, fy, &lineIndex, ft);
		}
	case FSAnnot::e_annotWidget:
		{
			FSPDFPage* page = pAnnot->getPage();
			if(!page) return FALSE;
			FSPDFDoc* doc = page->getDocument();
			if(!doc) return FALSE;
			FSFormFillerImp* formFiller = NULL;
			FSFormImp* form = (FSFormImp*)((FSPDFDocImp*)doc)->getForm();
			if(form) formFiller = (FSFormFillerImp*)form->getFormFiller();
			if(formFiller)
			{
				IFSPDF_AnnotHandler* annotHandler = formFiller->GetWidgetHandler();
				if (annotHandler)
				{
					FX_RECT rect;
					FS_RESULT ret = annotHandler->GetViewBBox(page, pAnnot, rect);
					if (FSCRT_ERRCODE_SUCCESS == ret)
					{
						rect.Normalize();	
						if (rect.Contains((int)fx, (int)fy))
							return TRUE;
					}
					return FALSE;
				}
			}
		}
	default:
		{
			int nPageRotate = pAnnot->getPage()->getRotation () * 90;
			return FSANNOT_Util_IsPointInRect(pAnnotDict, annotType, fx, fy, ft, nPageRotate, matrix);
		}
	};
	
	return FALSE;
}

FX_BOOL FSAnnot_Util_GetRotateMatrix(CPDF_Dictionary* pPageDict, CPDF_Rect annotRect, CPDF_Matrix& matrix)
{
	FX_FLOAT flWidth = annotRect.right - annotRect.left;
	FX_FLOAT flHeight = annotRect.top - annotRect.bottom;

	int nRotate = getPageRotation(pPageDict);
	switch(nRotate)
	{
	default:
	case 0:
		matrix = CPDF_Matrix(1, 0, 0, 1, 0, 0);
		break;
	case 1:
		matrix = CPDF_Matrix(0, 1, -1, 0, flWidth, 0);
		break;
	case 2:
		matrix = CPDF_Matrix(-1, 0, 0, -1, flWidth, flHeight);
		break;
	case 3:
		matrix = CPDF_Matrix(0,-1, 1, 0, 0, flHeight);
		break;
	}
	return TRUE;
}

FS_BOOL FSAnnot_Util_ChectAnnotIsReply(CPDF_Dictionary* pAnnotDict)
{
	//In adobe, a reply annot has "IRT" entry, but doesn't have a "RT" entry. -- "RT" entry is only used with value "Group" for grouping annotations.
	//In Foxit Phantom, a reply annot has "IRT" entry and "RT" entry(with value "R")
	//So, here, we should judge whether value of "RT" entry is "R" only if "RT" entry do exist.
	//Related to SEWQ-814.
	//----
	//Additional, state annot doesn't have "RT" entry but has "IRT" entry --> just same as reply annot created by Adobe.
	//But state annot must have "State" and "StateModel" entries.
	//So, here, we need to add adjustment for "State" and "StateModel" entries, in order to exclude state annot from reply annots.
	//Related to SDK-3928
	if (!pAnnotDict || !pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE).Equal(FSPDF_ANNOTTYPE_NOTE)
		|| !pAnnotDict->KeyExist(FSANNOT_DICTKEY_IRT) || pAnnotDict->KeyExist(FSANNOT_STRINGKEY_STATE) || pAnnotDict->KeyExist(FSANNOT_STRINGKEY_STATEMODEL) 
		|| (pAnnotDict->KeyExist(FSANNOT_NAMEKEY_RT) && !pAnnotDict->GetString(FSANNOT_NAMEKEY_RT).Equal(FX_BSTR("R"))))
		return FALSE;

	return TRUE;
}

FS_BOOL FSAnnot_Util_CheckIsStateAnnot(FSAnnot* pAnnot)
{
	// a state annot should be note annot -- no /RT, has /IRT, /STATE, /STATEMODEL.
	if (pAnnot && pAnnot->getType() == FSAnnot::e_annotNote)
	{
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		if (pAnnotDict && !pAnnotDict->KeyExist(FSANNOT_NAMEKEY_RT) && pAnnotDict->KeyExist(FSANNOT_DICTKEY_IRT)
			&& pAnnotDict->KeyExist(FSANNOT_STRINGKEY_STATEMODEL) && pAnnotDict->KeyExist(FSANNOT_STRINGKEY_STATE)
			)
			return TRUE;
	}
	return FALSE;
}

//This function would calculate a fixed matrix, which has no magnification or rotation.
//If no icon name, just pass "" to bsIconName.
void FSAnnot_Util_GetFixedMatrix(FX_INT32 rotate, FX_BSTR bsAnnotType, FX_BSTR bsIconName, const CPDF_Rect &annot_rect, const CFX_Matrix* pSrcMatrix, CFX_Matrix& matrix)
{
	FX_FLOAT scale_x = 1.0f, scale_y = 1.0f;
	CFX_ByteString sAnnotType = bsAnnotType;
	if (sAnnotType.Equal("Text") || sAnnotType.Equal("FileAttachment"))
	{
		CFSPDF_IconProviderManager* pIconProviderMgr = CFS_Library::getIconProviderMgr();
		if (pIconProviderMgr)
		{
			FSAnnotIconProvider* pAnnotIconProvider = pIconProviderMgr->GetAnnotIconProvider();
			if (pAnnotIconProvider)
			{
				FX_FLOAT fWidth = annot_rect.right - annot_rect.left;
				FX_FLOAT fHeight = annot_rect.top - annot_rect.bottom;
				FX_FLOAT fNewWidth = fWidth, fNewHeight = fHeight;
				FSM_ANNOTTYPE annotType = FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(bsAnnotType);
				if (CFSPDF_AnnotIconProvider::getDisplaySize(annotType, bsIconName, fNewWidth, fNewHeight))
				{
					scale_x = fNewWidth / fWidth;
					scale_y = fNewHeight / fHeight;
				}
			}
		}
	}

	float factor_x = 0;
	float factor_y = 0;

	matrix = *pSrcMatrix;
	if(matrix.Is90Rotated())
	{
		factor_x = 1.0f/FXSYS_fabs(matrix.c) * scale_y;
		factor_y = 1.0f/FXSYS_fabs(matrix.b) * scale_x;
	}
	else
	{
		factor_x = 1.0f/FXSYS_fabs(matrix.a) * scale_x;
		factor_y = 1.0f/FXSYS_fabs(matrix.d) * scale_y;
	}

	float x1, x2;
	float y1, y2;
	switch(rotate)
	{
	case 0:
		x1 = x2 = annot_rect.left;
		y1 = y2 = annot_rect.top;
		break;
	case 90:
		x1 = x2 = annot_rect.left;
		y1 = y2 = annot_rect.bottom;
		break;
	case 180:
		x1 = x2 = annot_rect.right;
		y1 = y2 = annot_rect.bottom;
		break;
	case 270:
		x1 = x2 = annot_rect.right;
		y1 = y2 = annot_rect.top;
		break;
	}

	matrix.TransformPoint(x1, y1);
	matrix.Scale(factor_x, factor_y);
	matrix.TransformPoint(x2, y2);
	matrix.Translate(x1-x2, y1-y2);
}

FS_BOOL FSAnnot_Util_IsAppearanceValid(CPDF_Dictionary* pAnnotDict)
{
	if (!pAnnotDict) return FALSE;

	if (CPDF_Dictionary* pAPDict = pAnnotDict->GetDict(FSANNOT_DICTKEY_AP))
		return (pAnnotDict->GetCount() > 0);

	return FALSE;
}

void FSAnnotUtil_GetUserSpaceLength(long lSize, FX_FLOAT& fSize, FX_FLOAT fUserUnit)
{
	int x = 96;
	int y = 96;
	fSize = (FX_FLOAT)lSize / (FX_FLOAT)FX_MAX(x, y) / fUserUnit;
}

void FSAnnotUtil_GetUserSpaceLength(long xSize, long ySize, FX_FLOAT& fxSize, FX_FLOAT& fySize, FX_FLOAT fUserUnit)
{
	int x = 96;
	int y = 96;

	fxSize = (FX_FLOAT)xSize / (FX_FLOAT)x / fUserUnit;
	fySize = (FX_FLOAT)ySize / (FX_FLOAT)y / fUserUnit;

}

FS_INT32 FSAnnot_CalculateCloudyNumberFromIntensity(FS_FLOAT fIntensity)
{
	if (!FLOAT_ISBIGGER(fIntensity, 0.0))
		return 0;
	if (FLOAT_ISBIGGER(fIntensity, 2.0))
		fIntensity = 2.0;

	// Decide delta: fIntensity<1.0, iDelta = 32; otherwise, iDelta = 16.
	FS_INT32 iDelta = 16;
	if (FLOAT_ISSMALLER(fIntensity, 1.0))
		iDelta = 32;

	// when fIntensity = 1.0, iNum = 32.
	FS_INT32 iNum = 32 - (fIntensity-1)*iDelta;
	if (iNum<2)
		iNum = 2;
	return iNum;
}

CPDF_Rect FSAnnot_CreateCloudyAP_Rectangle(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect, FS_FLOAT fIntensity, FX_FLOAT fRotateAngle /* = 0.0f */)
{
	CPDF_Rect pdfRect, cloudyRect;
	FS_INT32 iCloudyNum = FSAnnot_CalculateCloudyNumberFromIntensity(fIntensity);
	if (crRect.IsEmpty() || iCloudyNum <= 0) return pdfRect;

	CFX_PointF pdfPoint1, pdfPoint2, pdfPoint3;
	CFX_VectorF v1, v2;
	CFX_PointsF cPath;
	FX_DWORD i;
	FX_FLOAT fTotalLen, fStep, xCenter, yCenter, x, y, angle, radius;
	pdfPoint1.x = crRect.left, pdfPoint1.y = crRect.bottom;
	pdfPoint2.x = crRect.right, pdfPoint2.y = crRect.bottom;
	pdfPoint3.x = crRect.right, pdfPoint3.y = crRect.top;
	v1.x = pdfPoint2.x - pdfPoint1.x;
	v1.y = pdfPoint2.y - pdfPoint1.y;

	v2.x = pdfPoint3.x - pdfPoint2.x;
	v2.y = pdfPoint3.y - pdfPoint2.y;
	fTotalLen = (FX_FLOAT)(v1.Length() + v2.Length());
	xCenter = (crRect.right + crRect.left) / 2.0f;
	yCenter = (crRect.top + crRect.bottom) / 2.0f;
	radius = fTotalLen * 2.0f / (FX_FLOAT)iCloudyNum;
	fStep = 0.0f;
	for (i = 0; i < iCloudyNum / 2; i ++)
	{
		if (fStep <= v1.Length())
		{
			x = pdfPoint1.x + fStep - xCenter;
			y = pdfPoint1.y - yCenter;
		}
		else
		{
			x = pdfPoint2.x - xCenter;
			y = pdfPoint2.y + (fStep - (FX_FLOAT)v1.Length()) - yCenter;
		}
		pdfPoint3.x = xCenter + x * (FX_FLOAT)FXSYS_cos(fRotateAngle) - y * (FX_FLOAT)FXSYS_sin(fRotateAngle);
		pdfPoint3.y = yCenter + x * (FX_FLOAT)FXSYS_sin(fRotateAngle) + y * (FX_FLOAT)FXSYS_cos(fRotateAngle);
		cPath.Add(pdfPoint3);

		fStep += radius;
	}
	pdfPoint1.x = crRect.right, pdfPoint1.y = crRect.top;
	pdfPoint2.x = crRect.left, pdfPoint2.y = crRect.top;
	fStep = 0.0f;
	for (i = 0; i < iCloudyNum / 2; i ++)
	{
		if (fStep <= v1.Length())
		{
			x = pdfPoint1.x - fStep - xCenter;
			y = pdfPoint1.y - yCenter;
		}
		else
		{
			x = pdfPoint2.x - xCenter;
			y = pdfPoint2.y - (fStep - (FX_FLOAT)v1.Length()) - yCenter;
		}
		pdfPoint3.x = xCenter + x * (FX_FLOAT)FXSYS_cos(fRotateAngle) - y * (FX_FLOAT)FXSYS_sin(fRotateAngle);
		pdfPoint3.y = yCenter + x * (FX_FLOAT)FXSYS_sin(fRotateAngle) + y * (FX_FLOAT)FXSYS_cos(fRotateAngle);
		cPath.Add(pdfPoint3);

		fStep += radius;
	}
	radius = 0.0f;
	pdfPoint1 = cPath[0];
	iCloudyNum = cPath.GetSize();
	for (i = 1; i <= iCloudyNum; i ++)
	{
		pdfPoint2 = cPath[i % iCloudyNum];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		if (radius < v1.Length())
			radius = (FX_FLOAT)v1.Length();
		pdfPoint1 = pdfPoint2;
	}
	radius = radius * 5.0f / 8.0f;

	CFX_ByteString csTemp;
	pdfRect = crRect;
	FX_FLOAT startAngle, stopAngle;
	FX_BOOL bHasM = TRUE;
	for (i = 0; i < iCloudyNum; i ++)
	{
		pdfPoint1 = cPath[i];
		pdfPoint2 = cPath[(iCloudyNum - 1 + i) % iCloudyNum];
		pdfPoint3 = cPath[(i + 1) % iCloudyNum];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		v2.x = pdfPoint3.x - pdfPoint1.x;
		v2.y = pdfPoint3.y - pdfPoint1.y;

		startAngle = (FX_FLOAT)SlopeAngle(v1);
		//if (v1.y < 0.0f) startAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(v1.Length() / (2.0f * radius));
		startAngle += angle - FX_PI / 9.0f;
		stopAngle = (FX_FLOAT)SlopeAngle(v2);
		//if (v2.y < 0.0f) stopAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(v2.Length() / (2.0f * radius));
		stopAngle -= angle;
		if (stopAngle < startAngle) stopAngle += FX_PI * 2.0f;

		cloudyRect.left = pdfPoint1.x - radius;
		cloudyRect.bottom = pdfPoint1.y - radius;
		cloudyRect.right = pdfPoint1.x + radius;
		cloudyRect.top = pdfPoint1.y + radius;
		cloudyRect = FSAnnot_CreateCloudyAP_Arc(csAP, cloudyRect, startAngle, stopAngle, bHasM);
		if (bHasM) bHasM = FALSE;
		pdfRect.Union(cloudyRect);

		v1.x = pdfPoint1.x - pdfPoint3.x;
		v1.y = pdfPoint1.y - pdfPoint3.y;
		startAngle = (FX_FLOAT)SlopeAngle(v1);
		//if (v1.y < 0.0f) startAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(v1.Length() / (2.0f * radius));
		startAngle += angle;
		stopAngle = startAngle - FX_PI / 9.0f;
		cloudyRect.left = pdfPoint3.x - radius;
		cloudyRect.bottom = pdfPoint3.y - radius;
		cloudyRect.right = pdfPoint3.x + radius;
		cloudyRect.top = pdfPoint3.y + radius;
		FSAnnot_CreateCloudyAP_Arc(csAP, cloudyRect, startAngle, stopAngle, FALSE);
	}

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateCloudyAP_Arc(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect, FX_FLOAT fStartAngle, FX_FLOAT fStopAngle, FX_BOOL bHasM /* = FALSE */)
{
	CPDF_Rect pdfRect;
	if (crRect.IsEmpty()) return pdfRect;
	if (FXSYS_fabs(fStopAngle - fStartAngle) <= FSPDFANNOT_MIN) return pdfRect;

	FX_FLOAT fBezier = 0.5522847498308f;
	FX_FLOAT x, y, a, b, a1, a2, b1, b2, c1, c2, x0, y0, x1, y1, x2, y2, x3, y3, xx, yy, px, py, angle1, angle2;
	x = (crRect.right + crRect.left) / 2.0f;
	y = (crRect.top + crRect.bottom) / 2.0f;
	a = (FX_FLOAT)FXSYS_fabs(crRect.right - crRect.left) / 2.0f;
	b = (FX_FLOAT)FXSYS_fabs(crRect.top - crRect.bottom) / 2.0f;

	CFX_ByteString csTemp;
	angle1 = fStartAngle;
	x0 = a * (FX_FLOAT)FXSYS_cos(angle1);
	y0 = b * (FX_FLOAT)FXSYS_sin(angle1);
	px = x + x0;
	py = y + y0;
	pdfRect.left = pdfRect.right = px;
	pdfRect.bottom = pdfRect.top = py;
	if (bHasM)
	{
		csTemp.Format("%.3f %.3f m\n", px, py);
		csAP << csTemp;
	}

	FX_FLOAT iDirection = 1.0f;
	if (fStopAngle < fStartAngle) iDirection = -1.0f;
	FX_FLOAT fMultiple = (FX_FLOAT)FXSYS_floor(fStartAngle * 2.0f / FX_PI);
	if (iDirection > 0.0f) fMultiple += 1.0f;
	fMultiple *= FX_PI / 2.0f;
	FX_BOOL bBreak = FALSE;
	while (TRUE)
	{
		angle2 = fMultiple;
		if (iDirection > 0.0f)
		{
			if (fStopAngle <= fMultiple)
			{
				angle2 = fStopAngle;
				bBreak = TRUE;
			}
		}
		else
		{
			if (fStopAngle >= fMultiple)
			{
				angle2 = fStopAngle;
				bBreak = TRUE;
			}
		}

		x0 = a * (FX_FLOAT)FXSYS_cos(angle1);
		y0 = b * (FX_FLOAT)FXSYS_sin(angle1);
		x3 = a * (FX_FLOAT)FXSYS_cos(angle2);
		y3 = b * (FX_FLOAT)FXSYS_sin(angle2);

		a1 = b * b * x0;
		b1 = a * a * y0;
		a2 = b * b * x3;
		b2 = a * a * y3;
		c1 = c2 = - a * a * b * b;
		xx = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
		yy = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);

		px = xx - x0;
		py = yy - y0;
		x1 = x0 + px * fBezier;
		y1 = y0 + py * fBezier;
		px = xx - x3;
		py = yy - y3;
		x2 = x3 + px * fBezier;
		y2 = y3 + py * fBezier;

		px = x + x1;
		py = y + y1;
		x1 = px;
		y1 = py;
		if (pdfRect.left > x1) pdfRect.left = x1;
		if (pdfRect.bottom > y1) pdfRect.bottom = y1;
		if (pdfRect.right < x1) pdfRect.right = x1;
		if (pdfRect.top < y1) pdfRect.top = y1;
		px = x + x2;
		py = y + y2;
		x2 = px;
		y2 = py;
		if (pdfRect.left > x2) pdfRect.left = x2;
		if (pdfRect.bottom > y2) pdfRect.bottom = y2;
		if (pdfRect.right < x2) pdfRect.right = x2;
		if (pdfRect.top < y2) pdfRect.top = y2;
		px = x + x3;
		py = y + y3;
		x3 = px;
		y3 = py;
		if (pdfRect.left > x3) pdfRect.left = x3;
		if (pdfRect.bottom > y3) pdfRect.bottom = y3;
		if (pdfRect.right < x3) pdfRect.right = x3;
		if (pdfRect.top < y3) pdfRect.top = y3;

		csTemp.Format("%.3f %.3f %.3f %.3f %.3f %.3f c\n", x1, y1, x2, y2, x3, y3);
		csAP << csTemp;
		if (bBreak) break;

		angle1 = angle2;
		fMultiple += iDirection * FX_PI / 2.0f;
	}

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateCloudyAP_Ellipse(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect, FS_FLOAT fIntensity, FX_FLOAT fRotateAngle /* = 0.0f */)
{
	CPDF_Rect pdfRect, cloudyRect;
	FS_INT32 iCloudyNum = FSAnnot_CalculateCloudyNumberFromIntensity(fIntensity);
	if (crRect.IsEmpty() || iCloudyNum <= 0) return pdfRect;

	CPDF_Point pdfPoint1(0.0f,0.0f), pdfPoint2(0.0f,0.0f), pdfPoint3(0.0f,0.0f);
	CFX_VectorF v1, v2;
	CFX_ArrayTemplate<CPDF_Point> cPath;
	FX_DWORD i;
	FX_FLOAT a, b, xCenter, yCenter, x, y, angle, radius;
	a = (FX_FLOAT)FXSYS_fabs(crRect.right - crRect.left) / 2.0f;
	b = (FX_FLOAT)FXSYS_fabs(crRect.top - crRect.bottom) / 2.0f;
	xCenter = (crRect.right + crRect.left) / 2.0f;
	yCenter = (crRect.top + crRect.bottom) / 2.0f;
	for (i = 0; i < iCloudyNum; i ++)
	{
		angle = 2.0f* FX_PI * i / iCloudyNum;
		x = a * (FX_FLOAT)FXSYS_cos(angle);
		y = b * (FX_FLOAT)FXSYS_sin(angle);
		pdfPoint1.x = xCenter + x * (FX_FLOAT)FXSYS_cos(fRotateAngle) - y * (FX_FLOAT)FXSYS_sin(fRotateAngle);
		pdfPoint1.y = yCenter + x * (FX_FLOAT)FXSYS_sin(fRotateAngle) + y * (FX_FLOAT)FXSYS_cos(fRotateAngle);
		cPath.Add(pdfPoint1);
	}
	radius = 0.0f;
	pdfPoint1 = cPath[0];
	for (i = 1; i <= iCloudyNum; i ++)
	{
		pdfPoint2 = cPath[i % iCloudyNum];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		if (radius < v1.Length())
			radius = (FX_FLOAT)v1.Length();
		pdfPoint1 = pdfPoint2;
	}
	radius = radius * 5.0f / 8.0f;

	pdfRect = crRect;
	FX_FLOAT startAngle, stopAngle;
	FX_BOOL bHasM = TRUE;
	for (i = 0; i < iCloudyNum; i ++)
	{
		pdfPoint1 = cPath[i];
		pdfPoint2 = cPath[(iCloudyNum - 1 + i) % iCloudyNum];
		pdfPoint3 = cPath[(i + 1) % iCloudyNum];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		v2.x = pdfPoint3.x - pdfPoint1.x;
		v2.y = pdfPoint3.y - pdfPoint1.y;

		startAngle = (FX_FLOAT)SlopeAngle(v1);
		//if (v1.y < 0.0f) startAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(v1.Length() / (2.0f * radius));
		startAngle += angle - FX_PI / 9.0f;
		stopAngle = (FX_FLOAT)SlopeAngle(v2);
		//if (v2.y < 0.0f) stopAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(v2.Length() / (2.0f * radius));
		stopAngle -= angle;
		if (stopAngle < startAngle) stopAngle += FX_PI * 2.0f;

		cloudyRect.left = pdfPoint1.x - radius;
		cloudyRect.bottom = pdfPoint1.y - radius;
		cloudyRect.right = pdfPoint1.x + radius;
		cloudyRect.top = pdfPoint1.y + radius;
		cloudyRect = FSAnnot_CreateCloudyAP_Arc(csAP, cloudyRect, startAngle, stopAngle, bHasM);
		if (bHasM) bHasM = FALSE;

		pdfRect.Union(cloudyRect);

		v1.x = pdfPoint1.x - pdfPoint3.x;
		v1.y = pdfPoint1.y - pdfPoint3.y;
		startAngle = (FX_FLOAT)SlopeAngle(v1);
		//if (v1.y < 0.0f) startAngle *= -1.0f;
		angle = (FX_FLOAT)acos(v1.Length() / (2.0f * radius));
		startAngle += angle;
		stopAngle = startAngle - FX_PI / 9.0f;
		cloudyRect.left = pdfPoint3.x - radius;
		cloudyRect.bottom = pdfPoint3.y - radius;
		cloudyRect.right = pdfPoint3.x + radius;
		cloudyRect.top = pdfPoint3.y + radius;
		FSAnnot_CreateCloudyAP_Arc(csAP, cloudyRect, startAngle, stopAngle, FALSE);
	}

	return pdfRect;
}

void FSAnnot_CreateAP_Ellipse(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect)
{
	CFX_RectF rect;
	rect.Set(crRect.left, crRect.bottom, crRect.right - crRect.left, crRect.top - crRect.bottom);
	CFX_PointF ptStart;
	CFX_PointF points[4];
	FX_FLOAT fStart = 0.0f;
	for (FX_INT32 i = 0; i < 4; i ++)
	{
		FSAnnot_ArcToBezier_Segment(rect, fStart, FX_PI / 2.0f, points);
		CFX_ByteString csTemp;
		if (i == 0)
		{
			ptStart = points[0];
			csAP << ptStart.x << FX_BSTRC(" ") << ptStart.y << FX_BSTRC(" m\n");
		}

		if (i == 3)
		{
			csTemp.Format("%.3f %.3f %.3f %.3f %.3f %.3f c h\n", points[1].x, points[1].y, points[2].x, points[2].y, ptStart.x, ptStart.y);
		}
		else
			csTemp.Format("%.3f %.3f %.3f %.3f %.3f %.3f c\n", points[1].x, points[1].y, points[2].x, points[2].y, points[3].x, points[3].y);
		csAP << csTemp;

		fStart += FX_PI / 2.0f;
	}
}

FX_INT32 FSAnnot_ArcToBezier_Segment(const CFX_RectF &rect, FX_FLOAT fStart, FX_FLOAT fSweep, CFX_PointF bzPoints[4])
{
	fSweep /= 2.0f;
	FX_FLOAT x = FXSYS_cos(fSweep);
	FX_FLOAT y = FXSYS_sin(fSweep);
	FX_FLOAT tx = (1.0f - x) * 4.0f / 3.0f;
	FX_FLOAT ty = y - tx * x / y;
	FX_FLOAT px[4], py[4];
	px[0] = x;
	py[0] = -y;
	px[1] = x + tx;
	py[1] = -ty;
	px[2] = x + tx;
	py[2] = ty;
	px[3] = x;
	py[3] = y;
	fStart += fSweep;
	x = FXSYS_sin(fStart);
	y = FXSYS_cos(fStart);
	CFX_PointF cp = rect.Center();
	tx = rect.width / 2.0f;
	ty = rect.height / 2.0f;
	for (FX_INT32 i = 0; i < 4; i ++)
	{
		bzPoints[i].x = cp.x + tx * (px[i] * y - py[i] * x);
		bzPoints[i].y = cp.y + ty * (px[i] * x + py[i] * y);
	}
	return FXSYS_fabs(fSweep) < 0.001f ? 1 : 4;
}

FS_FLOAT FSAnnot_Util_Length(CPDF_Point point)
{
	return FXSYS_sqrt(point.x * point.x + point.y * point.y); 
}

CPDF_Point FSAnnot_Util_Rotate(CPDF_Point point, double angle)
{
	CPDF_Point pt(0.0f,0.0f);
	double cosValue = FXSYS_cos((FX_FLOAT)angle);
	double sinValue = FXSYS_sin((FX_FLOAT)angle);
	pt.x = (FX_FLOAT)((double)point.x * cosValue - (double)point.y * sinValue);
	pt.y = (FX_FLOAT)((double)point.x * sinValue + (double)point.y * cosValue);
	return pt;
}

FS_FLOAT	FSAnnot_Util_SlopeAngle(CPDF_Point pointA)
{
	CPDF_Point pointB(1.0f, 0.0f);
	return FXSYS_acos(FSAnnot_Util_Cosine(pointA, pointB));
}

FS_FLOAT	FSAnnot_Util_SlopeAngle(FSPointF pointA)
{
	CPDF_Point pt(pointA.x, pointA.y);
	return FSAnnot_Util_SlopeAngle(pt);
}

FX_FLOAT	FSAnnot_Util_Cosine(CPDF_Point pointA, CPDF_Point pointB/* = CPDF_Point(1.0f, 0.0f)*/)
{
	if (!(FSAnnot_Util_Length(pointA) != 0 && FSAnnot_Util_Length(pointB) != 0))
		return 0;

	FX_FLOAT dotProduct = FSAnnot_Util_DotProduct(pointA,pointB);
	return dotProduct / (FX_FLOAT)(FSAnnot_Util_Length(pointA) * FSAnnot_Util_Length(pointB));
}

FS_FLOAT	FSAnnot_Util_DotProduct(CPDF_Point pointA, CPDF_Point pointB)
{ 
	return pointA.x * pointB.x + pointA.y * pointB.y; 
}

FS_INT32	FSAnnot_Util_CalculateCloudyIntensity(CPDF_Rect rcEdit, FS_INT32 nIntensity)
{
	FS_INT32 nRet = 2;
	switch (nIntensity)
	{
	case 1:
		nRet = (FX_INT32)((rcEdit.Width() + rcEdit.Height()) / 4.0f);
		break;
	case 2:
	default:
		nRet = (FX_INT32)((rcEdit.Width() + rcEdit.Height()) / 6.0f);
		break;
	}
	if (nRet < 2)
		nRet = 2;
	return nRet;

}


CPDF_Rect FSAnnot_CreateLineEndingAP_None(CFX_ByteString& csAP, CPDF_Point& point,  FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect crRect;
	crRect.left = point.x - fWidth / 2.0f;
	crRect.bottom = point.y - fWidth / 2.0f;
	crRect.right = point.x + fWidth / 2.0f;
	crRect.top = point.y + fWidth / 2.0f;

	return crRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_Square(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect crRect, pdfRect;
	crRect.left = point.x - fWidth / 2.0f;
	crRect.bottom = point.y - fWidth / 2.0f;
	crRect.right = point.x + fWidth / 2.0f;
	crRect.top = point.y + fWidth / 2.0f;
	FX_FLOAT fSlopeAngle;
	if (FSAnnot_Util_Length(direction) < 0.0001f)
		fSlopeAngle = 0.0f;
	else
	{
		fSlopeAngle = (FX_FLOAT)FSAnnot_Util_SlopeAngle(direction);
		if (direction.y < 0.0f) fSlopeAngle *= -1.0f;
	}
	pdfRect = FSAnnot_CreateAppearanceStream_Rectangle(csAP, crRect, fSlopeAngle);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_Circle(CFX_ByteString& csAP, CPDF_Point& point, FS_FLOAT& fRadius)
{
	csAP = "";

	CPDF_Rect crRect, pdfRect;
	crRect.left = point.x - fRadius;
	crRect.bottom = point.y - fRadius;
	crRect.right = point.x + fRadius;
	crRect.top = point.y + fRadius;
	pdfRect = FSAnnot_CreateAppearanceStream_Ellipse(csAP, crRect);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_Diamond(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect crRect = CPDF_Rect(0,0,0,0);
	CPDF_Rect  pdfRect = CPDF_Rect(0,0,0,0);
	crRect.left = point.x - fWidth / 2.0f;
	crRect.bottom = point.y - fWidth / 2.0f;
	crRect.right = point.x + fWidth / 2.0f;
	crRect.top = point.y + fWidth / 2.0f;

	FX_FLOAT fSlopeAngle = 0.0f;
	if (FSAnnot_Util_Length(direction) < 0.0001f)
		fSlopeAngle = 0.0f;
	else
	{
		fSlopeAngle = (FX_FLOAT)FSAnnot_Util_SlopeAngle(direction);
		if (direction.y < 0.0f) fSlopeAngle *= -1.0f;
	}
	fSlopeAngle += FX_PI / 4.0f;

	pdfRect = FSAnnot_CreateAppearanceStream_Rectangle(csAP, crRect, fSlopeAngle);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_OpenArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect pdfRect;
	double dLength = FSAnnot_Util_Length(direction);
	if (dLength < 0.0001f)
	{
		direction.x = 1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.x = (FX_FLOAT)(direction.x / dLength);
		direction.y = (FX_FLOAT)(direction.y / dLength);
	}

	direction = FSAnnot_Util_Rotate(direction,FX_PI);
	direction.x *= fWidth;
	direction.y *= fWidth;
	CPDF_Point p1(0.0f,0.0f), p2(0.0f,0.0f);
	CPDF_Point rotatedVector = FSAnnot_Util_Rotate(direction, FX_PI / 6.0f);
	p1.x = point.x + rotatedVector.x;
	p1.y = point.y + rotatedVector.y;
	rotatedVector = FSAnnot_Util_Rotate(direction, -FX_PI / 6.0f);
	p2.x = point.x + rotatedVector.x;
	p2.y = point.y + rotatedVector.y;
	csAP.Format("%.3f %.3f m\n%.3f %.3f l\n%.3f %.3f l\n", p1.x, p1.y, point.x, point.y, p2.x, p2.y);
	pdfRect.left = FX_MIN(FX_MIN(p1.x, p2.x), point.x);
	pdfRect.bottom = FX_MIN(FX_MIN(p1.y, p2.y), point.y);
	pdfRect.right = FX_MAX(FX_MAX(p1.x, p2.x), point.x);
	pdfRect.top = FX_MAX(FX_MAX(p1.y, p2.y), point.y);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_ClosedArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect pdfRect;
	FX_FLOAT dLength = FSAnnot_Util_Length(direction);
	if (dLength < 0.0001f)
	{
		direction.x = 1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.x = (FX_FLOAT)(direction.x / dLength);
		direction.y = (FX_FLOAT)(direction.y / dLength);
	}
	direction = FSAnnot_Util_Rotate(direction,FX_PI);
	direction.x *= fWidth;
	direction.y *= fWidth;
	CPDF_Point p1(0.0f,0.0f), p2(0.0f,0.0f);
	CPDF_Point rotatedVector = FSAnnot_Util_Rotate(direction, FX_PI / 6.0f);
	p1.x = point.x + rotatedVector.x;
	p1.y = point.y + rotatedVector.y;
	rotatedVector = FSAnnot_Util_Rotate(direction, -FX_PI / 6.0f);
	p2.x = point.x + rotatedVector.x;
	p2.y = point.y + rotatedVector.y;
	csAP.Format("%.3f %.3f m\n%.3f %.3f l\n%.3f %.3f l\n%.3f %.3f l\n", p1.x, p1.y, point.x, point.y, p2.x, p2.y, p1.x, p1.y);
	pdfRect.left = FX_MIN(FX_MIN(p1.x, p2.x), point.x);
	pdfRect.bottom = FX_MIN(FX_MIN(p1.y, p2.y), point.y);
	pdfRect.right = FX_MAX(FX_MAX(p1.x, p2.x), point.x);
	pdfRect.top = FX_MAX(FX_MAX(p1.y, p2.y), point.y);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_Butt(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect pdfRect;
	FX_FLOAT dLength = FSAnnot_Util_Length(direction);
	if (dLength < 0.0001f)
	{
		direction.x = 1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.x = (FX_FLOAT)(direction.x / dLength);
		direction.y = (FX_FLOAT)(direction.y / dLength);
	}
	direction.x *= fWidth / 2.0f;
	direction.y *= fWidth / 2.0f;
	CPDF_Point p1(0.0f,0.0f), p2(0.0f,0.0f);
	CPDF_Point rotatedVector = FSAnnot_Util_Rotate(direction, FX_PI / 2.0f);
	p1.x = point.x + rotatedVector.x;
	p1.y = point.y + rotatedVector.y;
	rotatedVector = FSAnnot_Util_Rotate(direction, -FX_PI / 2.0f);
	p2.x = point.x + rotatedVector.x;
	p2.y = point.y + rotatedVector.y;
	csAP.Format("%.3f %.3f m\n%.3f %.3f l\n", p1.x, p1.y, p2.x, p2.y);
	pdfRect.left = FX_MIN(p1.x, p2.x);
	pdfRect.bottom = FX_MIN(p1.y, p2.y);
	pdfRect.right = FX_MAX(p1.x, p2.x);
	pdfRect.top = FX_MAX(p1.y, p2.y);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_ROpenArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect pdfRect;
	FX_FLOAT dLength = FSAnnot_Util_Length(direction);
	if (dLength < 0.0001f)
	{
		direction.x = 1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.x = (FX_FLOAT)(direction.x / dLength);
		direction.y = (FX_FLOAT)(direction.y / dLength);
	}
	direction.x *= fWidth;
	direction.y *= fWidth;

	CPDF_Point p1(0.0f,0.0f), p2(0.0f,0.0f);
	CPDF_Point rotatedVector = FSAnnot_Util_Rotate(direction, FX_PI / 6.0f);
	p1.x = point.x + rotatedVector.x;
	p1.y = point.y + rotatedVector.y;

	rotatedVector = FSAnnot_Util_Rotate(direction, -FX_PI / 6.0f);
	p2.x = point.x + rotatedVector.x;
	p2.y = point.y + rotatedVector.y;

	csAP.Format("%.3f %.3f m\n%.3f %.3f l\n%.3f %.3f l\n", p1.x, p1.y, point.x, point.y, p2.x, p2.y);
	pdfRect.left = FX_MIN(FX_MIN(p1.x, p2.x), point.x);
	pdfRect.bottom = FX_MIN(FX_MIN(p1.y, p2.y), point.y);
	pdfRect.right = FX_MAX(FX_MAX(p1.x, p2.x), point.x);
	pdfRect.top = FX_MAX(FX_MAX(p1.y, p2.y), point.y);

	return pdfRect;	
}

CPDF_Rect FSAnnot_CreateLineEndingAP_RClosedArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect pdfRect;
	FX_FLOAT dLength = FSAnnot_Util_Length(direction);
	if (dLength < 0.0001f)
	{
		direction.x = 1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.x = (FX_FLOAT)(direction.x / dLength);
		direction.y = (FX_FLOAT)(direction.y / dLength);
	}
	direction.x *= fWidth;
	direction.y *= fWidth;

	CPDF_Point p1(0.0f,0.0f), p2(0.0f,0.0f);
	CPDF_Point rotatedVector = FSAnnot_Util_Rotate(direction, FX_PI / 6.0f);
	p1.x = point.x + rotatedVector.x;
	p1.y = point.y + rotatedVector.y;
	rotatedVector = FSAnnot_Util_Rotate(direction, -FX_PI / 6.0f);
	p2.x = point.x + rotatedVector.x;
	p2.y = point.y + rotatedVector.y;

	csAP.Format("%.3f %.3f m\n%.3f %.3f l\n%.3f %.3f l\n%.3f %.3f l\n", p1.x, p1.y, point.x, point.y, p2.x, p2.y, p1.x, p1.y);
	pdfRect.left = FX_MIN(FX_MIN(p1.x, p2.x), point.x);
	pdfRect.bottom = FX_MIN(FX_MIN(p1.y, p2.y), point.y);
	pdfRect.right = FX_MAX(FX_MAX(p1.x, p2.x), point.x);
	pdfRect.top = FX_MAX(FX_MAX(p1.y, p2.y), point.y);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateLineEndingAP_Slash(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth)
{
	csAP = "";

	CPDF_Rect pdfRect;
	FX_FLOAT dLength = FSAnnot_Util_Length(direction);
	if (dLength < 0.0001f)
	{
		direction.x = 1.0f;
		direction.y = 0.0f;
	}
	else
	{
		direction.x = (FX_FLOAT)(direction.x / dLength);
		direction.y = (FX_FLOAT)(direction.y / dLength);
	}

	direction.x *= fWidth / 2.0f;
	direction.y *= fWidth / 2.0f;
	CPDF_Point p1(0.0f,0.0f), p2(0.0f,0.0f);
	CPDF_Point rotatedVector = FSAnnot_Util_Rotate(direction, FX_PI / 3.0f);
	p1.x = point.x + rotatedVector.x;
	p1.y = point.y + rotatedVector.y;
	rotatedVector = FSAnnot_Util_Rotate(direction, - FX_PI * 2.0f / 3.0f);
	p2.x = point.x + rotatedVector.x;
	p2.y = point.y + rotatedVector.y;

	csAP.Format("%.3f %.3f m\n%.3f %.3f l\n", p1.x, p1.y, p2.x, p2.y);
	pdfRect.left = FX_MIN(p1.x, p2.x);
	pdfRect.bottom = FX_MIN(p1.y, p2.y);
	pdfRect.right = FX_MAX(p1.x, p2.x);
	pdfRect.top = FX_MAX(p1.y, p2.y);

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateAppearanceStream_Rectangle(CFX_ByteString& csAP, CPDF_Rect& crRect, FX_FLOAT fRotateAngle /* = 0.0f */)
{
	csAP = "";
	CPDF_Rect pdfRect;
	if (crRect.IsEmpty()) return pdfRect;

	FX_FLOAT x, y, fx, fy, px[5], py[5];
	x = (crRect.right + crRect.left) / 2.0f;
	y = (crRect.top + crRect.bottom) / 2.0f;
	px[0] = px[1] = px[4] = crRect.left;
	px[2] = px[3] = crRect.right;
	py[0] = py[3] = py[4] = crRect.top;
	py[1] = py[2] = crRect.bottom;

	int i = 0;
	for (i = 0; i < 5; i ++)
	{
		fx = px[i] - x;
		fy = py[i] - y;

		px[i] = x + fx * (FX_FLOAT)FXSYS_cos(fRotateAngle) - fy * (FX_FLOAT)FXSYS_sin(fRotateAngle);
		py[i] = y + fx * (FX_FLOAT)FXSYS_sin(fRotateAngle) + fy * (FX_FLOAT)FXSYS_cos(fRotateAngle);
	}

	CFX_ByteString csTemp;
	pdfRect.left = pdfRect.right = px[0];
	pdfRect.bottom = pdfRect.top = py[0];
	csAP.Format("%.3f %.3f m\n", px[0], py[0]);
	for (i = 1; i < 5; i ++)
	{
		if (pdfRect.left > px[i]) pdfRect.left = px[i];
		if (pdfRect.bottom > py[i]) pdfRect.bottom = py[i];
		if (pdfRect.right < px[i]) pdfRect.right = px[i];
		if (pdfRect.top < py[i]) pdfRect.top = py[i];

		csTemp.Format("%.3f %.3f l\n", px[i], py[i]);
		csAP += csTemp;
	}

	return pdfRect;
}

CPDF_Rect FSAnnot_CreateAppearanceStream_Ellipse(CFX_ByteString& csAP, CPDF_Rect& crRect, FX_FLOAT fRotateAngle /* = 0.0f */)
{
	csAP = "";
	CPDF_Rect pdfRect;
	if (crRect.IsEmpty()) return pdfRect;

	FX_FLOAT fBezier = 0.2761423749154f;
	FX_FLOAT w, h, x, y, fx, fy, px[13], py[13];
	w = crRect.right - crRect.left;
	h = crRect.top - crRect.bottom;
	x = (crRect.right + crRect.left) / 2.0f;
	y = (crRect.top + crRect.bottom) / 2.0f;
	fx = w * fBezier;
	fy = h * fBezier;

	//MAGICAL CONSTANT to map ellipse to beziers
	//    2/3*(sqrt(2)-1)
	//------------------------/ 
	//　　　　　　　　　　　　/ 
	//　　　　2___3___4　　　 / 
	//　　 1　　　　　　 5　　/ 
	//　　 |　　　　　　 |　　/ 
	//　　 |　　　　　　 |　　/ 
	//　　 0,12　　　　　6　　/ 
	//　　 |　　　　　　 |　　/ 
	//　　 |　　　　　　 |　　/ 
	//　　11　　　　　　 7　　/ 
	//　　　 10___9___8　　　 / 
	//　　　　　　　　　　　　/ 
	//------------------------/
	px[0] = px[1] = px[11] = px[12] = crRect.left;
	px[5] = px[6] = px[7] = crRect.right;
	px[2] = px[10] = x - fx;
	px[4] = px[8] = x + fx;
	px[3] = px[9] = x;
	py[2] = py[3] = py[4] = crRect.top;
	py[8] = py[9] = py[10] = crRect.bottom;
	py[7] = py[11] = y - fy;
	py[1] = py[5] = y + fy;
	py[0] = py[12] = py[6] = y;

	int i;
	for (i = 0; i < 13; i ++)
	{
		fx = px[i] - x;
		fy = py[i] - y;

		px[i] = x + fx * (FX_FLOAT)FXSYS_cos(fRotateAngle) - fy * (FX_FLOAT)FXSYS_sin(fRotateAngle);
		py[i] = y + fx * (FX_FLOAT)FXSYS_sin(fRotateAngle) + fy * (FX_FLOAT)FXSYS_cos(fRotateAngle);

		if (i == 0)
		{
			pdfRect.left = pdfRect.right = px[i];
			pdfRect.bottom = pdfRect.top = py[i];
		}
		else
		{
			if (pdfRect.left > px[i]) pdfRect.left = px[i];
			if (pdfRect.bottom > py[i]) pdfRect.bottom = py[i];
			if (pdfRect.right < px[i]) pdfRect.right = px[i];
			if (pdfRect.top < py[i]) pdfRect.top = py[i];
		}
	}

	CFX_ByteString csTemp;
	csAP.Format("%.3f %.3f m\n", px[0], py[0]);
	for (i = 0; i < 4; i ++)
	{
		csTemp.Format("%.3f %.3f %.3f %.3f %.3f %.3f c\n", px[i * 3 + 1], py[i * 3 + 1], px[i * 3 + 2], py[i * 3 + 2], px[i * 3 + 3], py[i * 3 + 3]);
		csAP += csTemp;
	}

	return pdfRect;
}

//*****************************************************************************
//* Make the appearance of annotation.
//*****************************************************************************
CFSAnnot_DefaultAppearance::CFSAnnot_DefaultAppearance(CFX_ByteString csDA /*= ""*/)
{
	m_csDA = csDA;
}

FX_BOOL CFSAnnot_DefaultAppearance::HasEntry( FX_BSTR csEntry, FX_INT32 nParamNum ) 
{ 
	if ( m_csDA.IsEmpty() ) 
		return FALSE ; 

	CPDF_SimpleParser syntax(m_csDA) ; 

	return syntax.FindTagParam( csEntry, nParamNum ) ; 
} 

CFX_ByteString CFSAnnot_DefaultAppearance::GetEntryString( FX_BSTR csEntry, FX_INT32 nParamNum ) 
{ 
	if ( HasEntry(csEntry, nParamNum) == FALSE ) 
		return "" ; 

	CPDF_SimpleParser syntax(m_csDA) ; 

	FX_DWORD dwStart = syntax.GetPos() ; 
	syntax.SkipWord( csEntry ) ; 
	FX_DWORD dwStop = syntax.GetPos() ; 

	return m_csDA.Mid( dwStart, dwStop-dwStart ) ; 
}

CFX_ByteString CFSAnnot_DefaultAppearance::GetEntryParam( FX_BSTR csEntry, FX_INT32 nParamNum, FX_INT32 nParamIndex ) 
{ 
	if (m_csDA.IsEmpty()) return "";
	if (nParamIndex < 0 || nParamIndex >= nParamNum) return "";

	CPDF_SimpleParser syntax(m_csDA);
	if (!syntax.FindTagParam(csEntry, nParamNum)) return "";
	for (FX_INT32 i = 0; i < nParamNum; i ++)
	{
		CFX_ByteString csParam = (CFX_ByteString)syntax.GetWord();
		if (nParamIndex == i) return csParam;
	}
	return "";
}

void CFSAnnot_DefaultAppearance::SetEntry( FX_BSTR csEntry, FX_INT32 nParamNum, CFX_ByteStringArray& aParams ) 
{
	RemoveEntry( csEntry, nParamNum ) ; 

	if ( !m_csDA.IsEmpty() ) 
		m_csDA += " " ; 

	FX_INT32 nCount = aParams.GetSize() ; 

	for ( FX_INT32 i=0; i<nCount; i++ ) 
	{ 
		m_csDA += aParams[i] ; 
		m_csDA += " " ; 
	} 

	m_csDA += CFX_ByteString(csEntry) ; 
}

void CFSAnnot_DefaultAppearance::RemoveEntry( FX_BSTR csEntry, FX_INT32 nParamNum ) 
{
	CPDF_SimpleParser syntax(m_csDA) ; 

	if ( syntax.FindTagParam(csEntry, nParamNum) ) 
	{ 
		FX_DWORD dwStart = syntax.GetPos() ; 
		syntax.SkipWord(csEntry) ; 
		FX_DWORD dwStop = syntax.GetPos() ; 

		m_csDA.Delete( dwStart, dwStop-dwStart ) ; 
	}
}

FX_BOOL CFSAnnot_DefaultAppearance::HasHorzScale() 
{ 
	return HasEntry( "Tz", 1 ) ; 
} 

CFX_ByteString CFSAnnot_DefaultAppearance::GetHorzScaleString() 
{ 
	CFX_ByteString csScale ; 
	
	if ( m_csDA.IsEmpty() ) 
		return csScale ; 
	
	CPDF_SimpleParser syntax(m_csDA) ; 
	
	if ( syntax.FindTagParam("Tz", 1) ) 
	{
		csScale = (CFX_ByteString)syntax.GetWord() ; 
		csScale += " Tz" ; 
	}
	
	return csScale ; 
} 

void CFSAnnot_DefaultAppearance::GetHorzScale( FX_FLOAT& flScale ) 
{ 
	if ( m_csDA.IsEmpty() ) 
		return ; 
	
	flScale = 100.0f ; 
	
	CPDF_SimpleParser syntax(m_csDA) ; 
	
	if ( syntax.FindTagParam("Tz", 1) ) 
	{ 
		CFX_ByteString csScale = (CFX_ByteString)syntax.GetWord() ; 
		
		flScale = FX_atof( csScale ) ; 
	} 
} 

void CFSAnnot_DefaultAppearance::SetHorzScale( FX_FLOAT  flScale ) 
{ 
	CFX_ByteString csOldDA = GetHorzScaleString() ; 
	CFX_ByteString csNewDA ; 
	
	csNewDA = " " + CFX_ByteString::FormatFloat(flScale) + " Tz" ; 

	if ( csOldDA.IsEmpty() ) 
		m_csDA += csNewDA ; 
	else 
		m_csDA.Replace( csOldDA, csNewDA ) ; 
}

FX_BOOL CFSAnnot_DefaultAppearance::HasCharSpace() 
{ 
	return HasEntry( "Tc", 1 ) ; 
} 

CFX_ByteString CFSAnnot_DefaultAppearance::GetCharSpaceString() 
{
	CFX_ByteString csSpace ; 

	if ( m_csDA.IsEmpty() ) 
		return csSpace ; 

	CPDF_SimpleParser syntax(m_csDA) ; 
	
	if ( syntax.FindTagParam("Tc", 1) ) 
	{
		csSpace = (CFX_ByteString)syntax.GetWord() ; 
		csSpace += " Tc" ; 
	}
	
	return csSpace ; 
} 

void CFSAnnot_DefaultAppearance::GetCharSpace( FX_FLOAT& flSpace ) 
{ 
	if ( m_csDA.IsEmpty() ) 
		return ; 
	
	flSpace = 0.0f ; 
	
	CPDF_SimpleParser syntax(m_csDA) ; 
	
	if ( syntax.FindTagParam("Tc", 1) ) 
	{ 
		CFX_ByteString csSpace = (CFX_ByteString)syntax.GetWord() ; 
		
		flSpace = FX_atof( csSpace ) ; 
	} 
} 

void CFSAnnot_DefaultAppearance::SetCharSpace( FX_FLOAT flSpace ) 
{ 
	CFX_ByteString csOldDA = GetCharSpaceString() ; 
	CFX_ByteString csNewDA ; 
	
	csNewDA = " " + CFX_ByteString::FormatFloat(flSpace) + " Tc" ; 

	if ( csOldDA.IsEmpty() ) 
		m_csDA += csNewDA ; 
	else 
		m_csDA.Replace( csOldDA, csNewDA ) ; 

	m_csDA.Replace( "  ", " " ) ; 
}

FX_BOOL CFSAnnot_DefaultAppearance::HasLineLeading() 
{ 
	return HasEntry( "Tl", 1 ) ; 
} 

CFX_ByteString CFSAnnot_DefaultAppearance::GetLineLeadingString() 
{
	CFX_ByteString csLeading ; 
	
	if ( m_csDA.IsEmpty() ) 
		return csLeading ; 
	
	CPDF_SimpleParser syntax(m_csDA) ; 
	
	if ( syntax.FindTagParam("Tl", 1) ) 
	{
		csLeading = (CFX_ByteString)syntax.GetWord() ; 
		csLeading += " Tl" ; 
	}
	
	return csLeading ; 
} 

void CFSAnnot_DefaultAppearance::GetLineLeading( FX_FLOAT& flLeading ) 
{ 
	if ( m_csDA.IsEmpty() ) 
		return ; 

	flLeading = 0.0f ; 

	CPDF_SimpleParser syntax(m_csDA) ; 
	
	if ( syntax.FindTagParam("Tl", 1) ) 
	{ 
		CFX_ByteString csLeading = (CFX_ByteString)syntax.GetWord() ; 

		flLeading = FX_atof( csLeading ) ; 
	} 
} 

void CFSAnnot_DefaultAppearance::SetLineLeading( FX_FLOAT  flLeading ) 
{ 
	CFX_ByteString csOldDA = GetLineLeadingString() ; 
	CFX_ByteString csNewDA ; 

	csNewDA = " " + CFX_ByteString::FormatFloat(flLeading) + " Tl" ; 

	if ( csOldDA.IsEmpty() ) 
		m_csDA += csNewDA ; 
	else 
		m_csDA.Replace( csOldDA, csNewDA ) ; 
} 

void	CFSAnnot_DefaultAppearance::RemoveColor(FX_BOOL bStrokingOperation)
{
	if (m_csDA.IsEmpty()) return ;

	CPDF_SimpleParser syntax(m_csDA);
	const FS_INT32 nCount = 3;
	CFX_ByteString bsToken[nCount] = {"G", "RG", "K"};
	if (!bStrokingOperation)
	{
		bsToken[0].MakeLower();
		bsToken[1].MakeLower();
		bsToken[2].MakeLower();
	}
	FS_INT32 nParams[nCount] = {1, 3, 4};
	for (FS_INT32 i=0; i<nCount; i++)
	{
		syntax.SetPos(0);
		if (syntax.FindTagParam(bsToken[i], nParams[1])) 
			RemoveEntry(bsToken[i], nParams[1]);
	}
}

void	CFSAnnot_DefaultAppearance::RemoveTextMatrix()
{
	if (m_csDA.IsEmpty()) return ;
	RemoveEntry("Tm", 6);
}

////////////////////////////////////////////////////////////////////////////////////////

void FSANNOT_Util_BeaierCalcControlPoints(CFX_VectorF vertice[4], CFX_VectorF cp[2], FX_FLOAT k)
{
	// calculate the control points between vertice[1] and vertice[2].
	// vertice[0] is the previous vertex, and vertice[3] is the next one.

	FX_FLOAT xc1 = (vertice[0].x + vertice[1].x) / 2.0f;
	FX_FLOAT yc1 = (vertice[0].y + vertice[1].y) / 2.0f;
	FX_FLOAT xc2 = (vertice[1].x + vertice[2].x) / 2.0f;
	FX_FLOAT yc2 = (vertice[1].y + vertice[2].y) / 2.0f;
	FX_FLOAT xc3 = (vertice[2].x + vertice[3].x) / 2.0f;
	FX_FLOAT yc3 = (vertice[2].y + vertice[3].y) / 2.0f;

	FX_FLOAT xDiff, yDiff;
	xDiff = vertice[1].x - vertice[0].x;
	yDiff = vertice[1].y - vertice[0].y;
	FX_FLOAT len1 = FXSYS_sqrt(xDiff * xDiff + yDiff * yDiff);
	xDiff = vertice[2].x - vertice[1].x;
	yDiff = vertice[2].y - vertice[1].y;
	FX_FLOAT len2 = FXSYS_sqrt(xDiff * xDiff + yDiff * yDiff);
	xDiff = vertice[3].x - vertice[2].x;
	yDiff = vertice[3].y - vertice[2].y;
	FX_FLOAT len3 = FXSYS_sqrt(xDiff * xDiff + yDiff * yDiff);

	//FXSYS_assert((len1 + len2) > 0.001f);
	//FXSYS_assert((len2 + len3) > 0.001f);
	FX_FLOAT k1 = 0.5f;
	FX_FLOAT k2 = 0.5f;
	if (len1 + len2 > FSPDFANNOTUTIL_MIN && len2 + len3 > FSPDFANNOTUTIL_MIN) 
	{
		k1 = len1 / (len1 + len2);
		k2 = len2 / (len2 + len3);
	}

	FX_FLOAT xm1 = xc1 + (xc2 - xc1) * k1;
	FX_FLOAT ym1 = yc1 + (yc2 - yc1) * k1;
	FX_FLOAT xm2 = xc2 + (xc3 - xc2) * k2;
	FX_FLOAT ym2 = yc2 + (yc3 - yc2) * k2;

	cp[0].x = xm1 + (xc2 - xm1) * k + vertice[1].x - xm1;
	cp[0].y = ym1 + (yc2 - ym1) * k + vertice[1].y - ym1;
	cp[1].x = xm2 + (xc2 - xm2) * k + vertice[2].x - xm2;
	cp[1].y = ym2 + (yc2 - ym2) * k + vertice[2].y - ym2;
}

void FSANNOT_Util_BezierToStream(const CPDF_Array* pInkList, CFX_ByteTextBuf& bufAp, CPDF_Array* pVerticesList, FX_FLOAT k)
{
	CFX_ByteString tmp;
	int line_count = pInkList->GetCount();
	for (int i = 0; i < line_count; i ++) 
	{
		CPDF_Array* pLine = pInkList->GetArray(i);
		if (!pLine) continue;

		CPDF_Array* pCloneLine =  (CPDF_Array*)pLine->Clone();

		int ptnum = pLine->GetCount() / 2;
		if (ptnum < 1) continue;

		if (ptnum == 1) 
		{
			tmp.Format("%.3f %.3f m %.3f %.3f l\n", 
				pLine->GetNumber(0), pLine->GetNumber(1), 
				pLine->GetNumber(0) + 0.001f, pLine->GetNumber(1) + 0.001f);
			bufAp << tmp;
			continue;
		}
		CFX_VectorF vertice[4], cp[2];
		FX_BOOL bClosed = (pLine->GetNumber(0) == pLine->GetNumber((ptnum-1)*2)
			&& pLine->GetNumber(1) == pLine->GetNumber((ptnum-1)*2+1));

		if (bClosed)
			vertice[0].Set(pLine->GetNumber((ptnum-2)*2), pLine->GetNumber((ptnum-2)*2+1));
		else
			vertice[0].Set(pLine->GetNumber(0), pLine->GetNumber(1));

		CFX_VectorF startPt;
		startPt.Set(pLine->GetNumber(0), pLine->GetNumber(1));
		tmp.Format("%.3f %.3f m\n", startPt.x, startPt.y);
		bufAp << tmp;

		for (int j = 0; j < ptnum - 1; j ++) 
		{
			vertice[1].Set(pLine->GetNumber(j*2), pLine->GetNumber(j*2+1));
			vertice[2].Set(pLine->GetNumber((j+1)*2), pLine->GetNumber((j+1)*2+1));

			if (j == ptnum - 2) 
			{
				if (bClosed)
					vertice[3].Set(pLine->GetNumber(1*2), pLine->GetNumber(1*2+1));
				else
					vertice[3] = vertice[2];
			} else
				vertice[3].Set(pLine->GetNumber((j+2)*2), pLine->GetNumber((j+2)*2+1));

			FSANNOT_Util_BeaierCalcControlPoints(vertice, cp, k);

			pCloneLine->AddNumber(cp[0].x);
			pCloneLine->AddNumber(cp[0].y);
			pCloneLine->AddNumber(cp[1].x);
			pCloneLine->AddNumber(cp[1].y);

			tmp.Format("%.3f %.3f %.3f %.3f %.3f %.3f c\n",
			cp[0].x, cp[0].y, cp[1].x, cp[1].y, vertice[2].x, vertice[2].y);
			bufAp << tmp;

			vertice[0] = vertice[1]; 
			startPt = vertice[2];
		}
		pVerticesList->Add(pCloneLine);
	}
	bufAp << FX_BSTRC("S\n");
}

void FSANNOT_Util_NormalizeInkList(const CPDF_Array * pInkList, CPDF_Array * pNormalizeInkList)
{
	FS_DWORD nPathCount = pInkList->GetCount();
	for (FS_DWORD i = 0; i < nPathCount;i++) {
		CPDF_Array * pSubPath = pInkList->GetArray(i);
		FS_DWORD nPointCount = pSubPath->GetCount() / 2;
		if(nPointCount == 0)
			continue;
		FSPointF prvPoint;
		prvPoint.set(pSubPath->GetNumber(0), pSubPath->GetNumber(1));
		CPDF_Array* pNewSubPath = CPDF_Array::Create();
		pNewSubPath->AddNumber(prvPoint.x);
		pNewSubPath->AddNumber(prvPoint.y);
		FSPointF curPoint;
		for (FS_DWORD j = 1;j < nPointCount;j++) {
			curPoint.set(pSubPath->GetNumber(2 * j), pSubPath->GetNumber(2 * j + 1));
			if(curPoint.x == prvPoint.x && curPoint.y == prvPoint.y)
				continue;
			
			pNewSubPath->AddNumber(curPoint.x);
			pNewSubPath->AddNumber(curPoint.y);
			prvPoint = curPoint;
		} 
		pNormalizeInkList->Add(pNewSubPath);
	}
}

void FSANNOT_Util_LineToStream(const CPDF_Array* pInkList, CFX_ByteTextBuf& bufAp)
{
	CFX_ByteString tmp;
	int line_count = pInkList->GetCount();
	for (int i = 0; i < line_count; i ++) 
	{
		CPDF_Array* pLine = pInkList->GetArray(i);
		if (!pLine) continue;

		int ptnum = pLine->GetCount() / 2;
		if (ptnum < 1) continue;

		if (ptnum == 1)
		{
			tmp.Format("%.3f %.3f m %.3f %.3f l\n", 
				pLine->GetNumber(0), pLine->GetNumber(1), pLine->GetNumber(0), pLine->GetNumber(1));
			bufAp << tmp;
			continue;
		}

		tmp.Format("%.3f %.3f m\n", pLine->GetNumber(0), pLine->GetNumber(1));
		bufAp << tmp;

		for (int j = 1; j < ptnum; j ++)
		{			
			tmp.Format("%.3f %.3f l\n", pLine->GetNumber(j*2), pLine->GetNumber(j*2+1));
			bufAp << tmp;
		}
	}
	bufAp << FX_BSTRC("S\n");
}

void FSANNOT_Util_ExpandRectByPoint(CPDF_Point point, CPDF_Rect& crRect)
{
	crRect.Normalize();
	if (crRect.left > point.x) crRect.left = point.x;
	if (crRect.bottom > point.y) crRect.bottom = point.y;
	if (crRect.right < point.x) crRect.right = point.x;
	if (crRect.top < point.y) crRect.top = point.y;
}

void FSANNOT_Util_ExpandRectByPoint(FSPointF point, CPDF_Rect& crRect)
{
	CPDF_Point pt = CPDF_Point(point.x, point.y);
	FSANNOT_Util_ExpandRectByPoint(pt, crRect);
}

CPDF_Rect FSAnnot_CreateCloudyAP_Polygon(CFX_ByteTextBuf& csAP, FSPDFAnnot_Path& ctrlPoints, FS_FLOAT fIntensity)
{
	CPDF_Rect pdfRect, cloudyRect;
	FS_INT32 iCloudyNum = FSAnnot_CalculateCloudyNumberFromIntensity(fIntensity);
	if (ctrlPoints.GetSize() < 2 || iCloudyNum <= 0) return pdfRect;

	CPDF_Point pdfPoint1(0.0f,0.0f), pdfPoint2(0.0f,0.0f), pdfPoint3(0.0f,0.0f);
	CPDF_Point v1(0.0f,0.0f), v2(0.0f,0.0f);
	FSPDFAnnot_Path cPath;
	FX_DWORD i, j, dwCount;
	FX_FLOAT fTotalLen, fStep, fOver, angle, radius;

	fTotalLen = 0.0f;
	pdfPoint1 = ctrlPoints[0];
	pdfRect.left = pdfRect.right = pdfPoint1.x;
	pdfRect.bottom = pdfRect.top = pdfPoint1.y;
	dwCount = ctrlPoints.GetSize();
	for (i = 1; i <= dwCount; i ++)
	{
		pdfPoint2 = ctrlPoints[i % dwCount];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		fTotalLen += (FX_FLOAT)FSANNOT_Util_Length(v1);

		if (pdfRect.left > pdfPoint2.x) pdfRect.left = pdfPoint2.x;
		if (pdfRect.bottom > pdfPoint2.y) pdfRect.bottom = pdfPoint2.y;
		if (pdfRect.right < pdfPoint2.x) pdfRect.right = pdfPoint2.x;
		if (pdfRect.top < pdfPoint2.y) pdfRect.top = pdfPoint2.y;

		pdfPoint1 = pdfPoint2;
	}
	radius = fTotalLen / (FX_FLOAT)iCloudyNum;
	fStep = 0.0f;
	pdfPoint1 = ctrlPoints[0];
	pdfPoint2 = ctrlPoints[1];
	v1.x = pdfPoint2.x - pdfPoint1.x;
	v1.y = pdfPoint2.y - pdfPoint1.y;
	fTotalLen = (FX_FLOAT)FSANNOT_Util_Length(v1);
	fOver = 0.0f;
	for (i = 0, j = 0; i < iCloudyNum; i ++)
	{
		if (fStep > fTotalLen)
		{
			fOver = fTotalLen;
			j ++;
			pdfPoint1 = ctrlPoints[j];
			pdfPoint2 = ctrlPoints[(j + 1) % dwCount];
			v1.x = pdfPoint2.x - pdfPoint1.x;
			v1.y = pdfPoint2.y - pdfPoint1.y;
			fTotalLen += (FX_FLOAT)FSANNOT_Util_Length(v1);
		}

		v2 = v1;
		double dLength = FSANNOT_Util_Length(v2);
		if (dLength >0.0f)
		{
			v2.x = (FX_FLOAT)((double)v2.x / dLength);
			v2.y = (FX_FLOAT)((double)v2.y / dLength);
		}

		v2.x *= (fStep - fOver);
		v2.y *= (fStep - fOver);
		pdfPoint3.x = pdfPoint1.x + v2.x;
		pdfPoint3.y = pdfPoint1.y + v2.y;
		cPath.Add(pdfPoint3);

		fStep += radius;
	}

	radius = 0.0f;
	pdfPoint1 = cPath[0];
	iCloudyNum = cPath.GetSize();
	for (i = 1; i <= iCloudyNum; i ++)
	{
		pdfPoint2 = cPath[i % iCloudyNum];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		if (radius < FSANNOT_Util_Length(v1))
			radius = (FX_FLOAT)FSANNOT_Util_Length(v1);
		pdfPoint1 = pdfPoint2;
	}
	radius = radius * 5.0f / 8.0f;

	FX_FLOAT startAngle, stopAngle;
	FX_BOOL bHasM = TRUE;
	for (i = 0; i < iCloudyNum; i ++)
	{
		pdfPoint1 = cPath[i];
		pdfPoint2 = cPath[(iCloudyNum - 1 + i) % iCloudyNum];
		pdfPoint3 = cPath[(i + 1) % iCloudyNum];
		v1.x = pdfPoint2.x - pdfPoint1.x;
		v1.y = pdfPoint2.y - pdfPoint1.y;
		if (FSANNOT_Util_Length(v1) == 0) 
			continue;
		v2.x = pdfPoint3.x - pdfPoint1.x;
		v2.y = pdfPoint3.y - pdfPoint1.y;
		if (FSANNOT_Util_Length(v2) == 0) 
			continue;

		startAngle = (FX_FLOAT)FSAnnot_Util_SlopeAngle(v1);
		if (v1.y < 0.0f) startAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(FSANNOT_Util_Length(v1) / (2.0f * radius));
		startAngle += angle - FPDFANNOTUTIL_PI / 9.0f;
		stopAngle = (FX_FLOAT)FSAnnot_Util_SlopeAngle(v2);
		if (v2.y < 0.0f) stopAngle *= -1.0f;
		angle = (FX_FLOAT)FXSYS_acos(FSANNOT_Util_Length(v2) / (2.0f * radius));
		stopAngle -= angle;
		if (stopAngle < startAngle) stopAngle += FPDFANNOTUTIL_PI * 2.0f;

		cloudyRect.left = pdfPoint1.x - radius;
		cloudyRect.bottom = pdfPoint1.y - radius;
		cloudyRect.right = pdfPoint1.x + radius;
		cloudyRect.top = pdfPoint1.y + radius;
		cloudyRect = FSAnnot_CreateCloudyAP_Arc(csAP, cloudyRect, startAngle, stopAngle, bHasM);
		if (bHasM) bHasM = FALSE;
		pdfRect.Union(cloudyRect);

		v1.x = pdfPoint1.x - pdfPoint3.x;
		v1.y = pdfPoint1.y - pdfPoint3.y;
		startAngle = (FX_FLOAT)FSAnnot_Util_SlopeAngle(v1);
		if (v1.y < 0.0f) startAngle *= -1.0f;
		angle = (FX_FLOAT)acos(FSANNOT_Util_Length(v1) / (2.0f * radius));
		startAngle += angle;
		stopAngle = startAngle - FPDFANNOTUTIL_PI / 9.0f;
		cloudyRect.left = pdfPoint3.x - radius;
		cloudyRect.bottom = pdfPoint3.y - radius;
		cloudyRect.right = pdfPoint3.x + radius;
		cloudyRect.top = pdfPoint3.y + radius;
		FSAnnot_CreateCloudyAP_Arc(csAP, cloudyRect, startAngle, stopAngle, FALSE);
	}

	return pdfRect;	
}

FS_BOOL FSANNOT_Util_IsStateValueValid(FSM_ANNOTSTATEMODEL model, FSM_ANNOTSTATE state)
{
	if (model<FSAnnot::e_annotStateModelMarked || model>FSAnnot::e_annotStateModelReview
		|| state<FSAnnot::e_annotStateMarked || state>FSAnnot::e_annotStateNone)
		return FALSE;
	if (FSAnnot::e_annotStateModelMarked == model)
	{
		switch(state)
		{
		case FSAnnot::e_annotStateMarked:
		case FSAnnot::e_annotStateUnmarked:
			return TRUE;
		}
	}
	else
	{//FSAnnot::e_annotStateModelReview
		switch(state)
		{
		case FSAnnot::e_annotStateAccepted:
		case FSAnnot::e_annotStateCancelled:
		case FSAnnot::e_annotStateCompleted:
		case FSAnnot::e_annotStateRejected:
		case FSAnnot::e_annotStateNone:
			return TRUE;
		}
	}
	return FALSE;
}

FS_BOOL FSANNOT_Util_IsSignatureField(CPDF_Dictionary* pAnnotDict)
{
	if (!pAnnotDict) return FALSE;
	CFX_ByteString bsAnnotType = pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE);
	if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_WIDGET))
	{
		CFX_ByteString bsFieldType = pAnnotDict->GetString("FT");
		if (bsFieldType.Equal("Sig"))
			return TRUE;
	}
	return FALSE;
}

//due to complex reason, FSSignatureImp uses FSFormControlImp as a member.
FSFormControlImp* FSANNOT_Util_GetFormControlImp(FSAnnot* pAnnot)
{
    if (!pAnnot)
        return NULL;
    
    FSFormControlImp* pControl = NULL;
    if (FSANNOT_Util_IsSignatureField((CPDF_Dictionary*)pAnnot->getDict()))
#if defined(ENABLE_SIGNATURE)
        pControl = ((FSSignatureImp*)(FSSignature*)pAnnot)->getFormControlImp();
#else
		pControl = NULL;
#endif 
    else
        pControl = (FSFormControlImp*)(FSFormControl*)pAnnot;
    return pControl;
}


