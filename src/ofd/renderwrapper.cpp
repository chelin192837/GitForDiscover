#include "../fs_util.h"
#include "fr_ofdrenderImp.h"
#include "fr_ofdpageImp.h"
#include "../../include/fs_pdfpage.h"
#include "../inc/fs_renderImp.h"

#include "renderwrapper.h"

//class RenderWrapper: public FSRenderer, public CFX_Object
RenderWrapper::RenderWrapper()
	: m_CurrentRender(NULL)
	, m_PDFRender(NULL)
	, m_OFDRender(NULL)
{

}

RenderWrapper::~RenderWrapper()
{
	if (m_PDFRender) {
		delete m_PDFRender;
		m_PDFRender = NULL;
	}

	if (m_OFDRender) {
		delete m_OFDRender;
		m_OFDRender = NULL;
	}
}

RenderWrapper*	RenderWrapper::create(FSBitmap* bitmap, FS_BOOL rgbOrder)
{
	RenderWrapper* result = new RenderWrapper();
	result->m_PDFRender = FSRendererImp::create(bitmap, rgbOrder);
	result->m_OFDRender = FR_OFDRenderImp::create(bitmap, rgbOrder);
	return result;
}

#ifdef __APPLE__
RenderWrapper*	RenderWrapper::createFromContext(CGContextRef context, FS_DEVICETYPE deviceType)
{
	RenderWrapper* result = new RenderWrapper();
	result->m_PDFRender = FSRendererImp::createFromContext(context, deviceType);
//	result->m_OFDRender = FR_OFDRenderImp::createFromContext(context, deviceType);
	return result;
}
#endif

#ifndef _SWIG_IOS_
void RenderWrapper::release()
{
	if (m_PDFRender) {
		m_PDFRender->release();
		m_PDFRender = NULL;
	}
	if (m_OFDRender) {
		m_OFDRender->release();
		m_OFDRender = NULL;
	}
	delete this;
}
#endif

FS_PROGRESSSTATE	RenderWrapper::startRender(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause)
{
	if (isOFDPage(page)) {
		m_CurrentRender = m_OFDRender;
	} else {
		m_CurrentRender = m_PDFRender;
	}
	
	return m_CurrentRender->startRender(page,matrix, pause);
}

FS_PROGRESSSTATE	RenderWrapper::continueRender()
{
	if (!m_CurrentRender) {
		return FSCommonDefines::e_progressError;
	}
	return m_CurrentRender->continueRender();
}

FS_BOOL				RenderWrapper::renderAnnot(FSAnnot* annot, const FSMatrix& matrix)
{
	if (!m_CurrentRender) {
		return FALSE;
	}
	return m_CurrentRender->renderAnnot(annot, matrix);
}

void	RenderWrapper::setRenderContent(FS_DWORD renderContentFlag)
{
	if (m_CurrentRender) {
		m_CurrentRender->setRenderContent(renderContentFlag);
	} else {
        if (m_PDFRender) {
                 m_PDFRender->setRenderContent(renderContentFlag);
             }
//		m_OFDRender->setRenderContent(renderContentFlag);
	}
}


void	RenderWrapper::setTransformAnnotIcon(FS_BOOL transformAnnotIcon)
{
	if (m_CurrentRender) {
		m_CurrentRender->setTransformAnnotIcon(transformAnnotIcon);
	} else {
        if (m_PDFRender) {
            m_PDFRender->setTransformAnnotIcon(transformAnnotIcon);
        }
//		m_OFDRender->setTransformAnnotIcon(transformAnnotIcon);
	}
}

void	RenderWrapper::setColorMode(FS_RENDERCOLORMODE colorMode)
{
	if (m_CurrentRender) {
		m_CurrentRender->setColorMode(colorMode);
	} else {
        if (m_PDFRender) {
            m_PDFRender->setColorMode(colorMode);
        }
//		m_OFDRender->setColorMode(colorMode);
	}
}

void	RenderWrapper::setMappingModeColors(FS_ARGB backgroundColor, FS_ARGB foregroundColor)
{
	if (m_CurrentRender) {
		m_CurrentRender->setMappingModeColors(backgroundColor, foregroundColor);
	} else {
        if (m_PDFRender) {
            m_PDFRender->setMappingModeColors(backgroundColor, foregroundColor);
        }
//		m_OFDRender->setMappingModeColors(backgroundColor, foregroundColor);
	}
}


#ifndef _SWIG_ANDROID_
void	RenderWrapper::setClearType(FS_BOOL bClearType)
{
	if (m_CurrentRender) {
		m_CurrentRender->setClearType(bClearType);
	} else {
		m_PDFRender->setClearType(bClearType);
		m_OFDRender->setClearType(bClearType);
	}
}

void	RenderWrapper::setPrintGraphicText(FS_BOOL bPrintGraphicText)
{
	if (m_CurrentRender) {
		m_CurrentRender->setPrintGraphicText(bPrintGraphicText);
	} else {
		m_PDFRender->setPrintGraphicText(bPrintGraphicText);
		m_OFDRender->setPrintGraphicText(bPrintGraphicText);
	}
}


void	RenderWrapper::setForceDownSample(FS_BOOL bForceDownSample)
{
	if (m_CurrentRender) {
		m_CurrentRender->setForceDownSample(bForceDownSample);
	} else {
		m_PDFRender->setForceDownSample(bForceDownSample);
		m_OFDRender->setForceDownSample(bForceDownSample);
	}
}

void	RenderWrapper::setForceHalftone(FS_BOOL bForceHalftone)
{
	if (m_CurrentRender) {
		m_CurrentRender->setForceHalftone(bForceHalftone);
	} else {
		m_PDFRender->setForceHalftone(bForceHalftone);
		m_OFDRender->setForceHalftone(bForceHalftone);
	}
}

#endif
