#ifndef _OFD_CUSTOMTAG_H_
#define _OFD_CUSTOMTAG_H_

#include "../include/ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取自定义标签数组包含的标签个数。
 *
 * @param[in]	hCustomTags	自定义标签数组句柄。	
 * @return 自定义标签数组包含的标签个数。
 *				
 */
int		OFD_CustomTags_Count(OFD_CUSTOMTAGS hCustomTags);

/**
 * @brief 获取自定义标签。
 *
 * @param[in]	hCustomTags	自定义标签数组句柄。	
 * @return 自定义标签。
 *				
 */
OFD_CUSTOMTAG		OFD_CustomTags_Get(OFD_CUSTOMTAGS hCustomTags, int index);

/**
 * @brief 获取自定义标签类型。
 *
 * @param[in]	hCustomTag	自定义标签句柄。	
 * @param[out]	type		返回自定义标签类型。
 * @return 无。
 *				
 */
void		OFD_CustomTag_GetType(OFD_CUSTOMTAG hCustomTag, OFD_WSTR* type);

/**
 * @brief 提取自定义标签内容，保存到指定文件。
 *
 * @param[in]	hCustomTag	自定义标签句柄。	
 * @param[out]	lpszFile	保存文件名。
 * @return 无。
 *				
 */
void		OFD_CustomTag_ExtractFile(OFD_CUSTOMTAG hCustomTag, OFD_LPCWSTR lpszFile);

/**
 * @brief 添加自定义标签。
 *
 * @param[in]	hCustomTags	自定义标签数组句柄。	
 * @return 自定义标签。
 *				
 */
OFD_CUSTOMTAG		OFD_CustomTags_Add(OFD_CUSTOMTAGS hCustomTags);

/**
 * @brief 设置自定义标签类型。
 *
 * @param[in]	hCustomTag	自定义标签句柄。	
 * @param[out]	lpszType	自定义标签类型。
 * @return 无。
 *				
 */
void		OFD_CustomTag_SetType(OFD_CUSTOMTAG hCustomTag, OFD_LPCWSTR lpszType);

/**
 * @brief 设置自定义标签文件。
 *
 * @param[in]	hCustomTag	自定义标签句柄。	
 * @param[out]	lpszFile	文件名。
 * @return 无。
 *				
 */
void		OFD_CustomTag_SetFile(OFD_CUSTOMTAG hCustomTag, OFD_LPCWSTR lpszFile);

#ifdef __cplusplus
}
#endif

#endif //_OFD_CUSTOMTAG_H_
