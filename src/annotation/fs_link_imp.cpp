#include "../../include/common.h"
#include "../../include/fs_annot.h"
#include "../fs_util.h"
#include "fs_annot_imp.h"
#include "../inc/fs_actionImp.h"
#include "../inc/fs_pdfdocImp.h"

FSPDFLinkImp::FSPDFLinkImp(CPDF_Dictionary* pAnnotDict, FSPDFPage* page)
	:m_pLinkAct(NULL)
{
	FSPDFAnnotImp::init(pAnnotDict, page); 
}

FSPDFLinkImp::~FSPDFLinkImp()
{
	if (m_pLinkAct)
	{
		m_pLinkAct->release();
		m_pLinkAct = NULL;
	}
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL			FSPDFLinkImp::move(FSRectF rect)
{
	// link may not have /QuadPoints -- it's OK.
	// So no need to just if transformQuadPoints() succeeds.
	transformQuadPoints(rect);
	return FSPDFAnnotImp::moveImp((FSLink*)this, rect, TRUE);
}


///////////////////////////////

FS_BOOL				FSPDFLinkImp::resetAppearanceStream()
{
	FSBorderInfo borderInfo = getBorderInfo();
	FS_INT32 iBorderStyle = borderInfo.style;
	if (FSAnnot::e_borderStyleBeveled == borderInfo.style || FSAnnot::e_borderStyleCloudy == borderInfo.style || FSAnnot::e_borderStyleInset == borderInfo.style)
		iBorderStyle = FSAnnot::e_borderStyleSolid;
	FSRectF annotRect = getRect();
	CFX_FloatRect rtAnnot(annotRect.left, annotRect.bottom, annotRect.right, annotRect.top);
	CFX_ByteTextBuf csTemp;
	FS_ARGB color = 0;
	FS_BOOL bHasBorderColor = getBorderColorImp(color);
	if (FXSYS_fabs(borderInfo.width) >= 0.001f && bHasBorderColor/* && FXARGB_A(color) > 0*/)
	{	// borderwidth > 0 and alpha of color is not 0x00: it means a visible AP
		csTemp << FX_BSTRC("q\n");
		FSANNOT_Util_ColorToAPStream(color, FALSE, csTemp);
		csTemp << borderInfo.width << FX_BSTRC(" w\n");
		csTemp << FX_BSTRC("0 J\n0 j\n");

		CFX_PointsF points;
		FX_BOOL bRet = getQuadPointsImpl(points);
		FX_INT32 iSize = points.GetSize();
		if (iSize > 0 && 0 == iSize % 4)
		{

			CFX_PointF fp = points.GetAt(0);
			rtAnnot.left = rtAnnot.right = fp.x;
			rtAnnot.bottom = rtAnnot.top = fp.y;

			for (FX_INT32 i = 1; i < iSize; i ++)
			{
				fp = points.GetAt(i);
				if (rtAnnot.left > fp.x) rtAnnot.left = fp.x;
				if (rtAnnot.bottom > fp.y) rtAnnot.bottom = fp.y;
				if (rtAnnot.right < fp.x) rtAnnot.right = fp.x;
				if (rtAnnot.top < fp.y) rtAnnot.top = fp.y;
			}
			rtAnnot.left -= borderInfo.width, rtAnnot.right += borderInfo.width;
			rtAnnot.bottom -= borderInfo.width, rtAnnot.top += borderInfo.width;
			setRectImp(FSANNOT_RECTKEY_RECT, rtAnnot);
			if (e_borderStyleUnderLine == iBorderStyle)
			{
				csTemp << FX_BSTRC(" ") << points[0].x << FX_BSTRC(" ") << points[0].y << FX_BSTRC(" m\n");
				csTemp << FX_BSTRC(" ") << points[1].x << FX_BSTRC(" ") << points[1].y << FX_BSTRC(" l\n");
				csTemp << FX_BSTRC("S\n");
			}
			else
			{
				if (e_borderStyleDashed == iBorderStyle)
				{
					FX_FLOAT fDashOffset;
					CFX_FloatArray dashPattern;
					getBorderDash(fDashOffset, dashPattern);
					FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, csTemp);
				}

				for (FX_INT32 i = 0; i < points.GetSize(); i ++)
				{
					csTemp << FX_BSTRC(" ")<< points[i].x << FX_BSTRC(" ") << points[i].y;
					if ((i % 4) == 0)
						csTemp << FX_BSTRC(" m\n");
					else
						csTemp << FX_BSTRC(" l\n");
					if ((i % 4) == 3)
						csTemp << FX_BSTRC("h S\n");
				}
			}
		}
		else
		{
			if (e_borderStyleUnderLine == iBorderStyle)
			{
				csTemp << FX_BSTRC(" ") << rtAnnot.left << FX_BSTRC(" ") << rtAnnot.bottom << FX_BSTRC(" m\n");
				csTemp << FX_BSTRC(" ") << rtAnnot.right << FX_BSTRC(" ") << rtAnnot.bottom << FX_BSTRC(" l\n");
				csTemp << FX_BSTRC("S\n");
			}
			else
			{
				if (e_borderStyleDashed == iBorderStyle)
				{
					FX_FLOAT fDashOffset;
					CFX_FloatArray dashPattern;
					getBorderDash(fDashOffset, dashPattern);
					FSANNOT_Util_DashToAppStream(fDashOffset, dashPattern, csTemp);
				}
				csTemp << FX_BSTRC(" ") << rtAnnot.left << FX_BSTRC(" ") << rtAnnot.bottom << FX_BSTRC(" ") << rtAnnot.Width() << FX_BSTRC(" ") << rtAnnot.Height() << FX_BSTRC(" re S\n");
			}
			//This seems to be not useful.
			//When continue reseting appearance to a link only with rect several times, the rect will be increased by this code.
			//rtAnnot.Inflate(fThickness, fThickness, fThickness, fThickness);
			setRectImp(FSANNOT_RECTKEY_RECT, rtAnnot);
		}

		csTemp << FX_BSTRC("Q\n");
	}
	CFX_Matrix mt = CFX_Matrix(1, 0, 0, 1, -rtAnnot.left, -rtAnnot.bottom);
	writeAppearance(FX_BSTRC("N"), rtAnnot, mt, csTemp, FX_BSTRC(""));

	return TRUE;
}

FS_INT32			FSPDFLinkImp::getQuadPointsCount() const
{
	return FSPDFAnnotImp::getQuadPointsCount();
}

FSQuadPoints		FSPDFLinkImp::getQuadPoints(FS_INT32 index) const
{
	return FSPDFAnnotImp::getQuadpoints(index);
}

void		FSPDFLinkImp::setQuadPoints(const FSQuadPoints* quadPoints, FS_INT32 nCount)
{
	FSPDFAnnotImp::setQuadPoints(quadPoints, nCount);
}

FSM_HIGHLIGHTINGMODE	FSPDFLinkImp::getHighlightingMode()
{
	CFX_ByteString bsHMode = getStringImpl(FSANNOT_NAMEKEY_HIGHLIGHTINGMODE);
	if(bsHMode.IsEmpty())
		return (FSM_HIGHLIGHTINGMODE)-1;
	else if( bsHMode == FX_BSTRC("N"))
		return e_highlightingModeNone;
	else if(bsHMode == FX_BSTRC("I"))
		return e_highlightingModeInvert;
	else if(bsHMode == FX_BSTRC("O"))
		return e_highlightingModeOutline;
	else if(bsHMode == FX_BSTRC("P"))
		return e_highlightingModePush;
	else //"T" or no key; "T" is only useful for widget annot; 
		return e_highlightingModeNone;
}

void	FSPDFLinkImp::setHighlightingMode(FSM_HIGHLIGHTINGMODE mode)
{
	CFX_ByteString bsHMode;
	switch(mode)
	{
	case e_highlightingModeNone:
		bsHMode = "N";
		break;
	case e_highlightingModeInvert:
		bsHMode = "I";
		break;
	case e_highlightingModeOutline:
		bsHMode = "O";
		break;
	case e_highlightingModePush:
		bsHMode = "P";
		break;
	case e_highlightingModeToggle: //"T" is only useful for widget annot;
	default:
		return ;
	}
	setName(FSANNOT_NAMEKEY_HIGHLIGHTINGMODE, bsHMode);
}


FSAction*			FSPDFLinkImp::getAction()
{
	if (m_pLinkAct) return m_pLinkAct;

	CPDF_Dictionary* pActDict = getSubDict(FSANNOT_DICTKEY_ACTION);
	// When /Dest and /A exists at the same time, trigger which one first?
	// Adobe: trigger /Dest and not trigger /A
	// Phantom: trigger /A and not tirgger /Dest -- RDK does the same as phantom.
	if (pActDict)
	{// has /A dictionary
		FSPDFDoc* pFSPDFDoc = getPage()->getDocument();
		if (!pFSPDFDoc) return NULL;
		m_pLinkAct = FSActionImp::create(pFSPDFDoc, pActDict, TRUE);
	}
	else
	{// no /A dictionary, so try to check /Dest -- if has /Dest, it equals to a goto action.
		CPDF_Object* pDestObj = getPDFDict()->GetElement("Dest");
		if (!pDestObj) return NULL;
		m_pLinkAct = FSGotoActionImp::createFromDestination(getPage()->getDocument(), pDestObj);
	}
	return m_pLinkAct;
}

void	FSPDFLinkImp::setAction(FSAction* action)
{
	if (!action)
		return;

	CPDF_Dictionary* pNewActDict = NULL;
	switch(action->getType())
	{
		case FSActionDefines::e_actionTypeGoto:
			pNewActDict = ((FSGotoActionImp*)(FSGotoAction*)action)->getActionImp()->getActDict();
			break;
		case FSActionDefines::e_actionTypeURI:
			pNewActDict = ((FSURIActionImp*)(FSURIAction*)action)->getActionImp()->getActDict();
			break;
		default:
			pNewActDict = ((FSActionImp*)action)->getActDict();
			break;
	}
	if (!pNewActDict || !getPage()) return;
	FS_ACTIONTYPE actType = action->getType();
	if (FSActionDefines::e_actionTypeGoto != actType && FSActionDefines::e_actionTypeURI != actType)
		return ;

	CPDF_Document* pDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
	
	CPDF_Reference* pNewActRef = CPDF_Reference::Create(pDoc, pDoc->AddIndirectObject(pNewActDict));
	if (!pNewActRef) return ;
	CPDF_Dictionary* pLinkAnnotDict = getPDFDict();
	// Set to /A and remove /Dest
	pLinkAnnotDict->SetAt(FSANNOT_DICTKEY_ACTION, pNewActRef);
	pLinkAnnotDict->RemoveAt("Dest");

	// Cache the new action in FSPDFLinkImp.
	if (m_pLinkAct)
	{
		if (m_pLinkAct == action) {
			setModified();
			return;
		}
		m_pLinkAct->release();
		m_pLinkAct = NULL;
	}
	m_pLinkAct = FSActionImp::retain(action);
	setModified();
}

FS_BOOL	FSPDFLinkImp::removeAction()
{
	if (!m_pAnnotDict) return FALSE;

	// Remove /A and /Dest
	m_pAnnotDict->RemoveAt(FSANNOT_DICTKEY_ACTION);
	m_pAnnotDict->RemoveAt("Dest");
	// Release m_pLinkAct
	if (m_pLinkAct)
	{
		m_pLinkAct->release();
		m_pLinkAct = NULL;
	}
	setModified();
	return TRUE;
}

