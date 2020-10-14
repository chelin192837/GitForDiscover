#ifndef _OFD_PACKAGE_W_H_
#define _OFD_PACKAGE_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建一个空包。
 *
 * @param[in] lpwszFileName	指定的文件名称。
 * @return 成功返回包句柄，失败返回NULL。
 *				
 */
OFD_PACKAGE		OFD_Package_Create(OFD_LPCWSTR lpwszFileName);

/**
 * @brief 新建一个文档，并追加至包中。
 *
 * @param[in]	hPackage	包句柄。		
 * @return 成功返回文档句柄，失败返回NULL。
 *				
 */
OFD_DOCUMENT		OFD_Package_AddDocument(OFD_PACKAGE hPackage);

/**
 * @brief 新建一个文档，并按序号插入包中。
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	iIndex		文档序号(由0开始编号)。
 * @return 成功返回文档句柄，失败返回NULL。
 *				
 */
OFD_DOCUMENT		OFD_Package_InsertDocument(OFD_PACKAGE hPackage, int index);

/**
 * @brief 将其他包中的文档追加到此包中。
 *
 * @param[in]	hPackageTo		包句柄。		
 * @param[in]	hPackageForm	包句柄。		
 * @param[in]	index			文档索引。		
 * @return 成功返回文档句柄，失败返回NULL。
 *				
 */
void		OFD_Package_AddOtherDocument(OFD_PACKAGE hPackageTo, OFD_PACKAGE hPackageFrom, int index);

/**
 * @brief 从包中删除指定。
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	iIndex		文档序号（由0开始编号）。
 * @return 无。
 *				
 */
void		OFD_Package_RemoveDocument(OFD_PACKAGE hPackage, int index);

/**
 * @brief 从包中清除指定文档内容，仅限于 load documet 的情况。
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	iIndex		文档序号（由0开始编号）。
 * @return 无。
 *				
 */
OFD_DOCUMENT	OFD_Package_ClearDocument(OFD_PACKAGE hPackage, int index);

/**
 * @brief 设置浮点数的有效位数。（在保存时使用）
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	number		有效位数。	
 * @return 无。
 *				
 */
void OFD_Package_Optimizer_SetFloatBit(OFD_PACKAGE hPackage, int number);

/**
 * @brief 设置是否使用LZMA压缩。（在保存时使用）
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	flag		标志位。	
 * @return 无。
 *				
 */
void OFD_Package_Optimizer_SetCompress(OFD_PACKAGE hPackage, int flag);

/**
 * @brief 设置是否嵌入字体。（在保存时使用）
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	flag		标志位。	
 * @return 无。
 *				
 */
void OFD_Package_SetEmbedFont(OFD_PACKAGE hPackage, int flag);

/**
 * @brief 设置是否资源优化。（在保存时使用）
 *
 * @param[in]	hPackage	包句柄。	
 * @param[in]	flag		标志位。	
 * @return 无。
 *				
 */
void OFD_Package_SetOptimizerResource(OFD_PACKAGE hPackage, int flag);

/**
 * @brief 保存修改（用于OFD_Package_Create方式创建的包句柄）。
 *
 * @param[in]	hPackage	包句柄。	
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Package_Save(OFD_PACKAGE hPackage);

/**
 * @brief 保存到指定的文件。
 *
 * @param[in]	hPackage		包句柄。	
 * @param[in]	lpwszFileName	文件名。	
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Package_SaveAs(OFD_PACKAGE hPackage, OFD_LPCWSTR lpwszFileName);

#ifdef __cplusplus
}
#endif

#endif  //_OFD_PACKAGE_W_H_
