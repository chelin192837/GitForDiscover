//#include "../../include/fs_pdfpage.h"
#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"


FSPDFSquareImp::FSPDFSquareImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFSquareImp::move(FSRectF rect)
{
	return FSPDFAnnotImp::moveImp((FSSquare*)this, rect, TRUE);
}

//////////////
// Methods inherited from FSNote
FS_ARGB FSPDFSquareImp::getFillColor() const
{
	return getColor(TRUE);
}

void FSPDFSquareImp::setFillColor(FS_ARGB fillColor)
{
	setColor(fillColor, TRUE);
}

FSRectF FSPDFSquareImp::getInnerRect() const
{
	CFX_FloatRect fxInnerRect = getAnnotImp().getInnerRectImp();
	FSRectF rectF;
	rectF.set(fxInnerRect.left, fxInnerRect.bottom, fxInnerRect.right, fxInnerRect.top);
	return rectF;
}

void FSPDFSquareImp::setInnerRect(FSRectF innerRect)
{
	setRectMargin(innerRect);
}

FS_BOOL FSPDFSquareImp::resetAppearanceStream()
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
	FX_BOOL bFill = annotImp.getFillColor(color);
	if (bFill)
		FSANNOT_Util_ColorToAPStream(color, TRUE, bufContent);
	CPDF_Rect crRect = annotImp.getInnerRectImp();
	// If no color property, no need to generate the main appearance content for square.
	if (bBorderVisible || bFill)
	{
		if (fBorderWidth != 0)
		{
			FSM_BORDERSTYLE borderStyle = borderInfo.style;
			if(borderInfo.style == FSAnnot::e_borderStyleCloudy)
			{
				bufContent << fBorderWidth << FX_BSTRC(" w 0 J 2 j\n");
			}
			else
			{
				bufContent << fBorderWidth << FX_BSTRC(" w 0 J 0 j\n");

				if (borderInfo.style == FSAnnot::e_borderStyleDashed)
				{
					FX_FLOAT fDashOffset;
					CFX_FloatArray dashPattern;
					annotImp.getBorderDash(fDashOffset, dashPattern);
					FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, bufContent);
				}
			}

			float fx, fy;
			FSAnnotUtil_GetUserSpaceLength((long)fBorderWidth, (long)fBorderWidth, fx, fy, 1.0f/72.0f);
			if (borderInfo.style == FSAnnot::e_borderStyleCloudy && FLOAT_ISBIGGER(borderInfo.cloudIntensity, 0.0))
			{
				CPDF_Rect crRectangle = FSAnnot_CreateCloudyAP_Rectangle(bufContent, crRect, borderInfo.cloudIntensity);
				crRectangle.Inflate(fx, fy);
				CPDF_Rect crRD = crRectangle;
				crRectangle = crRect;
				crRect = crRD;
				crRD.left = (FX_FLOAT)(crRectangle.left - crRect.left);
				crRD.bottom = (FX_FLOAT)(crRectangle.bottom - crRect.bottom);
				crRD.right = (FX_FLOAT)(crRect.right - crRectangle.right);
				crRD.top = (FX_FLOAT)(crRect.top - crRectangle.top);
				annotImp.setRectImp(FSANNOT_RECTKEY_RD, crRD);
				annotImp.setRectImp(FSANNOT_RECTKEY_RECT, crRect);
			}
			else
			{
				CPDF_Rect crRectangle = crRect;
				crRectangle.Deflate(fx * 2.0f / 3.0f, fy * 2.0f / 3.0f);
				CFX_ByteString bsTemp;
				bsTemp.Format("%.3f %.3f %.3f %.3f re\n", crRectangle.left, crRectangle.bottom, crRectangle.Width() ,crRectangle.Height());
				bufContent << bsTemp;

				//pAnnot->SetDifferences(CPDF_Rect());
				CPDF_Rect crRD = annotImp.getRectImp(FSANNOT_RECTKEY_RD);
				crRect.Inflate(crRD);
				annotImp.setRectImp(FSANNOT_RECTKEY_RECT, crRect);
			}
			if (bFill)
				bufContent << (bBorderVisible ? FX_BSTRC("B\n") : FX_BSTRC("f\n"));
			else
				bufContent << FX_BSTRC("S\n");
		}
	}

	CPDF_Matrix mt = CPDF_Matrix(1, 0, 0, 1, -crRect.left, -crRect.bottom);
	annotImp.writeAppearance(FX_BSTRC("N"), crRect, mt, bufContent, FX_BSTRC(""), &param);

	return TRUE;
}
