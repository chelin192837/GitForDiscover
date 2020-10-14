#include "fr_include.h"
#include "../fs_util.h"
#include "../../include/fs_pdfpage.h"
#include "fr_ofddocimp.h"
#include "fr_ofdactionImp.h"

// class FR_OFDDestImp : public FSDestination, public CFX_Object
FR_OFDDestImp*		FR_OFDDestImp::create(FS_ZOOMMODE zoomMode, FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom, FS_FLOAT zoomFactor)
{
	FR_OFDDestImp* result = new FR_OFDDestImp;
	result->init(zoomMode, page, left, top, right, bottom, zoomFactor);
	return result;
}

FSDestination*		FR_OFDDestImp::createFromOFDDest(FR_OFDDocImp* doc, COFD_Dest* pDest)
{
	COFD_Dest& dest = *pDest;
	OFD_DESTTYPE type = dest.GetDestType();
	FX_DWORD pageid = dest.GetPageID();
	IOFD_Document* ofdDoc = doc->getOFDDocument();
	int index = ofdDoc->GetPageIndex(ofdDoc->GetPageByID(pageid));
	FSPDFPage* pFSPDFPage = doc->getPage(index);
	FSDestination* pRetDest = NULL;

	float left = MM_2_PT(dest.GetLeft());
	float top = MM_2_PT(dest.GetTop());
	float right = MM_2_PT(dest.GetRight());
	float bottom = MM_2_PT(dest.GetBottom());
	switch(type) {
	case OFD_DESTTYPE_XYZ:		//left, top, zoom
		{
			pRetDest = FSDestination::createXYZ(pFSPDFPage, left, top, dest.GetZoom());
			break;
		}
	case OFD_DESTTYPE_Fit:		//fit page
		{
			pRetDest = FSDestination::createFitPage(pFSPDFPage);
			break;
		}
	case OFD_DESTTYPE_FitH:		//fit width, top
		{
			pRetDest = FSDestination::createFitHorz(pFSPDFPage, top);
			break;
		}
	case OFD_DESTTYPE_FitV:		//fit height, left
		{
			pRetDest = FSDestination::createFitVert(pFSPDFPage, left);
			break;
		}
	case OFD_DESTTYPE_FitR:		//fit rectangle, left, top, right, bottom
		{
			pRetDest = FSDestination::createFitRect(pFSPDFPage, left, top, right, bottom);
			break;
		}
	default:
		break;
	};
	return pRetDest;
}

FR_OFDDestImp::FR_OFDDestImp()
	: m_PageIndex(-1)
	, m_Page(NULL)
	, m_ZoomMode(FSCommonDefines::e_zoomXYZ)
	, m_Left(0.0f)
	, m_Top(0.0f)
	, m_Right(0.0f)
	, m_Bottom(0.0f)
	, m_ZoomFactor(0.0f)
{

}

FR_OFDDestImp::~FR_OFDDestImp()
{

}

void		FR_OFDDestImp::init(FS_ZOOMMODE zoomMode, FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom, FS_FLOAT zoomFactor)
{
	m_ZoomMode = zoomMode;
	m_Left = left;
	m_Top = top;
	m_Right = right;
	m_Bottom = bottom;
	m_ZoomFactor = zoomFactor;
	m_PageIndex = page->getIndex();
	m_Page = page;
}

#ifndef _SWIG_IOS_
void FR_OFDDestImp::release()
{
	delete this;
}

#endif

FS_INT32 FR_OFDDestImp::getPageIndex() const
{
	return m_PageIndex;
}

FS_ZOOMMODE FR_OFDDestImp::getZoomMode() const
{
	return m_ZoomMode;
}

FS_FLOAT FR_OFDDestImp::getLeft() const
{
	return m_Left;
}

FS_FLOAT FR_OFDDestImp::getTop() const
{
	return m_Top;
}

FS_FLOAT FR_OFDDestImp::getRight() const
{
	return m_Right;
}

FS_FLOAT FR_OFDDestImp::getBottom() const
{
	return m_Bottom;
}

FS_FLOAT FR_OFDDestImp::getZoomFactor() const
{
	return m_ZoomFactor;
}
