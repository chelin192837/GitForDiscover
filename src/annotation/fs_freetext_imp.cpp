//#include "../../include/fs_pdfpage.h"
#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/commonImp.h"
#include "../inc/fs_pdfdocImp.h"

#include "../widget/pdfwindow/include/pdfwindow/pwl_wnd.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_fontmap.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_edit.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_utils.h"
#include "../widget/pdfwindow/include/fx_systemhandlerimp.h"

#include "../widget/ifpdf_widget.h"
#include "../widget/fpwl_edit.h"

#include "fpdf_richtext.h"

/////////////////////////////////
// Class CFT_Edit
CFT_Edit::CFT_Edit(FSPDFFreeTextImp* pAnnot, FX_BOOL bTextOverflow)
	:m_pFreeTextAnnot(pAnnot)
	,m_pEdit(NULL)
	//,m_pSpellCheck(NULL)
	,m_pFontMap(NULL)
	,m_pSystemHandler(NULL)
	,m_bRichText(FALSE)
{
	CFX_ByteString bsFontName; FX_FLOAT fontSize = 0.0f;
	FX_FLOAT fCharSpace = 0.0f;
	FX_FLOAT fHorzScale = 100;
	FX_FLOAT fLineLeading = 0.0f;
	pAnnot->getFontInfo(bsFontName, fontSize);
	// Not care about Tz,Tc --  Adobe does not take care of Tz and Tc when reseting ap of freetext as well. (SDK-8430)
	CFX_ByteString bsDA = pAnnot->getAnnotImp().getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);
	if (!bsDA.IsEmpty())
	{
		CFSAnnot_DefaultAppearance da(bsDA);
	//	da.GetCharSpace(fCharSpace);
	//	da.GetHorzScale(fHorzScale);
		da.GetLineLeading(fLineLeading);
	}

	m_pEdit = IFSPDF_Edit::Create();
	CFSPDF_WidgetProperties property;
	property.m_rtWidget = GetEditBBox(bTextOverflow);
	property.m_pSystemHandler = GetSystemHandler();	
	property.m_fFontName = bsFontName;
	property.m_fFontSize = fontSize;
	property.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_MultiLine; //multiple lines supported
	property.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_WantReturn;

	switch(pAnnot->getAlignment())
	{
	case 1:
		property.m_dwStyles |= FPDFW_WGTSTYLE_CenterAlignment;
		property.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_HCenter;
		break;
	case 2:
		property.m_dwStyles |= FPDFW_WGTSTYLE_RightAlignment;
		property.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_HFar;
		break;
	default:
		property.m_dwStyles |= FPDFW_WGTSTYLE_LeftAlignment;
		property.m_dwStyleExes |= FPDFW_STYLEEXT_EDT_HNear;
		break;
	}
	m_pEdit->Initialize((FSFreeText*)pAnnot, &property);

	m_pEdit->Update();

//#ifndef _NORICHTEXT_
//	CFX_WideString richText = pAnnot->GetRichText();	
//
//	if (!richText.IsEmpty())
//	{
//		CPWL_Wnd* pWnd = ((CFPWL_Edit*)m_pEdit)->m_pWnd;
//		if (!pWnd) return;
//
//		IFX_Edit* pEdit = ((CPWL_Edit*)pWnd)->GetEdit();
//		if (NULL == pEdit) return;
//
//		pEdit->SetRichText();
//		m_bRichText = TRUE;
//
//		CFX_ByteString bsGetDS;
//		pAnnot->GetDefaultStyle(bsGetDS);
//
//		CFX_WideString wsDS = CFX_WideString::FromLocal(bsGetDS);
//
//		FX_BOOL bAutoAlign = TRUE;
//		CFX_ByteString bsIT = m_pFreeTextAnnot->GetAnnotDict()->GetString("IT", "");
//
//		if (bsIT == FX_BSTRC("FreeTextTypewriter"))
//			bAutoAlign = FALSE;
//
//		m_pEdit->SetRichText(richText, wsDS, bAutoAlign);
//	}
//	else
//	{
//#endif
		//Not care about Tc and Tz -- adobe does not use them when resetting ap as well.(SDK-8430)
		//m_pEdit->SetCharSpace(fCharSpace);
		//m_pEdit->SetHorzScale(fHorzScale);
		m_pEdit->SetLineLeading(fLineLeading);	

		m_pEdit->SetText(pAnnot->getAnnotImp().getUnicodeString(FSANNOT_STRINGKEY_CONTENT));
//#ifndef _NORICHTEXT_
//	}
//#endif
}

CFT_Edit::~CFT_Edit()
{
	if (m_pSystemHandler)
	{
		m_pSystemHandler->Release();
		m_pSystemHandler = NULL;
	}
	if (m_pEdit)
	{
		m_pEdit->Release();
		m_pEdit = NULL;
	}
	if (m_pFontMap)
	{
		delete m_pFontMap;
		m_pFontMap = NULL;
	}
}

IFX_SystemHandler* CFT_Edit::GetSystemHandler()
{
	if (!m_pSystemHandler)
		m_pSystemHandler = FX_NEW FX_SystemHandlerImp(NULL);
	return m_pSystemHandler;
}

void CFT_Edit::OnSetFocus(CPWL_Wnd* pWnd)
{

}

void CFT_Edit::OnKillFocus(CPWL_Wnd* pWnd)
{
}

CPDF_Matrix	CFT_Edit::GetWindowMatrix(void* pAttachedData)
{
	CPDF_Matrix mt;
	return mt;
}

CFX_WideString CFT_Edit::LoadPopupMenuString(FX_INT32 nIndex)
{
	return L"";
}

FX_BOOL CFT_Edit::CanUndo(CPWL_Edit* pEdit)
{
	return FALSE;
}

FX_BOOL	CFT_Edit::CanRedo(CPWL_Edit* pEdit)
{
	return FALSE;
}

void CFT_Edit::Undo(CPWL_Edit* pEdit)
{

}

void CFT_Edit::Redo(CPWL_Edit* pEdit)
{

}

FSAnnot* CFT_Edit::GetAnnot() const
{
	return (FSFreeText*)m_pFreeTextAnnot;
}

CPDF_Rect CFT_Edit::GetEditBBox(FX_BOOL bTextOverflow) 
{
	if (!m_pFreeTextAnnot) return CPDF_Rect(0, 0, 0, 0);
	CFX_ByteString sIT = m_pFreeTextAnnot->getAnnotImp().getStringImpl(FSANNOT_NAMEKEY_INTENT);

	if (sIT == FX_BSTRC("FreeTextTypewriter"))
		return GetTWEditBBox(bTextOverflow);
	else if (sIT == FX_BSTRC("FreeTextCallout"))
		return GetTBCOEditBBox(bTextOverflow);
	else
		//Same as typewriter for a temporary solution, for the edit box doesn't support auto wrap, to do...
		return GetTWEditBBox(bTextOverflow);

	return CPDF_Rect(0, 0, 0, 0);
}

CPDF_Rect FTEdit_ClipPageRect(CPDF_Rect rcPage, CPDF_Rect rcChild, FX_BOOL bCut)
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

CPDF_Rect CFT_Edit::GetTWEditBBox(FX_BOOL bTextOverflow) 
{
	if (!m_pFreeTextAnnot) return CPDF_Rect(0, 0, 0, 0);

	CPDF_Rect rcText = m_pFreeTextAnnot->getTextRect();

	CPDF_Rect rcBBox;

	CPDF_Page* pPage = ((FSPDFPageImp*)m_pFreeTextAnnot->getPage())->getPage();
	if (!pPage) return rcBBox;

	CPDF_Dictionary* pPageDict = pPage->m_pFormDict;
	FS_ROTATION rotate = m_pFreeTextAnnot->getPage()->getRotation();

	FX_INT32 align = m_pFreeTextAnnot->getAlignment();
	if (bTextOverflow)
	{
		CPDF_Rect rcMediaBox = pPageDict->GetRect(FX_BSTRC("MediaBox"));

		CPDF_Rect rcPageBBox = pPage->GetPageBBox();

		FX_FLOAT fMaxSize = FX_MAX(rcMediaBox.Width(), rcMediaBox.Height());
		if(fMaxSize < 0.1)
			fMaxSize = FX_MAX(rcPageBBox.Width(), rcPageBBox.Height());

		switch (rotate)
		{
		case FSCommonDefines::e_rotation0:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - fMaxSize, fMaxSize, 0);	
				break;
			case 1:
				{
					FX_FLOAT fMidX = (rcText.left + rcText.right) / 2.0f;
					FX_FLOAT fMinWidth = 0.0f;

					if (fMidX < (rcPageBBox.left + rcPageBBox.right) / 2.0f)
						fMinWidth = fMidX - rcPageBBox.left;
					else
						fMinWidth = rcPageBBox.right - fMidX;

					rcBBox = CPDF_Rect(0 - fMinWidth, 0 - fMaxSize, fMinWidth, 0);
				}
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - fMaxSize, 0 - fMaxSize, 0, 0);
				break;
			}
			break;
		case FSCommonDefines::e_rotation90:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - fMaxSize, fMaxSize, 0);
				break;
			case 1:
				{
					FX_FLOAT fMidY = (rcText.top + rcText.bottom) / 2.0f;
					FX_FLOAT fMinWidth = 0.0f;

					if (fMidY < (rcPageBBox.top + rcPageBBox.bottom) / 2.0f)
						fMinWidth = fMidY - rcPageBBox.bottom;
					else
						fMinWidth = rcPageBBox.top - fMidY;

					rcBBox = CPDF_Rect(0 - fMinWidth, 0 - fMaxSize, 0 + fMinWidth, 0);
				}
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - fMaxSize, 0 - fMaxSize, 0, 0);
				break;
			}
			break;
		case FSCommonDefines::e_rotation180:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - fMaxSize, fMaxSize, 0);
				break;
			case 1:
				{
					FX_FLOAT fMidX = (rcText.left + rcText.right) / 2.0f;
					FX_FLOAT fMinWidth = 0.0f;

					if (fMidX < (rcPageBBox.left + rcPageBBox.right) / 2.0f)
						fMinWidth = fMidX - rcPageBBox.left;
					else
						fMinWidth = rcPageBBox.right - fMidX;

					rcBBox = CPDF_Rect(0 - fMinWidth, 0 - fMaxSize, 0 + fMinWidth, 0);
				}
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - fMaxSize, 0 - fMaxSize, 0, 0);
				break;
			}
			break;
		case FSCommonDefines::e_rotation270:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - fMaxSize, fMaxSize, 0);
				break;
			case 1:
				{
					FX_FLOAT fMidY = (rcText.top + rcText.bottom) / 2.0f;
					FX_FLOAT fMinWidth = 0.0f;

					if (fMidY < (rcPageBBox.top + rcPageBBox.bottom) / 2.0f)
						fMinWidth = fMidY - rcPageBBox.bottom;
					else
						fMinWidth = rcPageBBox.top - fMidY;

					rcBBox = CPDF_Rect(0 - fMinWidth, 0 - fMaxSize, 0 + fMinWidth, 0);
				}
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - fMaxSize, 0 - fMaxSize, 0, 0);
				break;
			}
			break;
		}

		CPDF_Matrix mt;
		mt.SetReverse(GetCurMatrix());

		mt.TransformRect(rcPageBBox);

		rcBBox = FTEdit_ClipPageRect(rcPageBBox, rcBBox, TRUE);
	}
	else
	{
		switch (rotate)
		{
		case FSCommonDefines::e_rotation0:
		case FSCommonDefines::e_rotation180:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - rcText.Height(), rcText.Width(), 0);	
				break;
			case 1:
				rcBBox = CPDF_Rect(0 - rcText.Width() / 2.0f, 0 - rcText.Height(), rcText.Width() / 2.0f, 0);
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - rcText.Width(), 0 - rcText.Height(), 0, 0);
				break;
			}
			break;
		case FSCommonDefines::e_rotation90:
		case FSCommonDefines::e_rotation270:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - rcText.Width(), rcText.Height(), 0);	
				break;
			case 1:
				rcBBox = CPDF_Rect(0 - rcText.Height() / 2.0f, 0 - rcText.Width(), rcText.Height() / 2.0f, 0);
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - rcText.Height(), 0 - rcText.Width(), 0, 0);
				break;
			}
			break;
		}
	}

	return rcBBox;
}

CPDF_Rect CFT_Edit::GetTBCOEditBBox(FX_BOOL bTextOverflow) 
{
	if (!m_pFreeTextAnnot) return CPDF_Rect(0, 0, 0, 0);

	CPDF_Rect rcText = m_pFreeTextAnnot->getTextRect();
	rcText.Inflate(0.5, 0.5); //to avoid that sometimes content of a callout will split lines. see jira MOB-87.
	CPDF_Rect rcBBox;

	CPDF_Page* pPage = ((FSPDFPageImp*)m_pFreeTextAnnot->getPage())->getPage();
	if (!pPage) return CPDF_Rect(0, 0, 0, 0);
	CPDF_Dictionary* pPageDict = pPage->m_pFormDict;
	FS_ROTATION rotate = m_pFreeTextAnnot->getPage()->getRotation();
	int align = m_pFreeTextAnnot->getAlignment();
	if (bTextOverflow)
	{

		CPDF_Rect rcMediaBox = pPageDict->GetRect(FX_BSTRC("MediaBox"));
		if (rcMediaBox.IsEmpty()) {
			// Mediabox is not present, Adobe treats it as Letter (8.5'x11')
			rcMediaBox = CFX_FloatRect(0, 0, 612, 792);			
		}

		FX_FLOAT fMaxSize = FX_MAX(rcMediaBox.Width(), rcMediaBox.Height());

		switch (rotate)
		{
		case FSCommonDefines::e_rotation0:
		case FSCommonDefines::e_rotation180:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - fMaxSize, rcText.Width(), 0);	
				break;
			case 1:
				rcBBox = CPDF_Rect(0 - rcText.Width() / 2.0f, 0 - fMaxSize, rcText.Width() / 2.0f, 0);
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - rcText.Width(), 0 - fMaxSize, 0, 0);
				break;
			}
			break;
		case FSCommonDefines::e_rotation90:
		case FSCommonDefines::e_rotation270:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - fMaxSize, rcText.Height(), 0);
				break;
			case 1:
				rcBBox = CPDF_Rect(0 - rcText.Height() / 2.0f, 0 - fMaxSize, 
					0 + rcText.Height() / 2.0f, 0);
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - rcText.Height(), 0 - fMaxSize, 0, 0);
				break;
			}
			break;		
		}

		CPDF_Rect rcPageBBox = pPage->GetPageBBox();

		CPDF_Matrix mt;
		mt.SetReverse(GetCurMatrix());

		mt.TransformRect(rcPageBBox);

		rcBBox = FTEdit_ClipPageRect(rcPageBBox, rcBBox, TRUE);

		float nBorderWidth = m_pFreeTextAnnot->getAnnotImp().getBorderWidth();
		CPDF_Rect rcRD = m_pFreeTextAnnot->getAnnotImp().getRectImp(FSANNOT_RECTKEY_RD);
		rcBBox.bottom += (nBorderWidth + rcRD.bottom);
	}
	else
	{
		switch (rotate)
		{
		case FSCommonDefines::e_rotation0:
		case FSCommonDefines::e_rotation180:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - rcText.Height(), rcText.Width(), 0);	
				break;
			case 1:
				rcBBox = CPDF_Rect(0 - rcText.Width() / 2.0f, 0 - rcText.Height(), rcText.Width() / 2.0f, 0);
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - rcText.Width(), 0 - rcText.Height(), 0, 0);
				break;
			}
			break;
		case FSCommonDefines::e_rotation90:
		case FSCommonDefines::e_rotation270:
			switch (align)
			{
			case 0:
				rcBBox = CPDF_Rect(0, 0 - rcText.Width(), rcText.Height(), 0);	
				break;
			case 1:
				rcBBox = CPDF_Rect(0 - rcText.Height() / 2.0f, 0 - rcText.Width(), rcText.Height() / 2.0f, 0);
				break;
			case 2:
				rcBBox = CPDF_Rect(0 - rcText.Height(), 0 - rcText.Width(), 0, 0);
				break;
			}
			break;
		}
	}

	return rcBBox;
}

CPDF_Point CFT_Edit::GetEditOrigin() const
{
	if (!m_pFreeTextAnnot) return CPDF_Point(0, 0);
	CPDF_Rect rcText = m_pFreeTextAnnot->getTextRect();
	CPDF_Page* pPage = ((FSPDFPageImp*)m_pFreeTextAnnot->getPage())->getPage();

	CPDF_Point ptOrigin(0.0f, 0.0f);
	int align = m_pFreeTextAnnot->getAlignment();
	FS_ROTATION rotate = m_pFreeTextAnnot->getPage()->getRotation();

	switch (rotate)
	{
	default:
	case FSCommonDefines::e_rotation0:
		switch (align)
		{
		case 0:
			ptOrigin = 	CPDF_Point(rcText.left, rcText.top);
			break;
		case 1:
			ptOrigin = 	CPDF_Point((rcText.left + rcText.right) / 2.0f, rcText.top);
			break;
		case 2:
			ptOrigin = 	CPDF_Point(rcText.right, rcText.top);
			break;
		}
		break;
	case FSCommonDefines::e_rotation90:
		switch (align)
		{
		case 0:
			ptOrigin = 	CPDF_Point(rcText.left, rcText.bottom);
			break;
		case 1:
			ptOrigin = 	CPDF_Point(rcText.left, (rcText.top + rcText.bottom) / 2.0f);
			break;
		case 2:
			ptOrigin = 	CPDF_Point(rcText.left, rcText.top);
			break;
		}
		break;
	case FSCommonDefines::e_rotation180:
		switch (align)
		{
		case 0:
			ptOrigin = 	CPDF_Point(rcText.right, rcText.bottom);
			break;
		case 1:
			ptOrigin = 	CPDF_Point((rcText.left + rcText.right) / 2.0f, rcText.bottom);
			break;
		case 2:
			ptOrigin = 	CPDF_Point(rcText.left, rcText.bottom);
			break;
		}
		break;
	case FSCommonDefines::e_rotation270:
		switch (align)
		{
		case 0:
			ptOrigin = 	CPDF_Point(rcText.right, rcText.top);
			break;
		case 1:
			ptOrigin = 	CPDF_Point(rcText.right, (rcText.top + rcText.bottom) / 2.0f);
			break;
		case 2:
			ptOrigin = 	CPDF_Point(rcText.right, rcText.bottom);
			break;
		}
		break;
	}

	return ptOrigin;
}

CPDF_Matrix CFT_Edit::GetCurMatrix() const
{
	if (!m_pFreeTextAnnot) return CPDF_Matrix(0, 0, 0, 0, 0, 0);
	CPDF_Matrix mt;
	FS_ROTATION nRotate = m_pFreeTextAnnot->getPage()->getRotation();
	switch (nRotate)
	{
	default:
	case FSCommonDefines::e_rotation0:		
		mt = CPDF_Matrix(1, 0, 0, 1, 0, 0);
		break;
	case FSCommonDefines::e_rotation90:
		mt = CPDF_Matrix(0, 1, -1, 0, 0, 0);
		break;
	case FSCommonDefines::e_rotation180:
		mt = CPDF_Matrix(-1, 0, 0, -1, 0, 0);
		break;
	case FSCommonDefines::e_rotation270:
		mt = CPDF_Matrix(0, -1, 1, 0, 0, 0);
		break;
	}

	CPDF_Point ptOrigin = this->GetEditOrigin();

	mt.e += ptOrigin.x;
	mt.f += ptOrigin.y;

	return mt;
}

CPDF_Rect CFT_Edit::GetTextBBox() const
{
	if (!m_pEdit) return CPDF_Rect(0, 0, 0, 0);
	CPDF_Rect cont_rec;
	m_pEdit->GetContentRect(cont_rec);

	CPDF_Matrix mt = this->GetCurMatrix();
	mt.TransformRect(cont_rec);

	return cont_rec;
}

CFX_ByteString CFT_Edit::GetAppearanceStream() const
{
	if (!m_pEdit) return CFX_ByteString();

	CPDF_Matrix mtText = GetTextMatrix();
	m_pEdit->SetTextMatrix(mtText);

	return m_pEdit->GetAppearanceStream();
	//return CPWL_Utils::GetEditAppStream(m_pEdit->GetEdit(), CPDF_Point(0.0f,0.0f), NULL, TRUE);
}

CPDF_Matrix CFT_Edit::GetTextMatrix() const
{
	if (!m_pFreeTextAnnot) return CPDF_Matrix(1, 0, 0, 1, 0, 0);
	CFX_ByteString sDA = m_pFreeTextAnnot->getAnnotImp().getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);

	CFSAnnot_DefaultAppearance da(sDA);
	return da.GetTextMatrix();
}

///////////////////////////////////////////////////////////////////////////
// Class FSPDFFreeTextImp
FSPDFFreeTextImp::FSPDFFreeTextImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFFreeTextImp::move(FSRectF rect)
{
	if (!getPage() || !CHECKVALID_FLOATRECT_PDF(rect))
		return FALSE;
	CPDF_Matrix matrix;
	if (!getTransformMatrix(rect, matrix))
		return FALSE;
	CPDF_Rect rcBBoxOld = getRectImp(FSANNOT_RECTKEY_RECT);
	CPDF_Rect rcBBoxNew(rect.left, rect.bottom, rect.right, rect.top);
	//Zoom is not allowed for the FreeText Annotation.
	CPDF_Page* pPage = ((FSPDFPageImp*)getPage())->getPage();
	CPDF_Rect rcPageBBox = pPage->GetPageBBox();
	if(!rcPageBBox.Contains(rcBBoxNew))
	{
		if(rcBBoxNew.left < rcPageBBox.left)
		{
			rcBBoxNew.right += rcPageBBox.left - rcBBoxNew.left;
			rcBBoxNew.left = 0;
		}
		if(rcBBoxNew.right > rcPageBBox.right)
		{
			rcBBoxNew.left -= rcBBoxNew.right - rcPageBBox.right;
			rcBBoxNew.right = rcPageBBox.right;
		}
		if(rcBBoxNew.top > rcPageBBox.top)
		{
			rcBBoxNew.bottom -= rcBBoxNew.top -rcPageBBox.top;
			rcBBoxNew.top = rcPageBBox.top;
		}
		if(rcBBoxNew.bottom < rcPageBBox.bottom)
		{
			rcBBoxNew.top += rcPageBBox.bottom - rcBBoxNew.bottom;
			rcBBoxNew.bottom = rcPageBBox.bottom;
		}
		matrix = CPDF_Matrix(matrix.a, 0, 0, matrix.d, rcBBoxNew.left - rcBBoxOld.left * matrix.a, rcBBoxNew.bottom - rcBBoxOld.bottom * matrix.d);
	}
	adjustPosition(matrix, rcBBoxOld, rcBBoxNew);

	// In GSDK, when moving a freetext, it will not resetap after moving.
	// Reason :GSDK r121668
	// "fix the issue that moving freetext annot slowly in embeded platform, because freetext will search font on the map when reseting appearance. See jira sdk-1904."
	return FSPDFAnnotImp::moveImp((FSFreeText*)this, rect, FALSE);
}

FS_BOOL FSPDFFreeTextImp::resetAppearanceStream()
{
	if (!getPDFDict() || !getPage()) 
		return FALSE;
	FSPDFAnnotImp annotImp = getAnnotImp();
	
	FS_BOOL bRet = FALSE;
	CFX_ByteString bsIT = annotImp.getStringImpl(FSANNOT_NAMEKEY_INTENT);//((CPDF_Dictionary*)getDict())->GetString(FSANNOT_NAMEKEY_INTENT, "");
	if (bsIT == FX_BSTRC("FreeTextTypewriter"))
		bRet = resetAppearance_TW(TRUE);
	else if (bsIT == FX_BSTRC("FreeTextCallout"))
	{
		readyAppearance_CO(TRUE);
		bRet = resetAppearance_CO(TRUE);
	}
	else //If "/IT" is empty or not "FreeTextTypewriter" or "FreeTextCallout", it will be treated as Textbox.
	{
		bRet = resetAppearance_TB(TRUE);
	}
	//if (bRet) 
	//	annotImp.setFreeTextBorderColorFlag(FALSE);
	return bRet;
}

FS_ARGB FSPDFFreeTextImp::getFillColor() const
{
	return getColor(TRUE);
}

void FSPDFFreeTextImp::setFillColor(FS_ARGB fillColor)
{
	setColor(fillColor, TRUE);
}

FS_INT32 FSPDFFreeTextImp::getAlignment() const
{
	if (FALSE == hasProperty(FSANNOT_INTEGERKEY_ALIGNMENT))
		return 0;
	return getInteger(FSANNOT_INTEGERKEY_ALIGNMENT);
}

void FSPDFFreeTextImp::setAlignment(FS_INT32 alignment)
{
	FS_INT32 newAlignment = alignment;
	if (newAlignment < 0 || newAlignment > 2) 
		newAlignment = 0;
	setInteger(FSANNOT_INTEGERKEY_ALIGNMENT, newAlignment);
}
	
FSRectF FSPDFFreeTextImp::getInnerRect() const
{
	CFX_FloatRect fxInnerRect =  ((FSPDFMarkupImp*)this)->getInnerRectImp();
	FSRectF rectF;
	rectF.set(fxInnerRect.left, fxInnerRect.bottom, fxInnerRect.right, fxInnerRect.top);
	return rectF;
}

void FSPDFFreeTextImp::setInnerRect(FSRectF innerRect)
{
	setRectMargin(innerRect);
}

FSDefaultAppearance FSPDFFreeTextImp::getDefaultAppearance()
{// This function is to get information from /DA -- textcolor and fontsize will try to get from DS(maybe with RC)
	FSDefaultAppearance retDa;
	retDa.set(0, NULL, 0, 0);
	FSPDFAnnotImp annotImp = getAnnotImp();
	if (!annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTAP))
		return retDa;

	// Get font and fontsize
	CFX_ByteString csFontNameTag;
	FS_FLOAT fFontSize = 0;
	if (getFontInfo(csFontNameTag, fFontSize))
	{
		CPDF_Font* pPDFFont = getPDFFont(csFontNameTag);
		if (pPDFFont)
		{
			FSPDFDocImp* pDoc = (FSPDFDocImp*)getPage()->getDocument();
			if (!pDoc) return retDa;
			retDa.font = (FSFont*)pDoc->getFSFont(pPDFFont);
			if (retDa.font)
				retDa.flags |= FSCommonDefines::e_defaultAPFont;
		}
		retDa.fontSize = fFontSize;
		retDa.flags |= FSCommonDefines::e_defaultAPFontSize;
	}

	// Get text color
	if (getTextColor(retDa.textColor));
		retDa.flags |= FSCommonDefines::e_defaultAPTextColor;

	//// Get textmatrix from /DA
	//if (annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTAP))
	//{
	//	CFX_ByteString csDA = annotImp.getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);
	//	CFSAnnot_DefaultAppearance da(csDA); 
	//	if(da.HasTextMatrix())
	//	{
	//		CFX_Matrix matrix = da.GetTextMatrix();
	//		retDa.textMatrix.set(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);
	//		retDa.flags |= FSCommonDefines::e_defaultAPTextMatrix;
	//	}
	//}
	return retDa;
}

FS_BOOL FSPDFFreeTextImp::setDefaultAppearance(FSDefaultAppearance defaultAP)
{// Update data according to defaultAP.flags.
	if (0 == defaultAP.flags)
		return TRUE;

	if (defaultAP.flags & FSCommonDefines::e_defaultAPFont && defaultAP.font == NULL)
		return FALSE;

	if (defaultAP.flags & FSCommonDefines::e_defaultAPFontSize && (FLOAT_ISBIGGER(0, defaultAP.fontSize) || FLOAT_ISZERO(defaultAP.fontSize)))
		return FALSE;

	// Backup old data.
	CFX_ByteString oldFontName;
	FS_FLOAT fOldFontSize = 0;
	getFontInfo(oldFontName, fOldFontSize);
	FS_ARGB oldTextColor = 0;
	getTextColor(oldTextColor);

	// Get old /DA and then update it.
	CFX_ByteString csOldDA = getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);
	CFSAnnot_DefaultAppearance oldDa(csOldDA);

	// Prepare font tag name to be set to dictionary.
	CFX_ByteString csNewFontNameTag = "";
	if (defaultAP.flags & FSCommonDefines::e_defaultAPFont)
	{// Has new font, so use the new font name as the new tag font name.
		CFX_WideString psName = ((FSFontImp*)defaultAP.font)->getPsName();
		csNewFontNameTag = CFX_ByteString::FromUnicode(psName);
		if (!oldFontName.Equal(csNewFontNameTag))
		{// If new font tag name is different from old one, the new font needs to be added to dictionary.
			FSPDFDoc* pPDFDoc = ((FSPDFPageImp*)this->getPage())->getDocument();
			if (NULL == pPDFDoc) return FALSE;
			CPDF_Document* pDoc = ((FSPDFDocImp*)pPDFDoc)->getPDFDocument();
			if (NULL == pDoc) return FALSE;
			CPDF_Dictionary* pFontDict = ((FSFontImp*)defaultAP.font)->getPDFFontDict((FSPDFDocImp*)pPDFDoc);
			if (!pFontDict) return FALSE;		

			CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
			if (!pAnnotDict) return FALSE;

			CPDF_Dictionary* pAPDict = pAnnotDict->GetDict(FX_BSTR("AP"));
			if (!pAPDict) {
				pAPDict = FX_NEW CPDF_Dictionary;
				pAnnotDict->SetAt(FX_BSTR("AP"), pAPDict);
			}

			CPDF_Stream* pAPNStream = pAPDict->GetStream(FX_BSTR("N"));
			if (!pAPNStream) 
			{

				CPDF_Dictionary* pDict = FX_NEW CPDF_Dictionary;
				if (NULL == pDict) return FALSE;
				//comment out to fixed SDK-5066 issue
				//pDoc->AddIndirectObject(pDict);

				pAPNStream = FX_NEW CPDF_Stream(NULL, 0, pDict);
				if (NULL == pAPNStream) {
					pDict->Release();
					return FALSE;
				}

				FX_INT32 objnum = pDoc->AddIndirectObject(pAPNStream);
				pAPDict->AddReference(FX_BSTR("N"), pDoc, objnum);
			}

			CPDF_Dictionary* pResourceDict = pAPNStream->GetDict()->GetDict(FX_BSTR("Resources"));
			if (!pResourceDict) {
				pResourceDict = FX_NEW CPDF_Dictionary;
				if (!pResourceDict) return FALSE; 
				pAPNStream->GetDict()->SetAt(FX_BSTR("Resources"), pResourceDict);
			}

			CPDF_Dictionary* pFontResDict = pResourceDict->GetDict(FX_BSTR("Font"));
			if (!pFontResDict) {
				pFontResDict = FX_NEW CPDF_Dictionary;
				pResourceDict->SetAt(FX_BSTR("Font"), pFontResDict);
			}

			FX_DWORD dwDictNum = pFontDict->GetObjNum();
			if (dwDictNum != 0)
				pFontResDict->AddReference(csNewFontNameTag, pDoc, dwDictNum);
			else
				pFontResDict->AddValue(csNewFontNameTag, pFontDict);
		}
	}
	else 
		csNewFontNameTag = oldFontName;
	// Prepare font size to be set to dictionary.
	FS_FLOAT fNewFontSize = 0;
	if (defaultAP.flags & FSCommonDefines::e_defaultAPFontSize)
		fNewFontSize = defaultAP.fontSize;
	else
		fNewFontSize = fOldFontSize;
	// Set font and fontsize to /DA, if font name is not empty. 
	if (!csNewFontNameTag.IsEmpty())
		oldDa.SetFont(csNewFontNameTag, fNewFontSize);

	// Prepare text color to be set to dictionary
	FS_ARGB newTextColor = 0;
	if (defaultAP.flags & FSCommonDefines::e_defaultAPTextColor)
		newTextColor = defaultAP.textColor;
	else
		newTextColor = oldTextColor;
	// Update /DA
	setString(FSANNOT_STRINGKEY_DEFAULTAP,(CFX_ByteString)oldDa);

	// Remove /RC and /DS -- remove old /DS and then will write a new /DS with textcolor and fontsize.
	m_pAnnotDict->RemoveAt(FSANNOT_STRINGKEY_RICHTEXT);
	m_pAnnotDict->RemoveAt(FSANNOT_STRINGKEY_DEFAULTSTYLE);

	// Store text color to new DS, and also store font size to new DS.
	CFX_ByteString bsNewDSFont;
	bsNewDSFont.Format("font-size:%.1fpt", fNewFontSize);
	CFX_ByteString bsNewDSColor;
	bsNewDSColor.Format("color:#%x", newTextColor);
	CFX_ByteString bsNewDS;
	bsNewDS = bsNewDSFont + ";" + bsNewDSColor;
	// Update /DS
	m_pAnnotDict->SetAtString(FSANNOT_STRINGKEY_DEFAULTSTYLE, bsNewDS);

	// For text matrix -- no need for current.
	//if(defaultAP.flags & FSCommonDefines::e_defaultAPTextMatrix)
	//{
	//	CFX_Matrix matrix;
	//	matrix.a = defaultAP.textMatrix.a;
	//	matrix.b = defaultAP.textMatrix.b;
	//	matrix.c = defaultAP.textMatrix.c;
	//	matrix.d = defaultAP.textMatrix.d;
	//	matrix.e = defaultAP.textMatrix.e;
	//	matrix.f = defaultAP.textMatrix.f;
	//	newDa.SetTextMatrix(matrix);
	//}

	setModified();

	return TRUE;
}

FSString FSPDFFreeTextImp::getCalloutLineEndingStyle() const
{ // Only for start point.
	CFX_ByteString bsIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
	if (0 != bsIntent.Compare("FreeTextCallout"))
		return FSString();
	CFX_ByteString startStyle, endStyle;
	((FSPDFMarkupImp*)this)->getLineEndingStyles(startStyle, endStyle);
	FSString retStyle(startStyle.GetBuffer(startStyle.GetLength()), startStyle.GetLength());
	startStyle.ReleaseBuffer();
	return retStyle;
}

void FSPDFFreeTextImp::setCalloutLineEndingStyle(const char* endingStyle)
{ // Only for start point.
	CFX_ByteString bsIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
	if (0 != bsIntent.Compare("FreeTextCallout"))
		return ;
	setLineEndingStyles(endingStyle, NULL);
}

FS_INT32 FSPDFFreeTextImp::getCalloutLinePointCount() const
{
	CFX_ByteString bsIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
	if (0 != bsIntent.Compare("FreeTextCallout"))
		return 0;
	CPDF_Array* pCalloutArray = getArray(FSANNOT_ARRAYKEY_CALLOUTLINE);
	if (!pCalloutArray)
		return 0;
	FS_INT32 iArrayCount = pCalloutArray->GetCount();
	if (4 == iArrayCount)
		return 2;
	else if (6 == iArrayCount)
		return 3;
	return 0;
}

FSPointF FSPDFFreeTextImp::getCalloutLinePoint(FS_INT32 index) const
{
	FSPointF pt;
	pt.set(0, 0);
	CFX_ByteString bsIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
	if (index<0 || 0 != bsIntent.Compare("FreeTextCallout")) 
		return pt;
	CPDF_Array* pCalloutArray = getArray(FSANNOT_ARRAYKEY_CALLOUTLINE);
	if (!pCalloutArray)
		return pt;
	FS_INT32 iArrayCount = pCalloutArray->GetCount();
	FS_INT32 iPointCount = 0;
	if (4 == iArrayCount)
		iPointCount = 2;
	else if (6 == iArrayCount)
		iPointCount = 3;
	if (index >= iPointCount)
		return pt;
	pt.x = pCalloutArray->GetFloat(2*index);
	pt.y = pCalloutArray->GetFloat(2*index+1);
	return pt;
}

void FSPDFFreeTextImp::setCalloutLinePoints(FSPointF point1, FSPointF point2, FSPointF point3)
{
	CFX_ByteString bsIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
	if (0 != bsIntent.Compare("FreeTextCallout"))
		return ;
	CPDF_Array* pCalloutArray = getArray(FSANNOT_ARRAYKEY_CALLOUTLINE, TRUE);
	if (!pCalloutArray)
		return;
	while (pCalloutArray->GetCount()>0)
		pCalloutArray->RemoveAt(0);
	FS_INT32 iTemp = pCalloutArray->GetCount();
	pCalloutArray->AddNumber(point1.x);
	pCalloutArray->AddNumber(point1.y);
	pCalloutArray->AddNumber(point2.x);
	pCalloutArray->AddNumber(point2.y);
	if (FALSE == FSCRT_Util_IsFloatPointEqual(point2, point3))
	{
		pCalloutArray->AddNumber(point3.x);
		pCalloutArray->AddNumber(point3.y);
	}
	setModified();
}

CPDF_Font*	FSPDFFreeTextImp::getPDFFont(CFX_ByteString sFontName)
{
	FSPDFDocImp* pPDFDoc = (FSPDFDocImp*)((FSPDFPageImp*)getPage())->getDocument();
	if (!pPDFDoc) return NULL;
	CPDF_Document* pDoc = pPDFDoc->getPDFDocument();
	if (!pDoc) return NULL;

	CPDF_Dictionary* pAPDict = getSubDict(FSANNOT_DICTKEY_AP);
	if (NULL == pAPDict)	return NULL;
	CPDF_Dictionary* pNDict = pAPDict->GetDict(FX_BSTRC("N"));
	if (NULL == pNDict)		return NULL; 
	CPDF_Dictionary* pResources = pNDict->GetDict(FX_BSTRC("Resources"));
	if (NULL == pResources)	return NULL;
	CPDF_Dictionary* pFontDict = pResources->GetDict(FX_BSTRC("Font"));
	if (NULL == pFontDict)	return NULL;
	CPDF_Dictionary* pSpecificFontDict = pFontDict->GetDict(sFontName);
	if (!pSpecificFontDict)
	{
		// Try to find if there is a font dict which /BaseFont equals to sFontName.
		FX_POSITION pos = pFontDict->GetStartPos();
		while(pos)
		{
			CFX_ByteString bsKey;
			CPDF_Object* pActualFont = pFontDict->GetNextElement(pos, bsKey);
			if (pActualFont && pActualFont->GetType() == PDFOBJ_REFERENCE && pActualFont->GetDirect())
			{
				CPDF_Dictionary* pActualFontDict = (CPDF_Dictionary*)pActualFont->GetDirect();
				CFX_ByteString bsBaseFont = pActualFontDict->GetString("BaseFont");
				if (bsBaseFont.EqualNoCase(sFontName))
				{
					pSpecificFontDict = pActualFontDict;
					break;
				}
			}
		}
		if (!pSpecificFontDict)
			return NULL;
	}
	return pDoc->LoadFont(pSpecificFontDict);
}

FS_BOOL FSPDFFreeTextImp::resetAppearance_TW(FX_BOOL bTextOverflow)
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	CFT_Edit Edit(this, bTextOverflow);	
	if (bTextOverflow)
	{
		CFX_WideString ws = getUnicodeString(FSANNOT_STRINGKEY_CONTENT);
		if(!ws.IsEmpty())
		{
			// The text rect of CFT_Edit will be calculated according to /Rect and /RD.
			CPDF_Rect rcText = Edit.GetTextBBox();
			transformRectWithTextMatrix(rcText);
			CPDF_Rect rcAnnot = rcText;

			annotImp.setRectImp(FSANNOT_RECTKEY_RECT, rcAnnot);
			CPDF_Rect rcRD(FXSYS_fabs(rcText.left - rcAnnot.left) , FXSYS_fabs(rcText.bottom - rcAnnot.bottom), FXSYS_fabs(rcAnnot.right - rcText.right), FXSYS_fabs(rcAnnot.top - rcText.top));
			annotImp.setRectImp(FSANNOT_RECTKEY_RD, rcRD);
		}
	}

	CPDF_Matrix mt = Edit.GetCurMatrix();

	CPDF_Point ptOrigin = Edit.GetEditOrigin();

	CPDF_Matrix mtAnnot = CPDF_Matrix(1, 0, 0, 1, -ptOrigin.x, -ptOrigin.y);

	CPDF_Matrix mtReverse;
	mtReverse.SetReverse(mt);

	CPDF_Rect rcAnnot = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);
	mtReverse.TransformRect(rcAnnot);

	mt.Concat(mtAnnot);

	CFX_ByteString sEdit = Edit.GetAppearanceStream();

	FX_ARGB crText = 0;
	getTextColor(crText);

	CFX_ByteTextBuf sApp;
	FX_FLOAT fOpacity = getOpacity();
	if (fOpacity < 0.999999 && fOpacity > -0.000001)
	{
		sApp << "/" << FT_OPACITY_ALIAS/*FX_BSTR("FreeText")*/ << " gs\n";
	}

	sApp << "q\n" << "/Tx BMC\n";
	sApp << rcAnnot.left << " " << rcAnnot.bottom << " " 
		<< rcAnnot.Width() << " " << rcAnnot.Height() << " re\nW\nn\n";

	sApp << "BT\n";
	if (!Edit.m_bRichText)
	{
		FX_INT32 r = (FX_INT32)(((crText >> 16) & 0xFF)) ;
		FX_INT32 g = (FX_INT32)(((crText >> 8) & 0xFF)) ;
		FX_INT32 b = (FX_INT32)((crText & 0xFF));
		CPWL_Color pwlColor(r,g,b);

		sApp << CPWL_Utils::GetColorAppStream(pwlColor);
	}

	sApp << sEdit  << "ET\n" << "EMC\nQ\n";

	CPDF_Matrix matrix = annotImp.getRotateMatrix();
	CPDF_Stream* pStream = annotImp.writeAppearance(FX_BSTRC("N"), rcAnnot, matrix, sApp, FX_BSTRC(""), NULL);

	if (fOpacity < 0.999999 && fOpacity > -0.000001)
		setStreamExtGState(pStream, FT_OPACITY_ALIAS, fOpacity);

	return TRUE;
}

void FSPDFFreeTextImp::setStreamExtGState(CPDF_Stream* pStream, const CFX_ByteString& sExtAlias, FX_FLOAT fOpacity)
{
	if (pStream == NULL) return;

	CPDF_Dictionary* pStreamDict = pStream->GetDict();
	if (pStreamDict == NULL) return;

	CPDF_Dictionary* pResDict = pStreamDict->GetDict("Resources");
	if (!pResDict)
	{
		pResDict = FX_NEW CPDF_Dictionary;
		pStreamDict->SetAt("Resources", pResDict);
	}

	CPDF_Dictionary* pExtGState = pResDict->GetDict("ExtGState"); 
	if (!pExtGState) 
	{
		pExtGState = FX_NEW CPDF_Dictionary ; 
		pResDict->SetAt("ExtGState", pExtGState); 
	} 

	CPDF_Dictionary* pOpaDict = pExtGState->GetDict(sExtAlias); 
	if (!pOpaDict) 
	{
		pOpaDict = pExtGState->GetDict("XJC"); 
		if (pOpaDict) 
			pExtGState->ReplaceKey("XJC", sExtAlias); 
		else
		{
			pOpaDict = FX_NEW CPDF_Dictionary; 
			pExtGState->SetAt(sExtAlias, pOpaDict); 
		}
	} 

	pOpaDict->SetAtName("Type", "ExtGState"); 
	pOpaDict->SetAtNumber( "CA", fOpacity);
	pOpaDict->SetAtNumber( "ca", fOpacity);
	pOpaDict->SetAt("AIS", FX_NEW CPDF_Boolean(FALSE));
}

void FSPDFFreeTextImp::transformRectWithTextMatrix(CPDF_Rect& rcText)
{
	//transform the size of rect according to the text matrix.
	FX_FLOAT width = rcText.right - rcText.left;
	FX_FLOAT height = rcText.top - rcText.bottom;

	CFX_ByteString sDA = getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);
	CFSAnnot_DefaultAppearance da(sDA);
	CFX_Matrix textMatrix = da.GetTextMatrix();
    // to calculate size, we just consider scale and rotation, translation is ignored
    textMatrix.e = 0;
    textMatrix.f = 0;
	textMatrix.TransformPoint(width, height);

	rcText.right = rcText.left + width;
	rcText.bottom = rcText.top - height;
}

void FSPDFFreeTextImp::readyAppearance_CO(FX_BOOL bTextOverflow)
{
	if (!getPage()) return ;
	FSPDFAnnotImp annotImp = getAnnotImp();
	if (!bTextOverflow)
	{
		CPDF_Rect rect = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);
		CPDF_Rect rectRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);
		correctCallOutTextArea(rect, rectRD);
	}

	FSBorderInfo borderInfo = getBorderInfo();
	FS_FLOAT nBorderWidth = borderInfo.width;
	FS_INT32 style = borderInfo.style;

	CPDF_Rect rcText = getTextRect();
	CPDF_Rect rcAnnot = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);

	CFT_Edit Edit(this, bTextOverflow);
	if (FSAnnot::e_borderStyleCloudy  == style && FLOAT_ISBIGGER(borderInfo.cloudIntensity, 0.0))
	{
		if (bTextOverflow)
		{
			// The text rect of CFT_Edit will be calculated according to /Rect and /RD.
			rcText = Edit.GetTextBBox();
			transformRectWithTextMatrix(rcText);
		}

		CFX_ByteTextBuf sCloudy;
		rcAnnot = FSAnnot_CreateCloudyAP_Rectangle(sCloudy, rcText, borderInfo.cloudIntensity);

		rcAnnot.left -= nBorderWidth / 2.0f;
		rcAnnot.right += nBorderWidth / 2.0f;
		rcAnnot.bottom -= nBorderWidth / 2.0f;
		rcAnnot.top += nBorderWidth / 2.0f;
	}
	else if (bTextOverflow)
	{
		//If there's no content, just don't re-calculate /Rect
		CFX_WideString ws = getUnicodeString(FSANNOT_STRINGKEY_CONTENT);
		if(!ws.IsEmpty())
		{
			// The text rect of CFT_Edit will be calculated according to /Rect and /RD.
			rcText = Edit.GetTextBBox();
			transformRectWithTextMatrix(rcText);
			rcAnnot = rcText;

			rcAnnot.left -= nBorderWidth;
			rcAnnot.right += nBorderWidth;
			rcAnnot.bottom -= nBorderWidth;
			rcAnnot.top += nBorderWidth;
		}
	}
	FSPointF startPoint = getCalloutLinePoint(0);
	FSPointF kneePoint, endPoint;
	if (hasKneePoint())
	{
		kneePoint = getCalloutLinePoint(1);
		endPoint = getCalloutLinePoint(2);
	}
	else
	{
		kneePoint = getCalloutLinePoint(1);
		endPoint = getCalloutLinePoint(1);
	}
	if (bTextOverflow)
	{
		updateCalloutPoints(rcText, startPoint, getKneeLength(), kneePoint, endPoint);
		setCalloutLinePoints(startPoint, kneePoint, endPoint);
	}

	CFX_ByteString sTemp;
	FSPointF vt;
	vt.set(startPoint.x - kneePoint.x, startPoint.y - kneePoint.y);
	FX_FLOAT fWidth = nBorderWidth*6.0f;
	FS_BOOL bNeedFill = FALSE;
	CPDF_Rect rcStarting = annotImp.getLineEndingStyleStream(TRUE, sTemp, startPoint, vt, fWidth, bNeedFill);

	rcStarting.left -= nBorderWidth;
	rcStarting.right += nBorderWidth;
	rcStarting.bottom -= nBorderWidth;
	rcStarting.top += nBorderWidth;

	CPDF_Rect rcKnee(kneePoint.x - nBorderWidth,
		kneePoint.y - nBorderWidth,
		kneePoint.x + nBorderWidth,
		kneePoint.y + nBorderWidth);

	rcAnnot.Union(rcStarting);
	rcAnnot.Union(rcKnee);

	CPDF_Rect rcRD(FXSYS_fabs(rcText.left - rcAnnot.left) , FXSYS_fabs(rcText.bottom - rcAnnot.bottom), 
					FXSYS_fabs(rcAnnot.right - rcText.right), FXSYS_fabs(rcAnnot.top - rcText.top));

	if (FSAnnot::e_borderStyleCloudy != style)
	{
		rcRD.left -= nBorderWidth;
		rcRD.right -= nBorderWidth;
		rcRD.bottom -= nBorderWidth;
		rcRD.top -= nBorderWidth;
	}

	CPDF_Page* pPDFPage = ((FSPDFPageImp*)getPage())->getPage();
	CPDF_Rect rcPage = pPDFPage->GetPageBBox();
	CPDF_Rect rcAnnotOld = rcAnnot;
	rcAnnot = FSAnnot_Util_ClipPageRect(rcPage, rcAnnot, FALSE);

	CPDF_Matrix mt(1, 0, 0, 1, rcAnnot.left - rcAnnotOld.left, rcAnnot.bottom - rcAnnotOld.bottom);
	this->adjustPosition(mt, rcAnnotOld, rcAnnot);

	if (bTextOverflow)
	{
		if (!correctCallOutTextArea(rcAnnot, rcRD))
		{
			annotImp.setRectImp(FSANNOT_RECTKEY_RECT, rcAnnot);
			annotImp.setRectImp(FSANNOT_RECTKEY_RD, rcRD);
		}
	}
	else
	{
		annotImp.setRectImp(FSANNOT_RECTKEY_RECT, rcAnnot);
		annotImp.setRectImp(FSANNOT_RECTKEY_RD, rcRD);
	}
}

FS_BOOL FSPDFFreeTextImp::hasKneePoint()
{
	return (3 == getCalloutLinePointCount());
}

void FSPDFFreeTextImp::updateCalloutPoints(CPDF_Rect rectBody, FSPointF startPoint, FX_FLOAT fKneeLength, 
											FSPointF& kneePoint, FSPointF& endPoint)
{
	if (startPoint.x < rectBody.left - fKneeLength) 
	{
		kneePoint.x = rectBody.left - fKneeLength;
		kneePoint.y = (rectBody.top + rectBody.bottom) / 2.0f;
		endPoint.x = rectBody.left;
		endPoint.y = kneePoint.y;
	}
	else if (startPoint.x > rectBody.right + fKneeLength) 
	{
		kneePoint.x = rectBody.right + fKneeLength;
		kneePoint.y = (rectBody.top + rectBody.bottom) / 2.0f;
		endPoint.x = rectBody.right;
		endPoint.y = kneePoint.y;
	}
	else if (startPoint.y < rectBody.bottom - fKneeLength) 
	{
		kneePoint.x = (rectBody.left + rectBody.right) / 2.0f;
		kneePoint.y = rectBody.bottom - fKneeLength;
		endPoint.x = kneePoint.x;
		endPoint.y = rectBody.bottom;
	}
	else if (startPoint.y > rectBody.top + fKneeLength) 
	{
		kneePoint.x = (rectBody.left + rectBody.right) / 2.0f;
		kneePoint.y = rectBody.top + fKneeLength;
		endPoint.x = kneePoint.x;
		endPoint.y = rectBody.top;
	}
	else if (startPoint.x < (rectBody.left + rectBody.right) / 2.0f) 
	{
		kneePoint.x = rectBody.left - fKneeLength;
		kneePoint.y = (rectBody.top + rectBody.bottom) / 2.0f;
		endPoint.x = rectBody.left;
		endPoint.y = kneePoint.y;
	}
	else
	{
		kneePoint.x = rectBody.right + fKneeLength;
		kneePoint.y = (rectBody.top + rectBody.bottom) / 2.0f;
		endPoint.x = rectBody.right;
		endPoint.y = kneePoint.y;
	}
}

FS_FLOAT FSPDFFreeTextImp::getKneeLength() const
{
	FS_FLOAT fBorderWidth = getBorderWidth();
	return FT_KNEE_DEFLENGTH + fBorderWidth * 2.0f;
}

void FSPDFFreeTextImp::adjustPosition(const CPDF_Matrix& mt, const CPDF_Rect& rcBBoxOld, const CPDF_Rect& rcBBoxNew)
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	CFX_ByteString sIT = annotImp.getStringImpl(FSANNOT_NAMEKEY_INTENT);

	if (0 == sIT.Compare("FreeTextTypewriter"))
	{
		// GSDK not support yet.
	}
	else if (0 == sIT.Compare("FreeTextCallout"))
	{
		FSPointF startPoint = getCalloutLinePoint(0);
		FSPointF kneePoint, endPoint;
		if (hasKneePoint())
		{
			kneePoint = getCalloutLinePoint(1);
			endPoint = getCalloutLinePoint(2);
		}
		else
		{
			kneePoint = getCalloutLinePoint(1);
			endPoint = getCalloutLinePoint(1);
		}

		mt.Transform(startPoint.x, startPoint.y);
		mt.Transform(endPoint.x, endPoint.y);
		mt.Transform(kneePoint.x, kneePoint.y);

		if (mt.GetA() != 1.0f || mt.GetB() != 0 ||
			mt.GetC() != 0 || mt.GetD() != 1.0f)
		{
			CPDF_Rect rcRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);
			CPDF_Rect rcText = rcBBoxOld;

			rcText.left += rcRD.left;
			rcText.bottom += rcRD.bottom;
			rcText.right -= rcRD.right;
			rcText.top -= rcRD.top;

			mt.TransformRect(rcText);

			rcRD.left = rcText.left - rcBBoxNew.left;
			rcRD.bottom = rcText.bottom - rcBBoxNew.bottom;
			rcRD.right = rcBBoxNew.right - rcText.right;
			rcRD.top = rcBBoxNew.top - rcText.top;

			annotImp.setRectImp(FSANNOT_RECTKEY_RD, rcRD);
		}

		if (hasKneePoint())
			setCalloutLinePoints(startPoint, kneePoint, endPoint);
		else
			setCalloutLinePoints(startPoint, endPoint, endPoint);
	}
	else
	{
		// GSDK not support yet.
	}
}

FS_BOOL FSPDFFreeTextImp::correctCallOutTextArea(const CPDF_Rect& rcBBox, const CPDF_Rect& rcRD)
{
	if (!getPage()) return FALSE;
	CPDF_Page* pPDFPage = ((FSPDFPageImp*)getPage())->getPage();
	CPDF_Rect rcPage = pPDFPage->GetPageBBox();
	CPDF_Rect rcPageInside = rcPage;
	FX_FLOAT fKneeLength = getKneeLength();

	CPDF_Rect rcText(rcBBox.left + rcRD.left,
					rcBBox.bottom + rcRD.bottom,
					rcBBox.right - rcRD.right,
					rcBBox.top - rcRD.top);

	FSPointF startPoint = getCalloutLinePoint(0);
	FSPointF kneePoint, endPoint;
	if (hasKneePoint())
	{
		kneePoint = getCalloutLinePoint(1);
		endPoint = getCalloutLinePoint(2);
	}
	else
	{
		kneePoint = getCalloutLinePoint(1);
		endPoint = getCalloutLinePoint(1);
	}

	FX_BOOL bStartingOutSide = (FLOAT_ISSMALLER(startPoint.x, rcPageInside.left) 
								|| FLOAT_ISBIGGER(startPoint.x, rcPageInside.right) 
								|| FLOAT_ISSMALLER(startPoint.y, rcPageInside.bottom) 
								||FLOAT_ISBIGGER(startPoint.y, rcPageInside.top)
								);

	FX_BOOL bKneeOutSide = (FLOAT_ISSMALLER(kneePoint.x, rcPageInside.left) 
							|| FLOAT_ISBIGGER(kneePoint.x, rcPageInside.right) 
							|| FLOAT_ISSMALLER(kneePoint.y, rcPageInside.bottom) 
							|| FLOAT_ISBIGGER(kneePoint.y, rcPageInside.top));

	FX_BOOL bTextOutSide = (FLOAT_ISSMALLER(rcText.left, rcPageInside.left) 
							|| FLOAT_ISBIGGER(rcText.right, rcPageInside.right) 
							|| FLOAT_ISSMALLER(rcText.bottom, rcPageInside.bottom) 
							|| FLOAT_ISBIGGER(rcText.top, rcPageInside.top));

	FX_BOOL bTextOnStarting = (rcText.left < startPoint.x && rcText.right > startPoint.x 
								&& rcText.bottom < startPoint.y && rcText.top > startPoint.y);

	if (bStartingOutSide || bKneeOutSide || bTextOutSide || bTextOnStarting)
	{
		//startPoint = ClipPagePoint(rcPageInside, startPoint);

		CPDF_Rect rcMax(startPoint.x - fKneeLength - rcText.Width(),
						startPoint.y - fKneeLength - rcText.Height(),
						startPoint.x + fKneeLength + rcText.Width(),
						startPoint.y + fKneeLength + rcText.Height());

		FX_FLOAT fTextWidth = rcText.Width();
		FX_FLOAT fTextHeight = rcText.Height();

		if (rcMax.left > rcPageInside.left && rcMax.right < rcPageInside.right &&
			rcMax.bottom > rcPageInside.bottom && rcMax.top < rcPageInside.top)
		{

			rcText.left = rcMax.left;
			rcText.right = rcMax.left + fTextWidth;

			rcText.top = rcMax.top;
			rcText.bottom = rcMax.top - fTextHeight;
		}
		else 
		{
			if (rcMax.left > rcPageInside.left && rcMax.right < rcPageInside.right)
			{
				rcText.left = rcMax.left;
				rcText.right = rcMax.left + fTextWidth;
			}
			else if (rcMax.left > rcPageInside.left || rcMax.right < rcPageInside.right)
			{
				if (rcMax.left > rcPageInside.left)
				{
					rcText.left = rcMax.left;
					rcText.right = rcMax.left + fTextWidth;
				}
				else
				{
					rcText.right = rcMax.right;
					rcText.left = rcText.right - fTextWidth;
				}
			}
			else if (rcMax.bottom > rcPageInside.bottom && rcMax.top < rcPageInside.top)
			{
				rcText.top = rcMax.top;
				rcText.bottom = rcMax.top - fTextHeight;
			}
			else if (rcMax.bottom > rcPageInside.bottom || rcMax.top < rcPageInside.top)
			{
				if (rcMax.bottom > rcPageInside.bottom)
				{
					rcText.top = rcMax.top;
					rcText.bottom = rcMax.top - fTextHeight;
				}
				else
				{
					rcText.bottom = rcMax.bottom;
					rcText.top = rcMax.bottom + fTextHeight;
				}
			}
			else
			{
			}
		}

		updateCalloutPoints(rcText, startPoint, fKneeLength, kneePoint, endPoint);
		setCalloutLinePoints(startPoint, kneePoint, endPoint);

		CPDF_Rect rcBBoxNew = getBBoxByTextRect(rcText, FALSE);

		CPDF_Rect rcRDNew(rcText.left - rcBBoxNew.left,
						rcText.bottom - rcBBoxNew.bottom,
						rcBBoxNew.right - rcText.right,
						rcBBoxNew.top - rcText.top);

		CPDF_Rect rcBBoxOld = rcBBoxNew;
		rcBBoxNew = FSAnnot_Util_ClipPageRect(rcPage, rcBBoxNew, FALSE);

		CPDF_Matrix mt(1, 0, 0, 1, rcBBoxNew.left - rcBBoxOld.left, rcBBoxNew.bottom - rcBBoxOld.bottom);
		adjustPosition(mt, rcBBoxOld, rcBBoxNew);

		setRectImp(FSANNOT_RECTKEY_RECT, rcBBoxNew);
		setRectImp(FSANNOT_RECTKEY_RD, rcRDNew);
		return TRUE;
	}
	return FALSE;
}

CPDF_Rect FSPDFFreeTextImp::getBBoxByTextRect(const CPDF_Rect& rcText, FX_BOOL bCalculateKnee)
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	CPDF_Rect rcBBox = rcText;
	CFX_ByteString sIT = annotImp.getStringImpl(FSANNOT_NAMEKEY_INTENT);
	int type = -1;
	if(sIT == FX_BSTRC("FreeTextTypewriter"))
		type = 0;
	else if (sIT == FX_BSTRC("FreeTextCallout"))
		type = 1;
	else
		type = 2;

	if (0 == sIT.Compare("FreeTextTypewriter"))
	{
	}
	else if (0 == sIT.Compare("FreeTextCallout"))
	{
		FSBorderInfo borderInfo = getBorderInfo();
		FSM_BORDERSTYLE borderStyle = borderInfo.style;
		float nBorderWidth = borderInfo.width;
		if (FSAnnot::e_borderStyleCloudy == borderStyle && FLOAT_ISBIGGER(borderInfo.cloudIntensity, 0.0))
		{
			CFX_ByteTextBuf sCloudy;
			CPDF_Rect rcBBox = FSAnnot_CreateCloudyAP_Rectangle(sCloudy, rcText, borderInfo.cloudIntensity);

			rcBBox.left -= nBorderWidth / 2.0f;
			rcBBox.right += nBorderWidth / 2.0f;
			rcBBox.bottom -= nBorderWidth / 2.0f;
			rcBBox.top += nBorderWidth / 2.0f;
		}
		else
		{
			rcBBox.left -= nBorderWidth;
			rcBBox.right += nBorderWidth;
			rcBBox.bottom -= nBorderWidth;
			rcBBox.top += nBorderWidth;
		}

		FSPointF startPoint = getCalloutLinePoint(0);
		FSPointF kneePoint, endPoint;
		if (hasKneePoint())
		{
			kneePoint = getCalloutLinePoint(1);
			endPoint = getCalloutLinePoint(2);
		}
		else
		{
			kneePoint = getCalloutLinePoint(1);
			endPoint = getCalloutLinePoint(1);
		}
		if (bCalculateKnee)
			updateCalloutPoints(rcText, startPoint, getKneeLength(), kneePoint, endPoint);

		CPDF_Rect rcKnee(kneePoint.x - nBorderWidth,
						kneePoint.y - nBorderWidth,
						kneePoint.x + nBorderWidth,
						kneePoint.y + nBorderWidth);

		CFX_ByteString sLine;
		FSPointF vt;
		vt.set(startPoint.x - kneePoint.x, startPoint.y - kneePoint.y);
		FX_FLOAT fWidth = nBorderWidth*6.0f;
		FS_BOOL bNeedFill = FALSE;
		CPDF_Rect rcStarting = annotImp.getLineEndingStyleStream(TRUE, sLine, startPoint, vt, fWidth, bNeedFill);

		rcStarting.left -= nBorderWidth;
		rcStarting.right += nBorderWidth;
		rcStarting.bottom -= nBorderWidth;
		rcStarting.top += nBorderWidth;

		rcBBox.Union(rcStarting);
		rcBBox.Union(rcKnee);
	}
	else //2
	{
		FSBorderInfo borderInfo = getBorderInfo();
		FSM_BORDERSTYLE borderStyle = borderInfo.style;
		float nBorderWidth = borderInfo.width;

		if (FSAnnot::e_borderStyleCloudy == borderStyle && FLOAT_ISBIGGER(borderInfo.cloudIntensity, 0.0))
		{
			CFX_ByteTextBuf sCloudy;
			CPDF_Rect rcBBox = FSAnnot_CreateCloudyAP_Rectangle(sCloudy, rcText, borderInfo.cloudIntensity);

			rcBBox.left -= nBorderWidth / 2.0f;
			rcBBox.right += nBorderWidth / 2.0f;
			rcBBox.bottom -= nBorderWidth / 2.0f;
			rcBBox.top += nBorderWidth / 2.0f;
		}
		else
		{
			rcBBox.left -= nBorderWidth;
			rcBBox.right += nBorderWidth;
			rcBBox.bottom -= nBorderWidth;
			rcBBox.top += nBorderWidth;
		}
	}
	return rcBBox;
}

CPDF_Rect FSPDFFreeTextImp::getTextRect() const
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	CFX_ByteString sIT = annotImp.getStringImpl(FSANNOT_NAMEKEY_INTENT);
	int type = -1;
	if(sIT == FX_BSTRC(""))
		type = 0;
	else if (sIT == FX_BSTRC(""))
		type = 1;
	else
		type = 2;

	//Get "/Rect" from annotation's dictionary.
	CPDF_Rect rcAnnot = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);
	//Get "/RD" from annotation's dictionary.
	CPDF_Rect rcRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);

	CPDF_Rect rcText;
	rcText.left = rcAnnot.left + rcRD.left;
	rcText.right = rcAnnot.right - rcRD.right;
	rcText.bottom = rcAnnot.bottom + rcRD.bottom;
	rcText.top = rcAnnot.top - rcRD.top;

	if (0 == sIT.Compare("FreeTextTypewriter"))
	{
	}
	else if (0 == sIT.Compare("FreeTextCallout"))
	{
		FS_INT32 effect = annotImp.getBorderEffect();

		if (effect != FSANNOT_BORDEREFFECT_CLOUDY)
		{
			float nBorderWidth = annotImp.getBorderWidth();

			rcText.left += (nBorderWidth);
			rcText.right -= (nBorderWidth);
			rcText.bottom += (nBorderWidth);
			rcText.top -= (nBorderWidth);
		}
	}
	else 
	{
		FS_INT32 effect = annotImp.getBorderEffect();

		if (effect != FSANNOT_BORDEREFFECT_CLOUDY)
		{
			float nBorderWidth = annotImp.getBorderWidth();

			rcText.left += (nBorderWidth);
			rcText.right -= (nBorderWidth);
			rcText.bottom += (nBorderWidth);
			rcText.top -= (nBorderWidth);
		}
	}

	if (rcText.left > rcText.right)
	{
		FX_FLOAT fMidX = (rcText.left + rcText.right) / 2.0f;
		rcText.left = fMidX;
		rcText.right = fMidX;
	}
	//properties.m_pSystemHandler = FX_NEW FX_SystemHandlerImp(NULL);

	if (rcText.bottom > rcText.top)
	{
		FX_FLOAT fMidY = (rcText.top + rcText.bottom) / 2.0f;
		rcText.top = fMidY;
		rcText.bottom = fMidY;
	}

	return rcText;
}

FS_BOOL FSPDFFreeTextImp::getTextColor(FS_ARGB& textColor)
{
	// if no text color can be found , use black as default.
	textColor = 0; 
	FSPDFAnnotImp annotImp = getAnnotImp();
	// Try to get text color from /RC + /DS
	if (annotImp.hasProperty(FSANNOT_STRINGKEY_RICHTEXT) || annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTSTYLE))
	{
		// /RC may be a text string, or a text stream.
		CFX_WideString wsRC = annotImp.getUnicodeString(FSANNOT_STRINGKEY_RICHTEXT);
		CFX_WideString wsDS = annotImp.getUnicodeString(FSANNOT_STRINGKEY_DEFAULTSTYLE);

		CPDF_RichTextXML richtextXML;
		if (richtextXML.SetXML(wsRC, wsDS) && richtextXML.getColor(textColor))
			return TRUE;
	}
	// If not find in RC+DS, then try to get text color from /DA ( rg), as freetext border color
	if (annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTAP))
	{
		CFX_ByteString csDA = annotImp.getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);
		CFSAnnot_DefaultAppearance da(csDA); 
		if (da.HasColor(FALSE))
		{
			FS_INT32 iColorType = 0;// as output parameter
			da.GetColor(textColor, iColorType, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

FS_BOOL FSPDFFreeTextImp::resetAppearance_CO(FX_BOOL bTextOverflow)
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	// The text rect of CFT_Edit will be calculated according to /Rect and /RD.
	CFT_Edit Edit(this, bTextOverflow);
	CPDF_Matrix mt = Edit.GetCurMatrix();
	CPDF_Point ptOrigin = Edit.GetEditOrigin();

	CPDF_Matrix mtAnnot = CPDF_Matrix(1, 0, 0, 1, -ptOrigin.x, -ptOrigin.y);
	CPDF_Matrix mtReverse;
	mtReverse.SetReverse(mt);

	CPDF_Rect rcAnnot = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);
	mtReverse.TransformRect(rcAnnot);

	CPDF_Rect rcText = getTextRect();
	mtReverse.TransformRect(rcText);

	FSPointF startPoint = getCalloutLinePoint(0);
	FSPointF kneePoint, endPoint;
	if (hasKneePoint())
	{
		kneePoint = getCalloutLinePoint(1);
		endPoint = getCalloutLinePoint(2);
	}
	else
	{
		kneePoint = getCalloutLinePoint(1);
		endPoint = getCalloutLinePoint(1);
	}

	mtReverse.Transform(startPoint.x, startPoint.y);
	mtReverse.Transform(kneePoint.x, kneePoint.y);
	mtReverse.Transform(endPoint.x, endPoint.y);
	mt.Concat(mtAnnot);

	CFX_ByteString sEdit = Edit.GetAppearanceStream();

	FX_ARGB crText = 0;
	getTextColor(crText);
	FX_ARGB crFill;
	FX_BOOL bFill = annotImp.getFillColor(crFill);
	FX_ARGB crBorder = 0;
	FX_BOOL bBorder = annotImp.getBorderColorImp(crBorder/*, !m_FSMarkupImp.getAnnotImp().isSetFreeTextBorderColor()*/);
	FX_FLOAT nBorderWidth = annotImp.getBorderWidth();

	FSM_BORDERSTYLE iBorderStyle = annotImp.getBorderStyle();
	FS_INT32 iBorderEfferct = annotImp.getBorderEffect();
	FX_FLOAT fCloudyIntensity = annotImp.getCloudyIntensity();

	CFX_ByteTextBuf sApp;

	float opacity = getOpacity();

	if (opacity < 0.999999 && opacity > -0.000001)
	{
		sApp << "/" << FT_OPACITY_ALIAS << " gs\n";
	}

	sApp << "q\n";
	if (bFill)
		sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crFill), FXARGB_G(crFill), FXARGB_B(crFill)), TRUE);
	// If no border property, no need to generate ap stream for border.
	if (bBorder && nBorderWidth != 0)
	{	
		sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), FALSE);
		sApp << nBorderWidth << " w\n";
		if (FSANNOT_BORDEREFFECT_CLOUDY != iBorderEfferct)
		{
			if (FSAnnot::e_borderStyleDashed == iBorderStyle)
			{
				FX_FLOAT fDashOffset;
				CFX_FloatArray aDashPattern;
				annotImp.getBorderDash(fDashOffset, aDashPattern);
				FS_INT32 iArraySize = aDashPattern.GetSize();
				if (iArraySize == 1)
				{
					sApp << "[" << aDashPattern[0] << " " << aDashPattern[0] << "] 0 d\n";
				}
				else
				{
					sApp << "[";
					for (int i=0; i<iArraySize; i++)
					{
						sApp << aDashPattern[i];
						if (i != iArraySize - 1)
							sApp << " ";
					}
					sApp << "] 0 d\n";
				}
			}
		}
		CFX_ByteString sLine;
		FSPointF vt;
		vt.set(startPoint.x - kneePoint.x, startPoint.y - kneePoint.y);
		FX_FLOAT fWidth = nBorderWidth*6.0f;
		FS_BOOL bNeedFill = FALSE;
		annotImp.getLineEndingStyleStream(TRUE, sLine, startPoint, vt, fWidth, bNeedFill);
		if (sLine.IsEmpty() == FALSE)
		{
			if (bNeedFill == TRUE)
			{
				if (bFill)
					sLine += "h\nB\n";
				else
					sLine += "h\nS\n";
			}
			else
				sLine += "S\n";
		}
		sApp << sLine;

		sApp << startPoint.x << " " << startPoint.y << " m\n";
		sApp << kneePoint.x << " " << kneePoint.y << " l\n";
		sApp << endPoint.x << " " << endPoint.y << " l S\n";
		sApp << "Q\n";
	}
	if (FSANNOT_BORDEREFFECT_CLOUDY == iBorderEfferct && FLOAT_ISBIGGER(fCloudyIntensity, 0.0))
	{
		CPDF_Rect rcRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);

		CFX_ByteTextBuf sCloudy;
		CPDF_Rect rcBBox = FSAnnot_CreateCloudyAP_Rectangle(sCloudy, rcText, fCloudyIntensity);

		if (bFill)
		{
			sApp << "q\n";
			sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crFill), FXARGB_G(crFill), FXARGB_B(crFill)), TRUE);
			sApp << sCloudy << "f\nQ\n";
		}

		sApp << "q\n";
		sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), FALSE);
		sApp << nBorderWidth << " w\n";
		sApp << sCloudy << "S\nQ\n";

		sApp << "q\n/Tx BMC\n";
		sApp << rcText.left << " " << rcText.bottom << " " 
			<< rcText.Width() << " " << rcText.Height() << " re\nW\nn\n";

		if (!Edit.m_bRichText)
			sApp << "BT\n" << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crText), FXARGB_G(crText), FXARGB_B(crText))) 
			<< sEdit << "ET\n" << "EMC\nQ\n";
		else
			sApp << "BT\n" << sEdit << "ET\n" << "EMC\nQ\n";
	}
	else
	{
		if (bFill)
			sApp << CPWL_Utils::GetRectFillAppStream(rcText, CPWL_Color(FXARGB_R(crFill), FXARGB_G(crFill), FXARGB_B(crFill)));

		CPDF_Rect rcStroke = rcText;

		rcStroke.left -= nBorderWidth / 2.0f;
		rcStroke.right += nBorderWidth / 2.0f;
		rcStroke.bottom -= nBorderWidth / 2.0f;
		rcStroke.top += nBorderWidth / 2.0f;

		if (rcStroke.Width() > nBorderWidth * 1.5f &&
			rcStroke.Height() > nBorderWidth * 1.5f)
		{
			sApp << "q\n";
			// If no border property, no need to generate ap stream for border.
			if (bBorder && nBorderWidth != 0)
			{	
				sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), FALSE);
				sApp << nBorderWidth << " w\n";
				if (FSAnnot::e_borderStyleDashed == iBorderStyle)
				{
					FX_FLOAT fDashOffset;
					CFX_FloatArray aDashPattern;
					annotImp.getBorderDash(fDashOffset, aDashPattern);

					FS_INT32 iArraySize = aDashPattern.GetSize();

					if (iArraySize == 1)
					{
						sApp << "[" << aDashPattern[0] << " " << aDashPattern[0] << "] 0 d\n";
					}
					else
					{
						sApp << "[";
						for (int i=0; i<iArraySize; i++)
						{
							sApp << aDashPattern[i];

							if (i != iArraySize - 1)
								sApp << " ";
						}
						sApp << "] 0 d\n";
					}
				}
				sApp << rcStroke.left << " " << rcStroke.bottom << " "
					<< rcStroke.right - rcStroke.left << " " << rcStroke.top - rcStroke.bottom << " re S\n";
			}
			sApp << "Q\n";
		}
		else
		{
			rcStroke.left -= nBorderWidth / 2.0f;
			rcStroke.right += nBorderWidth / 2.0f;
			rcStroke.bottom -= nBorderWidth / 2.0f;
			rcStroke.top += nBorderWidth / 2.0f;

			sApp << "q\n";
			// If no border property, no need to generate ap stream for border.
			if (bBorder && nBorderWidth != 0)
			{
				sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), TRUE);
				sApp << rcStroke.left << " " << rcStroke.bottom << " "
					<< rcStroke.right - rcStroke.left << " " << rcStroke.top - rcStroke.bottom << " re f\n";
			}
			sApp << "Q\n";
		}

		if (!rcText.IsEmpty())
		{
			sApp << "q\n/Tx BMC\n";
			sApp << rcText.left << " " << rcText.bottom << " " 
				<< rcText.Width() << " " << rcText.Height() << " re\nW\nn\n";

			if (!Edit.m_bRichText)
				sApp << "BT\n" << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crText), FXARGB_G(crText), FXARGB_B(crText))) 
				<< sEdit << "ET\n" << "EMC\nQ\n";
			else
				sApp << "BT\n" << sEdit << "ET\n" << "EMC\nQ\n";
		}
	}

	CPDF_Stream* pStream = annotImp.writeAppearance("N", rcAnnot, mt, sApp, "", NULL);
	if (opacity < 0.999999 && opacity > -0.000001)
	{
		setStreamExtGState(pStream, FT_OPACITY_ALIAS, opacity);
	}

	return TRUE;
}

FS_BOOL FSPDFFreeTextImp::resetAppearance_TB(FX_BOOL bTextOverflow)
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	CFT_Edit Edit(this, bTextOverflow);

	FS_FLOAT nBorderWidth = annotImp.getBorderWidth();
	FS_INT32 effect = annotImp.getBorderEffect();
	FS_FLOAT fCloudyIntensity = annotImp.getCloudyIntensity();

	if (FSANNOT_BORDEREFFECT_CLOUDY == effect && FLOAT_ISBIGGER(fCloudyIntensity, 0.0))
	{
		CPDF_Rect rcText(0, 0, 0, 0);

		if (bTextOverflow)
			// The text rect of CFT_Edit will be calculated according to /Rect and /RD.
			rcText = Edit.GetTextBBox();
		else
			rcText = getTextRect();

		transformRectWithTextMatrix(rcText);

		CFX_ByteTextBuf sCloudy;
		CPDF_Rect rcAnnot = FSAnnot_CreateCloudyAP_Rectangle(sCloudy, rcText, fCloudyIntensity);
		rcAnnot.left -= nBorderWidth / 2.0f;
		rcAnnot.right += nBorderWidth / 2.0f;
		rcAnnot.bottom -= nBorderWidth / 2.0f;
		rcAnnot.top += nBorderWidth / 2.0f;

		annotImp.setRectImp(FSANNOT_RECTKEY_RECT, rcAnnot);
		CPDF_Rect rcRD(FXSYS_fabs(rcText.left - rcAnnot.left) , FXSYS_fabs(rcText.bottom - rcAnnot.bottom), FXSYS_fabs(rcAnnot.right - rcText.right), FXSYS_fabs(rcAnnot.top - rcText.top));
		annotImp.setRectImp(FSANNOT_RECTKEY_RD, rcRD);
	}
	else if (bTextOverflow)
	{
		//If Contents is empty, then do not change the rect, or it would result to very small rect.
		CFX_WideString ws = getUnicodeString(FSANNOT_STRINGKEY_CONTENT);
		if(!ws.IsEmpty())
		{
			// The text rect of CFT_Edit will be calculated according to /Rect and /RD.
			CPDF_Rect rcText = Edit.GetTextBBox();
			rcText.left -= nBorderWidth;
			rcText.right += nBorderWidth;
			rcText.bottom -= nBorderWidth;
			rcText.top += nBorderWidth;
			transformRectWithTextMatrix(rcText);
			CPDF_Rect rcAnnot = rcText;

			annotImp.setRectImp(FSANNOT_RECTKEY_RECT, rcAnnot);
			CPDF_Rect rcRD(FXSYS_fabs(rcText.left - rcAnnot.left) , FXSYS_fabs(rcText.bottom - rcAnnot.bottom), FXSYS_fabs(rcAnnot.right - rcText.right), FXSYS_fabs(rcAnnot.top - rcText.top));
			annotImp.setRectImp(FSANNOT_RECTKEY_RD, rcRD);
		}
	}

	CPDF_Matrix mt = Edit.GetCurMatrix();
	CPDF_Point ptOrigin = Edit.GetEditOrigin();
	CPDF_Matrix mtAnnot = CPDF_Matrix(1, 0, 0, 1, -ptOrigin.x, -ptOrigin.y);

	CPDF_Matrix mtReverse;
	mtReverse.SetReverse(mt);

	CPDF_Rect rcAnnot = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);
	mtReverse.TransformRect(rcAnnot);
	mt.Concat(mtAnnot);

	CFX_ByteString sEdit = Edit.GetAppearanceStream();

	FX_ARGB crText = 0;
	getTextColor(crText);
	FX_ARGB crFill;
	FX_BOOL bFill = annotImp.getFillColor(crFill);
	// TODO: need default border color??
	FX_ARGB crBorder = 0;//0xFFFF0000; //Use red as default border color, same as phantom.
	FX_BOOL bBorder = annotImp.getBorderColorImp(crBorder/*, !m_FSMarkupImp.getAnnotImp().isSetFreeTextBorderColor()*/);
	CFX_ByteTextBuf sApp;

	FS_FLOAT opa = getOpacity();
	int nOpacity = FXSYS_round(opa * 100);
	if (nOpacity <= 100 && nOpacity >= 0)
	{
		sApp << "/" << FT_OPACITY_ALIAS << " gs\n";
	}

	if (FSANNOT_BORDEREFFECT_CLOUDY == effect && FLOAT_ISBIGGER(fCloudyIntensity, 0.0))
	{
		CPDF_Rect rcRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);

		CPDF_Rect rcEdit = rcAnnot;

		if (rcEdit.Width() > rcRD.left + rcRD.right &&
			rcEdit.Height() > rcRD.top + rcRD.bottom)
		{
			rcEdit.left += rcRD.left;
			rcEdit.right -= rcRD.right;
			rcEdit.bottom += rcRD.bottom;
			rcEdit.top -= rcRD.top;

			CFX_ByteTextBuf sCloudy;
			CPDF_Rect rcBBox = FSAnnot_CreateCloudyAP_Rectangle(sCloudy, rcEdit, fCloudyIntensity);

			if (bFill)
			{
				sApp << "q\n";
				sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crFill), FXARGB_G(crFill), FXARGB_B(crFill)), TRUE);
				sApp << sCloudy << "f\nQ\n";
			}

			sApp << "q\n";
			// If no border property, no need to generate ap stream for border.
			if (bBorder)
			{
				sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), FALSE);
				sApp << nBorderWidth << " w\n";
			}
			sApp << sCloudy << "S\nQ\n";


			sApp << "q\n/Tx BMC\n";
			sApp << rcEdit.left << " " << rcEdit.bottom << " " 
				<< rcEdit.Width() << " " << rcEdit.Height() << " re\nW\nn\n";

			if (!Edit.m_bRichText)
				sApp << "BT\n" << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crText), FXARGB_G(crText), FXARGB_B(crText))) 
				<< sEdit << "ET\n" << "EMC\nQ\n";
			else
				sApp << "BT\n" << sEdit << "ET\n" << "EMC\nQ\n";
		}
	}
	else
	{
		CPDF_Rect rcFill = rcAnnot;

		if (rcFill.Width() > nBorderWidth * 2.0f &&
			rcFill.Height() > nBorderWidth * 2.0f)
		{
			rcFill.left += nBorderWidth;
			rcFill.right -= nBorderWidth;
			rcFill.bottom += nBorderWidth;
			rcFill.top -= nBorderWidth;

			if (bFill)
				sApp << CPWL_Utils::GetRectFillAppStream(rcFill, CPWL_Color(FXARGB_R(crFill), FXARGB_G(crFill), FXARGB_B(crFill)));
		}
		else
		{
			rcFill = CPDF_Rect(0,0,0,0);
		}

		FSM_BORDERSTYLE boderStyle = annotImp.getBorderStyle();

		CPDF_Rect rcStroke = rcAnnot;
		// If no border property, no need to generate ap stream for border.
		if (bBorder && nBorderWidth != 0)
		{
			if (rcStroke.Width() > nBorderWidth * 2.0f &&
				rcStroke.Height() > nBorderWidth * 2.0f)
			{
				sApp << "q\n";

				rcStroke.left += (nBorderWidth / 2.0f);
				rcStroke.right -= (nBorderWidth / 2.0f);
				rcStroke.bottom += (nBorderWidth / 2.0f);
				rcStroke.top -= (nBorderWidth / 2.0f);
				sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), FALSE);
				sApp << nBorderWidth << " w\n";

				if (FSAnnot::e_borderStyleDashed == boderStyle)
				{
					sApp << "q\n";
					rcStroke.left += (nBorderWidth / 2.0f);
					rcStroke.right -= (nBorderWidth / 2.0f);
					rcStroke.bottom += (nBorderWidth / 2.0f);
					rcStroke.top -= (nBorderWidth / 2.0f);
					sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), FALSE);
					sApp << nBorderWidth << " w\n";

					FX_FLOAT fDashOffset;
					CFX_FloatArray aDashPattern;
					annotImp.getBorderDash(fDashOffset, aDashPattern);

					FS_INT32 iArraySize = aDashPattern.GetSize();

					if (iArraySize == 1)
					{
						sApp << "[" << aDashPattern[0] << " " << aDashPattern[0] << "] 0 d\n";
					}
					else
					{
						sApp << "[";
						for (int i=0; i<iArraySize; i++)
						{
							sApp << aDashPattern[i];

							if (i != iArraySize - 1)
								sApp << " ";
						}
						sApp << "] 0 d\n";
					}
					sApp << rcStroke.left << " " << rcStroke.bottom << " "
						<< rcStroke.right - rcStroke.left << " " << rcStroke.top - rcStroke.bottom << " re S\n";
					sApp << "Q\n";
				}
				else {
					sApp << rcStroke.left << " " << rcStroke.bottom << " "
						<< rcStroke.right - rcStroke.left << " " << rcStroke.top - rcStroke.bottom << " re S\n";
				}

				sApp << "Q\n";
			}
			else
			{
				sApp << "q\n";
				sApp << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crBorder), FXARGB_G(crBorder), FXARGB_B(crBorder)), TRUE);
				sApp << rcStroke.left << " " << rcStroke.bottom << " "
					<< rcStroke.right - rcStroke.left << " " << rcStroke.top - rcStroke.bottom << " re f\n";
				sApp << "Q\n";
			}
		}

		if (!rcFill.IsEmpty())
		{
			sApp << "q\n/Tx BMC\n";
			sApp << rcFill.left << " " << rcFill.bottom << " " 
				<< rcFill.Width() << " " << rcFill.Height() << " re\nW\nn\n";

			if (!Edit.m_bRichText)
				sApp << "BT\n" << CPWL_Utils::GetColorAppStream(CPWL_Color(FXARGB_R(crText), FXARGB_G(crText), FXARGB_B(crText))) 
				<< sEdit << "ET\n" << "EMC\nQ\n";
			else
				sApp << "BT\n" << sEdit << "ET\n" << "EMC\nQ\n";
		}
	}

	CPDF_Stream* pStream = annotImp.writeAppearance("N", rcAnnot, mt, sApp, "", NULL);
	if (nOpacity <= 100 && nOpacity >= 0)
		setStreamExtGState(pStream, FT_OPACITY_ALIAS, nOpacity / 100.0f);

	return TRUE;
}

FS_BOOL FSPDFFreeTextImp::getFontInfo(CFX_ByteString& fontName, FS_FLOAT& fontSize)
{// Get font name from DA, and get font size from DS first, then DA.
	CFX_ByteString tempFontName = "";
	FS_FLOAT tempFontSize = 0;
	FSPDFAnnotImp annotImp = getAnnotImp();
	// If no /DA, /RC and /DS, no font info can be gotten.
	if (!annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTAP) && !annotImp.hasProperty(FSANNOT_STRINGKEY_RICHTEXT) 
		&& !annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTSTYLE))
		return FALSE;

	// First try to get font name(or tag name) from /DA
	if (annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTAP))
	{
		// Get font information from DA first.
		CFX_ByteString csDA = annotImp.getStringImpl(FSANNOT_STRINGKEY_DEFAULTAP);
		CFSAnnot_DefaultAppearance da(csDA);
		if (da.HasFont())
			da.GetFont(tempFontName, tempFontSize);
	}
	// Then check fontsize in DS(with RC) in order to match what is actually used for appearance in adobe/phantom.
	if (annotImp.hasProperty(FSANNOT_STRINGKEY_RICHTEXT) || annotImp.hasProperty(FSANNOT_STRINGKEY_DEFAULTSTYLE))
	{
		CFX_WideString wsRC = annotImp.getUnicodeString(FSANNOT_STRINGKEY_RICHTEXT);
		CFX_WideString wsDS = annotImp.getUnicodeString(FSANNOT_STRINGKEY_DEFAULTSTYLE);
		CPDF_RichTextXML richtextXML;
		if (richtextXML.SetXML(wsRC, wsDS))
		{
			CFX_WideString wsCSS, wsFontName;
			richtextXML.getFontSize(tempFontSize);
		}
	}
	if (tempFontName.IsEmpty() && !FLOAT_ISBIGGER(tempFontSize, 0))
		return FALSE;
	else
	{
		fontName = tempFontName;
		fontSize = tempFontSize;
		return TRUE;
	}
}
