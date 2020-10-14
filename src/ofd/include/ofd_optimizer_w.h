#ifndef _OFD_OPTIMIZER_W_H_
#define _OFD_OPTIMIZER_W_H_

#include "ofd_base_r.h"

class IFX_FileRead;
class IFX_FileWrite;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ����OFD�Ż����
 *
 * @param[in] lpwszFileName	ָ�����ļ����ơ�
 * @return �Ż������
 *
 */
OFD_OPTIMIZER	OFD_Optimizer_Create(OFD_LPCWSTR lpwszFileName);

/**
 * @brief ����OFD�Ż����
 *
 * @param[in] lpwszFileName	ָ�����ļ����ơ�
 * @param[in] bLzmaComparess LZMAѹ����־
 * @return �Ż������
 *
 */
OFD_OPTIMIZER OFD_Optimizer_LZMA_Create(OFD_LPCWSTR lpwszFileName, int bLzmaComparess);

/**
 * @brief ����OFD�Ż����
 *
 * @param[in] pwOFDFile			ָ�����ļ�����
 * @param[in] bLzmaComparess	LZMAѹ����־
 * @return �Ż������
 *
 */
OFD_OPTIMIZER OFD_Optimizer_LZMA_Create2(void* pOFDFile, int bLzmaComparess);

/**
 * @brief ��ʼ�Ż�
 *
 * @param[in] hOptimizer	OFD�Ż������
 * @param[in] lpwszFileName	ָ���Ż��󱣴��ļ����ơ�
 * @return 1 ��ȷ��0 ����
 *
 */
OFD_BOOL	OFD_Optimizer_Start(OFD_OPTIMIZER hOptimizer, OFD_LPCWSTR lpwszFileName);

/**
 * @brief ��ʼ�Ż�
 *
 * @param[in] hOptimizer	OFD�Ż������
 * @param[in] pwOFDFile		ָ���Ż��󱣴��ļ�����
 * @return 1 ��ȷ��0 ����
 *
 */
OFD_BOOL	OFD_Optimizer_Start2(OFD_OPTIMIZER hOptimizer, void* pOFDFile);

/**
 * @brief �����Ż�
 *
 * @param[in] hOptimizer	OFD�Ż������
 * @return 1 ��ȷ��0 ����
 *
 */
OFD_BOOL	OFD_Optimizer_Continue(OFD_OPTIMIZER hOptimizer);

/**
 * @brief �ͷ�OFD�Ż����
 *
 * @param[in] hOptimizer	OFD�Ż������
 * @return �ޡ�
 *
 */
void	OFD_Optimizer_Release(OFD_OPTIMIZER hOptimizer);

/**
 * @brief ��ѹ�����Ż��ļ�
 *
 * @param[in] lpwszSrcFileName		Դ�ļ�����
 * @param[in] lpwszDstFileName		Ŀ���ļ�����
 * @param[in] pOptimizerParam		OFD_OPTIMIZERPARAM�Ż�������
 * @return 1 ��ȷ��0 ����
 *
 */
OFD_BOOL OFD_Optimizer_Compress(OFD_LPCWSTR lpwszSrcFileName, OFD_LPCWSTR lpwszDstFileName, OFD_OPTIMIZERPARAM* pOptimizerParam);

/**
 * @brief ��ѹ�����Ż��ļ�
 *
 * @param[in] pFileRead			�����ļ�����
 * @param[in] pFileWrite		����ļ�����
 * @param[in] pOptimizerParam		OFD_OPTIMIZERPARAM�Ż�������
 * @return 1 ��ȷ��0 ����
 *
 */
OFD_BOOL OFD_Optimizer_Stream_Compress(IFX_FileRead* pFileRead, IFX_FileWrite* pFileWrite, OFD_OPTIMIZERPARAM* pOptimizerParam);

#ifdef __cplusplus
}
#endif

#endif //_OFD_OPTIMIZER_W_H_