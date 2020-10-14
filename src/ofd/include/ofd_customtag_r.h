#ifndef _OFDSDK_CUSTOMTAGS_R_H_
#define _OFDSDK_CUSTOMTAGS_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获得自定义标签数组。
 *
 * @param[in]	hDocument			文档句柄。
 * @return 无。
 *
 */
OFD_CUSTOMTAGS		OFD_Document_GetCustomTags(OFD_DOCUMENT hDocument);

/**
 * @brief 获得公文语义根节点。
 *
 * @param[in]	hCustomTag	自定义标签数组句柄。
 * @return 公文语义根节点。
 *				
 */
OFD_OFFICENODE		OFD_CustomTags_GetOffice(OFD_CUSTOMTAGS hCustomTags);

/**
 * @brief 获取公文语义节点属性。
 *
 * @param[in]	hNode		公文语义节点句柄。	
 * @param[in]	lpszName	公文语义节点属性。
 * @param[out]	value		返回公文语义节点属性值。
 * @return 无。
 *				
 */
void		OFD_OfficeNode_GetAttrValue(OFD_OFFICENODE hNode, OFD_LPCWSTR lpszName, OFD_WSTR* value);

/**
 * @brief 获取公文语义节点子节点个数。
 *
 * @param[in]	hNode	公文语义节点句柄。	
 * @return 公文语义节点子节点个数。
 *				
 */
int		OFD_OfficeNode_Count(OFD_OFFICENODE hNode);

/**
 * @brief 获取公文语义节点子节点。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	index	索引。
 * @return 公文语义节点子节点句柄。
 *				
 */
OFD_OFFICENODE OFD_OfficeNode_Get(OFD_OFFICENODE hNode, int index);

/**
 * @brief 获取公文语义节点标签名称。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[out]	name	返回公文语义节点标签名称。
 * @return 无。
 *				
 */
void		OFD_OfficeNode_GetTagName(OFD_OFFICENODE hNode, OFD_WSTR* name);

/**
 * @brief 获取公文语义节点包含的文本。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[out]	text	返回公文语义节点包含的文本。
 * @return 无。
 *				
 */
void		OFD_OfficeNode_GetText(OFD_OFFICENODE hNode, OFD_WSTR* text);

/**
 * @brief 判断公文语义节点类型。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @return 节点类型。
 *				
 */
OFD_BOOL OFD_OfficeNode_IsGroup(OFD_OFFICENODE hNode);

/**
 * @brief 判断公文语义节点类型。
 *
 * @param[in]	hNode		公文语义节点句柄。
 * @param[in]	index		pageObj序号。
 * @param[out]	pageID		页面ID。
 * @param[out]	pageObjID	pageObjID。
 * @return TRUE，FALSE。
 *				
 */
OFD_BOOL OFD_OfficeNode_GetObjID(OFD_OFFICENODE hNode,int index,int* pageObjID,int* pageID);

/**
 * @brief 计算公文语义节点中图元对象个数。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @return 图元对象的个数。
 *				
 */
int OFD_OfficeNode_CountPageObj(OFD_OFFICENODE hNode);

/**
 * @brief 获取公文语义节点中指定的图元对象。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	index	索引（从0开始）。
 * @return 图元对象。
 *				
 */
OFD_PAGEOBJECT OFD_OfficeNode_GetPageObj(OFD_OFFICENODE hNode, int index);

#ifdef __cplusplus
}
#endif

#endif //_OFDSDK_CUSTOMTAGS_R_H_
