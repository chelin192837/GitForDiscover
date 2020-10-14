#ifndef _OFD_RENDERER_WINDOWS_R_H_
#define _OFD_RENDERER_WINDOWS_R_H_

#include "../ofd_base_r.h"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  ����һ����Ⱦ����windowsƽ̨����
 *
 * @param[in]	hDC			�豸�����ľ����
 * @param[in]	width		��	
 * @param[in]	height		�ߡ�	
 * @return ��Ⱦ�������
 *				
 */
OFD_RENDERER		OFD_Renderer_CreateOnWindowsDC(HDC hDC, int width, int height);

#ifdef __cplusplus
};
#endif

#endif // _OFD_RENDERER_WINDOWS_R_H_
