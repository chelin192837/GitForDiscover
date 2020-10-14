#include "../fxcore.h"
#include "../fs_util.h"
#include "inc/fs_wminfo.h"
#include "inc/fs_wmtext.h"

extern FS_INT32 getPageRotation(CPDF_Dictionary* pFormDict);
////////////////////////////////////////////////////////////////////////
// Class CFSPDF_WatermarkInfo
CFSPDF_WatermarkInfo::CFSPDF_WatermarkInfo()
	: m_pFormObj(NULL)
{

}

CFSPDF_WatermarkInfo::~CFSPDF_WatermarkInfo()
{
	if (m_pFormObj)
	{
		m_pFormObj->m_pForm->m_pFormStream->Release();
		m_pFormObj->Release();
	}
}

FS_BOOL CFSPDF_WatermarkInfo::CreateWatermark(CPDF_Document *pPDFDoc, CFX_WideString wText, /*IFX_FontEx* pFontEx,*/ CPDF_Font* pPDFFont, FS_FLOAT fFontSize, FX_ARGB color, 
											  FS_BOOL bUnderline, FX_FLOAT linespace, FX_DWORD blockAlignment, FX_BOOL bSDKWatermark /*= FALSE*/)
{
	CreateBlankFormObj(pPDFDoc, bSDKWatermark);

	CFS_TextWatermark textWatermark;

	if (!(FSPDF_WATERMARK_TEXTALIGNMENT_LEFT == FX_TXTLINEALIGNMENT_Left 
		&& FSPDF_WATERMARK_TEXTALIGNMENT_RIGHT == FX_TXTLINEALIGNMENT_Right 
		&& FSPDF_WATERMARK_TEXTALIGNMENT_CENTER == FX_TXTLINEALIGNMENT_Center)
		)
		return FALSE;

	textWatermark.Init(fFontSize, color, bUnderline, linespace, blockAlignment);
	return textWatermark.FillText(m_pFormObj->m_pForm, wText, /*pFontEx, */pPDFFont);
}

//FS_BOOL CFSPDF_WatermarkInfo::CreateWatermark(CPDF_Document *pPDFDoc, CFX_DIBitmap *pBitmap)
//{
//	CreateBlankFormObj(pPDFDoc);
//	CFX_ImageWatermark imageWatermark;
//	return imageWatermark.FillDIBitmap(m_pFormObj->m_pForm, pBitmap);
//}
//
//FS_BOOL CFSPDF_WatermarkInfo::CreateWatermark(CPDF_Document *pPDFDoc, IFX_Image *pImage, FX_HIMAGE hImage)
//{
//	CreateBlankFormObj(pPDFDoc);
//	CFX_ImageWatermark imageWatermark;
//	return imageWatermark.FillImage(m_pFormObj->m_pForm, pImage, hImage);
//}
//
//FS_BOOL CFSPDF_WatermarkInfo::CreateWatermark(CPDF_Document *pPDFDoc, const CPDF_Page *pSrcPage, CPDF_ObjectArray* ObjectArray)
//{
//	CreateBlankFormObj(pPDFDoc, FALSE, TRUE);
//	CFX_PageWatermark pdfWatermark;
//	return pdfWatermark.FillPDFPage(m_pFormObj->m_pForm, pSrcPage, ObjectArray);
//}

FS_BOOL CFSPDF_WatermarkInfo::InsertIntoPage(const FSPDF_WATERMARK_SETTINGS* pSettings, CPDF_Page *pPage, CPDF_Dictionary* pAnnotDict)
{
	if (!m_pFormObj) return FALSE;

	if (!pPage->m_pResources)
	{
		pPage->m_pResources = CPDF_Dictionary::Create();
		pPage->m_pFormDict->SetAt(FX_BSTRC("Resources"), pPage->m_pResources);
	}

	if (pAnnotDict)
	{
		CPDF_Dictionary *pOCMDDict = CreateOCMD(pPage->m_pDocument, pSettings);
		pAnnotDict->SetAtReference(FX_BSTRC("OC"), pPage->m_pDocument, pOCMDDict);
		pAnnotDict->SetAtRect(FX_BSTRC("Rect"), CPDF_Rect(10, 10, 100, 100));
		CPDF_Dictionary *pAPDict = CreateAP(pPage->m_pDocument);
		pAnnotDict->SetAt(FX_BSTRC("AP"), pAPDict);

		pAnnotDict->SetAtInteger(FX_BSTRC("F"),4);

		CreateContentData(pPage, pSettings, m_pFormObj, pAnnotDict);
	}
	else
	{
		CPDF_FormObject *pFormObj = (CPDF_FormObject*)m_pFormObj->Clone();
		CPDF_Stream *pStream = pFormObj->m_pForm->m_pFormStream;
		CPDF_Dictionary *pDict = pStream->GetDict();
		if (!pDict->GetDict("OC"))  
			pDict->SetAtReference(FX_BSTRC("OC"), pPage->m_pDocument, CreateOCMD(pPage->m_pDocument, pSettings));

		CreateContentData(pPage, pSettings, pFormObj);

		if (pSettings->flags & FSPDF_WATERMARKFLAG_ONTOP)
			pPage->InsertObject(pPage->GetLastObjectPosition(), pFormObj);
		else
			pPage->InsertObject(NULL, pFormObj);
	} 

	if (!(pSettings->flags & FSPDF_WATERMARKFLAG_ASANNOT))
	{
		CPDF_ContentGenerator cg(pPage);

		cg.GenerateContent();
	}

	return TRUE;
}

void CFSPDF_WatermarkInfo::CreateBlankFormObj(CPDF_Document *pPDFDoc, FX_BOOL bSDKWaterMark /* = FALSE*/, FX_BOOL bTransparency/* = FALSE*/)
{
	CPDF_Dictionary *pStreamDict = CPDF_Dictionary::Create();
	pStreamDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
	pStreamDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
	pStreamDict->SetAtRect(FX_BSTRC("BBox"), CPDF_Rect(10, 10, 100, 100));

	CPDF_Dictionary *pACTDict = CPDF_Dictionary::Create();
	if (TRUE == bSDKWaterMark)
		pACTDict->SetAtName(FX_BSTRC("Private"), FX_BSTRC(FSDK_TRIALWATERMARK_RDK));
	else
		pACTDict->SetAtName(FX_BSTRC("Private"), FX_BSTRC("Watermark"));

	CPDF_Dictionary *pPieceInfoDict = CPDF_Dictionary::Create();
	pPieceInfoDict->SetAt(FX_BSTRC("ADBE_CompoundType"), pACTDict);
	pStreamDict->SetAt(FX_BSTRC("PieceInfo"), pPieceInfoDict);

	CPDF_Dictionary *pResDict = CPDF_Dictionary::Create();
	pStreamDict->SetAt(FX_BSTRC("Resources"), pResDict);

	if (bTransparency)
	{
		CPDF_Dictionary *pGroupDict = CPDF_Dictionary::Create();
		pStreamDict->SetAtReference(FX_BSTRC("Group"), pPDFDoc, pPDFDoc->AddIndirectObject(pGroupDict));
		pGroupDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("Group"));
		pGroupDict->SetAtName(FX_BSTRC("S"), FX_BSTRC("Transparency"));
		pGroupDict->SetAtBoolean(FX_BSTRC("K"), TRUE);
	}

	CPDF_Stream *pStream = FX_NEW CPDF_Stream(NULL, 0, pStreamDict);
	CPDF_Form *pForm = FX_NEW CPDF_Form(pPDFDoc, pResDict, pStream);
	m_pFormObj = FX_NEW CPDF_FormObject;
	m_pFormObj->m_pForm = pForm;
}

void CFSPDF_WatermarkInfo::UpdateSettingsXML(CFX_ByteString& xmlData)
{
	CPDF_Dictionary *pStreamDict = m_pFormObj ? (m_pFormObj->m_pForm ? m_pFormObj->m_pForm->m_pFormDict : NULL) : NULL;
	if(!pStreamDict) return;

	CPDF_Dictionary *pPieceInfoDict = pStreamDict->GetDict("PieceInfo");
	if(!pPieceInfoDict) return;

	CPDF_Dictionary *pACTDict = pPieceInfoDict->GetDict("ADBE_CompoundType");
	if(!pACTDict) return;

	CPDF_Dictionary* pDocSettingsDict = FX_NEW CPDF_Dictionary;
	if(!pDocSettingsDict) return;
	CPDF_Stream *pDocSettings = FX_NEW CPDF_Stream(NULL, 0, pDocSettingsDict);
	if(!pDocSettings) return;

	m_pFormObj->m_pForm->m_pDocument->AddIndirectObject(pDocSettings);
	pDocSettings->SetData(xmlData, xmlData.GetLength(), FALSE, FALSE);
	pACTDict->SetAtReference("DocSettings", m_pFormObj->m_pForm->m_pDocument, pDocSettings);

	CFSPDF_DateTime datetime;
	datetime.SetCurrentSysTime();
	pACTDict->SetAtString("LastModified", datetime.ToPDFDateTimeString());
}

CPDF_Dictionary* CFSPDF_WatermarkInfo::CreateOCMD(CPDF_Document *pPDFDoc, const FSPDF_WATERMARK_SETTINGS* pSettings)
{
	CFX_ByteStringC bsValue = !(pSettings->flags & FSPDF_WATERMARKFLAG_INVISIBLE) ? "ON" : "OFF";
	CPDF_Dictionary *pViewDict = CPDF_Dictionary::Create();
	pViewDict->SetAtName(FX_BSTRC("ViewState"), bsValue);

	bsValue = !(pSettings->flags & FSPDF_WATERMARKFLAG_NOPRINT) ? "ON" : "OFF";
	CPDF_Dictionary *pPrintDict = CPDF_Dictionary::Create();
	pPrintDict->SetAtName(FX_BSTRC("PrintState"), bsValue);

	CPDF_Dictionary *pExportDict = CPDF_Dictionary::Create();
	pExportDict->SetAtName(FX_BSTRC("ExportState"), FX_BSTRC("ON"));

	bsValue = pSettings->flags & FSPDF_WATERMARKFLAG_ONTOP ? "FG" : "BG";
	CPDF_Dictionary *pPageElementDict = CPDF_Dictionary::Create();
	pPageElementDict->SetAtName(FX_BSTRC("Subtype"), bsValue);

	CPDF_Dictionary *pUsageDict = CPDF_Dictionary::Create();
	pUsageDict->SetAt(FX_BSTRC("Export"), pExportDict);
	pUsageDict->SetAt(FX_BSTRC("PageElement"), pPageElementDict);
	pUsageDict->SetAt(FX_BSTRC("Print"), pPrintDict);
	pUsageDict->SetAt(FX_BSTRC("View"), pViewDict);

	CPDF_Dictionary *pOCGDict = CPDF_Dictionary::Create();
	pOCGDict->SetAtString(FX_BSTRC("Name"), FX_BSTRC("Watermark"));
	pOCGDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("OCG"));
	pOCGDict->SetAt(FX_BSTRC("Usage"), pUsageDict);
	pPDFDoc->AddIndirectObject(pOCGDict);

	CPDF_Dictionary *pOCMDDict = CPDF_Dictionary::Create();
	pOCMDDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("OCMD"));
	pOCMDDict->SetAtReference(FX_BSTRC("OCGs"), pPDFDoc, pOCGDict);
	pPDFDoc->AddIndirectObject(pOCMDDict);

	////////////////////////////////////////////////////
	//	if(pSettings->flags & FSPDF_WATERMARKFLAG_ASANNOT)
	{
		CPDF_Dictionary* pRootDict = pPDFDoc->GetRoot();

		CPDF_Dictionary *pOCProperties = pRootDict->GetDict(FX_BSTRC("OCProperties"));
		if(pOCProperties == NULL)
		{
			pOCProperties = CPDF_Dictionary::Create();
			pRootDict->SetAt(FX_BSTRC("OCProperties"),pOCProperties);
		}

		CPDF_Dictionary *pASDict = pOCProperties->GetDict(FX_BSTRC("D"));
		if(pASDict == NULL)
		{
			pASDict = CPDF_Dictionary::Create();
			pOCProperties->SetAt(FX_BSTRC("D"),pASDict);
		}

		CPDF_Array* pArrayCategory = pASDict->GetArray(FX_BSTRC("AS"));
		if(pArrayCategory == NULL)
		{
			pArrayCategory = CPDF_Array::Create();
			pASDict->SetAt(FX_BSTRC("AS"), pArrayCategory);
		}

		CPDF_Array* pArryOn = pASDict->GetArray(FX_BSTRC("ON"));
		if(pArryOn == NULL)
		{
			pArryOn = CPDF_Array::Create();

			pASDict->SetAt(FX_BSTRC("ON"), pArryOn);
		}

		CPDF_Array* pOCGs = pOCProperties->GetArray(FX_BSTRC("OCGs"));
		if(pOCGs == NULL)
		{
			pOCGs = CPDF_Array::Create();
			pOCProperties->SetAt(FX_BSTRC("OCGs"), pOCGs);

		}
		pArryOn->AddReference(pPDFDoc, pOCGDict);
		pOCGs->AddReference(pPDFDoc, pOCGDict);

		AddOCProperties(pPDFDoc, pOCGDict, FX_BSTRC("View"), pArrayCategory);

		AddOCProperties(pPDFDoc, pOCGDict, FX_BSTRC("Print"), pArrayCategory);
		AddOCProperties(pPDFDoc, pOCGDict, FX_BSTRC("Export"), pArrayCategory);
	}

	return pOCMDDict;
}

void CFSPDF_WatermarkInfo::AddOCProperties(CPDF_Document *pPDFDoc, CPDF_Dictionary *pOCGDict, const CFX_ByteString& name, CPDF_Array* pArrayCategory)
{
	CPDF_Dictionary *pCategoryDict = CPDF_Dictionary::Create();
	pArrayCategory->Add(pCategoryDict);
	CPDF_Array *pSubCategory = CPDF_Array::Create();
	pCategoryDict->SetAt(FX_BSTRC("Category"), pSubCategory);
	pSubCategory->AddName(name);
	pCategoryDict->SetAtName(FX_BSTRC("Event"), name);

	CPDF_Array *pTmpCategory = CPDF_Array::Create();

	pTmpCategory->AddReference(pPDFDoc, pOCGDict);
	pCategoryDict->SetAt(FX_BSTRC("OCGs"), pTmpCategory);
}

void CFSPDF_WatermarkInfo::CreateContentData(const CPDF_Page *pPage, const FSPDF_WATERMARK_SETTINGS* pSettings, CPDF_FormObject *pFormObj)
{
	CPDF_Dictionary *pCMDDict = CPDF_Dictionary::Create();
	pCMDDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("Pagination"));
	pCMDDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Watermark"));
	CPDF_ContentMarkData *pWMFormObjCMD = pFormObj->m_ContentMark.GetModify();
	pWMFormObjCMD->AddMark("Artifact", pCMDDict, TRUE);
	pCMDDict->Release();

	CPDF_GeneralStateData *pWMObjGS = pFormObj->m_GeneralState.GetModify();
	pWMObjGS->m_FillAlpha = pSettings->opacity / 100.0f;
	pWMObjGS->m_StrokeAlpha = pSettings->opacity / 100.0f;
	CPDF_Rect rtWatermark = pFormObj->m_pForm->m_pFormDict->GetRect(FX_BSTRC("BBox"));
	pFormObj->m_FormMatrix = GetMatrix(pPage, rtWatermark, pSettings);
	pFormObj->CalcBoundingBox();
}

CFX_AffineMatrix CFSPDF_WatermarkInfo::GetMatrix(const CPDF_Page *pPage, CPDF_Rect rtWatermark, const FSPDF_WATERMARK_SETTINGS* pSettings, FX_DWORD dwFlag /* = FX_WMALL */)
{
	CPDF_Rect rtPage = pPage->GetPageBBox();

	CFX_AffineMatrix mt;
	if (dwFlag & FX_WM_ROTATION)
		DoRotation(pPage, mt, rtPage, rtWatermark, pSettings->rotation);

	if (dwFlag & FX_WM_SCALE)
	{
		FX_FLOAT fScaleX = 1.0f;
		FX_FLOAT fScaleY = 1.0f;
		//	if (pSettings->scaleX < 0)
		//		fScale = FitPageBox(mt, rtPage, rtWatermark);
		//	fScale = FXSYS_fabs(fScale * pSettings->scaleX);

		if (pSettings->scaleX < 0 || pSettings->scaleY < 0 /*||pSettings->autoFit */) 
		{
			fScaleX = fScaleY = FitPageBox(mt, rtPage, rtWatermark);
		}
		else
		{
			fScaleX = pSettings->scaleX;
			fScaleY = pSettings->scaleY;
		}

		mt.Scale(fScaleX, fScaleY);
	}
	if (dwFlag & FX_WM_OFFSET)
		DoAlign(pPage, mt, rtPage, rtWatermark, pSettings);

	return mt;
}

void CFSPDF_WatermarkInfo::DoRotation(const CPDF_Page *pPage, CFX_AffineMatrix &mt, const CPDF_Rect &rtPageBox, CPDF_Rect rtWM, FX_FLOAT fRotation)
{
	FX_FLOAT fPageRotation = getPageRotation(pPage->m_pFormDict) * 90;

	mt.TransformRect(rtWM);

	FX_FLOAT fx = rtWM.Width() / 2.0f;
	FX_FLOAT fy = rtWM.Height() / 2.0f;
	CFX_AffineMatrix mtRotation(1, 0, 0, 1, fx, fy); 
	FX_FLOAT fAngle = (fPageRotation + fRotation) * FX_PI / 180.0f;

	mtRotation.Concat((FX_FLOAT)FXSYS_cos(fAngle), (FX_FLOAT)FXSYS_sin(fAngle), (FX_FLOAT)-sin(fAngle),
		(FX_FLOAT)FXSYS_cos(fAngle), -fx, -fy);
	mt.Concat(mtRotation);
}

FX_FLOAT CFSPDF_WatermarkInfo::FitPageBox(const CFX_AffineMatrix &mt, CPDF_Rect rtPageBox, CPDF_Rect rtWM)
{
	mt.TransformRect(rtWM);

	FX_FLOAT fVScale = rtPageBox.Height() / rtWM.Height();
	FX_FLOAT fHScale = rtPageBox.Width() / rtWM.Width();

	return fVScale < fHScale ? fVScale : fHScale;
}

void CFSPDF_WatermarkInfo::DoAlign(const CPDF_Page *pPage, CFX_AffineMatrix &mt, CPDF_Rect rtPageBox, CPDF_Rect rtWM, const FSPDF_WATERMARK_SETTINGS* pSettings)
{
	mt.TransformRect(rtWM);

	FX_DWORD dwHAlign = pSettings->position % 3;
	FX_DWORD dwVAlign = pSettings->position / 3;

	FX_FLOAT fXOffset = (dwHAlign == 2) ? -pSettings->offsetX : pSettings->offsetX;
	FX_FLOAT fYOffset = (dwVAlign == 0) ? -pSettings->offsetY : pSettings->offsetY;

	CFX_AffineMatrix mtOffset;

	FX_FLOAT fPageRotation = getPageRotation(pPage->m_pFormDict) * 90;
	FX_DWORD tempAlign = 0;
	FX_FLOAT fTmpOffset=0;
	switch ((FX_DWORD)fPageRotation)
	{
	case 90:
		tempAlign = dwHAlign;
		dwHAlign = dwVAlign;
		dwVAlign = 2 - tempAlign;
		//SEWQ-526
		/*when offset is set, the watermark is abnormal*/
		fTmpOffset = fXOffset;
		fXOffset = fYOffset;
		fYOffset = fTmpOffset;
		fXOffset = -fXOffset;

		break;
	case 180:
		dwHAlign = 2 - dwHAlign;
		dwVAlign = 2 - dwVAlign;

		/*when offset is set, the watermark is abnormal*/
		fXOffset = -fXOffset;
		fYOffset = -fYOffset;

		break;
	case 270:
		tempAlign = dwVAlign;
		dwVAlign = dwHAlign;
		dwHAlign = 2 - tempAlign;

		/*when offset is set, the watermark is abnormal*/
		fTmpOffset = fXOffset;
		fXOffset = fYOffset;
		fYOffset = fTmpOffset;
		fYOffset = -fYOffset;	
		break;
	default:
		break;
	}

	// horizontal
	switch (dwHAlign)
	{
	case 0: //left
		mtOffset.e = rtPageBox.left - rtWM.left + fXOffset;
		break;
	case 1: //center
		{
			FX_FLOAT fPageCenter = rtPageBox.left + rtPageBox.Width() / 2.0f;
			FX_FLOAT fWMCenter = rtWM.left + rtWM.Width() / 2.0f;
			mtOffset.e = fPageCenter - fWMCenter + fXOffset;
		}
		break;
	case 2: //right
		mtOffset.e = rtPageBox.right - rtWM.right + fXOffset;
		break;
	}

	//vertical
	switch (dwVAlign)
	{
	case 0: //top
		mtOffset.f = rtPageBox.top - rtWM.top + fYOffset;
		break;
	case 1: //center
		{
			FX_FLOAT fPageCenter = rtPageBox.bottom + rtPageBox.Height() / 2;
			FX_FLOAT fWMCenter = rtWM.bottom + rtWM.Height() / 2;
			mtOffset.f = fPageCenter - fWMCenter + fYOffset;
		}
		break;
	case 2: //bottom
		mtOffset.f = rtPageBox.bottom - rtWM.bottom + fYOffset;
		break;
	}
	mt.Concat(mtOffset);
}

CPDF_Dictionary* CFSPDF_WatermarkInfo::CreateAP(CPDF_Document *pPDFDoc)
{
	CPDF_Dictionary *pNDict = CPDF_Dictionary::Create();
	pNDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
	pNDict->SetAtRect(FX_BSTRC("BBox"), CPDF_Rect(10, 10, 100, 100));
	pNDict->SetAtMatrix(FX_BSTRC("Matrix"), CPDF_Matrix(1, 0, 0, 1, 0, 0));

	CPDF_Stream *pStream = FX_NEW CPDF_Stream(NULL, 0, pNDict);
	pPDFDoc->AddIndirectObject(pStream);
	CPDF_Dictionary *pAPDict = CPDF_Dictionary::Create();
	pAPDict->SetAtReference(FX_BSTRC("N"), pPDFDoc, pStream);
	return pAPDict;
}

void CFSPDF_WatermarkInfo::CreateContentData(const CPDF_Page *pPage, const FSPDF_WATERMARK_SETTINGS* pSettings, CPDF_FormObject *pFormObj, CPDF_Dictionary *pAnnotDict)
{
	CPDF_Document *pPDFDoc = pPage->m_pDocument;
	CPDF_Rect rtPage = pPage->GetPageBBox();
	CPDF_Rect rtWatermark = pFormObj->m_pForm->m_pFormDict->GetRect(FX_BSTRC("BBox"));

	CPDF_Matrix mt = GetMatrix(pPage, rtWatermark, pSettings, FX_WM_MASKNOOFFSET);
	pFormObj->m_FormMatrix = mt;
	mt.TransformRect(rtWatermark);

	CPDF_Dictionary *pSObjectDict = CPDF_Dictionary::Create();
	pFormObj->m_pForm->m_pFormDict->SetAt(FX_BSTRC("Group"), pSObjectDict);
	pSObjectDict->SetAtName(FX_BSTRC("S"), FX_BSTRC("Transparency"));

	CPDF_Dictionary *pAPDict = pAnnotDict->GetDict(FX_BSTRC("AP"));
	CPDF_Stream *pAPStream = pAPDict->GetStream(FX_BSTRC("N"));
	CPDF_Dictionary *pNDict = pAPStream->GetDict();
	pNDict->SetAtRect(FX_BSTRC("BBox"), rtWatermark);

	CPDF_Dictionary *pResDict = CPDF_Dictionary::Create();
	CPDF_Dictionary *pXObjectDict = CPDF_Dictionary::Create();
	pResDict->SetAt(FX_BSTRC("XObject"), pXObjectDict);
	pPDFDoc->AddIndirectObject(pFormObj->m_pForm->m_pFormStream);
	pXObjectDict->SetAtReference(FX_BSTRC("Fm0"), pPDFDoc, pFormObj->m_pForm->m_pFormStream);	

	// ExtGState
	CPDF_Dictionary *pExtGStateDict = CPDF_Dictionary::Create();
	pExtGStateDict->SetAtName(FX_BSTRC("Type"), "ExtGState");
	pExtGStateDict->SetAtNumber(FX_BSTRC("CA"), pSettings->opacity / 100.0f);
	pExtGStateDict->SetAtNumber(FX_BSTRC("ca"), pSettings->opacity / 100.0f);
	pPDFDoc->AddIndirectObject(pExtGStateDict);

	CPDF_Dictionary *pResGSDict = CPDF_Dictionary::Create();
	pResDict->SetAt(FX_BSTRC("ExtGState"), pResGSDict);
	pResGSDict->SetAtReference(FX_BSTRC("GS0"), pPDFDoc, pExtGStateDict);
	pNDict->SetAt(FX_BSTRC("Resources"), pResDict);

	//CFX_ByteString bclipStream;

	//bclipStream.Format("\r\n %f %f %f %f re W n\r\n",
	//	rtWatermark.left, rtWatermark.top, rtWatermark.right, rtWatermark.bottom);


	CPDF_Rect rtBBox;
	rtBBox.left = FX_MIN(rtWatermark.left, rtWatermark.right);
	rtBBox.right = FX_MAX(rtWatermark.left, rtWatermark.right);
	rtBBox.bottom = FX_MIN(rtWatermark.top, rtWatermark.bottom);
	rtBBox.top = FX_MAX(rtWatermark.top, rtWatermark.bottom);

	CPDF_Matrix mt2 = GetMatrix(pPage, rtBBox, pSettings, FX_WM_OFFSET);
	mt2.TransformRect(rtBBox);


#if 0
	CPDF_Rect temp = rtPage;
	temp.Intersect(rtBBox);
	CFX_ByteString bsStream;
	bsStream.Format("q\r\n %f %f %f %f re W* n\r\n\r\n %f %f %f %f %f %f cm \r\n/GS0 gs\r\n/Fm0 Do\r\nQ",
		temp.left, temp.bottom, temp.right, temp.top,
		mt.a, mt.b, mt.c, mt.d, mt.e, mt.f);
	pAPStream->SetData((FX_LPCBYTE)bsStream.GetBuffer(bsStream.GetLength()), bsStream.GetLength(), FALSE, FALSE);
	bsStream.ReleaseBuffer();
#else
	CFX_ByteString bsStream;
	bsStream.Format("q\r\n %f %f %f %f %f %f cm \r\n/GS0 gs\r\n/Fm0 Do\r\nQ",
		mt.a, mt.b, mt.c, mt.d, mt.e, mt.f);
	pAPStream->SetData((FX_LPCBYTE)bsStream.GetBuffer(bsStream.GetLength()), bsStream.GetLength(), FALSE, FALSE);
	bsStream.ReleaseBuffer();
#endif

	//rtBBox.Intersect(rtPage);
	pAnnotDict->SetAtRect(FX_BSTRC("Rect"), rtBBox);
	//	pAnnot->m_pAnnotDict->SetAtRect(FX_BSTRC("CropBox"), rtPage);
}

FS_BOOL CFSPDF_WatermarkInfo::GetWaterMarkSize(FS_FLOAT* width, FS_FLOAT* hight)
{
	if(m_pFormObj && m_pFormObj->m_pForm && m_pFormObj->m_pForm->m_pFormDict)
	{
		CPDF_Rect rtWatermark =	m_pFormObj->m_pForm->m_pFormDict->GetRect(FX_BSTRC("BBox"));
		*width = rtWatermark.Width();
		*hight = rtWatermark.Height();
		return TRUE;
	}
	return FALSE;
}