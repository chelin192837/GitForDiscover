#ifndef _OFD_OUTLINE_R_H_
#define _OFD_OUTLINE_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获得大纲根节点。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回大纲根节点句柄，失败返回NULL。
 *				
 */
OFD_OUTLINE		OFD_Document_GetOutline(OFD_DOCUMENT hDocument);

/**
 * @brief 计算子节点个数。
 *
 * @param[in]	hParent	父节点句柄。		
 * @return 成功返回子节点个数。
 *				
 */
int		OFD_Outline_CountSubOutline(OFD_OUTLINE hParent);

/**
 * @brief 获得子节点。
 *
 * @param[in]	hParent	父节点句柄。		
 * @param[in]	index	子节点索引。		
 * @return 成功返回子节点。
 *				
 */
OFD_OUTLINE		OFD_Outline_GetSubOutline(OFD_OUTLINE hParent, int index);

/**
 * @brief 获得大纲节点标题。
 *
 * @param[in]	hOutlineItem	大纲节点句柄。	
 * @param[out]	data			标题。	
 * @return 无。
 *				
 */
void		OFD_Outline_GetTitle(OFD_OUTLINE hOutline, OFD_WSTR* data);

/**
 * @brief 获得大纲节点动作类型。
 *
 * @param[in]	hOutline		大纲节点句柄。
 * @return 返回大纲节点动作类型。
 *				
 */
int		OFD_Outline_GetActionType(OFD_OUTLINE hOutline);

/**
 * @brief 获得大纲节点跳转位置。
 *
 * @param[in]	hOutline		大纲节点句柄。
 * @param[out]	iPage			返回跳转位置页号（由0开始编号）。
 * @param[out]	destPoint		返回跳转目标位置。
 * @return 无。
 *				
 */
void		OFD_Outline_GetDest(OFD_OUTLINE hOutline, int* iPage, OFD_POINT* destPoint);

/**
 * @brief 返回URI地址。
 *
 * @param[in]	hOutline		大纲节点句柄。
 * @param[in]	data			返回URI地址。	
 * @return 无。
 *				
 */
void		OFD_Outline_GetURI(OFD_OUTLINE hOutline, OFD_WSTR* data);

#ifdef __cplusplus
}
#endif

#endif //_OFD_OUTLINE_R_H_
