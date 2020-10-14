#ifndef _OFD_PATH_W_H_
#define _OFD_PATH_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建图形数据对象。
 *
 * @return 图形数据对象句柄。
 *				
 */
OFD_PATH	OFD_Path_Create();

/**
 * @brief 定义子绘制图形边线的起始点坐标(x,y)。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] x			坐标x。
 * @param[in] y			坐标y。
 * @return 返回无。
 *				
 */
void		OFD_Path_StartFigure(OFD_PATH hPath,float x, float y);

/**
 * @brief 将当前点移动到指定点(x, y) 。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] x			坐标x。
 * @param[in] y			坐标y。
 * @return 返回无。
 *				
 */
void		OFD_Path_MoveTo(OFD_PATH hPath,float x, float y);

/**
 * @brief 从当前点连接一条到指定点(x, y)的线段，并将当前点移动到指定点。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] x			坐标x。
 * @param[in] y			坐标y。
 * @return 返回无。
 *				
 */
void		OFD_Path_LineTo(OFD_PATH hPath,float x, float y);	

/**
 * @brief 从当前点链接一条到点(x2, y2)的二次贝塞尔曲线，并将当前点移动到点(x2, y2)，此贝塞尔曲线使用点(x1, y1)作为其控制点。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] x1		坐标x1。
 * @param[in] y1		坐标y1。
 * @param[in] x2		坐标x2。
 * @param[in] y2		坐标y2。
 * @return 返回无。
 *				
 */
void		OFD_Path_QuadraticBezierTo(OFD_PATH hPath,float x1, float y1, float x2, float y2);

/**
 * @brief 从当前点连接一条到点(x3, y3)的三次贝塞尔曲线，并将当前点移动到点(x3, y3)，此贝塞尔曲线使用点(x1, y1)和点(x2, y2)作为其控制点。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] x1		坐标x1。
 * @param[in] y1		坐标y1。
 * @param[in] x2		坐标x2。
 * @param[in] y2		坐标y2。
 * @param[in] x3		坐标x3。
 * @param[in] y3		坐标y3。
 * @return 返回无。
 *				
 */
void		OFD_Path_CubicBezierTo(OFD_PATH hPath,float x1, float y1, float x2, float y2, float x3, float y3);	

/**
 * @brief 从当前点连接一条到点(x, y)的圆弧，并将当前点移动到点(x, y)。
	rx表示椭圆的长轴长度，ry表示椭圆的短轴长度。
	angle表示椭圆在当前坐标系下旋转的角度，正值为顺时针，负值为逆时针，
	large为1时表示对应度数大于180°的弧，为0时表示对应度数小于180°的弧。
	sweep为1时表示由圆弧起始点到结束点是顺时针旋转，为0时表示由圆弧起始点到结束点是逆时针旋转。
 *
 * @param[in] hPath			图形数据对象。
 * @param[in] xRadius		坐标xRadius。
 * @param[in] yRadius		坐标yRadius。
 * @param[in] fAngle		角度。
 * @param[in] fLarge		为0时表示对应度数小于180°的弧，为1时表示度数大于180的弧。
 * @param[in] fSweep		sweep为1时表示由圆弧起始点到结束点是顺时针旋转，为0时表示由圆弧起始点到结束点是逆时针旋转。。
 * @param[in] x				坐标x。
 * @param[in] y				坐标x。
 * @return 返回无。
 *				
 */
void		OFD_Path_ArcTo(OFD_PATH hPath,float xRadius, float yRadius, float fAngle, float fLarge, float fSweep, float x, float y);

/**
 * @brief 添加操作。
 *
 * @param[in] hPath		图形数据对象。
 * @param[in] pPoints	添加操作。
 * @param[in] count		添加操作。
 * @return 返回无。
 *				
 */
void		OFD_Path_AddPoints(OFD_PATH hPath, OFD_PATHPT *pPoints,int count);

/**
 * @brief SubPath自动闭合，表示将当前点和SubPath的起始点用线段直接连接。
 *
 * @param[in] hPath		图形数据对象。
 * @return 返回无。
 *				
 */
void		OFD_Path_CloseFigure(OFD_PATH hPath);

/**
 * @brief 清空path操作。
 *
 * @param[in] hPath		图形数据对象。
 * @return 返回无。
 *				
 */
void		OFD_Path_Clear(OFD_PATH hPath);

/**
 * @brief 释放内存。
 *
 * @param[in] hPath		图形数据对象。
 * @return 返回无。
 *				
 */
void		OFD_Path_Release(OFD_PATH hPath);

/**
 * @brief 附加path操作。
 *
 * @param[in] hPathSrc			图形数据对象源。
 * @param[in] hPathAppend		附加图形数据对象
 * @return 返回无。
 *				
 */
void		OFD_Path_Append(OFD_PATH hPathSrc,OFD_PATH hPathAppend);

/**
 * @brief 拷贝path。
 *
 * @param[in] hPathSrc		图形数据对象源。
 * @param[in] hPathDst		图形数据对象目标。
 * @return 返回无。
 *				
 */
void		OFD_Path_Copy(OFD_PATH hPathSrc,OFD_PATH hPathDst);

#ifdef __cplusplus
}
#endif

#endif //_OFD_PATH_W_H_
