#ifndef _OFD_BOOKMARK_W_H_
#define _OFD_BOOKMARK_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建书签数组。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回书签数组句柄，失败返回NULL。
 *				
 */
OFD_BOOKMARKS		OFD_Document_CreateBookmarks(OFD_DOCUMENT hDocument);

/**
 * @brief 添加子节点。
 *
 * @param[in]	hParent	父节点句柄。		
 * @return 成功返回子节点句柄，失败返回NULL。
 *				
 */
OFD_BOOKMARK		OFD_Bookmarks_AddSubItem(OFD_BOOKMARKS hParent);

/**
 * @brief 删除子节点。
 *
 * @param[in]	hParent	父节点句柄。
 * @param[in]	index	子节点索引。
 * @return 无。
 *				
 */
void		OFD_Bookmarks_DeleteSubItem(OFD_BOOKMARKS hParent, int index);

/**
 * @brief 设置书签节点名称。
 *
 * @param[in]	hBookmarkItem	书签节点句柄。	
 * @param[in]	lpwszName		名称。
 * @return 无。
 *				
 */
void		OFD_Bookmark_SetName(OFD_BOOKMARK hBookmark, OFD_LPCWSTR lpwszName);

/**
 * @brief 设置书签节点跳转位置。
 *
 * @param[in]	hBookmark		书签节点句柄。
 * @param[in]	iPage			跳转位置页号（由0开始编号）。
 * @param[in]	destPoint		跳转目标位置。
 * @return 。
 *				
 */
void		OFD_Bookmark_SetDest(OFD_BOOKMARK hBookmark, int iPage, OFD_POINT destPoint);

#ifdef __cplusplus
}
#endif

#endif //_OFD_BOOKMARK_W_H_
