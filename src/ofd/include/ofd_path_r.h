#ifndef _OFD_PATH_R_H_
#define _OFD_PATH_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 计算path操作个数。
 *
 * @param[in] hPath		图形数据对象。
 * @return 成功返回path操作个数，失败返回-1。
 *				
 */
int				OFD_Path_CountPoints(OFD_PATH hPath);

/**
 * @brief 获取指定的path操作数据。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] index		序号（从0开始）。
 * @return 成功返回path操作数据，失败返回NULL。
 *				
 */
OFD_PATHPT*		OFD_Path_GetPathPoint(OFD_PATH hPath,int index);

/**
 * @brief 获取path全部操作数据。
 *
 * @param[in] hPath		图形数据对象。
 * @return 成功返回path操作数据，失败返回NULL。
 *				
 */
OFD_PATHPT*		OFD_Path_GetBuffer(OFD_PATH hPath);

/**
 * @brief 获取path外接矩形框。
 *
 * @param[in] hPath		图形数据对象。
 * @return 成功返回外接矩形框，失败返回0。
 *				
 */
//OFD_RECT		OFD_Path_GetBoundary(OFD_PATH hPath); //目前底层不支持

/**
 * @brief 获取path变换矩阵。
 *
 * @param[in] hPath		图形数据对象。
 * @return 成功返回变换矩阵，失败返回0。
 *				
 */
OFD_MATRIX		OFD_Path_Transform(OFD_PATH hPath);


#ifdef __cplusplus
}
#endif

#endif //_OFD_PATH_R_H_
