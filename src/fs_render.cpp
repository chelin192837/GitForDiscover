#include "../include/fs_pdfpage.h"
#include "../include/fs_pdfdoc.h"
#include "../include/fs_render.h"
#include "../include/fs_annot.h"
#include "../include/fs_formfiller.h"

#include "fxcore.h"
#include "fs_util.h"
#include "fs_pause.h"
#include "annotation/fs_annot_imp.h"
#include "annotation/fs_annot_util.h"
#include "form/fs_pdfformImp.h"
#include "form/fs_formfillerImp.h"
#include "form/fpdf_annothandler.h"
#include "fs_iconprovider.h"
#include "inc/fs_pdfdocImp.h"
#include "inc/fs_pdfpageImp.h"
#include "inc/fs_renderImp.h"

#include "../src/ofd/fr_ofdrenderImp.h"
#include "../src/ofd/renderwrapper.h"

//class FSBitmap
#define FSCRT_BITMAP_SIZE_LIMIT 2 * 1024 * 1024 * 1024uL


/** @brief	Use ClearType-like anti-aliasing to render text objects, this OPTION is set in default. */
#define e_renderClearType				0x00000001
/** @brief	Always output text as graphics (path or bitmap), don't allow device font substitution. */
#define e_renderPrintGraphicText		0x00000002
/** @brief	Always use downsampling for image stretching. */
#define e_renderForceDownSample			0x00000004
/** @brief	Print preview mode. */
#define e_renderPrintPreview			0x00000008
/** @brief	Always use halftone for image stretching. */
#define e_renderForceHalftone			0x00000040
/** @brief	Always use anti-aliasing for rectangle drawing. */
#define e_renderRectAntiAliasing		0x00000080
/** @brief	Always output text as bitmap in less than 50 font size case, use for epson stylus printer or other specified printers. */
#define e_renderPrintImageText			0x00000200
/** @brief	Using overprint to render page. */
#define e_renderOverPrint				0x00000400
/** @brief	Using thin line for path drawing. */
#define e_renderThinLine				0x00000800
/** @brief	enhance zero line. */
#define e_renderEnhanceZeroLine			0x00001000
/** @brief	Do not use anti-aliasing to render text objects, this OPTION doesn't set in default. */
#define e_renderNoTextSmooth		 	0x10000000
/** @brief	Do not use anti-aliasing to render path objects, this OPTION doesn't set in default. */
#define e_renderNoPathSmooth		 	0x20000000
/** @brief	Do not smooth images when render image objects, this OPTION doesn't set in default. */
#define e_renderNoImageSmooth			0x40000000
/** @brief	Limit image cache size. */
#define e_renderLimitedImageCache		0x80000000
/** @brief	The text knockout is true. */
#define e_renderTextKnockout			0x00010000

////////////////////////////////////////////
// Class FSBitmap
FSBitmap*	FSBitmap::create(FS_INT32 width, FS_INT32 height, FS_DIBFORMAT format, FS_LPBYTE buffer/* = NULL*/, int pitch/* = 0*/)
{
	return FSBitmapImp::create(width, height, format, buffer, pitch);
}

////////////////////////////////////////////
// Class FSBitmapImp
FSBitmapImp::FSBitmapImp()
	:m_pBitmap(NULL)
{

}

FSBitmapImp::~FSBitmapImp()
{
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

FSBitmapImp* FSBitmapImp::create(FS_INT32 width, FS_INT32 height, FS_DIBFORMAT format, FS_LPBYTE buffer, int pitch)
{
	if (width < 1 || height <1 || (FSCommonDefines::e_dibArgb != format 
		&& FSCommonDefines::e_dibRgb != format && FSCommonDefines::e_dibRgb32 != format))// todo..
		return NULL;
	
	CFX_DIBitmap* pBitmap = FX_NEW CFX_DIBitmap();
	if (!pBitmap) return NULL;

	if (!((CFX_DIBitmap*)pBitmap)->Create(width, height, (FXDIB_Format)format, buffer, pitch))
	{
		delete pBitmap;
		return NULL;
	}
	FSBitmapImp* pBitmapImp = FX_NEW FSBitmapImp();
	if (!pBitmapImp)
	{
		delete pBitmap;
		return NULL;
	}
	pBitmapImp->m_pBitmap = pBitmap;
	return pBitmapImp;
}

void FSBitmapImp::release()
{
	delete this;
}

FSBitmapImp *FSBitmapImp::clone(const FSRectI* clip) const
{
	if (!m_pBitmap) return NULL;

	CFX_DIBitmap* pBitmap = m_pBitmap->Clone((FX_RECT*)clip);
	if (pBitmap == NULL)
		return NULL;

	FSBitmapImp* pBitmapImp = FX_NEW FSBitmapImp();
	if (!pBitmapImp)
	{
		delete pBitmap;
		return NULL;
	}
	pBitmapImp->m_pBitmap = pBitmap;
	return pBitmapImp;
}

extern void RgbByteOrderCompositeRect(CFX_DIBitmap* pBitmap, int left, int top, int width, int height, FX_ARGB argb);
void FSBitmapImp::fillRect(FS_ARGB color, const FSRectI* fillRect)
{
	if (NULL == m_pBitmap) return;

	FX_RECT clipRect(0, 0, getWidth(), getHeight());
	if (fillRect)
		clipRect.Intersect(fillRect->left, fillRect->top, fillRect->right, fillRect->bottom);
	if (clipRect.IsEmpty()) 
		return;
	
	FS_BOOL hasAlpha = ((CFX_DIBitmap*)m_pBitmap)->HasAlpha();
	FS_BOOL isAlphaMask = ((CFX_DIBitmap*)m_pBitmap)->IsAlphaMask();
	//RgbByteOrderCompositeRect() and CFX_DIBitmap::CompositeRect() only affect the bitmaps which contain alpha component. 
	//for the bitmaps which don't contain alpha, alpha should be 0xff.
	if (!hasAlpha && !isAlphaMask)
		color |= 0xff000000;

	if (!fillRect && (!hasAlpha || isAlphaMask))//CFX_DIBitmap::Clear will replace all pixels.
	{
		((CFX_DIBitmap*)m_pBitmap)->Clear(color);
		return ;
	}

	// we are just filling (compositing) all of the clipped rectangle. Only suitable for bitmap which contain alpha channel.

	//fxcore always assume the format of color is 0xAABBGGRR. But the format of param is 0xAARRGGBB. So we should adjust it.
	FS_ARGB oldColor = color;
	color = oldColor & 0xff00ff00;
	color |= (oldColor << 16) & 0x00ff0000;
	color |= (oldColor >> 16) & 0x000000ff;
	RgbByteOrderCompositeRect((CFX_DIBitmap*)m_pBitmap, clipRect.left, clipRect.top, clipRect.Width(), clipRect.Height(), color);
}

FS_INT32 FSBitmapImp::getWidth()const
{
	if (!m_pBitmap) return -1;
	return m_pBitmap->GetWidth();
}

FS_INT32 FSBitmapImp::getHeight() const
{
	if (!m_pBitmap) return -1;
	return m_pBitmap->GetHeight();
}

FS_INT32 FSBitmapImp::getPitch() const
{
	if (!m_pBitmap) return -1;
	return m_pBitmap->GetPitch();
}

FS_INT32 FSBitmapImp::getBpp() const
{
	if (!m_pBitmap) return -1;
	return m_pBitmap->GetBPP();
}

FS_LPBYTE FSBitmapImp::getBuffer() const
{
	if (!m_pBitmap) return NULL;
	return m_pBitmap->GetBuffer();
}

FS_DIBFORMAT FSBitmapImp::getFormat() const
{
	if (!m_pBitmap) return FSCommonDefines::e_dibInvalid;

	return (FS_DIBFORMAT)m_pBitmap->GetFormat();
}

///////////////////////////////
//class FSRenderer
FSRenderer*	FSRenderer::create(FSBitmap* bitmap, FS_BOOL rgbOrder)
{
	//return FSRendererImp::create(bitmap, rgbOrder);
	//return FR_OFDRenderImp::create(bitmap, rgbOrder);
	return RenderWrapper::create(bitmap, rgbOrder);
}

#ifdef __APPLE__
FSRenderer*	FSRenderer::createFromContext(CGContextRef context, FS_DEVICETYPE deviceType)
{
	//return FSRendererImp::createFromContext(context, deviceType);
	return RenderWrapper::createFromContext(context, deviceType);
}
#endif //__APPLE__

//////////////////////////////
// Class FSRendererImp
FSRendererImp::FSRendererImp(FS_INT16 deviceType/* = -1*/)
	:m_bRendererWithBitmap(TRUE)
	,m_pInitParam(NULL)
	,m_renderFlags(RENDER_CLEARTYPE | RENDER_LIMITEDIMAGECACHE | RENDER_FORCE_HALFTONE)
	,m_pRenderDevice(NULL)
	,m_pProgressRender(NULL)
	,m_pRenderContext(NULL)
	,m_pRenderOptions(NULL)
	,m_pRenderPause(NULL)
	,m_backColor(0)
	,m_foreColor(0)
	,m_colorMode(e_colorModeNormal)
	,m_pCurRenderPage(NULL)
	,m_pMatrix(NULL)
	,m_bRgbOrder(FALSE)
	,m_pAnnotList(NULL)
	,m_dwRenderContentFlag(e_renderPage|e_renderAnnot)
	,m_bTransformAnnotIcon(TRUE)
	,m_pFormFiller(NULL)
    ,m_pAnnot(NULL)
#ifdef __APPLE__
	,m_iDeviceType(deviceType)
#endif //__APPLE__
{
}

FSRendererImp* FSRendererImp::create(FSBitmap* bitmap, FS_BOOL rgbOrder /* = FALSE*/)
{
	if (NULL == bitmap || NULL == ((FSBitmapImp*)bitmap)->getBitmap())
		return NULL;

	FSRendererImp* pRenderer = FX_NEW FSRendererImp();
	if (pRenderer) {
		pRenderer->m_bRendererWithBitmap = TRUE;
		pRenderer->m_pInitParam = (void*)bitmap;
		pRenderer->m_bRgbOrder = rgbOrder;
	}
    if (!pRenderer->createFXDevice()) {
		delete pRenderer;
        return NULL;
	}
	return pRenderer;
}

#ifdef __APPLE__
FSRendererImp* FSRendererImp::createFromContext(CGContextRef context, FS_DEVICETYPE deviceType)
{
	if (!context || deviceType < e_deviceTypePrinter|| deviceType > e_deviceTypeDisplay)
		return NULL;

	FSRendererImp* pRenderer = FX_NEW FSRendererImp(deviceType);
	if (pRenderer)
	{
		pRenderer->m_bRendererWithBitmap = FALSE;
		pRenderer->m_pInitParam = (void*)context;
	}
    if (!pRenderer->createFXDevice()){
		delete pRenderer;
        return NULL;
	}
	return pRenderer;
}
#endif

FS_BOOL FSRendererImp::createFXDevice()
{
    if (m_bRendererWithBitmap)
    {
        FSBitmapImp* pBitmapImp = (FSBitmapImp*)m_pInitParam;
        m_pRenderDevice = FX_NEW CFX_FxgeDevice();
        if (!m_pRenderDevice)
            return FALSE;
        if (!((CFX_FxgeDevice*)m_pRenderDevice)->Attach(pBitmapImp->getBitmap(), 0, m_bRgbOrder))
            return FALSE;
        return TRUE;
    } else {
#ifdef __APPLE__
        m_pRenderDevice = FX_NEW CFX_QuartzDevice();
        if (!m_pRenderDevice)
            return FALSE;
        if (!((CFX_QuartzDevice*)m_pRenderDevice)->Attach((CGContextRef)m_pInitParam, m_iDeviceType))
            return FALSE;
        return TRUE;
#endif
    }
    return FALSE;
}

FSRendererImp::~FSRendererImp()
{
	releaseResources(TRUE);
}

void FSRendererImp::release()
{
	delete this;
}

void FSRendererImp::releaseResources(FS_BOOL bReleaseProgressRender,FS_BOOL bReleaseDevice)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (m_pProgressRender && bReleaseProgressRender) {
		delete m_pProgressRender; 
		m_pProgressRender = NULL; 
	}
	if (m_pRenderContext) {
		delete m_pRenderContext;
		m_pRenderContext = NULL;
	}
	if (m_pRenderOptions) {
		if (m_pRenderOptions->m_pOCContext)
			delete m_pRenderOptions->m_pOCContext;
		m_pRenderOptions->m_pOCContext = NULL;
		delete m_pRenderOptions;
		m_pRenderOptions = NULL;
	}
	if (m_pRenderDevice && bReleaseDevice) { 
		delete m_pRenderDevice; 
		m_pRenderDevice = NULL; 
	} 
	if (m_pRenderPause) { 
		delete m_pRenderPause; 
		m_pRenderPause = NULL; 
	}
	if (m_pAnnotList) { 
		delete m_pAnnotList; 
		m_pAnnotList = NULL; 
	} 
	if (m_pMatrix) {
		delete m_pMatrix;
		m_pMatrix = NULL;
	}
	m_pCurRenderPage = NULL;
}

FS_PROGRESSSTATE FSRendererImp::getRenderProgressState()
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (NULL == m_pProgressRender || NULL == m_pRenderDevice)
		return FSCommonDefines::e_progressError;

	CPDF_ProgressiveRenderer::RenderStatus status = m_pProgressRender->GetStatus();
	switch(status)
	{
		case CPDF_ProgressiveRenderer::Failed: 
			return FSCommonDefines::e_progressError;
		case CPDF_ProgressiveRenderer::Ready:
		case CPDF_ProgressiveRenderer::ToBeContinued: 
			return FSCommonDefines::e_progressToBeContinued;
		case CPDF_ProgressiveRenderer::Done: 
		{
#if _FX_OS_ == _FX_ANDROID_
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

			m_pRenderDevice->EndRendering();
			return FSCommonDefines::e_progressFinished;
		}
	}
	return FSCommonDefines::e_progressError;
}

FS_PROGRESSSTATE FSRendererImp::renderAnnots(FSPDFPage* page, FSAnnot* pAnnot)
{
	if (pAnnot)
	{
		if (!m_pAnnotList)
			m_pAnnotList = FX_NEW CPDF_AnnotList(m_pCurRenderPage->getPage());
		if (!m_pAnnotList) 
			return FSCommonDefines::e_progressError;
		CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)pAnnot->getDict();
		FS_INT32 iCount = m_pAnnotList->Count();
		CPDF_Annot* pPDFAnnot = NULL;
		for (FS_INT32 i = 0; i < iCount; i++)
		{
			CPDF_Annot* pTempAnnot = m_pAnnotList->GetAt(i);
			if (!pTempAnnot) 
				continue;
			if (pTempAnnot->m_pAnnotDict->GetObjNum() == pAnnotDict->GetObjNum())
			{
				pPDFAnnot = pTempAnnot;
				break;
			}
		}
		if (NULL == pPDFAnnot) 
			return FSCommonDefines::e_progressError;
		if (!renderAnnotToRenderer(pPDFAnnot, pAnnot, m_pAnnotList, page))
			return FSCommonDefines::e_progressError;
	}
	else if(e_renderAnnot == (m_dwRenderContentFlag & e_renderAnnot)
		)
	{//If render all annotations, then append the annotation objects to render context.
		if (!m_pAnnotList)
			m_pAnnotList = FX_NEW CPDF_AnnotList(m_pCurRenderPage->getPage());
		if (!m_pAnnotList) return FSCommonDefines::e_progressError;

		FS_INT32 iCount = m_pAnnotList->Count();
		for (FS_INT32 i = 0; i < iCount; i++)
		{
			CPDF_Annot* pAnnot = m_pAnnotList->GetAt(i);
			if (!pAnnot) continue;
	
			renderAnnotToRenderer(pAnnot, ((FSPDFPageImp*)page)->getAnnotbyDict(pAnnot->m_pAnnotDict), m_pAnnotList, page);
		}
	}

	return FSCommonDefines::e_progressFinished;
}

FS_BOOL FSRendererImp::renderAnnotToRenderer(CPDF_Annot* pAnnot, FSAnnot* fsAnnot, CPDF_AnnotList* pAnnotList, FSPDFPage* pPage)
{
	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (!pAnnot || !pPage || !pAnnotList || !m_pCurRenderPage->getPage() || !m_pRenderContext || !m_pRenderOptions || !m_pMatrix) 
		return FALSE;

	//TODO: FixTextFieldAP. It may be affect efficiency. 
	CPDF_Dictionary* pAnnotDict = pAnnot->m_pAnnotDict;
	if (!pAnnotDict) 
		return FALSE;

	if (pAnnotDict->GetDict(FSANNOT_DICTKEY_IRT) && pAnnotDict->GetString(FSANNOT_NAMEKEY_RT) != FX_BSTRC("Group"))
		return FALSE;

	FX_BOOL bRenderAnnot = FSPDF_Util_CheckShouldRenderAnnot(pAnnot);
	if (!bRenderAnnot) return FALSE;

	CFX_ByteString bsAnnotType = pAnnot->GetSubType();

	CFX_Matrix annotMatrix;
	pAnnotList->GetAnnotMatrix(pAnnotDict, m_pMatrix, annotMatrix);

	IPDF_OCContext* pOCContext = m_pRenderOptions->m_pOCContext;
	if (pOCContext != NULL && pAnnotDict != NULL && 
		!pOCContext->CheckOCGVisible(pAnnotDict->GetDict(FX_BSTRC("OC"))))
		return FALSE;

	FS_INT32 rotate = m_pCurRenderPage->getRotation() * 90;
	CPDF_Rect annotRect = pAnnotDict->GetRect(FSANNOT_RECTKEY_RECT);
	if (FALSE == m_bTransformAnnotIcon 
		&& (bsAnnotType.Equal("Text") ||bsAnnotType.Equal("FileAttachment"))
		)
	{
		CFX_ByteString bsIconName = pAnnotDict->GetString(FSANNOT_NAMEKEY_ICONNAME, bsAnnotType == FX_BSTRC("Text") ? FX_BSTRC("Note") : FX_BSTRC("PushPin"));
		CFX_Matrix matrix(m_pMatrix->a, m_pMatrix->b, m_pMatrix->c, m_pMatrix->d, m_pMatrix->e, m_pMatrix->f);
		CPDF_Matrix outMatrix;
		FSAnnot_Util_GetFixedMatrix(rotate, bsAnnotType, bsIconName, annotRect, &matrix, outMatrix);
		matrix = outMatrix;
		annotMatrix = matrix;
	}

	if(bsAnnotType.Equal("Widget"))
	{
		FSForm* form = pPage->getDocument()->getForm();
		if(form)
		{
			FSFormFillerImp* formFiller = (FSFormFillerImp*)form->getFormFiller();
			if(formFiller)
			{
				if(formFiller->GetFocusAnnot() != fsAnnot)
				{
					IFSPDF_AnnotHandler* pAnnotHandler = formFiller->GetWidgetHandler();
					if(pAnnotHandler)
					{
						FSCRT_LOCKOBJECT(lock, &formFiller->m_lock);
						FSRectI rcWindow = {0, 0,0,0};
						pAnnotHandler->OnDraw(pPage, this, fsAnnot, &annotMatrix, rcWindow,
							fsAnnot->getFlags());
						return TRUE;
					}
				}
				else
					//Don't draw the focused annot.
					return TRUE;
			}
		}
	}

	// If the annotation's AP is invalid, we should reset AP, EXCEPT link annot.
	// For link annot, if it has /AP, render it; if not have /AP, not reset its ap here. (SDK-7184)
	// Here, we do not need to check whether the annotation is support because it can not be also 
	// render if its AP is invalid.(FSPDFAnnotImp's resetAP will return FALSE);
	if (FALSE == bsAnnotType.Equal(FSPDF_ANNOTTYPE_LINK) && FALSE == FSAnnot_Util_IsAppearanceValid(pAnnotDict))
	{
		FSAnnot* annotTmp = NULL;
		if(bsAnnotType.Equal(FSPDF_ANNOTTYPE_NOTE))
			annotTmp = (FSNote*)FX_NEW FSPDFNoteImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_UNDERLINE))
			annotTmp = (FSUnderline*)FX_NEW FSPDFUnderlineImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_HIGHLIGHT))
			annotTmp = (FSHighlight*)FX_NEW FSPDFHighlightImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_SQUIGGLY))
			annotTmp = (FSSquiggly*)FX_NEW FSPDFSquigglyImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_STRIKEOUT))
			annotTmp = (FSStrikeOut*)FX_NEW FSPDFStrikeOutImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_SQUARE))
			annotTmp = (FSSquare*)FX_NEW FSPDFSquareImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_CIRCLE))
			annotTmp = (FSCircle*)FX_NEW FSPDFCircleImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_FREETEXT))
			annotTmp = (FSFreeText*)FX_NEW FSPDFFreeTextImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_STAMP))
			annotTmp = (FSStamp*)FX_NEW FSPDFStampImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_CARET))
			annotTmp = (FSCaret*)FX_NEW FSPDFCaretImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_INK))
			annotTmp = (FSInk*)FX_NEW FSPDFInkImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_LINE))
			annotTmp = (FSLine*)FX_NEW FSPDFLineImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_POLYGON))
			annotTmp = (FSPolygon*)FX_NEW FSPDFPolygonImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_POLYLINE))
			annotTmp = (FSPolyLine*)FX_NEW FSPDFPolyLineImp(pAnnotDict, pPage);
		else if (bsAnnotType.Equal(FSPDF_ANNOTTYPE_FILEATTACHMENT))
			annotTmp = (FSFileAttachment*)FX_NEW FSFileAttachmentImp(pAnnotDict, pPage);
		else
			annotTmp = (FSAnnot*)FX_NEW FSPDFAnnotImp(pAnnotDict, pPage);

		if (!annotTmp) 
			return FALSE;
		FS_BOOL bRet = annotTmp->resetAppearanceStream();
		delete annotTmp;
		
		if (!bRet)
			return bRet;
	}

	return pAnnot->DrawAppearance(m_pCurRenderPage->getPage(), m_pRenderDevice, &annotMatrix, CPDF_Annot::Normal, m_pRenderOptions);
}

FS_PROGRESSSTATE FSRendererImp::startRenderImp(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause, FSAnnot* pAnnot)
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

    m_pAnnot = pAnnot;
	m_pCurRenderPage = ((FSPDFPageImp*)page);

	m_pRenderOptions = FX_NEW CPDF_RenderOptions();
	if (!m_pRenderOptions) 
		return FSCommonDefines::e_progressError;
	m_pRenderOptions->m_Flags = m_renderFlags;
	FX_INT32 a = 0;
	ArgbDecode(m_foreColor, a, m_pRenderOptions->m_ForeColor);
	ArgbDecode(m_backColor, a, m_pRenderOptions->m_BackColor);
	if (m_colorMode == e_colorModeMapping)
		m_pRenderOptions->m_ColorMode = RENDER_COLOR_GRAY;

	m_pRenderContext = FX_NEW CPDF_RenderContext();
	if (!m_pRenderContext) 
		return FSCommonDefines::e_progressError;
	m_pRenderContext->Create(m_pCurRenderPage->getPage());
	if (!m_pRenderOptions->m_pOCContext) {
		CPDF_OCContext::UsageType eUsageType = CPDF_OCContext::View;
		FSPDFDocImp* pDocImp= (FSPDFDocImp*)((FSPDFPageImp*)page)->getDocument();
		m_pRenderOptions->m_pOCContext = FX_NEW CPDF_OCContext(pDocImp->getPDFDocument(), eUsageType);
		if (!m_pRenderOptions->m_pOCContext)
			return FSCommonDefines::e_progressError;
	}
	m_pMatrix = FX_NEW CFX_Matrix;
	m_pMatrix->Set(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f);

	//If render page, first, append page to render context.
	if (NULL == pAnnot && e_renderPage == (m_dwRenderContentFlag & e_renderPage))
		m_pRenderContext->AppendObjectList(m_pCurRenderPage->getPage(), m_pMatrix);

	m_pRenderOptions->m_Flags |= RENDER_LIMITEDIMAGECACHE;
	void* lpValue = NULL;
	if (CFS_Library::get()->getConfig((void*)FSPDF_CONFIG_ID_CACHESIZE, lpValue))
	{
		FX_DWORD dwLimitImageCacheSize = (FX_DWORD)(FX_UINTPTR)lpValue;
		m_pRenderOptions->m_dwLimitCacheSize = dwLimitImageCacheSize;
	}
	m_pRenderDevice->StartRendering();

	if (pause) {
		m_pRenderPause = FX_NEW CFSCRT_Pause(pause);
		if (!m_pRenderPause) 
			return FSCommonDefines::e_progressError;
	}

	m_pProgressRender = FX_NEW CPDF_ProgressiveRenderer();
	if (!m_pProgressRender) 
		return FSCommonDefines::e_progressError;

	m_pProgressRender->Start(m_pRenderContext, m_pRenderDevice, m_pRenderOptions, m_pRenderPause, FALSE);

	FS_PROGRESSSTATE state =  getRenderProgressState();
	if(state == FSCommonDefines::e_progressFinished)
		state = renderAnnots(page, pAnnot);
	return state;
}

FS_PROGRESSSTATE FSRendererImp::startRender(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause)
{
	return startRenderImp(page, matrix, pause, NULL);
}

FS_PROGRESSSTATE FSRendererImp::continueRender()
{
	FS_PROGRESSSTATE state = getRenderProgressState();

	if (state != FSCommonDefines::e_progressToBeContinued)
		return state;

	FSCRT_LOCKOBJECT(lock, &m_lock);
	if (m_pProgressRender)
		m_pProgressRender->Continue(m_pRenderPause);
	else 
		return FSCommonDefines::e_progressError;

	state =  getRenderProgressState();
	if(state == FSCommonDefines::e_progressFinished)
		state = renderAnnots(m_pCurRenderPage, NULL);
	return state;
}

FS_BOOL	FSRendererImp::renderAnnot(FSAnnot* annot, const FSMatrix& matrix)
{
	if (NULL == annot) return FALSE;
	FSPDFPageImp* pAnnotPage = (FSPDFPageImp*)annot->getPage();
	if (m_pCurRenderPage && m_pCurRenderPage->getPage())
	{
		FSPDFDocImp* pAnnotDoc = (FSPDFDocImp*)pAnnotPage->getDocument();
		if (pAnnotDoc != m_pCurRenderPage->getDocument()
			|| pAnnotPage->getDict()->getObjNum() != m_pCurRenderPage->getPage()->m_pFormDict->GetObjNum())
			return FALSE;
	}

	FS_PROGRESSSTATE state = startRenderImp(pAnnotPage, matrix, NULL, annot);
	FS_BOOL bRet = FALSE;
	if (FSCommonDefines::e_progressFinished == state)
		bRet = TRUE;
	return bRet;
}

void FSRendererImp::setRenderContent(FS_DWORD renderContentFlag)
{
	m_dwRenderContentFlag = renderContentFlag;
}

void FSRendererImp::setMappingModeColors(FS_ARGB backColor, FS_ARGB foreColor)
{
	m_backColor = backColor;
	m_foreColor = foreColor;
}

void FSRendererImp::setColorMode(FS_RENDERCOLORMODE colorMode)
{
	if (colorMode != e_colorModeNormal && colorMode != e_colorModeMapping) //todo..
		return;

	m_colorMode = colorMode;
}

/** @brief	Use ClearType-like anti-aliasing to render text objects, default is TRUE. */
void FSRendererImp::setClearType(FS_BOOL bClearType)
{
	if (bClearType){
		m_renderFlags |= e_renderClearType;
	}else{
		m_renderFlags &= ~e_renderClearType;
	}
}

/** @brief	Always output text as graphics (path or bitmap), don't allow device font substitution. */
void FSRendererImp::setPrintGraphicText(FS_BOOL bPrintGraphicText)
{
	if (bPrintGraphicText){
		m_renderFlags |= e_renderPrintGraphicText;
	}else{
		m_renderFlags &= ~e_renderPrintGraphicText;
	}
}
/** @brief	Always use downsampling for image stretching. */
void FSRendererImp::setForceDownSample(FS_BOOL bForceDownSample)
{
	if (bForceDownSample)
		m_renderFlags |= e_renderForceDownSample;
	else
		m_renderFlags &= ~e_renderForceDownSample;
}
/** @brief	Print preview mode. */
void FSRendererImp::setPrintPreview(FS_BOOL bPrintPreview)
{
	if (bPrintPreview)
		m_renderFlags |= e_renderPrintPreview;
	else
		m_renderFlags &= ~e_renderPrintPreview;
}
/** @brief	Always use halftone for image stretching. */
void FSRendererImp::setForceHalftone(FS_BOOL bForceHalftone)
{
	if (bForceHalftone)
		m_renderFlags |= e_renderForceHalftone;
	else
		m_renderFlags &= ~e_renderForceHalftone;
}
/** @brief	Always use anti-aliasing for rectangle drawing. */
void FSRendererImp::setRectAntiAliasing(FS_BOOL bRectAntiAliasing)
{
	if (bRectAntiAliasing)
		m_renderFlags |= e_renderRectAntiAliasing;
	else
		m_renderFlags &= ~e_renderRectAntiAliasing;
}
/** @brief	Always output text as bitmap in less than 50 font size case, use for epson stylus printer or other specified printers. */
void FSRendererImp::setPrintImageText(FS_BOOL bPrintImageText)
{
	if (bPrintImageText)
		m_renderFlags |= e_renderPrintImageText;
	else
		m_renderFlags &= ~e_renderPrintImageText;
}
/** @brief	Using overprint to render page. */
void FSRendererImp::setOverPrint(FS_BOOL bOverPrint)
{
	if (bOverPrint)
		m_renderFlags |= e_renderOverPrint;
	else
		m_renderFlags &= ~e_renderOverPrint;
}
/** @brief	Using thin line for path drawing. */
void FSRendererImp::setThinLine(FS_BOOL bThinLine)
{
	if (bThinLine)
		m_renderFlags |= e_renderThinLine;
	else
		m_renderFlags &= ~e_renderThinLine;
}
/** @brief	enhance zero line. */
void FSRendererImp::setEnhanceZeroLine(FS_BOOL bEnhanceZeroLine)
{
	if (bEnhanceZeroLine)
		m_renderFlags |= e_renderEnhanceZeroLine;
	else
		m_renderFlags &= ~e_renderEnhanceZeroLine;
}
/** @brief	Do not use anti-aliasing to render text objects, this OPTION doesn't set in default. */
void FSRendererImp::setTextSmooth(FS_BOOL bTextSmooth)
{
	if (bTextSmooth)
		m_renderFlags &= ~e_renderNoTextSmooth;
	else
		m_renderFlags |= e_renderNoTextSmooth;
}
/** @brief	Do not use anti-aliasing to render path objects, this OPTION doesn't set in default. */
void FSRendererImp::setPathSmooth(FS_BOOL bPathSmooth)
{
	if (bPathSmooth)
		m_renderFlags &= ~e_renderNoPathSmooth;
	else
		m_renderFlags |= e_renderNoPathSmooth;
}
/** @brief	Do not smooth images when render image objects, this OPTION doesn't set in default. */
void FSRendererImp::setImageSmooth(FS_BOOL bImageSmooth)
{
	if (bImageSmooth)
		m_renderFlags &= ~e_renderNoImageSmooth;
	else
		m_renderFlags |= e_renderNoImageSmooth;
}

/** @brief	The text knockout is true. */
void FSRendererImp::setTextKnockout(FS_BOOL bTextKnockout)
{
	if (bTextKnockout)
		m_renderFlags |= e_renderTextKnockout;
	else
		m_renderFlags &= ~e_renderTextKnockout;
}

CFX_RenderDevice* FSRendererImp::getRenderDevice()
{
	return m_pRenderDevice;
}

void FSRendererImp::setTransformAnnotIcon(FS_BOOL bTransformAnnotIcon)
{
	m_bTransformAnnotIcon = bTransformAnnotIcon;
}
