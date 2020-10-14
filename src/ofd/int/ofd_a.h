#ifndef _OFD_A_H_
#define _OFD_A_H_

#include "../include/ofd_base_r.h"

// Internal interface

// ASDK
void OFD_Page_PartBlack(OFD_PAGE hPage, OFD_RECT rect);

// Linear
OFD_DEFINEHANDLE(OFD_LINEAR);
OFD_DEFINEHANDLE(OFD_LINEARDATA);

OFD_LINEAR OFD_Linear_LoadFromFile(OFD_LPCWSTR lpwszFileName);
OFD_LINEARDATA OFD_Linear_OpenFile(OFD_LINEAR hLinear, OFD_LPCWSTR lpwszFilePath, int* size);
void OFD_Linear_Destroy(OFD_LINEAR hLinear);

int OFD_LinearData_Copy(OFD_LINEARDATA hLinearData, OFD_LPBYTE lpBuf, int* size);

// MRC
int OFD_MRC_Compress(OFD_LPCWSTR lpwszImageFile, OFD_LPCWSTR lpwszOFDFile, OFD_OPTIMIZERPARAM* pOptimizerParam);

/**
 * @brief 页面信息屏蔽
 *
 * @param[in] hPage		页句柄。
 * @param[in] rect		矩形区域。
 * @return 无。
 *				
 */
void		OFD_Page_InfoCover(OFD_PAGE hPage, OFD_RECT rect);

/**
 * @brief 遮蔽 OBJ 信息恢复
 *
 * @param[in] hDocument	文档句柄。
 * @return 无。
 *				
 */
void		OFD_Document_InfoReCover(OFD_DOCUMENT hDocument);

/**
 * @brief 删除所有签名。
 *
 * @param[in]	hDocument			文档句柄。
 * @param[in]	bChange2Image		删除签章，是否转换为图片标注
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Document_RemoveSignatures(OFD_DOCUMENT hDocument, OFD_BOOL bChange2Image = 0);

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
void		OFD_WaterMarkHelper_Generate_Pattern(OFD_WATERMARKHELPER hWaterMark, float x, float y,
							OFD_BOOL bTiled, float fXStep, float fYStep);

/**
 * @brief 多文件注释合并开始。
 * @param[in]	lpwszDstFile		目标文件名。		
 * @return  处理句柄。
 *				
 */
void*		FS_AnnotCombine_Start(OFD_LPCWSTR lpwszDstFile);

/**
 * @brief 多文件注释合并开始。
 * @param[in]	pFileRead		目标文件流。		
 * @return  处理句柄。
 *				
 */
void *FS_AnnotCombine_Stream_Start(IFX_FileRead *pFileRead);

/**
 * @brief 多文件注释合并添加注释文件。
 * @param[in]   handle				处理句柄
 * @param[in]	lpwszDstFile		注释文件名。		
 * @return  成功或失败。
 *				
 */
OFD_BOOL	 FS_AnnotCombine_Add(void* handle, OFD_LPCWSTR lpwszSrcFile);

/**
 * @brief 多文件注释合并添加注释文件。
 * @param[in]   handle				处理句柄
 * @param[in]	pFileRead		注释文件流。		
 * @return  成功或失败。
 *				
 */
OFD_BOOL FS_AnnotCombine_Stream_Add(void* handle, IFX_FileRead *pFileRead);

/**
 * @brief 多文件注释合并结束。
 * @param[in]   handle				处理句柄
 * @return  成功或失败。
 *				
 */
OFD_BOOL	FS_AnnotCombine_End(void* handle);

/**
 * @brief 多文件注释合并结束。
 * @param[in]   handle				处理句柄
 * @param[out]  pFileWrite			目标写文件流
 * @return  成功或失败。
 *				
 */
FX_BOOL FS_AnnotCombine_Stream_End(void* handle, IFX_FileWrite* pFileWrite);

#endif //_OFD_A_H_
