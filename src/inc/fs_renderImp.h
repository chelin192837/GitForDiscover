#ifndef _FS_RENDER_IMP_H_
#define _FS_RENDER_IMP_H_

#include "../../include/fs_render.h"

class CFSCRT_Pause;
class CFX_DIBitmap;
class CFX_RenderDevice;
class CPDF_RenderOptions;
class CPDF_RenderContext;
class CPDF_ProgressiveRenderer;
class CPDF_AnnotList;
class CPDF_Annot;
class CPDF_Page;
class CFX_Matrix;
class FSFormFillerImp;
class FSPDFPageImp;

class FSBitmapImp: public FSBitmap, public CFX_Object
{
public:
	static FSBitmapImp*	create(FS_INT32 width, FS_INT32 height, FS_DIBFORMAT format, FS_LPBYTE buffer, int pitch);
	
	virtual ~FSBitmapImp();
	virtual void			release();
	virtual FSBitmapImp*	clone(const FSRectI* clip) const;
	virtual FS_INT32		getWidth() const;
	virtual FS_INT32		getHeight() const;
	virtual FS_INT32		getPitch() const;
	virtual FS_INT32		getBpp() const;
	virtual FS_LPBYTE		getBuffer() const;
	virtual FS_DIBFORMAT	getFormat() const;
	virtual void			fillRect(FS_ARGB color, const FSRectI* rect);

	//Internal use
	CFX_DIBitmap*		getBitmap() { return m_pBitmap; }
	void				setBitmap(CFX_DIBitmap* pBitmap) { m_pBitmap = pBitmap;}

protected:
	FSBitmapImp();
private:
	//friend class FSRenderer;
	//friend class FSPDFPage;
	CFX_DIBitmap*	m_pBitmap;
};

class FSRendererImp: public FSRenderer, public CFX_Object
{
public:
	/** @brief	Create a FSRenderer from bitmap.*/
	static FSRendererImp*	create(FSBitmap* bitmap, FS_BOOL rgbOrder);
#ifdef __APPLE__
	/** @brief	Create a FSRenderer from device context.*/
	static FSRendererImp*	createFromContext(CGContextRef context, FS_DEVICETYPE deviceType);
#endif
	virtual void release();
	virtual ~FSRendererImp();

	/** @brief	Start rendering a pdf page.
	* @details	It will take a long time to render a page with complex or large contents, so Foxit PDF SDK uses a progressive process to do this.<br>
	* 			Caller continueRender to continue rendering in steps.
	* 
	* @param[in]	page			A PDF page handle. It should be a valid handle of PDF page that has been parsed.
	* @param[in]	isRenderAnnot	Whether or not render annotations. 
	* @param[in]	matrix			The transformation matrix to display page.
	* @param[out]	pause			If it is <b>NULL</b> or FSPause::NeedPauseNow is <b>NULL</b>, this function will not return until it is finished or an error occurs.
	*/
	virtual FS_PROGRESSSTATE	startRender(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause);
	/** @brief	Continue rendering a pdf page.*/
	virtual FS_PROGRESSSTATE	continueRender();

	virtual	FS_BOOL				renderAnnot(FSAnnot* annot, const FSMatrix& matrix);

	/**
	 *@brief	Set render flag to decide what content will be rendered. Default value: (e_renderContentPage | e_renderContentAnnot).
	 */
	virtual void	setRenderContent(FS_DWORD renderContentFlag);
	/** 
	 * @brief	Set a boolean value which indicates whether to transform annotation icon or not when display. Default value: TRUE.
	 * 
	 * @details	This flag is only useful for note and file attachment annotations.
	 */
	virtual void	setTransformAnnotIcon(FS_BOOL bTransformAnnotIcon);
	/** @brief	Set color mode. Default value: e_colormodeNormal.*/
	virtual void	setColorMode(FS_RENDERCOLORMODE colorMode);
	/** @brief	Set background color and foreground color to render when color mode is set e_colorModeMapping.*/
	virtual void	setMappingModeColors(FS_ARGB backColor, FS_ARGB foreColor);
	/** @brief	Use ClearType-like anti-aliasing to render text objects. Default value: TRUE. */
	virtual void	setClearType(FS_BOOL bClearType);
	/** @brief	Always output text as graphics (path or bitmap), don't allow device font substitution. */
	virtual void	setPrintGraphicText(FS_BOOL bPrintGraphicText);
	/** @brief	Always use downsampling for image stretching. */
	virtual void	setForceDownSample(FS_BOOL bForceDownSample);
	/** @brief	Print preview mode. */
	virtual void	setPrintPreview(FS_BOOL bPrintPreview);
	/** @brief	Always use halftone for image stretching. */
	virtual void	setForceHalftone(FS_BOOL bForceHalftone);
	/** @brief	Always use anti-aliasing for rectangle drawing. */
	virtual void	setRectAntiAliasing(FS_BOOL bRectAntiAliasing);
	/** @brief	Always output text as bitmap in less than 50 font size case, use for epson stylus printer or other specified printers. */
	virtual void	setPrintImageText(FS_BOOL bPrintImageText);
	/** @brief	Using overprint to render page. */
	virtual void	setOverPrint(FS_BOOL bOverPrint);
	/** @brief	Using thin line for path drawing. */
	virtual void	setThinLine(FS_BOOL bThinLine);
	/** @brief	enhance zero line. */
	virtual void	setEnhanceZeroLine(FS_BOOL bEnhanceZeroLine);
	/** @brief	Do not use anti-aliasing to render text objects. Default value: TRUE. */
	virtual void	setTextSmooth(FS_BOOL bTextSmooth);
	/** @brief	Do not use anti-aliasing to render path objects. Default value: TRUE. */
	virtual void	setPathSmooth(FS_BOOL bPathSmooth);
	/** @brief	Do not smooth images when render image objects. Default value: TRUE. */
	virtual void	setImageSmooth(FS_BOOL bImageSmooth);
	/** @brief	The text knockout is true. */
	virtual void	setTextKnockout(FS_BOOL bTextKnockout);

	CFX_RenderDevice* getRenderDevice();
	CFX_Matrix* getMatrix() {return m_pMatrix;}
    FS_BOOL createFXDevice();
protected:
	FSRendererImp(FS_INT16 deviceType = -1);
	void releaseResources(FS_BOOL bReleaseProgressRender, FS_BOOL bReleaseDevice = TRUE);
	FS_PROGRESSSTATE startRenderImp(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause, FSAnnot* pAnnot);

private:
	FS_PROGRESSSTATE	getRenderProgressState();
	FS_PROGRESSSTATE	renderAnnots(FSPDFPage* page,FSAnnot* fsAnnot=NULL);
	FS_BOOL				renderAnnotToRenderer(CPDF_Annot* pAnnot, FSAnnot* fsAnnot, CPDF_AnnotList* pAnnotList, FSPDFPage* pPage);

private:
	FSCRT_DEFINELOCK(m_lock);
	void*		m_pInitParam;
	FS_BOOL		m_bRendererWithBitmap;
#ifdef __APPLE__
	FS_INT16			m_iDeviceType;
#endif

	CFX_RenderDevice*			m_pRenderDevice;
	CPDF_RenderOptions*			m_pRenderOptions;
	CPDF_ProgressiveRenderer*	m_pProgressRender;
	CPDF_RenderContext*			m_pRenderContext;
	CFSCRT_Pause*				m_pRenderPause;
	CPDF_AnnotList*				m_pAnnotList;
	FSPDFPageImp*				m_pCurRenderPage;
	CFX_Matrix*					m_pMatrix;

	FS_BOOL				m_bRgbOrder;
	FS_ARGB				m_backColor;
	FS_ARGB				m_foreColor;
	FS_DWORD			m_dwRenderContentFlag;
	FS_BOOL				m_bTransformAnnotIcon;
	FS_RENDERCOLORMODE	m_colorMode;
	FS_DWORD			m_renderFlags;

	FSFormFillerImp*	m_pFormFiller;
    FSAnnot*            m_pAnnot;
};

#endif //_FS_RENDER_IMP_H_
