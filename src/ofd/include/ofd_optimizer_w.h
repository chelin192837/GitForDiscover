#ifndef _OFD_OPTIMIZER_W_H_
#define _OFD_OPTIMIZER_W_H_

#include "ofd_base_r.h"

class IFX_FileRead;
class IFX_FileWrite;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建OFD优化句柄
 *
 * @param[in] lpwszFileName	指定的文件名称。
 * @return 优化句柄。
 *
 */
OFD_OPTIMIZER	OFD_Optimizer_Create(OFD_LPCWSTR lpwszFileName);

/**
 * @brief 创建OFD优化句柄
 *
 * @param[in] lpwszFileName	指定的文件名称。
 * @param[in] bLzmaComparess LZMA压缩标志
 * @return 优化句柄。
 *
 */
OFD_OPTIMIZER OFD_Optimizer_LZMA_Create(OFD_LPCWSTR lpwszFileName, int bLzmaComparess);

/**
 * @brief 创建OFD优化句柄
 *
 * @param[in] pwOFDFile			指定的文件流。
 * @param[in] bLzmaComparess	LZMA压缩标志
 * @return 优化句柄。
 *
 */
OFD_OPTIMIZER OFD_Optimizer_LZMA_Create2(void* pOFDFile, int bLzmaComparess);

/**
 * @brief 开始优化
 *
 * @param[in] hOptimizer	OFD优化句柄。
 * @param[in] lpwszFileName	指定优化后保存文件名称。
 * @return 1 正确，0 错误。
 *
 */
OFD_BOOL	OFD_Optimizer_Start(OFD_OPTIMIZER hOptimizer, OFD_LPCWSTR lpwszFileName);

/**
 * @brief 开始优化
 *
 * @param[in] hOptimizer	OFD优化句柄。
 * @param[in] pwOFDFile		指定优化后保存文件流。
 * @return 1 正确，0 错误。
 *
 */
OFD_BOOL	OFD_Optimizer_Start2(OFD_OPTIMIZER hOptimizer, void* pOFDFile);

/**
 * @brief 继续优化
 *
 * @param[in] hOptimizer	OFD优化句柄。
 * @return 1 正确，0 错误。
 *
 */
OFD_BOOL	OFD_Optimizer_Continue(OFD_OPTIMIZER hOptimizer);

/**
 * @brief 释放OFD优化句柄
 *
 * @param[in] hOptimizer	OFD优化句柄。
 * @return 无。
 *
 */
void	OFD_Optimizer_Release(OFD_OPTIMIZER hOptimizer);

/**
 * @brief 高压缩并优化文件
 *
 * @param[in] lpwszSrcFileName		源文件名。
 * @param[in] lpwszDstFileName		目标文件名。
 * @param[in] pOptimizerParam		OFD_OPTIMIZERPARAM优化参数。
 * @return 1 正确，0 错误。
 *
 */
OFD_BOOL OFD_Optimizer_Compress(OFD_LPCWSTR lpwszSrcFileName, OFD_LPCWSTR lpwszDstFileName, OFD_OPTIMIZERPARAM* pOptimizerParam);

/**
 * @brief 高压缩并优化文件
 *
 * @param[in] pFileRead			输入文件流。
 * @param[in] pFileWrite		输出文件流。
 * @param[in] pOptimizerParam		OFD_OPTIMIZERPARAM优化参数。
 * @return 1 正确，0 错误。
 *
 */
OFD_BOOL OFD_Optimizer_Stream_Compress(IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite, OFD_OPTIMIZERPARAM* pOptimizerParam);

#ifdef __cplusplus
}
#endif

#endif //_OFD_OPTIMIZER_W_H_