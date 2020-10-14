#ifndef _OFD_DOCUMENT_R_H_
#define _OFD_DOCUMENT_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 关闭文档，并释放对应内存。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 无。
 *				
 */
void		OFD_Document_Close(OFD_DOCUMENT hDocument);

/**
 * @brief 获取文档页数。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 成功返回页个数，失败返回-1。
 *				
 */
int		OFD_Document_CountPage(OFD_DOCUMENT hDocument);

/**
 * @brief 加载文档一页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			页序号（由0开始编号）。
 * @return 成功返回页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_LoadPage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 获取文档中模板页的个数。
 *
 * @param[in]	hDocument		文档句柄。	
 * @return 成功返回模板页个数，失败返回-1。
 *				
 */
int		OFD_Document_CountTemplatePage(OFD_DOCUMENT hDocument);

/**
 * @brief 根据名称获取文档中的指定模板页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	lpwszName		模板页名称。
 * @return 成功返回模板页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_GetTemplatePageByName(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszName);

/**
 * @brief 获取文档中的指定模板页。
 *
 * @param[in]	hDocument		文档句柄。	
 * @param[in]	index			模板页序号（由0开始编号）。
 * @return 成功返回模板页句柄，失败返回NULL。
 *				
 */
OFD_PAGE		OFD_Document_LoadTemplatePage(OFD_DOCUMENT hDocument, int index);

/**
 * @brief 根据名称获取元数据。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	lpwszMetadataName	元数据名称(OFD_DOCUMENT_METADATA_XXX)。
 * @param[out]	data				返回元数据。
 * @return 无。
 *				
 */
void		OFD_Document_GetMetadata(OFD_DOCUMENT hDocument, OFD_LPCWSTR lpwszMetadataName, OFD_WSTR* data);

/**
 * @brief 获取指定元数据关键字。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	index				元数据关键字索引。
 * @param[out]  key					元数据关键字
 * @return 无。
 *				
 */
void        OFD_Document_GetKeyWord(OFD_DOCUMENT hDocument, int index, OFD_WSTR& key);

/**
 * @brief 获取关键字个数。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回关键字个数，失败返回-1
 *				
 */
int			OFD_Document_CountKeyWords(OFD_DOCUMENT hDocument);

/**
 * @brief 获取自定义元数据。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	index				元数据关键字索引。
 * @param[out]  data				自定义元数据
 * @return 无
 *				
 */
void       OFD_Document_GetCustomData(OFD_DOCUMENT hDocument, int index, OFD_WSTR& data);

/**
 * @brief 获取自定义元数据名称。
 *
 * @param[in]	hDocument			文档句柄。	
 * @param[in]	index				元数据关键字索引。
 * @param[out]  name				自定义元数据
 * @return 无
 *				
 */
void       OFD_Document_GetCustomDataName(OFD_DOCUMENT hDocument, int index, OFD_WSTR& name);

/**
 * @brief 获取自定义元数据个数。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回自定义元数据个数，失败返回-1
 *				
 */
int			OFD_Document_CountCustomData(OFD_DOCUMENT hDocument);

/**
 * @brief 获取文档动作数组句柄。
 *
 * @param[in]	hDocument	文档句柄。		
 * @return 成功返回文档动作数组句柄，失败返回NULL。
 *				
 */
OFD_ACTIONS		OFD_Document_GetActions(OFD_DOCUMENT hDocument);

/**
 * @brief 判断是否是所有者权限。
 *
 * @param[in]	hDocument	文档句柄。	
 * @return 是否是所有者权限。
 *
 */
OFD_BOOL	OFD_Document_IsAdmin(OFD_DOCUMENT hDocument);

/**
 * @brief 获取最大版本ID。
 *
 * @param[in]	hDocument	文档句柄。		
 * @return 成功返回最大版本ID，失败返回0。
 *
 */
int OFD_Document_GetMaxVersionID(OFD_DOCUMENT hDocument);

/**
 * @brief 获取版本个数。
 *
 * @param[in]	hDocument	文档句柄。		
 * @return 成功返回版本个数，失败返回0。
 *
 */
int OFD_Document_CountVersions(OFD_DOCUMENT hDocument);

/**
 * @brief 获取版本内容。
 *
 * @param[in]	hDocument	文档句柄。	
 * @param[in]	nIndex		版本索引ID。
 * @param[out]	data		返回版本内容。
 * @return 无
 *
 */
void OFD_Document_GetVersion(OFD_DOCUMENT hDocument, int nIndex, OFD_WSTR* data);

/**
 * @brief 获取版本名称。
 *
 * @param[in]	hDocument	文档句柄。	
 * @param[in]	nIndex		版本索引ID。
 * @param[out]	data		返回版本名称。
 * @return 无
 *
 */
void OFD_Document_GetVersionName(OFD_DOCUMENT hDocument, int nIndex, OFD_WSTR* data);

/**
 * @brief 获取文档根目录。
 *
 * @param[in]	hDocument	文档句柄。	
 * @param[in]	nIndex		版本索引ID。
 * @param[out]	data		返回文档根目录。
 * @return 无
 *
 */
void OFD_Document_VersionGetDocRoot(OFD_DOCUMENT hDocument, int nIndex, OFD_WSTR* data);

/**
 * @brief 获取该版本文件数。
 *
 * @param[in]	hDocument	文档句柄。
 * @param[in]	nIndex		版本索引ID。
 * @return 成功返回文件个数，失败返回0。
 *
 */
int OFD_Document_VersionCountFiles(OFD_DOCUMENT hDocument, int nIndex);

#ifdef __cplusplus
}
#endif

#endif  //_OFD_DOCUMENT_R_H_
