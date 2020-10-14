//#include "../../include/fs_pdfpage.h"
#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"


FSPDFCircleImp::FSPDFCircleImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFCircleImp::move(FSRectF rect)
{
	return FSPDFAnnotImp::moveImp((FSCircle*)this, rect, TRUE);
}

//////////////
// Methods inherited from FSNote
FS_ARGB FSPDFCircleImp::getFillColor() const
{
	return getColor(TRUE);
}

void FSPDFCircleImp::setFillColor(FS_ARGB fillColor)
{
	setColor(fillColor, TRUE);
}

FSRectF FSPDFCircleImp::getInnerRect() const
{
	CFX_FloatRect fxInnerRect = getAnnotImp().getInnerRectImp();
	FSRectF rectF;
	rectF.set(fxInnerRect.left, fxInnerRect.bottom, fxInnerRect.right, fxInnerRect.top);
	return rectF;
}

void FSPDFCircleImp::setInnerRect(FSRectF innerRect)
{
	setRectMargin(innerRect);
}

FS_BOOL FSPDFCircleImp::resetAppearanceStream()
{
	FSPDFAnnotImp annotImp = getAnnotImp();

	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	param.fOpacity = getOpacity();
	CFX_ByteTextBuf bufContent;
	if (param.fOpacity < 1.0f)
	{
		param.bUseOpacity = TRUE;
		bufContent << FX_BSTRC("/TransGs gs\n");
	}

	FX_ARGB color = 0xFF000000;
	FS_BOOL bHasBorderColor = annotImp.getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, color);
	FSBorderInfo borderInfo = annotImp.getBorderInfo();
	FX_FLOAT fBorderWidth = borderInfo.width;
	FS_BOOL bBorderVisible = bHasBorderColor && (fBorderWidth != 0);
	if (bBorderVisible)
		FSANNOT_Util_ColorToAPStream(color, FALSE, bufContent);
	FX_BOOL bHasFillColor = annotImp.getFillColor(color);
	if (bHasFillColor)
		FSANNOT_Util_ColorToAPStream(color, TRUE, bufContent);
	CPDF_Rect crRect = annotImp.getInnerRectImp();
	// If no color property, no need to generate the main ap stream content for circle.
	if (bBorderVisible || bHasFillColor)
	{
		if (fBorderWidth != 0)
		{
			if(FSAnnot::e_borderStyleCloudy == borderInfo.style)
			{
				bufContent << fBorderWidth << FX_BSTRC(" w 0 J 2 j\n");
			}
			else
			{
				bufContent << fBorderWidth << FX_BSTRC(" w 0 J 0 j\n");

				if (FSAnnot::e_borderStyleDashed == borderInfo.style)
				{
					FX_FLOAT fDashOffset;
					CFX_FloatArray dashPattern;
					annotImp.getBorderDash(fDashOffset, dashPattern);
					FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, bufContent);
				}
			}

			float fx, fy;
			FSAnnotUtil_GetUserSpaceLength((long)fBorderWidth, (long)fBorderWidth, fx, fy, 1.0f / 72.0f);
			if (FSAnnot::e_borderStyleCloudy == borderInfo.style && FLOAT_ISBIGGER(borderInfo.cloudIntensity, 0.0))
			{
				CPDF_Rect crRectangle = FSAnnot_CreateCloudyAP_Ellipse(bufContent, crRect, borderInfo.cloudIntensity);
				crRectangle.Inflate(fx, fy);
				CPDF_Rect crRD = crRectangle;
				crRectangle = crRect;
				crRect = crRD;
				crRD.left = (FX_FLOAT)(crRectangle.left - crRect.left);
				crRD.bottom = (FX_FLOAT)(crRectangle.bottom - crRect.bottom);
				crRD.right = (FX_FLOAT)(crRect.right - crRectangle.right);
				crRD.top = (FX_FLOAT)(crRect.top - crRectangle.top);
				annotImp.setRectImp(FSANNOT_RECTKEY_RD, crRD);
				annotImp.setRectImp(FSANNOT_RECTKEY_RECT, crRect);	}
			else
			{
				CPDF_Rect crRectangle = crRect;
				crRectangle.Deflate(fx * 2.0f / 3.0f, fy * 2.0f / 3.0f);
				FSAnnot_CreateAP_Ellipse(bufContent, crRectangle);

				//pAnnot->SetDifferences(CPDF_Rect());
				CPDF_Rect crRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);
				crRect.Inflate(crRD);
				annotImp.setRectImp(FSANNOT_RECTKEY_RECT, crRect);
			}
			if (bHasFillColor)
				bufContent << (bBorderVisible ? FX_BSTRC("B\n") : FX_BSTRC("f\n"));
			else
				bufContent << FX_BSTRC("S\n");
		}		
	}

	CPDF_Matrix mt = CPDF_Matrix(1, 0, 0, 1, -crRect.left, -crRect.bottom);
	annotImp.writeAppearance(FX_BSTRC("N"), crRect, mt, bufContent, FX_BSTRC(""), &param);

	return TRUE;
}
