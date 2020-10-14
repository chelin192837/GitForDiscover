#include "../fs_util.h"
#include "fr_ofdrenderImp.h"
#include "fr_ofdpageImp.h"
#include "../../include/fs_pdfpage.h"
#include "../inc/fs_renderImp.h"

#define e_renderClearType				0x00000001
#define e_renderPrintGraphicText		0x00000002
#define e_renderForceDownSample			0x00000004
#define e_renderPrintPreview			0x00000008
#define e_renderForceHalftone			0x00000040

FR_OFDRenderImp::FR_OFDRenderImp()
:m_pOFDFxgeDevice(NULL)
,m_pOFDRenderDevice(NULL)
,m_pOFDRenderOptions(NULL)
,m_pOFDRenderContext(NULL)
,m_pOFDProgressiveRenderer(NULL)
,m_pRenderDevice(NULL)
,m_renderFlags(RENDER_CLEARTYPE | RENDER_LIMITEDIMAGECACHE | RENDER_FORCE_HALFTONE)
,m_bRendererWithBitmap(FALSE)
,m_pInitParam(NULL)
,m_bRgbOrder(FALSE)
,m_dwRenderContentFlag(e_renderPage|e_renderAnnot)
,m_colorMode(e_colorModeNormal)
,m_backColor(0)
,m_foreColor(0)

{

}

FR_OFDRenderImp::~FR_OFDRenderImp(){
	if (m_pOFDProgressiveRenderer) {
		m_pOFDProgressiveRenderer->StopRender();
		m_pOFDProgressiveRenderer = NULL;
	}
	if (m_pOFDProgressiveRenderer) {
		m_pOFDProgressiveRenderer->Release(); 
		m_pOFDProgressiveRenderer = NULL;
	}
	if (m_pOFDRenderContext) {
		m_pOFDRenderContext->Release();
		m_pOFDRenderContext = NULL;
	}
	//releaseResources(TRUE,FALSE);
}

FR_OFDRenderImp* FR_OFDRenderImp::create(FSBitmap* bitmap, FS_BOOL rgbOrder /* = FALSE*/){
	/*if (NULL == bitmap || NULL == ((FSBitmapImp*)bitmap)->getBitmap())
		return NULL;*/

	FR_OFDRenderImp* p_Renderer = FX_NEW FR_OFDRenderImp();
	if (p_Renderer) {
		p_Renderer->m_bRendererWithBitmap = TRUE;
		p_Renderer->m_pInitParam = (void*)bitmap;
		p_Renderer->m_bRgbOrder = rgbOrder;
	}
    if (!p_Renderer->createFXDevice()) {
		delete p_Renderer;
        return NULL;
	}
	return p_Renderer;
}

void FR_OFDRenderImp::release()
{
	if(m_pOFDRenderOptions){
		delete m_pOFDRenderOptions;
	}
	
	delete this;
}

void FR_OFDRenderImp::releaseResources(FS_BOOL bReleaseProgressRender,FS_BOOL bReleaseDevice){
	FSCRT_LOCKOBJECT(lock, &m_lock);

/*	
	if (m_pOFDProgressiveRenderer && bReleaseProgressRender) {
		m_pOFDProgressiveRenderer->Release();  
	}
	if (m_pOFDRenderContext) {
		m_pOFDRenderContext->Release();
	}
	if (m_pOFDRenderOptions) {
		//if (m_pOFDRenderOptions->m_pOCContext)
		//	delete m_pOFDRenderOptions->m_pOCContext;
		//m_pOFDRenderOptions->m_pOCContext = NULL;
		delete m_pOFDRenderOptions;
		m_pOFDRenderOptions = NULL;
	}
	if (m_pRenderDevice && bReleaseDevice) { 
		delete m_pRenderDevice; 
		m_pRenderDevice = NULL; 
	} 
	if (m_pRenderPause) { 
		delete m_pRenderPause; 
		m_pRenderPause = NULL; 
	}*/
/*	if (m_pAnnotList) { 
		delete m_pAnnotList; 
		m_pAnnotList = NULL; 
	} 
	if (m_pMatrix) {
		delete m_pMatrix;
		m_pMatrix = NULL;
	}*/
	//m_pCurRenderPage = NULL;
}

FS_PROGRESSSTATE	FR_OFDRenderImp::getRenderProgressState(){
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (NULL == m_pOFDProgressiveRenderer || NULL == m_pOFDRenderDevice)
		return FSCommonDefines::e_progressError;
	OFD_RENDERSTATUS  status = m_pOFDProgressiveRenderer->GetStatus();
	switch(status)
	{
	case OFD_RENDERSTATUS_Failed: 
		return FSCommonDefines::e_progressError;
	case OFD_RENDERSTATUS_Begin:
	case OFD_RENDERSTATUS_Continued: 
		return FSCommonDefines::e_progressToBeContinued;
	case OFD_RENDERSTATUS_Done: 
	{
/*#if _FX_OS_ == _FX_ANDROID_
			if (m_pCurRenderPage->getPage()->BackgroundAlphaNeeded())
			{
				CFX_DIBitmap* pBitmap = ((FSBitmapImp*)m_pInitParam)->getBitmap();
				for (int row = 0; row < pBitmap->GetHeight(); row ++)
				{
					void* buffer = (void*)pBitmap->GetScanline(row);
					char* line = (char *)buffer;
					for (int col = 0; col < pBitmap->GetWidth(); col ++)
					{   
						//On the X86 device, we found that the value of each byte in bitmap buffer is 0xFFFFFFXX
						//so the premultiplied result would be overflow.
						line[0] = (line[0] & 0x000000FF) * (line[3] & 0x000000FF)/0xFF;
						line[1] = (line[1] & 0x000000FF) * (line[3] & 0x000000FF)/0xFF;
						line[2] = (line[2] & 0x000000FF) * (line[3] & 0x000000FF)/0xFF;
						line+=4;
					}
				}	
			}
#endif			
*/
			m_pRenderDevice->EndRendering();
			return FSCommonDefines::e_progressFinished;
		}
	}
	return FSCommonDefines::e_progressError;
}



FS_PROGRESSSTATE FR_OFDRenderImp::startRenderImp(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause, FSAnnot* pAnnot)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	//"pAnnot is valid" means just to render single annotation, so m_dwRenderContentFlag will be ignored.
	//"pAnnot=NULL" means m_dwRenderContentFlag should be taken into consideration.
	//Page must been parsed only when rendering page. If only render annot(s), no need to care about if page has been parsed.
	if (!m_pInitParam || !page || (e_renderPage == (m_dwRenderContentFlag & e_renderPage) && !page->isParsed()))
		return FSCommonDefines::e_progressError;

	if (NULL == pAnnot && 0 == m_dwRenderContentFlag) 
		return FSCommonDefines::e_progressFinished;

	releaseResources(TRUE, FALSE);

   // m_pAnnot = pAnnot;
	//m_pOFDCurRenderPage = ((FR_OFDPageImp*)page);

	//m_pOFDRenderOptions->m_Flags = m_renderFlags ;
	CFX_Matrix ctm(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);
	CFX_Matrix ofd_2_pdf = ((FR_OFDPageImp*)page)->getFormatMatrix();
	ofd_2_pdf.Concat(ctm);
	ctm = ofd_2_pdf;

	//m_pOFDRenderOptions->m_Flags = m_renderFlags;
	m_pOFDRenderOptions = new COFD_RenderOptions();
	//m_pOFDRenderOptions->m_bBlendSeal = FALSE;
	//m_pOFDRenderOptions->m_Flags = m_renderFlags;
	FX_INT32 a = 0;
	//ArgbDecode(m_foreColor, a, m_pOFDRenderOptions->m_ForeColor);
	//ArgbDecode(m_backColor, a, m_pOFDRenderOptions->m_BackColor);
	//COFD_RenderOptions options;
	if (m_colorMode == e_colorModeMapping) //{
		m_pOFDRenderOptions->m_ColorMode = OFD_RENDER_COLOR_TWOCOLOR;
		m_pOFDRenderOptions->m_BackColor = m_backColor;//0xff000000;
		m_pOFDRenderOptions->m_ForeColor = m_foreColor;//0xffffffff;
				//
		//options.m_ColorMode = OFD_RENDER_COLOR_GRAY;
		//options.m_BackColor = 0xff000000;
		//options.m_ForeColor = 0xffffffff;
	//}

	m_pOFDRenderContext = IOFD_RenderContext::Create();
	if (!m_pOFDRenderContext) {
		return FSCommonDefines::e_progressError;
	}

	m_pOFDRenderContext->AddPage(((FR_OFDPageImp*)page)->getPage(), &ctm);

	m_pOFDRenderDevice = new COFD_RenderDevice();
    // add by gm, ofd render only support default rgb order
#if _FX_OS_ == _FX_ANDROID_
    CFX_DIBitmap dib;
	dib.Create(m_pOFDFxgeDevice->GetWidth(), m_pOFDFxgeDevice->GetHeight(), FXDIB_Argb);
	if (m_colorMode == e_colorModeMapping) {
		dib.Clear(m_backColor);
	} else {
		dib.Clear(-1);
	}
	CFX_FxgeDevice fxgeDevice;
    fxgeDevice.Attach(&dib);
    m_pOFDRenderDevice->Create(&fxgeDevice);
#else
	m_pOFDRenderDevice->Create(m_pOFDFxgeDevice);
#endif

	m_pOFDProgressiveRenderer = IOFD_ProgressiveRenderer::Create();
	if (!m_pOFDProgressiveRenderer) 
		return FSCommonDefines::e_progressError;
	bool ret = false;
//	unsigned int m_bkcolor = 0xff000000;
//	unsigned int m_forecolor = 0xffffffff;
	do {
		FX_BOOL result;
		//COFD_RenderOptions options;
		//options.m_ColorMode = OFD_RENDER_COLOR_GRAY;
		//options.m_BackColor = 0xff000000;
		//options.m_ForeColor = 0xffffffff;
		result = m_pOFDProgressiveRenderer->StartRender(m_pOFDRenderDevice, m_pOFDRenderContext, m_pOFDRenderOptions, &ctm);
		if (!result) {break;}
		result = m_pOFDProgressiveRenderer->RenderAnnots(((FR_OFDPageImp*)page)->getPage(),true);
		//if (!result) {break;}
		result = m_pOFDProgressiveRenderer->DoRender(NULL);
		if (!result) {break;}
		/*	
		result = m_pOFDProgressiveRenderer->RenderAnnots(((FR_OFDPageImp*)page)->getPage(),TRUE, FXDIB_BLEND_MULTIPLY);		
		result = m_pOFDProgressiveRenderer->RenderAnnots(((FR_OFDPageImp*)page)->getPage(),false);
		if (!result) {break;}			
		*/
		result = m_pOFDProgressiveRenderer->RenderAnnots(((FR_OFDPageImp*)page)->getPage(),false);
		//if (!result) {break;}
		m_pOFDProgressiveRenderer->RenderStampAnnots();
		m_pOFDProgressiveRenderer->StopRender();
		ret = true;
	} while (0);

	FS_PROGRESSSTATE state = ret ? FSCommonDefines::e_progressFinished : FSCommonDefines::e_progressError;
	/*
	FS_PROGRESSSTATE state =  getRenderProgressState();
	if(state == FSCommonDefines::e_progressFinished) {
		//state = renderAnnots(page, pAnnot);
	}
	*/

#if _FX_OS_ == _FX_ANDROID_
    m_pOFDFxgeDevice->SetDIBits(fxgeDevice.GetBitmap(), 0, 0);
#endif

	return state;
}


FS_PROGRESSSTATE FR_OFDRenderImp::startRender(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause)
{
	return startRenderImp(page, matrix, pause, NULL);
}

FS_PROGRESSSTATE FR_OFDRenderImp::continueRender(){
	FS_PROGRESSSTATE state = getRenderProgressState();
	if (state != FSCommonDefines::e_progressToBeContinued)
		return state;

	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (m_pOFDProgressiveRenderer)
		m_pOFDProgressiveRenderer->Continue(m_pRenderPause);
	else 
		return FSCommonDefines::e_progressError;

	state =  getRenderProgressState();
//	if(state == FSCommonDefines::e_progressFinished)
		//state = renderAnnots(m_pCurRenderPage, NULL);
	return state;
}

FS_BOOL	FR_OFDRenderImp::renderAnnot(FSAnnot* annot, const FSMatrix& matrix){
	return FALSE;
}

void FR_OFDRenderImp::setRenderContent(FS_DWORD renderContentFlag){
	m_dwRenderContentFlag = renderContentFlag;
}

void FR_OFDRenderImp::setTransformAnnotIcon(FS_BOOL bTransformAnnotIcon){
}

void FR_OFDRenderImp::setColorMode(FS_RENDERCOLORMODE colorMode){
	if (colorMode != e_colorModeNormal && colorMode != e_colorModeMapping) //todo..
		return;
	m_colorMode = colorMode;
}

void FR_OFDRenderImp::setMappingModeColors(FS_ARGB backColor, FS_ARGB foreColor){
	m_backColor = backColor;
	m_foreColor = foreColor;
}

void FR_OFDRenderImp::setClearType(FS_BOOL bClearType){
	if (bClearType){
		m_renderFlags |= e_renderClearType;
	}else{
		m_renderFlags &= ~e_renderClearType;
	}
}

void FR_OFDRenderImp::setPrintGraphicText(FS_BOOL bPrintGraphicText){
	if (bPrintGraphicText){
		m_renderFlags |= e_renderPrintGraphicText;
	}else{
		m_renderFlags &= ~e_renderPrintGraphicText;
	}
}

void FR_OFDRenderImp::setForceDownSample(FS_BOOL bForceDownSample){
	if (bForceDownSample)
		m_renderFlags |= e_renderForceDownSample;
	else
		m_renderFlags &= ~e_renderForceDownSample;
}

void FR_OFDRenderImp::setForceHalftone(FS_BOOL bForceHalftone){
	if (bForceHalftone)
		m_renderFlags |= e_renderForceHalftone;
	else
		m_renderFlags &= ~e_renderForceHalftone;
}


FS_BOOL			FR_OFDRenderImp::createFXDevice(){

	if (m_bRendererWithBitmap)
    {
		FSBitmapImp* pBitmapImp = (FSBitmapImp*)m_pInitParam;
        m_pOFDFxgeDevice = FX_NEW CFX_FxgeDevice();
        if (!m_pOFDFxgeDevice)
            return FALSE;
        if (!(CFX_FxgeDevice*)m_pOFDFxgeDevice->Attach(pBitmapImp->getBitmap(),0,m_bRgbOrder))
            return FALSE;
        return TRUE;
    } 
    return FALSE;
}




