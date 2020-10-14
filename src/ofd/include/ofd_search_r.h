#ifndef _OFD_SEARCH_R_H_
#define _OFD_SEARCH_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取查找结果的数量。
 *	
 * @param[in]	hSearch			查找句柄。	
 * @return 返回查找结果的数量。				
 */
int		OFD_Search_Count(OFD_SEARCH hSearch);

/**
 * @brief 从查找结果中获得相关属性。
 *	
 * @param[in]	hSearch			查找句柄。	
 * @param[in]	index			查找结果的索引值	
 * @param[in]	flag			标志：	0 页面索引		1 文档ID		2 查找结果包含的 RECT 的数量
 * @return 成功返回查找结果，失败返回0。				
 */
int		OFD_Search_GetProperty(OFD_SEARCH hSearch, int index, int flag);

/**
 * @brief 从查找结果中获得 RECT 信息。
 *
 * @param[in]	hSearch			OFD包中查找相关字符串后返回的句柄。	
 * @param[in]	index			查找结果的索引值	
 * @param[in]	iRect			相应查找结果包含的 RECT 的索引值
 * @return 成功返回查找矩形，失败返回矩形值为0。
 *				
 */
OFD_RECT		OFD_Search_GetRect(OFD_SEARCH hSearch, int index, int iRect);

/**
 * @brief 释放查找句柄。
 *
 * @param[in]	hSearch			OFD包中查找相关字符串后返回的句柄。	
 * @return 无
 *				
 */
void		OFD_Search_Release(OFD_SEARCH hSearch);

#ifdef __cplusplus
}
#endif

#endif  //_OFD_SEARCH_R_H_
