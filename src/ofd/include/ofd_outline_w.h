#ifndef _OFD_OUTLINE_W_H_
#define _OFD_OUTLINE_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/* @brief 创建大纲根节点。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回大纲根节点句柄，失败返回NULL。
 *				
 */
OFD_OUTLINE		OFD_Document_CreateOutline(OFD_DOCUMENT hDocument);

/**
 * @brief 添加子节点。
 *
 * @param[in]	hParent	父节点句柄。		
 * @return 成功返回子节点句柄，失败返回NULL。
*				
 */
OFD_OUTLINE		OFD_Outline_AddSubItem(OFD_OUTLINE hParent);

/**
 * @brief 设置大纲节点标题。
 *
 * @param[in]	hOutlineItem	大纲节点句柄。	
 * @param[in]	lpwszTitle		标题。	
 * @return 无。
 *				
 */
void		OFD_Outline_SetTitle(OFD_OUTLINE hOutline, OFD_LPCWSTR lpwszTitle);

/**
 * @brief 设置大纲初始时是否展开子节点属性。
 *
 * @param[in]	hOutline		大纲节点句柄。
 * @param[in]	bExpanded		是否展开子节点（True：展开；False：不展开）。	
 * @return 返回TRUE展开子节点，FALSE不展开。
 *				
 */
void		OFD_Outline_SetExpanded(OFD_OUTLINE hOutline, OFD_BOOL bExpanded);

/**
 * @brief 设置大纲节点跳转位置。
 *
 * @param[in]	hOutline		大纲节点句柄。
 * @param[in]	iPage			跳转位置页号（由0开始编号）。
 * @param[in]	destPoint		跳转目标位置。
 * @return 。
 *				
 */
void		OFD_Outline_SetDest(OFD_OUTLINE hOutline, int iPage, OFD_POINT destPoint);
	
/**
 * @brief 设置大纲节点跳转位置。
 *
 * @param[in]	hOutline		大纲节点句柄。
 * @param[in]	lpwszUri		跳转位置。	
 * @return 返回TRUE展开子节点，FALSE不展开。
 *				
 */
void		OFD_Outline_SetURI(OFD_OUTLINE hOutline, OFD_LPCWSTR lpwszUri);

#ifdef __cplusplus
}
#endif

#endif //_OFD_OUTLINE_W_H_
