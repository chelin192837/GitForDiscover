#ifndef _FS_ANNOTUTIL_H_
#define _FS_ANNOTUTIL_H_

#include "../fxcore.h"
#include"../fs_util.h"
#include "../../include/fs_annot.h"

class FSPDFAnnotImp;
class FSFormControlImp;

#define FSPDF_ANNOTTYPE_NOTE				"Text"
#define FSPDF_ANNOTTYPE_LINK				"Link"
#define FSPDF_ANNOTTYPE_FREETEXT			"FreeText"
#define FSPDF_ANNOTTYPE_LINE				"Line"
#define FSPDF_ANNOTTYPE_SQUARE				"Square"
#define FSPDF_ANNOTTYPE_CIRCLE				"Circle"
#define FSPDF_ANNOTTYPE_POLYGON				"Polygon"
#define FSPDF_ANNOTTYPE_POLYLINE			"PolyLine"
#define FSPDF_ANNOTTYPE_HIGHLIGHT			"Highlight"
#define FSPDF_ANNOTTYPE_UNDERLINE			"Underline"
#define FSPDF_ANNOTTYPE_SQUIGGLY			"Squiggly"
#define FSPDF_ANNOTTYPE_STRIKEOUT			"StrikeOut"
#define FSPDF_ANNOTTYPE_STAMP				"Stamp"
#define FSPDF_ANNOTTYPE_CARET				"Caret"
#define FSPDF_ANNOTTYPE_INK					"Ink"
#define FSPDF_ANNOTTYPE_FILEATTACHMENT		"FileAttachment"
#define FSPDF_ANNOTTYPE_SOUND				"Sound"
#define FSPDF_ANNOTTYPE_MOVIE				"Movie"
#define FSPDF_ANNOTTYPE_WIDGET				"Widget"
#define FSPDF_ANNOTTYPE_SCREEN				"Screen"
#define FSPDF_ANNOTTYPE_PRINTERMARK			"PrinterMark"
#define FSPDF_ANNOTTYPE_TRAPNET				"TrapNet"
#define FSPDF_ANNOTTYPE_WATERMARK			"Watermark"
#define FSPDF_ANNOTTYPE_PSI					"PSInk"
#define FSPDF_ANNOTTYPE_3D					"3D"
#define FSPDF_ANNOTTYPE_POPUP				"Popup"
// signature type is only used internally
#define FSPDF_ANNOTTYPE_SIGNATURE			"Signature"

//macro  for  constant
#define	FSPDFANNOT_MIN				0.0001f
#define	FSPDFAnnot_Path CFX_ArrayTemplate<CPDF_Point>

FSM_ANNOTTYPE FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(CFX_ByteString bsType);
CFX_ByteString FSANNOT_Util_ConvertAnnotTypeFromEnumToStr(FSM_ANNOTTYPE annotType);
FSM_ANNOTSTATEMODEL FSANNOT_Util_ConvertStateModelToEnum(CFX_ByteString bsModel);
CFX_ByteString FSANNOT_Util_ConvertStateModelFromEnum(FSM_ANNOTSTATEMODEL model);
FSM_ANNOTSTATE FSANNOT_Util_ConvertStateToEnum(CFX_ByteString bsState);
CFX_ByteString FSANNOT_Util_ConvertStateFromEnum(FSM_ANNOTSTATE state);

FS_BOOL FSANNOT_Util_CheckAnnotIsMarkup(FSM_ANNOTTYPE bsType);
FS_BOOL FSANNOT_Util_CheckAnnotIsMarkup(CFX_ByteString bsAnnotType);

FS_BOOL FSANNOT_Util_CheckAnnotIsSupport(CFX_ByteString bsAnnotType);
FS_BOOL FSANNOT_Util_CheckAnnotIsSupport(FSM_ANNOTTYPE annotType);

FS_BOOL FSANNOT_Util_IsBorderInfoValid(FSBorderInfo borderInfo);

FS_BOOL	FSANNOT_Util_ColorToAPStream(FX_ARGB color, FS_BOOL bFill, CFX_ByteTextBuf& bufContent);
void FSANNOT_Util_DashToAppStream(FX_FLOAT fDashOffset, const CFX_FloatArray& dashPattern, CFX_ByteTextBuf& csTemp);

void FSANNOT_Util_BeaierCalcControlPoints(CFX_VectorF vertice[4], CFX_VectorF cp[2], FX_FLOAT k);
void FSANNOT_Util_BezierToStream(const CPDF_Array* pInkList, CFX_ByteTextBuf& bufAp, CPDF_Array* pVerticesList,FX_FLOAT k = 0.5f);
void FSANNOT_Util_NormalizeInkList(const CPDF_Array* pInkList,CPDF_Array* pNormalizeInkList);
void FSANNOT_Util_LineToStream(const CPDF_Array* pInkList, CFX_ByteTextBuf& bufAp);
void FSANNOT_Util_ExpandRectByPoint(CPDF_Point point, CPDF_Rect& crRect);
void FSANNOT_Util_ExpandRectByPoint(FSPointF point, CPDF_Rect& crRect);

FS_BOOL FSANNOT_Util_IsStateValueValid(FSM_ANNOTSTATEMODEL model, FSM_ANNOTSTATE state);

#define FSPDF_GETSIGN(a)			((a) > 0 ? 1 : (a) < 0 ? -1 : 0)
#define FSPDF_FLOAT_MIN			0.001f

#define FSPDF_FDIVVAL(a)			( \
	(FXSYS_fabs(a) < FSPDF_FLOAT_MIN) \
	? (FSPDF_GETSIGN(a) >= 0 ? FSPDF_FLOAT_MIN : (-1)*FSPDF_FLOAT_MIN) \
	: (a))

#define	 FSPDFANNOTUTIL_PI					3.14159265358979f
#define  FSPDFANNOTUTIL_MIN					0.0001f
#define  FSPDFANNOTUTIL_MAXIMUM_DEFLATESIZE	16
#define	 FSPDFANNOTUTIL_BEZIER				0.5522847498308f

struct  FSPDFANNOT_APPPARAM
{
	FSPDFANNOT_APPPARAM() : fOpacity(1.0f), bUseOpacity(FALSE)
	{

	}
	FS_FLOAT		fOpacity;
	FS_BOOL			bUseOpacity;
	CFX_ByteStringC	bsExtGState;
	CFX_ByteStringC	bsBlenMode;
};

class CFS_Vector
{
public:
	CFS_Vector(FX_FLOAT xx = 0.0f, FX_FLOAT yy = 0.0f) 
	{ x = xx; y = yy; }

	FX_FLOAT		x;
	FX_FLOAT		y;

	void			SetValue(FX_FLOAT xx, FX_FLOAT yy)
	{ 
		x = xx; 
		y = yy; 
	}

	FX_FLOAT		Length() const { return x * x + y * y; }

	static FX_BOOL	IsRect(CFS_Vector pts[4]){
		if (pts[0].x != pts[3].x && pts[0].y != pts[3].y)
			return FALSE;
		for (int i = 1; i < 4; i ++) {
			if (pts[i].x != pts[i-1].x && pts[i].y != pts[i-1].y)
				return FALSE;
		}
		return TRUE;
	}

	FX_FLOAT		DotProduct(const CFS_Vector& vector) const
	{ return x * vector.x + y * vector.y; }

	friend FX_FLOAT	_Vector_Cosine(const CFS_Vector& a, const CFS_Vector& b)
	{
		FX_FLOAT dotProduct = a.DotProduct(b);
		return dotProduct / FSPDF_FDIVVAL(a.Length() * b.Length());
	}

	friend FX_FLOAT	_Vector_ArcCosine(const CFS_Vector& a, const CFS_Vector& b) 
	{ return FXSYS_acos(_Vector_Cosine(a, b)); }

	friend FX_FLOAT	_Vector_SlopeAngle(const CFS_Vector& a) 
	{ return _Vector_ArcCosine(a, CFS_Vector(1.0f, 0.0f)); }	

	friend CFS_Vector operator + (const CFS_Vector& p1, const CFS_Vector& p2)
	{
		CFS_Vector p;
		p.x = p1.x + p2.x;
		p.y = p1.y + p2.y;
		return p;
	}

	friend CFS_Vector operator - (const CFS_Vector& p1, const CFS_Vector& p2)
	{
		CFS_Vector p;
		p.x = p1.x - p2.x;
		p.y = p1.y - p2.y;
		return p;
	}

	friend CFS_Vector operator * (const CFS_Vector& point, float t)
	{
		CFS_Vector p;
		p.x = point.x * t;
		p.y = point.y * t;
		return p;
	}

	friend CFS_Vector operator / (const CFS_Vector& p1, const CFS_Vector& p2)
	{
		CFS_Vector p;
		p.x = p1.x / FSPDF_FDIVVAL(p2.x);
		p.y = p1.y / FSPDF_FDIVVAL(p2.y);
		return p;
	}

	friend CFS_Vector operator / (const CFS_Vector& point, float t)
	{
		CFS_Vector p;
		p.x = point.x / FSPDF_FDIVVAL(t);
		p.y = point.y / FSPDF_FDIVVAL(t);
		return p;
	}
};

CPDF_Rect FSAnnot_Util_ClipPageRect(CPDF_Rect rcPage, CPDF_Rect rcChild, FX_BOOL bCut);

CPDF_Dictionary* FSANNOT_UTIL_InsertAnnot(FSM_ANNOTTYPE annotType, const FS_INT32 annotIndex, const FSRectF annotRect, CPDF_Dictionary* pPageDict, CPDF_Document* pDoc, FS_BOOL bRDKTrialWM = FALSE);
FSAnnot* FSANNOT_UTIL_InsertAnnot(int annotIndex, CPDF_Dictionary* pAnnotDict, FSPDFPage* pPage);
void	FSANNOT_UTIL_SetDefaultProperties(FSAnnot* pAnnot);

FX_BOOL FSANNOT_Util_PolygonIsPtIn(FX_FLOAT x, FX_FLOAT y, CFS_Vector* pts, int ptnum);
FX_BOOL FSANNOT_Util_TextMarkupIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, FS_FLOAT tolerance);
CPDF_Rect FSANNOT_Util_DeflateRectangle(CPDF_Rect crRect, FX_FLOAT fDeflateX, FX_FLOAT fDeflateY);
CPDF_Rect FSANNOT_Util_DeflateRectangle(CPDF_Rect crRect, FX_FLOAT fDeflateLeft, FX_FLOAT fDeflateBottom, FX_FLOAT fDeflateRight, FX_FLOAT fDeflateTop);
FX_BOOL FSANNOT_Util_EclipseIsPtIn(FX_FLOAT x, FX_FLOAT y, CPDF_Rect crRect);
FS_INT32 FSANNOT_Util_GetBorderThickness(CPDF_Dictionary* pAnnotDict);
FX_BOOL FSANNOT_Util_SquareCirleIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, FS_FLOAT tolerance);
FX_FLOAT FSANNOT_Util_Length(CPDF_Point& point);
CPDF_Point FSANNOT_Util_Rotate(CPDF_Point& point, double angle);
void FSANNOT_Util_Normalize(CPDF_Point& point);
CPDF_Point FSANNOT_Util_LineGetLLStartingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2);
CPDF_Point FSANNOT_Util_LineGetLLEndingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2);
CPDF_Point FSANNOT_Util_LineGetLLEStartingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2);
CPDF_Point FSANNOT_Util_LineGetLLEEndingPoint(CPDF_Dictionary* pAnnotDict, CPDF_Point p1, CPDF_Point p2);
FX_FLOAT FSANNOT_Util_DistancePointToLine(CPDF_Dictionary* pAnnotDict, CPDF_Point linePoint1, CPDF_Point linePoint2);
FX_BOOL FSANNOT_Util_LineIsPtIn(CPDF_Point& pBegin, CPDF_Point& pEnd, CPDF_Rect& crRect, FX_FLOAT magnifySize, FX_FLOAT fx, FX_FLOAT fy);
FX_BOOL FSANNOT_Util_InkIsPointIn(CPDF_Dictionary* pAnnotDict, FX_FLOAT x, FX_FLOAT y, int* lineindex, FS_FLOAT tolerance);
FX_BOOL FSANNOT_Util_IsPointInRect(CPDF_Dictionary* pAnnotDict, FSM_ANNOTTYPE annotType, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, FS_INT32 nPageRotate, const FSMatrix* pSrcMatrix = NULL);
FX_BOOL	FSANNOT_Util_CheckAnnotIsAtPoint(FSAnnot* pAnnot, FS_FLOAT x, FS_FLOAT y, FS_FLOAT tolerance, const FSMatrix* pSrcMatrix = NULL);

FS_BOOL FSAnnot_Util_ChectAnnotIsReply(CPDF_Dictionary* pAnnotDict);
FS_BOOL FSAnnot_Util_CheckIsStateAnnot(FSAnnot* pAnnot);

FS_BOOL FSANNOT_Util_IsSignatureField(CPDF_Dictionary* pAnnotDict);
FSFormControlImp* FSANNOT_Util_GetFormControlImp(FSAnnot* pAnnot);

FX_BOOL FSAnnot_Util_GetRotateMatrix(CPDF_Dictionary* pPageDict, CPDF_Rect annotRect, CPDF_Matrix& matrix);

//This function would calculate a fixed matrix, which has no magnification or rotation.
//If no icon name, just pass "" to bsIconName.
void FSAnnot_Util_GetFixedMatrix(FX_INT32 rotate, FX_BSTR bsAnnotType, FX_BSTR bsIconName, const CPDF_Rect &annot_rect, const CFX_Matrix* pSrcMatrix, CFX_Matrix& matrix);

FS_BOOL FSAnnot_Util_IsAppearanceValid(CPDF_Dictionary* pAnnotDict);

void FSAnnotUtil_GetUserSpaceLength(long lSize, FX_FLOAT& fSize, FX_FLOAT fUserUnit);
void FSAnnotUtil_GetUserSpaceLength(long xSize, long ySize, FX_FLOAT& fxSize, FX_FLOAT& fySize, FX_FLOAT fUserUnit);
CPDF_Rect FSAnnot_CreateCloudyAP_Rectangle(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect, FS_FLOAT fIntensity, FX_FLOAT fRotateAngle = 0.0f);
CPDF_Rect FSAnnot_CreateCloudyAP_Arc(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect, FX_FLOAT fStartAngle, FX_FLOAT fStopAngle, FX_BOOL bHasM = FALSE);
CPDF_Rect FSAnnot_CreateCloudyAP_Ellipse(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect, FS_FLOAT fIntensity, FX_FLOAT fRotateAngle = 0.0f );
void FSAnnot_CreateAP_Ellipse(CFX_ByteTextBuf& csAP, const CPDF_Rect& crRect);
FX_INT32 FSAnnot_ArcToBezier_Segment(const CFX_RectF &rect, FX_FLOAT fStart, FX_FLOAT fSweep, CFX_PointF bzPoints[4]);

FS_FLOAT FSAnnot_Util_Length(CPDF_Point point);
CPDF_Point FSAnnot_Util_Rotate(CPDF_Point point, double angle);
FS_FLOAT	FSAnnot_Util_SlopeAngle(CPDF_Point pointA);
FS_FLOAT	FSAnnot_Util_SlopeAngle(FSPointF pointA);

FX_FLOAT	FSAnnot_Util_Cosine(CPDF_Point pointA, CPDF_Point pointB = CPDF_Point(1.0f, 0.0f));
FS_FLOAT	FSAnnot_Util_DotProduct(CPDF_Point pointA, CPDF_Point pointB);

FS_INT32	FSAnnot_Util_CalculateCloudyIntensity(CPDF_Rect rcEdit, FS_INT32 nIntensity);

CPDF_Rect FSAnnot_CreateLineEndingAP_None(CFX_ByteString& csAP, CPDF_Point& point, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_Square(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_Circle(CFX_ByteString& csAP, CPDF_Point& point, FS_FLOAT& fRadius);
CPDF_Rect FSAnnot_CreateLineEndingAP_Diamond(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_OpenArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_ClosedArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_Butt(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_ROpenArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_RClosedArrow(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateLineEndingAP_Slash(CFX_ByteString& csAP, CPDF_Point& point, CPDF_Point& direction, FS_FLOAT& fWidth);
CPDF_Rect FSAnnot_CreateAppearanceStream_Rectangle(CFX_ByteString& csAP, CPDF_Rect& crRect, FX_FLOAT fRotateAngle = 0.0f);
CPDF_Rect FSAnnot_CreateAppearanceStream_Ellipse(CFX_ByteString& csAP, CPDF_Rect& crRect, FX_FLOAT fRotateAngle = 0.0f);
CPDF_Rect FSAnnot_CreateCloudyAP_Polygon(CFX_ByteTextBuf& csAP, FSPDFAnnot_Path& ctrlPoints, FS_FLOAT fIntensity);

//*****************************************************************************
//* Make the appearance of annotation.
//*****************************************************************************
class  CFSAnnot_DefaultAppearance : public CPDF_DefaultAppearance
{
public : 
	CFSAnnot_DefaultAppearance( CFX_ByteString csDA = "" );
	CFSAnnot_DefaultAppearance& operator = ( const CFSAnnot_DefaultAppearance& cDA )
	{
		m_csDA = cDA.m_csDA ; 
		return *this ; 
	}
	virtual ~CFSAnnot_DefaultAppearance() {}

	FX_BOOL				HasEntry(FX_BSTR csEntry, FX_INT32 nParamNum ) ; 
	CFX_ByteString		GetEntryString( FX_BSTR csEntry, FX_INT32 nParamNum ) ; 
	CFX_ByteString		GetEntryParam( FX_BSTR csEntry, FX_INT32 nParamNum, FX_INT32 nParamIndex ) ; 
	void				SetEntry( FX_BSTR csEntry, FX_INT32 nParamNum, CFX_ByteStringArray &aParams ) ; 
	void				RemoveEntry( FX_BSTR csEntry, FX_INT32 nParamNum ) ; 
	
	FX_BOOL				HasHorzScale() ; 
	CFX_ByteString		GetHorzScaleString() ; 
	void				GetHorzScale( FX_FLOAT& flScale ) ; 
	void				SetHorzScale( FX_FLOAT  flScale ) ; 
	
	FX_BOOL				HasCharSpace() ; 
	CFX_ByteString		GetCharSpaceString() ; 
	void				GetCharSpace( FX_FLOAT& flSpace ) ; 
	void				SetCharSpace( FX_FLOAT  flSpace ) ; 
	
	FX_BOOL				HasLineLeading() ; 
	CFX_ByteString		GetLineLeadingString() ; 
	void				GetLineLeading( FX_FLOAT& flLeading ) ; 
	void				SetLineLeading( FX_FLOAT  flLeading ) ; 

	void				RemoveColor(FX_BOOL bStrokingOperation);
	void				RemoveTextMatrix();
}; 
#endif
