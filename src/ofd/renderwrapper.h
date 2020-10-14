#ifndef _RENDER_WRAPPER_H_
#define _RENDER_WRAPPER_H_

#include "fr_include.h"
#include "../../include/fs_render.h"
#include "../fs_pause.h"



class RenderWrapper: public FSRenderer, public CFX_Object
{
protected:
	RenderWrapper();
	~RenderWrapper();

public:
	/** 
	 * @brief	Create a renderer object from bitmap.
	 *
	 * @param[in]	bitmap		A bitmap used for rendering.
	 * @param[in]	rgbOrder	<b>TRUE</b> means Foxit PDF SDK needs to use RGB byte-order 
	 *							(Red is in the lowest order) when rendering. <br>
	 *							<b>FALSE</b> means Foxit PDF SDK needs to use BGR byte-order 
	 *							(Blue is in the lowest order) when rendering.
	 *
	 * @return	A new renderer object.
	 */
	static RenderWrapper*	create(FSBitmap* bitmap, FS_BOOL rgbOrder);
#ifdef __APPLE__
	/** 
	 * @brief	Create a renderer object from device context.
	 *
	 * @param[in]	context		A CGContextRef object.
	 * @param[in]	deviceType	The device type of parameter <i>context</i>.
	 *
	 * @return	A new renderer object.
	 */
	static RenderWrapper*	createFromContext(CGContextRef context, FS_DEVICETYPE deviceType);
#endif

#ifndef _SWIG_IOS_
	/** 
	 * @brief	Release current renderer object.
	 *
	 * @return	None.
	 */
	virtual void release();
#endif
	/** 
	 * @brief	Start rendering a PDF page.
	 *
	 * @details	It will take a long time to render a page with complex or large contents, so Foxit PDF SDK uses a progressive process to do this.<br>
	 * 			If the rendering is not finished, please call function {@link FS_Renderer::continueRender} to continue the rendering until it is finished.
	 * 
	 * @param[in]	page			A PDF page. It should be parsed.
	 * @param[in]	matrix			The transformation matrix used for rendering, which is usually returned by function {@link FSPDFPage::getDisplayMatrix}.
	 * @param[in]	pause			Pause object which decides if the rendering process needs to be paused.
	 *								This can be <b>NULL</b> which means not to pause during the rendering process.
	 *								If this is not <b>NULL</b>, it should be a valid pause object implemented by user.
	 *
	 * @return	{@link FS_PROGRESSSTATE::e_progressFinished} means the rendering is finished successfully.<br>
	 *			{@link FS_PROGRESSSTATE::e_progressToBeContinued} means the rendering process is not finished yet and function {@link FSRenderer::continueRender} should be called to continue the process.
	 *			{@link FS_PROGRESSSTATE::e_progressError} means any error occurs.
	 */
	virtual FS_PROGRESSSTATE	startRender(FSPDFPage* page, const FSMatrix& matrix, FSPause* pause);
	/** 
	 * @brief	Continue rendering process.
	 *
	 * @return	{@link FS_PROGRESSSTATE::e_progressFinished} means the rendering is finished successfully.<br>
	 *			{@link FS_PROGRESSSTATE::e_progressToBeContinued} means the rendering process is not finished yet and function {@link FSRenderer::continueRender} should be called to continue the process.
	 *			{@link FS_PROGRESSSTATE::e_progressError} means any error occurs.
	 */
	virtual FS_PROGRESSSTATE	continueRender();

	/** 
	 * @brief	Render a specified annotation.
	 *
	 * @details	When this function is called, render flag (set by function {@link FSRenderer::setRenderContent} or by default) will be ignored.
	 *
	 * @param[in]	annot		An annotation object to be rendered.
	 * @param[in]	matrix		The transformation matrix used for rendering, which is usually returned by function {@link FSPDFPage::getDisplayMatrix}.
	 *
	 * @return	<b>TRUE</b> means success, while <b>FALSE</b> means failure.
	 */
	virtual	FS_BOOL				renderAnnot(FSAnnot* annot, const FSMatrix& matrix);

	/**
	 * @brief	Set render flag to decide what content will be rendered. 
	 *
	 * @details	If this function is not called, default value ({@link FS_RENDERCONTENTFLAG::e_renderPage} | {@link FS_RENDERCONTENTFLAG::e_renderAnnot}) will be used.
	 *
	 * @param[in]	renderContentFlag		Render content flags.
	 *										Please refere to {@link FS_RENDERCONTENTFLAG::e_renderPage FS_RENDERCONTENTFLAG::e_renderXXX} values and this should be one or a combination of these values.
	 *
	 * @return	None.
	 */
	virtual void	setRenderContent(FS_DWORD renderContentFlag);
	/** 
	 * @brief	Decide whether to transform annotation icon or not when display.
	 *
	 * @details	The flag set by this function is only useful for note and file attachment annotations.
	 *			If this function is not called, default value <b>TRUE</b> will be used.
	 *
	 * @param[in]	transformAnnotIcon			<b>TRUE</b> means to transform annotation icon when display.
	 *											<b>FALSE</b> means not to transform annotation icon when display.
	 *
	 * @return	None.
	 */
	virtual void	setTransformAnnotIcon(FS_BOOL transformAnnotIcon);
	/** 
	 * @brief	Set color mode.
	 *
	 * @details	If this function is not called, default value {@link FS_RENDERCOLORMODE::e_colorModeNormal} will be used.
	 *
	 * @param[in]	colorMode	Color mode value. It should be one of following values:
	 *							<ul>
	 *							<li>{@link FS_RENDERCOLORMODE::e_colorModeNormal} means normal color mode.</li>
	 *							<li>{@link FS_RENDERCOLORMODE::e_colorModeMapping} means map a color value to the color range defined by a background color and a foreground color.
	 *								If this mode is used, please call function {@link FSRenderer::setColorMode} to set "background color" and "foreground color".</li>
	 *							</ul>
	 *
	 * @return	None.
	 */
	virtual void	setColorMode(FS_RENDERCOLORMODE colorMode);
	/** 
	 * @brief	Set background color and foreground color when color mode is set {@link FS_RENDERCOLORMODE::e_colorModeMapping}.
	 *
	 * @param[in]	backGroundColor		The background color.
	 * @param[in]	foreGroundColor		The foreground color. 
	 *
	 * @return	None.
	 */
	virtual void	setMappingModeColors(FS_ARGB backgroundColor, FS_ARGB foregroundColor);
#ifndef _SWIG_ANDROID_
	/** 
	 * @brief	Decide whether to use ClearType-like anti-aliasing to render text objects. 
	 *
	 * @details	If this function is not called, default value <b>TRUE</b> will be used.
	 *
	 * @param[in]	bClearType		<b>TRUE</b> means to use ClearType-like anti-aliasing to render text objects.
	 *								<b>FALSE</b> means not to use ClearType-like anti-aliasing to render text objects
	 *
	 * @return	None.
	 */
	virtual void	setClearType(FS_BOOL bClearType);
	/** 
	 * @brief	Decide whether to output text as graphics (path or bitmap) and do not allow device font substitution.
	 *
	 * @details	If this function is not called, default value <b>FALSE</b> will be used.
	 *
	 * @param[in]	bPrintGraphicText	<b>TRUE</b> means to output text as graphics (path or bitmap) and do not allow device font substitution.
	 *									<b>FALSE</b> means not to output text as graphics.
	 *
	 * @return	None.
	 */
	virtual void	setPrintGraphicText(FS_BOOL bPrintGraphicText);
	/** 
	 * @brief	Decide whether to use downsampling for image stretching.
	 *
	 * @details	If this function is not called, default value <b>FALSE</b> will be used.
	 *
	 * @param[in]	bForceDownSample	<b>TRUE</b> means to use downsampling for image stretching.
	 *									<b>FALSE</b> means not to use downsampling for image stretching.
	 *
	 * @return	None.
	 */
	virtual void	setForceDownSample(FS_BOOL bForceDownSample);
#endif
	/** 
	 * @brief	Decide whether to use halftone for image stretching.
	 *
	 * @details	If this function is not called, default value <b>TRUE</b> will be used.
	 *
	 * @param[in]	bForceHalftone		<b>TRUE</b> means to use halftone for image stretching.
	 *								<b>FALSE</b> means not to use halftone for image stretching.
	 *
	 * @return	None.
	 */
	virtual void	setForceHalftone(FS_BOOL bForceHalftone);

protected:
	FSRenderer*		m_CurrentRender;
	FSRenderer*		m_PDFRender;
	FSRenderer*		m_OFDRender;
};


#endif