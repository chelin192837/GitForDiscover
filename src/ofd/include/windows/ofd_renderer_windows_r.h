#ifndef _OFD_RENDERER_WINDOWS_R_H_
#define _OFD_RENDERER_WINDOWS_R_H_

#include "../ofd_base_r.h"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  创建一个渲染器（windows平台）。
 *
 * @param[in]	hDC			设备上下文句柄。
 * @param[in]	width		宽。	
 * @param[in]	height		高。	
 * @return 渲染器句柄。
 *				
 */
OFD_RENDERER		OFD_Renderer_CreateOnWindowsDC(HDC hDC, int width, int height);

#ifdef __cplusplus
};
#endif

#endif // _OFD_RENDERER_WINDOWS_R_H_
