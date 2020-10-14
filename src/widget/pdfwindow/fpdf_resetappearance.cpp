#include "../../inc/fpdf_define.h"
#include "include/fpdf_resetappearance.h"
//#include "../../annotation/fpdfannot_imp.h"
#include "include/pdfwindow/ipdfwindow.h"
#include "../fpwl_fontmap.h"

#if defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_) || defined(_FSDK_PDFFORM_R_) || defined(_FSDK_PDFFORM_W_)

#define IsFloatZero(f)		((f) < 0.01 && (f) > -0.01)

CFSCRT_STPDFResetApperance::CFSCRT_STPDFResetApperance(CPDF_FormControl* control)
{
	m_pControl = control;
}

CFSCRT_STPDFResetApperance::~CFSCRT_STPDFResetApperance()
{

}

 CPDF_Rect CFSCRT_STPDFResetApperance::GetRotatedRect()
{
	FXSYS_assert(m_pControl !=NULL);

	CPDF_Rect rectAnnot = m_pControl->GetRect();
	rectAnnot.Normalize();

	FX_FLOAT fWidth = rectAnnot.right - rectAnnot.left;
	FX_FLOAT fHeight = rectAnnot.top - rectAnnot.bottom;

	CPDF_Rect rcPDFWindow;
	switch(abs(m_pControl->GetRotation() % 360))
	{
	case 0:
	case 180:
	default:
		rcPDFWindow = CPDF_Rect(0, 0, fWidth, fHeight);	
		break;
	case 90:
	case 270:
		rcPDFWindow = CPDF_Rect(0, 0, fHeight, fWidth);
		break;
	}

	return rcPDFWindow;
}


FS_RESULT	CFSCRT_STPDFResetApperance::ST_ResetAppearance_RadioButton()
{
	FXSYS_assert(m_pControl !=NULL);

	CPDF_FormField* pField = m_pControl->GetField();
	FXSYS_assert(pField != NULL);

	CPWL_Color crBackground, crBorder, crText;

	int iColorType = 0;// as output parameter
	FX_FLOAT fc[4];

	m_pControl->GetOriginalBackgroundColor(iColorType, fc);
	if (iColorType > 0)
		crBackground = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	m_pControl->GetOriginalBorderColor(iColorType, fc);
	if (iColorType > 0)
		crBorder = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	FSBorderInfo border;

	FS_RESULT ret = WidgetGetBorder(&border);

	if(ret != FSCRT_ERRCODE_SUCCESS) return ret;

	FX_FLOAT fBorderWidth = border.width;
	FX_INT32 nBorderStyle = 0;
	CPWL_Dash dsBorder(3,0,0);
	CPWL_Color crLeftTop,crRightBottom;

	switch (border.style)
	{
	case FSAnnot::e_borderStyleDashed:
		nBorderStyle = PBS_DASH;
		dsBorder = CPWL_Dash(3, 3, 0);
		break;
	case FSAnnot::e_borderStyleBeveled:
		nBorderStyle = PBS_BEVELED;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,1);
		crRightBottom = CPWL_Utils::DevideColor(crBackground,2);
		break;
	case FSAnnot::e_borderStyleInset:
		nBorderStyle = PBS_INSET;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,0.5);
		crRightBottom = CPWL_Color(COLORTYPE_GRAY,0.75);
		break;
	case FSAnnot::e_borderStyleUnderLine:
		nBorderStyle = PBS_UNDERLINED;
		break;
	default: 
		nBorderStyle = PBS_SOLID;
		break;
	}

	CPDF_Rect rcWindow = GetRotatedRect();
	
	CPDF_Rect rcClient = CPWL_Utils::DeflateRect(rcWindow, fBorderWidth);

	CPDF_DefaultAppearance da = m_pControl->GetDefaultAppearance();
	if (da.HasColor(FALSE))
	{
		da.GetColor(iColorType, fc, FALSE);
		crText = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);
	}

	FX_INT32 nStyle = 0;

	CFX_WideString csWCaption = m_pControl->GetNormalCaption();
	if (csWCaption.GetLength() > 0)
	{
		switch (csWCaption[0])
		{
		default: //L'l':
			nStyle = PCS_CIRCLE;			
			break;
		case L'8':
			nStyle = PCS_CROSS;
			break;
		case L'u':
			nStyle = PCS_DIAMOND;
			break;
		case L'n':
			nStyle = PCS_SQUARE;
			break;
		case L'H':
			nStyle = PCS_STAR;
			break;
		case L'4':
			nStyle = PCS_CHECK;
			break;
		}
	}
	else
	{
		nStyle = PCS_CIRCLE;
	}

	CFX_ByteString csAP_N_ON;

	CPDF_Rect rcCenter = CPWL_Utils::DeflateRect(CPWL_Utils::GetCenterSquare(rcWindow), 1.0f);

	if (nStyle == PCS_CIRCLE)
	{
		if (nBorderStyle == PBS_BEVELED)
		{
			crLeftTop = CPWL_Color(COLORTYPE_GRAY, 1);
			crRightBottom = CPWL_Utils::SubstractColor(crBackground,0.25f);
		}
		else if (nBorderStyle == PBS_INSET)
		{
			crLeftTop = CPWL_Color(COLORTYPE_GRAY,0.5f);
			crRightBottom = CPWL_Color(COLORTYPE_GRAY,0.75f);
		}

		csAP_N_ON = CPWL_Utils::GetCircleFillAppStream(rcCenter,crBackground) + 
			CPWL_Utils::GetCircleBorderAppStream(rcCenter,fBorderWidth,crBorder,crLeftTop,crRightBottom,nBorderStyle,dsBorder);
	}
	else
	{
		csAP_N_ON = CPWL_Utils::GetRectFillAppStream(rcWindow,crBackground) + 
			CPWL_Utils::GetBorderAppStream(rcWindow,fBorderWidth,crBorder,crLeftTop,crRightBottom,nBorderStyle,dsBorder);
	}

	CFX_ByteString csAP_N_OFF = csAP_N_ON;

	switch (nBorderStyle)
	{
	case PBS_BEVELED:
		{
			CPWL_Color crTemp = crLeftTop;
			crLeftTop = crRightBottom;
			crRightBottom = crTemp;
		}
		break;
	case PBS_INSET:
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,0);
		crRightBottom = CPWL_Color(COLORTYPE_GRAY,1);
		break;
	}

	CFX_ByteString csAP_D_ON;

	if (nStyle == PCS_CIRCLE)
	{
		CPWL_Color crBK = CPWL_Utils::SubstractColor(crBackground,0.25f);
		if (nBorderStyle == PBS_BEVELED)
		{
			crLeftTop = CPWL_Utils::SubstractColor(crBackground,0.25f);
			crRightBottom = CPWL_Color(COLORTYPE_GRAY, 1);
			crBK = crBackground;
		}
		else if (nBorderStyle == PBS_INSET)
		{
			crLeftTop = CPWL_Color(COLORTYPE_GRAY,0);
			crRightBottom = CPWL_Color(COLORTYPE_GRAY,1);
		}

		csAP_D_ON = CPWL_Utils::GetCircleFillAppStream(rcCenter,crBK)
			+ CPWL_Utils::GetCircleBorderAppStream(rcCenter,fBorderWidth,crBorder,crLeftTop,crRightBottom,nBorderStyle,dsBorder);
	}
	else
	{
		csAP_D_ON = CPWL_Utils::GetRectFillAppStream(rcWindow,CPWL_Utils::SubstractColor(crBackground,0.25f)) + 
			CPWL_Utils::GetBorderAppStream(rcWindow,fBorderWidth,crBorder,crLeftTop,crRightBottom,nBorderStyle,dsBorder);		
	}

	CFX_ByteString csAP_D_OFF = csAP_D_ON;

	csAP_N_ON += CPWL_Utils::GetRadioButtonAppStream(rcClient,nStyle,crText);
	csAP_D_ON += CPWL_Utils::GetRadioButtonAppStream(rcClient,nStyle,crText);

	CPDF_Rect rect = GetRotatedRect();
	CPDF_Matrix matrix =  GetMatrix();
	CFX_ByteString checkedAPState = m_pControl->GetCheckedAPState();
	WriteAppearance("N", rect, matrix, csAP_N_ON, checkedAPState);
	WriteAppearance("N", rect, matrix, csAP_N_OFF, "Off");

	WriteAppearance("D", rect, matrix, csAP_D_ON, checkedAPState);
	WriteAppearance("D", rect, matrix, csAP_D_OFF, "Off");

	CFX_ByteString csAS =GetAppState();
	if (csAS.IsEmpty())
		SetAppState("Off");

	return 0;
}

void CFSCRT_STPDFResetApperance::WriteAppearance(const CFX_ByteString& sAPType, const CPDF_Rect& rcBBox, 
	const CPDF_Matrix& matrix, const CFX_ByteString& sContents,
	const CFX_ByteString& sAPState)


{
/*
	FXSYS_assert(m_pLTControl !=NULL);
	CPDFAnnot_Base* pAnnot = m_pLTControl->m_pAnnot;
	FXSYS_assert(pAnnot != NULL);
	FXSYS_assert(pAnnot->GetAnnotDict() != NULL);

	CPDF_Dictionary* pAPDict = pAnnot->GetAnnotDict()->GetDict("AP");

	if (!pAPDict) 
	{
		pAPDict = FX_NEW CPDF_Dictionary;
		pAnnot->GetAnnotDict()->SetAt("AP", pAPDict);
	}
	*/
	CPDF_Dictionary* pAPDict = m_pControl->GetWidget()->GetDict("AP");

	if (!pAPDict) 
	{
		pAPDict = FX_NEW CPDF_Dictionary;
		m_pControl->GetWidget()->SetAt("AP", pAPDict);
	}

	CPDF_Stream* pStream = NULL;
	CPDF_Dictionary* pParentDict = NULL;

	CFX_ByteString apType = sAPType;
	if (sAPState.IsEmpty())
	{
		pParentDict = pAPDict;
		pStream = pAPDict->GetStream(apType);
	}
	else
	{
		CPDF_Object* pObj = pAPDict->GetElement(apType);
		if (!pObj || pObj->GetType() == PDFOBJ_REFERENCE)
		{
			pObj = FX_NEW CPDF_Dictionary;
			pAPDict->SetAt(apType, pObj);
		}

		pParentDict = (CPDF_Dictionary*)pObj;
		pStream = ((CPDF_Dictionary*)pObj)->GetStream(sAPState);
		apType = sAPState;
	}

	if (!pStream) 
	{
	//	FXSYS_assert(m_pPageView != NULL);
		CPDF_Document* pDoc = m_pControl->GetInterForm()->GetDocument();
		FXSYS_assert(pDoc !=NULL);
	//	CPDF_Document* pDoc = m_pLTControl->GetPDFPage()->m_pDocument;
		
		pStream = FX_NEW CPDF_Stream(NULL, 0, NULL);
		FX_INT32 objnum = pDoc->AddIndirectObject(pStream);
		//pAPDict->SetAtReference(sAPType, pDoc, objnum);
		ASSERT(pParentDict != NULL);
		pParentDict->SetAtReference(apType, pDoc, objnum);
	}

	CPDF_Dictionary * pStreamDict = pStream->GetDict();

	if (!pStreamDict)
	{
		pStreamDict = FX_NEW CPDF_Dictionary;
		pStreamDict->SetAtName("Type", "XObject");
		pStreamDict->SetAtName("Subtype", "Form");
		pStreamDict->SetAtInteger("FormType", 1);
		pStream->InitStream(NULL,0,pStreamDict);
	}

	if (pStreamDict)
	{
		pStreamDict->SetAtMatrix("Matrix",matrix);	
		pStreamDict->SetAtRect("BBox", rcBBox);		
	}

	pStream->SetData((FX_BYTE*)(FX_LPCSTR)sContents, sContents.GetLength(), FALSE, FALSE);
}


FS_RESULT	CFSCRT_STPDFResetApperance::ST_ResetAppearance_PushButton(IFX_Edit_FontMap* fontmap)
{

//	FXSYS_assert(m_pLTControl !=NULL);
//	CPDF_FormControl* pControl = m_pLTControl->GetCPDFFormControl();
	FXSYS_assert(m_pControl != NULL);
	CPDF_FormField* pField = m_pControl->GetField();
	FXSYS_assert(pField != NULL);

	CPDF_Rect rcWindow = GetRotatedRect();

	FX_INT32 nLayout = 0;

	switch (m_pControl->GetTextPosition())
	{
	case TEXTPOS_ICON:
		nLayout = PPBL_ICON;
		break;
	case TEXTPOS_BELOW:
		nLayout = PPBL_ICONTOPLABELBOTTOM;
		break;
	case TEXTPOS_ABOVE:
		nLayout = PPBL_LABELTOPICONBOTTOM;
		break;
	case TEXTPOS_RIGHT:
		nLayout = PPBL_ICONLEFTLABELRIGHT;
		break;
	case TEXTPOS_LEFT:
		nLayout = PPBL_LABELLEFTICONRIGHT;
		break;
	case TEXTPOS_OVERLAID:
		nLayout = PPBL_LABELOVERICON;
		break;
	default:
		nLayout = PPBL_LABEL;
		break;
	}

	CPWL_Color crBackground, crBorder;

	int iColorType;
	FX_FLOAT fc[4];

	m_pControl->GetOriginalBackgroundColor(iColorType, fc);
	if (iColorType > 0)
		crBackground = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	m_pControl->GetOriginalBorderColor(iColorType, fc);
	if (iColorType > 0)
		crBorder = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);


	FSBorderInfo border;

	// now, pAnnot have not be used ,so comment out the next lines.
	/*CPDFAnnot_BaseData*  pAnnot = FPDFAnnot_CreateBase();
	pAnnot->LoadAnnot(m_pControl->GetWidget(), NULL);*/
	FS_RESULT ret = WidgetGetBorder(&border);
	if(ret != FSCRT_ERRCODE_SUCCESS) return ret;

	FX_FLOAT fBorderWidth = border.width;
	FX_INT32 nBorderStyle = 0;
	CPWL_Dash dsBorder(3,0,0);
	CPWL_Color crLeftTop,crRightBottom;

	switch (border.style)
	{
	case FSAnnot::e_borderStyleDashed:
		nBorderStyle = PBS_DASH;
		dsBorder = CPWL_Dash(3, 3, 0);
		break;
	case FSAnnot::e_borderStyleBeveled:
		nBorderStyle = PBS_BEVELED;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,1);
		crRightBottom = CPWL_Utils::DevideColor(crBackground,2);
		break;
	case FSAnnot::e_borderStyleInset:
		nBorderStyle = PBS_INSET;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,0.5);
		crRightBottom = CPWL_Color(COLORTYPE_GRAY,0.75);
		break;
	case FSAnnot::e_borderStyleUnderLine:
		nBorderStyle = PBS_UNDERLINED;
		break;
	default: 
		nBorderStyle = PBS_SOLID;
		break;
	}

	CPDF_Rect rcClient = CPWL_Utils::DeflateRect(rcWindow,fBorderWidth);	

	CPWL_Color crText(COLORTYPE_GRAY,0);

	FX_FLOAT fFontSize = 12.0f;
	CFX_ByteString csNameTag;

	CPDF_DefaultAppearance da = m_pControl->GetDefaultAppearance();
	if (da.HasColor(FALSE))
	{
		da.GetColor(iColorType, fc, FALSE);
		crText = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);
	}

	if (da.HasFont()) 
		da.GetFont(csNameTag, fFontSize);

	CFX_WideString csNormalCaption, csRolloverCaption, csDownCaption;
	if (m_pControl->HasMKEntry("CA"))
	{
		csNormalCaption = m_pControl->GetNormalCaption();
	}
	if (m_pControl->HasMKEntry("RC"))
	{
		csRolloverCaption = m_pControl->GetRolloverCaption();
	}
	if (m_pControl->HasMKEntry("AC"))
	{
		csDownCaption = m_pControl->GetDownCaption();
	}

	CPDF_Stream* pNormalIcon = NULL;
	CPDF_Stream* pRolloverIcon = NULL;
	CPDF_Stream* pDownIcon = NULL;

	if (m_pControl->HasMKEntry("I"))
	{
		pNormalIcon = m_pControl->GetNormalIcon();
	}
	if (m_pControl->HasMKEntry("RI"))
	{
		pRolloverIcon = m_pControl->GetRolloverIcon();
	}
	if (m_pControl->HasMKEntry("IX"))
	{
		pDownIcon = m_pControl->GetDownIcon();
	}

	if (pNormalIcon)
	{
		if (CPDF_Dictionary* pImageDict = pNormalIcon->GetDict())
		{
			if (pImageDict->GetString("Name").IsEmpty())
				pImageDict->SetAtString("Name", "ImgA");
		}
	}

	if (pRolloverIcon)
	{
		if (CPDF_Dictionary* pImageDict = pRolloverIcon->GetDict())
		{
			if (pImageDict->GetString("Name").IsEmpty())
				pImageDict->SetAtString("Name", "ImgB");
		}
	}

	if (pDownIcon)
	{
		if (CPDF_Dictionary* pImageDict = pDownIcon->GetDict())
		{
			if (pImageDict->GetString("Name").IsEmpty())
				pImageDict->SetAtString("Name", "ImgC");
		}
	}

	CPDF_IconFit iconFit = m_pControl->GetIconFit();

//	FXSYS_assert(m_pLTControl->GetPDFPage() != NULL);
//	FXSYS_assert(m_pLTControl->GetPDFPage()->m_pDocument != NULL);

#if 0
	CPDFDoc_Environment* pEnv = pDoc->GetEnv();


	CBA_FontMap FontMap(this,pEnv->GetSysHandler());//, ISystemHandle::GetSystemHandler(m_pBaseForm->GetEnv()));
	FontMap.Initial();

	FontMap.SetAPType("N");
#endif

	rcWindow = GetRotatedRect();
	CPDF_Matrix matrix = GetMatrix();

	CFX_ByteString csAP = CPWL_Utils::GetRectFillAppStream(rcWindow, crBackground) + 
		CPWL_Utils::GetBorderAppStream(rcWindow, fBorderWidth, crBorder, crLeftTop, crRightBottom, nBorderStyle, dsBorder) +
		CPWL_Utils::GetPushButtonAppStream(iconFit.GetFittingBounds() ? rcWindow : rcClient, fontmap, pNormalIcon, iconFit, csNormalCaption, crText, fFontSize, nLayout);
		WriteAppearance("N", rcWindow, matrix, csAP, "");
	if (pNormalIcon)
			AddImageToAppearance("N", pNormalIcon);

	CPDF_FormControl::HighlightingMode eHLM = m_pControl->GetHighlightingMode();
	if (eHLM == CPDF_FormControl::Push || eHLM == CPDF_FormControl::Toggle)
	{
		if (csRolloverCaption.IsEmpty() && !pRolloverIcon)			
		{
			csRolloverCaption = csNormalCaption;
			pRolloverIcon = pNormalIcon;
		}

		//We should set font to /Resource <</Font >> in "R" dictionary to avoid unrecognizable code.
		//If /Resource in "R" dictionary doesn't exist,this funciton will create a new one.
		((CBA_FontMap*) fontmap)->SetAPType("R");
		
		csAP = CPWL_Utils::GetRectFillAppStream(rcWindow, crBackground) + 
			CPWL_Utils::GetBorderAppStream(rcWindow, fBorderWidth, crBorder, crLeftTop, crRightBottom, nBorderStyle, dsBorder) +
			CPWL_Utils::GetPushButtonAppStream(iconFit.GetFittingBounds() ? rcWindow : rcClient, fontmap/*&FontMap*/, pRolloverIcon, iconFit, csRolloverCaption, crText, fFontSize, nLayout);

		CPDF_Rect rcWindow  = GetRotatedRect();
		CPDF_Matrix matrix = GetMatrix();

		WriteAppearance("R", rcWindow, matrix, csAP,"");
		if (pRolloverIcon)
			AddImageToAppearance("R", pRolloverIcon);

		if (csDownCaption.IsEmpty() && !pDownIcon)
		{
			csDownCaption = csNormalCaption;
			pDownIcon = pNormalIcon;
		}

		switch (nBorderStyle)
		{
		case PBS_BEVELED:
			{
				CPWL_Color crTemp = crLeftTop;
				crLeftTop = crRightBottom;
				crRightBottom = crTemp;
			}
			break;
		case PBS_INSET:
			crLeftTop = CPWL_Color(COLORTYPE_GRAY,0);
			crRightBottom = CPWL_Color(COLORTYPE_GRAY,1);
			break;
		}

		//We should set font to /Resource <</Font >> in "D" dictionary to avoid unrecognizable code.
		//If /Resource in "D" dictionary doesn't exist,this funciton will create a new one.
		((CBA_FontMap*) fontmap)->SetAPType("D");

		csAP = CPWL_Utils::GetRectFillAppStream(rcWindow, CPWL_Utils::SubstractColor(crBackground,0.25f)) + 
			CPWL_Utils::GetBorderAppStream(rcWindow, fBorderWidth, crBorder, crLeftTop, crRightBottom, nBorderStyle, dsBorder) + 
			CPWL_Utils::GetPushButtonAppStream(iconFit.GetFittingBounds() ? rcWindow : rcClient, fontmap/*&FontMap*/, pDownIcon, iconFit, csDownCaption, crText, fFontSize, nLayout);

		WriteAppearance("D", rcWindow, matrix, csAP,"");
		if (pDownIcon)
			AddImageToAppearance("D", pDownIcon);
	}
	else
	{
		RemoveAppearance("D");
		RemoveAppearance("R");
	}

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	CFSCRT_STPDFResetApperance::ST_ResetAppearance_CheckBox()
{

	FXSYS_assert(m_pControl !=NULL);

	CPDF_FormField* pField = m_pControl->GetField();
	FXSYS_assert(pField != NULL);


	CPWL_Color crBackground, crBorder, crText;

	int iColorType;
	FX_FLOAT fc[4];

	m_pControl->GetOriginalBackgroundColor(iColorType, fc);
	if (iColorType > 0)
		crBackground = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	m_pControl->GetOriginalBorderColor(iColorType, fc);
	if (iColorType > 0)
		crBorder = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	FSBorderInfo border;

	FS_RESULT ret = WidgetGetBorder(&border);
	if(ret != FSCRT_ERRCODE_SUCCESS) return ret;

	FX_FLOAT fBorderWidth = border.width;
	FX_INT32 nBorderStyle = 0;
	CPWL_Dash dsBorder(3,0,0);
	CPWL_Color crLeftTop,crRightBottom;
	switch (border.style)
	{
	case FSAnnot::e_borderStyleDashed:
		nBorderStyle = PBS_DASH;
		dsBorder = CPWL_Dash(3, 3, 0);
		break;
	case FSAnnot::e_borderStyleBeveled:
		nBorderStyle = PBS_BEVELED;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,1);
		crRightBottom = CPWL_Utils::DevideColor(crBackground,2);
		break;
	case FSAnnot::e_borderStyleInset:
		nBorderStyle = PBS_INSET;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,0.5);
		crRightBottom = CPWL_Color(COLORTYPE_GRAY,0.75);
		break;
	case FSAnnot::e_borderStyleUnderLine:
		nBorderStyle = PBS_UNDERLINED;
		break;
	default: 
		nBorderStyle = PBS_SOLID;
		break;
	}

	CPDF_Rect rcWindow = GetRotatedRect();
	CPDF_Rect rcClient = CPWL_Utils::DeflateRect(rcWindow,fBorderWidth);

	CPDF_DefaultAppearance da = m_pControl->GetDefaultAppearance();
	if (da.HasColor(FALSE))
	{
		da.GetColor(iColorType, fc, FALSE);
		crText = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);
	}

	FX_INT32 nStyle = 0;

	CFX_WideString csWCaption = m_pControl->GetNormalCaption();
	if (csWCaption.GetLength() > 0)
	{
		switch (csWCaption[0])
		{
		case L'l':
			nStyle = PCS_CIRCLE;			
			break;
		case L'8':
			nStyle = PCS_CROSS;
			break;
		case L'u':
			nStyle = PCS_DIAMOND;
			break;
		case L'n':
			nStyle = PCS_SQUARE;
			break;
		case L'H':
			nStyle = PCS_STAR;
			break;
		default: //L'4'
			nStyle = PCS_CHECK;
			break;
		}
	}
	else
	{
		nStyle = PCS_CHECK;
	}

	CFX_ByteString csAP_N_ON = CPWL_Utils::GetRectFillAppStream(rcWindow,crBackground) +
		CPWL_Utils::GetBorderAppStream(rcWindow,fBorderWidth,crBorder,crLeftTop,crRightBottom,nBorderStyle,dsBorder);

	CFX_ByteString csAP_N_OFF = csAP_N_ON;

	switch (nBorderStyle)
	{
	case PBS_BEVELED:
		{
			CPWL_Color crTemp = crLeftTop;
			crLeftTop = crRightBottom;
			crRightBottom = crTemp;
		}
		break;
	case PBS_INSET:
		crLeftTop = CPWL_Color(COLORTYPE_GRAY,0);
		crRightBottom = CPWL_Color(COLORTYPE_GRAY,1);
		break;
	}

	CFX_ByteString csAP_D_ON = CPWL_Utils::GetRectFillAppStream(rcWindow,CPWL_Utils::SubstractColor(crBackground,0.25f)) + 
		CPWL_Utils::GetBorderAppStream(rcWindow,fBorderWidth,crBorder,crLeftTop,crRightBottom,nBorderStyle,dsBorder);

	CFX_ByteString csAP_D_OFF = csAP_D_ON;

	csAP_N_ON += CPWL_Utils::GetCheckBoxAppStream(rcClient,nStyle,crText);
	csAP_D_ON += CPWL_Utils::GetCheckBoxAppStream(rcClient,nStyle,crText);

	rcWindow = GetRotatedRect();
	CPDF_Matrix matrix = GetMatrix();

	WriteAppearance("N", rcWindow, matrix, csAP_N_ON, m_pControl->GetCheckedAPState());
	WriteAppearance("N", rcWindow, matrix, csAP_N_OFF, "Off");

	WriteAppearance("D", rcWindow, matrix, csAP_D_ON, m_pControl->GetCheckedAPState());
	WriteAppearance("D", rcWindow, matrix, csAP_D_OFF, "Off");

	CFX_ByteString csAS = GetAppState();
	if (csAS.IsEmpty())
		SetAppState("Off");

	return FSCRT_ERRCODE_SUCCESS;
}

FS_RESULT	CFSCRT_STPDFResetApperance::ST_ResetAppearance_ComboBox(IFX_Edit_FontMap* fontmap, FX_LPCWSTR sValue)
{
	FXSYS_assert(m_pControl !=NULL);

	CPDF_FormField* pField = m_pControl->GetField();
	FXSYS_assert(pField != NULL);

	CFX_ByteTextBuf sBody, sLines;

	CPDF_Rect rcClient = GetClientRect();
	CPDF_Rect rcButton = rcClient;
	rcButton.left = rcButton.right - 13;
	rcButton.Normalize();

	if (IFX_Edit * pEdit = IFX_Edit::NewEdit())
	{
		pEdit->EnableRefresh(FALSE);


/*
		CPDFDoc_Environment* pEnv = pDoc->GetEnv();
		CBA_FontMap FontMap(this,pEnv->GetSysHandler());//, ISystemHandle::GetSystemHandler(m_pBaseForm->GetEnv()));
		FontMap.Initial();
		*/

		pEdit->SetFontMap(fontmap);

		CPDF_Rect rcEdit = rcClient;
		rcEdit.right = rcButton.left;
		rcEdit.Normalize();
		
		pEdit->SetPlateRect(rcEdit);
		pEdit->SetAlignmentV(1);

		SetFontSize(pEdit, TRUE);
		pEdit->Initialize();
				
		if (sValue)
			pEdit->SetText(sValue);
		else
		{
			FX_INT32 nCurSel = pField->GetSelectedIndex(0);

			/*
			if (nCurSel < 0)
				nCurSel = pField->FindOptionValue(pField->GetValue());
			*/

			if (nCurSel < 0)
				pEdit->SetText((FX_LPCWSTR)pField->GetValue());
			else
				pEdit->SetText((FX_LPCWSTR)pField->GetOptionLabel(nCurSel));
		}

		CPDF_Rect rcContent = pEdit->GetContentRect();

		CFX_ByteString sEdit = CPWL_Utils::GetEditAppStream(pEdit,CPDF_Point(0.0f,0.0f));
		if (sEdit.GetLength() > 0)
		{
			sBody << "/Tx BMC\n" << "q\n";
			if (rcContent.Width() > rcEdit.Width() ||
				rcContent.Height() > rcEdit.Height())
			{
				sBody << rcEdit.left << " " << rcEdit.bottom << " " 
					<< rcEdit.Width() << " " << rcEdit.Height() << " re\nW\nn\n";
			}

			CPWL_Color crText = GetTextPWLColor();	
			sBody << "BT\n" << CPWL_Utils::GetColorAppStream(crText) << sEdit << "ET\n" << "Q\nEMC\n";
		}

		IFX_Edit::DelEdit(pEdit);
	}

	sBody << CPWL_Utils::GetDropButtonAppStream(rcButton);

	CFX_ByteString sAP = GetBackgroundAppStream() + GetBorderAppStream() + sLines.GetByteString() + sBody.GetByteString();

	WriteAppearance("N", GetRotatedRect(), GetMatrix(), sAP, "");
	
	return FSCRT_ERRCODE_SUCCESS;
}

CPDF_Rect CFSCRT_STPDFResetApperance::GetClientRect( )
{
	FXSYS_assert(m_pControl !=NULL);
//	CPDFAnnot_Base* pAnnot = m_pLTControl->m_pAnnot;
//	FXSYS_assert(pAnnot != NULL);

	CPDF_Rect rcWindow	= GetRotatedRect();

	FSBorderInfo border;
	WidgetGetBorder(&border);

	FX_FLOAT fBorderWidth = (FX_FLOAT)border.width;
	switch (border.style)
	{
	case FSAnnot::e_borderStyleBeveled:
	case FSAnnot::e_borderStyleInset:
		fBorderWidth *= 2.0f;
		break;
	}

	return CPWL_Utils::DeflateRect(rcWindow, fBorderWidth);
}


FS_RESULT CFSCRT_STPDFResetApperance::ST_ResetAppearance_ListBox(IFX_Edit_FontMap* fontmap)
{
	FXSYS_assert(m_pControl !=NULL);

	CPDF_FormField* pField = m_pControl->GetField();
	FXSYS_assert(pField != NULL);
	
	CPDF_Rect rcClient = GetClientRect();

	CFX_ByteTextBuf sBody, sLines;

	if (IFX_Edit * pEdit = IFX_Edit::NewEdit())
	{
		pEdit->EnableRefresh(FALSE);
#if 0
		FXSYS_assert(this->m_pInterForm != NULL);
		CPDFSDK_Document* pDoc = m_pInterForm->GetDocument();
		ASSERT(pDoc != NULL);
		CPDFDoc_Environment* pEnv = pDoc->GetEnv();

		CBA_FontMap FontMap(this,pEnv->GetSysHandler());//, ISystemHandle::GetSystemHandler(m_pBaseForm->GetEnv()));
		FontMap.Initial();
#endif
		pEdit->SetFontMap(fontmap);

		pEdit->SetPlateRect(CPDF_Rect(rcClient.left,0.0f,rcClient.right,0.0f));	
		pEdit->SetAlignmentH(m_pControl->GetControlAlignment());
		SetFontSize(pEdit);
		pEdit->Initialize();
		
		CFX_ByteTextBuf sList;
		FX_FLOAT fy = rcClient.top;

		FX_INT32 nTop = pField->GetTopVisibleIndex();
		FX_INT32 nCount = pField->CountOptions();
		FX_INT32 nSelCount = pField->CountSelectedItems();

		for (FX_INT32 i=nTop; i<nCount; i++)
		{
			FX_BOOL bSelected = FALSE;				
			for (FX_INT32 j=0; j<nSelCount; j++)
			{
				if (pField->GetSelectedIndex(j) == i)
				{
					bSelected = TRUE;
					break;
				}
			}

			pEdit->SetText((FX_LPCWSTR)pField->GetOptionLabel(i));

			CPDF_Rect rcContent = pEdit->GetContentRect();
			FX_FLOAT fItemHeight = rcContent.Height();

			if (bSelected)
			{
				CPDF_Rect rcItem = CPDF_Rect(rcClient.left,fy-fItemHeight,rcClient.right,fy);
				sList << "q\n" << CPWL_Utils::GetColorAppStream(CPWL_Color(COLORTYPE_RGB,0,51.0f/255.0f,113.0f/255.0f),TRUE)
					<< rcItem.left << " " << rcItem.bottom << " " << rcItem.Width() << " " << rcItem.Height() << " re f\n" << "Q\n";

				sList << "BT\n" << CPWL_Utils::GetColorAppStream(CPWL_Color(COLORTYPE_GRAY,1),TRUE) << 
					CPWL_Utils::GetEditAppStream(pEdit,CPDF_Point(0.0f,fy)) << "ET\n";
			}
			else
			{
				
				CPWL_Color crText = GetTextPWLColor();
				sList << "BT\n" << CPWL_Utils::GetColorAppStream(crText,TRUE) << 
					CPWL_Utils::GetEditAppStream(pEdit,CPDF_Point(0.0f,fy)) << "ET\n";
					
			}

			fy -= fItemHeight;
		}

		if (sList.GetSize() > 0)
		{
			sBody << "/Tx BMC\n" << "q\n" << rcClient.left << " " << rcClient.bottom << " " 
				<< rcClient.Width() << " " << rcClient.Height() << " re\nW\nn\n";
			sBody << sList << "Q\nEMC\n";
		}

		IFX_Edit::DelEdit(pEdit);
	}

	CFX_ByteString sAP = GetBackgroundAppStream() + GetBorderAppStream() + sLines.GetByteString() + sBody.GetByteString();

	CPDF_Rect pdfRect = GetRotatedRect();
	CPDF_Matrix matrix = GetMatrix();

	WriteAppearance("N", pdfRect, matrix, sAP, "");
	return FSCRT_ERRCODE_SUCCESS;
}


FS_RESULT CFSCRT_STPDFResetApperance::ST_ResetAppearance_TextField(IFX_Edit_FontMap* fontmap, FX_LPCWSTR sValue)
{
	FXSYS_assert(m_pControl !=NULL);

	CPDF_FormField* pField = m_pControl->GetField();
	FXSYS_assert(pField != NULL);

	CFX_ByteTextBuf sBody, sLines;

	if (IFX_Edit * pEdit = IFX_Edit::NewEdit())
	{
		pEdit->EnableRefresh(FALSE);

/*
		ASSERT(this->m_pInterForm != NULL);
		CPDFSDK_Document* pDoc = m_pInterForm->GetDocument();
		ASSERT(pDoc != NULL);
		CPDFDoc_Environment* pEnv = pDoc->GetEnv();

		CBA_FontMap FontMap(this,pEnv->GetSysHandler());//, ISystemHandle::GetSystemHandler(m_pBaseForm->GetEnv()));
		FontMap.Initial();
		
		pEdit->SetFontMap(&FontMap);
		*/
		/***** those codes are cut from below, it's used by SetPlateRect().*****/
		FSBorderInfo border;
		FS_RESULT ret = WidgetGetBorder(&border);
		if(ret != FSCRT_ERRCODE_SUCCESS) return ret;
		FX_FLOAT fBorderWidth = border.width;

		pEdit->SetFontMap(fontmap);
		CPDF_Rect rcClient = GetClientRect();
		pEdit->SetPlateRect(CPWL_Utils::DeflateRect(rcClient, border.style > 2 ? fBorderWidth*3 : fBorderWidth));//we should margin the text field, and it's width is depend on the border style.
		pEdit->SetAlignmentH(m_pControl->GetControlAlignment());

		FX_DWORD dwFieldFlags = pField->GetFieldFlags();
		FX_BOOL bMultiLine = (dwFieldFlags >> 12) & 1;

		if (bMultiLine)
		{
			pEdit->SetMultiLine(TRUE);
			pEdit->SetAutoReturn(TRUE);
		}
		else
		{
			pEdit->SetAlignmentV(1);
		}

		FX_WORD subWord = 0;
		if ((dwFieldFlags >> 13) & 1)
		{
			subWord = '*';
			pEdit->SetPasswordChar(subWord);
		}

		int nMaxLen = pField->GetMaxLen();
		FX_BOOL bCharArray = (dwFieldFlags >> 24) & 1;

		FX_FLOAT fFontSize = SetFontSize(pEdit);

		if (nMaxLen > 0)
		{
			if (bCharArray)
			{
				pEdit->SetCharArray(nMaxLen);

				if (IsFloatZero(fFontSize))
				{
					fFontSize = CPWL_Edit::GetCharArrayAutoFontSize(fontmap->GetPDFFont(0),rcClient,nMaxLen);
				}
			}
			else
			{
				if (sValue)
					nMaxLen = FXSYS_wcslen((const wchar_t*)sValue); 
				pEdit->SetLimitChar(nMaxLen);
			}
		}

		if (IsFloatZero(fFontSize))
			pEdit->SetAutoFontSize(TRUE);
		else
			pEdit->SetFontSize(fFontSize);

		pEdit->Initialize();

		if (sValue)
			pEdit->SetText(sValue);
		else
			pEdit->SetText((FX_LPCWSTR)pField->GetValue());

		CPDF_Rect rcContent = pEdit->GetContentRect();

		CFX_ByteString sEdit = CPWL_Utils::GetEditAppStream(pEdit,CPDF_Point(0.0f,0.0f),
			NULL,!bCharArray,subWord);

		if (sEdit.GetLength() > 0)
		{
			sBody << "/Tx BMC\n" << "q\n";
			if (rcContent.Width() > rcClient.Width() ||
				rcContent.Height() > rcClient.Height())
			{
				sBody << rcClient.left << " " << rcClient.bottom << " " 
					<< rcClient.Width() << " " << rcClient.Height() << " re\nW\nn\n";
			}
			CPWL_Color crText = GetTextPWLColor();	
			if (!m_pControl->GetWidget()->KeyExist("MK"))
			{
				CPDF_Matrix mtText = GetTextPWLMatrix();
				sBody << "BT\n" << CPWL_Utils::GetColorAppStream(crText) << " " << CPWL_Utils::GetTextMatrixAppStream(mtText) << " " << sEdit << "ET\n" << "Q\nEMC\n";
			}
			else
				sBody << "BT\n" << CPWL_Utils::GetColorAppStream(crText) << sEdit << "ET\n" << "Q\nEMC\n";
		}

		if (bCharArray)
		{
			switch (border.style)
			{
			case FSAnnot::e_borderStyleSolid:
				{
					CFX_ByteString sColor = CPWL_Utils::GetColorAppStream(GetBorderPWLColor(),FALSE);
					if (sColor.GetLength() > 0)
					{
						sLines << "q\n" << fBorderWidth << " w\n" 
							<< CPWL_Utils::GetColorAppStream(GetBorderPWLColor(),FALSE) << " 2 J 0 j\n";					

						for (FX_INT32 i=1;i<nMaxLen;i++)
						{
							sLines << rcClient.left + ((rcClient.right - rcClient.left)/nMaxLen)*i << " "
								<< rcClient.bottom << " m\n"
								<< rcClient.left + ((rcClient.right - rcClient.left)/nMaxLen)*i << " "
								<< rcClient.top << " l S\n";						
						}

						sLines << "Q\n";		
					}
				}
				break;
			case FSAnnot::e_borderStyleDashed:
				{
					CFX_ByteString sColor = CPWL_Utils::GetColorAppStream(GetBorderPWLColor(),FALSE);
					if (sColor.GetLength() > 0)
					{
						CPWL_Dash dsBorder = CPWL_Dash(3, 3, 0);

						sLines << "q\n" << fBorderWidth << " w\n" 
							<< CPWL_Utils::GetColorAppStream(GetBorderPWLColor(),FALSE)
							<< "[" << dsBorder.nDash << " " 
							<< dsBorder.nGap << "] " 
							<< dsBorder.nPhase << " d\n";

						for (FX_INT32 i=1;i<nMaxLen;i++)					
						{
							sLines << rcClient.left + ((rcClient.right - rcClient.left)/nMaxLen)*i << " "
								<< rcClient.bottom << " m\n"
								<< rcClient.left + ((rcClient.right - rcClient.left)/nMaxLen)*i << " "
								<< rcClient.top << " l S\n";	
						}

						sLines << "Q\n";
					}
				}
				break;
			}
		}

		IFX_Edit::DelEdit(pEdit);
	}

	CFX_ByteString sAP = GetBackgroundAppStream() + GetBorderAppStream() + sLines.GetByteString() + sBody.GetByteString();
	WriteAppearance("N", GetRotatedRect(), GetMatrix(), sAP,"");

	return FSCRT_ERRCODE_SUCCESS;
}

CFX_ByteString CFSCRT_STPDFResetApperance::GetBackgroundAppStream() 
{
	CPWL_Color crBackground = GetFillPWLColor();
	if (crBackground.nColorType != COLORTYPE_TRANSPARENT)
	{
		CPDF_Rect rcWindow = GetRotatedRect();
		return CPWL_Utils::GetRectFillAppStream(rcWindow, crBackground);
	}
	else
		return "";
}

CFX_ByteString CFSCRT_STPDFResetApperance::GetBorderAppStream() 
{
	FXSYS_assert(m_pControl !=NULL);
	CPDF_Rect rcWindow = GetRotatedRect();

	CPWL_Color crBorder = GetBorderPWLColor();
	CPWL_Color crBackground = GetFillPWLColor();
	CPWL_Color crLeftTop, crRightBottom;

	FSBorderInfo border;
	FS_RESULT ret = WidgetGetBorder(&border);
	if(ret != FSCRT_ERRCODE_SUCCESS) return ret;

	FX_FLOAT fBorderWidth = border.width;

	FX_INT32 nBorderStyle = 0;
	CPWL_Dash dsBorder(3,0,0);

	switch (border.style)
	{
	case FSAnnot::e_borderStyleDashed:
		nBorderStyle = PBS_DASH;
		dsBorder = CPWL_Dash(3, 3, 0);
		break;
	case FSAnnot::e_borderStyleBeveled:
		nBorderStyle = PBS_BEVELED;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY, 1);
		crRightBottom = CPWL_Utils::DevideColor(crBackground, 2);
		break;
	case FSAnnot::e_borderStyleInset:
		nBorderStyle = PBS_INSET;
		fBorderWidth *= 2;
		crLeftTop = CPWL_Color(COLORTYPE_GRAY, 0.5);
		crRightBottom = CPWL_Color(COLORTYPE_GRAY, 0.75);
		break;
	case FSAnnot::e_borderStyleUnderLine:
		nBorderStyle = PBS_UNDERLINED;
		break;
	default: 
		nBorderStyle = PBS_SOLID;
		break;
	}

	return CPWL_Utils::GetBorderAppStream(rcWindow, fBorderWidth, crBorder, crLeftTop, 
		crRightBottom, nBorderStyle, dsBorder);
}

CPWL_Color CFSCRT_STPDFResetApperance::GetBorderPWLColor() const
{

	FXSYS_assert(m_pControl !=NULL);
	CPWL_Color crBorder;

	CPDF_FormControl* pFormCtrl = m_pControl;
	ASSERT(pFormCtrl != NULL);

	FX_INT32 iColorType;
	FX_FLOAT fc[4];
	pFormCtrl->GetOriginalBorderColor(iColorType, fc);
	if (iColorType > 0)
		crBorder = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	return crBorder;
}

CPWL_Color CFSCRT_STPDFResetApperance::GetFillPWLColor() const
{
	FXSYS_assert(m_pControl !=NULL);

	CPWL_Color crFill;

	CPDF_FormControl* pFormCtrl = m_pControl;
	ASSERT(pFormCtrl != NULL);

	FX_INT32 iColorType;
	FX_FLOAT fc[4];
	pFormCtrl->GetOriginalBackgroundColor(iColorType, fc);
	if (iColorType > 0)
		crFill = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);

	return crFill;
}


CPWL_Color CFSCRT_STPDFResetApperance::GetTextPWLColor()
{
	FXSYS_assert(m_pControl !=NULL);

	CPWL_Color crText = CPWL_Color(COLORTYPE_GRAY, 0);

	CPDF_DefaultAppearance da = m_pControl->GetDefaultAppearance();
	if (da.HasColor(FALSE))
	{
		FS_INT32 iColorType = 0;// as output parameter
		FX_FLOAT fc[4];
		da.GetColor(iColorType, fc, FALSE);
		crText = CPWL_Color(iColorType, fc[0], fc[1], fc[2], fc[3]);
	}

	return crText;
}

CPDF_Matrix CFSCRT_STPDFResetApperance::GetTextPWLMatrix()
{
	FXSYS_assert(m_pControl !=NULL);

	CPDF_Matrix mtText = CPDF_Matrix(1, 0, 0, 1, 0, 0);;

	CPDF_DefaultAppearance da = m_pControl->GetDefaultAppearance();
	if (da.HasTextMatrix())
	{
		mtText = da.GetTextMatrix();
	}

	return mtText;
}

CPDF_Matrix CFSCRT_STPDFResetApperance::GetMatrix()
{
	CPDF_Matrix mt;
	FXSYS_assert(m_pControl !=NULL);

	CPDF_Rect rcAnnot = m_pControl->GetRect();
	rcAnnot.Normalize();
	FX_FLOAT fWidth = rcAnnot.right - rcAnnot.left;
	FX_FLOAT fHeight = rcAnnot.top - rcAnnot.bottom;
	FX_FLOAT fLeft = rcAnnot.left;
	FX_FLOAT fBottom = rcAnnot.bottom;

	switch (abs(m_pControl->GetRotation() % 360))
	{
	case 0:
	default:
		mt = CPDF_Matrix(1, 0, 0, 1, 0, 0);
		break;
	case 90:
		mt = CPDF_Matrix(0, 1, -1, 0, fWidth, 0);
		break;
	case 180:
		mt = CPDF_Matrix(-1, 0, 0, -1, fWidth, fHeight);
		break;
	case 270:
		mt = CPDF_Matrix(0, -1, 1, 0, 0, fHeight);
		break;
	}

	return mt;
}


void CFSCRT_STPDFResetApperance::SetAppState(const CFX_ByteString& str)
{
	FXSYS_assert(m_pControl !=NULL);
	FXSYS_assert(m_pControl->GetWidget() != NULL);

	if (str.IsEmpty())
		m_pControl->GetWidget()->RemoveAt("AS");
	else
		m_pControl->GetWidget()->SetAtString("AS", str);
}

CFX_ByteString CFSCRT_STPDFResetApperance::GetAppState()
{
	FXSYS_assert(m_pControl !=NULL);
	FXSYS_assert(m_pControl->GetWidget() != NULL);

	return m_pControl->GetWidget()->GetString("AS");
}


void CFSCRT_STPDFResetApperance::AddImageToAppearance(const CFX_ByteString& sAPType, CPDF_Stream* pImage)
{
	FXSYS_assert(pImage != NULL);
	FXSYS_assert(m_pControl !=NULL);
//	CPDFAnnot_Base* pAnnot = m_pLTControl->m_pAnnot;
//	FXSYS_assert(pAnnot != NULL);
//	FXSYS_assert(pAnnot->GetAnnotDict() != NULL);

	// 	CReader_Page* pPage = GetPage();
	// 	ASSERT(pPage != NULL);
	// 
	// 	CPDFSDK_Document* pDocument = pPage->GetDocument();
	// 	ASSERT(pDocument != NULL);

	CPDF_Document* pDoc = m_pControl->GetInterForm()->GetDocument();
	FXSYS_assert(pDoc !=NULL);

	CPDF_Dictionary* pAPDict = m_pControl->GetWidget()->GetDict("AP");
	FXSYS_assert(pAPDict != NULL);

	CPDF_Stream* pStream = pAPDict->GetStream(sAPType);
	FXSYS_assert(pStream != NULL);

	CPDF_Dictionary* pStreamDict = pStream->GetDict();
	FXSYS_assert(pStreamDict != NULL);

	CFX_ByteString sImageAlias = "IMG";

	if (CPDF_Dictionary* pImageDict = pImage->GetDict())
	{
		sImageAlias = pImageDict->GetString("Name");
		if (sImageAlias.IsEmpty())
			sImageAlias = "IMG";
	}	

	CPDF_Dictionary* pStreamResList = pStreamDict->GetDict("Resources");
	if (!pStreamResList)
	{
		pStreamResList = FX_NEW CPDF_Dictionary();
		pStreamDict->SetAt("Resources", pStreamResList);
	}

	if (pStreamResList) 
	{
		CPDF_Dictionary* pXObject = FX_NEW CPDF_Dictionary;			
		pXObject->SetAtReference(sImageAlias, pDoc, pImage);
		pStreamResList->SetAt("XObject", pXObject);
	}
}

void CFSCRT_STPDFResetApperance::RemoveAppearance(const CFX_ByteString& sAPType)
{
	FXSYS_assert(m_pControl !=NULL);
	//CPDFAnnot_Base* pAnnot = m_pLTControl->m_pAnnot;
	//FXSYS_assert(pAnnot != NULL);
	FXSYS_assert(m_pControl->GetWidget() != NULL);

	if (CPDF_Dictionary* pAPDict = m_pControl->GetWidget()->GetDict("AP"))
	{
		pAPDict->RemoveAt(sAPType);
	}
}



FS_RESULT CFSCRT_STPDFResetApperance::WidgetGetBorder(FSBorderInfo* border)
{
	FSAnnot* pTempAnnot = FX_NEW FSPDFAnnotImp(m_pControl->GetWidget(), NULL);
	if(!pTempAnnot) return FSCRT_ERRCODE_OUTOFMEMORY;
	*border = pTempAnnot->getBorderInfo();
	/*border->borderWidth = pAnnot->GetBorderThickness();


	if (pAnnot->GetBorderEffect() == FPDFANNOT_BORDEREFFECT_Cloudy)
	{
		border->borderStyle = FSPDF_ANNOT_BORDERSTYLE_CLOUDY;
		border->cloudIntensity = pAnnot->GetCloudyIntensity();
	}
	else
	{
		border->borderStyle = pAnnot->GetBorderStyle();
		border->cloudIntensity = -1;
	}

	FX_FLOAT fDashOffset = 0;
	CFX_FloatArray arrDash;
	pAnnot->GetBorderDash(fDashOffset, arrDash);
	FX_INT32 iDashCount = arrDash.GetSize();
	border->dashPhase = fDashOffset;
	border->dashCount = iDashCount;
	for (FX_INT32 i=0; i<iDashCount; i++)
	{
		border->dashPattern[i] = arrDash[i];
	}*/

	delete pTempAnnot;
	return FSCRT_ERRCODE_SUCCESS;
}

FX_FLOAT	CFSCRT_STPDFResetApperance::SetFontSize(IFX_Edit* pEdit, FX_BOOL isAutoFontSize)
{
	FXSYS_assert(m_pControl !=NULL);
	FX_FLOAT fFontSize;
	CFX_ByteString csFontNameTag;
	m_pControl->GetDefaultAppearance().GetFont(csFontNameTag, fFontSize);
	if (IsFloatZero(fFontSize))
		pEdit->SetFontSize(12.0f);
	else
		pEdit->SetFontSize(fFontSize);
	if(isAutoFontSize)
	{
		if (IsFloatZero(fFontSize))
			pEdit->SetAutoFontSize(TRUE);
		else
			pEdit->SetFontSize(fFontSize);
	}
	return fFontSize;
}

#endif  //defined(_FSDK_ALL_) || defined(_FSDK_PDFFORM_)
