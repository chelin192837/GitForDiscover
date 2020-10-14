//#include "../../include/fs_pdfpage.h"
#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"


FSPDFCaretImp::FSPDFCaretImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFCaretImp::move(FSRectF rect)
{
	return FSPDFAnnotImp::moveImp((FSCaret*)this, rect, TRUE);
}
FS_BOOL FSPDFCaretImp::resetAppearanceStream()
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	if (!annotImp.hasProperty(FSANNOT_RECTKEY_RECT))
		return FALSE;
	CPDF_Rect pdfRect = annotImp.getRectImp(FSANNOT_RECTKEY_RECT);
	CPDF_Rect rdRec = annotImp.getRectImp(FSANNOT_RECTKEY_RD);
	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	param.fOpacity = getOpacity();
	CFX_ByteTextBuf bufContent;
	if (param.fOpacity < 1.0f)
	{
		param.bUseOpacity = TRUE;
		bufContent << FX_BSTRC("/TransGs gs\n");
	}
	// If no color property, no need to generate the main appearance stream content for caret.
	FS_ARGB borderColor = 0;
	if (annotImp.getBorderColorImp(borderColor))
	{
		FSANNOT_Util_ColorToAPStream(borderColor, FALSE, bufContent);
		// Please refer to following url about the calculation for the 3 points for drawing caret:
		// http://confluence.foxitsoftware.com:8090/pages/viewpage.action?pageId=10262099
		FSRectF innerRect = getInnerRect();
		FS_FLOAT innerHeight = innerRect.top - innerRect.bottom;
		FS_FLOAT innerWidth = innerRect.right - innerRect.left;

		FX_FLOAT fLineWidth = innerWidth/5;
		FS_FLOAT fFactor = innerHeight/innerWidth;
		
		FS_FLOAT yDelta = FXSYS_sqrt(((fLineWidth/2)*(fLineWidth/2))/(1+fFactor*fFactor));
		FS_FLOAT xDelta = fFactor*yDelta;

		FSPointF leftBottom;
		leftBottom.x = 0 + xDelta;
		leftBottom.y = 0 + yDelta;
		
		FSPointF rightBottom;
		rightBottom.x = innerWidth - xDelta;
		rightBottom.y = leftBottom.y;

		FSPointF top;
		top.x = (innerWidth)/2;
		if (fFactor<1)
		{
			FS_FLOAT fTemp = (innerWidth/2)/(innerHeight+innerWidth/2);
			top.y = innerHeight - (innerHeight*fTemp)/2;
		}
		else
			top.y = innerHeight - fLineWidth*(fFactor);

		CFX_ByteString bsTemp;
		bsTemp.Format("%f w\n%f %f m\n%f %f l\n%f %f l S\n", 
						fLineWidth,
						leftBottom.x, leftBottom.y,
						top.x, top.y,
						rightBottom.x, rightBottom.y);

		bufContent<<(FX_LPCSTR)bsTemp;
	}

	CPDF_Matrix mt = getRotateMatrix(getPDFDict()->GetInteger("Rotate", 0), pdfRect);
	mt.e += rdRec.right; 
	mt.f += rdRec.top;

	CPDF_Rect crRect(-rdRec.left, -rdRec.bottom, pdfRect.right-pdfRect.left-rdRec.right, pdfRect.top-pdfRect.bottom-rdRec.top);
	annotImp.writeAppearance(FX_BSTRC("N"), crRect, mt, bufContent, FX_BSTRC(""), &param);

	return TRUE;
}

FSRectF FSPDFCaretImp::getInnerRect() const
{
	CFX_FloatRect fxInnerRect =  getAnnotImp().getInnerRectImp();
	FSRectF rectF;
	rectF.set(fxInnerRect.left, fxInnerRect.bottom, fxInnerRect.right, fxInnerRect.top);
	return rectF;
}

void FSPDFCaretImp::setInnerRect(FSRectF innerRect)
{
	setRectMargin(innerRect);
}