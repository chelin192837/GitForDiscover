#ifndef _OFD_IMAGE_R_H_
#define _OFD_IMAGE_R_H_

#include "ofd_base_r.h"

/**
 * @name	Macro Definitions for Bitmap Format
 */
/**@{*/

/** @brief	24bpp format, bits order: Blue, Green, Red. Blue is the lowest order. */
#define OFD_BITMAPFORMAT_24BPP_BGR		1
/** @brief	32bpp format, bits order: Blue, Green, Red, not used. Blue is the lowest order. */
#define OFD_BITMAPFORMAT_32BPP_BGRx		2
/** @brief	32bpp format, bits order: Blue, Green, Red, Alpha. Blue is the lowest order. */
#define OFD_BITMAPFORMAT_32BPP_BGRA		3
/** @brief	8bpp format, gray scale. */
#define OFD_BITMAPFORMAT_8BPP_GRAY		4
/** @brief	24bpp format, bits order: Red, Green, Blue. Red is the lowest order. */
#define OFD_BITMAPFORMAT_24BPP_RGB		5
/** @brief	32bpp format, bits order: Red, Green, Blue, not used. Red is the lowest order. */
#define OFD_BITMAPFORMAT_32BPP_RGBx		6
/** @brief	32bpp format, bits order: Red, Green, Blue, Alpha. Red is the lowest order. */
#define OFD_BITMAPFORMAT_32BPP_RGBA		7
/**@}*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建一个位图对象（使用外部内存）。
 *
 * @param[in] width			位图宽。
 * @param[in] height		位图高。
 * @param[in] format		位图类型。
 * @param[in] buffer		位图数据。
 * @param[in] stride		行间距（以字节为单位）。
 * @return 成功返回位图对象句柄，失败返回NULl。
 *				
 */
OFD_BITMAP	OFD_Bitmap_Create(int width, int height, int format, void* buffer, int stride);

/**
 * @brief 创建一个位图对象。
 *
 * @param[in] width			位图宽。
 * @param[in] height		位图高。
 * @param[in] format		位图类型。
 * @param[in] buffer		位图数据。
 * @param[in] stride		行间距（以字节为单位）。
 * @return 成功返回位图对象句柄，失败返回NULl。
 *				
 */
OFD_BITMAP	OFD_Bitmap_Create2(int width, int height, int format);

/**
 * @brief 渲染一个对象，仅限在linux下使用cairo渲染。
 *
 * @param[in] pDIB_			CFX_DibBitmap。
 * @param[in] hPage			页面对象。
 * @param[in] dpi			默认96。
 * @return。
 *				
 */
void RenderPage_Cairo(OFD_BITMAP pDIB_, OFD_PAGE hPage, int dpi = 96);

/**
 * @brief 位图对象保存至PNG文件。
 *
 * @param[in] dib			位图对象句柄。
 * @param[in] file			保存文件路径。
 * @return 成功返回0。
 *				
 */
int	OFD_Bitmap_EncodePngFormat(OFD_BITMAP dib, OFD_LPCWSTR file);

/**
 * @brief 释放。
 *
 * @param[in] dib			位图对象句柄。
 * @return 无。
 *				
 */
void	OFD_Bitmap_Release(OFD_BITMAP dib);

#ifdef __cplusplus
}
#endif

#endif //_OFD_IMAGE_R_H_
