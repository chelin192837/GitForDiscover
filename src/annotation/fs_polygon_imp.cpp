#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/commonImp.h"
#include "../inc/fs_pdfdocImp.h"
#include "../inc/fpdf_define.h"


FSPDFPolygonImp::FSPDFPolygonImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}
//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFPolygonImp::move(FSRectF rect)
{
	if (transformVertices(rect))
		return FSPDFAnnotImp::moveImp((FSPolygon*)this, rect, TRUE);
	return FALSE;
}

FS_BOOL		FSPDFPolygonImp::resetAppearanceStream()
{   
	FSPDFAnnot_Path* pVertices = getVerticesImp();
	if (pVertices == NULL || pVertices->GetSize() < 2)
	{
		return FALSE;
	}

	CFX_ByteString csTemp;

	CFX_ByteTextBuf byteTextBuf;

	FX_ARGB dwColor;
	FS_BOOL bHasBorderColor = getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, dwColor);
	FX_FLOAT fBorderThickness = getBorderWidth();
	FS_BOOL bBorderVisible = (bHasBorderColor && fBorderThickness != 0.0f);
	if (bBorderVisible)
		FSANNOT_Util_ColorToAPStream(dwColor, FALSE, byteTextBuf);

	FX_ARGB dwIC;
	FX_BOOL bHasIC = getColorImpl(FSANNOT_COLORKEY_FILLCOLOR, dwIC);
	if(bHasIC)
		FSANNOT_Util_ColorToAPStream(dwIC, TRUE, byteTextBuf);

	CPDF_Rect crRect;
	FX_INT32 borderStyle = getBorderStyle();
	FS_FLOAT fIntensity = 0;

	if (bBorderVisible) {
		if (FSAnnot::e_borderStyleCloudy == borderStyle)
		{//Not support fully?? by Yvonne
			fIntensity = FSPDFAnnotImp::getCloudyIntensity();
			csTemp.Empty();
			csTemp.Format("%f w\n0 J\n2 j\n", fBorderThickness);
			byteTextBuf << csTemp;
		}
		else
		{
			borderStyle = getBorderStyle();
			csTemp.Empty();
			csTemp.Format("%f w\n0 J\n0 j\n", fBorderThickness);
			byteTextBuf << csTemp;

			if (FSAnnot::e_borderStyleDashed == borderStyle)
			{
				FX_FLOAT fDashOffset;
				CFX_FloatArray dashPattern;
				getBorderDash(fDashOffset, dashPattern);
				FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, byteTextBuf);
			}
		}
	}

	if (bBorderVisible || bHasIC)
	{

		FX_FLOAT fOpacity = getOpacity();
		CFX_ByteString csGSResourceName = "TransGs";

		if (fOpacity != 1.0f)
		{
			csTemp.Empty();
			csTemp.Format("/%s gs\n", (FX_LPCSTR)csGSResourceName);
			byteTextBuf << csTemp;
		}

		FX_FLOAT fx, fy;
		FX_FLOAT fWidth = fBorderThickness;
		FSAnnotUtil_GetUserSpaceLength((int)fWidth, (int)fWidth, fx, fy,(1.0f / 72.0f));
		if (FSAnnot::e_borderStyleCloudy == borderStyle && FLOAT_ISBIGGER(fIntensity, 0.0))
		{
			CPDF_Rect crPolygon = FSAnnot_CreateCloudyAP_Polygon(byteTextBuf, *pVertices, fIntensity);
			crRect = FSANNOT_Util_DeflateRectangle(crPolygon, -fx, -fy);
			setRectImp(FSANNOT_RECTKEY_RECT, crRect);
		}
		else
		{
			CPDF_Point fp = (*pVertices)[0];
			crRect.left = crRect.right = fp.x;
			crRect.bottom = crRect.top = fp.y;
			csTemp.Empty();
			csTemp.Format("%.3f %.3f m\n", fp.x, fp.y);
			byteTextBuf << csTemp;
			for (FX_INT32 i = 1; i < pVertices->GetSize(); i ++)
			{
				fp = (*pVertices)[i];
				if (crRect.left > fp.x) crRect.left = fp.x;
				if (crRect.bottom > fp.y) crRect.bottom = fp.y;
				if (crRect.right < fp.x) crRect.right = fp.x;
				if (crRect.top < fp.y) crRect.top = fp.y;
				csTemp.Empty();
				csTemp.Format("%.3f %.3f l\n", fp.x, fp.y);
				byteTextBuf << csTemp;
			}
			byteTextBuf << "h\n";

			fWidth += 3;
			crRect = FSANNOT_Util_DeflateRectangle(crRect, -fx, -fy);
			FSAnnotUtil_GetUserSpaceLength(FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE, FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE, fx, fy,(1.0f / 72.0f));
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
			setRectImp(FSANNOT_RECTKEY_RECT, crRect);
		}
		if (!bHasIC) {
			byteTextBuf << "S\n";
		}
		else
		{
			byteTextBuf << (bBorderVisible ?"B\n": "f\n");
		}
	}

	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	param.bsBlenMode = ""; 
	param.fOpacity = getOpacity();
	param.bUseOpacity = TRUE;
	setRectImp(FSANNOT_RECTKEY_RECT,crRect);
	CFX_Matrix mt = CFX_Matrix(1, 0, 0, 1, -crRect.left, -crRect.bottom);
	writeAppearance(FSANNOT_STREAMKEY_NORMALAP, crRect, mt, byteTextBuf, FX_BSTRC(""), &param);

	if (pVertices) {
		for ( FS_INT32 i = 0; i < pVertices->GetSize(); i ++)
		{
			pVertices->RemoveAt(i);		
		}
		delete pVertices;
	}
	
	return TRUE;
}

void		FSPDFPolygonImp::setIntent(const char* intent)
{
	FSPDFMarkupImp::setIntent(intent);
	if (0 == FXSYS_strcmp("PolygonCloud", intent)) {
		FSBorderInfo borderInfo = getBorderInfo();
		borderInfo.style = FSAnnot::e_borderStyleCloudy;
		borderInfo.cloudIntensity = 2.0;
		setBorderInfo(borderInfo);
	}
}

FS_ARGB FSPDFPolygonImp::getFillColor() const
{
	return getColor(TRUE);
}

void FSPDFPolygonImp::setFillColor(FS_ARGB fillColor)
{
	setColor(fillColor, TRUE);
}

FS_INT32 FSPDFPolygonImp::getVertexCount()
{
	return getAnnotImp().getVertexCount();
}

FSPointF FSPDFPolygonImp::getVertex(FS_INT32 index)
{	
	return getAnnotImp().getVertex(index);
}

void FSPDFPolygonImp::setVertexes(const FSPointF* vertexes, FS_INT32 count)
{	
	getAnnotImp().setVertexes(vertexes, count);	
}
