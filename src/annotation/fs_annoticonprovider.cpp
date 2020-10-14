#include "../../include/fs_annot.h"
#include "../fs_util.h"
#include "../fs_iconprovider.h"
#include "fs_annot_imp.h"
#include "fs_annot_util.h"
#include "fs_annoticonprovider.h"
#include "../inc/fs_pdfpageImp.h"
#include "../form/fs_pdfformImp.h"
#include "../form/fpdf_stinterform.h"

void FPDFUtils_GetRGBColorComponents(FS_ARGB argb, FX_FLOAT &a, FX_FLOAT &r, FX_FLOAT &g, FX_FLOAT &b)
{
	a = (FX_FLOAT)FXARGB_A(argb) / 255.0f;
	r = (FX_FLOAT)FXARGB_R(argb) / 255.0f;
	g = (FX_FLOAT)FXARGB_G(argb) / 255.0f;
	b = (FX_FLOAT)FXARGB_B(argb) / 255.0f;
}

CPDF_Array*	FPDFUtils_CreateRGBColorArray(FX_ARGB argb, FX_BOOL bSetAlpha = FALSE)
{
	FX_FLOAT a, r, g, b;
	FPDFUtils_GetRGBColorComponents(argb, a, r, g, b);
	CPDF_Array *pColor = CPDF_Array::Create();
	//	if (FXARGB_A(argb))
	{
		if(bSetAlpha)
			pColor->AddNumber(a);
		pColor->AddNumber(r);
		pColor->AddNumber(g);
		pColor->AddNumber(b);
	}
	return pColor;
}

void FPDFUtils_SetRGBColorArray(CPDF_Dictionary* pDict, FX_BSTR key, FS_ARGB argb, FX_BOOL bSetAlpha = FALSE)
{
	if (NULL == pDict) return ;
	pDict->SetAt(key, FPDFUtils_CreateRGBColorArray(argb, bSetAlpha));
}

///////////////////////////////////////////////////////
// Class CFSPDF_AnnotIconProvider
CFSPDF_AnnotIconProvider::CFSPDF_AnnotIconProvider(CPDF_Document* pPDFDoc):
m_pPDFDoc(pPDFDoc)
{

}

CFSPDF_AnnotIconProvider::~CFSPDF_AnnotIconProvider()
{
	m_pPDFDoc = NULL;
}

FS_BOOL CFSPDF_AnnotIconProvider::generateAPStream(FSPDFMarkupImp* pMarkupImp)
{
	if (NULL == m_pPDFDoc || NULL == pMarkupImp)
		return FALSE;
	
	FSM_ANNOTTYPE annotType = pMarkupImp->getType();
	CFX_ByteString bsIconName;
	if (isSupportIconProvider(annotType))
	{
		bsIconName = pMarkupImp->getAnnotImp().getStringImpl(FSANNOT_NAMEKEY_ICONNAME);
		// If icon name is an empty string, we should treat: (according to PDF reference)
		// note annotation as "Note"
		// stamp annotation as "Draft"
		// file attachment as "PushPin"
		if (bsIconName.IsEmpty())
		{
			if (FSAnnot::e_annotNote == annotType)
				bsIconName = "Note";
			else if (FSAnnot::e_annotStamp == annotType)
				bsIconName = "Draft";
			else if (FSAnnot::e_annotFileAttachment == annotType)
				bsIconName = "PushPin";
		}

	}
	else
		return FALSE;
	FSPDFPage* pIconPage = NULL;

	FX_ARGB fillColor = 0xFF000000;
	FS_ARGB strokeColor = 0;
	if (pMarkupImp->getAnnotImp().getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, fillColor));
		strokeColor = fillColor^0x00A0A0A0;

	FSRectF fsAnnotRect = pMarkupImp->getAnnotImp().getRotateRect(); 
	CFX_FloatRect annotRect(fsAnnotRect.left, fsAnnotRect.bottom, fsAnnotRect.right, fsAnnotRect.top);
	CFX_ByteString bsAPName = getAPNameString(annotType, bsIconName, fillColor, strokeColor);
	CPDF_Stream* pStampStream = findExistingAPStream(bsAPName);
//comment and revert the fix(SDK-5956) because it has side effect to efficiency.
//	CFX_FloatRect apBBox;
//	if (pAPStream)
//		apBBox = pAPStream->GetDict()->GetRect(FX_BSTRC("BBox"));
//	FS_BOOL bRectChanged = (FALSE == apBBox.IsEmpty() && FALSE == FSCRT_Util_IsFloatRectEqual(annotRect, apBBox));
//
//	if (!pAPStream || TRUE == bRectChanged)
//	{// Re-generate ap stream if no existed AP stream or Annot's /Rect is not equal to BBox of ap stream.
    if (!pStampStream) {
		FX_BOOL bHasCustomerIcon = FALSE;
		CFSPDF_IconProviderManager* pIconProviderMgr = CFS_Library::getIconProviderMgr();
		if (pIconProviderMgr)
		{
			FX_LPSTR sIconName = bsIconName.GetBuffer(bsIconName.GetLength());
			FSAnnotIconProvider* pAnnotIconProvider = pIconProviderMgr->GetAnnotIconProvider();
			if (pAnnotIconProvider && pAnnotIconProvider->hasIcon(annotType, sIconName))
			{
				pIconPage = pAnnotIconProvider->getIcon(annotType, sIconName, fillColor);
				if(pIconPage && ((FSPDFPageImp*)pIconPage)->getPage())
				{
					//clone page contents ap. this note added by bing_chen.
					pStampStream = createAPFormFromPage(((FSPDFPageImp*)pIconPage)->getPage());
					CFX_PtrArray shadings;
					CFX_PtrArray functions;
					GetShadingFunctions(pStampStream, shadings, functions);
					FS_ARGB c0, c1;
					FS_INT32 nCount = functions.GetSize(), n;
					for (n = 0; n < nCount; n ++)
					{
						CPDF_Dictionary* pDict = (CPDF_Dictionary*)functions[n];
						if (NULL == pDict)
							continue;
						c0 = c1 = fillColor;
						FSShadingColor shadingColors;
						FS_BOOL bRet = pAnnotIconProvider->getShadingColor(annotType, sIconName, fillColor, n, shadingColors);
						if (bRet && pAnnotIconProvider->canChangeColor(annotType, sIconName))
						{
							c0 = shadingColors.firstColor;
							c1 = shadingColors.secondColor;
							FS_BOOL bSetAlpha = FALSE;
							if(CPDF_Array* pArray = pDict->GetArray(FX_BSTRC("C0")))
							{
								if(pArray->GetCount()==4)
									bSetAlpha = TRUE;
							}
							if(bSetAlpha)
							{
								FPDFUtils_SetRGBColorArray(pDict, FX_BSTRC("C0"), c0, TRUE);
								FPDFUtils_SetRGBColorArray(pDict, FX_BSTRC("C1"), c1, TRUE);
							}
							else
							{
								c0 |= 0xFF000000, c1 |= 0xFF000000;
								FPDFUtils_SetRGBColorArray(pDict, FX_BSTRC("C0"), c0);
								FPDFUtils_SetRGBColorArray(pDict, FX_BSTRC("C1"), c1);
							}
						}
					}
					nCount = shadings.GetSize();
					for (n = 0; n < nCount; n ++)
					{
						CPDF_Dictionary* pShading = (CPDF_Dictionary*)shadings[n];
						CPDF_Pattern* pPattern = m_pPDFDoc->LoadPattern(pShading, TRUE);
						if (!pPattern || pPattern->m_PatternType != PATTERN_SHADING) continue;
						((CPDF_ShadingPattern*)pPattern)->Reload();
					}
					bHasCustomerIcon = TRUE;
				}
			}
			bsIconName.ReleaseBuffer();
			sIconName = NULL;
			if (FALSE == bHasCustomerIcon) 
			{//no customer icon can be set to annotation.
				if (annotType == FSAnnot::e_annotStamp)
				{//If current annot is stamp, return FALSE. -- Not support standard stamp AP.
					return FALSE;
				}
			}

			if (!pStampStream/* || (bRectChanged && !pAnnotIconProvider)*/)
			{
				pStampStream = createStdFormStream(annotRect, annotType, bsIconName, fillColor, strokeColor);
				if (!pStampStream) return FALSE;
			}
			CPDF_NameTree nt(m_pPDFDoc, FX_BSTRC("AP"));
			CPDF_Reference* pRef = CPDF_Reference::Create(m_pPDFDoc, pStampStream->GetObjNum());
			nt.SetValue(m_pPDFDoc, FX_BSTRC("AP"), bsAPName, pRef);
		}
	}
	CPDF_Stream* pDynamicAPStream = NULL;
	if (FSAnnot::e_annotStamp == annotType)
	{// For stamp annotation, try to generate dynamic ap stream if there's any.
		// Dynamic stream cannot be cached because it should always be changed at least by time.
		if (!pIconPage)
		{
			CFSPDF_IconProviderManager* pIconProviderMgr = CFS_Library::getIconProviderMgr();
			if (pIconProviderMgr)
			{
				FX_LPSTR sIconName = bsIconName.GetBuffer(bsIconName.GetLength());
				FSAnnotIconProvider* pAnnotIconProvider = pIconProviderMgr->GetAnnotIconProvider();
				if (pAnnotIconProvider && pAnnotIconProvider->hasIcon(annotType, sIconName))
				{
					pIconPage = pAnnotIconProvider->getIcon(annotType, sIconName, fillColor);
					bsIconName.ReleaseBuffer();
				}
			}
		}
		pDynamicAPStream = generateDynamicStampAPStream(pIconPage, annotRect);
	}

	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	// For stamp annotation, use 1 as opacity for current created N AP, because stamp's AP structure will be changed by FSPDFStampImp::updateStampAP() then and the opacity will be use there.
	if (FSAnnot::e_annotStamp == annotType)
		param.fOpacity = 1;
	else
		param.fOpacity = pMarkupImp->getOpacity();	
	param.bUseOpacity = TRUE; // This means to generate /ExtGState in the new N AP stream.

	CFX_ByteString bsExtGStateName = "TransGs"; //same as that one used in writeAppearance(). 
	CFX_ByteString bsStampXObjectName = "FRDK";
	CFX_ByteString bsStream_1;
	bsStream_1.Format("/%s gs\n/%s Do\n", (FX_LPCSTR)bsExtGStateName, (FX_LPCSTR)bsStampXObjectName);

	CFX_ByteString bsStream_2;
	CFX_ByteString bsDynamicXObjectName = "FFT0";
	if (pDynamicAPStream)
		bsStream_2.Format("/%s Do\n", (FX_LPCSTR)bsDynamicXObjectName);

	// AP stream data
	CFX_ByteTextBuf bufContent;
	bufContent << (FX_LPCSTR)bsStream_1 << (FX_LPCSTR)bsStream_2;

	FS_INT32 iPageIndex = pMarkupImp->getPage()->getIndex();
	CFX_Matrix mtAnnot;
	FSRectF fsAnnotOrgRect = pMarkupImp->getRect();
	CPDF_Rect annotOrgRect(fsAnnotOrgRect.left, fsAnnotOrgRect.bottom, fsAnnotOrgRect.right, fsAnnotOrgRect.top);
	FSAnnot_Util_GetRotateMatrix(m_pPDFDoc->GetPage(iPageIndex), annotOrgRect, mtAnnot);
	// Get /BBox of /AP dictionary again since it may have been updated.
	CFX_FloatRect apBBox = pStampStream->GetDict()->GetRect(FX_BSTRC("BBox"));
	// Remove old /N in /AP and then update the new /N by writeAppearance()
	CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pMarkupImp->getDict();
	CPDF_Dictionary* pAPDict = pAnnotDict->GetDict("AP");
	if (pAPDict)
		pAPDict->RemoveAt("N");
	CPDF_Stream* pNAPStream = pMarkupImp->getAnnotImp().writeAppearance(FX_BSTRC("N"), apBBox, mtAnnot, bufContent, FX_BSTRC(""), &param);
	if (!pNAPStream) return FALSE;
	// Set AP stream with "FRDK" to the new /N ap stream; for stamp, also set dynamic ap stream if there's any.
	CPDF_Dictionary* pStreamDict = pNAPStream->GetDict();	
	CPDF_Dictionary* pAPRes = pStreamDict->GetDict(FX_BSTRC("Resources"));
	if (!pAPRes)
	{
		pAPRes = CPDF_Dictionary::Create();
		pStreamDict->SetAt(FX_BSTRC("Resources"), pAPRes);
	}
	CPDF_Dictionary* pXObject = pAPRes->GetDict(FX_BSTRC("XObject"));
	if (!pXObject)
	{
		pXObject = CPDF_Dictionary::Create();
		pAPRes->SetAt(FX_BSTRC("XObject"), pXObject);
	}
	pXObject->SetAtReference(bsStampXObjectName, m_pPDFDoc, pStampStream);
	if (pDynamicAPStream)
		pXObject->SetAtReference(bsDynamicXObjectName, m_pPDFDoc, pDynamicAPStream);

	return TRUE;
}

CPDF_Stream* CFSPDF_AnnotIconProvider::generateDynamicStampAPStream(FSPDFPage* pIconPage, CFX_FloatRect stampAnnotRect)
{
	if (!pIconPage) return NULL;
	FSFormImp* pForm = (FSFormImp*)((FSPDFDocImp*)pIconPage->getDocument())->getForm();
	if (!pForm ) return NULL;
	CFSCRT_STPDFInterForm* pSTInterForm = pForm->ST_GetSTInterForm();
	if (!pSTInterForm) return NULL;
	//run javascript for dynamic stamps function. see jira SDK-146. added by bing_chen.
	pSTInterForm->OnCalculate();

	CPDF_Dictionary* pPageDict = (CPDF_Dictionary*)pIconPage->getDict();
	CPDF_Array* pAnnots = pPageDict->GetArray("Annots");
	if (!pAnnots) return NULL;

	CPDF_Stream* pDynamicAPStream = NULL;
	CPDF_Dictionary* pXObjectTemp = NULL;
	FS_INT32 iClonedAnnot = -1;
	FX_DWORD dwSize = pAnnots->GetCount();
	for (FX_DWORD i = 0; i < dwSize; i++)
	{
		CPDF_Object* pObj = pAnnots->GetElementValue(i);
		if (!pObj && pObj->GetType() != PDFOBJ_DICTIONARY)
			continue;

		CPDF_Dictionary* pAnnotDic = (CPDF_Dictionary*)pObj;
		CFX_ByteString sSubtype = pAnnotDic->GetString("Subtype");
		if (!sSubtype.Equal("Widget")) continue;

		CPDF_Dictionary* pAnnotAP = pAnnotDic->GetDict(FX_BSTRC("AP"));
		if (!pAnnotAP) continue;

		// Get the AP stream or subdirectory
		CPDF_Object* pSubObj = pAnnotAP->GetElementValue(FX_BSTRC("N"));
		if (NULL == pSubObj) continue;

		CPDF_Stream* pNAPStream = NULL;
		if (pSubObj->GetType() == PDFOBJ_STREAM) {
			// The only one stream, use it
			pNAPStream = (CPDF_Stream*)pSubObj;
		} else if (pSubObj->GetType() == PDFOBJ_DICTIONARY) {
			// Need to check the annotation status
			CFX_ByteString as = pAnnotDic->GetString("AS");
			if (as.IsEmpty()) {
				// Sometimes a buggy PDF doesn't have AS for form widget, we need to use field value to determine which one is active
				// TESTDOC: Bug #7999 - web.pdf
				CFX_ByteString value = pAnnotDic->GetString(FX_BSTRC("V"));
				if (value.IsEmpty())
					value = pAnnotDic->GetDict(FX_BSTRC("Parent"))->GetString(FX_BSTRC("V"));
				if (value.IsEmpty() || !((CPDF_Dictionary*)pSubObj)->KeyExist(value))
					as = FX_BSTRC("Off");
				else
					as = value;
			}
			pNAPStream = ((CPDF_Dictionary*)pSubObj)->GetStream(as);
		}
		if (NULL == pNAPStream) continue;

		CPDF_Dictionary* pNAPDic = pNAPStream->GetDict();
		CFX_AffineMatrix matrix = pNAPDic->GetMatrix(FX_BSTRC("Matrix"));
		CPDF_Rect rcAnnot = pAnnotDic->GetRect(FX_BSTRC("Rect"));
		rcAnnot.Normalize();

		CPDF_Rect rectNewStream;
		if (pNAPDic->KeyExist(FX_BSTRC("Rect")))
			rectNewStream = pNAPDic->GetRect(FX_BSTRC("Rect"));
		else if (pNAPDic->KeyExist(FX_BSTRC("BBox")))
			rectNewStream = pNAPDic->GetRect(FX_BSTRC("BBox"));
		if (rectNewStream.IsEmpty()) continue;

		matrix.TransformRect(rectNewStream);
		rectNewStream.Normalize();
		FX_FLOAT a = rcAnnot.Width() / rectNewStream.Width();
		FX_FLOAT d = rcAnnot.Height() / rectNewStream.Height();
		FX_FLOAT e = rcAnnot.left - rectNewStream.left * a;
		FX_FLOAT f = rcAnnot.bottom - rectNewStream.bottom * d;
		CFX_AffineMatrix m = CFX_AffineMatrix(a, 0, 0, d, e, f);

		CFX_MapPtrToPtr mergeObjs;
		FX_DWORD dwTargetDocLastObjNum = 0;
		CPDF_Stream* pAnnotStream = (CPDF_Stream*)FSPDF_ST_CloneObject2Doc(pNAPStream, m_pPDFDoc, FALSE, mergeObjs, dwTargetDocLastObjNum);
		if (!pAnnotStream) return NULL;
		CPDF_Dictionary* pObjDic = pAnnotStream->GetDict();
		if (pObjDic){
			pObjDic->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
			pObjDic->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
		}

		iClonedAnnot++;
		if (0 == iClonedAnnot)
		{
			CPDF_Dictionary* pFormDict = CPDF_Dictionary::Create();
			if (!pFormDict) return NULL;
			pFormDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
			pFormDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
			pFormDict->SetAtNumber( "FormType", (int)1 ); 
			pFormDict->SetAtRect(FX_BSTRC("BBox"), stampAnnotRect);
			pDynamicAPStream = CPDF_Stream::Create(NULL, 0, pFormDict);
			m_pPDFDoc->AddIndirectObject(pDynamicAPStream);

			CPDF_Dictionary* pFormRes = CPDF_Dictionary::Create();
			if (!pFormRes) return NULL;
			pFormDict->SetAt(FX_BSTRC("Resources"), pFormRes);

			pXObjectTemp = CPDF_Dictionary::Create();
			if (!pXObjectTemp) return NULL;
			pFormRes->SetAt(FX_BSTRC("XObject"), pXObjectTemp);
		}
		if (!pDynamicAPStream) return NULL;

		CFX_ByteString sFormName;
		sFormName.Format("DSF%d", iClonedAnnot);
		FX_DWORD dwObjNum = m_pPDFDoc->AddIndirectObject(pAnnotStream);
		pXObjectTemp->SetAtReference(sFormName, m_pPDFDoc, dwObjNum);

		CPDF_StreamAcc acc;
		acc.LoadAllData(pDynamicAPStream);
		CFX_ByteString sStream(acc.GetData(), acc.GetSize());
		CFX_ByteString sTemp;
		sTemp.Format("q %f 0 0 %f %f %f cm /%s Do Q\n", m.a, m.d, m.e, m.f, (FX_LPCSTR)sFormName);
		sStream += sTemp;
		pDynamicAPStream->SetData((FX_LPCBYTE)sStream, sStream.GetLength(), FALSE, FALSE);
	}
	return pDynamicAPStream;
}

CFX_ByteString CFSPDF_AnnotIconProvider::getAPNameString(FSM_ANNOTTYPE annotType, FX_BSTR bsIconName, FS_ARGB fillColor, FS_ARGB strokeColor) const
{
	fillColor |= 0xFF000000;
	strokeColor |= 0xFF000000;

	CFX_ByteString bsID;
	CFX_ByteString bsVer;
	
	CFSPDF_IconProviderManager* pIconProviderMgr = CFS_Library::getIconProviderMgr();
	if (pIconProviderMgr)
	{
		FSAnnotIconProvider* pAnnotIconProvider = pIconProviderMgr->GetAnnotIconProvider();
		if (pAnnotIconProvider && pAnnotIconProvider->hasIcon(annotType, bsIconName.GetCStr()))
		{
			FSString sProviderID = pAnnotIconProvider->getProviderID();
			bsID = CFX_ByteString(sProviderID.getBuffer(), sProviderID.getBufferLen());
			FSString sProviderVersion = pAnnotIconProvider->getProviderVersion();
			bsVer = CFX_ByteString(sProviderVersion.getBuffer(), sProviderVersion.getBufferLen());
			if (FALSE == pAnnotIconProvider->canChangeColor(annotType, bsIconName.GetCStr()))
			{
				fillColor = 0x00FFFFFF;
				strokeColor = 0;
			}
		}
	}
	if (bsID.IsEmpty())
		bsID = FX_BSTRC("RDK");
	if (bsVer.IsEmpty())
		bsVer = FX_BSTRC("0");
	CFX_ByteString bsName;
	CFX_ByteString bsAnnotType = FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(annotType);
	CFX_ByteString bstrIconName(bsIconName.GetCStr(), bsIconName.GetLength());
	bsName.Format("Icon+%s+%s+%08X+%08X-Foxit-%s-%s", (FX_LPCSTR)bsAnnotType, (FX_LPCSTR)bstrIconName, fillColor, strokeColor, (FX_LPCSTR)bsID, (FX_LPCSTR)bsVer);
	return bsName;
}

FS_BOOL CFSPDF_AnnotIconProvider::isSupportStdIcon(FSM_ANNOTTYPE annotType) const
{
	return (annotType == FSAnnot::e_annotNote || annotType == FSAnnot::e_annotFileAttachment);
}

FS_BOOL CFSPDF_AnnotIconProvider::isSupportIconProvider(FSM_ANNOTTYPE annotType)
{
	return (annotType == FSAnnot::e_annotNote || annotType == FSAnnot::e_annotFileAttachment || annotType == FSAnnot::e_annotStamp);
}

CPDF_Stream* CFSPDF_AnnotIconProvider::findExistingAPStream(const CFX_ByteString &bsAPName) const
{
	CPDF_NameTree nt(m_pPDFDoc, FX_BSTRC("AP"));
	CPDF_Object* pObj = nt.LookupValue(bsAPName);
	if (!pObj || pObj->GetType() != PDFOBJ_STREAM) return NULL;
	return (CPDF_Stream*)pObj;
}

const FX_LPCSTR gs_FSPDF_IconProvider_ResourceItems[8] = {"ColorSpace", "ExtGState", "Font", "Pattern", "ProcSet", "Shading", "XObject", "Properties"};
CPDF_Stream* CFSPDF_AnnotIconProvider::createAPFormFromPage(CPDF_Page* pSrcPage)
{
	if (NULL == pSrcPage) return NULL;
	CPDF_Dictionary* pPageDict = pSrcPage->m_pFormDict;
	CPDF_Object* pContents = pPageDict->GetElementValue(FX_BSTRC("Contents"));
	if (!pContents) return NULL;
	CPDF_Rect box = pPageDict->GetRect(FX_BSTRC("MediaBox"));
	if (box.IsEmpty()) return NULL;

	CPDF_Dictionary* pFormDict = CPDF_Dictionary::Create();
	pFormDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
	pFormDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
	pFormDict->SetAtRect(FX_BSTRC("BBox"), box);
	CPDF_Object* pGroup = pPageDict->GetElement(FX_BSTRC("Group"));
	if (pGroup)
	{
		CFX_MapPtrToPtr MergeObjs;
		FX_DWORD TargetDocLastObjNum = 0;
		pGroup = FSPDF_ST_CloneObject2Doc(pGroup, m_pPDFDoc, FALSE, MergeObjs, TargetDocLastObjNum);
		pFormDict->SetAt(FX_BSTRC("Group"), pGroup);
	}
	CPDF_Dictionary* pResources = pPageDict->GetDict(FX_BSTRC("Resources"));
	{
		CPDF_Dictionary* pNewRes = CPDF_Dictionary::Create();
		pFormDict->SetAt(FX_BSTRC("Resources"), pNewRes);
		FX_POSITION pos = pResources->GetStartPos();
		while (pos)
		{
			CFX_ByteString key;
			CPDF_Object* pObj = pResources->GetNextElement(pos, key);
			if (!pObj) continue;
			FX_INT32 n = 0;
			for (; n < 8; n ++)
			{
				if (key == gs_FSPDF_IconProvider_ResourceItems[n])
					break;
			}
			if (n >= 8) continue;
			CFX_MapPtrToPtr MergeObjs;
			FX_DWORD TargetDocLastObjNum = 0;
			pObj = FSPDF_ST_CloneObject2Doc(pObj, m_pPDFDoc, key == FX_BSTRC("Shading"), MergeObjs, TargetDocLastObjNum);
			pNewRes->SetAt(key, pObj);
		}
	}

	CFX_BinaryBuf data;
	if (pContents->GetType() == PDFOBJ_STREAM)
	{
		CPDF_StreamAcc acc;
		acc.LoadAllData((CPDF_Stream*)pContents, FALSE);
		data.AppendBlock(acc.GetData(), acc.GetSize());
	}
	else if (pContents->GetType() == PDFOBJ_ARRAY)
	{
		FX_DWORD nCount = ((CPDF_Array*)pContents)->GetCount();
		for (FX_DWORD n = 0; n < nCount; n ++)
		{
			CPDF_Stream* pStream = ((CPDF_Array*)pContents)->GetStream(n);
			if (!pStream) continue;
			CPDF_StreamAcc acc;
			acc.LoadAllData(pStream, FALSE);
			data.AppendBlock(acc.GetData(), acc.GetSize());
		}
	}
	CPDF_Stream* pFormStream = CPDF_Stream::Create(data.GetBuffer(), data.GetSize(), pFormDict);
	data.DetachBuffer();
	m_pPDFDoc->AddIndirectObject(pFormStream);
	return pFormStream;
}

#define FSPDF_IconProvider_APString_Note_1	"%.3f %.3f %.3f rg\n"
#define FSPDF_IconProvider_APString_Note_2	"%.3f %.3f %.3f RG\n"
#define FSPDF_IconProvider_APString_Note_3	"0.600 w\n"
#define FSPDF_IconProvider_APString_FileAttachment_1	FX_BSTRC("0 1 1 RG\n")
#define FSPDF_IconProvider_APString_FileAttachment_2	"%.3f %.3f %.3f rg\n"
#define FSPDF_IconProvider_APString_FileAttachment_3	FX_BSTRC("0 w 0 J 2 j 0 G\n5 18 m\n8 15.4 l\n8 10 l\n8 10.8 12 10.8 12 10 c\n12 15.4 l\n15 18 l\n5 18 l\n8 15.4 l\n12 15.4 l\n8 10 m\n6 10 3 8.2 3 7 c\n17 7 l\n17 8.2 14 10 12 10 c\n12 10.8 8 10.8 8 10 c\n10 7 m\n10 2 l\nB*\nS")

CPDF_Stream* CFSPDF_AnnotIconProvider::createStdFormStream(CPDF_Rect annotRect, FSM_ANNOTTYPE annotType, FX_BSTR bsIconName, FX_ARGB fillColor, FX_ARGB strokeColor/*=0*/)
{
	if (FALSE == isSupportStdIcon(annotType)) 
		return NULL;

	CPDF_Dictionary* pFormDict = CPDF_Dictionary::Create();
	pFormDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
	pFormDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
	pFormDict->SetAtNumber( "FormType", (int)1 ); 
	pFormDict->SetAtRect(FX_BSTRC("BBox"), annotRect);
	CPDF_Stream* pFormStream = CPDF_Stream::Create(NULL, 0, pFormDict);
	m_pPDFDoc->AddIndirectObject(pFormStream);

	CFX_ByteTextBuf bsData;
	FX_FLOAT f_a, f_r, f_g, f_b;
	FPDFUtils_GetRGBColorComponents(fillColor, f_a, f_r, f_g, f_b);
	FX_FLOAT s_a, s_r, s_g, s_b;
	FPDFUtils_GetRGBColorComponents(strokeColor, s_a, s_r, s_g, s_b);

	CFX_ByteString bsFormat;
	bsFormat.Format(FSPDF_IconProvider_APString_Note_1, f_r, f_g, f_b);
	bsData << bsFormat;
	bsFormat.Empty();
	bsFormat.Format(FSPDF_IconProvider_APString_Note_2, s_r, s_g, s_b);
	bsData << bsFormat;
	bsFormat.Empty();
	bsFormat.Format(FSPDF_IconProvider_APString_Note_3);
	bsData << bsFormat;
	bsFormat.Empty();
	CFX_ByteString bsType = FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(annotType);
	bsFormat = CFSPDF_AnnotStdIconProvider::GetIconAppStream(bsType, bsIconName, annotRect, CFSPDFAnnot_Color(2, f_r, f_g, f_b), CFSPDFAnnot_Color(2, s_r, s_g, s_b));
	bsData << bsFormat;

	pFormStream->SetData(bsData.GetBuffer(), bsData.GetSize(), FALSE, TRUE);
	bsData.DetachBuffer();

	return pFormStream;
}

void CFSPDF_AnnotIconProvider::GetShadingFunctions(CPDF_Stream* pAPStream, CFX_PtrArray &shadings, CFX_PtrArray &functions)
{
	if (!pAPStream) return;

	CPDF_Dictionary* pDict = pAPStream->GetDict();
	if (!pDict) return;
	pDict = pDict->GetDict(FX_BSTRC("Resources"));
	if (!pDict) return;
	pDict = pDict->GetDict(FX_BSTRC("Shading"));
	if (!pDict) return;
	FX_POSITION pos = pDict->GetStartPos();
	while (pos)
	{
		CFX_ByteString key;
		CPDF_Object* pValue = pDict->GetNextElement(pos, key);
		if (!pValue) continue;
		CPDF_Dictionary* pShading = pValue->GetDict();
		if (!pShading) continue;
		shadings.Add(pShading);
		CPDF_Dictionary* pFunction = pShading->GetDict(FX_BSTRC("Function"));
		if (!pFunction) continue;
		CPDF_Array* pFunctions = pFunction->GetArray(FX_BSTRC("Functions"));
		if (!pFunctions) continue;
		FX_DWORD dwCount = pFunctions->GetCount();
		for (FX_DWORD n = 0; n < dwCount; n ++)
		{
			CPDF_Dictionary* pItem = pFunctions->GetDict(n);
			if (!pItem) continue;
			functions.Add(pItem);
		}
	}
}

FS_BOOL CFSPDF_AnnotIconProvider::getDisplaySize(FSM_ANNOTTYPE annotType, FX_BSTR bsIconName, FS_FLOAT& iconWidth, FS_FLOAT& iconHeight)
{
	if (FSAnnot::e_annotNote != annotType && FSAnnot::e_annotFileAttachment != annotType && FSAnnot::e_annotStamp != annotType)
		return FALSE;
	FX_LPCSTR sIconName = bsIconName.GetCStr();
	CFSPDF_IconProviderManager* pIconProviderMgr = CFS_Library::getIconProviderMgr();
	if (pIconProviderMgr)
	{
		FSAnnotIconProvider* pAnnotIconProvider = pIconProviderMgr->GetAnnotIconProvider();
		if (pAnnotIconProvider && pAnnotIconProvider->hasIcon(annotType, sIconName))
		{
			iconWidth = pAnnotIconProvider->getDisplayWidth(annotType, sIconName);
			iconHeight = pAnnotIconProvider->getDisplayHeight(annotType, sIconName);
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////
CFSPDFAnnot_Point::CFSPDFAnnot_Point() : CPDF_Point(0.0f,0.0f) 
{
} 

CFSPDFAnnot_Point::CFSPDFAnnot_Point( FX_FLOAT fx, FX_FLOAT fy ) : CPDF_Point(fx,fy) 
{
}

CFSPDFAnnot_Point::CFSPDFAnnot_Point( const CFSPDFAnnot_Point& point ) : CPDF_Point(point.x, point.y) 
{
} 

CFSPDFAnnot_PathData::CFSPDFAnnot_PathData() : m_point(), m_nType(FPDFANNOT_PATHDATA_UNKNOW) 
{ 
}

CFSPDFAnnot_PathData::CFSPDFAnnot_PathData( const CFSPDFAnnot_Point& point, int nType ) : m_point(point), m_nType(nType)
{
}

CFSPDFAnnot_Color::CFSPDFAnnot_Color(FX_INT32 type, FX_FLOAT color1, FX_FLOAT color2, FX_FLOAT color3, FX_FLOAT color4) :
m_nColorType(type),
	m_flColor1(color1),
	m_flColor2(color2),
	m_flColor3(color3),
	m_flColor4(color4)
{
}

//void CFSPDFAnnot_Color::ConvertColorType(int nColorType)
//{
//
//}

///////////////////////////////////////////////////////////////////////////
//Class CFSPDF_AnnotStdIconProvider
CFX_ByteString CFSPDF_AnnotStdIconProvider::GetIconAppStream(CFX_ByteString bsAnnotType, CFX_ByteString csIconType, const CPDF_Rect& crRect, 
															const CFSPDFAnnot_Color& crFill, const CFSPDFAnnot_Color& crStroke /*= FSPDFANNOT_DEFAULT_BLACKCOLOR*/)
{
	CFX_ByteString sAppStream = CFSPDF_AnnotStdIconProvider::GetColorAppStream( crStroke, FALSE ) ; 
	sAppStream += CFSPDF_AnnotStdIconProvider::GetColorAppStream( crFill, TRUE ) ; 

	CFX_ByteString sPath ; 
	CFX_PathData path ; 
	if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_NOTE))
	{
		if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_CHECK) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Check( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_CIRCLE) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Circle( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_COMMENT) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Comment( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_CROSS) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Cross( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_HELP) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Help( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_INSERT) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_InsertText( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_KEY) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Key( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_NEWPARAGRAPH) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_NewParagraph( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_NOTE) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_TextNote( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_PARAGRAPH) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Paragraph( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_RIGHARROW) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_RightArrow( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_RIGHPOINTER) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_RightPointer( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_STAR) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Star( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_UPARROW) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_UpArrow( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_TEXT_UPLEFTARROW) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_UpLeftArrow( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else //Default: TextNote
			CFSPDF_AnnotStdIconProvider::GetGraphics_TextNote( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ;
	}
	else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_FILEATTACHMENT))
	{
		if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_FILEATTACH_GRAPH) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Graph( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_FILEATTACH_PAPERCLIP) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Paperclip( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_FILEATTACH_PUSHPIN) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_PushPin( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else if ( csIconType.Equal(FSPDF_ANNOT_ICONNAME_FILEATTACH_TAG) ) 
			CFSPDF_AnnotStdIconProvider::GetGraphics_Tag( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
		else //Default: PushPin
			CFSPDF_AnnotStdIconProvider::GetGraphics_PushPin( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM ) ; 
	}
	else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_SIGNATURE))
	{
		if (csIconType.Equal(FSPDF_ANNOT_ICONNAME_SIGNATURE_FOXIEFILAG))
			CFSPDF_AnnotStdIconProvider::GetGraphics_FoxitFlag( sPath, path, crRect, FPDFANNOT_PATHTYPE_STREAM );
	}
	else
		return "";

	sAppStream += sPath ; 

	if ( crStroke.m_nColorType != COLORTYPE_TRANSPARENT ) 
		sAppStream += "B*\n" ; 
	else
		sAppStream += "f*\n" ; 

	return sAppStream ; 
}

CFX_ByteString CFSPDF_AnnotStdIconProvider::GetColorAppStream( const CFSPDFAnnot_Color& color, const FX_BOOL& bFillOrStroke /*= TRUE*/ ) 
{
	CFX_ByteTextBuf sColorStream ; 

	switch ( color.m_nColorType ) 
	{		
	case COLORTYPE_RGB : 
		sColorStream << color.m_flColor1 << " " << color.m_flColor2 << " " << color.m_flColor3 << " " << (bFillOrStroke ? "rg" : "RG") << "\n" ; 
		break ; 
	case COLORTYPE_GRAY : 
		sColorStream << color.m_flColor1 << " " << (bFillOrStroke ? "g" : "G") << "\n" ; 
		break ; 
	case COLORTYPE_CMYK : 
		sColorStream << color.m_flColor1 << " " << color.m_flColor2 << " " << color.m_flColor3 << " " << color.m_flColor4 << " " << (bFillOrStroke ? "k" : "K") << "\n" ; 
		break ; 
	}

	return sColorStream.GetByteString() ; 
} 

CFX_ByteString CFSPDF_AnnotStdIconProvider::GetAppStreamFromArray( const CFSPDFAnnot_PathData* pPathData, FX_INT32 nCount ) 
{
	CFX_ByteTextBuf csAP ; 

	for ( FX_INT32 i=0; i<nCount; i++ ) 
	{
		switch ( pPathData[i].m_nType ) 
		{
		case FPDFANNOT_PATHDATA_MOVETO : 
			csAP << pPathData[i].m_point.x << " " << pPathData[i].m_point.y << " m\n" ; 
			break ; 
		case FPDFANNOT_PATHDATA_LINETO : 
			csAP << pPathData[i].m_point.x << " " << pPathData[i].m_point.y << " l\n";
			break ; 
		case FPDFANNOT_PATHDATA_BEZIERTO : 
			csAP << pPathData[i].m_point.x << " " << pPathData[i].m_point.y << " " 
				<< pPathData[i+1].m_point.x << " " << pPathData[i+1].m_point.y << " " 
				<< pPathData[i+2].m_point.x << " " << pPathData[i+2].m_point.y << " c\n" ; 

			i += 2 ; 
			break ; 
		}
	}

	return csAP.GetByteString() ; 
}

void CFSPDF_AnnotStdIconProvider::GetPathDataFromArray( CFX_PathData& cpPath, const CFSPDFAnnot_PathData* pPathData, FX_INT32 nCount) 
{
	cpPath.SetPointCount( nCount ) ; 

	for ( FX_INT32 i=0; i<nCount; i++ ) 
	{
		switch ( pPathData[i].m_nType ) 
		{
		case FPDFANNOT_PATHDATA_MOVETO : 
			cpPath.SetPoint( i, pPathData[i].m_point.x, pPathData[i].m_point.y, FXPT_MOVETO ) ; 
			break ; 
		case FPDFANNOT_PATHDATA_LINETO : 
			cpPath.SetPoint( i, pPathData[i].m_point.x, pPathData[i].m_point.y, FXPT_LINETO ) ; 
			break ; 
		case FPDFANNOT_PATHDATA_BEZIERTO : 
			cpPath.SetPoint( i, pPathData[i].m_point.x, pPathData[i].m_point.y, FXPT_BEZIERTO ) ; 
			break ; 
		}
	}
} 

void CFSPDF_AnnotStdIconProvider::GetGraphics_Check( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{ 
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 15.0f, crRect.bottom + fHeight * 2 / 5.0f), FPDFANNOT_PATHDATA_MOVETO), 
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 15.0f + FPDFANNOTUTIL_BEZIER*(fWidth / 7.0f - fWidth / 15.0f), 
		crRect.bottom + fHeight * 2 / 5.0f + FPDFANNOTUTIL_BEZIER*(fHeight * 2 / 7.0f - fHeight * 2 / 5.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 4.5f + FPDFANNOTUTIL_BEZIER*(fWidth / 5.0f - fWidth / 4.5f),
		crRect.bottom + fHeight / 16.0f + FPDFANNOTUTIL_BEZIER*(fHeight / 5.0f - fHeight / 16.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 4.5f, crRect.bottom + fHeight / 16.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 4.5f + FPDFANNOTUTIL_BEZIER*(fWidth / 4.4f - fWidth / 4.5f),
		crRect.bottom + fHeight / 16.0f - FPDFANNOTUTIL_BEZIER*fHeight / 16.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 3.0f + FPDFANNOTUTIL_BEZIER*(fWidth / 4.0f - fWidth / 3.0f),
		crRect.bottom), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 3.0f, crRect.bottom), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 3.0f + FPDFANNOTUTIL_BEZIER*fWidth*(1/7.0f + 2/15.0f),
		crRect.bottom + FPDFANNOTUTIL_BEZIER*fHeight * 4 / 5.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth * 14 / 15.0f + FPDFANNOTUTIL_BEZIER*fWidth*(1/7.0f - 7/15.0f),
		crRect.bottom + fHeight * 15/16.0f + FPDFANNOTUTIL_BEZIER*(fHeight * 4/5.0f - fHeight * 15/16.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth * 14 / 15.0f,crRect.bottom + fHeight * 15 / 16.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth * 14 / 15.0f + FPDFANNOTUTIL_BEZIER*(fWidth * 7 / 15.0f - fWidth * 14 / 15.0f),
		crRect.bottom + fHeight * 15 / 16.0f + FPDFANNOTUTIL_BEZIER*(fHeight * 8 / 7.0f - fHeight * 15 / 16.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 3.6f + FPDFANNOTUTIL_BEZIER*(fWidth / 3.4f - fWidth / 3.6f),
		crRect.bottom + fHeight / 3.5f + FPDFANNOTUTIL_BEZIER*(fHeight / 3.5f - fHeight / 3.5f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 3.6f,crRect.bottom + fHeight / 3.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 3.6f,
		crRect.bottom + fHeight / 3.5f + FPDFANNOTUTIL_BEZIER*(fHeight / 4.0f - fHeight / 3.5f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 15.0f + FPDFANNOTUTIL_BEZIER*(fWidth / 3.5f - fWidth / 15.0f),
		crRect.bottom + fHeight * 2 / 5.0f + FPDFANNOTUTIL_BEZIER*(fHeight * 3.5f / 5.0f - fHeight * 2 / 5.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth / 15.0f,crRect.bottom + fHeight * 2 / 5.0f), FPDFANNOT_PATHDATA_BEZIERTO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 16 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 16 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Circle( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] =
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*(fHeight*14/15.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f - FPDFANNOTUTIL_BEZIER*(fWidth/2.0f - fWidth/15.0f), crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + FPDFANNOTUTIL_BEZIER*(fWidth*14/15.0f - fWidth/2.0f), crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*(fHeight*14/15.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.bottom + fHeight/2.0f - FPDFANNOTUTIL_BEZIER*(fHeight/2.0f - fHeight/15.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + FPDFANNOTUTIL_BEZIER*(fWidth*14/15.0f - fWidth/2.0f), crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f - FPDFANNOTUTIL_BEZIER*(fWidth/2.0f - fWidth/15.0f), crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.bottom + fHeight/2.0f - FPDFANNOTUTIL_BEZIER*(fHeight/2.0f - fHeight/15.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*3/15.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*3/15.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*(fHeight*4/5.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f - FPDFANNOTUTIL_BEZIER*(fWidth/2.0f - fWidth*3/15.0f), crRect.top - fHeight*3/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight*3/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + FPDFANNOTUTIL_BEZIER*(fWidth*4/5.0f - fWidth/2.0f), crRect.top - fHeight*3/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/15.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*(fHeight*4/5.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/15.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/15.0f, crRect.bottom + fHeight/2.0f - FPDFANNOTUTIL_BEZIER*(fHeight*4/5.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + FPDFANNOTUTIL_BEZIER*(fWidth*4/5.0f - fWidth/2.0f), crRect.bottom + fHeight*3/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.bottom + fHeight*3/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f - FPDFANNOTUTIL_BEZIER*(fWidth*4/5.0f - fWidth/2.0f), crRect.bottom + fHeight*3/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*3/15.0f, crRect.bottom + fHeight/2.0f - FPDFANNOTUTIL_BEZIER*(fHeight*4/5.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*3/15.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_BEZIERTO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 26 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 26 ) ; 
}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Comment( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{ 
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.top - fHeight/6.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.top - fHeight/6.0f + FPDFANNOTUTIL_BEZIER*(fHeight/6.0f - fHeight/10.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15.0f - FPDFANNOTUTIL_BEZIER*fWidth/15.0f, crRect.top - fHeight/10.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15.0f, crRect.top - fHeight/10.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*2/15.0f, crRect.top - fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*2/15.0f + FPDFANNOTUTIL_BEZIER*fWidth/15.0f, crRect.top - fHeight/10.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.top - fHeight/6 + FPDFANNOTUTIL_BEZIER*(fHeight/6.0f - fHeight/10.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.top - fHeight/6.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.bottom + fHeight/3.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.bottom + fHeight*4/15.0f + FPDFANNOTUTIL_BEZIER*fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*2/15.0f + FPDFANNOTUTIL_BEZIER*fWidth/15.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*2/15.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*5/15.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*5/15.0f, crRect.bottom + fHeight*2/15 + FPDFANNOTUTIL_BEZIER*fHeight*2/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*5/15.0f - FPDFANNOTUTIL_BEZIER*fWidth*2/15.0f, crRect.bottom + fHeight*2/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*6/30.0f, crRect.bottom + fHeight*2/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*7/30.0f + FPDFANNOTUTIL_BEZIER*fWidth/30.0f, crRect.bottom + fHeight*2/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*7/30.0f, crRect.bottom + fHeight*2/15.0f + FPDFANNOTUTIL_BEZIER*fHeight*2/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*7/30.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15.0f - FPDFANNOTUTIL_BEZIER*fWidth/15.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.bottom + fHeight/3.0f - FPDFANNOTUTIL_BEZIER*fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.bottom + fHeight/3.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/15.0f, crRect.top - fHeight/6.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15.0f, crRect.top - fHeight*8/30.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*2/15.0f, crRect.top - fHeight*8/30.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15, crRect.top - fHeight*25/60.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*2/15.0f, crRect.top - fHeight*25/60.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*2/15.0f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*4/15.0f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 30 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 30 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Cross( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	FX_FLOAT fcatercorner = (FX_FLOAT)sqrt( fWidth*fWidth + fHeight*fHeight ) ; 
	CFSPDFAnnot_Point center_point(crRect.left + fWidth/2, crRect.bottom + fHeight/2) ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x, center_point.y + fHeight/10.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x + fWidth*0.3f, center_point.y + fHeight/10.0f + fWidth*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x + fWidth/10.0f + fWidth*0.3f, center_point.y + fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x + fWidth/10.0f, center_point.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x + fWidth/10.0f + fWidth*0.3f, center_point.y - fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x + fWidth*0.3f, center_point.y - fHeight/10.0f - fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x, center_point.y - fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x - fWidth*0.3f, center_point.y - fHeight/10 - fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x - fWidth/10.0f - fWidth*0.3f, center_point.y - fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x - fWidth/10, center_point.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x - fWidth/10 - fWidth*0.3f, center_point.y + fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x - fWidth*0.3f, center_point.y + fHeight/10.0f + fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(center_point.x, center_point.y + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 13 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 13 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Help( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*(fHeight/60.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f - FPDFANNOTUTIL_BEZIER*(fWidth/2.0f - fWidth/60.0f), crRect.bottom + fHeight/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.bottom + fHeight/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + FPDFANNOTUTIL_BEZIER*fWidth*29/60.0f, crRect.bottom + fHeight/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/60.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*(fHeight/60.0f - fHeight/2.0f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/60.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/60.0f, crRect.bottom + fHeight/2.0f + FPDFANNOTUTIL_BEZIER*fHeight*29/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + FPDFANNOTUTIL_BEZIER*fWidth*29/60.0f, crRect.top - fHeight/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),		
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f - FPDFANNOTUTIL_BEZIER*fWidth*29/60.0f, crRect.top - fHeight/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60.0f, crRect.bottom + fHeight/2.0f  + FPDFANNOTUTIL_BEZIER*fHeight*29/60.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60.0f, crRect.bottom + fHeight/2.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.27f, crRect.top - fHeight*0.36f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.27f, crRect.top - fHeight*0.36f + FPDFANNOTUTIL_BEZIER*fHeight*0.23f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f - FPDFANNOTUTIL_BEZIER*fWidth*0.23f, crRect.bottom + fHeight*0.87f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.bottom + fHeight*0.87f), FPDFANNOT_PATHDATA_BEZIERTO),		
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f + FPDFANNOTUTIL_BEZIER*fWidth*0.23f, crRect.bottom + fHeight*0.87f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.27f, crRect.top - fHeight*0.36f + FPDFANNOTUTIL_BEZIER*fHeight*0.23f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.27f, crRect.top - fHeight*0.36f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.27f - fWidth*0.08f*0.2f, crRect.top - fHeight*0.36f - fHeight*0.15f*0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.35f + fWidth*0.08f*0.2f, crRect.top - fHeight*0.51f + fHeight*0.15f*0.2f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.35f, crRect.top - fHeight*0.51f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.35f - fWidth*0.1f*0.5f, crRect.top - fHeight*0.51f - fHeight*0.15f*0.3f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f - fWidth*0.1f*0.5f, crRect.top - fHeight*0.68f + fHeight*0.15f*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f, crRect.top - fHeight*0.68f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f, crRect.bottom + fHeight*0.30f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f, crRect.bottom + fHeight*0.30f - fWidth*0.1f*0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.55f, crRect.bottom + fHeight*0.30f - fWidth*0.1f*0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.55f, crRect.bottom + fHeight*0.30f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.55f, crRect.top - fHeight*0.66f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.55f - fWidth*0.1f*0.05f, crRect.top - fHeight*0.66f + fHeight*0.18f*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f - fWidth*0.1f*0.05f, crRect.top - fHeight*0.48f - fHeight*0.18f*0.3f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f, crRect.top - fHeight*0.48f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.45f + fWidth*0.08f*0.2f, crRect.top - fHeight*0.48f + fHeight*0.18f*0.2f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.37f - fWidth*0.08f*0.2f, crRect.top - fHeight*0.36f - fHeight*0.18f*0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.37f, crRect.top - fHeight*0.36f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.37f, crRect.top - fHeight*0.36f + FPDFANNOTUTIL_BEZIER*fHeight*0.13f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f + FPDFANNOTUTIL_BEZIER*fWidth*0.13f, crRect.bottom + fHeight*0.77f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.bottom + fHeight*0.77f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f - FPDFANNOTUTIL_BEZIER*fWidth*0.13f, crRect.bottom + fHeight*0.77f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.37f, crRect.top - fHeight*0.36f + FPDFANNOTUTIL_BEZIER*fHeight*0.13f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.37f, crRect.top - fHeight*0.36f), FPDFANNOT_PATHDATA_BEZIERTO),		
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.37f, crRect.top - fHeight*0.36f - fWidth*0.1f*0.6f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.27f, crRect.top - fHeight*0.36f - fWidth*0.1f*0.6f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.27f, crRect.top - fHeight*0.36f), FPDFANNOT_PATHDATA_BEZIERTO),		
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.56f, crRect.bottom + fHeight*0.13f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.56f, crRect.bottom + fHeight*0.13f + FPDFANNOTUTIL_BEZIER*fHeight*0.055f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.505f - FPDFANNOTUTIL_BEZIER*fWidth*0.095f, crRect.bottom + fHeight*0.185f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.505f, crRect.bottom + fHeight*0.185f), FPDFANNOT_PATHDATA_BEZIERTO),		
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.505f + FPDFANNOTUTIL_BEZIER*fWidth*0.065f, crRect.bottom + fHeight*0.185f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.44f, crRect.bottom + fHeight*0.13f + FPDFANNOTUTIL_BEZIER*fHeight*0.055f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.44f, crRect.bottom + fHeight*0.13f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.44f, crRect.bottom + fHeight*0.13f - FPDFANNOTUTIL_BEZIER*fHeight*0.055f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.505f + FPDFANNOTUTIL_BEZIER*fWidth*0.065f, crRect.bottom + fHeight*0.075f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.505f, crRect.bottom + fHeight*0.075f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.505f - FPDFANNOTUTIL_BEZIER*fWidth*0.065f, crRect.bottom + fHeight*0.075f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.56f, crRect.bottom + fHeight*0.13f - FPDFANNOTUTIL_BEZIER*fHeight*0.055f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.56f, crRect.bottom + fHeight*0.13f), FPDFANNOT_PATHDATA_BEZIERTO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 59 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 59 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_InsertText( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/10, crRect.bottom + fHeight/10), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2, crRect.top - fHeight*2/15), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10, crRect.bottom + fHeight/10), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/10, crRect.bottom + fHeight/10), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 4 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 4 ) ; 
}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Key( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 
	FX_FLOAT k = -fHeight/fWidth ; 

	CFSPDFAnnot_Point tail;
	CFSPDFAnnot_Point CicleCenter;
	tail.x = crRect.left + fWidth*0.9f;
	tail.y = k*(tail.x - crRect.right) + crRect.bottom;
	CicleCenter.x = crRect.left + fWidth*0.15f;
	CicleCenter.y = k*(CicleCenter.x - crRect.right) + crRect.bottom;

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30.0f, -fWidth/30.0f/k + tail.y), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30.0f - fWidth*0.18f, -k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f + fWidth*0.07f,
		-fWidth*0.07f/k - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f - fWidth/20 + fWidth*0.07f,
		-fWidth*0.07f/k - k*fWidth/20 - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f - fWidth/20,
		-k*fWidth/20 - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f - fWidth/20 - fWidth/15,
		-k*fWidth/15 - k*fWidth/20 - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f - fWidth/20 - fWidth/15 + fWidth*0.07f,
		-fWidth*0.07f/k - k*fWidth/15 - k*fWidth/20 - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f - fWidth/20 - fWidth/15 - fWidth/20 + fWidth*0.07f,
		-fWidth*0.07f/k + -k*fWidth/20 + -k*fWidth/15 - k*fWidth/20 - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.18f - fWidth/20 - fWidth/15 - fWidth/20,
		-k*fWidth/20 + -k*fWidth/15 - k*fWidth/20 - k*fWidth*0.18f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.45f, -k*fWidth*0.45f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30 - fWidth*0.45f + fWidth*0.2f,
		-fWidth*0.4f/k - k*fWidth*0.45f - fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.2f, - fWidth*0.1f/k + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x, CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x - fWidth/60.0f, -k*fWidth/60 + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x - fWidth/60, -k*fWidth/60 + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x, CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x - fWidth*0.22f, fWidth*0.35f/k + CicleCenter.y - fHeight*0.05f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x - fWidth/30 - fWidth*0.45f - fWidth*0.18f, fWidth*0.05f/k - k*fWidth*0.45f + fWidth/30/k + tail.y - fHeight*0.05f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x - fWidth/30.0f - fWidth*0.45f, -k*fWidth*0.45f + fWidth/30.0f/k + tail.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x - fWidth/30.0f, fWidth/30.0f/k + tail.y), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/30, -fWidth/30/k + tail.y), FPDFANNOT_PATHDATA_LINETO),		
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.08f, k*fWidth*0.08f + CicleCenter.y), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.08f + fWidth*0.1f, -fWidth*0.1f/k + k*fWidth*0.08f + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.22f + fWidth*0.1f, k*fWidth*0.22f + CicleCenter.y - fWidth*0.1f/k), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.22f, k*fWidth*0.22f + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.22f - fWidth*0.1f, fWidth*0.1f/k + k*fWidth*0.22f + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.08f - fWidth*0.1f, fWidth*0.1f/k + k*fWidth*0.08f + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(CicleCenter.x + fWidth*0.08f, k*fWidth*0.08f + CicleCenter.y), FPDFANNOT_PATHDATA_BEZIERTO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 28 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 28 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_NewParagraph( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/20.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/10.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/20.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.12f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.12f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.22f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.22f, crRect.top - fHeight*17/30.0f - fWidth*0.14f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.38f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.48f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.48f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.38f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.38f, crRect.bottom + fWidth*0.24f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.22f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.12f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.bottom + fHeight/10.0f + fHeight/7.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.97f, crRect.bottom + fHeight/10.0f + fHeight/7.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.97f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*17/30.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.bottom + fHeight/10.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.bottom + fHeight/7 + fHeight*0.18f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.85f, crRect.bottom + fHeight/7 + fHeight*0.18f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.85f, crRect.top - fHeight*17/30.0f - fHeight*0.08f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.top - fHeight*17/30.0f - fHeight*0.08f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.bottom + fHeight/7 + fHeight*0.18f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 28 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 28 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_TextNote( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/10.0f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*7/10.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/10.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/10.0f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/10.0f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/10.0f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/10.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10.0f, crRect.bottom + fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/5.0f, crRect.top - fHeight*4/15.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/5.0f, crRect.top - fHeight*4/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/5.0f, crRect.top - fHeight*7/15.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/5.0f, crRect.top - fHeight*7/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/5.0f, crRect.top - fHeight*10/15.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*3/10.0f, crRect.top - fHeight*10/15.0f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 17 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 17 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Paragraph( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] =
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.634f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.634f, crRect.top - fHeight*2/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.566f, crRect.top - fHeight*2/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.566f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/15.0f - fHeight*0.4f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.2f, crRect.top - fHeight/15.0f - fHeight*0.4f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.2f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_BEZIERTO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 12 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 12 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_RightArrow( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + fWidth/8.0f, crRect.bottom + fHeight/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.bottom + fHeight/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f - fWidth*0.15f, crRect.top - fHeight/2.0f - fWidth/25.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.1f, crRect.top - fHeight/2.0f - fWidth/25.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.1f, crRect.top - fHeight/2.0f + fWidth/25.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f - fWidth*0.15f, crRect.top - fHeight/2.0f + fWidth/25.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f + fWidth/8.0f, crRect.top - fHeight/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 10 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 10 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_RightPointer( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/30.0f, crRect.bottom + fHeight/6.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*4/15.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/30.0f, crRect.top - fHeight/6.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30.0f, crRect.top - fHeight/2.0f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 5 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 5 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Star( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fLongRadius = (crRect.top - crRect.bottom)/(1+(FX_FLOAT)FXSYS_cos(FPDFANNOTUTIL_PI/5.0f)) ; 
	fLongRadius = fLongRadius * 0.7f ; 

	FX_FLOAT fShortRadius = fLongRadius * 0.55f ; 

	CPDF_Point ptCenter = CPDF_Point( (crRect.left + crRect.right)/2.0f, (crRect.top + crRect.bottom)/2.0f ) ; 

	FX_FLOAT px1[5], py1[5] ; 
	FX_FLOAT px2[5], py2[5] ; 

	FX_FLOAT fAngel = FPDFANNOTUTIL_PI/10.0f ; 

	for ( FX_INT32 i=0; i<5; i++ ) 
	{
		px1[i] = ptCenter.x + fLongRadius * (FX_FLOAT)FXSYS_cos(fAngel) ; 
		py1[i] = ptCenter.y + fLongRadius * (FX_FLOAT)FXSYS_sin(fAngel) ; 

		fAngel += FPDFANNOTUTIL_PI * 2 / 5.0f ; 
	}

	fAngel = FPDFANNOTUTIL_PI/5.0f + FPDFANNOTUTIL_PI/10.0f ; 

	for ( FX_INT32 j=0; j<5; j++ ) 
	{
		px2[j] = ptCenter.x + fShortRadius * (FX_FLOAT)FXSYS_cos(fAngel) ; 
		py2[j] = ptCenter.y + fShortRadius * (FX_FLOAT)FXSYS_sin(fAngel) ; 

		fAngel += FPDFANNOTUTIL_PI * 2 / 5.0f ; 
	}

	CFSPDFAnnot_PathData PathArray[11] ; 
	PathArray[0] = CFSPDFAnnot_PathData(CFSPDFAnnot_Point(px1[0], py1[0]), FPDFANNOT_PATHDATA_MOVETO) ; 
	PathArray[1] = CFSPDFAnnot_PathData(CFSPDFAnnot_Point(px2[0], py2[0]), FPDFANNOT_PATHDATA_LINETO) ; 

	for ( FX_INT32 k=0; k<4; k++ ) 
	{
		PathArray[(k+1)*2] = CFSPDFAnnot_PathData(CFSPDFAnnot_Point(px1[k+1], py1[k+1]), FPDFANNOT_PATHDATA_LINETO) ; 
		PathArray[(k+1)*2 + 1] = CFSPDFAnnot_PathData(CFSPDFAnnot_Point(px2[k+1], py2[k+1]), FPDFANNOT_PATHDATA_LINETO) ; 
	}

	PathArray[10] = CFSPDFAnnot_PathData(CFSPDFAnnot_Point(px1[0], py1[0]), FPDFANNOT_PATHDATA_LINETO) ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 11 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 11 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_UpArrow( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/10.0f, crRect.top - fWidth*3/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fWidth*3/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.bottom + fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fWidth*3/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/10, crRect.top - fWidth*3/5.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/2.0f, crRect.top - fHeight/15.0f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 8 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 8 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_UpLeftArrow( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_Point leftup(crRect.left, crRect.top);
	CFSPDFAnnot_Point rightdown(crRect.right, crRect.bottom);
	FX_FLOAT k = -fHeight/fWidth;
	CFSPDFAnnot_Point tail;
	tail.x = crRect.left + fWidth*4/5.0f;
	tail.y = k*(tail.x - crRect.right) + rightdown.y;

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/20.0f, k*(crRect.left + fWidth/20.0f - rightdown.x) + rightdown.y), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(fHeight*17/60.0f/k + tail.x + fWidth/10.0f + fWidth/5.0f,
		-fWidth/5.0f/k + tail.y - fWidth/10.0f/k + fHeight*17/60.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(fHeight*17/60.0f/k + tail.x + fWidth/10.0f,
		tail.y - fWidth/10.0f/k + fHeight*17/60.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x + fWidth/10.0f, tail.y - fWidth/10.0f/k), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(tail.x - fWidth/10.0f, tail.y + fWidth/10.0f/k), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(fHeight*17/60.0f/k + tail.x - fWidth/10.0f, tail.y + fWidth/10.0f/k + fHeight*17/60.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(fHeight*17/60.0f/k + tail.x - fWidth/10.0f - fWidth/5.0f,
		fWidth/5.0f/k + tail.y + fWidth/10.0f/k + fHeight*17/60.0f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/20.0f, k*(crRect.left + fWidth/20.0f - rightdown.x) + rightdown.y), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 8 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 8 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Graph( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left ; 
	FX_FLOAT fHeight = crRect.top - crRect.bottom ; 

	CFSPDFAnnot_PathData PathArray[] = 
	{ 
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.05f, crRect.top - fWidth*0.15f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.25f, crRect.top - fHeight*0.15f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.275f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.05f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.05f, crRect.top - fWidth*0.15f), FPDFANNOT_PATHDATA_LINETO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.275f, crRect.top - fWidth*0.45f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.475f, crRect.top - fWidth*0.45f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.475f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.275f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.275f, crRect.top - fWidth*0.45f), FPDFANNOT_PATHDATA_LINETO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.top - fHeight*0.05f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.top - fHeight*0.05f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.7f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.top - fHeight*0.05f), FPDFANNOT_PATHDATA_LINETO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.725f, crRect.top - fWidth*0.35f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.925f, crRect.top - fWidth*0.35f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.925f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.725f, crRect.bottom + fHeight*0.08f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.725f, crRect.top - fWidth*0.35f), FPDFANNOT_PATHDATA_LINETO)
	} ;

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 20 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 20 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Paperclip( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType )
{
	FX_FLOAT fWidth = crRect.right - crRect.left;
	FX_FLOAT fHeight = crRect.top - crRect.bottom;

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60, crRect.top - fHeight*0.25f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60, crRect.bottom + fHeight*0.25f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60, crRect.bottom + fHeight*0.25f - fWidth*57/60.0f*0.35f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30, crRect.bottom + fHeight*0.25f - fWidth*57/60.0f*0.35f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30, crRect.bottom + fHeight*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30, crRect.top - fHeight*0.33f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30, crRect.top - fHeight*0.33f + fHeight/15*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30 - fWidth*0.12f, crRect.top - fHeight*0.33f + fHeight/15*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30 - fWidth*0.12f, crRect.top - fHeight*0.33f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30 - fWidth*0.12f, crRect.bottom + fHeight*0.2f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/30 - fWidth*0.12f, crRect.bottom + fHeight*0.2f - (fWidth*57/60.0f - fWidth*0.24f)*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60 + fWidth*0.12f, crRect.bottom + fHeight*0.2f - (fWidth*57/60.0f - fWidth*0.24f)*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60 + fWidth*0.12f, crRect.bottom + fHeight*0.2f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60 + fWidth*0.12f, crRect.top - fHeight*0.2f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60 + fWidth*0.12f, crRect.top - fHeight*0.2f + (fWidth*11/12.0f - fWidth*0.36f)*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.24f, crRect.top - fHeight*0.2f + (fWidth*11/12.0f - fWidth*0.36f)*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.24f, crRect.top - fHeight*0.2f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.24f, crRect.bottom + fHeight*0.25f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.24f, crRect.bottom + fHeight*0.25f - (fWidth*14/15.0f - fWidth*0.53f)*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.29f, crRect.bottom + fHeight*0.25f - (fWidth*14/15.0f - fWidth*0.53f)*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.29f, crRect.bottom + fHeight*0.25f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.29f, crRect.top - fHeight*0.33f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.29f, crRect.top - fHeight*0.33f + fWidth*0.12f*0.35f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.17f, crRect.top - fHeight*0.33f + fWidth*0.12f*0.35f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.17f, crRect.top - fHeight*0.33f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.17f, crRect.bottom + fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.17f, crRect.bottom + fHeight*0.3f - fWidth*(14/15.0f - 0.29f)*0.35f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.12f, crRect.bottom + fHeight*0.3f - fWidth*(14/15.0f - 0.29f)*0.35f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.12f, crRect.bottom + fHeight*0.3f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.12f, crRect.top - fHeight*0.25f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth/15 - fWidth*0.12f, crRect.top - fHeight*0.25f + fWidth*0.35f*(11/12.0f - 0.12f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60, crRect.top - fHeight*0.25f + fWidth*0.35f*(11/12.0f - 0.12f)), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth/60, crRect.top - fHeight*0.25f), FPDFANNOT_PATHDATA_BEZIERTO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 33 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 33 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_PushPin( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType ) 
{
	FX_FLOAT fWidth = crRect.right - crRect.left;
	FX_FLOAT fHeight = crRect.top - crRect.bottom;

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.25f, crRect.top - fHeight*0.1f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.23f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.5f + fWidth*0.04f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*0.5f + fWidth*0.04f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*0.23f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.25f, crRect.top - fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.25f, crRect.top - fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.23f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*0.23f), FPDFANNOT_PATHDATA_LINETO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f - fWidth*0.25f*0.4f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.15f, crRect.top - fHeight*0.65f + fHeight*0.15f*0.4f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.15f, crRect.top - fHeight*0.65f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.15f, crRect.top - fHeight*0.65f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.15f, crRect.top - fHeight*0.65f + fHeight*0.15f*0.4f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f + fWidth*0.25f*0.4f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.6f, crRect.top - fHeight*0.5f + fWidth*0.04f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.5f + fWidth*0.04f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.top - fHeight*0.65f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.5f, crRect.bottom + fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 24 ) ; 
	else
		GetPathDataFromArray( cpPath, PathArray, 24 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_Tag( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& crRect, 
	const FX_INT32 nType ) 
{
	FX_FLOAT fWidth = crRect.right - crRect.left;
	FX_FLOAT fHeight = crRect.top - crRect.bottom;

	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.1f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.1f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.3f, crRect.bottom + fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.1f, crRect.bottom + fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.1f, crRect.top - fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.1f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.3f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.2f, crRect.top - fHeight*0.3f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.2f, crRect.top - fHeight*0.5f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.left + fWidth*0.4f, crRect.top - fHeight*0.7f), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crRect.right - fWidth*0.2f, crRect.top - fHeight*0.7f), FPDFANNOT_PATHDATA_LINETO)
	} ; 

	if ( nType == FPDFANNOT_PATHTYPE_STREAM ) 
		csPathData = GetAppStreamFromArray( PathArray, 12 ) ; 
	else 
		GetPathDataFromArray( cpPath, PathArray, 12 ) ; 

}

void CFSPDF_AnnotStdIconProvider::GetGraphics_FoxitFlag( CFX_ByteString& csPathData, CFX_PathData& cpPath, const CPDF_Rect& rcPos, const FX_INT32 nType )
{
	FX_FLOAT fOutWidth = rcPos.right - rcPos.left;
	FX_FLOAT fOutHeight = rcPos.top - rcPos.bottom;

	CPDF_Rect crInBox = rcPos;
	crInBox.left = rcPos.left + fOutWidth*0.08f;
	crInBox.right = rcPos.right - fOutWidth*0.08f;
	crInBox.top = rcPos.top - fOutHeight*0.08f;
	crInBox.bottom = rcPos.bottom + fOutHeight*0.08f;

	FX_FLOAT fWidth = crInBox.right - crInBox.left;
	FX_FLOAT fHeight = crInBox.top - crInBox.bottom;
	CFSPDFAnnot_PathData PathArray[] = 
	{
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.top), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.45f, crInBox.top), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.45f, crInBox.top - FPDFANNOTUTIL_BEZIER * fHeight * 0.4f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.45f  - FPDFANNOTUTIL_BEZIER * fWidth * 0.45f, crInBox.top - fHeight*0.4f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.top - fHeight*0.4f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.top), FPDFANNOT_PATHDATA_LINETO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.60f, crInBox.top), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.75f, crInBox.top), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.75f, crInBox.top - FPDFANNOTUTIL_BEZIER * fHeight * 0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.75f  - FPDFANNOTUTIL_BEZIER * fWidth * 0.75f, crInBox.top - fHeight*0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.top - fHeight*0.7f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.top - fHeight*0.55f), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + FPDFANNOTUTIL_BEZIER * fWidth*0.60f, crInBox.top - fHeight*0.55f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth * 0.60f, crInBox.top - FPDFANNOTUTIL_BEZIER * fHeight * 0.55f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth * 0.60f, crInBox.top), FPDFANNOT_PATHDATA_BEZIERTO),

		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.90f, crInBox.top), FPDFANNOT_PATHDATA_MOVETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.90f, crInBox.top - FPDFANNOTUTIL_BEZIER * fHeight * 0.85f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.90f  - FPDFANNOTUTIL_BEZIER * fWidth * 0.90f, crInBox.top - fHeight*0.85f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.top - fHeight*0.85f), FPDFANNOT_PATHDATA_BEZIERTO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left, crInBox.bottom), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.right, crInBox.bottom), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.right, crInBox.top), FPDFANNOT_PATHDATA_LINETO),
		CFSPDFAnnot_PathData(CFSPDFAnnot_Point(crInBox.left + fWidth*0.90f, crInBox.top), FPDFANNOT_PATHDATA_LINETO),
	};

	csPathData = GetAppStreamFromArray(PathArray, 23);
}