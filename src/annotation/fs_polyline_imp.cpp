#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/commonImp.h"
#include "../inc/fs_pdfdocImp.h"
#include "../inc/fpdf_define.h"

FSPDFPolyLineImp::FSPDFPolyLineImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}
//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFPolyLineImp::move(FSRectF rect)
{
	if (transformVertices(rect))
		return FSPDFAnnotImp::moveImp((FSPolyLine*)this, rect, TRUE);
	return FALSE;
}

//////////////
// 
FS_BOOL		FSPDFPolyLineImp::resetAppearanceStream()
{
	FSPDFAnnot_Path* pPath = getVerticesImp();
	if (pPath == NULL || pPath->GetSize() < 2)
	{
		return FALSE;
	}

	CFX_ByteString csTemp;//csAP, 
	CFX_ByteTextBuf byteTextBuf;

	FX_ARGB dwColor;
	CPDF_Rect crRect;
	FS_BOOL bHasBorderColor = getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, dwColor);
	FX_FLOAT fBorderThickness = getBorderWidth();
	FS_BOOL bBorderVisible = fBorderThickness != 0 && bHasBorderColor;
	if (bBorderVisible)
	{
		FSANNOT_Util_ColorToAPStream(dwColor, FALSE, byteTextBuf);
	}
		
	FX_BOOL bHasIC = getColorImpl(FSANNOT_COLORKEY_FILLCOLOR, dwColor);
	if(bHasIC)
		FSANNOT_Util_ColorToAPStream(dwColor, TRUE, byteTextBuf);

	if (bBorderVisible || bHasIC)
	{
		csTemp.Format("%.3f w\n0 J\n0 j\n", fBorderThickness);
		byteTextBuf << csTemp;

		FX_INT32 borderStyle = getBorderStyle();

		if (FSAnnot::e_borderStyleDashed == borderStyle)
		{
			FX_FLOAT fDashOffset;
			CFX_FloatArray dashPattern;
			getBorderDash(fDashOffset, dashPattern);
			FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, byteTextBuf);
		}


		FX_FLOAT fOpacity = getOpacity();
		CFX_ByteString csGSResourceName = "TransGs";

		if (fOpacity != 1.0f)
		{
			csTemp.Empty();
			csTemp.Format("/%s gs\n", (FX_LPCSTR)csGSResourceName);
			byteTextBuf << csTemp;
		}

		CPDF_Point point1(0.0f,0.0f),point2(0.0f,0.0f),point3(0.0f,0.0f),point4(0.0f,0.0f);
		CPDF_Point fp = (*pPath)[0];
		point1 = point2 = point3 = point4 = fp;
		crRect.left = crRect.right = fp.x;
		crRect.bottom = crRect.top = fp.y;
		csTemp.Empty();
		csTemp.Format("%.3f %.3f m\n", fp.x, fp.y);
		byteTextBuf << csTemp;
		FX_DWORD dwCount = pPath->GetSize();
		for (FX_DWORD i = 1; i < dwCount; i ++)
		{
			fp = (*pPath)[i];
			if (i == 1) point2 = fp;
			if (i == (dwCount - 2)) point3 = fp;
			if (i == (dwCount - 1)) point4 = fp;
			if (crRect.left > fp.x) crRect.left = fp.x;
			if (crRect.bottom > fp.y) crRect.bottom = fp.y;
			if (crRect.right < fp.x) crRect.right = fp.x;
			if (crRect.top < fp.y) crRect.top = fp.y;
			if (bBorderVisible)
			{
				csTemp.Empty();
				csTemp.Format("%.3f %.3f l\n", fp.x, fp.y);
				byteTextBuf << csTemp;
			}
			
		}
		byteTextBuf << "S\n";

		FX_FLOAT fWidth;
		long iWidth = (long)fBorderThickness;
		if (iWidth < 1) iWidth = 1;
		iWidth += 3;
		FSAnnotUtil_GetUserSpaceLength(iWidth, fWidth, (1.0f / 72.0f));
		fWidth *= 5.0f;

		CPDF_Rect cr1, cr2;

		FSPointF v1, v2;
		v1.x = point1.x - point2.x;
		v1.y = point1.y - point2.y;
		v2.x = point4.x - point3.x;
		v2.y = point4.y - point3.y;

		FSPointF startPoint, endPoint;
		startPoint.set(point1.x, point1.y);
		endPoint.set(point4.x, point4.y);

		FS_BOOL bNeedFill = FALSE;
		csTemp = "";
		// LE of start point.
		CPDF_Rect rcStarting = getLineEndingStyleStream(TRUE, csTemp, startPoint, v1, fWidth, bNeedFill);
		if (csTemp.IsEmpty() == FALSE)
		{
			if (bNeedFill == TRUE)
			{
				if (bHasIC)
					csTemp += (bBorderVisible ? "B\n": "f\n");
				else
					csTemp += "h\nS\n";
			}
			else
				csTemp += (bBorderVisible ? "S\n": "");
		}
		byteTextBuf << csTemp;

		bNeedFill = FALSE;
		csTemp = "";
		// LE of end point.
		CPDF_Rect rcEnding = getLineEndingStyleStream(FALSE, csTemp, endPoint, v2, fWidth, bNeedFill);
		if (csTemp.IsEmpty() == FALSE)
		{
			if (bNeedFill == TRUE)
			{
				if (bHasIC)
					csTemp += (bBorderVisible ? "B\n": "f\n");
				else
					csTemp += "h\nS\n";
			}
			else
				csTemp += (bBorderVisible ? "S\n": "");
		}
		byteTextBuf << csTemp;

		FSString strStartingStyle = getLineStartingStyle();
		FSString strEndingStyle = getLineEndingStyle();
		FX_FLOAT fx, fy;
		FSAnnotUtil_GetUserSpaceLength(iWidth, iWidth, fx, fy, (1.0f / 72.0f));
		crRect = FSANNOT_Util_DeflateRectangle(crRect, -fx, -fy);
		rcStarting = FSANNOT_Util_DeflateRectangle(rcStarting, -fx, -fy);
		rcEnding = FSANNOT_Util_DeflateRectangle(rcEnding, -fx, -fy);
		FSAnnotUtil_GetUserSpaceLength(FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE, FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE, fx, fy, (1.0f / 72.0f));
		if (fabs(crRect.right - crRect.left) < fx)
		{
			FX_FLOAT xCenter = (crRect.left + crRect.right) / 2.0f;
			crRect.left = xCenter - fx / 2.0f;
			crRect.right = xCenter + fx / 2.0f;
		}
		if (fabs(crRect.top - crRect.bottom) < fy)
		{
			FX_FLOAT yCenter = (crRect.bottom + crRect.top) / 2.0f;
			crRect.bottom = yCenter - fy / 2.0f;
			crRect.top = yCenter + fy / 2.0f;
		}
		if (0 != FXSYS_strcmp(strStartingStyle.getBuffer(), "None")) 
			crRect.Union(rcStarting);
		if (0 != FXSYS_strcmp(strEndingStyle.getBuffer(), "None")) 
			crRect.Union(rcEnding);
		setRectImp(FSANNOT_RECTKEY_RECT, crRect);
	}

	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	param.bsBlenMode = ""; 
	param.fOpacity = ((FSPDFLineImp*)this)->getOpacity();
	param.bUseOpacity = TRUE;
	setRectImp(FSANNOT_RECTKEY_RECT, crRect);
	CFX_Matrix mt = CFX_Matrix(1, 0, 0, 1, -crRect.left, -crRect.bottom);
	writeAppearance(FSANNOT_STREAMKEY_NORMALAP, crRect, mt, byteTextBuf, FX_BSTRC(""), &param);

	if (pPath) {
		pPath->RemoveAll();
		delete pPath;
	}

	return TRUE;
}

FS_ARGB FSPDFPolyLineImp::getStyleFillColor() const
{
	return getColor(TRUE);
}

void FSPDFPolyLineImp::setStyleFillColor(FS_ARGB fillColor)
{
	setColor(fillColor, TRUE);
}

FS_INT32 FSPDFPolyLineImp::getVertexCount()
{
	return getAnnotImp().getVertexCount();
}

FSPointF FSPDFPolyLineImp::getVertex(FS_INT32 index)
{
	return getAnnotImp().getVertex(index);
}

void FSPDFPolyLineImp::setVertexes(const FSPointF* vertexes, FS_INT32 count)
{
	getAnnotImp().setVertexes(vertexes, count);
}

FSString	FSPDFPolyLineImp::getLineStartingStyle() const
{
	CFX_ByteString startStyle, endStyle;
	getAnnotImp().getLineEndingStyles(startStyle, endStyle);
	FSString retStyle(startStyle.GetBuffer(startStyle.GetLength()), startStyle.GetLength());
	startStyle.ReleaseBuffer();
	return retStyle;
}

void		FSPDFPolyLineImp::setLineStartingStyle(const char *startingStyle)
{
	getAnnotImp().setLineEndingStyles(startingStyle, getLineEndingStyle().getBuffer());
}

FSString	FSPDFPolyLineImp::getLineEndingStyle() const
{
	CFX_ByteString startStyle, endStyle;
	getAnnotImp().getLineEndingStyles(startStyle, endStyle);
	FSString retStyle(endStyle.GetBuffer(endStyle.GetLength()), endStyle.GetLength());
	endStyle.ReleaseBuffer();
	return retStyle;
}

void		FSPDFPolyLineImp::setLineEndingStyle(const char *endingStyle)
{
	getAnnotImp().setLineEndingStyles(getLineStartingStyle().getBuffer(), endingStyle);
}
