#include "fs_annot_imp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/commonImp.h"
#include "../inc/fs_pdfdocImp.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_wnd.h"
#include "../widget/ifpdf_widget.h"
#include "../widget/pdfwindow/include/fx_systemhandlerimp.h"
#include "../widget/pdfwindow/include/pdfwindow/pwl_edit.h"

FSPDFLineImp::FSPDFLineImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
{ 
	FSPDFMarkupImp::init(pAnnotDict, page); 
}

FS_BOOL			FSPDFLineImp::move(FSRectF rect)
{
	if (transformLinePoints(rect))
		return FSPDFAnnotImp::moveImp((FSLine*)this, rect, TRUE);
	return FALSE;
}
void		FSPDFLineImp::setIntent(const char* intent)
{
	FSPDFMarkupImp::setIntent(intent);
	if (0 == FXSYS_strcmp("LineArrow", intent)) {
		setLineEndingStyles("None", "OpenArrow");
	}
}

FSString	FSPDFLineImp::getLineStartingStyle() const 
{
	CFX_ByteString startStyle, endStyle;
	getAnnotImp().getLineEndingStyles(startStyle, endStyle);
	FSString retStyle(startStyle.GetBuffer(startStyle.GetLength()), startStyle.GetLength());
	startStyle.ReleaseBuffer();
	return retStyle;
}

void		FSPDFLineImp::setLineStartingStyle(const char *startingStyle) 
{
	setLineEndingStyles(startingStyle, getLineEndingStyle().getBuffer());
}

FSString	FSPDFLineImp::getLineEndingStyle() const
{
	CFX_ByteString startStyle, endStyle;
	getAnnotImp().getLineEndingStyles(startStyle, endStyle);
	FSString retStyle(endStyle.GetBuffer(endStyle.GetLength()), endStyle.GetLength());
	endStyle.ReleaseBuffer();
	return retStyle;
}

void		FSPDFLineImp::setLineEndingStyle(const char *endingStyle)
{
	getAnnotImp().setLineEndingStyles(getLineStartingStyle().getBuffer(), endingStyle);
}

FS_ARGB		FSPDFLineImp::getStyleFillColor() const
{	
	FS_ARGB color = 0x00000000;
	getFillColor(color);
	return color;
}

void		FSPDFLineImp::setStyleFillColor(FS_ARGB color)
{
	setFillColor(color);
}

FSPointF	FSPDFLineImp::getStartPoint() const 
{
	FSPointF startPoint;
	startPoint.set(0, 0);
	CPDF_Array* pArray = getArray(FSANNOT_ARRAYKEY_LINEPOINTS);
	if (pArray != NULL)
	{
		FX_DWORD dwCount = pArray->GetCount();
		if (dwCount != 4)
			return startPoint;

		startPoint.x = pArray->GetNumber(0);
		startPoint.y = pArray->GetNumber(1);
	}
	return startPoint;
}

void		FSPDFLineImp::setStartPoint(FSPointF point) 
{
	CPDF_Array* pArray =  CPDF_Array::Create();
	if (!pArray) return; 
	FSPointF endPoint = getEndPoint();

	pArray->AddNumber(point.x);
	pArray->AddNumber(point.y);
	pArray->AddNumber(endPoint.x);
	pArray->AddNumber(endPoint.y);
	setArray(FSANNOT_ARRAYKEY_LINEPOINTS, pArray);
}

FSPointF	FSPDFLineImp::getEndPoint() const 
{
	FSPointF endPoint;
	endPoint.set(0, 0);
	CPDF_Array* pArray = getArray(FSANNOT_ARRAYKEY_LINEPOINTS);
	if (pArray != NULL)
	{
		FX_DWORD dwCount = pArray->GetCount();
		if (dwCount != 4)
			return endPoint;
		
		endPoint.x = pArray->GetNumber(2);
		endPoint.y = pArray->GetNumber(3);
	}
	return endPoint;
}

void		FSPDFLineImp::setEndPoint(FSPointF point) 
{
	CPDF_Array* pArray =  CPDF_Array::Create();
	if (!pArray) return; 
	FSPointF startPoint = getStartPoint();

	pArray->AddNumber(startPoint.x);
	pArray->AddNumber(startPoint.y);
	pArray->AddNumber(point.x);
	pArray->AddNumber(point.y);

	setArray(FSANNOT_ARRAYKEY_LINEPOINTS, pArray);
}

FS_BOOL		FSPDFLineImp::hasCaption() const 
{
	if(!hasProperty(FSANNOT_BOOLEANKEY_CAP))
		return FALSE;
	return getBoolean(FSANNOT_BOOLEANKEY_CAP);
}

void		FSPDFLineImp::enableCaption(FS_BOOL cap) 
{
	setBoolean(FSANNOT_BOOLEANKEY_CAP, cap);
}

FSString	FSPDFLineImp::getCaptionPositionType() const 
{
	//CP is meaningful only when Cap exists and true.
	//BUT CP can exist separated no matter it's meaningful or not.
	return getName(FSANNOT_NAMEKEY_CAPPOSITIONING);
}

void		FSPDFLineImp::setCaptionPositionType(const char* captionPos) 
{
	//CP is meaningful only when Cap exists and true.
	//BUT CP can be set separated no matter it will be meaningful or not.
	setName(FSANNOT_NAMEKEY_CAPPOSITIONING, captionPos);
}

FSOffset	FSPDFLineImp::getCaptionOffset() const 
{
	FSOffset offset;
	offset.set(0, 0);
	if(!hasProperty(FSANNOT_ARRAYKEY_CAPOFFSET))
		return offset;
	CPDF_Array* pArray = getArray(FSANNOT_ARRAYKEY_CAPOFFSET);
	if(!pArray || pArray->GetCount() != 2)
		return offset;

	offset.x = pArray->GetNumber(0);
	offset.y = pArray->GetNumber(1);
	return offset;
}

void		FSPDFLineImp::setCaptionOffset(FSOffset offset) 
{
	CPDF_Array* pArray = CPDF_Array::Create();
	if(!pArray)
		return;
	pArray->AddNumber(offset.x);
	pArray->AddNumber(offset.y);
	setArray(FSANNOT_ARRAYKEY_CAPOFFSET, pArray);
}

//////////////
// Methods for resetAp
void		FSAnnot_Util_GetUserSpaceLength(long lSize, FX_FLOAT& fSize, FX_FLOAT fUserUnit)
{
	int x = 96;
	int y = 96;
	fSize = (FX_FLOAT)lSize / (FX_FLOAT)FX_MAX(x, y) / fUserUnit;
}

void		FSAnnot_Util_GetUserSpaceLength(long xSize, long ySize, FX_FLOAT& fxSize, FX_FLOAT& fySize, FX_FLOAT fUserUnit)
{
	int x = 96;
	int y = 96;

	fxSize = (FX_FLOAT)xSize / (FX_FLOAT)x / fUserUnit;
	fySize = (FX_FLOAT)ySize / (FX_FLOAT)y / fUserUnit;

}

CPDF_Rect	FSAnnot_Util_RotateRectangle(CPDF_Rect crRect, FX_FLOAT fRotateAngle)
{
	CPDF_Point centerPoint((crRect.left + crRect.right) / 2.0f, (crRect.bottom + crRect.top) / 2.0f);
	CPDF_Point point0(0.0f,0.0f),point1(0.0f,0.0f),point2(0.0f,0.0f),point3(0.0f,0.0f);
	point0 = CPDF_Point(crRect.left, crRect.top);
	point1 = CPDF_Point(crRect.left, crRect.bottom);
	point2 = CPDF_Point(crRect.right, crRect.bottom);
	point3 = CPDF_Point(crRect.right, crRect.top);
	CPDF_Point v(0.0f,0.0f);
	CPDF_Rect newRect;
	newRect.left = newRect.right = centerPoint.x;
	newRect.bottom = newRect.top = centerPoint.y;

	for (int i = 0; i < 4; i ++)
	{

		CPDF_Point tempPoint = CPDF_Point(0.0f,0.0f);
		if(i == 0) tempPoint = point0;
		else if(i == 1) tempPoint = point1;
		else if(i == 2) tempPoint = point2;
		else tempPoint = point3;

		v.x = tempPoint.x - centerPoint.x; v.y = tempPoint.y - centerPoint.y;
		v = FSAnnot_Util_Rotate(v,fRotateAngle);
		tempPoint.x = v.x + centerPoint.x; tempPoint.y = v.y + centerPoint.y;

		if (newRect.left > tempPoint.x) newRect.left = tempPoint.x;
		if (newRect.bottom > tempPoint.y) newRect.bottom = tempPoint.y;
		if (newRect.right < tempPoint.x) newRect.right = tempPoint.x;
		if (newRect.top < tempPoint.y) newRect.top = tempPoint.y;
	}

	return newRect;
}

//////////////
// resetAp
FS_BOOL		FSPDFLineImp::resetAppearanceStream()
{
	FSPDFAnnotImp annotImp = getAnnotImp();
	if (!annotImp.hasProperty(FSANNOT_ARRAYKEY_LINEPOINTS)) return FALSE;
	FSPointF p1 = getStartPoint();
	FSPointF p2 = getEndPoint();
	CPDF_Point p3 = getLLPoint(TRUE); 
	CPDF_Point p4 = getLLPoint(FALSE);
	CPDF_Point p5 = getLLEPoint(TRUE);
	CPDF_Point p6 = getLLEPoint(FALSE);
	CFX_ByteTextBuf byteTextBuf;
	CFX_ByteString csTemp;

	// Re-calculate rectangle.
	CPDF_Rect crRect;
	crRect.left = crRect.right = p1.x;
	crRect.bottom = crRect.top = p1.y;
	FSANNOT_Util_ExpandRectByPoint(p2, crRect);
	FSANNOT_Util_ExpandRectByPoint(p3, crRect);
	FSANNOT_Util_ExpandRectByPoint(p4, crRect);
	FSANNOT_Util_ExpandRectByPoint(p5, crRect);
	FSANNOT_Util_ExpandRectByPoint(p6, crRect);
	crRect.Normalize();
	FS_ARGB dwColor = 0;
	FS_BOOL bHasBorderColor = annotImp.getBorderColorImp(dwColor);
	//If no color property, no need to generate the main appearance stream content for line.
	FX_FLOAT fBorderWidth = annotImp.getBorderWidth();
	// Fill color to AP stream
	FX_BOOL bBorderVisible = bHasBorderColor&&(fBorderWidth!=0);
	FS_ARGB dwIC;
	FS_BOOL bHasIC = annotImp.getFillColor(dwIC);

	CPDF_Point pointBegin = p3;
	CPDF_Point pointEnd = p4;

	//
	FSPointF v1, v2;
	v1.x = pointBegin.x - pointEnd.x;
	v1.y = pointBegin.y - pointEnd.y;
	v2.x = pointEnd.x - pointBegin.x;
	v2.y = pointEnd.y - pointBegin.y;

	if (bBorderVisible ||bHasIC)
	{
		
		if (bHasIC)
			FSANNOT_Util_ColorToAPStream(dwIC, TRUE, byteTextBuf);
		
		if (bBorderVisible)
		{
			
			// Border color to AP stream
			FSANNOT_Util_ColorToAPStream(dwColor, FALSE, byteTextBuf);
			// Border width to AP stream
			csTemp.Format("%.3f w\n0 J\n0 j\n", fBorderWidth);
			byteTextBuf << csTemp;

			// Border style "Dashed" to AP stream
			FSM_BORDERSTYLE borderStyle = annotImp.getBorderStyle();
			if (FSAnnot::e_borderStyleDashed == borderStyle)
			{
				FX_FLOAT fDashOffset;
				CFX_FloatArray dashPattern;
				annotImp.getBorderDash(fDashOffset, dashPattern);
				FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, byteTextBuf);
			}
			// Opacity to AP stream
			FX_FLOAT fOpacity = getOpacity();
			CFX_ByteString csGSResourceName = "TransGs";
			if (fOpacity != 1.0f)
			{
				csTemp.Empty();
				csTemp.Format("/%s gs\n", (FX_LPCSTR)csGSResourceName);
				byteTextBuf << csTemp;
			}
		}
			// Line leading related
			if (annotImp.hasProperty(FSANNOT_FLOATKEY_LL) || annotImp.hasProperty(FSANNOT_FLOATKEY_LLE))
			{
				FX_FLOAT fll = (FX_FLOAT)FXSYS_fabs(annotImp.getFloat(FSANNOT_FLOATKEY_LL));
				FX_FLOAT flle = (FX_FLOAT)FXSYS_fabs(annotImp.getFloat(FSANNOT_FLOATKEY_LLE));
				if (fll >= 0.5f || flle >= 0.5f)
				{
					csTemp.Empty();
					csTemp.Format("%.3f %.3f m\n", p1.x, p1.y);
					byteTextBuf <<csTemp;

					csTemp.Empty();
					csTemp.Format("%.3f %.3f l\n", p5.x, p5.y);
					byteTextBuf <<csTemp;

					csTemp.Empty();
					csTemp.Format("%.3f %.3f m\n", p2.x, p2.y);
					byteTextBuf <<csTemp;

					csTemp.Empty();
					csTemp.Format("%.3f %.3f l\n", p6.x, p6.y);
					byteTextBuf <<csTemp;
				}
			}
			
			if (bBorderVisible)
			{
				csTemp.Empty();
				csTemp.Format("%.3f %.3f m\n",pointBegin.x, pointBegin.y);
				byteTextBuf << csTemp;
				csTemp.Empty();
				csTemp.Format("%.3f %.3f l\n", pointEnd.x, pointEnd.y);
				byteTextBuf << csTemp;
				byteTextBuf << "S\n";
			}
			
			//
			FX_FLOAT fWidth;
			long iWidth = (long)fBorderWidth;
			if (iWidth < 1) iWidth = 1;
			iWidth += 3;
			FSAnnot_Util_GetUserSpaceLength(iWidth, fWidth, (1.0f / 72.0f));
			fWidth *= 2.5f;

			

			FSPointF startPoint, endPoint;
			startPoint.set(pointBegin.x, pointBegin.y);
			endPoint.set(pointEnd.x, pointEnd.y);

			FS_BOOL bNeedFill = FALSE;
			csTemp = "";
			// LE of start point.	
			CPDF_Rect rcStarting = annotImp.getLineEndingStyleStream(TRUE, csTemp, startPoint, v1, fWidth, bNeedFill);
			if (csTemp.IsEmpty() == FALSE)
			{
				if (bNeedFill == TRUE)
				{
					if (bHasIC)
						csTemp += ((bBorderVisible)?"B\n": "f\n");
					else
						csTemp += "h\nS\n";
				}
				else
					csTemp += ((bBorderVisible)?"S\n": "");
			}
			byteTextBuf << csTemp;

			bNeedFill = FALSE;
			csTemp = "";
			// LE of end point.
			CPDF_Rect rcEnding = annotImp.getLineEndingStyleStream(FALSE, csTemp, endPoint, v2, fWidth, bNeedFill);
			if (csTemp.IsEmpty() == FALSE)
			{
				if (bNeedFill == TRUE)
				{
					if (bHasIC)
						csTemp += ((bBorderVisible)?"B\n": "f\n");
					else
						csTemp += "h\nS\n";
				}
				else
					csTemp += ((bBorderVisible)?"S\n": "");
			}
			byteTextBuf << csTemp;

			//
			CPDF_Rect cr1, cr2;
			FX_FLOAT fx, fy;

			FSString strStartingStyle = getLineStartingStyle();
			FSString strEndingStyle = getLineEndingStyle();
			
			FSAnnot_Util_GetUserSpaceLength(iWidth, iWidth, fx, fy, (1.0f /72.0f));
			crRect = FSANNOT_Util_DeflateRectangle(crRect, -fx, -fy);
			rcStarting = FSANNOT_Util_DeflateRectangle(rcStarting, -fx, -fy);
			rcEnding = FSANNOT_Util_DeflateRectangle(rcEnding, -fx, -fy);
			FSAnnot_Util_GetUserSpaceLength(FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE, FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE, fx, fy, (1.0f /72.0f));
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
			
			if (0 != FXSYS_strcmp(strStartingStyle.getBuffer(), "None")) 
				crRect.Union(rcStarting);
			if (0 != FXSYS_strcmp(strEndingStyle.getBuffer(), "None"))
				crRect.Union(rcEnding);
			annotImp.setRectImp(FSANNOT_RECTKEY_RECT, crRect);		
	}
	//Cap, CP, CO
	CPDF_Font* pFont = NULL;
	if (hasCaption())
	{// /Cap true
		CFX_WideString csContents = annotImp.getUnicodeString(FSANNOT_STRINGKEY_CONTENT);
		if (!csContents.IsEmpty())
		{
			CPDF_Rect editRect(0.0f, 0.0f, 0.0f, 0.0f);

			IFSPDF_Edit* pEdit = IFSPDF_Edit::Create();
			CFSPDF_WidgetProperties widgetProp;
			widgetProp.m_rtWidget = editRect;
			//phantom flags = PWS_VISIBLE | PES_MULTILINE | PES_MIDDLE | PES_CENTER;
			widgetProp.m_dwStyleExes = FPDFW_STYLEEXT_EDT_MultiLine | FPDFW_STYLEEXT_EDT_HCenter | FPDFW_STYLEEXT_EDT_VCenter;
			widgetProp.m_sBackgroundColor = 0xff000000;// phantom: CPWL_Color();
			widgetProp.m_fFontSize = 14;
			if (dwColor)
			{
				FS_INT32 iAlpha, iRed, iGreen, iBlue;
				iAlpha = iRed = iGreen = iBlue = 255;
				ArgbDecode((dwColor ^ 0x00A5A5A5), iAlpha, iRed, iGreen, iBlue);
				widgetProp.m_sTextColor = ArgbEncode(iAlpha, (255 - iRed), (255 - iGreen), (255 - iBlue));
			}
			widgetProp.m_pSystemHandler = FX_NEW FX_SystemHandlerImp(NULL);
			if (FPDFW_ERR_Succeeded == pEdit->Initialize((FSLine*)this, &widgetProp))
			{
				pEdit->Update();
				pEdit->SetCharSpace(0.0f);
				pEdit->SetHorzScale(100);
				pEdit->SetText(csContents);
				pEdit->GetContentRect(editRect);

				v1.x = pointEnd.x - pointBegin.x; v1.y = pointEnd.y - pointBegin.y;
				FX_FLOAT angle = 0.0f;
				if (!(0 == v1.x && 0 == v1.y))
					angle = FSAnnot_Util_SlopeAngle(v1);
				if (v1.y < 0.0f) angle *= -1.0f;
				FX_FLOAT xCenter = (pointBegin.x + pointEnd.x) / 2.0f;
				FX_FLOAT yCenter = (pointBegin.y + pointEnd.y) / 2.0f;
				FSOffset offset = getCaptionOffset();

				CFX_WideString wsCapPosition = getUnicodeString(FSANNOT_NAMEKEY_CAPPOSITIONING);
				if (wsCapPosition.Equal(FX_WSTRC(L"Top")))
				{
					xCenter -= (FXSYS_fabs(editRect.bottom - editRect.top) / 2 * FXSYS_sin(angle));
					yCenter += (FXSYS_fabs(editRect.bottom - editRect.top) / 2 * FXSYS_cos(angle));
				}

				xCenter += (offset.x * FXSYS_cos(angle) - offset.y * FXSYS_sin(angle));
				yCenter += (offset.y * FXSYS_cos(angle) + offset.x * FXSYS_sin(angle));
				editRect.left += xCenter;
				editRect.right += xCenter;
				editRect.top += yCenter;
				editRect.bottom += yCenter;

				CFX_ByteString csTemp;
				csTemp.Format("%.3f %.3f %.3f %.3f %.3f %.3f cm\n", FXSYS_cos(angle), FXSYS_sin(angle), -FXSYS_sin(angle), FXSYS_cos(angle), xCenter, yCenter);
				byteTextBuf << csTemp;
				csTemp.Empty();
				csTemp = pEdit->GetTextAppearanceStream(CPDF_Point(0.0f, 0.0f));
				byteTextBuf << csTemp;
				editRect = FSAnnot_Util_RotateRectangle(editRect, angle);
				crRect.Union(editRect);
				crRect.Normalize();
				annotImp.setRectImp(FSANNOT_RECTKEY_RECT, crRect);
			}
			if (widgetProp.m_pSystemHandler)
				widgetProp.m_pSystemHandler->Release();
			pEdit->Release();
		}

	}
	FSPDFANNOT_APPPARAM param;
	param.bsExtGState = FX_BSTRC("TransGs");
	param.bsBlenMode = ""; 
	param.fOpacity = getOpacity();
	param.bUseOpacity = TRUE;
	annotImp.setRectImp(FSANNOT_RECTKEY_RECT,crRect);
	CFX_Matrix mt = CFX_Matrix(1, 0, 0, 1, -crRect.left, -crRect.bottom);
	annotImp.writeAppearance(FSANNOT_STREAMKEY_NORMALAP, crRect, mt, byteTextBuf, FX_BSTRC(""), &param);
	
	return TRUE;
}

CPDF_Point FSPDFLineImp::getLLPoint(FS_BOOL isStartPoint)
{
	FSPointF p1 = getStartPoint();
	FSPointF p2 = getEndPoint();
	CPDF_Point p3 = CPDF_Point(0.0f,0.0f);
	p3.x = p2.x - p1.x;
	p3.y = p2.y - p1.y;
	if (FSAnnot_Util_Length(p3)>0.0f ) 
		FSANNOT_Util_Normalize(p3);

	FX_FLOAT ll = 0.0f;
	if (hasProperty(FSANNOT_FLOATKEY_LL)) 
		ll = getFloat(FSANNOT_FLOATKEY_LL);
	if (ll >= 0.0f)
		p3 = FSANNOT_Util_Rotate(p3, FX_PI / 2.0f);
	else
		p3 = FSANNOT_Util_Rotate(p3,-FX_PI / 2.0f);
	p3.x *= (FX_FLOAT)FXSYS_fabs(ll);
	p3.y *= (FX_FLOAT)FXSYS_fabs(ll);
	CPDF_Point p4 = CPDF_Point(p1.x + p3.x,p1.y + p3.y); // LL start point
	if (FALSE == isStartPoint)
		p4 = CPDF_Point(p2.x + p3.x,p2.y + p3.y); // LL end point
	return p4;
}

CPDF_Point FSPDFLineImp::getLLEPoint(FS_BOOL isStartPoint)
{
	FSPointF p1 = getStartPoint();
	FSPointF p2 = getEndPoint();
	CPDF_Point p3 = CPDF_Point(0.0f,0.0f);
	p3.x = p2.x - p1.x;
	p3.y = p2.y - p1.y;
	if (FSAnnot_Util_Length(p3)>0.0f ) 
		FSANNOT_Util_Normalize(p3);

	FX_FLOAT ll = 0.0f;
	if (hasProperty(FSANNOT_FLOATKEY_LL)) 
		ll = getFloat(FSANNOT_FLOATKEY_LL);
	FX_FLOAT lle = 0.0f;
	if (hasProperty(FSANNOT_FLOATKEY_LLE)) 
		lle = getFloat(FSANNOT_FLOATKEY_LLE);
	if (ll >= 0.0f)
		p3 = FSANNOT_Util_Rotate(p3,FX_PI / 2.0f);
	else
		p3 = FSANNOT_Util_Rotate(p3,-FX_PI / 2.0f);

	p3.x *= (lle + (FX_FLOAT)FXSYS_fabs(ll));
	p3.y *= (lle + (FX_FLOAT)FXSYS_fabs(ll));
	CPDF_Point p4 = CPDF_Point(p1.x + p3.x,p1.y + p3.y);// LLE start point
	if (FALSE ==isStartPoint)
		p4 = CPDF_Point(p2.x + p3.x,p2.y + p3.y);// LLE end point
	return p4;
}

FS_BOOL FSPDFLineImp::transformLinePoints(FSRectF newRect)
{	
	CPDF_Matrix matrix;
	if (!getTransformMatrix(newRect, matrix))
		return FALSE;

	FSPointF startPoint = getStartPoint();
	FSPointF endPoint = getEndPoint();
	matrix.TransformPoint(startPoint.x, startPoint.y);
	matrix.TransformPoint(endPoint.x,endPoint.y);
	setStartPoint(startPoint);
	setEndPoint(endPoint);
	return TRUE;
}
