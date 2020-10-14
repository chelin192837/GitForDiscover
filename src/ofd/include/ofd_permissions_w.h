#ifndef _OFD_PERMISSIONS_R_H_
#define _OFD_PERMISSIONS_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建文档操作权限对象。
 *
 * @param[in]	hPackage	包句柄。		
 * @return 文档操作权限句柄。
 *				
 */
OFD_PERMISSIONS		OFD_Document_CreatePermissions(OFD_DOCUMENT hDocument);

/**
 * @brief 设置是否允许编辑。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetEdit(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置是否允许使用注释。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetAnnot(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置是否允许导出。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL OFD_Permissions_SetExport(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置是否允许使用签章。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetSignature(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置是否允许使用水印。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetWatermark(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置是否允许打印。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetPrintScreen(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置是否允许打印。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	bSet		设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetPrintable(OFD_PERMISSIONS hPerssions, OFD_BOOL bSet);

/**
 * @brief 设置允许打印分数。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	num			设置值。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetPrintCopies(OFD_PERMISSIONS hPerssions, int num);

/**
 * @brief 设置开始日期。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	startDate	开始日期。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetStartDate(OFD_PERMISSIONS hPerssions, OFD_LPCWSTR startDate);

/**
 * @brief 设置结束日期。
 *
 * @param[in]	hPerssions	文档权限句柄。		
 * @param[in]	endDate		结束日期。		
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_Permissions_SetEndDate(OFD_PERMISSIONS hPerssions, OFD_LPCWSTR endDate);

#ifdef __cplusplus
}
#endif

#endif  //_OFD_PERMISSIONS_R_H_
