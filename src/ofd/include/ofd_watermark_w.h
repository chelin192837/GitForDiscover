#ifndef _OFD_WATERMARK_W_H_
#define _OFD_WATERMARK_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建文字类型水印。
 *	
 * @param[in]	hPage			页对象句柄。	
 * @param[in]	lpwszText		文本。	
 * @param[in]	lpwszFont		字体名称。
 * @param[in]	fFontSize		字体大小(若设置字体大小为0,则字体大小默认为单行自适应)。
 * @param[in]	FontColor		字体颜色。
 * @param[in]	fWidth			图元对象框宽度。
 * @param[in]	fHeight			图元对象框高度。
 * @param[in]	fRotate			旋转角度。(暂不支持同时旋转和平铺)
 * @param[in]	alpha			透明值。
 * @return 成功返回水印句柄，失败返回0。				
 *				
 */
OFD_WATERMARKHELPER		OFD_WaterMarkHelper_CreateFormText(OFD_PAGE hPage, OFD_LPCWSTR lpwszText,
							OFD_LPCWSTR lpwszFont, float fFontSize, float fCharSpace, OFD_ARGB FontColor, float fWidth,
							float fHeight, float fRotate, OFD_BYTE alpha);
	
/**
 * @brief 创建图像类型水印。
 *
 * @param[in]	hPage			页对象句柄。	
 * @param[in]	lpwszImageFile	图像文件。
 * @param[in]	fImageWidth		图像宽。
 * @param[in]	fImageHeight	图像高。
 * @param[in]	pImageBuf		图像数据，如果设置图像文件参数，忽略。	
 * @param[in]	iImageBufLen	图像数据长度，如果设置图像文件参数，忽略。	
 * @param[in]	iImageFormat	图像数据类型，如果设置图像文件参数，忽略。	
 * @param[in]	fWidth			图元对象框宽度。
 * @param[in]	fHeight			图元对象框高度。
 * @param[in]	fRotate			旋转角度。(暂不支持同时旋转和平铺)
 * @return 成功返回水印句柄，失败返回0。				
 *				
 */
OFD_WATERMARKHELPER		OFD_WaterMarkHelper_CreateFromImage(OFD_PAGE hPage,
							OFD_LPCWSTR lpwszImageFile, float fImageWidth, float fImageHeight, OFD_LPCBYTE pImageBuf,
							int iImageBufLen, int iImageFormat, float fWidth, float fHeight, float fRotate);

/**
 * @brief 创建图像类型水印。
 *
 * @param[in]	hPage			页对象句柄。	
 * @param[in]	imgRes			图像资源ID，通过OFD_Document_AddImageResFromBuf或OFD_Document_AddImageResFromFile创建。
 * @param[in]	fImageWidth		图像宽。
 * @param[in]	fImageHeight	图像高。
 * @param[in]	fWidth			图元对象框宽度。
 * @param[in]	fHeight			图元对象框高度。
 * @param[in]	fRotate			旋转角度。(暂不支持同时旋转和平铺)
 * @return 成功返回水印句柄，失败返回0。				
 *				
 */
OFD_WATERMARKHELPER		OFD_WaterMarkHelper_CreateFromImage2(OFD_PAGE hPage,
							OFD_DWORD imgRes, float fImageWidth, float fImageHeight, float fWidth, float fHeight, float fRotate);

/**
 * @brief 插入水印对象。
 * @param[in]	hWaterMark		水印句柄。		
 * @param[in]	x				水印x坐标。
 * @param[in]	y				水印y坐标。
 * @param[in]	bTiled			是否平铺。(暂不支持同时旋转和平铺)
 * @param[in]	fXStep			X方向步长。
 * @param[in]	fYStep			Y方向步长。
 * @return  无。
 *				
 */
void		OFD_WaterMarkHelper_Generate(OFD_WATERMARKHELPER hWaterMark, float x, float y,
							OFD_BOOL bTiled, float fXStep, float fYStep);

#define  OFD_WATERMARKHELP_HALIGN_LEFT		0
#define  OFD_WATERMARKHELP_HALIGN_MID		1
#define  OFD_WATERMARKHELP_HALIGN_RIGHT		2

#define  OFD_WATERMARKHELP_VALIGN_TOP		0
#define  OFD_WATERMARKHELP_VALIGN_MID		1
#define  OFD_WATERMARKHELP_VALIGN_BUTTON	2
/**
 * @brief 按照对齐方式插入水印对象。
 * @param[in]	hWaterMark		水印句柄。		
 * @param[in]	hAlign			水平(0:居左，1：居中，2：居右)。
 * @param[in]	vAlign			垂直(0:居上，1：居中，2：居下)。
 * @return  无。
 *				
 */
void  OFD_WaterMarkHelper_Generate2(OFD_WATERMARKHELPER hWaterMark, int hAlign, int vAlign);

/**
 * @brief 插入水印对象。
 * @param[in]	hWaterMark		水印句柄。
 * @param[in]	position		水印显示位置。
 * @param[in]	x				水印x坐标。
 * @param[in]	y				水印y坐标。
 * @return  无。
 *				
 */
void		OFD_WaterMarkHelper_Generate3(OFD_WATERMARKHELPER hWaterMark, int position, float x, float y);

/**
 * @brief 释放水印对象。
 *
 * @param[in]	WaterMarkrk		水印句柄。	
 * @return 无。
 *
 */
void		OFD_WaterMarkHelper_Release(OFD_WATERMARKHELPER hWaterMark);

/**
 * @brief 获取水印文字宽。
 *
 * @param[in]	WaterMarkrk		水印句柄。	
 * @return 水印宽。
 *
 */
float		OFD_WaterMarkHelper_GetTextWidth(OFD_WATERMARKHELPER hWaterMark);

/**
 * @brief 获取水印文字高。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @return 水印高。
 *
 */
float		OFD_WaterMarkHelper_GetTextHeight(OFD_WATERMARKHELPER hWaterMark);

/**
 * @brief 根据设备dpi获取默认水印图像宽。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @param[in]	dpi				单位设备分辨率。
 * @return 水印宽。
 *
 */
float		OFD_WaterMarkHelper_GetImageWidth(OFD_WATERMARKHELPER hWaterMark, int dpi);

/**
 * @brief 根据设备dpi获取默认水印图像高。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @param[in]	dpi				单位设备分辨率。
 * @return 水印高。
 *
 */
float		OFD_WaterMarkHelper_GetImageHeight(OFD_WATERMARKHELPER hWaterMark, int dpi);

/**
 * @brief 更新文本水印外接矩形框宽度。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @param[in]	width			水印宽度。
 * @return 水印高。
 *
 */
void		OFD_WaterMarkHelper_Update_TextWidth(OFD_WATERMARKHELPER hWaterMark, float width);

/**
 * @brief 更新文本水印外接矩形框高度。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @param[in]	height			水印高度
 * @return 水印高。
 *
 */
void		OFD_WaterMarkHelper_Update_TextHeight(OFD_WATERMARKHELPER hWaterMark, float height);

/**
 * @brief 更新图像水印外接矩形框宽度。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @param[in]	width			水印宽度。
 * @return 水印高。
 *
 */
void		OFD_WaterMarkHelper_Update_ImageWidth(OFD_WATERMARKHELPER hWaterMark, float width);

/**
 * @brief 更新图像水印外接矩形框高度。
 *
 * @param[in]	WaterMarkrk		水印句柄。
 * @param[in]	height			水印高度
 * @return 水印高。
 *
 */
void		OFD_WaterMarkHelper_Update_ImageHeight(OFD_WATERMARKHELPER hWaterMark, float height);


#ifdef __cplusplus
}
#endif

#endif  //_OFD_WATERMARK_W_H_
