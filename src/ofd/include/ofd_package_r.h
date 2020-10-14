#ifndef _OFD_PACKAGE_R_H_
#define _OFD_PACKAGE_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @brief 从文件中加载一个包。
 *
 * @param[in] lpwszFileName	指定的文件名。		
 * @return 成功返回包句柄，失败返回NULL。
 *				
 */
OFD_PACKAGE		OFD_Package_LoadFromFile(OFD_LPCWSTR lpwszFileName, OFD_BOOL bClone = 0);

OFD_PACKAGE		OFD_Package_LoadFromFile2(OFD_LPCSTR lpszFileName, OFD_BOOL bClone = 0);

/**
 * @brief 从内存中加载一个包。
 *
 * @param[in] lpbBuffer	文件内存流数据。		
 * @param[in] len		文件内存流数据长度。
 * @param[in] takeOver	是否有SDK接管内存
 * @return 成功返回包句柄，失败返回NULL。
 *				
 */
OFD_PACKAGE		OFD_Package_LoadFromBuffer(OFD_LPCBYTE lpbBuffer, int len, OFD_BOOL takeOver, OFD_BOOL bClone = 0);

/**
 * @brief 销毁包，并释放内存。
 *
 * @param[in]	hPackage	包句柄。		
 * @return 无。
 *				
 */
void		OFD_Package_Destroy(OFD_PACKAGE hPackage);

/**
 * @brief 获取包中文档数。
 *
 * @param[in]	hPackage	包句柄。			
 * @return 成功返回文档的数量，失败返回-1。
 *				
 */
int		OFD_Package_Count(OFD_PACKAGE hPackage);

/**
 * @brief 加载指定文档。
 *
 * @param[in]	hPackage	包句柄。		
 * @param[in]	index		文档序号。
 * @param[in]	password	密码

 * @return 成功返回文档句柄，失败返回NULL。
 *				
 */
OFD_DOCUMENT		OFD_Package_LoadDocument(OFD_PACKAGE hPackage, int index, OFD_LPCSTR password);

/**
 * @brief 加载指定文档（证书加密过的文档）。
 *
 * @param[in]	hPackage	包句柄。		
 * @param[in]	index		文档序号。
 * @param[in]	password	证书密码。
 * @param[in]	certPath	证书。

 * @return 成功返回文档句柄，失败返回NULL。
 *				
 */
OFD_DOCUMENT OFD_Package_LoadDocument2(OFD_PACKAGE hPackage, int index, OFD_LPCSTR password, OFD_LPCWSTR certPath);

/**
 * @brief 指定文档是否加密。
 *
 * @param[in]	hPackage	包句柄。		
 * @param[in]	index		文档序号。
 * @return 是否加密。
 *				
 */
OFD_BOOL		OFD_Package_IsEncrypted(OFD_PACKAGE hPackage, int index);

/**
 * @brief 获取指定文档使用的加密方式。
 *
 * @param[in]	hPackage	包句柄。		
 * @param[in]	index		文档序号。
 * @return 返回文档加密方法。（1是密码加密,2是证书加密） 0没有加密;
 *				
 */
int 	OFD_Package_GetEncryptMethod(OFD_PACKAGE hPackage, int index);

/**
 * @brief 获取指定文档使用的加密算法类型。
 *
 * @param[in]	hPackage	包句柄。		
 * @param[in]	index		文档序号。
 * @return 返回文档使用的加密算法类型。
 *				
 */
int		OFD_Package_GetEncryptType(OFD_PACKAGE hPackage, int index);

#ifdef __cplusplus
}
#endif

#endif  //_OFDSDK_PACKAGE_R_H_
