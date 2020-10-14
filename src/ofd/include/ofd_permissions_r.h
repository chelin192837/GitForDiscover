#ifndef _OFD_PERMISSIONS_W_H_
#define _OFD_PERMISSIONS_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取文档操作权限对象。
 *
 * @param[in]	hDocument	文档句柄。		
 * @return 成功返回文档操作权限句柄，失败返回NULL。
 *				
 */
OFD_PERMISSIONS		OFD_Document_GetPermissions(OFD_DOCUMENT hDocument);

/**
 * @brief 检查是否允许编辑。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @return 是否允许编辑。
 *				
 */
OFD_BOOL		OFD_Permissions_IsEdit(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查是否允许使用注释。
 *
 * @param[in]	hPerssions	文档权限句柄。				
 * @return 是否允许使用注释。
 *				
 */
OFD_BOOL		OFD_Permissions_IsAnnot(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查是否允许导出。
 *
 * @param[in]	hPerssions	文档权限句柄。				
 * @return 是否允许导出。
 *				
 */
OFD_BOOL		OFD_Permissions_IsExport(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查是否允许使用签章。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @return 是否允许使用签章。
 *				
 */
OFD_BOOL		OFD_Permissions_IsSignature(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查是否允许使用水印。
 *
 * @param[in]	hPerssions	文档权限句柄。				
 * @return 是否允许使用水印。
 *				
 */
OFD_BOOL		OFD_Permissions_IsWatermark(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查是否允许打印到屏幕。
 *
 * @param[in]	hPerssions	文档权限句柄。				
 * @return 是否允许打印到屏幕。
 *				
 */
OFD_BOOL		OFD_Permissions_IsPrintScreen(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查是否允许打印。
 *
 * @param[in]	hPerssions	文档权限句柄。				
 * @return 是否允许打印。
 *				
 */
OFD_BOOL		OFD_Permissions_IsPrintable(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查允许打印分数。
 *
 * @param[in]	hPerssions	文档权限句柄。				
 * @return 允许打印分数。
 *				
 */
int		OFD_Permissions_GetPrintCopies(OFD_PERMISSIONS hPerssions);

/**
 * @brief 检查权限起始时间。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[out]	date		返回权限起始时间。
 * @return 允许打印分数。
 *				
 */
void		OFD_Permissions_GetStartDate(OFD_PERMISSIONS hPerssions, OFD_WSTR* date);

/**
 * @brief 检查权限截止时间。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[out]	date		返回权限截止时间。
 * @return 允许打印分数。
 *				
 */
void		OFD_Permissions_GetEndDate(OFD_PERMISSIONS hPerssions, OFD_WSTR* date);

#ifdef __cplusplus
}
#endif

#endif  //_OFD_PERMISSIONS_W_H_
