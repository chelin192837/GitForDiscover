#include "fs_annot_imp.h"

static FS_FLOAT FSPDFAnnot_TextMarkup_GetLineWidth(FS_FLOAT x[4], FS_FLOAT y[4])
{
#if 0
	CFX_VectorF vt;
	vt.Set(x[3] - x[0], y[3] - y[0]);
	return vt.Length() / 8;
#else
	FS_FLOAT fLW = (FS_FLOAT)FXSYS_fabs((y[3] - y[1]) / 8);
	if (fLW < 1.0f) fLW = 1.0f;
	return fLW;
#endif
}
/////////////////////////////////////////////////
// Class FSPDFTextMarkupImp
FSPDFTextMarkupImp::FSPDFTextMarkupImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	init(pAnnotDict, page);
}

void	FSPDFTextMarkupImp::init(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFMarkupImp::init(pAnnotDict, page);
}


//////////////
// Methods inherited from FSPDFTextMarkup
FS_INT32 FSPDFTextMarkupImp::getQuadPointsCount() const
{
	return FSPDFAnnotImp::getQuadPointsCount();
}

FSQuadPoints FSPDFTextMarkupImp::getQuadPoints(FS_INT32 index) const
{
	return FSPDFAnnotImp::getQuadpoints(index);
}

void FSPDFTextMarkupImp::setQuadPoints(const FSQuadPoints* quadPoints, FS_INT32 count)
{
	FSPDFAnnotImp::setQuadPoints(quadPoints, count);
}

FS_BOOL FSPDFTextMarkupImp::initParam(FX_BSTR bsBM, FSPDFANNOT_APPPARAM & param, CFX_ByteTextBuf & bufContent)
{
	param.bsExtGState = FX_BSTRC("TransGs");
	param.bsBlenMode = bsBM;
	param.fOpacity = getOpacity();
	if (!bsBM.IsEmpty() || param.fOpacity<1.0f)
	{
		bufContent << FX_BSTRC("/TransGs gs\n");
		param.bUseOpacity = TRUE;
	}
	return TRUE;
}

FS_BOOL FSPDFTextMarkupImp::setColorToAPStream(FS_BOOL bFill, CFX_ByteTextBuf & bufContent)
{
	FS_ARGB color = 0xFF000000;
	FS_BOOL bHasBorderColor = getAnnotImp().getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, color);
	if (bHasBorderColor)
		return FSANNOT_Util_ColorToAPStream(color, bFill, bufContent);
	else 
		return FALSE;
}

FS_BOOL	FSPDFTextMarkupImp::setQuadPointsToAPStream(FSM_ANNOTTYPE annotType, CFX_ByteTextBuf& bufContent)
{
	FS_INT32 iQuadPointsCount = getQuadPointsCount();
	if (iQuadPointsCount<1) return FALSE;
	for (FS_INT32 i=0; i<iQuadPointsCount; i++)
	{
		FSQuadPoints quadPoints;
		quadPoints = getQuadPoints(i);
		switch(annotType)
		{
		case FSAnnot::e_annotHighlight:
			((FSPDFHighlightImp*)(FSHighlight*)(FSTextMarkup*)this)->getQuadPointsAPStream(quadPoints, bufContent);
			break;
		case FSAnnot::e_annotUnderline:
			((FSPDFUnderlineImp*)(FSUnderline*)(FSTextMarkup*)this)->getQuadPointsAPStream(quadPoints, bufContent);
			break;
		case FSAnnot::e_annotSquiggly:
			((FSPDFSquigglyImp*)(FSSquiggly*)(FSTextMarkup*)this)->getQuadPointsAPStream(quadPoints, bufContent);
			break;
		case FSAnnot::e_annotStrikeOut:
			((FSPDFStrikeOutImp*)(FSStrikeOut*)(FSTextMarkup*)this)->getQuadPointsAPStream(quadPoints, bufContent);
			break;
		}
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Class FSPDFHighlightImp

FSPDFHighlightImp::FSPDFHighlightImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFTextMarkupImp::init(pAnnotDict, page);
}
//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFHighlightImp::move(FSRectF rect)
{
	if (transformQuadPoints(rect))
		return FSPDFAnnotImp::moveImp((FSHighlight*)this, rect, TRUE);
	return FALSE;
}

// Methods from FSPDFHighlight
FS_BOOL FSPDFHighlightImp::resetAppearanceStream()
{
	if (FALSE == hasProperty(FSANNOT_ARRAYKEY_QUADPOINTS))
		return FALSE;
	CFX_ByteTextBuf bufContent;
	FSPDFANNOT_APPPARAM param;
	initParam("Multiply", param, bufContent);	
	// If no color property, no need to generate AP stream content for quadpoints.
	if(setColorToAPStream(TRUE,bufContent))
	{
		if(!setQuadPointsToAPStream(FSAnnot::e_annotHighlight, bufContent)) 
			return FALSE;
	}
	CFX_FloatRect rcRect = getPDFDict()->GetRect(FSANNOT_RECTKEY_RECT);
	CFX_Matrix matrix(1, 0, 0, 1, -rcRect.left, -rcRect.bottom);
	return writeAppearance(FX_BSTRC("N"), rcRect, matrix, bufContent, FX_BSTRC(""), &param) ? TRUE : FALSE;
}

FS_BOOL FSPDFHighlightImp::getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf& bufContent)
{
	bufContent << quadPoints.first.x << FX_BSTRC(" ") << quadPoints.first.y;
	bufContent << FX_BSTRC(" m ");
	bufContent << quadPoints.second.x << FX_BSTRC(" ") << quadPoints.second.y;
	bufContent << FX_BSTRC(" l ");
	bufContent << quadPoints.fourth.x << FX_BSTRC(" ") << quadPoints.fourth.y;
	bufContent << FX_BSTRC(" l ");
	bufContent << quadPoints.third.x << FX_BSTRC(" ") << quadPoints.third.y;
	bufContent << FX_BSTRC(" l ");
	bufContent << FX_BSTRC("h f\n");

	return TRUE;
}

/////////////////////////////////////////
//Class FSPDFUnderlineImp
FSPDFUnderlineImp::FSPDFUnderlineImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFTextMarkupImp::init(pAnnotDict, page);
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFUnderlineImp::move(FSRectF rect)
{
	if (transformQuadPoints(rect))
		return FSPDFAnnotImp::moveImp((FSUnderline*)this, rect, TRUE);
	return FALSE;
}
/////////////
// Methods from FSPDFUnderline
FS_BOOL FSPDFUnderlineImp::resetAppearanceStream()
{
	if (FALSE == hasProperty(FSANNOT_ARRAYKEY_QUADPOINTS))
		return FALSE;
	CFX_ByteTextBuf bufContent;
	FSPDFANNOT_APPPARAM param;
	initParam("", param, bufContent);
	// If no color property, no need to generate AP stream content for quadpoints.
	if(setColorToAPStream(FALSE,bufContent))
	{
		if(!setQuadPointsToAPStream(FSAnnot::e_annotUnderline, bufContent)) 
			return FALSE;
	}
	CFX_FloatRect rcRect = getPDFDict()->GetRect(FSANNOT_RECTKEY_RECT);
	CFX_Matrix matrix(1, 0, 0, 1, -rcRect.left, -rcRect.bottom);
	return writeAppearance(FX_BSTRC("N"), rcRect, matrix, bufContent, FX_BSTRC(""), &param) ? TRUE : FALSE;
}

FS_BOOL FSPDFUnderlineImp::getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf& bufContent)
{
	CFX_ByteString csTemp;
	FS_FLOAT x[4], y[4];
	x[0] = quadPoints.first.x;y[0] = quadPoints.first.y;
	x[1] = quadPoints.second.x;y[1] = quadPoints.second.y;
	x[2] = quadPoints.third.x;y[2] = quadPoints.third.y;
	x[3] = quadPoints.fourth.x;y[3] = quadPoints.fourth.y;
	FS_FLOAT fLW = FSPDFAnnot_TextMarkup_GetLineWidth(x, y);
	FS_FLOAT fX1 = x[0] + (x[2] - x[0]) / 16;
	FS_FLOAT fY1 = y[2] + (y[0] - y[2]) / 16;
	FS_FLOAT fX2 = x[1] + (x[3] - x[1]) / 16;
	FS_FLOAT fY2 = y[3] + (y[1] - y[3]) / 16;
	csTemp.Format("%.3f w %.3f %.3f m %.3f %.3f l S\n", fLW, fX1, fY1, fX2, fY2);
	bufContent << csTemp;
	return TRUE;
}

/////////////////////////////////////////
//Class FSPDFStrikeOutImp
FSPDFStrikeOutImp::FSPDFStrikeOutImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFTextMarkupImp::init(pAnnotDict, page);
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFStrikeOutImp::move(FSRectF rect)
{
	if (transformQuadPoints(rect))
		return FSPDFAnnotImp::moveImp((FSStrikeOut*)this, rect, TRUE);
	return FALSE;
}

/////////////
// Methods from FSPDFStrikeOut
FS_BOOL FSPDFStrikeOutImp::resetAppearanceStream()
{
	if (FALSE == hasProperty(FSANNOT_ARRAYKEY_QUADPOINTS))
		return FALSE;
	CFX_ByteTextBuf bufContent;
	FSPDFANNOT_APPPARAM param;
	initParam("", param, bufContent);
	// If no color property, no need to generate AP stream content for quadpoints.
	if(setColorToAPStream(FALSE,bufContent))
	{
		if(!setQuadPointsToAPStream(FSAnnot::e_annotStrikeOut, bufContent)) 
			return FALSE;
	}

	CFX_FloatRect rcRect = getPDFDict()->GetRect(FSANNOT_RECTKEY_RECT);
	CFX_Matrix matrix(1, 0, 0, 1, -rcRect.left, -rcRect.bottom);
	return writeAppearance(FX_BSTRC("N"), rcRect, matrix, bufContent, FX_BSTRC(""), &param) ? TRUE : FALSE;
}

FS_BOOL FSPDFStrikeOutImp::getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf& bufContent)
{
	CFX_ByteString csTemp;
	FS_FLOAT x[4], y[4];
	x[0] = quadPoints.first.x;y[0] = quadPoints.first.y;
	x[1] = quadPoints.second.x;y[1] = quadPoints.second.y;
	x[2] = quadPoints.third.x;y[2] = quadPoints.third.y;
	x[3] = quadPoints.fourth.x;y[3] = quadPoints.fourth.y;
	FS_FLOAT fLW = FSPDFAnnot_TextMarkup_GetLineWidth(x, y);
	FS_FLOAT fX1 = (quadPoints.third.x + quadPoints.first.x) / 2;
	FS_FLOAT fY1 = (quadPoints.third.y + quadPoints.first.y) / 2;
	FS_FLOAT fX2 = (quadPoints.fourth.x + quadPoints.second.x) / 2;
	FS_FLOAT fY2 = (quadPoints.fourth.y + quadPoints.second.y) / 2;
	csTemp.Format("%.3f w %.3f %.3f m %.3f %.3f l S\n", fLW, fX1, fY1, fX2, fY2);
	bufContent << csTemp;

	return TRUE;
}

/////////////////////////////////////////
//Class FSPDFSquigglyImp
FSPDFSquigglyImp::FSPDFSquigglyImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{
	FSPDFTextMarkupImp::init(pAnnotDict, page);
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFSquigglyImp::move(FSRectF rect)
{
	if (transformQuadPoints(rect))
		return FSPDFAnnotImp::moveImp((FSSquiggly*)this, rect, TRUE);
	return FALSE;
}

/////////////
// Methods from FSPDFSquiggly
FS_BOOL FSPDFSquigglyImp::resetAppearanceStream()
{
	if (FALSE == hasProperty(FSANNOT_ARRAYKEY_QUADPOINTS))
		return FALSE;
	CFX_ByteTextBuf bufContent;
	FSPDFANNOT_APPPARAM param;
	initParam("", param, bufContent);
	//If no color property, no need to generate AP stream content for quadpoints.
	if (setColorToAPStream(FALSE,bufContent))
	{
		if (!setQuadPointsToAPStream(FSAnnot::e_annotSquiggly, bufContent))
			return FALSE;
	}
	CFX_FloatRect rcRect = getPDFDict()->GetRect(FSANNOT_RECTKEY_RECT);
	CFX_Matrix matrix(1, 0, 0, 1, -rcRect.left, -rcRect.bottom);
	return writeAppearance(FX_BSTRC("N"), rcRect, matrix, bufContent, FX_BSTRC(""), &param) ? TRUE : FALSE;
}

FS_BOOL FSPDFSquigglyImp::getQuadPointsAPStream(FSQuadPoints quadPoints, CFX_ByteTextBuf& bufContent)
{
	CFX_ByteString csTemp;
	FS_FLOAT x[4], y[4];
	FS_BOOL bRightOrder = quadPoints.first.y < quadPoints.third.y ? TRUE : FALSE;
	x[0] = quadPoints.first.x;
	x[1] = quadPoints.second.x;
	x[2] = quadPoints.third.x;
	x[3] = quadPoints.fourth.x;
	if (bRightOrder)
	{
		y[0] = quadPoints.fourth.y;
		y[1] = quadPoints.third.y;
		y[2] = quadPoints.second.y;
		y[3] = quadPoints.first.y;
	}
	else
	{
		y[0] = quadPoints.first.y;
		y[1] = quadPoints.second.y;
		y[2] = quadPoints.third.y;
		y[3] = quadPoints.fourth.y;
	}

	FS_FLOAT fLW = FSPDFAnnot_TextMarkup_GetLineWidth(x, y);
	FS_FLOAT fX1 = x[0] + (x[2] - x[0]) / 16;
	FS_FLOAT fY1 = y[2] + (y[0] - y[2]) / 16;
	FS_FLOAT fX2 = x[1] + (x[3] - x[1]) / 16;
	FS_FLOAT fY2 = y[3] + (y[1] - y[3]) / 16;
	FS_FLOAT fStepX = (fX2 - fX1) / 8;
	FS_FLOAT fStepY = (fY2 - fY1) / 8;

	CFX_VectorF v, v1, v2;
	v.Set(fStepX, fStepY);
	if (v.Length() <= 0.001f) return FALSE;
	v.Normalize();
	v *= fLW;
	if ((v.x <= 0.001f&&v.x >= -0.001f) && (v.y <= 0.001f&&v.y >= -0.001)) return FALSE;
	v1 = v2 = v;
	v1.Rotate(FSPDFANNOTUTIL_PI / 2.0);
	v2.Rotate(-FSPDFANNOTUTIL_PI / 2.0);

	csTemp.Format("%.3f w %.3f %.3f m ", fLW, fX1 + v1.x, fY1 + v1.y);
	bufContent << csTemp;

	FS_INT32 i = 1;
	FS_FLOAT fX = fX1, fY = fY1;
	FS_FLOAT fMinX = FX_MIN(fX1, fX2), fMaxX = FX_MAX(fX1, fX2);
	FS_FLOAT fMinY = FX_MIN(fY1, fY2), fMaxY = FX_MAX(fY1, fY2);
	while ((fX >= fMinX && fX <= fMaxX) && (fY >= fMinY && fY <= fMaxY))
	{
		fX += v.x * 2.0f;
		fY += v.y * 2.0f;
		csTemp.Empty();
		csTemp.Format("%.3f %.3f l ", fX + ((i % 2) == 0 ? v1.x : v2.x), fY + ((i % 2) == 0 ? v1.y : v2.y));
		bufContent << csTemp;
		i++;
	}
	bufContent << FX_BSTRC("S\n");

	return TRUE;
}
