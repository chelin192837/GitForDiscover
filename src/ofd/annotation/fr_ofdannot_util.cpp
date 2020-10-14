#include "fr_ofdannot_util.h"
#include "../../annotation/fs_annot_util.h"
#include "../fr_ofdutil.h"
#include "../../../include/fs_pdfpage.h"
#include "./fr_ofdannotImp.h"

OFD_ANNOTTYPE FR_OFDANNOT_Util_ConvertAnnotTypeFromEnumToOFDType(FSM_ANNOTTYPE annotType) {
    OFD_ANNOTTYPE result = OFD_ANNOTTYPE_Unknown;
    switch (annotType) {
        case FSAnnot::e_annotStamp:
            result = OFD_ANNOTTYPE_Stamp;
            break;
        case FSAnnot::e_annotLine:
        case FSAnnot::e_annotInk:
        case FSAnnot::e_annotFreeText:
            result = OFD_ANNOTTYPE_Path;
            break;
        default:
            break;
    }
    return result;
}

COFD_WriteAnnot *FR_OFDANNOT_UTIL_InsertAnnot(FSM_ANNOTTYPE annotType, const FS_INT32 annotIndex,
                                              const FSRectF annotRect, IOFD_Page *pPageDict,
                                              IOFD_Document *pDoc,
                                              FS_BOOL bRDKTrialWM /*= FALSE*/) {
    IOFD_WriteDocument *writeDoc = OFD_WriteDoc_Create(pDoc);

    OFD_ANNOTTYPE ofdAnnotType = FR_OFDANNOT_Util_ConvertAnnotTypeFromEnumToOFDType(annotType);
    COFD_WriteAnnot *writeAnnot = OFD_WriteAnnot_Create(writeDoc, ofdAnnotType);

    CFX_ByteString strAnnotType = FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(annotType);
    writeAnnot->SetSubType(CFX_WideString::FromUTF8((FX_LPCSTR) strAnnotType));

    CFX_FloatRect fxRectF(annotRect.left, annotRect.bottom, annotRect.right, annotRect.top);
    // need convert to OFD coordinator
    CFX_RectF bound;
    CoordinatorAssistant assistant(pPageDict);
    assistant.fromPDF(fxRectF, bound);

    writeAnnot->SetBoundary(bound);
    // add by gm 20170722, add required properties
    writeAnnot->SetCreator((FX_LPCWSTR) L"HTFX");
    fxutil::CFX_Unitime md;
    md.Now();
    writeAnnot->SetLastModDate(md);
    // add end 20170722

    int pageindex = pDoc->GetPageIndex(pPageDict);
    IOFD_WritePageAnnots *pageAnnots = writeDoc->CreatePageAnnots(pageindex);
    int index = pageAnnots->InsertAnnot(writeAnnot, annotIndex);
    return writeAnnot;
}

/*
FSAnnot* FR_OFDANNOT_UTIL_InsertAnnot(int annotIndex, CPDF_Dictionary* pAnnotDict, FSPDFPage* pPage)
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
*/


void FR_OFDANNOT_UTIL_SetDefaultProperties(
        FSAnnot *pAnnot) {// All the default value of properties are based on Phantom 8.0.
    if (!pAnnot) return;
    FS_DWORD dwColor = 0xFF000000;
    FS_FLOAT fDashes[16] = {-1};
    FSBorderInfo borderInfo;
    borderInfo.set(1.0, FSAnnot::e_borderStyleSolid, 0, 0, fDashes);
    switch (pAnnot->getType()) {
        /*
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
        // */
        default: //Here, unsupport type
            return;
    }
}


FS_BOOL FRANNOT_Util_IsSignatureField(COFD_Annot *pAnnotDict) {
    /*
    if (!pAnnotDict) return FALSE;
    CFX_ByteString bsAnnotType = pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE);
    if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_WIDGET))
    {
        CFX_ByteString bsFieldType = pAnnotDict->GetString("FT");
        if (bsFieldType.Equal("Sig"))
            return TRUE;
    }
    // */
    return FALSE;
}

FX_BOOL
FRANNOT_Util_CheckAnnotIsAtPoint(FSAnnot *pAnnot, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance,
                                 const FSMatrix *matrix) {
    if (!pAnnot) return FALSE;
    COFD_Annot *pAnnotDict = ((FROFDAnnotImp *) pAnnot)->GetOFDAnnot();
    if (!pAnnotDict) return FALSE;

    FS_FLOAT fx = x, fy = y, ft = tolerance;
    if (matrix) {
        CFX_Matrix m(matrix->a, matrix->b, matrix->c, matrix->d, matrix->e, matrix->f);
        CFX_Matrix rm;
        rm.SetReverse(m);
        rm.TransformPoint(fx, fy);
        ft = rm.TransformDistance(ft);
    }
    FSM_ANNOTTYPE annotType = pAnnot->getType();
    switch (annotType) {
        /*
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
        // */
        default: {
            int nPageRotate = pAnnot->getPage()->getRotation() * 90;
            return FRANNOT_Util_IsPointInRect(pAnnot, annotType, fx, fy, ft, nPageRotate, matrix);
        }
    };

    return FALSE;
}

FX_BOOL
FRANNOT_Util_IsPointInRect(FSAnnot *pAnnotDict, FSM_ANNOTTYPE annotType, FS_FLOAT x, FS_FLOAT y,
                           FS_FLOAT tolerance, FS_INT32 pageRotate, const FSMatrix *pSrcMatrix) {
    if (!pAnnotDict) return FALSE;

    FS_FLOAT fIconWidth = 0;
    FS_FLOAT fIconHeight = 0;

    FSRectF pdfRect = pAnnotDict->getRect();
    CPDF_Rect annotRect(pdfRect.left, pdfRect.top, pdfRect.right, pdfRect.bottom);
    /*
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
    // */

    CFX_Matrix matrix;
    if (pSrcMatrix) {
        matrix = CFX_Matrix(pSrcMatrix->a, pSrcMatrix->b, pSrcMatrix->c, pSrcMatrix->d,
                            pSrcMatrix->e, pSrcMatrix->f);
    }

    FX_FLOAT scale_x = 1.0f, scale_y = 1.0f;
    /*
    if (fIconWidth != 0 && fIconHeight != 0)
    {
        FX_FLOAT fWidth = annotRect.Width();
        FX_FLOAT fHeight = annotRect.Height();

        scale_x = fIconWidth / fWidth;
        scale_y = fIconHeight / fHeight;
    }
    else
        return FALSE;
    // */
    float factor_x = 0;
    float factor_y = 0;
    if (matrix.Is90Rotated()) {
        factor_x = 1.0f / FXSYS_fabs(matrix.c) * scale_y;
        factor_y = 1.0f / FXSYS_fabs(matrix.b) * scale_x;
    } else {
        factor_x = 1.0f / FXSYS_fabs(matrix.a) * scale_x;
        factor_y = 1.0f / FXSYS_fabs(matrix.d) * scale_y;
    }
    float x1, x2;
    float y1, y2;
    switch (pageRotate) {
        case 0: {
            x1 = x2 = annotRect.left;
            y1 = y2 = annotRect.top;
        }
            break;
        case 90: {
            x1 = x2 = annotRect.left;
            y1 = y2 = annotRect.bottom;
        }
            break;
        case 180: {
            x1 = x2 = annotRect.right;
            y1 = y2 = annotRect.bottom;
        }
            break;
        case 270: {
            x1 = x2 = annotRect.right;
            y1 = y2 = annotRect.top;
        }
            break;
    }
    matrix.TransformPoint(x1, y1);
    matrix.Scale(factor_x, factor_y);
    matrix.TransformPoint(x2, y2);
    matrix.Translate(x1 - x2, y1 - y2);

    annotRect.Transform(&matrix);
    FX_RECT annotDeviceRect = annotRect.GetClosestRect();

    CFX_Matrix srcMatrix;
    if (pSrcMatrix) {
        srcMatrix = CFX_Matrix(pSrcMatrix->a, pSrcMatrix->b, pSrcMatrix->c, pSrcMatrix->d,
                               pSrcMatrix->e, pSrcMatrix->f);
    }
    srcMatrix.TransformPoint(x, y);
    int transTolerance = (int) srcMatrix.TransformDistance(tolerance);

    annotDeviceRect.left -= transTolerance;
    annotDeviceRect.right += transTolerance;
    annotDeviceRect.top -= transTolerance;
    annotDeviceRect.bottom += transTolerance;

    if (annotDeviceRect.Contains((int) x, (int) y))
        return TRUE;
    else
        return FALSE;
}
