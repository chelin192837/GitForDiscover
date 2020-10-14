#ifndef _OFD_BOOKMARK_R_H_
#define _OFD_BOOKMARK_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获得书签数组。
 *
 * @param[in]	hDocument			文档句柄。	
 * @return 成功返回书签数组句柄，失败返回NULL。
 *				
 */
OFD_BOOKMARKS		OFD_Document_GetBookmarks(OFD_DOCUMENT hDocument);

/**
 * @brief 计算子节点个数。
 *
 * @param[in]	hParent	父节点句柄。		
 * @return 成功返回子节点个数。
 *				
 */
int		OFD_Bookmarks_CountSubBookmark(OFD_BOOKMARKS hParent);

/**
 * @brief 获得子节点。
 *
 * @param[in]	hParent	父节点句柄。		
 * @param[in]	index	子节点索引。		
 * @return 成功返回子节点。
 *				
 */
OFD_BOOKMARK		OFD_Bookmarks_GetSubBookmark(OFD_BOOKMARKS hParent, int index);

/**
 * @brief 获得书签节点名称。
 *
 * @param[in]	hBookmark		书签节点句柄。	
 * @param[out]	data			名称。	
 * @return 无。
 *				
 */
void		OFD_Bookmark_GetName(OFD_BOOKMARK hBookmark, OFD_WSTR* data);

/**
 * @brief 获得书签节点跳转位置。
 *
 * @param[in]	hBookmark		书签节点句柄。
 * @param[out]	iPage			返回跳转位置页号（由0开始编号）。
 * @param[out]	destPoint		返回跳转目标位置。
 * @return 无。
 *				
 */
void		OFD_Bookmark_GetDest(OFD_BOOKMARK hBookmark, int* iPage, OFD_POINT* destPoint);

#ifdef __cplusplus
}
#endif

#endif //_OFD_BOOKMARK_R_H_
