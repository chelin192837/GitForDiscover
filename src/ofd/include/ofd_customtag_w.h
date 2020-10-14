#ifndef _OFDSDK_CUSTOMTAGS_W_H_
#define _OFDSDK_CUSTOMTAGS_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建自定义标签数组。
 *
 * @param[in]	hDocument			文档句柄。
 * @return 无。
 *
 */
OFD_CUSTOMTAGS		OFD_Document_CreateCustomTags(OFD_DOCUMENT hDocument);

/**
 * @brief 开始新的语义树节点标标签。
 *
 * @param[in]	hCustomTags		自定义标签数组句柄。	
 * @param[in]	lpwszTagName	标签名称。
 * @return 自定义标签。
 *				
 */
void		OFD_CustomTags_BeginOfficeNode(OFD_CUSTOMTAGS hCustomTags, OFD_LPCWSTR lpwszTagName);

/**
 * @brief 结束当前语义树节点标标签。
 *
 * @param[in]	hCustomTags		自定义标签数组句柄。	
 * @param[in]	lpwszTagName	标签名称。
 * @return 自定义标签。
 *				
 */
void		OFD_CustomTags_EndOfficeNode(OFD_CUSTOMTAGS hCustomTags, OFD_LPCWSTR lpwszTagName);

/**
 * @brief 创建公文语义根节点。
 *
 * @param[in]	hCustomTag	自定义标签数组句柄。
 * @return 公文语义根节点。
 *				
 */
OFD_OFFICENODE		OFD_CustomTags_CreateOffice(OFD_CUSTOMTAGS hCustomTags);

/**
 * @brief 设置自定义标签信息。
 *
 * @param[in]	hDocument				文档句柄。	
 * @param[in]	lpwszSchemaShortname	自定义标签前缀名称（默认为“od”）。
 * @param[in]	lpwszSchemaNamespace	自定义标签命名空间（默认为“http://www.officedocument.org”）。
 * @param[in]	lpwszRootName			自定义标签根节点名称（默认为“电子公文”）。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL		OFD_CustomTags_SetOfficeSchema(OFD_CUSTOMTAGS hCustomTags, OFD_LPCWSTR lpwszSchemaShortname, OFD_LPCWSTR lpwszSchemaNamespace, OFD_LPCWSTR lpwszRootName);

/**
 * @brief 设置公文语义节点标签名称。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	lpszName	公文语义节点标签名称。
 * @return 无。
 *				
 */
void		OFD_OfficeNode_SetTagName(OFD_OFFICENODE hNode, OFD_LPCWSTR lpszName);

/**
 * @brief 添加子节点。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	tagName	标签名称。
 * @param[in]	bGroup	组类型。
 * @return 公文语义子节点。
 *				
 */
OFD_OFFICENODE		OFD_OfficeNode_Add(OFD_OFFICENODE hNode, OFD_LPCWSTR tagName, OFD_BOOL bGroup);

/**
 * @brief 关联指定页面、指定区域的对象。
 *
 * @param[in]   hDocument 文档句柄
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	nPageIndex	页面索引。
 * @param[in]	rect	坐标区域。
 * @return 成功返回TRUE，失败返回FALSE。
 *				
 */
OFD_BOOL OFD_OfficeNode_InsertObject(OFD_DOCUMENT hDocument, OFD_OFFICENODE hNode, int nPageIndex, OFD_RECT rect);

/**
 * @brief 删除子节点。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	index	删除子节点索引。
 * @return 无。
 *				
 */
void		OFD_OfficeNode_Delete(OFD_OFFICENODE hNode, int index);

/**
 * @brief 添加关联对象。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	pageID	页面ID。
 * @param[in]	objID	对象ID。
 * @return 无。
 *				
 */
void OFD_OfficeNode_AddObject(OFD_OFFICENODE hNode, OFD_DWORD pageID, OFD_DWORD objID);

/**
 * @brief 删除关联对象。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	pageID	页面ID。
 * @param[in]	objID	对象ID。
 * @return 无。
 *				
 */
void OFD_OfficeNode_DeleteObject(OFD_OFFICENODE hNode, OFD_DWORD pageID, OFD_DWORD objID);

/**
 * @brief 设置属性。
 *
 * @param[in]	hNode	公文语义节点句柄。
 * @param[in]	pageID	属性键。
 * @param[in]	objID	属性值。
 * @return 无。
 *				
 */
void OFD_OfficeNode_SetAttrValue(OFD_OFFICENODE hNode, OFD_LPCWSTR attrKey, OFD_LPCWSTR attrVal);

#ifdef __cplusplus
}
#endif

#endif //_OFDSDK_CUSTOMTAGS_W_H_
