#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../../include/common.h"
#include "../inc/commonImp.h"
#include "../form/fs_pdfformImp.h"

FSPDFInkImp::FSPDFInkImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
	:m_pInkList(NULL)
{
	FSPDFMarkupImp::init(pAnnotDict, page);
}

FSPDFInkImp::~FSPDFInkImp()
{
	if (m_pInkList)
	{
		m_pInkList->release();
		m_pInkList = NULL;
	}
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFInkImp::move(FSRectF rect)
{
	if (transformInkList(rect))
		return FSPDFAnnotImp::moveImp((FSInk*)this, rect, TRUE);
	return FALSE;
}

FSPDFPath* FSPDFInkImp::getInkList()
{
	if (!getDict()->hasKey(FSANNOT_ARRAYKEY_INKLIST))
		return NULL;
	if (m_pInkList)
		return m_pInkList;
	CPDF_Array* pInkList = getArray(FSANNOT_ARRAYKEY_INKLIST);
	if (!pInkList)
		return NULL;
	FS_DWORD nPathCount = pInkList->GetCount();
	FS_INT32 nPointCount = 0;
	// Calculate the count of points in all the sub paths  of ink list.
	for (FX_DWORD i = 0; i < nPathCount; i++)
	{
		CPDF_Array* pSubArray = pInkList->GetArray(i);
		if(!pSubArray)
			continue;
		FS_DWORD nSubPtCount = pSubArray->GetCount()/2;
		nPointCount += nSubPtCount;
	}
	m_pInkList = (FSPDFPathImp*)FSPDFPath::create();
	if (m_pInkList)
	{
		m_pInkList->setPointCount(nPointCount);
		FS_INT32 iPtIndex = 0;
		for (FX_DWORD i = 0; i < nPathCount; i++)
		{
			CPDF_Array* pSubArray = pInkList->GetArray(i);
			if(!pSubArray)
				continue;
			
			FX_DWORD nSubPtCount = pSubArray->GetCount()/2;
			if (nSubPtCount > 0)
			{	
				// Record first point of a sub path, with point type "MoveTo".
				FSPointF newPoint;
				newPoint.set(pSubArray->GetFloat(0), pSubArray->GetFloat(1));
				m_pInkList->setPoint(iPtIndex++, newPoint, FSCommonDefines::e_pointTypeMoveTo);
				// Then record rest points of these sub path. Most points should be with point type "BezierTo"
				for(FX_DWORD j = 1; j < nSubPtCount; j++)
				{
					newPoint.set(pSubArray->GetFloat(2*j),pSubArray->GetFloat(2*j + 1));
					m_pInkList->setPoint(iPtIndex++, newPoint, FSCommonDefines::e_pointTypeLineTo);
				}
				
			}
		}
		if (m_pInkList->getPointCount()<1)
		{
			m_pInkList->release();
			m_pInkList = NULL;
		}
	}
	return m_pInkList;
}

void FSPDFInkImp::setInkList(FSPDFPath* inkList)
{
	CPDF_Array* pNewInkList = CPDF_Array::Create();
	if (!pNewInkList)
		return;
	if (!normalizeInkList(inkList))
		return;
	CPDF_Array* pSubArray  = NULL;

	FS_INT32 pointCount = inkList->getPointCount();
	for(FS_INT32 i=0; i < pointCount; i++)
	{
		FS_PATHPOINTTYPE iFxPtType = inkList->getPointType(i);
		if(iFxPtType == FSCommonDefines::e_pointTypeMoveTo)
		{// That means current point is the first point of a new sub path.
			pSubArray = NULL;
			pSubArray = CPDF_Array::Create();
			if (!pSubArray)
			{
				pNewInkList->Release();
				return;
			}
			pNewInkList->Add(pSubArray);
		}
		FSPointF point = inkList->getPoint(i);
		if (pSubArray)
		{
			pSubArray->AddNumber(point.x);
			pSubArray->AddNumber(point.y);
		}
	}
	setArray(FSANNOT_ARRAYKEY_INKLIST, pNewInkList);
    if (m_pInkList != inkList) {
        if (m_pInkList)
        {
            m_pInkList->release();
            m_pInkList = NULL;
        }
        m_pInkList = ((FSPDFPathImp*)inkList)->retain();
    }
}

CPDF_Rect FSInkAnnot_CalculateRectFromInkList(const CPDF_Array* pInkList)
{
	CPDF_Rect crRect(0.0f, 0.0f, 0.0f, 0.0f);
	FX_DWORD nPathCount = pInkList->GetCount();
	for (FX_DWORD i = 0; i<nPathCount; i++)
	{
		CPDF_Array* pPointsArray = pInkList->GetArray(i);
		FX_DWORD nCount = pPointsArray->GetCount() / 2 * 2;
		for (FX_DWORD j = 0; j < nCount; j += 2)
		{
			CPDF_Point point(pPointsArray->GetNumber(j), pPointsArray->GetNumber(j+1));
			if (i == 0 && j == 0)
			{
				crRect.left = crRect.right = point.x;
				crRect.bottom = crRect.top = point.y;
			}
			else
			{
				if (crRect.left > point.x) crRect.left = point.x;
				if (crRect.bottom > point.y) crRect.bottom = point.y;
				if (crRect.right < point.x) crRect.right = point.x;
				if (crRect.top < point.y) crRect.top = point.y;
			}
		}
	}
	return crRect;
}

FS_BOOL FSPDFInkImp::resetAppearanceStream()
{
	CPDF_Array* pInkList = getArray(FSANNOT_ARRAYKEY_INKLIST);
	if (!pInkList || pInkList->GetCount() <= 0) 
		return FALSE;

	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	param.fOpacity = this->getOpacity();
	CFX_ByteTextBuf bufContent;
	if (param.fOpacity < 1.0f)
	{
		param.bUseOpacity = TRUE;
		bufContent << FX_BSTRC("/TransGs gs\n");
	}
	FX_ARGB color = 0;
	CFX_FloatRect crRect = FSInkAnnot_CalculateRectFromInkList(pInkList);
	// If no color property, no need to generate main appearance stream content for ink.
	FX_FLOAT fBorderWidth = getBorderWidth();
	if (getBorderColorImp(color))
	{
		if (fBorderWidth != 0)
		{
			FSANNOT_Util_ColorToAPStream(color, FALSE, bufContent);

			bufContent << fBorderWidth << FX_BSTRC(" w 1 J 1 j\n");
			if (fBorderWidth > 0.001f)
				bufContent << fBorderWidth / (FX_FLOAT)FXSYS_sin(FSPDFANNOTUTIL_PI / 6.0f) << FX_BSTRC(" M\n");
			CPDF_Array* pNormalizeInkList = CPDF_Array::Create();
			CPDF_Array* pVerticesList = CPDF_Array::Create();
			FSANNOT_Util_NormalizeInkList(pInkList, pNormalizeInkList);
			FSANNOT_Util_BezierToStream(pNormalizeInkList, bufContent, pVerticesList,0.8f);
			pNormalizeInkList->Release();
			CFX_FloatRect rectTmp = FSInkAnnot_CalculateRectFromInkList(pVerticesList);
			pVerticesList->Release();

			crRect = rectTmp;
			crRect.Inflate(fBorderWidth / 2, fBorderWidth / 2);	
		}
		
	}
	FX_FLOAT fx = 12.0f, fy = 12.0f;
	if (FXSYS_fabs(crRect.right - crRect.left) < fx)
	{
		FX_FLOAT xCenter = (crRect.left + crRect.right) / 2.0f;
		crRect.left = xCenter - fx / 2.0f;
		crRect.right = xCenter + fx / 2.0f;
	}
	if (FXSYS_fabs(crRect.top - crRect.bottom) < fy)
	{
		FX_FLOAT yCenter = (crRect.bottom + crRect.top) / 2.0f;
		crRect.bottom = yCenter - fy / 2.0f;
		crRect.top = yCenter + fy / 2.0f;
	}
	setRectImp(FSANNOT_RECTKEY_RECT, crRect);

	CFX_Matrix mtAnnot = CPDF_Matrix(1, 0, 0, 1, -crRect.left, -crRect.bottom);
	writeAppearance(FSANNOT_STREAMKEY_NORMALAP, crRect, mtAnnot, bufContent, FX_BSTRC(""), &param);

	return TRUE;
}

FS_BOOL FSPDFInkImp::transformInkList(FSRectF newRect)
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	if (!getDict()) return FALSE;
	CPDF_Matrix matrix;
	if (!annotImp.getTransformMatrix(newRect, matrix))
		return FALSE;

	CPDF_Array* pOld = annotImp.getArray(FSANNOT_ARRAYKEY_INKLIST);
	if (!pOld) return FALSE;
	CPDF_Array* pNew = CPDF_Array::Create();
	if (!pNew) return FALSE;
	FX_INT32 line_count = pOld->GetCount();
	for (FX_INT32 i = 0; i < line_count; i ++)
	{
		CPDF_Array* pOldLine = pOld->GetArray(i);
		if (!pOldLine) continue;
		FX_INT32 ptnum = pOldLine->GetCount() / 2;
		if (ptnum < 1) continue;
		CPDF_Array* pNewLine = CPDF_Array::Create();
		pNew->Add(pNewLine);

		FX_FLOAT x, y;
		for (FX_INT32 j = 0; j < ptnum; j ++)
		{
			x = pOldLine->GetFloat(j * 2);
			y = pOldLine->GetFloat(j * 2 + 1);
			matrix.TransformPoint(x, y);
			pNewLine->AddNumber(x);
			pNewLine->AddNumber(y);
		}
	}
	annotImp.setArray(FSANNOT_ARRAYKEY_INKLIST, pNew);
    
    if (m_pInkList)
    {
        m_pInkList->release();
        m_pInkList = NULL;
    }
    
	return TRUE;
}

FS_BOOL FSPDFInkImp::normalizeInkList(FSPDFPath * inkList)
{
	if (inkList == NULL)
		return FALSE;
	int pointCount = inkList->getPointCount();

	
	FSPDFPath* pNewInkList = FSPDFPath::create();
	if (!pNewInkList) {
		pNewInkList = NULL;
		return FALSE;
	}

	//copy valid point to pNewInkList
	FS_BOOL isValid = FALSE;
	for (FS_INT32 i = 0; i < pointCount; i++)
	{
		FS_PATHPOINTTYPE iFxPtType = inkList->getPointType(i);
		if (iFxPtType == FSCommonDefines::e_pointTypeMoveTo)
		{
			isValid = TRUE;
			if (isValid)
			{
				FSPointF point = inkList->getPoint(i);
				pNewInkList->moveTo(point);
			}

		}
		else
		{
			if (iFxPtType &FSCommonDefines::e_pointTypeBezierTo)
				i = FX_MIN(i + 2, pointCount - 1);
			if (isValid)
			{
				FSPointF point = inkList->getPoint(i);
				pNewInkList->lineTo(point);
			}
		}
	}

	//check where InkList is valid;
	if(pNewInkList->getPointCount()==0)
	{
		pNewInkList->release();
		pNewInkList = NULL;
		return FALSE;
	}

	while (pointCount > 0) {
		inkList->removePoint(--pointCount);
	}

	pointCount = pNewInkList->getPointCount();
	for (int i = 0;i < pointCount;i++) {
		FS_PATHPOINTTYPE iFxPtType = pNewInkList->getPointType(i);
		FSPointF point = pNewInkList->getPoint(i);
		if (iFxPtType == FSCommonDefines::e_pointTypeMoveTo)
		{
			inkList->moveTo(point);
		}
		else
		{
			inkList->lineTo(point);
		}
	}
	pNewInkList->release();
	pNewInkList = NULL;
	return TRUE;
}
