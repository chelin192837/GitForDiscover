#ifndef _OFD_RENDERER_R_H_
#define _OFD_RENDERER_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	渲染状态定义
 */
#define OFD_RENDERSTATE_Reset		0	//ready to start rendering
#define OFD_RENDERSTATE_Begin		1   //begin render
#define OFD_RENDERSTATE_Paused		2	//paused
#define OFD_RENDERSTATE_Done		3	//done
#define OFD_RENDERSTATE_Failed		4	//error

#define OFD_RENDER_COLORMODE_NORMAL		0
#define OFD_RENDER_COLORMODE_GRAY		1
#define OFD_RENDER_COLORMODE_TWOCOLOR	2

#define OFD_RENDER_NOCACHERESOURCE		0x20
#define OFD_RENDER_PRINTSUBIMAGE		0x40
#define OFD_RENDER_DISPLAYTHUMBNAIL		0x80

#define OFDDIB_BLEND_NORMAL			0
#define OFDDIB_BLEND_MULTIPLY		1

typedef struct _OFD_RENDEROPTIONS_ {
	OFD_BOOL		bIsPrinting;
	int				colorMode;
	OFD_ARGB		backColor;
	OFD_ARGB		foreColor;
	int				flags;
} OFD_RENDEROPTIONS;

/**
 * @brief  创建一个渲染器。
 *
 * @param[in]	dib			位图对象句柄。	
 * @return 渲染器句柄。
 *				
 */
OFD_RENDERER	OFD_Renderer_CreateOnBitmap(OFD_BITMAP dib);

/**
 * @brief  删除渲染器，并释放内存。
 *
 * @param[in]	renderer	渲染器句柄。	
 * @return 无。
 *				
 */
void	OFD_Renderer_Release(OFD_RENDERER renderer);

/**
 * @brief  创建渲染上下文的对象。
 * @return 渲染上下文的对象。
 *				
 */
OFD_RENDERCONTEXT	OFD_RenderContext_Create();

/**
 * @brief  删除渲染上下文的对象，并释放内存。
 *
 * @param[in]	renderContext	渲染上下文的对象。	
 * @return 无。
 *				
 */
void	OFD_RenderContext_Release(OFD_RENDERCONTEXT renderContext);

/**
 * @brief  设置上下文矩阵。
 *
 * @param[in]	renderContext	渲染上下文的对象。
 * @param[in]	matrix			矩阵对象。	
 * @return 无。
 *				
 */
void	OFD_RenderContext_SetMatrix(OFD_RENDERCONTEXT renderContext, OFD_MATRIX matrix);

/**
 * @brief  设置渲染选项。
 *
 * @param[in]	renderContext	渲染上下文的对象。
 * @param[in]	options			渲染选项。
 * @return 无。
 *				
 */
void	OFD_RenderContext_SetOption(OFD_RENDERCONTEXT renderContext, OFD_RENDEROPTIONS* options);

/**
 * @brief  开始页面渲染。
 *
 * @param[in]	renderContext		渲染上下文的对象。
 * @param[in]	renderer			渲染器对象。
 * @param[in]	page				页面对象。
 * @param[in]	renderProgress		步进式渲染对象。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL	OFD_RenderContext_StartPage(OFD_RENDERCONTEXT renderContext, OFD_RENDERER renderer, OFD_PAGE page, OFD_PROGRESS* renderProgress);

/**
 * @brief  渲染页面所有内容。
 *
 * @param[in]	progress			步进式渲染对象。
 * @return 返回渲染状态。
 *				
 */
int	OFD_Progress_Continue(OFD_PROGRESS progress);

/**
 * @brief  渲染页面高亮标注。
 *
 * @param[in]	progress			步进式渲染对象。
 * @param[in]	blendMode			混合模式。
 * @return 返回渲染状态。
 *				
 */
int		OFD_Progress_Continue_HighLightAnnots(OFD_PROGRESS progress, int blendMode);

/**
 * @brief  渲染页面普通标注（非高亮）。
 *
 * @param[in]	progress			步进式渲染对象。
 * @param[in]	blendMode			混合模式。
 * @return 返回渲染状态。
 *				
 */
int		OFD_Progress_Continue_CommonAnnots(OFD_PROGRESS progress, int blendMode);

/**
 * @brief  渲染页面文字图像。
 *
 * @param[in]	progress			步进式渲染对象。
 * @return 返回渲染状态。
 *				
 */
int		OFD_Progress_Continue_Contents(OFD_PROGRESS progress);

/**
 * @brief  渲染页面印章。
 *
 * @param[in]	progress			步进式渲染对象。
 * @param[in]	bSealAllGray		是否灰度。
 * @return 返回渲染状态。
 *				
 */
int 	OFD_Progress_Continue_StampAnnots(OFD_PROGRESS progress, OFD_BOOL bSealAllGray);

#ifdef __cplusplus
};
#endif

#endif // _OFD_RENDERER_R_H_
